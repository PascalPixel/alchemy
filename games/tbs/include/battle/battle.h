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
/// The weights an enemy picks among its eight actions by, one table for each
/// weighted action strategy (enum EnemyActionStrategy, battle_internal.h):
/// even, falling by 6, or falling steeply from 113 to 5.
extern u8 gBattleCommand_SelectionWeights[3][8];
/// The knockback's speed gained each frame by variant (enum BattleKnockback),
/// 16.16 pixels a frame: 1, but 8 for KNOCKBACK_HURL.
extern s32 gBattleMotion_VariantAcceleration[];
/// The knockback's reach by variant, in percent of the unit's distance from
/// the centre: 140 to 200, 100 for KNOCKBACK_BOUNCE and KNOCKBACK_JOLT.
extern s32 gBattleMotion_VariantDistancePercent[];
/// The knockback's top speed by variant, 16.16 pixels a frame: 4, but 20 for
/// KNOCKBACK_HURL.
extern s32 gBattleMotion_VariantSpeedLimit[];
/// The knockback's hop by variant, 16.16 pixels a frame upward: 2 to 10.
extern s32 gBattleMotion_VariantVelocityY[];
/// A fixed formation's places by order, as offsets from the middle place:
/// 0, -1, 1, -2, 2 and so on out to 4.
extern s8 gBattleParty_CenterOrderOffsets[];
/// Round-end effects go to the party (BATTLE_SIDE_PARTY), then the enemies
/// (BATTLE_SIDE_ENEMIES).
extern const struct BattleGroupOrder gBattleParty_RoundEndGroupOrder;
/// A lone SC_END: a battle unit's object script, which BattleObjMake starts.
extern const s32 gBattlePresentation_ActorObjectScript[];
/// The advance arrow beside a battle message: eight 128-byte frames of a
/// 256-colour sprite, one every four frames.
extern u8 gBattlePresentation_AdvanceArrowTiles[];
/// The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each.
extern const u8 gBattlePresentation_CurtainTiles[];
/// Eight curtain tiles, 32 bytes each, of which the transition copies one at a
/// time into the curtain's first tile.
extern const u8 gBattlePresentation_TileVariants[];
/// Garet's weapon sprite by weapon kind (BattleUnit_LookupWeaponValueByClass).
extern u16 gBattleUnit_WeaponAnimsGaret[];
/// Ivan's weapon sprite by weapon kind.
extern u16 gBattleUnit_WeaponAnimsIvan[];
/// Mia's weapon sprite by weapon kind.
extern u16 gBattleUnit_WeaponAnimsMia[];
/// Jenna's weapon sprite by weapon kind; Felix draws none.
extern u16 gBattleUnit_WeaponAnimsJenna[];
/// The size of BitDecoder_DecodeImage, which BattleBackground_Load copies to
/// HEAP_SLOT_TEMP_RENDERER; constants.ld defines it as an address equal to the
/// size.
extern u8 BitDecoder_Size[];
/// The extra matrix the battle camera's projection applies first while
/// commands are chosen (FLAG_CHOOSING_COMMANDS), in 16.16 words.
extern u8 gCamera_FlagTransformWork[];
/// Where SparkWork.sheet holds the spark's square picture for each frame of
/// life left, 0 to 6: byte offsets, a byte a pixel.
extern s32 gElementGlow_StarWorkOffsets[];
/// The spark's width and height in pixels by life left: 1 to 4, 4 from three.
extern u8 gElementGlow_StarSizes[];
/// Where SparkWork.sheet holds the ring's four 32 by 32 frames: byte offsets,
/// 0x400 apart.
extern s32 gElementGlow_RingFrameOffsets[];
/// The highlighted units' BLDY lightening: 15 down to 0 once, then the second
/// sixteen (a pulse up to 8 and back) over and over.
extern s8 gBattlePresentation_HighlightBlendRamp[];
/// The 104 encounter zones (encounter.tsv), read as struct EncounterZone rows.
extern u16 gEncounter_EnemyGroupTable[];
#ifndef OWN_gBattleBgFxWork
/// The HEAP_SLOT_MIND_READ work slot: Mind Read's wave work while its effect
/// runs.
extern struct WaveFxWork *gBattleBgFxWork;
#endif
#ifndef OWN_gBattleFxWork
/// The work slots from HEAP_SLOT_BATTLE_EFFECT, then the canvas and sheet:
/// the running battle effect's BattleEffectWork first.
extern void *gBattleFxWork[];
#endif
#ifndef OWN_gBattleOwnerStates
/// The HEAP_SLOT_LINK_OWNER_STATES work slot: the six enemy battle units,
/// BATTLE_UNIT_SIZE bytes each, that UnitGet returns for unit ids 128 to 133
/// during a battle.
extern u8 *gBattleOwnerStates;
#endif
#ifndef OWN_gRandomSeed
/// Random16's seed; the slot machine seeds it with the play time, and a link
/// battle's parent sends it as the round's gBattleRandomSeed.
extern u32 gRandomSeed;
#endif
#ifndef OWN_gTransitionWork
/// The HEAP_SLOT_BATTLE_BACKGROUND work slot as the camera's turn work:
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
/// Isaac's weapon sprite by weapon kind (BattleUnit_LookupWeaponValueByClass).
extern u16 gBattleUnit_WeaponAnimsIsaac[];
/// The 20 enemy classes whose defeated flags a monster battle clears before it
/// picks a formation (BattleFormation_SelectLevelMatchedCandidate).
extern u16 gBattleFormation_LevelMatchedEnemies[];
/// Battle backdrops by scene: a backdrop word (bit 31 set, resource id below)
/// and then its scenes, ended by 0; a scene not listed takes the last backdrop
/// (Battle_SelectSceneBackdrop).
extern s32 gBattle_BackdropScenes[];
/// The template BattleEffect_BuildBlitter walks word by word to write a blitter
/// (battle_effect_composite.s); none of it runs where it is.
extern const u32 BattleEffect_Composite[];
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

