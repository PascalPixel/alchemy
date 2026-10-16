#ifndef TLA_SHRINE_OF_THE_SEA_GOD_H
#define TLA_SHRINE_OF_THE_SEA_GOD_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ShrineOfTheSeaGodObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 field18;
  s32 field1c;
  u8 unk20[2];
  u8 layer;
  u8 priorityFlags;
  u8 unk24[4];
  s32 speed;
  u8 unk2c[0x24];
  void *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[8];
  u8 state;
  u8 unk63[9];
  void (*update)(struct ShrineOfTheSeaGodObject *object);
};

struct ShrineOfTheSeaGodSprite {
  u8 unk00[0x12];
  u16 rotation;
};

struct ShrineOfTheSeaGodEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  u16 unk15a;
  u8 unk15c[0x50];
  s32 screenEffect;
};

struct ShrineOfTheSeaGodMapLayer {
  u8 unk00[8];
  s32 x;
  s32 y;
  u8 unk10[0x28];
};

struct ShrineOfTheSeaGodMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
  u8 unk018[0xf0];
  struct ShrineOfTheSeaGodMapLayer layers[3];
};

struct ShrineOfTheSeaGodObject *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Object_SetPositionAndResetMotion(struct ShrineOfTheSeaGodObject *object,
                                      s32 x, s32 y, s32 z);
void Object_SetModeById(s32 actor, s32 mode);
void Func_020039d8(void);
void Func_020039e0(void);
void Func_02003b08(s32 skip);
void Func_02003aa0(s32 actor);
void Func_02003998(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020039a8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003ae0(s32 actor, s32 value);
void Func_02003b28(void);
void Func_02003b38(const void *script);
void Func_02003b40(void);
void Func_02003b48(s32 cue);
void Func_02001200(s32 show);
void Func_0200142c(struct ShrineOfTheSeaGodObject *object);

static inline void ShrineOfTheSeaGod_CopyRect(s32 sx, s32 sy, s32 w, s32 h,
                                              s32 dx, s32 dy) {
  /* FAKEMATCH: the rectangle copy reaches the engine through one inline layer.
   */
  Func_02003998(sx, sy, w, h, dx, dy);
}

static inline void ShrineOfTheSeaGod_BlendStart(s32 a, s32 b, s32 c, s32 d,
                                                s32 e, s32 f) {
  /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
  Func_020039a8(a, b, c, d, e, f);
}

void Func_02003a30(s32 actor, s32 x, s32 z);
void Func_02003a38(s32 actor, s32 x, s32 z, s32 angle);
void Func_02003a70(s32 message);
void Func_02003a78(s32 actor, s32 a, s32 b);
void Func_02003a80(s32 speaker, s32 flags);
void Func_02003aa8(s32 actor, s32 height, s32 frames);
void Func_02003ab0(s32 actor, s32 emote);
void Func_02003ab8(s32 speed, s32 acceleration);
void Func_02003ac8(void);
struct ShrineOfTheSeaGodObject *Func_02003ad0(void);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ShrineOfTheSeaGodObject *object,
                                        u32 value);

static inline void ShrineOfTheSeaGod_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH: the native loads the actor after the height and frames. */
  register s32 h __asm__("r1") = height;
  /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
  register s32 f __asm__("r2") = frames;

  Func_02003aa8(actor, h, f);
}

static inline void ShrineOfTheSeaGod_ObjSpeed(s32 actor, s32 speed,
                                              s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void ShrineOfTheSeaGod_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void ShrineOfTheSeaGod_ObjCommit(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static inline void ShrineOfTheSeaGod_ObjTurn(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void ShrineOfTheSeaGod_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02003a30(actor, x, z);
}

static inline void
ShrineOfTheSeaGod_ObjSetPos(struct ShrineOfTheSeaGodObject *object, s32 x,
                            s32 y, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_SetPositionAndResetMotion(object, x, y, z);
}

static inline void ShrineOfTheSeaGod_CamSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02003ab8(speed, acceleration);
}

static inline void ShrineOfTheSeaGod_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void ShrineOfTheSeaGod_ObjLaunch(s32 actor, s32 speed,
                                               s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_Launch(actor, speed, frames);
}

static inline void ShrineOfTheSeaGod_MessagePut(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02003a80(speaker, flags);
}

static inline void ShrineOfTheSeaGod_ObjModeWait(s32 actor, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_SetModeAndWaitAnimation(actor, mode);
}

static inline void ShrineOfTheSeaGod_Wait(s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  FieldEvent_WaitFrames(frames);
}

static inline void ShrineOfTheSeaGod_ObjVar(s32 actor, s32 value) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_SetVarCbAndRefresh(actor, value);
}

static inline void ShrineOfTheSeaGod_ObjMode(s32 actor, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_SetModeById(actor, mode);
}

static inline void ShrineOfTheSeaGod_ObjEmote(s32 actor, s32 emote) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02003ab0(actor, emote);
}

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

static inline void ShrineOfTheSeaGod_TaskAdd(void (*callback)(void),
                                             s32 priority) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  s32 p = priority;

  Scheduler_AddOrUpdateCallback(callback, p);
}

#define SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE(callback, priority)                \
  do {                                                                         \
    s32 priority_;                                                             \
    priority_ = (priority);                                                    \
    /* FAKEMATCH: a barrier keeps the callback load after the priority. */     \
    __asm__ volatile("" : : "r"(priority_));                                   \
    Scheduler_AddOrUpdateCallback((callback), priority_);                      \
  } while (0)

static inline s32 ShrineOfTheSeaGod_FlagCheck(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return GameFlag_Test(flag);
}

static inline void ShrineOfTheSeaGod_FlagSet(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  GameFlag_SetBit(flag);
}

static inline void ShrineOfTheSeaGod_ObjPlaceAt(s32 actor, s32 x, s32 z,
                                                s32 angle) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02003a38(actor, x, z, angle);
}

static inline void ShrineOfTheSeaGod_ObjPlaceTurn(s32 actor, s32 x, s32 z,
                                                  s32 angle) {
  s32 g = angle;

  Func_02003a38(actor, x, z, g);
}

#endif
