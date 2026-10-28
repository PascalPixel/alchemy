#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

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

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldMessage_Show(s32 message, s32 mode);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_020010bc(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020010c4(struct Actor *object);
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
void Func_02000270(struct Actor *object);
void Func_020008d0(struct Actor *object);
void Func_02000958(void);
void Func_02000a28(void);
void Func_02000c94(void);
void Func_02000cf4(void);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001134(id, x, z);
}

static __inline__ void Wait(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  wait(frames);
}

static __inline__ void Cue(s32 cue) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001214(cue);
}

void Func_02000874(void) {
  struct Actor *object;
  s32 x;

  if (Flag_Test(0x8cc) == 0) {
    object = Actor_GetById(64);
    x = object->x;
    /* FAKEMATCH: ⚓️ loads x before it builds the limit */
    asm volatile("" : "+l"(x));
    if (x < (186 << 16)) {
      Func_0200110c();
      Func_020011ac(0);
      Cue(204);
      object->motionFlags = 3;
      Wait(10);
      Actor_Place(64, 0, 0);
      Flag_Set(0x8cc);
      Func_02001114();
    }
  }
}
