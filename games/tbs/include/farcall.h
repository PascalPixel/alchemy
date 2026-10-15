#ifndef ALCHEMY_FARCALL_H
#define ALCHEMY_FARCALL_H

// The far-call banks (games/tbs/src/system/far_call) as C declarations.
// Each bank is one list: FAR_CALL names a stub and declares it, FAR_ENTRY
// names a stub no declaration is established for, FAR_SLOT is a stub with
// no name. The bank's assembly reads the same list for the stubs.
// A translation unit that keeps its own declaration of a listed name
// defines OWN_<name> before its includes, and the list leaves it out there.

#include "callback_scheduler.h"
#include "effect_slot.h"
#include "irq.h"
#include "types.h"
#include "edition.h"

struct AffineTransform;
struct AnimationEntry;
struct AnimationMetadata;
struct AnimationObject;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleAct;
struct BattleObj;
struct BattleUnit;
struct DispatchObject;
struct EffectLaunch;
struct EffectPosition;
struct EffectSlot;
struct EffectVector;
struct EnemyDefinition;
struct FieldActor;
struct FieldPosition;
struct FxObject;
struct ItemDefinition;
struct LevelUpResult;
struct MenuSprite;
struct MotionObject;
struct ObjectRuntime;
struct PolarOffsetOutput;
struct RenderInput;
struct RenderOutput;
struct ResourceObject;
struct ResourceObjectWork;
struct ScenePlacement;
struct SceneRegionEntry;
struct ScriptObjectEntry;
struct ShopCursor;
struct SummonDefinition;
struct TransitionActorList;
struct UiWindow;
struct WorldPosition;

// Each stub is declared with its target's parameters (params). Seven rows
// have empty parameters instead, a declaration with no prototype: the
// bytes show their callers passing other arguments than the target takes,
// which C89 accepts only with no prototype in scope. They are the
// blank-display runners (the entrance in r0), OwnerAction_AddFar (called
// with none), Party_ShowJoinedMessageFar (two), UiTimedNotice_CreateFar,
// Menu_RunWorkspaceResultLoopFar and Runtime_GetBuildStampTimeFar (one
// each). The near call UiWork_ResetCounters is declared the same way in
// unit/window/window.h (UiWindow_Create passes it one).
#define FAR_CALL(name, target, type, params) type name params;
#define FAR_ENTRY(name, target)
#define FAR_SLOT(target)

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

#undef FAR_CALL
#undef FAR_ENTRY
#undef FAR_SLOT

#endif
