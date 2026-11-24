#ifndef ALCHEMY_WINDOW_CLOSE_H
#define ALCHEMY_WINDOW_CLOSE_H

/// @file
/// Closing a window.

#include "edition.h"
#include "uiwindow.h"

/// Closes a window: with instant 0 it zooms shut over WINDOW_CLOSE_FRAMES;
/// otherwise it is erased at once and freed.
void Window_Close(struct Window *window, s32 instant);

#endif
