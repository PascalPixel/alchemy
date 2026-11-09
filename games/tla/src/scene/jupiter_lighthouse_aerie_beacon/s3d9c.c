#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "text/msg_ids.h"
#include "event_inline.h"
s32 Func_020059b4();
s32 Func_02004644();
s32 Actor_SetSingleChildFlags();
s32 Func_02000cd0();
s32 Party_AddActiveOwner();
s32 Func_02001388();
s32 Func_020016e8();
s32 Func_02002ae4();
s32 Func_02001bc4();
extern u8 gMainRoutineWork[];
extern u8 Func_020005b8[];

s32 Func_02003d9c(void) {
  s32 t11;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t25;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x201;
  Call2(Func_020059b4, 0xf, 1);
  Func_020059b4(0x10, 1);
  t2 = Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532));
  FIELD_AT((t2 + 0x23), u8, 0) = (0x40 | FIELD_AT((t2 + 0x23), u8, 0));
  t3 = Actor_Get(17);
  FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 0x80);
  t4 = Actor_Get(18);
  FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 0x80);
  t5 = Actor_Get(19);
  FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 0x80);
  t6 = Actor_Get(20);
  FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 0x80);
  t7 = Actor_Get(21);
  FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 0x80);
  t8 = Actor_Get(22);
  FIELD_AT((t8 + 0x23), u8, 0) = (0x80 | FIELD_AT((t8 + 0x23), u8, 0));
  t9 = Event_GetFlag(0xa21);
  if (t9 == 0) {
    Func_02004644();
  } else {
    t11 = Actor_Get(24);
    FIELD_AT(t11, s32, 24) = 0;
  }
  if ((u32)((FIELD_AT(gMainRoutineWork, u16, 482) - 1) << 16) > 0x10000) {
    Actor_SetPosition(24, 0, 0);
  }
  Actor_SetSingleChildFlags(Actor_Get(36), 0);
  Actor_SetSingleChildFlags(Actor_Get(36), 0);
  Task_AddFrame((s32)Func_020005b8, 0x480);
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 9) {
    Func_02000cd0();

    FIELD_AT(gMainRoutineWork, u16, 608) = (s32)MsgRobe;
    FIELD_AT(gMainRoutineWork, u16, 610) = 1;
  } else {
    if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xa) {
      Party_AddActiveOwner(5);
      Func_02001388();
    } else {
      if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xb) {
        Party_AddActiveOwner(6);
        Func_020016e8();
      } else {
        if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xc) {
          Func_02002ae4();
        } else {
          if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xd) {
            Func_02001bc4();
          } else {
            t25 = Event_GetFlag(0xa21);
            if (t25 != 0) {
              Actor_SetPosition(27, 0, 0);
              Actor_SetPosition(28, 0, 0);
            }
          }
        }
      }
    }
  }
  return 0;
}
