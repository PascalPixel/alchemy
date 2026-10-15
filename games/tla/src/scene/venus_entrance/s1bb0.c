#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
void Func_020028b4(void);
void Func_020029cc(s32 a);
void Func_0200293c(s32 a);
void Func_020029d4(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 a);
void Func_0200294c(s32 a, s32 b, s32 c);
void Object_SetModeById(s32 a, s32 b);
s32 Object_GetById(s32 a);
void ObjectMotion_ResetAndSetPosition(s32 a, s32 b, s32 c);
void Func_0200290c(s32 a, s32 b, s32 c);
void Battle_WaitMode0(s32 a);
void ObjectMotion_CommitPositionAndActivate(s32 a, s32 b, s32 c);
void Func_020028bc(void);
extern u8 MsgNotThatWayThe[];

void Func_02001bb0(void) {
  s32 t7;

  Func_020028b4();
  Call1(Func_020029cc, 0);
  Call1(Func_0200293c, (s32)MsgNotThatWayThe);
  Call4(Func_020029d4, 8, 0x10, 0, 0x8000);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 8);
  Call3(Func_0200294c, 0x2008, 0, 0xa);
  Call2(Object_SetModeById, 8, 2);
  t7 = Value1(Object_GetById, 5);
  if (t7 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 8, FIELD_AT(t7, s16, 10),
          FIELD_AT(t7, s16, 18));
  }
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 8);
  Call3(Func_0200290c, 8, 0, 0);
  Call1(Battle_WaitMode0, 0xa);
  Call3(ObjectMotion_CommitPositionAndActivate, 5, -16, 0);
  Func_020028bc();
}
