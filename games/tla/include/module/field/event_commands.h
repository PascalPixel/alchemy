#ifndef ALCHEMY_MODULE_FIELD_EVENT_COMMANDS_H
#define ALCHEMY_MODULE_FIELD_EVENT_COMMANDS_H

/// @file
/// What games/tla/src/field/ declares for the files that use it, written once.

#include "types.h"
#include "effect_sprite.h"

struct Actor;
struct AnimationEntry;
struct Effect04Vector;
struct EffectVector;
struct FieldParticle;

struct ObjectPairPosition {
  u8 unk00[6];
  s16 angle;
  s32 x;
  s32 y;
  s32 z;
};

struct ObjectPart {
  u8 unk00[5];
  u8 attribute;
  u8 unk06[0xa];
  void *graphics;
};

struct ObjectPartList {
  u8 unk00[0x19];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0xc];
  struct ObjectPart *parts[1];
};

struct EventWaitWork {
  u8 unk000[0x1b8];
  s32 waitMode;
};

struct EventWordWork {
  u8 unk00[0x10];
  s32 word10;
};

struct OwnerVitals {
  u8 unk000[20];
  s16 hpRatio;
  s16 ppRatio;
  u8 unk018[28];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u8 unk03c[0x131 - 60];
  u8 status;
};

struct ItemHead {
  u8 unk00[3];
  u8 flags;
};

struct WaveFxWork {
  u16 lines[2][162];
  u16 phase;
  u8 page;
  s8 red;
  s8 green;
  s8 blue;
  u16 hue;
  u16 sourceId;
  u16 targetId;
  u8 delay;
  u8 counter;
};

struct ScatterPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ParticleTimer {
  u8 unk00[0x5e];
  u16 timer;
};

// ⚓️'s composite sprite (as worka0/f0daecc.c's CompositeAnimation).
struct CloakSprite {
  u8 unk00[0x19];
  u8 dirty; // redraw the parts
  u8 blend; // 2 draws the caster half transparent, 1 shows it plainly
  u8 unk1b[0x0d];
  struct AnimationEntry *entries[4];
};

struct CloakCaster {
  u8 unk00[0x50];
  struct CloakSprite *sprite;
  u8 unk54[0x18];
  void (*hook)(void);
};

struct TouchCell {
  s16 x;
  s16 z;
};

struct TouchLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[40];
};

struct TouchMap {
  u8 unk000[264];
  struct TouchLayer layers[1];
};

struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unk06[2];
};

struct TouchCopies {
  struct TouchCell unk0000[1491];
  struct TouchCopy copies[64];
  s8 count;
};

struct BurstParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 heading;
  s32 speed;
  s32 spin;
  s32 delay;
};

struct BurstWork {
  u8 tiles[384];
  struct EffectSprite sprites[96];
  struct BurstParticle particles[96];
  s16 resource;
  s16 vram;
  u8 unk1b04[2];
  s16 frame;
  s16 active;
};

struct BurstCell {
  s16 x;
  s16 z;
};

struct CueObject {
  u8 unk00[8];
  s32 x;
  s32 base;
  s32 top;
};

struct ScaledSprite {
  u8 unk00[0xc];
  s32 scale;
};

struct SparklePicture {
  s16 dy;
  u16 tile;
};

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern const s32 Data_080f085c[];
extern const s32 Data_080f0874[];

s32 PartyInventory_FindRoomFar(s32 item);

s32 Func_08108068(s32 *member, s32 *slot);

void BattleEffect_AdvanceHueCycle(void);
void BattleEffect_SetCallbackWhenTargetUnset(struct Actor *object);
void BattleEffect_ApplyFlickerValue(void *object, s32 value);

void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z);

s32 ScriptObject_CheckOverlapFar(struct Actor *object,
                                 struct Effect04Vector *position);

void Func_080dd754(void);

void Func_080ddbd8(void);

// Far call to the target classifier Func_080237c8.
s32 Func_08020330(struct Actor *object);

extern const s32 gData_080f0e60[];

struct Actor *BattleEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);

void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot);

void FieldEffect_RunPly(void);
void Func_080debd8(void);

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

struct Actor *BattleEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);

void Func_080201f0(s32 toX, s32 toZ, s32 width, s32 height, s32 fromX,
                   s32 fromZ);

void Func_080e2c38(void);

extern char ResourceId_Leaves;

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);

void Func_080e3d04(void);

void Func_080e4a34(void);

void Func_080e572c(void);

void Func_080e5d5c(void);

void Func_080e68d0(void);
void Func_080e7844(void);

void Func_080e9af4(void);

void Func_080e4730(void);

extern char ResourceId_SparkleFade;

s32 Func_080eab70(struct Actor *object);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080ec1d0(s32 *screenX, s32 *screenY);

extern s16 gData_0202a642;

void Func_080e09c0(void);

void BattleEffect_ShrinkObjectScaleUntilHalf(struct Actor *object);

void Camera_WorldToScreen(struct EffectVector *position);

void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height);

void Func_080e1f2c(void);

void FieldPsynergy_Halt(void);

void FieldPsynergy_Catch(void);
void Func_080e0cec(struct FieldParticle *slot);

void FieldEffect_RunWhirlwind(void);

void BattleEffect_StartArcEffect(void);

void BattleEffect_WaitForArcGroups(void);

void BattleEffect_EndArcEffect(void);

void BattleEffect_CleanupSceneObjects(void);
void Func_080dc6d8(void);

void Djinn_ResolvePending(s32 shown);

void Func_080e2718(void);

void Djinn_RestoreActorAfterBattle(void);

void FieldEffect_ResumeCloakCheck(void);

void Func_080e7804(void);
void Func_080e7238(void);

void Field_StartEncounterTransition(s32 unused);

void Func_080ea14c(s32 targetId);

void BattleEffect_ResumeObject(s32 id);

void Actor_JumpDownLedge(s32 id, s32 mode, s32 particle);

void *Sprite_ShareVramCacheEntry(void *sprite, void *last);

void Djinn_RunVenusJoinEffect(s32 actor);

void Djinn_RunMercuryJoinEffect(s32 actor);

void Djinn_RunMarsJoinEffect(s32 actor);

void Djinn_RunJupiterJoinEffect(s32 actor);

void FieldEffect_RunMove(void);

void Func_080df6f4(void);

void Func_080de21c(void);

void FieldEffect_RunGrowth(void);

void FieldEffect_RunRetreat(void);

void BattleEffect_RunEffect16(void);

void Func_080decb8(void);

void Func_080dd820(void);

void Func_080e9090(void);

void BattleEffect_RunViewSway(void);

void BattleEffect_RunEffect30(void);

void Func_080e82cc(void);

s32 BattleEffect_FilterObjectIdByFlags(s32 id);
void BattleEffect_PauseObject(s32 id);

void Func_080dd950(void);

void BattleEffect_SteerLiftedTarget(s32 targetId);

void Func_080dda30(s32 unused);

void Func_080e0978(s32 value);

void FieldEvent_ShowStatusMessage(void);

void Func_080e9db4(void);

void Func_080e87c4(void);

void BattleEffect_FinishSceneAndReleaseHeapBlock(void);

void Scene_QueueMusic(s32 soundId);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);

s32 Actor_GetScreenPosition(s32 objectId, s32 *position);

void Weather_UpdateLightning(void);

void Field_RequestBattleFromEncounterTable(s32 row, s32 column);

void Party_LearnAbility(s32 unit, s32 ability);
void Party_RemoveMemberRestored(s32 owner);

#endif
