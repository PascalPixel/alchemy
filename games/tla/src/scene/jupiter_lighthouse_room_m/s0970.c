#include "jupiter_lighthouse_room_m.h"

extern const u8 MsgNoWhyAre[];
extern const u8 MsgDonTWorry[];
extern const u8 MsgWeHaveTo[];
extern const u8 MsgIDonT[];
extern const u8 MsgYouVeBeen[];

void Func_020047d4(void);
void Func_020047dc(void);
void Func_0200488c(const u8 *message);
void Func_020048a4(s32 actor, s32 flags);
void Func_0200494c(s32 a);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);

/// Talking to one of the party: each answers with its own message.
void Func_02000970(s32 actor) {
  Func_020047d4();
  Func_0200494c(0);
  switch (actor) {
  case 0:
    Motion_SetVarCbAndRefresh(0, 2);
    Func_0200488c(MsgNoWhyAre);
    Func_020048a4(0, 0);
    break;
  case 2:
    Motion_SetVarCbAndRefresh(2, 2);
    Func_0200488c(MsgDonTWorry);
    Func_020048a4(2, 0);
    break;
  case 28:
    Func_0200488c(MsgWeHaveTo);
    Func_020048a4(28, 0);
    break;
  case 5:
    Func_0200488c(MsgIDonT);
    Func_020048a4(5, 0);
    break;
  case 6:
    Object_SetModeById(6, 4);
    FieldEvent_WaitFrames(33);
    Func_0200488c(MsgYouVeBeen);
    Func_020048a4(6, 0);
    break;
  }
  Func_020047dc();
}
