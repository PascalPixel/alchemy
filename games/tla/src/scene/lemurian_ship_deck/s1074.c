#include "types.h"
#include "main_routine_work.h"
#include "actor.h"

extern const u8 MsgWellThereS[];

#include "flag_call.h"
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_020020ac(s32 a);
void Func_02001fd4(s32 id, s32 target);
void Func_02002044(s32 id, s32 angle);
void Func_0200201c(const u8 *message);
void Func_02002034(s32 id, s32 a);
void Func_02001fc4(s32 id, s32 a, s32 b);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);

static __inline__ void SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPosition(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Actor_SetPositionAndReset(id, x, z);
}

void Func_02001074(void) {
  struct Actor *object;

  if (Flag_IsSet(0x212))
    return;
  if (Flag_IsSet(0x988))
    return;
  Flag_Set(0x212);
  if (Flag_IsSet(0x211)) {
    Func_02001f5c();
    Func_020020ac(0);
    Func_02001fd4(9, gMainRoutineWork.currentOwner);
    wait(1);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
    SetSpeed(9, 0xcccc, 0x6666);
    SetPosition(9, 416, 428);
    Func_02002044(9, 0x8000);
    Func_0200201c(MsgWellThereS);
    Func_02002034(9, 0);
    Actor_SetModeById(9, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_ResetAndSetPosition(9, *(s16 *)((u8 *)object + 10),
                                       *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(9);
    Func_02001fc4(9, 0, 0);
    Flag_Set(0x988);
    Func_02001f64();
  } else if (Flag_IsSet(0x210)) {
    Flag_Set(0x211);
  } else {
    Flag_Set(0x210);
  }
}
