#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

using namespace std;

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");

	return AssertLoadPngSupport();
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	for (list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		SDL_DestroyTexture(*it);

	textures.clear();
	return true;
}

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path, SDL_Color* transparentPixelColor)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr)
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError())
	else
	{
		Uint32 transparentPixel = SDL_MapRGB(surface->format, 
			transparentPixelColor->r, transparentPixelColor->g, transparentPixelColor->b);
		SDL_SetColorKey(surface, SDL_TRUE, transparentPixel);

		TryToCreateTextureFromSurface(&texture, surface);
	}
		
	return texture;
}

// Free texture from memory
void ModuleTextures::Unload(SDL_Texture* texture)
{
	for (list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (*it == texture)
		{
			SDL_DestroyTexture(*it);
			textures.erase(it);
			break;
		}
	}
}

bool ModuleTextures::AssertLoadPngSupport() const
{
	int flags = IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		return false;
	}
	return true;
}

void ModuleTextures::TryToCreateTextureFromSurface(SDL_Texture** texture, SDL_Surface* surface)
{
	*texture = SDL_CreateTextureFromSurface(App->GetModuleRender()->GetRenderer(), surface);

	if (texture == nullptr)
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError())
	else
		textures.push_back(*texture);

	SDL_FreeSurface(surface);
}


