#ifndef ALCHEMY_FAR_CALL_H
#define ALCHEMY_FAR_CALL_H

/// @file
/// The far calls: how one bank of the main image calls a routine of another.
/// A bank's code moves whenever the bank is rebuilt, so each bank opens with a
/// table of 8-byte veneers that never moves, and callers jump through it. An
/// entry is its target's name with Far added (Text_DrawNumberAtOffsetFar
/// reaches Text_DrawNumberAtOffset); each bank's header here lists its entries
/// in the order of its table, src/gamepak/far_call/<bank>.s.

#include "edition.h"
#include "types.h"
#include "field/field_particle.h"
#include "system/irq.h"
#include "system/task.h"

struct AffineTransform;
struct SpriteAnimation;
struct Sprite;
struct BattleAction;
struct BattleEffectParams;
struct Actor;
struct Vector3;
struct FieldPosition;
struct MenuSprite;
struct Trigger;
struct ActorPlacement;
struct MenuCursor;
struct SpritePlacement;
struct SpriteScale;
struct WorldPosition;

// An entry takes its target's parameters, except the seven declared with
// empty lists, whose callers pass other arguments than the target takes.
#include "gamepak/far_call/system.h"
#include "gamepak/far_call/field.h"
#include "gamepak/far_call/window.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/field_sequence.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/shop.h"
#include "gamepak/far_call/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "gamepak/far_call/credits.h"
#include "gamepak/far_call/title.h"
#include "gamepak/far_call/lucky_dice.h"
#include "gamepak/far_call/lucky_wheels.h"
#include "gamepak/far_call/sound.h"
#include "gamepak/far_call/sprite.h"

#endif
