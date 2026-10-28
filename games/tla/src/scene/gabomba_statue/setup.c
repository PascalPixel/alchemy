#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Func_02005714(s32 id, s32 a, s32 b);
void Actor_Stop(s32 id);
void Func_020022d0(s32 id, s32 x, s32 z, s32 facing);
void Func_020011bc(s32 message, s32 value);
void GameFlag_SetBit(s32 flag);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void Func_0200533c(void);
void FieldEvent_Wait(s32 frames);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020057b4(void);
void wait(s32 frames);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
s32 Func_02004d48(s32 a, s32 b);
void Func_0200581c(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057c4(s32 exit);

static __inline__ void Func_020057e4_(s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps the call its own 0x10000,
   * which the later call to Func_020057dc rebuilds (called directly, the
   * constant is shared in r6: differ 12). */
  Func_020057e4(a, b);
}

void Func_02000f50(s32 id) {
  Func_02005714(id, 0, 0);
  Actor_Stop(id);
  Actor_SetModeById(id, 1);
}

void Func_02000f6c(void) {
  Func_02000f50(11);
  Func_02000f50(12);
  Func_02000f50(13);
  Func_02000f50(14);
  Func_020022d0(11, 11, 9, 0);
  Func_020022d0(12, 11, 9, 0x8000);
  Func_020022d0(13, 10, 10, 0x4000);
  Func_020022d0(14, 10, 10, 0xc000);
}

void Func_02000fc0(void) {
  s32 result;

  GameFlag_SetBit(0x205);
  Func_020056b4();
  Func_0200580c(0);
  Func_02005854(78);
  Func_0200533c();
  FieldEvent_Wait(40);
  Func_02005754(MsgGabombaAcceptOffering);
  Func_02005764(24, 0);
  Camera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(256), 1);
  Func_020057b4();
  Func_02005854(220);
  wait(1);
  Func_020057e4_(0x10000, 0);
  Func_020057dc(0x7fff, 0);
  Func_020057ec(40);
  wait(80);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Func_02000f6c();
  wait(1);
  Func_020057dc(0x10000, 0);
  Func_020057ec(10);
  wait(20);
  Func_02005854(130);
  Actor_SetPartAttribute(Actor_GetById(9), 7);
  Actor_SetPartAttribute(Actor_GetById(10), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(10), 3);
  Actor_SetPartAttribute(Actor_GetById(9), 0);
  wait(8);
  Func_02005854(11);
  result = Func_02004d48(11, 1);
  Func_0200581c();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  switch (result) {
  case 3:
    Func_020057c4(14);
    break;
  case 1:
    Func_020057c4(15);
    break;
  case 2:
    Func_020057c4(16);
    break;
  case 0:
    Func_020057c4(17);
    break;
  }
}
