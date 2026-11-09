#include "kibombo.h"

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
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Func_02003640(1, 1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  wait(1);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
}
