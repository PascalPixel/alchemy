#ifndef BATTLE_H
#define BATTLE_H

/// @file
/// The battle bank's routines and globals, as the other banks see them.

#include "types.h"
#include "edition.h"
#include "vector3.h"
#include "battle_effect_runtime.h"
#include "farcall.h"

struct BattleActionRecord;
struct BattleEffectArgument;
struct BattlePlan;
struct BattleUnit;
struct DispatchObject;
struct EffectSlot;
struct ObjectRuntime;
struct SceneAreaNameRule;

extern u16 gBattleFx_CyclePatternWords[];
extern s8 gBattleFx_RandomChildValues[];
extern const s8 gBattlePlacement_StepPairs[];
extern u8 gPlayerObjectId[];
extern u16 gResource_SlotAssignments[];
struct BattleObj;

struct AnimationEntry;
struct AnimationObject;
struct ArcEffectObject;
struct ArcObject;
struct ArcParticle;
struct BattleAct;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleEffectEntry;
struct BattleEventState;
struct BattleGroupOrder;
struct BattleLocationRule;
struct BattleObj;
struct BattlePlan;
struct BattlePresentationTransition;
struct BattleSummonState;
struct BattleUnit;
struct DescendingOrbitObject;
struct EffectDescriptor;
struct EffectSlot;
struct EnemyDefinition;
struct FieldActor;
struct FxObject;
struct SpriteScale;
struct ItemBreakFragmentSource;
struct LinkedCaptureObject;
struct MotionObject;
struct ObjectRuntime;
struct OrbitEffect;
struct PolarOffsetOutput;
struct RockWallHeights;
struct SceneInteractionEntry;
struct ScriptObjectEntry;
struct BattleSource;
struct SparkObject;
struct TransitionActorList;
struct WaveFxWork;

struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};
struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};
struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
  s32 age;
};
struct ScatterPosition {
  s32 x;
  s32 y;
  s32 z;
};
extern u8 gAnimation_ChildPaletteCycle[];
extern u8 gBattleCommand_SelectionWeights[3][8];
extern const u8 gBattleFx_ArcSparkTiles[];
extern const s32 gBattleFx_BurstParticleObjectScript[];
extern struct BattleEffectEntry gBattleFx_DefinitionTable[];
extern const s32 gBattleFx_LinkedObjectScript[];
extern const s32 gBattleFx_MarkerParticleScript[];
extern const s32 gBattleFx_ParticleEmitterScript[];
extern const s32 gBattleFx_ParticleScript[];
extern u32 gBattleFx_PulseScales[];
extern const s32 gBattleFx_SourceHoldScript[];
extern s16 gBattleFx_TargetRangeByMode[];
extern const s32 gBattleFx_UntargetedObjectScript[];
extern s32 gBattleMotion_VariantAcceleration[];
extern s32 gBattleMotion_VariantDistancePercent[];
extern s32 gBattleMotion_VariantSpeedLimit[];
extern s32 gBattleMotion_VariantVelocityY[];
extern s8 gBattleParty_CenterOrderOffsets[];
extern const struct BattleGroupOrder gBattleParty_RoundEndGroupOrder;
extern const s32 gBattlePres_ActorObjectScript[];
extern u8 gBattlePres_AdvanceArrowTiles[];
extern const u8 gBattlePres_CurtainTiles[];
extern const u8 gBattlePres_TileVariants[];
extern u16 gBattleUnit_WeaponAnimsClass1[];
extern u16 gBattleUnit_WeaponAnimsClass2[];
extern u16 gBattleUnit_WeaponAnimsClass3[];
extern u16 gBattleUnit_WeaponAnimsClass5[];
extern u8 BitDecoder_Size[];
extern u8 gCamera_FlagTransformWork[];
extern const u8 gBattleFx_SparkTiles[];
extern const u8 gBattleFx_QuadrantModes[];
extern const s32 gBattleFx_TargetColors[];
extern s32 gBattleFx_StarWorkOffsets[];
extern u8 gBattleFx_StarSizes[];
extern s32 gBattleFx_RingFrameOffsets[];
extern s8 gBattleFx_HighlightBlendRamp[];
extern u16 gEncounter_EnemyGroupTable[];
extern s16 gFieldFx_GroundParticleFrames[];
extern const u8 gFieldFx_GroundParticleTiles[];
#ifndef OWN_gBattleBgFxWork
extern struct WaveFxWork *gBattleBgFxWork;
#endif
#ifndef OWN_gBattleFxWork
extern void *gBattleFxWork[];
#endif
#ifndef OWN_gBattleOwnerStates
extern u8 *gBattleOwnerStates;
#endif
#ifndef OWN_gRandomSeed
extern u32 gRandomSeed;
#endif
#ifndef OWN_gTransitionWork
extern struct BattlePresentationTransition *gTransitionWork;
#endif
extern const u8 gObjectMotion_LinkedActionScript[];
extern const s16 gParty_PairResolveRules[];
extern const s32 gBattleFx_RisingPulseScript[];
extern u16 gBattleUnit_WeaponAnimsClass0[];
extern u16 gBattleFormation_LevelMatchedEnemies[];
extern s32 gBattle_BackdropScenes[];
extern const u32 BattleFx_Composite[];
extern u8 gUi_RenderResultValues[];
extern const u16 gWorldMap_CursorDirectionAngles[];
extern const u8 gWorldMap_MarkerBlendCycle[];
extern const s32 gWorldMap_PlaceMarkers[];
u32 __udivsi3(u32 numerator, u32 denominator);
#ifndef OWN__call_via_r0
void _call_via_r0(u32 routine);
#endif
void Animation_ApplyChildArgumentFar(void *, s32);
void BitDecoder_DecodeImage(void);
void FarCall_WindowTable(void);
struct AnimationObject *GetBattleEffectObject(s32 kind);
void ItemIcon_LoadTilesFar(s32 item);
#ifndef OWN_Object_InitializeMode
s32 Object_InitializeMode(struct AnimationObject *state, s32 flags);
#endif

/// The battle sprites' animations (AnimationObjects_SelectAnimation numbers)
/// that the battle code picks by name.
enum BattlePose {
  POSE_ENEMY_HELD = 0, ///< an enemy asleep, stunned or held
  POSE_STANDING = 1,
  POSE_PARTY_HELD = 4, ///< asleep, stunned or held; also a downed member
  POSE_DAMAGE = 5,     ///< taking damage
  POSE_ENEMY_DOWN = 5  ///< a downed enemy stays in its damage pose
};

#endif
