#ifndef BATTLE_H
#define BATTLE_H

/// @file
/// The battle bank's routines and globals, as the other banks see them.

#include "types.h"
#include "edition.h"
#include "vector3.h"
#include "speaker.h"
#include "farcall.h"

struct BattleAction;
struct BattleAnimArgs;
struct BattleTurn;
struct Unit;
struct Object;
struct FieldEffectSprite;
struct AreaNameRule;

/// Battlefield (x, y) pixel pairs by place: 0 to 3 the party's column at x 80,
/// 32 pixels apart, and 5 to 10 a column at x -80, 20 apart (battle_data.s).
extern const s8 gBattlePlacement_StepPairs[];
/// Read as a halfword by Actor_SetOthersPaused: the actor id of the
/// player's object, which it leaves alone.
extern u8 gPlayerObjectId[];
/// Each weapon's kind: bits 15..9 the kind, which indexes the
/// gBattleUnit_WeaponAnimations tables, and 8..0 the item id; ended by 0xffff
/// (BattleUnit_GetWeaponKind).
extern u16 gBattleUnit_WeaponKinds[];
struct BattleObject;

struct SpriteAnim;
struct Sprite;
struct ArcObject;
struct ArcParticle;
struct AbilityDefinition;
struct BattleAction;
struct BattleAnimArgs;
struct BattlePlaybackWork;
struct BattleSideOrder;
struct BattleLocationRule;
struct BattleObject;
struct BattleTurn;
struct BattleBackgroundWork;
struct BattleSummonState;
struct Unit;
struct TriggerView;
struct FieldEffectSprite;
struct EnemyDefinition;
struct Actor;
struct BattleAnimObject;
struct SpriteScale;
struct ItemBreakFragmentSource;
struct LinkedCaptureObject;
struct OrbitEffect;
struct PolarOffsetOutput;
struct RockWallHeights;
struct MapMusicRule;
struct BattleSource;
struct TransitionActorList;
struct MindReadWork;

