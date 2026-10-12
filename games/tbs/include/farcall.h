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
struct BtlAct;
struct BtlObj;
struct BtlUnit;
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
