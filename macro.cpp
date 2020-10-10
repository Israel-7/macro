#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <lua.hpp>

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "winmm.lib")

enum KeyboardEventType
{
    KeyDown,
    KeyUp,
    KeyPress
};

void SendKeys(int keyCode, KeyboardEventType keyEventType)
{
    int kc = ((keyCode == 160) || (keyCode == 0x10)) ? 0xA1 : keyCode;
    uint32_t mvk = MapVirtualKey((uint32_t)(kc & 0xFF), 0);
    uint32_t flags = 0;

    if ((((kc >= 0x21) && (kc <= 0x2E)) || ((kc >= 0x5B) && (kc <= 0x5D))) || ((kc >= 0x60) && (kc <= 0x69)))
    {
        flags = 1;
    }

    INPUT pInputs;
    
    pInputs.type = 1;
    pInputs.ki.wVk = (uint16_t)kc;
    pInputs.ki.wScan = (uint16_t)mvk;
    pInputs.ki.time = 0;
    pInputs.ki.dwExtraInfo = 0;
    
    switch (keyEventType)
    {
    case KeyboardEventType::KeyDown:
        pInputs.ki.dwFlags = flags;
        SendInput(1, &pInputs, sizeof(INPUT));

        break;
    case KeyboardEventType::KeyPress:
        pInputs.ki.dwFlags = flags;
        SendInput(1, &pInputs, sizeof(INPUT));
    case KeyboardEventType::KeyUp:
        pInputs.ki.dwFlags = 2 | flags;
        SendInput(1, &pInputs, sizeof(INPUT));
    }
}

HWND _foundHwd;
const char *_caption;

bool EnumRegexWindowsProc(HWND hWnd, int lParam)
{
    if (IsWindowVisible(hWnd))
    {
        HWND num = FindWindow(L"Shell_TrayWnd", 0);
        HWND desktopWindow = GetDesktopWindow();
        HWND num2 = FindWindow(L"Progman", L"Program Manager");
        HWND windowLong = (HWND)GetWindowLong(hWnd, -8);

        if (hWnd != num2 && hWnd != desktopWindow && windowLong != num)
        {
            char windowText[255];
            GetWindowTextA(hWnd, windowText, sizeof(windowText));

            if (!strcmp(windowText, _caption))
            {
                _foundHwd = hWnd;
            }
        }
    }

    return true;
}

HWND GetHWndByCaption(const char *caption)
{
    _foundHwd = 0;
    _caption = caption;
    EnumDesktopWindows(0, (WNDENUMPROC)EnumRegexWindowsProc, 0);
    return _foundHwd;
}

void SwitchToWindow(HWND hwnd)
{
    HWND intPtr = hwnd;

    if (IsIconic(intPtr))
    {
        ShowWindow(intPtr, 9);
    }

    SetForegroundWindow(intPtr);
    ShowWindowAsync(intPtr, 5);
    SetForegroundWindow(intPtr);
    
    HWND hWnd = GetForegroundWindow();

    DWORD windowThreadProcessId = GetWindowThreadProcessId(hWnd, 0);
    DWORD windowThreadProcessId2 = GetWindowThreadProcessId(intPtr, 0);

    if (AttachThreadInput((uint32_t)windowThreadProcessId2, (uint32_t)windowThreadProcessId, true))
    {
        BringWindowToTop(intPtr);
        SetForegroundWindow(intPtr);
        AttachThreadInput((uint32_t)windowThreadProcessId2, (uint32_t)windowThreadProcessId, false);
    }
    
    if (GetForegroundWindow() != hwnd)
    {
        SystemParametersInfo(8192U, 0U, 0, 0U);
        SystemParametersInfo(8193U, 0U, 0, 2U);
        BringWindowToTop(intPtr);
        SetForegroundWindow(intPtr);
        SystemParametersInfo(8193U, 0U, 0, 2U);
    }
}

void SwitchToWndByCaption(const char* caption)
{
    HWND hwndByCaption = GetHWndByCaption(caption);

    if (hwndByCaption != 0)
    {
        SwitchToWindow(hwndByCaption);
    }
}

void waitMS(int ms)
{
    auto time = timeGetTime();
    ms = ms <= 0 ? 1 : ms;

    while (true) {
        MsgWaitForMultipleObjects(0, 0, false, ms, 255);
        if (timeGetTime() >= time + ms) return;
    }
}

int findImage(lua_State *L)
{
    const char *filename = lua_tostring(L, 1);
    bool cursorPos = lua_toboolean(L, 2);

    size_t x = lua_tointeger(L, 3);
    size_t y = lua_tointeger(L, 4);
    size_t width = lua_tointeger(L, 5);
    size_t heigth = lua_tointeger(L, 6);

    char line[512];
    sprintf(line, "easyfuncs FindImage \"data/%s\" %d %d %d %d %d", filename, cursorPos, x, y, width, heigth);

    lua_pushboolean(L, system(line) == 0 ? false : true);
    return 1;
}

