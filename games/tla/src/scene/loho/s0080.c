#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

extern char SceneId_Ov6A6Room2;
extern char MsgLoho23;
extern char MsgLoho27;
extern const u8 gData_02001900[];
extern const u8 gData_02001a80[];

void Battle_WaitMode0(s32 frames);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
void Func_020012b0(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020012b8(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02001380(s32 message);
void Func_02001390(s32 id, s32 flags);
s32 Func_02001420(void);

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

const u8 *Func_02000080(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001a80;
  return gData_02001900;
}

void Func_020000ac(void) {
  Func_020012b0(69, 70, 10, 12, 24, 7);
  Func_020012b8(69, 70, 10, 12, 24, 7);
  Func_020012b0(79, 70, 10, 12, 88, 7);
  Func_020012b8(79, 70, 10, 12, 88, 7);
  Func_020012b0(69, 82, 10, 12, 24, 71);
  Func_020012b8(69, 82, 10, 12, 24, 71);
}

void Func_0200012c(s32 actor) {
  s32 msg = (s32)&MsgLoho23;

  Func_02001380(msg);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001420(), 0) == 0) {
    Battle_WaitMode0(10);
    Func_02001380(msg + 1);
  } else {
    Battle_WaitMode0(20);
    Func_02001380(msg + 2);
  }
  Func_02001390(actor, 0);
}

void Func_02000178(void) {
  UiText_ShowPositionedMessageAndWait((s32)&MsgLoho27, 1);
  ((struct EventWork15a *)Ram_HeapSlots->eventWork)->flag = 1;
}