/// One of the star field's three rings, moving outward on the animation canvas.
struct Ring {
  s32 x;         ///< 1/1024 canvas pixels
  s32 y;         ///< 1/1024 canvas pixels
  s32 velocityX; ///< 1/1024 canvas pixels per frame
  s32 velocityY; ///< 1/1024 canvas pixels per frame
  s32 age;       ///< frames
};
extern u8 gMindRead_CasterPaletteCycle[];
/// The weights an enemy picks among its eight actions by, one table for each
/// weighted action strategy (enum EnemyActionStrategy, battle_internal.h):
/// even, falling by 6, or falling steeply from 113 to 5.
extern u8 gBattleAI_ActionWeights[3][8];
/// The knockback's speed gained each frame by variant (enum BattleKnockback),
/// 16.16 pixels a frame: 1, but 8 for KNOCKBACK_HURL.
extern s32 gBattleKnockback_Acceleration[];
/// The knockback's reach by variant, in percent of the unit's distance from
/// the centre: 140 to 200, 100 for KNOCKBACK_BOUNCE and KNOCKBACK_JOLT.
extern s32 gBattleKnockback_DistancePercent[];
/// The knockback's top speed by variant, 16.16 pixels a frame: 4, but 20 for
/// KNOCKBACK_HURL.
extern s32 gBattleKnockback_SpeedLimit[];
/// The knockback's hop by variant, 16.16 pixels a frame upward: 2 to 10.
extern s32 gBattleKnockback_VelocityY[];
/// A fixed formation's places by order, as offsets from the middle place:
/// 0, -1, 1, -2, 2 and so on out to 4.
extern s8 gBattleParty_CenterOrderOffsets[];
/// Round-end effects go to the party (BATTLE_SIDE_MASK_PARTY), then the enemies
/// (BATTLE_SIDE_MASK_ENEMIES).
extern const struct BattleSideOrder gBattleParty_RoundEndSideOrder;
/// A lone SC_END: a battle unit's object script, which BattleObject_Make
/// starts.
extern const s32 gBattleObject_EmptyScript[];
/// The advance arrow beside a battle message: eight 128-byte frames of a
/// 256-colour sprite, one every four frames.
extern u8 gBattleMessage_AdvanceArrowTiles[];
/// The battle transition's curtain tiles, 32 bytes (one 4-bit tile) each.
extern const u8 gBattleTransition_CurtainTiles[];
/// Eight curtain tiles, 32 bytes each, of which the transition copies one at a
/// time into the curtain's first tile.
extern const u8 gBattleTransition_CurtainVariants[];
/// Garet's weapon sprite by weapon kind (BattleUnit_LookupWeaponValueByClass).
extern u16 gBattleUnit_WeaponAnimationsGaret[];
/// Ivan's weapon sprite by weapon kind.
extern u16 gBattleUnit_WeaponAnimationsIvan[];
/// Mia's weapon sprite by weapon kind.
extern u16 gBattleUnit_WeaponAnimationsMia[];
/// Jenna's weapon sprite by weapon kind; Felix draws none.
extern u16 gBattleUnit_WeaponAnimationsJenna[];
/// The size of BitDecoder_DecodeImage, which BattleBackground_Load copies to
/// HEAP_SLOT_RAM_CODE_TEMP; constants.ld defines it as an address equal to the
/// size.
extern u8 BitDecoder_Size[];
/// The extra matrix the battle camera's projection applies first while
/// commands are chosen (FLAG_CHOOSING_COMMANDS), in 16.16 words.
extern u8 gBattleCamera_CommandMatrix[];
/// Where SparkWork.sheet holds the spark's square picture for each frame of
/// life left, 0 to 6: byte offsets, a byte a pixel.
extern s32 gElementGlow_StarFrameOffsets[];
/// The spark's width and height in pixels by life left: 1 to 4, 4 from three.
extern u8 gElementGlow_StarSizes[];
/// Where SparkWork.sheet holds the ring's four 32 by 32 frames: byte offsets,
/// 0x400 apart.
extern s32 gElementGlow_RingFrameOffsets[];
/// The highlighted units' BLDY lightening: 15 down to 0 once, then the second
/// sixteen (a pulse up to 8 and back) over and over.
extern s8 gBattleHighlight_BlendRamp[];
/// The 104 encounter zones (encounter_zones.c), read as struct EncounterZone
/// rows.
extern u16 gEncounterZones[];
#ifndef OWN_gMindReadWork
/// The HEAP_SLOT_MIND_READ heap slot: Mind Read's wave work while its animation
/// runs.
extern struct MindReadWork *gMindReadWork;
#endif
#ifndef OWN_gBattleAnimWork
/// The heap slots from HEAP_SLOT_BATTLE_ANIM, then the canvas and sheet:
/// the running battle animation's BattleAnimWork first.
extern void *gBattleAnimWork[];
#endif
#ifndef OWN_gEnemyUnits
/// The HEAP_SLOT_ENEMY_UNITS heap slot: the six enemy battle units,
/// BATTLE_UNIT_SIZE bytes each, that Unit_Get returns for unit ids 128 to 133
/// during a battle.
extern u8 *gEnemyUnits;
#endif
#ifndef OWN_gRandomSeed
/// Random16's seed; Lucky Wheels seeds it with the play time, and a link
/// battle's parent sends it as the round's gBattleRandomSeed.
extern u32 gRandomSeed;
#endif
#ifndef OWN_gBattleBackgroundWork
/// The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the camera's turn, which
/// staging.c, auto_battle.c and round.c set for each action, and the
/// background's display mode.
extern struct BattleBackgroundWork *gBattleBackgroundWork;
#endif
/// SC_FACE_LINK every frame, forever: the script of an actor told to
/// keep facing its linked object.
extern const u8 gObjectMotion_LinkedActionScript[];
/// Where the party returns after a defeat: {map, entrance (-1 any),
/// return map, return entrance} rows, ended by map -1
/// (Scene_SetDefeatReturnPoint).
extern const s16 gDefeatReturnPoints[];
/// Isaac's weapon sprite by weapon kind (BattleUnit_LookupWeaponValueByClass).
extern u16 gBattleUnit_WeaponAnimationsIsaac[];
/// Battle backgrounds by map: a background word (bit 31 set, resource id below)
/// and then its maps, ended by 0; a map not listed takes the last background
/// (BattleBackground_SelectForMap).
extern s32 gBattleBackground_Maps[];
/// The template BattleAnim_BuildBlitter walks word by word to write a blitter
/// (battle_anim_composite.s); none of it runs where it is.
extern const u32 gBlitterTemplate[];
/// The message voice's rate by message speed option: 1, 3, then 10
/// (Message_SetVoiceFromActor). The same values as
/// gMessageSpeed_VoiceGaps, which the text printer reads, in a copy of its own.
extern u8 gMessageSpeed_VoiceRates[];
extern const u16 gWorldMap_CursorDirectionAngles[];
extern const u8 gWorldMap_MarkerBlendCycle[];
/// The world map's area markers, {tile << 16 | flag, area} for
/// each area's actor id from 11, ended by -1; a zero row reserves an
/// actor id with no marker.
extern const s32 gRegionMap_AreaMarkers[];
u32 __udivsi3(u32 numerator, u32 denominator);
#ifndef OWN__call_via_r0
void _call_via_r0(u32 routine);
#endif
void Anim_ApplyChildArgumentFar(void *, s32);
void BitDecoder_DecodeImage(void);
void FarCall_WindowTable(void);
struct Sprite *GetBattleEffectObject(s32 kind);
#ifndef OWN_Object_InitializeMode
#endif

/// BattleMotion_ApplyKnockback's knockbacks, the columns of the
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

/// The battle sprites' animations (Sprite_StartAnim numbers)
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

/// A battle unit's sprites' OBJ mode, as BattleHighlight_SetSpriteMode
/// sets it: faded units are semi-transparent, and the battle's blends act on
/// them.
enum BattleSpriteMode { SPRITE_OPAQUE, SPRITE_SEMI_TRANSPARENT };

/// BattleHighlight_SetModes' mode, kept in BattleWork.highlightMode:
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
