#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
void GameFlag_SetBit(s32 a);
void PartyInventory_Remove(s32 a);
void Func_02001be4(void);
void Func_02001cfc(s32 a);
void Func_02001c74(s32 a);
void FieldEvent_Wait(s32 a);
void Actor_SetAngleToward(s32 a, s32 b, s32 c);
void Func_02001c84(s32 a, s32 b, s32 c);
void Actor_SetModeById(s32 a, s32 b);
void Actor_Jump(s32 a, s32 b, s32 c);
void Func_02000398(s32 a);
void Actor_ArmCallback(s32 a, s32 b, s32 c);
void Func_02001bec(void);
extern u8 MsgAlhafra69[];
extern u8 gMainRoutineWork[];

void Func_020002f8(void) {
  Call1(GameFlag_SetBit, 0x8b5);
  Call1(PartyInventory_Remove, 0x1c9);
  Func_02001be4();
  Call1(Func_02001cfc, 0);
  Call1(Func_02001c74, (s32)MsgAlhafra69);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetAngleToward, 27, FIELD_AT(gMainRoutineWork, s32, 532), 0);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001c84, 0x1b, 0, 0xa);
  Call3(Func_02001c84, -1, 0, 0xa);
  Call2(Actor_SetModeById, 27, 1);
  Call3(Actor_Jump, 27, 6, 15);
  Call3(Actor_Jump, 27, 6, 23);
  Call3(Func_02001c84, 0x1b, 0, 0);
  Call1(Func_02000398, 0x1b);
  Call3(Actor_ArmCallback, 27, 0x4000, 0);
  Func_02001bec();
}
