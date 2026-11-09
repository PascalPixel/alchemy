#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void PartyInventory_Remove(s32 a);
void Func_02001be4(void);
void Func_02000398(s32 a);
void Func_02001bec(void);
extern u8 MsgAlhafra69[];
extern u8 gMainRoutineWork[];

void Func_020002f8(void) {
  GameFlag_SetBit(0x8b5);
  PartyInventory_Remove(0x1c9);
  Func_02001be4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra69);
  Event_Wait(10);
  Actor_SetAngleToward(27, FIELD_AT(gMainRoutineWork, s32, 532), 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1b, 0, 0xa);
  Event_ShowMessageThenWait(-1, 0, 0xa);
  Actor_SetModeById(27, 1);
  Actor_Jump(27, 6, 15);
  Actor_Jump(27, 6, 23);
  Event_ShowMessageThenWait(0x1b, 0, 0);
  Func_02000398(0x1b);
  Actor_FaceDirection(27, 0x4000, 0);
  Func_02001bec();
}
