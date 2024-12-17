/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file window_func.h %Window functions not directly related to making/drawing windows. */

#ifndef WINDOW_FUNC_H
#define WINDOW_FUNC_H

#include "window_type.h"
#include "company_type.h"
#include "core/geometry_type.hpp"
#include "core/strong_typedef_type.hpp"

// Export macro definition
#ifdef _WIN32
  #ifdef BUILDING_WINDOW_FUNC  // This will be defined when building the DLL or shared library
    #define EXPORT __declspec(dllexport)
  #else
    #define EXPORT __declspec(dllimport)
  #endif
#else
  #define EXPORT __attribute__((visibility("default")))  // For Linux/GCC/Clang
#endif

EXPORT Window *FindWindowById(WindowClass cls, WindowNumber number);
EXPORT Window *FindWindowByClass(WindowClass cls);
EXPORT Window *GetMainWindow();
EXPORT void ChangeWindowOwner(Owner old_owner, Owner new_owner);

template<typename T, std::enable_if_t<std::is_base_of<StrongTypedefBase, T>::value, int> = 0>
EXPORT Window *FindWindowById(WindowClass cls, T number)
{
    return FindWindowById(cls, number.base());
}

EXPORT void ResizeWindow(Window *w, int x, int y, bool clamp_to_screen = true, bool schedule_resize = true);
EXPORT int PositionMainToolbar(Window *w);
EXPORT int PositionStatusbar(Window *w);
EXPORT int PositionNewsMessage(Window *w);
EXPORT int PositionNetworkChatWindow(Window *w);

EXPORT int GetMainViewTop();
EXPORT int GetMainViewBottom();

EXPORT void InitWindowSystem();
EXPORT void UnInitWindowSystem();
EXPORT void ResetWindowSystem();
EXPORT void SetupColoursAndInitialWindow();
EXPORT void InputLoop();

EXPORT void InvalidateWindowData(WindowClass cls, WindowNumber number, int data = 0, bool gui_scope = false);
EXPORT void InvalidateWindowClassesData(WindowClass cls, int data = 0, bool gui_scope = false);

template<typename T, std::enable_if_t<std::is_base_of<StrongTypedefBase, T>::value, int> = 0>
EXPORT void InvalidateWindowData(WindowClass cls, T number, int data = 0, bool gui_scope = false)
{
    InvalidateWindowData(cls, number.base(), data, gui_scope);
}

EXPORT void CloseNonVitalWindows();
EXPORT void CloseAllNonVitalWindows();
EXPORT void DeleteAllMessages();
EXPORT void CloseConstructionWindows();
EXPORT void HideVitalWindows();
EXPORT void ShowVitalWindows();

/**
 * Re-initialize all windows.
 * @param zoom_changed Set if windows are being re-initialized due to a zoom level changed.
 */
EXPORT void ReInitAllWindows(bool zoom_changed);

EXPORT void SetWindowWidgetDirty(WindowClass cls, WindowNumber number, WidgetID widget_index);
EXPORT void SetWindowDirty(WindowClass cls, WindowNumber number);
EXPORT void SetWindowClassesDirty(WindowClass cls);

template<typename T, std::enable_if_t<std::is_base_of<StrongTypedefBase, T>::value, int> = 0>
EXPORT void SetWindowDirty(WindowClass cls, T number)
{
    SetWindowDirty(cls, number.base());
}

EXPORT void CloseWindowById(WindowClass cls, WindowNumber number, bool force = true, int data = 0);
EXPORT void CloseWindowByClass(WindowClass cls, int data = 0);

template<typename T, std::enable_if_t<std::is_base_of<StrongTypedefBase, T>::value, int> = 0>
EXPORT void CloseWindowById(WindowClass cls, T number, bool force = true, int data = 0)
{
    CloseWindowById(cls, number.base(), force, data);
}

EXPORT bool EditBoxInGlobalFocus();
EXPORT bool FocusedWindowIsConsole();
EXPORT Point GetCaretPosition();

#endif /* WINDOW_FUNC_H */