/// BattleMotion_ApplyVariantMotion's knockbacks, the columns of the
/// gBattleMotion_Variant* tables in battle_vars.s. Each is named by how far
/// and how high it sends the unit, as percent of its spot's distance from the
/// centre and its hop in pixels a frame; all but KNOCKBACK_HURL accelerate by
/// 1 pixel a frame to at most 4.
enum BattleKnockback {
  KNOCKBACK_NUDGE,  ///< 140 percent, a hop of 3
  KNOCKBACK_HOP,    ///< 140 percent, a hop of 6
  KNOCKBACK_LEAP,   ///< 150 percent, a hop of 7
  KNOCKBACK_LAUNCH, ///< 200 percent, a hop of 10
  /// The hardest: 200 percent at 8 pixels a frame more each frame, to at
  /// most 20, with a hop of 5.
  KNOCKBACK_HURL,
  /// 150 percent, a hop of 2. From here on a unit already in the air takes
  /// the new hop too.
  KNOCKBACK_SKID,
  KNOCKBACK_BOUNCE, ///< the spot itself, a hop of 3
  KNOCKBACK_JOLT    ///< the spot itself, a hop of 5.5
};

/// The battle sprites' animations (AnimationObjects_SelectAnimation numbers)
/// that the battle code picks by name.
enum BattlePose {
  POSE_ENEMY_HELD = 0, ///< an enemy asleep, stunned or held
  POSE_STANDING = 1,
  POSE_LEAP = 2,       ///< running up to a target and leaping
  POSE_CAST = 3,       ///< a hand raised to cast or use an item
  POSE_PARTY_HELD = 4, ///< asleep, stunned or held; also a downed member
  POSE_DAMAGE = 5,     ///< taking damage
  POSE_ENEMY_DOWN = 5  ///< a downed enemy stays in its damage pose
};

/// A battle unit's sprites' OBJ mode, as BattlePresentation_SetActorRecordMode
/// sets it: faded units are semi-transparent, and the battle's blends act on
/// them.
enum BattleActorMode { ACTOR_OPAQUE, ACTOR_SEMI_TRANSPARENT };

/// BattlePresentation_SetActorModes' mode, kept in BattleWork.highlightMode:
/// whether the selected units are highlighted, every other unit faded.
enum BattleHighlight { HIGHLIGHT_OFF, HIGHLIGHT_SELECTED };

/// The glows ElementGlow_Play plays about a unit in its element.
enum BattleElementGlow {
  /// The star field gathering on the unit while the background darkens: a
  /// cast.
  GLOW_STAR_FIELD,
  GLOW_STREAKS, ///< the element's streaks: an enemy's cast
  GLOW_PILLAR,  ///< the element's pillar, at a strength: a summon's boost
  GLOW_BURST    ///< the element's burst: a Djinni set
};

#endif
