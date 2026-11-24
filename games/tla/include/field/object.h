#ifndef ALCHEMY_FIELD_OBJECT_H
#define ALCHEMY_FIELD_OBJECT_H

/// @file
/// The field's cells, actors, ropes, tightropes and hanging vines, field
/// effects, weather and palette fades, as the scenes and the event commands
/// call them.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/scene_tables.h"

struct Actor;
struct FieldParticle;
struct ParticleEmitter;
struct HangingVineSlot;

/// The map's cells: their attributes and what sand, water and Cyclone make of
/// them.
s32 MapGrid_GetCellAttributesAt(s32 x, s32 z, s32 layer);
u8 MapGrid_SetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value);
s32 MapGrid_IsSandCell(s32 x, s32 z, s32 layer);
s32 MapGrid_IsWaterCell(s32 x, s32 z, s32 layer);
s32 MapGrid_GetCycloneCellKind(s32 x, s32 z, s32 layer);

/// What Cyclone does at a cell (MapGrid_GetCycloneCellKind).
enum CycloneCellKind {
  CYCLONE_CELL_NONE = 0,
  CYCLONE_CELL_BURST = 1, ///< blown away, with the particle burst
  CYCLONE_CELL_CARRY = 2, ///< the whirlwind carries the leader to it
  CYCLONE_CELL_LIFT = 3   ///< the whirlwind lifts the leader to it
};

/// The actors' sprites: their depth and priority, a silhouette, a sprite id
/// and graphics copied from another actor.
void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *actor,
                                               s32 magnitude, s32 angle);
s32 Actor_GetSpriteDepth(struct Actor *actor);
s32 Actor_GetSpritePriority(struct Actor *actor);
void Actor_SetSpritePriorityDirectFar(struct Actor *actor, s32 priority);
struct Actor *Actor_CreateSilhouette(s32 x, s32 y, s32 z, s32 type);
s32 Actor_GetSpriteById(s32 key);
void Actor_CopyGraphics(u32 targetId, u32 sourceId);

/// A facing table entry: the picture to draw, counted from the sprite
/// animation's base picture (bits 0-2), mirrored when bit 7 is set.
#define FACING_MIRRORED_BIT 0x80
#define MIRRORED(picture) (FACING_MIRRORED_BIT | (picture))

/// The facing tables: the picture each heading draws, by how many pictures a
/// sprite animation has for its facings.
extern const u8 gAnimationFacing_OnePicture[16];
extern const u8 gAnimationFacing_FrontBackSwapped[8];
extern const u8 gAnimationFacing_FrontBack[8];
extern const u8 gAnimationFacing_ThreePictures[16];
extern const u8 gAnimationFacing_FivePictures[64];
extern const u8 gAnimationFacing_EightPictures[8];
extern const s32 gActorSprite_FullScale[2];

/// The actors' own scripts: launched, leaving footprints, ripples, sand
/// grains, a wake and a splash.
extern const s32 gActor_LaunchScript[];
extern const s32 gActor_FootprintScript[];
extern const s32 gActor_RippleScript[];
extern const s32 gSand_GrainScript[];
extern const s32 gActor_WakeScript[];
extern const s32 gActor_SplashScript[];

extern const struct ActorPlacement gField_DefaultPlacements[];

/// The arrival Entrance_PlayArrival played, for the way the last scene was
/// left.
enum EntranceArrival {
  ARRIVAL_NONE = 0,        ///< none: the scene plays its own
  ARRIVAL_STAIRS_UP = 1,   ///< up stairs, ANIM_LEADER_ARRIVE
  ARRIVAL_STAIRS_DOWN = 2, ///< down stairs, ANIM_LEADER_ARRIVE_ALTERNATE
  ARRIVAL_FALLING = 3      ///< falling in from above
};

/// Arriving in a scene, leaving by stairs, and the leader's jumps and climbs.
s32 Entrance_PlayArrival(void);
void Exit_StartStairsDown(void);
void Exit_StartStairsUp(void);
void Exit_AddHiddenLeaderLayer(void);
s32 Field_TryJumpForward(void);
s32 Field_ClimbDown(void);
s32 Field_ClimbUp(void);

