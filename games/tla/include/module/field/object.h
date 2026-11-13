#ifndef ALCHEMY_MODULE_FIELD_OBJECT_H
#define ALCHEMY_MODULE_FIELD_OBJECT_H

/// @file
/// What games/tla/src/field/object/ declares for the files that use it, written
/// once.

#include "types.h"
#include "scene_tables.h"

struct Actor;
struct FieldParticle;
struct ParticleEmitter;
struct WorkA4Slot;

s32 MapGrid_GetAttributeBAt(s32 x, s32 z, s32 layer);
u8 MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value);

extern const s32 gActor_LaunchScript[];

void ItemDisplay_ShowItem(s32 item, s32 flags);
void FieldEffect_SpawnRevealMarkers(void);

void HangingVine_Hang(s32 objectId, s32 grow);
void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                               s32 magnitude, s32 angle);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

s32 FieldPsynergy_CheckOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

s32 BattleEffect_GetObjectPriority(struct Actor *object);

s32 MapGrid_IsSandCell(s32 x, s32 z, s32 layer);
void FieldPsynergy_GrowFromCaster(struct Actor *object, s32 x, s32 y, s32 z);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 MapGrid_IsWaterCell(s32 x, s32 z, s32 layer);

s32 Rope_FindById(s32 id, s32 *value, s32 *index);

void Rope_Lash(s32 index, s32 fromId, s32 toId);

void HangingVine_Swing(s32 id);

s32 MapGrid_GetCycloneCellKind(s32 x, s32 z, s32 layer);

void Djinn_RunBattleStartHop(s32 djinniId);

extern const u8 gAnimationFacing_OnePicture[16];
extern const u8 gAnimationFacing_FrontBackSwapped[8];
extern const u8 gAnimationFacing_FrontBack[8];
extern const u8 gAnimationFacing_ThreePictures[16];
extern const u8 gAnimationFacing_SixPictures[64];
extern const u8 gData_0802eb44[8];

extern const struct ActorPlacement gData_080f21d4[];

void BattleEffect_InitializeBuffers(void);

s32 Entrance_PlayArrival(void);
void Exit_StartStairsDown(void);
void Exit_StartStairsUp(void);
void Func_080d5fd4(void);

s32 Field_TryJumpForward(void);
s32 Field_ClimbDown(void);
s32 Field_ClimbUp(void);

void ItemDisplay_EmitSparkles(struct ParticleEmitter *emitter);

extern const u32 gWorldMap_GraphicsResources[4][9];

void Actor_SetSpritePriorityDirectFar(struct Actor *object, s32 priority);

s32 PaletteFade_ClampComponent(s32 value);
s32 PaletteFade_ClampShiftedComponent(s32 value);
void BattleEffect_SetPrimaryBufferValue(u32 value);

extern char ResourceId_DefaultMapAnimation;
extern char ResourceId_WorldMapTilesA1;
extern char ResourceId_WorldMapTilesA2;
extern char ResourceId_WorldMapTilesA3;
extern char ResourceId_WorldMapTilesA4;
extern char ResourceId_WorldMapTilesA5;
extern char ResourceId_WorldMapTilesA6;
extern char ResourceId_WorldMapPaletteC;
extern char ResourceId_Data1AD;
extern char ResourceId_Data1AE;
extern char ResourceId_WorldMapTilesC1;
extern char ResourceId_WorldMapTilesC2;
extern char ResourceId_WorldMapTilesC3;
extern char ResourceId_WorldMapTilesC4;
extern char ResourceId_WorldMapTilesC5;
extern char ResourceId_WorldMapTilesC6;
extern char ResourceId_Data1A4;
extern char ResourceId_Data1A5;
extern char ResourceId_WorldMapTilesB1;
extern char ResourceId_WorldMapTilesB2;
extern char ResourceId_WorldMapTilesB3;
extern char ResourceId_WorldMapTilesB4;
extern char ResourceId_WorldMapTilesB5;
extern char ResourceId_WorldMapTilesB6;
extern char ResourceId_WorldMapPaletteD;
extern char ResourceId_Data1B6;
extern char ResourceId_Data1B7;
extern char ResourceId_WorldMapTilesD1;
extern char ResourceId_WorldMapTilesD2;
extern char ResourceId_WorldMapTilesD3;
extern char ResourceId_WorldMapTilesD4;
extern char ResourceId_WorldMapTilesD5;
extern char ResourceId_WorldMapTilesD6;

s32 Rope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Rope_SwingBetween(s32 a0, s32 a1, s32 a2, s32 a3);
void Rope_Tie(s32 index, s32 a1, s32 a2);

void Rope_Coil(s32 index, s32 a1, s32 a2);

void Tightrope_Draw(void);
void Tightrope_Update(void);

s32 Rope_IsLeaderAtEnd(s32 first, s32 second);

void Tightrope_WobbleBetween(s32 index, s32 first, s32 second, s32 wobble);
s32 Tightrope_Lay(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);
s32 Tightrope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y,
                       s32 to_z, s32 mode);
s32 Tightrope_LayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
                      s32 toZ);

s32 Tightrope_Recoil(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                     s32 wave);
void HangingVine_Draw(void);

void HangingVine_Update(void);
struct WorkA4Slot *HangingVine_FindSlot(s32 id);

void BattleEffect_DrawSceneSprite(void);

void GroundParticles_Update(void);

void FieldEffect_ShrinkActorAndDestroyFast(void *obj);
void FieldEffect_UpdateRadialSpread(struct FieldParticle *effect);

void FieldEffect_InitializeSlots(void);
void FieldEffect_ReleaseSlots(void);

void BattleEffect_SetObjectAlternatingWords(u8 *object);
void FieldEffect_AdvanceAngleWithRamp(void *obj);
void FieldEffect_ShrinkActorAndDestroySlow(void *obj);
void FieldEffect_UpdateMarsSpark(struct FieldParticle *effect);

void BattleEffect_AdvanceSpinAngle();

void Rope_Draw(void);
void Rope_Update(void);
s32 Rope_LayTied(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

s32 Rope_LayBowed(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Message_SetVoiceFromActor(s32 key);
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);

void FieldMotes_Update(void);

void Weather_UpdateRain(void);

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames);

extern const s32 gData_0802eb90[2];
extern const s32 gCamera_FixedViewMatrix[12];
extern const s32 gData_0802ec7c[];
extern const s32 gWorldMap_KeyYaws[16];
extern const s32 gData_0802ec94[];
extern const s32 gData_0802ed28[];
extern const s32 gData_0802ece0[];
extern const s32 gData_0802ec88[];

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);

s32 Actor_GetSpriteById(s32 key);
void Actor_CopyGraphics(u32 targetId, u32 sourceId);

s32 ResourceTable_CountFreeBlocks(void);
void FieldPsynergy_ReleaseSceneSprite(void);

#endif
