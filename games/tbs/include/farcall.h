#ifndef ALCHEMY_FARCALL_H
#define ALCHEMY_FARCALL_H

/// @file
/// Public interfaces to the engine's code banks. Each bank exposes a stable
/// jump table, so another bank can reach its routines beyond Thumb's call
/// range. These headers declare ordinary functions; the ordered assembly tables
/// in src/system/far_call own the entry names and the routines they jump to.
/// For example, Text_DrawNumberAtOffsetFar is the window bank's entry for
/// Text_DrawNumberAtOffset. Each bank's header, farcall/window.h and the
/// rest, is in its table's order, and the table is the far_call file of the
/// same name in capitals (WINDOW.S). Overlays bind their imported names to
/// these entries in src/system/overlay.inc, using the same 8-byte jump
/// mechanism. A file with a local declaration defines `OWN_<name>` before
/// including this header to keep that declaration in scope.

#include "task.h"
#include "field_effect_sprite.h"
#include "irq.h"
#include "types.h"
#include "edition.h"

struct AffineTransform;
struct SpriteAnim;
struct AnimationMetadata;
struct Sprite;
struct BattleAction;
struct BattleAnimArgs;
struct AbilityDefinition;
struct BattleObject;
struct Unit;
struct Object;
struct EffectLaunch;
struct ScreenPoint;
struct FieldEffectSprite;
struct EffectVector;
struct EnemyDefinition;
struct ObjectEvent;
struct FieldPosition;
struct BattleAnimObject;
struct ItemDefinition;
struct LevelUpResult;
struct MenuSprite;
struct PolarOffsetOutput;
struct GlyphRect;
struct WindowSprite;
struct MapEvent;
struct ObjectEventTemplate;
struct MenuCursor;
struct SpritePlacement;
struct SpriteScale;
struct SummonDefinition;
struct TransitionActorList;
struct Window;
struct WorldPosition;

// Entries have their target's parameters. Seven keep empty parameter lists
// (no prototype): their callers pass different arguments than the target
// takes, which C89 accepts with no prototype in scope. They are the
// blank-display runners (the warp destination in r0), Unit_LearnPsynergyFar
// (called with none), Party_ShowJoinedMessageFar (two),
// MapNamePopup_ShowFar, PauseMenu_RunFar and
// BuildStamp_GetTimeFar (one each). The near call Text_ResetStyle
// is declared the same way in unit/window/window.h (Window_Create passes it
// one).
#include "farcall/runtime.h"
#include "farcall/field.h"
#include "farcall/window.h"
#include "farcall/game.h"
#include "farcall/overworld.h"
#include "farcall/menu.h"
#include "farcall/shop.h"
#include "farcall/battle.h"
#include "farcall/battle_anim.h"
#include "farcall/credits.h"
#include "farcall/title.h"
#include "farcall/lucky_dice.h"
#include "farcall/lucky_wheels.h"
#include "farcall/sound.h"
#include "farcall/resource.h"

#endif
