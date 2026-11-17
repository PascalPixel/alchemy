#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

#include "types.h"

struct SelectionMenuWork;
struct SelectionNode;

struct SelectionNode *
SelectionMenu_GetCursorNode(struct SelectionMenuWork *screen);

#endif
