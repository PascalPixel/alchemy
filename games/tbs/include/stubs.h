#ifndef ALCHEMY_STUBS_H
#define ALCHEMY_STUBS_H

/* @file */
/* The stub calls: how one module of the main image calls a function of */
/* another. A module's code moves whenever the module is rebuilt, so each */
/* module opens with a table of 8-byte stubs that never moves, and callers */
/* jump through it. An entry is its target's name with Far added */
/* (mnTextDrawNumAtOffsetStub reaches mnTextDrawNumAtOffset); each module's */
/* header here lists its entries in the order of its table, the stubs.s of */
/* its own folder. */

#include "edition.h"
#include "types.h"
#include "field/field_particle.h"
#include "system/globals.h"

struct AFFINE_TRANSFORM;
struct SPR_ANIMATION;
struct SPR;
struct BATTLE_ACTION;
struct BATTLE_EFFECT_PARM;
struct ACTOR;
struct VECTOR3;
struct FIELD_POS;
struct MENU_SPR;
struct TRIGGER;
struct ACTOR_PLACE;
struct MENU_CURSOR;
struct SPR_PLACEMENT;
struct SPR_SCALE;
struct WORLD_POS;

/* An entry takes its target's parameters, except the seven declared with */
/* empty lists, whose callers pass other arguments than the target takes. */
#include "system/stubs.h"
#include "field/stubs.h"
#include "window/stubs.h"
#include "game/stubs.h"
#include "field/sequence_stubs.h"
#include "menu/stubs.h"
#include "shop/stubs.h"
#include "battle/stubs.h"
#include "battle_effect/stubs.h"
#include "menu/credits_stubs.h"
#include "menu/title_stubs.h"
#include "field/lucky_dice_stubs.h"
#include "field/lucky_wheels_stubs.h"
#include "sound/stubs.h"
#include "system/sprite_stubs.h"

#endif