/// The ropes Lash ties between two posts, given by their actor ids: placing,
/// tying, coiling and swinging them, laying their nodes and drawing them.
void Rope_Initialize(void);
void Rope_Place(s32 index, s32 flag, s32 firstId, s32 secondId);
s32 Rope_GetState(s32 index);
s32 Rope_FindById(s32 id, s32 *value, s32 *index);
void Rope_Lash(s32 index, s32 fromId, s32 toId);
void Rope_Tie(s32 index, s32 firstId, s32 secondId);
void Rope_Coil(s32 index, s32 firstId, s32 secondId);
void Rope_SwingBetween(s32 index, s32 fromId, s32 toId, FIXED height);
s32 Rope_IsLeaderAtEnd(s32 firstId, s32 secondId);
s32 Rope_LayTied(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);
s32 Rope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);
s32 Rope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  FIXED height);
void Rope_Draw(void);
void Rope_Update(void);

/// The tightropes the party walks across, laid between two points or two
/// posts as the ropes are.
void Tightrope_WobbleBetween(s32 index, s32 fromId, s32 toId, FIXED height);
s32 Tightrope_Lay(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);
s32 Tightrope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                       s32 toZ, FIXED height);
s32 Tightrope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                      s32 toZ);
s32 Tightrope_Recoil(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                     s32 wave);
void Tightrope_Draw(void);
void Tightrope_Update(void);

/// The hanging vines, each hung from an actor.
void HangingVine_Hang(s32 actorId, s32 grow);
void HangingVine_Place(s32 id, s32 force);
void HangingVine_Swing(s32 id);
struct HangingVineSlot *HangingVine_FindSlot(s32 id);
void HangingVine_Draw(void);
void HangingVine_Update(void);

/// The field effects the Psynergy and the found items show.
s32 FieldPsynergy_CheckOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void FieldPsynergy_GrowFromCaster(struct Actor *actor, s32 x, s32 y, s32 z);
void FieldPsynergy_DrawCanvasSprite(void);
void FieldPsynergy_ReleaseSceneSprite(void);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
void FieldEffect_SpawnRevealMarkers(void);
void FieldEffect_InitializeSlots(void);
void FieldEffect_ReleaseSlots(void);
void FieldEffect_ShrinkActorAndDestroyFast(void *obj);
void FieldEffect_ShrinkActorAndDestroySlow(void *obj);
void FieldEffect_UpdateRadialSpread(struct FieldParticle *effect);
void FieldEffect_PulseActorScale(struct Actor *actor);
void FieldEffect_AdvanceAngleWithRamp(struct Actor *actor);
void FieldEffect_UpdateMarsSpark(struct FieldParticle *effect);
void ItemDisplay_ShowItem(s32 item, s32 flags);
void ItemDisplay_EmitSparkles(struct Actor *emitter);

/// The weather and the ground's particles, each frame.
void GroundParticles_Update(void);
void FieldMotes_Update(void);
void Weather_UpdateRain(void);

/// A Djinni's hop as its battle starts, and its spin.
void Djinn_RunBattleStartHop(s32 djinniId);
void Djinn_AdvanceSpinAngle();

/// The palette fades: their steps, their channels kept in range, and the
/// backdrop colour.
void PaletteFade_Initialize(void);
void PaletteFade_CalculateSteps(s16 *source, s16 *target, s16 *step,
                                s32 frames);
s32 PaletteFade_ClampComponent(s32 value);
s32 PaletteFade_ClampShiftedComponent(s32 value);
void PaletteFade_SetBackdrop(u32 color);

/// The World Map's graphics, its fixed camera and the yaw its camera leans to
/// for each state of the direction pad.
extern const u32 gWorldMap_GraphicsResources[4][9];
extern const FIXED gCamera_FixedViewMatrix[12];
extern const s32 gWorldMap_KeyYaws[16];

/// gWorldMap_KeyHeadings' entry when no direction is held, or only an
/// opposite pair: no heading, since every real one is a multiple of an eighth
/// turn.
#define WORLD_MAP_KEY_HEADING_NONE 65535

/// A message's voice from its speaker, and a message shown at a place.
void Message_SetVoiceFromActor(s32 key);
void Event_ShowMessageAtPosition(s32 unusedA, s32 unusedB, s32 x, s32 y);

/// Counts the free blocks of the sprite VRAM cache.
s32 ResourceTable_CountFreeBlocks(void);

#endif
