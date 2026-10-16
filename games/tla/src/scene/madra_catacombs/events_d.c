#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 flag;
};

struct AnimationRecord18 {
  u8 unk00[0x12];
  u16 frame;
};

extern u8 MsgFieldEyesOfTruth;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];
extern const u8 gData_020013ca[];
extern const u8 gData_0200121c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
void FieldEvent_WaitFrames(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_020010bc(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020010c4(struct ObjectRuntime *object);
void Func_020010cc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020010dc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001134(s32 id, s32 x, s32 z);
void Func_0200113c(s32 id, s32 value);
void Func_0200115c(s32 id);
void Func_0200117c(s32 entrance);
s32 Func_02001184(s32 owner);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
void Func_020011c4(const void *data);
void Func_020011dc(const void *data);
s32 Func_020011fc(void);
void Func_02001204(void);
void Func_02000038(const void *data);
void Func_0200014c(const void *data);
void Func_02000270(struct ObjectRuntime *object);
void Func_020008d0(struct ObjectRuntime *object);
void Func_02000958(void);
void Func_02000a28(void);
void Func_02000c94(void);
void Func_02000cf4(void);

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

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020010cc(a, b, c, d, e, f);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020010dc(a, b, c, d, e, f);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001134(id, x, z);
}

static __inline__ void Wait(s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  WaitFrames(frames);
}

static __inline__ void Cue(s32 cue) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001214(cue);
}

static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

void Func_0200072c(void) {
  if (Flag_Test(0x8cb) != 0)
    Map_CopyCells(72, 63, 36, 38, 1, 2);
  else
    Map_CopyCells(71, 63, 36, 38, 1, 2);
  Flag_Set(0x203);
}

void Func_02000778(void) {
  Map_CopyCells(70, 63, 36, 38, 1, 2);
  Flag_Clear(0x203);
}

void Func_020007a0(void) {
  if (Flag_Test(0x203) != 0 && Flag_Test(0x8cb) == 0) {
    Func_0200110c();
    Func_020011ac(1);
    Cue(161);
    Map_CopyCells(72, 63, 36, 38, 1, 2);
    FieldEvent_WaitFrames(40);
    Cue(188);
    Map_CopyCells(71, 66, 97, 39, 2, 2);
    Wait(2);
    Map_CopyCells(73, 66, 97, 39, 2, 2);
    Wait(2);
    Map_CopyCells(75, 66, 97, 39, 2, 2);
    Map_CopyAttributes(33, 37, 2, 2, 33, 39);
    Wait(1);
    Flag_Set(0x8cb);
    Func_02001114();
  } else {
    Func_0200110c();
    Func_020011ac(0);
    UiText_ShowPositionedMessageAndWait((s32)&MsgFieldEyesOfTruth, 1);
    Func_02001114();
  }
}
