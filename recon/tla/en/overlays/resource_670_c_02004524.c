// near miss: differ 6 (listing 65 instructions, draft 65); scheduling after
// Func_02005bbc: the native loads the owner's address (ldr r5) between the
// zero's offset and its store; the task add now goes through KIBOMBO_TASK_ADD
// (was differ 8). Tried: an empty asm after the store (differ 4, but it swaps
// the camera call's negs), a one-pass block (6), a u8 zero (8)
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

extern const u8 gData_0200613c[];
extern const u8 gData_0200614c[];
extern const u8 gData_02006324[];

void Func_020000a0(struct KibomboObject *object);
void Func_02002f34(void);
void Func_02003030(void);
void Func_02003184(void);
void Func_02003640(s32 open, s32 hide);
void Func_020036e0(s32 open, s32 mode);
void Func_02004524(void);
void Func_020045d0(void);
void Func_020050ac(void);
void Func_02005114(void);
void Func_0200516c(void);
void Func_020054e0(void);
void Func_02005594(void);
void Func_020057e0(void);
void Func_02005898(void);
void Func_02005bcc(void);
void Func_02005c04(s32 a);
void Func_02005c54(void);
void Func_02005c5c(s32 a, s32 b, s32 c, s32 d);
void Func_02005c64(const void *script);

void Func_02004524(void) {
  struct KibomboEventWork *work;

  Func_02005a9c();
  Func_02005c3c(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Func_02003640(1, 1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    WaitFrames(1);
    gData_02007950->motionFlags = 4;
  }
  WaitFrames(1);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(40);
}
