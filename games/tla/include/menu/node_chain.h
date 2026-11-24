#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

/// @file
/// The selection screen's icon under the hand.

#include "types.h"

struct SelectionMenuState;
struct SelectionNode;

/// Returns the icon of the row the hand points at (cursorIndex).
struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionMenuState *screen);

#endif
