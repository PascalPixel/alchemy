#ifndef TLA_SHRINE_OF_THE_SEA_GOD_H
#define TLA_SHRINE_OF_THE_SEA_GOD_H

#include "types.h"
#include "main_routine_work.h"
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

struct ShrineOfTheSeaGodObject *Actor_GetById(s32 id);
void Event_Wait(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldMessage_Show(s32 message, s32 position);
void Actor_SetSpritePriority(s32 actor, s32 variant);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetPositionAndResetMotion(struct ShrineOfTheSeaGodObject *object,
                                     s32 x, s32 y, s32 z);
void Actor_SetModeById(u32 actorId, s32 animation);
void Event_Start(void);
void Event_End(void);
void Event_ResolvePendingActions(s32 skip);
void Func_02003aa0(s32 actor);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 width,
                                        s32 height, s32 destX, s32 destY);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Field_RequestDjinnBattle(s32 actor, s32 value);
void Func_02003b28(void);
void Func_02003b38(const void *script);
void Func_02003b40(void);
void Sound_Play(s32 cue);
void Func_02001200(s32 show);
void Func_0200142c(struct ShrineOfTheSeaGodObject *object);

static inline void ShrineOfTheSeaGod_CopyRect(s32 sx, s32 sy, s32 w, s32 h,
                                              s32 dx, s32 dy) {
  /* FAKEMATCH(wrapper): the rectangle copy reaches the main image through one
   * inline layer; as direct MapGrid_CopyCellPictureIndicesRect calls, 19 of 360
   * instructions differ in tla-en (Func_02000994), and 55 in all over the
   * seven callers that change. */
  MapGrid_CopyCellPictureIndicesRect(sx, sy, w, h, dx, dy);
}

static inline void ShrineOfTheSeaGod_BlendStart(s32 a, s32 b, s32 c, s32 d,
                                                s32 e, s32 f) {
  /* FAKEMATCH(wrapper): the blend start reaches the main image through one
   * inline layer; as direct Map_CopyCellAttributeRect calls, 12 of 153
   * instructions differ in tla-en (Func_0200338c), and 48 in all over the
   * ten callers that change. */
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);
void Event_SetMessageId(s32 message);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
s32 Actor_ShowEmoteById(s32 actorId, s32 emote, s32 frames);
void FieldEffect_SetAttached(s32 actor, s32 emote);
void Camera_SetSpeedParameters(s32 speed, s32 acceleration);
void FieldCamera_WaitForArrival(void);
struct ShrineOfTheSeaGodObject *FieldCamera_GetActor(void);
void Actor_SetDestinationById(u32 objectId, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Actor_Stop(s32 actor);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_SetSpriteFlags(struct ShrineOfTheSeaGodObject *object,
                               u32 value);

static inline void ShrineOfTheSeaGod_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH(pin): the height pinned to r1, so the actor loads after the
   * height and frames; as a plain local, 18 of 656 instructions differ in
   * tla-en (ShrineOfTheSeaGod_LashRope), and 24 in all over its four callers. */
  register s32 h __asm__("r1") = height;
  /* FAKEMATCH(pin): the frames pinned to r2 with the height above; as a plain
   * local, 8 of 656 instructions differ in tla-en (ShrineOfTheSeaGod_LashRope), and 12 in
   * all over three callers. */
  register s32 f __asm__("r2") = frames;

  Actor_ShowEmoteById(actor, h, f);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void ShrineOfTheSeaGod_ObjSpeed(s32 actor, s32 speed,
                                              s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void ShrineOfTheSeaGod_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_WalkToAndWaitById(actor, x, z);
}

static inline void ShrineOfTheSeaGod_ObjCommit(s32 actor, s32 x, s32 z) {
  Actor_MoveToAndWaitById(actor, x, z);
}

static inline void ShrineOfTheSeaGod_ObjTurn(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static inline void ShrineOfTheSeaGod_ObjPlace(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static inline void
ShrineOfTheSeaGod_ObjSetPos(struct ShrineOfTheSeaGodObject *object, s32 x,
                            s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static inline void ShrineOfTheSeaGod_CamSpeed(s32 speed, s32 acceleration) {
  Camera_SetSpeedParameters(speed, acceleration);
}

static inline void ShrineOfTheSeaGod_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void ShrineOfTheSeaGod_MessagePut(s32 speaker, s32 flags) {
  Event_ShowMessageAndWait(speaker, flags);
}

static inline void ShrineOfTheSeaGod_ObjEmote(s32 actor, s32 emote) {
  FieldEffect_SetAttached(actor, emote);
}

void Task_Add(void (*func)(void), s32 priority);

static inline void ShrineOfTheSeaGod_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

#define SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE(func, priority)                    \
  do {                                                                         \
    s32 priority_;                                                             \
    priority_ = (priority);                                                    \
    Task_Add((func), priority_);                                               \
  } while (0)

static inline s32 ShrineOfTheSeaGod_FlagCheck(s32 flag) {
  return GameFlag_Test(flag);
}

static inline void ShrineOfTheSeaGod_ObjPlaceAt(s32 actor, s32 x, s32 z,
                                                s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

static inline void ShrineOfTheSeaGod_ObjPlaceTurn(s32 actor, s32 x, s32 z,
                                                  s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

#endif
