#ifndef TLA_YALLAM_INTERIORS_H
#define TLA_YALLAM_INTERIORS_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct YallamInteriorsObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct YallamInteriorsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

extern s32 gFrameCount;

u32 Random16Far(void);
void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct YallamInteriorsObject *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct YallamInteriorsObject *object,
                                      s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct YallamInteriorsObject *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_020009b0(void);
void Func_020009b8(void);
void Func_02000a18(s32 message);
void Func_02000a28(s32 speaker, s32 flags);
void Func_02000a50(s32 skip);
void Func_02000a68(s32 shop, s32 keeper);
void Func_02000a70(s32 keeper);
void Func_02000a78(s32 shop, s32 keeper);
void Func_020009f8(s32 actor, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct YallamInteriorsObject *object,
                                        u32 value);
void Animation_SetStateFlags(struct YallamInteriorsObject *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000a48(s32 a);
void Event_StartScreenTransition(void);

static inline s32 YallamInteriors_FlagCheck(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return GameFlag_Test(flag);
}

static inline void YallamInteriors_FlagSet(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  GameFlag_SetBit(flag);
}

static inline void YallamInteriors_ObjArm(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void
YallamInteriors_ObjSetPos(struct YallamInteriorsObject *object, s32 x, s32 y,
                          s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_SetPositionAndResetMotion(object, x, y, z);
}

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void YallamInteriors_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_CamBounds(x, y, z, mode);
}

static inline void YallamInteriors_ObjSpeed(s32 actor, s32 speed,
                                            s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void YallamInteriors_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void YallamInteriors_Wait(s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Battle_WaitMode0(frames);
}

static inline void YallamInteriors_WaitFrames(s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  WaitFrames(frames);
}

#define YALLAM_INTERIORS_TASK_ADD(callback, priority)                          \
  {                                                                            \
    /* FAKEMATCH: a temporary keeps the priority constant ahead of the         \
     * callback load. */                                                       \
    s32 priority_ = (priority);                                                \
    Scheduler_AddOrUpdateCallback((callback), priority_);                      \
  }

static inline void YallamInteriors_TaskAdd(void (*callback)(void),
                                           s32 priority) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void YallamInteriors_MessagePut(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02000a28(speaker, flags);
}

static inline void YallamInteriors_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  UiText_OpenMessageAtObject(speaker, flags);
}

s32 Func_02000a58(void);
void ObjectMotion_SetVariantCallback(s32 actor, s32 value);

// ---- this overlay

extern u8 gData_02000c8c[];
extern u8 gData_02000ca4[];
extern u8 gData_02000dac[];
extern u8 gData_02000e84[];
extern u8 gData_02000ea8[];
extern u8 gData_02000fe0[];

#endif
