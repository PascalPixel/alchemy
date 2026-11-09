#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

extern s32 gData_020022bc;

void Func_02001a14(void);
void Func_02001a1c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

s32 Func_020002d8(void) {
  s32 set;

  set = GameFlag_Test(0x203);
  if (set != 0) {
    return set;
  }
  if (++gData_020022bc == 300) {
    gData_020022bc = 0;
    GameFlag_ClearBit(0x200);
  }
  set = Flag_Test(0x200);
  if (set != 0) {
    return set;
  }
  Func_02001a14();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgIfYouWantToParticipate);
  FieldMessage_ShowAtActor(8, 0);
  wait(5);
  GameFlag_SetBit(0x200);
  Func_02001a1c();
}

s32 Func_02000344(void) {
  struct LobbyEventWork *work = Ram_HeapSlots->eventWork;

  LinkLobby_WriteSlotValue(4);
  GameFlag_ClearBit(0x200);
  GameFlag_ClearBit(0x203);
  Func_02001a14();
  Event_ResolvePendingActions(0);
  work->raisedTrigger = 0;
  Event_SetMessageId((s32)MsgTheLinkWas);
  FieldMessage_ShowAtActor(8, 0);
  GameFlag_ClearBit(0x205);
  Func_02001a1c();
}
