#include "types.h"
#include "main_routine_work.h"
#include "actor.h"

extern const u8 MsgThenLetS[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_020020ac(s32 a);
void Func_020020b4(s32 id, s32 a, s32 b, s32 c);
void Func_0200201c(const u8 *message);
void Func_02002034(s32 id, s32 a);
void Func_02001fc4(s32 id, s32 a, s32 b);
void Func_02001d60(struct Actor *object);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);

static __inline__ void Func_020020b4_(s32 id, s32 a, s32 b, s32 c) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Func_020020b4(id, a, b, c);
}

void Func_02001da0(void) {
  if (GameFlag_Test(7)) {
    struct Actor *object;

    Func_02001f5c();
    Func_020020ac(0);
    Actor_SetSpeedParameters(4, 0xcccc, 0x6666);
    Actor_GetById(7)->update = (void (*)(void))Func_02001d60;
    Func_020020b4_(7, 16, 0, 0x8000);
    Actor_SetSingleChildFlags(Actor_GetById(7), 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    Func_0200201c(MsgThenLetS);
    Actor_WaitForMoveById(7);
    Actor_GetById(7)->update = 0;
    Func_02002034(7, 0);
    Motion_SetModeAndWaitAnimation(4, 3);
    Actor_GetById(gMainRoutineWork.currentOwner)->update =
        (void (*)(void))Func_02001d60;
    Actor_CommitPositionAndActivate(4, 0, 16);
    Actor_GetById(gMainRoutineWork.currentOwner)->update = 0;
    Actor_GetById(7)->update = (void (*)(void))Func_02001d60;
    Actor_SetModeById(7, 2);
    object = Actor_GetById(4);
    if (object != 0)
      Actor_ResetAndSetPosition(7, *(s16 *)((u8 *)object + 10),
                                       *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(7);
    Func_02001fc4(7, 0, 0);
    Actor_GetById(7)->update = 0;
    Actor_SetSingleChildFlags(Actor_GetById(7), 1);
    Func_02001f64();
  }
}
