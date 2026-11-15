#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

#include "types.h"

struct SelectionScreen;
struct SelectionNode;

struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionScreen *screen);

#endif
