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
/// Battlefield (x, y) pixel pairs by place: 0 to 3 the party's column at x 80,
/// 32 pixels apart, and 5 to 10 a column at x -80, 20 apart (battle_data.s).
extern const s8 gBattlePlacement_StepPairs[];
/// Read as a halfword by ObjectGroup_SetActionForOthers: the actor id of the
/// player's object, which it leaves alone.
extern u8 gPlayerObjectId[];
/// Each weapon's kind: bits 15..9 the kind, which indexes the
/// gBattleUnit_WeaponAnims tables, and 8..0 the item id; ended by 0xffff
/// (BattleUnit_GetWeaponKind).
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

/// The battle camera (struct BattleCamera) as the effects that swing it see it.
struct CameraWork {
  u8 unk00[0x36]; ///< BattleCamera's eye to pitch; not named in this view
  u16 scroll;     ///< the camera's yaw, in 1/65536 turns
};
/// A world position, as Camera_WorldToScreen converts it to the screen.
struct EffectVector {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};
/// One of the star field's three rings, moving outward on the effect canvas.
struct Ring {
  s32 x;    ///< 1/1024 canvas pixels
  s32 y;    ///< 1/1024 canvas pixels
  s32 velX; ///< 1/1024 canvas pixels per frame
  s32 velY; ///< 1/1024 canvas pixels per frame
  s32 age;  ///< frames
};
/// The position an item-break effect spawns its objects and shards from.
struct ScatterPosition {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};
extern u8 gAnimation_ChildPaletteCycle[];
/// The weights an enemy of action strategy 0, 1 or 2 picks among its eight
/// actions by: even, falling by 6, or falling steeply from 113 to 5.
extern u8 gBattleCommand_SelectionWeights[3][8];
/// The 32 by 16 spark of Avoid's falling arcs (spark_b), 4 bits a pixel;
/// FieldFx_RunAvoid loads it.
extern const u8 gBattleFx_ArcSparkTiles[];
extern const s32 gBattleFx_BurstParticleObjectScript[];
extern struct BattleEffectEntry gBattleFx_DefinitionTable[];
/// The emote bubble's script (BattleFx_SpawnLinked): eight 2-frame steps of
/// squash and stretch (SC_ADD to SW_SCALE_X and SW_SCALE_Y), 30 frames still,
/// then SC_FREE.
extern const s32 gBattleFx_LinkedObjectScript[];
extern const s32 gBattleFx_MarkerParticleScript[];
extern const s32 gBattleFx_ParticleEmitterScript[];
extern const s32 gBattleFx_ParticleScript[];
extern u32 gBattleFx_PulseScales[];
extern const s32 gBattleFx_SourceHoldScript[];
extern s16 gBattleFx_TargetRangeByMode[];
/// A lone SC_FREE: an object that has arrived frees itself
/// (BattleFx_SetCallbackWhenTargetUnset).
extern const s32 gBattleFx_UntargetedObjectScript[];
/// BattleMotion_ApplyVariantMotion's knockback by variant 0 to 7: 16.16 pixels
/// a frame gained each frame, 1 but 8 for variant 4.
extern s32 gBattleMotion_VariantAcceleration[];
/// The knockback's reach by variant, in percent of the unit's distance from
/// the centre: 140 to 200, 100 for variants 6 and 7.
extern s32 gBattleMotion_VariantDistancePercent[];
/// The knockback's top speed by variant, 16.16 pixels a frame: 4 but 20 for
/// variant 4.
extern s32 gBattleMotion_VariantSpeedLimit[];
/// The knockback's hop by variant, 16.16 pixels a frame upward: 2 to 10.
extern s32 gBattleMotion_VariantVelocityY[];
/// A fixed formation's places by order, as offsets from the middle place:
/// 0, -1, 1, -2, 2 and so on out to 4.
extern s8 gBattleParty_CenterOrderOffsets[];
/// Round-end effects go to the party (1), then the enemies (2).
extern const struct BattleGroupOrder gBattleParty_RoundEndGroupOrder;
/// A lone SC_END: a battle unit's object script, which BattleObjMake starts.
extern const s32 gBattlePres_ActorObjectScript[];
/// The advance arrow beside a battle message: eight 128-byte frames of a
/// 256-colour sprite, one every four frames.
extern u8 gBattlePres_AdvanceArrowTiles[];
/// The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each.
extern const u8 gBattlePres_CurtainTiles[];
/// Eight curtain tiles, 32 bytes each, of which the transition copies one at a
/// time into the curtain's first tile.
extern const u8 gBattlePres_TileVariants[];
/// Garet's weapon sprite by weapon kind (BattleUnit_LookupWeaponValueByClass).
extern u16 gBattleUnit_WeaponAnimsClass1[];
/// Ivan's weapon sprite by weapon kind.
extern u16 gBattleUnit_WeaponAnimsClass2[];
/// Mia's weapon sprite by weapon kind.
extern u16 gBattleUnit_WeaponAnimsClass3[];
/// Jenna's weapon sprite by weapon kind; Felix (4) draws none.
extern u16 gBattleUnit_WeaponAnimsClass5[];
/// The size of BitDecoder_DecodeImage, which BattleBackground_Load copies to
/// heap slot 49; constants.ld defines it as an address equal to the size.
extern u8 BitDecoder_Size[];
/// The extra matrix the battle camera's projection applies first while
/// commands are chosen (FLAG_CHOOSING_COMMANDS), in 16.16 words.
extern u8 gCamera_FlagTransformWork[];
/// The 32 by 16 spark a field Psynergy effect's particles draw (spark_a), 4
/// bits a pixel; BattleFx_LoadActionEffectResources loads it.
extern const u8 gBattleFx_SparkTiles[];
extern const u8 gBattleFx_QuadrantModes[];
/// The eight tints (PALETTE_TRANSFORM_TINT and an RGB555 colour) a field
/// Psynergy effect fades the palettes toward, picked by the frame counter or
/// by flags 0x148 to 0x14f (BattleEffect_InitializeSharedScene).
extern const s32 gBattleFx_TargetColors[];
/// Where SparkWork.sheet holds the spark's square picture for each frame of
/// life left, 0 to 6: byte offsets, a byte a pixel.
extern s32 gBattleFx_StarWorkOffsets[];
/// The spark's width and height in pixels by life left: 1 to 4, 4 from three.
extern u8 gBattleFx_StarSizes[];
/// Where SparkWork.sheet holds the ring's four 32 by 32 frames: byte offsets,
/// 0x400 apart.
extern s32 gBattleFx_RingFrameOffsets[];
/// The highlighted units' BLDY lightening: 15 down to 0 once, then the second
/// sixteen (a pulse up to 8 and back) over and over.
extern s8 gBattleFx_HighlightBlendRamp[];
/// The 104 encounter zones (encounter.tsv), read as struct EncounterZone rows.
extern u16 gEncounter_EnemyGroupTable[];
extern s16 gFieldFx_GroundParticleFrames[];
/// The ground particles' 16 by 16 tiles, packed (Resource_DecodeByteLz).
extern const u8 gFieldFx_GroundParticleTiles[];
#ifndef OWN_gBattleBgFxWork
/// Work slot 22: Mind Read's wave work while its effect runs.
extern struct WaveFxWork *gBattleBgFxWork;
#endif
#ifndef OWN_gBattleFxWork
/// Work slots 39 to 41 (HEAP_SLOT_BATTLE_EFFECT, then the canvas and sheet):
/// the running battle effect's BattleEffectWork first.
extern void *gBattleFxWork[];
#endif
#ifndef OWN_gBattleOwnerStates
/// Work slot 54: the six enemy battle units, BATTLE_UNIT_SIZE bytes each, that
/// UnitGet returns for unit ids 128 to 133 during a battle.
extern u8 *gBattleOwnerStates;
#endif
#ifndef OWN_gRandomSeed
/// Random16's seed; the slot machine seeds it with the play time, and a link
/// battle's parent sends it as the round's gBattleRandomSeed.
extern u32 gRandomSeed;
#endif
#ifndef OWN_gTransitionWork
/// Work slot 44 (HEAP_SLOT_BATTLE_BACKGROUND) as the camera's turn work:
/// staging.c, auto_battle.c and round.c set its yaw for each action, and
/// background.c reads the same slot as struct BattleBackgroundView.
extern struct BattlePresentationTransition *gTransitionWork;
#endif
/// SC_FACE_LINK every frame, forever: the script of an actor told to keep
/// facing its linked object.
extern const u8 gObjectMotion_LinkedActionScript[];
/// Where the party returns after a defeat: {scene, entrance (-1 any), return
/// scene, return entrance} rows, ended by scene -1
/// (Scene_SetDefeatReturnPoint).
extern const s16 gParty_PairResolveRules[];
extern const s32 gBattleFx_RisingPulseScript[];
/// Isaac's weapon sprite by weapon kind (BattleUnit_LookupWeaponValueByClass).
extern u16 gBattleUnit_WeaponAnimsClass0[];
/// The 20 enemy classes whose defeated flags a monster battle clears before it
/// picks a formation (BattleFormation_SelectLevelMatchedCandidate).
extern u16 gBattleFormation_LevelMatchedEnemies[];
/// Battle backdrops by scene: a backdrop word (bit 31 set, resource id below)
/// and then its scenes, ended by 0; a scene not listed takes the last backdrop
/// (Battle_SelectSceneBackdrop).
extern s32 gBattle_BackdropScenes[];
/// The template BattleFx_BuildBlitter walks word by word to write a blitter
/// (battle_effect_composite.s); none of it runs where it is.
extern const u32 BattleFx_Composite[];
/// The message voice's rate by message speed option: 1, 3, then 10
/// (Ui_SetMessageVoiceFromActor).
extern u8 gUi_RenderResultValues[];
extern const u16 gWorldMap_CursorDirectionAngles[];
extern const u8 gWorldMap_MarkerBlendCycle[];
/// The world map's place markers, {tile << 16 | flag, place} for each place
/// actor from 11, ended by -1; a zero row reserves an actor with no marker.
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
