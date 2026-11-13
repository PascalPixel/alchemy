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

s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);

extern const s32 gData_080f32e0[];

void BattleEffect_StartEffectObject22(s32 item, s32 flags);
void Func_080cf554(void);

void Func_080daecc(s32 objectId, s32 grow);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

s32 Func_080dbe80(s32 *a, s32 radius_a, s32 *b, s32 radius_b);

s32 BattleEffect_GetObjectPriority(struct Actor *object);

s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);

s32 Func_080d9a74(s32 id, s32 *value, s32 *index);

void Func_080d92d4(s32 index, s32 fromId, s32 toId);

void Func_080db0b0(s32 id);

s32 Func_080dbda8(s32 x, s32 z, s32 layer);

void Func_080d7524(s32 djinniId);

extern const u8 gData_0802eac4[16];
extern const u8 gData_0802ead4[8];
extern const u8 gData_0802eadc[8];
extern const u8 gData_0802eae4[16];
extern const u8 gData_0802eb04[64];
extern const u8 gData_0802eb44[8];

extern const struct ActorPlacement gData_080f21d4[];

void BattleEffect_InitializeBuffers(void);

s32 ObjectEffect_RunPendingFlagEvent(void);
void ObjectEffect_StartContextEffect25(void);
void ObjectEffect_StartContextEffect26(void);
void Func_080d5fd4(void);

s32 Func_080d4d08(void);
s32 Func_080d50f8(void);
s32 Func_080d53b8(void);

void Marker_MoveLeaderAwayFromEmitter(struct ParticleEmitter *emitter);

extern const u32 gData_0802ed34[4][9];

void Actor_SetSpritePriorityDirectFar(struct Actor *object, s32 priority);

s32 Graphics_ClampRgb555Channel(s32 value);
s32 Graphics_ClampRgb555Component(s32 value);
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

s32 Func_080d9194(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080d8fa8(s32 a0, s32 a1, s32 a2, s32 a3);
void Func_080d8d68(s32 index, s32 a1, s32 a2);

void Func_080d9104(s32 index, s32 a1, s32 a2);

void Func_080d9b80(void);
void Func_080d9d40(void);

s32 Func_080d99b8(s32 first, s32 second);

void Func_080da060(s32 index, s32 first, s32 second, s32 wobble);
s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);
s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);
s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave);
void Func_080daab0(void);

void Func_080dacb8(void);
struct WorkA4Slot *Func_080dae70(s32 id);

void BattleEffect_DrawSceneSprite(void);

void Func_080d64f0(void);

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);
void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect);

void BattleEffect_InitializeSlots(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);

void BattleEffect_SetObjectAlternatingWords(u8 *object);
void BattleEffect_SpinUpAngle(void *obj);
void BattleEffect_ShrinkObjectAndDestroySlow(void *obj);
void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect);

void BattleEffect_AdvanceSpinAngle();

void Func_080d8a00(void);
void Func_080d8c10(void);
s32 Func_080d8e08(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

s32 Func_080d8ff8(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Message_SetVoiceFromActor(s32 key);
void Event_ShowCounterAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);

void Func_080d6c60(void);

void Func_080d6674(void);

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames);

extern const s32 gData_0802eb90[2];
extern const s32 gData_0802eb98[12];
extern const s32 gData_0802ec7c[];
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ec94[];
extern const s32 gData_0802ed28[];
extern const s32 gData_0802ece0[];
extern const s32 gData_0802ec88[];

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);

s32 ObjectTable_ReadActiveValue(s32 key);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

s32 ResourceTable_CountFreeBlocks(void);
void Func_080dbb40(void);

#endif