int keyDown(lua_State* L) {
    SendKeys(lua_tointeger(L, 1), KeyboardEventType::KeyDown);
    return 1;
}

int keyUp(lua_State* L) {
    SendKeys(lua_tointeger(L, 1), KeyboardEventType::KeyUp);
    return 1;
}

int keyPress(lua_State* L) {
    SendKeys(lua_tointeger(L, 1), KeyboardEventType::KeyPress);
    return 1;
}

int wait(lua_State* L) {
    waitMS(lua_tointeger(L, 1));
    return 1;
}

int move(lua_State* L) {
    SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 1;
}

int leftDown(lua_State* L) 
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(MOUSEEVENTF_LEFTDOWN, -1, -1, 0, 0);
    return 1;
}

int leftUp(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(MOUSEEVENTF_LEFTUP, -1, -1, 0, 0);
    return 1;
}

int rightDown(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(MOUSEEVENTF_RIGHTDOWN, -1, -1, 0, 0);
    return 1;
}

int rightUp(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(MOUSEEVENTF_RIGHTUP, -1, -1, 0, 0);
    return 1;
}

int middleDown(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(MOUSEEVENTF_MIDDLEDOWN, -1, -1, 0, 0);
    return 1;
}

int middleUp(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(MOUSEEVENTF_MIDDLEUP, -1, -1, 0, 0);
    return 1;
}

int wheel(lua_State* L) {
    mouse_event(MOUSEEVENTF_WHEEL, lua_tointeger(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3) * 120, 0);
    return 1;
}

int click(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(GetSystemMetrics(23) != 0 ? 8 : 2, -1, -1, 0, 0);
    waitMS(80);
    mouse_event(GetSystemMetrics(23) != 0 ? 16 : 4, -1, -1, 0, 0);

    return 1;
}

int rightClick(lua_State* L)
{
    if (lua_gettop(L) == 2) {
        SetCursorPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
    }

    mouse_event(GetSystemMetrics(23) != 0 ? 2 : 8, -1, -1, 0, 0);
    waitMS(80);
    mouse_event(GetSystemMetrics(23) != 0 ? 4 : 16, -1, -1, 0, 0);

    return 1;
}

int switchWindow(lua_State* L)
{
    SwitchToWndByCaption(lua_tostring(L, 1));
    waitMS(80);

    return 1;
}

int wasKeyPressed(lua_State* L)
{
    uint32_t key = lua_tointeger(L, 1);
    lua_pushboolean(L, GetAsyncKeyState(key) ? true : false);

    while (GetAsyncKeyState(key)) {
        waitMS(0);
    }

    return 1;
}

int setClipboard(lua_State* L)
{
    const char* text = lua_tostring(L, 1);
    size_t len = strlen(text);

    auto ga = GlobalAlloc(GMEM_MOVEABLE, len + 1);

    OpenClipboard(HWND_DESKTOP);
    EmptyClipboard();

    memcpy(GlobalLock(ga), text, len + 1);
    GlobalUnlock(ga);
    SetClipboardData(CF_TEXT, ga);

    CloseClipboard();
    GlobalFree(ga);

    return 1;
}

int getClipboard(lua_State* L)
{
    OpenClipboard(NULL);

    HANDLE hData = GetClipboardData(CF_TEXT);
    char* text = static_cast<char*>(GlobalLock(hData));

    GlobalUnlock(hData);
    CloseClipboard();
    
    lua_pushstring(L, text);

    return 1;
}

int getMousePos(lua_State* L)
{
    POINT point;
    GetCursorPos(&point);

    lua_pushinteger(L, point.x);
    lua_pushinteger(L, point.y);

    return 2;
}

int main()
{
    lua_State *L = lua_open();
    luaL_openlibs(L);

    lua_register(L, "findImage", findImage);
    lua_register(L, "keyDown", keyDown);
    lua_register(L, "keyUp", keyUp);
    lua_register(L, "keyPress", keyPress);
    lua_register(L, "wait", wait);
    lua_register(L, "move", move);
    lua_register(L, "leftDown", leftDown);
    lua_register(L, "leftUp", leftUp);
    lua_register(L, "rightUp", rightUp);
    lua_register(L, "middleDown", middleDown);
    lua_register(L, "middleUp", middleUp);
    lua_register(L, "wheel", wheel);
    lua_register(L, "click", click);
    lua_register(L, "rightClick", rightClick);
    lua_register(L, "switchWindow", switchWindow);
    lua_register(L, "wasKeyPressed", wasKeyPressed);
    lua_register(L, "setClipboard", setClipboard);
    lua_register(L, "getClipboard", getClipboard);
    lua_register(L, "getMousePos", getMousePos);

    //-----
    luaL_dofile(L, "main.lua");
    
    lua_getglobal(L, "main");
    lua_pcall(L, 0, 1, -1);

    lua_close(L);

    system("pause > nul");
}