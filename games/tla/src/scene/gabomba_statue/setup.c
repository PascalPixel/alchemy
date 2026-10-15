#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02005714(s32 id, s32 a, s32 b);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void Func_020022d0(s32 id, s32 x, s32 z, s32 facing);
void Func_020011bc(s32 message, s32 value);
void GameFlag_SetBit(s32 flag);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void Func_0200533c(void);
void Battle_WaitMode0(s32 frames);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020057b4(void);
void WaitFrames(s32 frames);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
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
  ObjectMotion_EnableActionAndResetMotion(id);
  Object_SetModeById(id, 1);
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
  Battle_WaitMode0(40);
  Func_02005754(MsgGabombaAcceptOffering);
  Func_02005764(24, 0);
  Motion_CamBounds(0x1880000, -1, 0x1000000, 1);
  Func_020057b4();
  Func_02005854(220);
  WaitFrames(1);
  Func_020057e4_(0x10000, 0);
  Func_020057dc(0x7fff, 0);
  Func_020057ec(40);
  WaitFrames(80);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  Func_02000f6c();
  WaitFrames(1);
  Func_020057dc(0x10000, 0);
  Func_020057ec(10);
  WaitFrames(20);
  Func_02005854(130);
  Object_SetPartAttribute(Object_GetById(9), 7);
  Object_SetPartAttribute(Object_GetById(10), 7);
  WaitFrames(8);
  Object_SetPartAttribute(Object_GetById(10), 3);
  Object_SetPartAttribute(Object_GetById(9), 0);
  WaitFrames(8);
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
