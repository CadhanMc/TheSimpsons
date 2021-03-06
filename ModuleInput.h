#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Point.h"
#include <map>
#include "SDL/include/SDL.h"


#define NUM_MOUSE_BUTTONS 5

using namespace std;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Init() override;

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	update_status PreUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

private:

	bool InitSDLEvents() const;
	void InitializeKeyboardToIdle() const;
	void InitializeMouseButtonsToIdle();
	void SetWindowEventsToFalse();
	void HandleKeyboard() const;
	void SetKeyToPressed(int keyId) const;
	void SetKeyToReleased(int keyId) const;
	void HandleMouseButtons();
	void HandleSDLEvents();
	update_status GetNewUpdateStatus() const;
	void SetTrueWindowEvent(int event);
	void SetStateMouseButton(int keyState);
	void UpdateMouseMotionAndPosition(int);
	void InitializeEventMap();
	void InitializeEventParameterMap();
	void SetWindowEvents(int);

	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint mouse_motion;
	iPoint mouse;
	SDL_Event event;

	typedef void(ModuleInput::*Func)(int);
	map<Uint32, Func> eventMap;
	map<Uint32, int> eventParameterMap;
};

#endif // __MODULEINPUT_H__