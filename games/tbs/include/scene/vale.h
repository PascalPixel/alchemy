#ifndef ALCHEMY_VALE_H
#define ALCHEMY_VALE_H

/* @file */
/* What Vale (vale.c) and Vale in the storm (vale_prologue.c), which draw the */
/* same village, share. */

#include "flags.h"

/* The leader is on the rope bridge over the river. While it is set, the */
/* bridge's cells take the deck's shapes and cell behaviors: its two middle */
/* rows are walkable at the deck's height, and its rails are walls. Stepping */
/* off puts back the river's, walled water between open banks. A scene flag, */
/* cleared on every ordinary arrival. */
#define FLAG_VALE_ON_BRIDGE FLAG_SCENE_TEMP_10

#endif
