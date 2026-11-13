#ifndef ALCHEMY_FAR_CALL_H
#define ALCHEMY_FAR_CALL_H

/// @file
/// Public interfaces to the main image's code banks. Each bank exposes a stable
/// jump table, so another bank can reach its routines beyond Thumb's call
/// range. These headers declare ordinary functions; the ordered assembly tables
/// in src/gamepak/far_call own the entry names and the routines they jump to.
/// For example, Text_DrawNumberAtOffsetFar is the window bank's entry for
/// Text_DrawNumberAtOffset. Each bank's header, far_call/window.h and the
/// rest, is in its table's order, and the table is the far_call file of the
/// same name (window.s). Overlays bind their imported names to
/// these entries in src/system/overlay/imports.inc, using the same 8-byte jump
/// mechanism. A file with a local declaration defines `OWN_<name>` before
/// including this header to keep that declaration in scope.

#include "task.h"
#include "field_particle.h"
#include "irq.h"
#include "types.h"
#include "edition.h"

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

// Entries have their target's parameters, except seven declared with empty
// lists, whose callers pass other arguments than the target takes.
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
