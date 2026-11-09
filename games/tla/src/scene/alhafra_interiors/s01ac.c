#include "alhafra_interiors.h"

struct AlhafraInteriorsObject *Actor_GetById(s32 id);
void Event_SetMessageId(s32 message);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
void Func_020001ac(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000bc4(9, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors14);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_020001fc(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000bd4(2, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors16);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_0200024c(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000bcc(actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors49);
    Event_ShowMessageAndWait(actor, 0);
  }
}
