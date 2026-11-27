#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

/* @file */
/* What every part of the system library's object shares: its includes and */
/* the one call into the compiler's library that two parts make. system.c */
/* includes it before the parts. */

#include "resource_ids.h"
#include "battle/battle.h"
#include "field/map_grid_scroll.h"
#include "game/game.h"
#include "system/rom_header.h"
#include "graphics/graphics.h"
#include "system/save.h"

/* The compiler library's call through r3, which system_oam.c uses to run code
 */
/* copied to RAM. */
s32 _call_via_r3(s32, s32, s32, s32);

#endif
