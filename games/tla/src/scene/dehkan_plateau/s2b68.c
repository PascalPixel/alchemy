#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003d78(s32 skip);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003df8(s32 cue);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003de8(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_020015b8(struct ObjectRuntime *object);
void Func_0200172c(void);
void Func_02001804(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_ClearBit(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Wait(s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  WaitFrames(frames);
}

void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc8(s32 id, s32 a, s32 b);
void Func_02003cf0(void);
struct ObjectRuntime *Func_02003cf8(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Func_02003cb0(s32 id, s32 value);
void Func_02003cc0(s32 id);
void Func_02003cd0(s32 id, s32 value);
void Func_02003d40(struct ObjectRuntime *object, s32 mode);
void Func_02003bb8(struct ObjectRuntime *object);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void PartyInventory_GiveItem(s32 item, s32 mode);
void Func_02002b68(void);
void Func_02002f08(void);

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void ObjectMotion_ArmCallback(s32 id, s32 a, s32 b);
void Object_LinkObjectAndSetCallback(s32 id, s32 target);
void FieldEvent_WaitFrames(s32 frames);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);

static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_FaceToward(s32 id, s32 target, s32 mode) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetAngleToward(id, target, mode);
}

static __inline__ void Actor_SetTurn(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003cc8(id, a, b);
}

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_MoveTo(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Launch(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_Launch(id, a, b);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003c70(id, x, z, angle);
}

static __inline__ void Actor_PlaceOnTerrain(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003c68(id, x, z);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003be0(a, b, c, d, e, f);
}

static __inline__ void Cue(s32 cue) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003df8(cue);
}

static __inline__ void Pause(s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  FieldEvent_WaitFrames(frames);
}

static __inline__ void Actor_SetVariant(s32 id, s32 variant) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ struct ObjectRuntime *Actor_Get(s32 id) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return Object_GetById(id);
}

void Func_02002b68(void) {
  if (((struct EffectWork7c4 *)Ram_HeapSlots->effectWork)->active != 0) {
    Cue(229);
    Map_CopyCells(64, 64, 15, 8, 1, 1);
    Map_CopyCells(64, 66, 15, 72, 1, 2);
    ObjectDispatch_SetSingleChildFlags(Actor_Get(gPartyState.currentOwner), 0);
  }
}
