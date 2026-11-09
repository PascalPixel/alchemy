#include "jupiter_lighthouse_room_m.h"
#include "event_inline.h"

extern const u8 MsgNoWhyAre[];
extern const u8 MsgDonTWorry[];
extern const u8 MsgWeHaveTo[];
extern const u8 MsgIDonT[];
extern const u8 MsgYouVeBeen[];

void Func_020047d4(void);
void Func_020047dc(void);

/// Talking to one of the party: each answers with its own message.
void Func_02000970(s32 actor) {
  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  switch (actor) {
  case 0:
    Actor_RunRepeatedMotion(0, 2);
    FieldEvent_SetMessageId(MsgNoWhyAre);
    Event_ShowMessage(0, 0);
    break;
  case 2:
    Actor_RunRepeatedMotion(2, 2);
    FieldEvent_SetMessageId(MsgDonTWorry);
    Event_ShowMessage(2, 0);
    break;
  case 28:
    FieldEvent_SetMessageId(MsgWeHaveTo);
    Event_ShowMessage(28, 0);
    break;
  case 5:
    FieldEvent_SetMessageId(MsgIDonT);
    Event_ShowMessage(5, 0);
    break;
  case 6:
    Actor_SetAnimation(6, 4);
    FieldEvent_Wait(33);
    FieldEvent_SetMessageId(MsgYouVeBeen);
    Event_ShowMessage(6, 0);
    break;
  }
  Func_020047dc();
}
