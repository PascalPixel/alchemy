#include "jupiter_lighthouse_room_m.h"

extern const u8 MsgNoWhyAre[];
extern const u8 MsgDonTWorry[];
extern const u8 MsgWeHaveTo[];
extern const u8 MsgIDonT[];
extern const u8 MsgYouVeBeen[];

void Func_020047d4(void);
void Func_020047dc(void);
void Event_SetMessageId(const u8 *message);
void Event_ShowMessageAndWait(s32 actor, s32 flags);
void Event_ResolvePendingActions(s32 a);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);

/// Talking to one of the party: each answers with its own message.
void Func_02000970(s32 actor) {
  Func_020047d4();
  Event_ResolvePendingActions(0);
  switch (actor) {
  case 0:
    Motion_SetVarCbAndRefresh(0, 2);
    Event_SetMessageId(MsgNoWhyAre);
    Event_ShowMessageAndWait(0, 0);
    break;
  case 2:
    Motion_SetVarCbAndRefresh(2, 2);
    Event_SetMessageId(MsgDonTWorry);
    Event_ShowMessageAndWait(2, 0);
    break;
  case 28:
    Event_SetMessageId(MsgWeHaveTo);
    Event_ShowMessageAndWait(28, 0);
    break;
  case 5:
    Event_SetMessageId(MsgIDonT);
    Event_ShowMessageAndWait(5, 0);
    break;
  case 6:
    Actor_SetModeById(6, 4);
    Event_Wait(33);
    Event_SetMessageId(MsgYouVeBeen);
    Event_ShowMessageAndWait(6, 0);
    break;
  }
  Func_020047dc();
}
