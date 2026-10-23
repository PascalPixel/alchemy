#ifndef ALCHEMY_FARCALL_H
#define ALCHEMY_FARCALL_H

/// @file
/// Public interfaces to the engine's code banks. Each bank exposes a stable
/// jump table, so another bank can reach its routines beyond Thumb's call
/// range. These headers declare ordinary functions; the ordered assembly tables
/// in src/system/far_call own the entry names and the routines they jump to.
/// For example, UiText_DrawNumberAtOffsetFar is the window bank's entry for
/// UiText_DrawNumberAtOffset. Each bank's header, farcall/window.h and the
/// rest, is in its table's order, and the table is the far_call file of the
/// same name in capitals (WINDOW.S). Overlays bind their imported names to
/// these entries in src/system/overlay.inc, using the same 8-byte jump
/// mechanism. A file with a local declaration defines `OWN_<name>` before
/// including this header to keep that declaration in scope.

#include "callback_scheduler.h"
#include "effect_slot.h"
#include "irq.h"
#include "types.h"
#include "edition.h"

struct AffineTransform;
struct AnimationEntry;
struct AnimationMetadata;
struct AnimationObject;
struct BattleAction;
struct BattleAnimArgs;
struct AbilityDefinition;
struct BattleObject;
struct Unit;
struct DispatchObject;
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
struct MotionObject;
struct Object;
struct PolarOffsetOutput;
struct RenderInput;
struct RenderOutput;
struct MapEvent;
struct ObjectEventTemplate;
struct ScriptObjectEntry;
struct ShopCursor;
struct SpritePlacement;
struct SpriteScale;
struct SummonDefinition;
struct TransitionActorList;
struct UiWindow;
struct WorldPosition;

// Entries have their target's parameters. Seven keep empty parameter lists
// (no prototype): their callers pass different arguments than the target
// takes, which C89 accepts with no prototype in scope. They are the
// blank-display runners (the warp destination in r0), Unit_LearnPsynergyFar
// (called with none), Party_ShowJoinedMessageFar (two),
// UiTimedNotice_CreateFar, Menu_RunWorkspaceResultLoopFar and
// Runtime_GetBuildStampTimeFar (one each). The near call UiWork_ResetCounters
// is declared the same way in unit/window/window.h (UiWindow_Create passes it
// one).
#include "farcall/runtime.h"
#include "farcall/field.h"
#include "farcall/window.h"
#include "farcall/game.h"
#include "farcall/object.h"
#include "farcall/menu.h"
#include "farcall/shop.h"
#include "farcall/battle.h"
#include "farcall/effect.h"
#include "farcall/scroll.h"
#include "farcall/palette.h"
#include "farcall/blank_run.h"
#include "farcall/blank_load.h"
#include "farcall/sound.h"
#include "farcall/resource.h"

#endif
