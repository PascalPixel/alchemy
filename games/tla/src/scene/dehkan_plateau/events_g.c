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
void Func_020021b4(void);
void Func_02002604(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Wait(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  WaitFrames(frames);
}

void Func_02003c70(u32 id, s32 x, s32 z, s16 angle);
struct ObjectRuntime *Func_02003cc8(s32 id, s32 a, s32 b);
void Func_02003cf0(void);
struct ObjectRuntime *Func_02003cf8(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
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
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_FaceToward(s32 id, s32 target, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetAngleToward(id, target, mode);
}

static __inline__ void Actor_SetTurn(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003cc8(id, a, b);
}

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_MoveTo(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Launch(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_Launch(id, a, b);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003c70(id, x, z, angle);
}

static __inline__ void Actor_PlaceOnTerrain(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003c68(id, x, z);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003be0(a, b, c, d, e, f);
}

static __inline__ void Cue(s32 cue) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003df8(cue);
}

static __inline__ void Pause(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitFrames(frames);
}

static __inline__ void Actor_SetVariant(s32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ struct ObjectRuntime *Actor_Get(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Object_GetById(id);
}

void Func_020021e8(void) {
  Func_02003c28();
  Func_02003d78(0);
  Func_02003cf8()->motionFlags = 0;
  Camera_SetBounds(658 << 16, -1, 164 << 16, 1);
  Func_02003cf0();
  Actor_FaceToward(24, gPartyState.currentOwner, 0);
  Actor_SetTurn(24, 0x100, 40);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 664, 136);
  Map_CopyCells(68, 33, 105, 6, 1, 1);
  Cue(161);
  Func_020021b4();
  Flag_Set(0x247);
  Actor_MoveTo(24, 664, 104);
  Map_CopyCells(69, 33, 105, 4, 1, 1);
  Cue(229);
  Func_020021b4();
  Pause(10);
  Actor_Launch(24, 6, 16);
  Actor_PlaceFacing(24, 222 << 18, 168 << 16, 0x4000);
  Pause(20);
  Flag_Set(0x86c);
  Func_02003c30();
}

void Func_020022e0(void) {
  Func_02003c28();
  Func_02003d78(0);
  Camera_SetBounds(245 << 18, -1, 168 << 16, 1);
  Func_02003cf0();
  Actor_FaceToward(24, gPartyState.currentOwner, 0);
  Actor_SetTurn(24, 0x100, 40);
  Actor_Get(24)->motionFlags = 0;
  Actor_SetVariant(24, 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 940, 168);
  Actor_MoveTo(24, 984, 152);
  Actor_MoveTo(24, 1040, 152);
  Actor_PlaceOnTerrain(24, 0, 0);
  Pause(20);
  Flag_Set(0x86d);
  Func_02003c30();
}

void Func_02002390(void) {
  Func_02003c28();
  Func_02003d78(0);
  Camera_SetBounds(144 << 16, -1, 214 << 15, 1);
  Func_02003cf0();
  Actor_FaceToward(25, gPartyState.currentOwner, 0);
  Actor_SetTurn(25, 0x100, 40);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_MoveTo(25, 168, 104);
  Pause(10);
  Cue(153);
  Actor_Get(25)->velocityY = 0x80000;
  ObjectDispatch_SetSingleChildFlags(Actor_Get(25), 0);
  Actor_MoveTo(25, 200, 104);
  ObjectDispatch_SetSingleChildFlags(Actor_Get(25), 1);
  Actor_MoveTo(25, 294, 104);
  Actor_PlaceFacing(25, 212 << 17, 208 << 15, 0x4000);
  Flag_Set(0x86e);
  Func_02003c30();
}

void Func_02002458(void) {
  Func_02003c28();
  Func_02003d78(0);
  Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 25);
  Func_02003cf8()->motionFlags = 0;
  Camera_SetBounds(212 << 17, -1, 208 << 15, 1);
  Func_02003cf0();
  Actor_FaceToward(25, gPartyState.currentOwner, 0);
  Actor_SetTurn(25, 0x100, 20);
  Camera_SetBounds(212 << 17, -1, 152 << 16, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_MoveTo(25, 424, 152);
  ObjectMotion_ArmCallback(25, 0, 20);
  Func_02002604();
  Actor_MoveTo(25, 440, 152);
  Actor_MoveTo(25, 456, 168);
  Actor_MoveTo(25, 456, 184);
  Flag_Set(0x241);
  Map_CopyCells(69, 51, 92, 11, 1, 1);
  Cue(161);
  Func_020021b4();
  Actor_MoveTo(25, 456, 216);
  Actor_MoveTo(25, 584, 216);
  Actor_PlaceFacing(25, 154 << 18, 140 << 17, 0x4000);
  Flag_Set(0x86f);
  Func_02003c30();
}
