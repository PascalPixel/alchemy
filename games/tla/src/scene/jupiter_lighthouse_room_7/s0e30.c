#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000060();
void Func_020019dc();
void Func_02001db0();
void Func_02001eb8();
void Func_02001ee8();
extern u8 gData_020031c4[];

s32 Func_02000e30(void) {
  s32 t1, t2, t3, t5, t8, t15, t16, t17, t19, t20, t21, t23, t24, t25, t26, t27,
      t28, t29, v5;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x204;
  if ((u32)((FIELD_AT(&gMainRoutineWork, u16, 482) + -3) << 16) > (u32)0x10000)
    goto L_02008e6e;
  t1 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT(t1, u8, 35) = (32 | FIELD_AT(t1, u8, 35));
L_02008e6e:
  t2 = ((s32)Actor_Get(8));
  FIELD_AT(t2, s32, 24) = 0xc000;
  t3 = ((s32)Actor_Get(8));
  FIELD_AT(t3, s32, 28) = 0xc000;
  Func_020019dc((s32)&gData_020031c4);
  t5 = Event_GetFlag(0xa1b);
  if (t5 == 0)
    goto L_02008eaa;
  Actor_SetPosition(16, PIXELS(136), PIXELS(600));
  Func_02000060(0);
L_02008eaa:
  t8 = Event_GetFlag(0x202);
  if (t8 == 0)
    goto L_02008ebe;
  Func_02000060(0);
L_02008ebe:
  Func_02001db0();
  Func_02001ee8(2);
  Func_02001eb8(18, 2);
  Func_02001eb8(19, 1);
  Func_02001eb8(20, 0);
  t15 = Event_GetFlag(0xa10);
  if (t15 != 0)
    goto L_02008f08;
  t16 = Event_GetFlag(0xa12);
  if (t16 != 0)
    goto L_02008f08;
  t17 = Event_GetFlag(0xa14);
  if (t17 == 0)
    goto L_02008f12;
L_02008f08:
  Actor_SetPosition(18, 0, 0);
L_02008f12:
  t19 = Event_GetFlag(0xa11);
  if (t19 != 0)
    goto L_02008f3c;
  t20 = Event_GetFlag(0xa13);
  if (t20 != 0)
    goto L_02008f3c;
  t21 = Event_GetFlag(0xa15);
  if (t21 == 0)
    goto L_02008f46;
L_02008f3c:
  Actor_SetPosition(19, 0, 0);
L_02008f46:
  t23 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT(t23, u8, 35) = (64 | FIELD_AT(t23, u8, 35));
  t24 = ((s32)Actor_Get(10));
  FIELD_AT(t24, u8, 35) = (FIELD_AT(t24, u8, 35) | 128);
  t25 = ((s32)Actor_Get(11));
  FIELD_AT(t25, u8, 35) = (FIELD_AT(t25, u8, 35) | 128);
  t26 = ((s32)Actor_Get(12));
  FIELD_AT(t26, u8, 35) = (FIELD_AT(t26, u8, 35) | 128);
  t27 = ((s32)Actor_Get(13));
  FIELD_AT(t27, u8, 35) = (FIELD_AT(t27, u8, 35) | 128);
  t28 = ((s32)Actor_Get(14));
  FIELD_AT(t28, u8, 35) = (FIELD_AT(t28, u8, 35) | 128);
  t29 = ((s32)Actor_Get(15));
  v5 = (128 | FIELD_AT(t29, u8, 35));
  FIELD_AT(t29, u8, 35) = v5;
  return 0;
}
