// Draft, differ 84 of 180, skeleton gap 24 (tla-en, 2026-11-01): Func_02000134,
// measured with --symbol Func_02000134. Overlay 6B5, from ☀️ link_lobby/lobby.c.
// Func_020002d8 and Func_02000344 are differ 0. What is left in Func_02000134 is
// the peer clear loop and what it costs: every plain loop tried (for, do, while,
// for (;;), by index, pointer or stepped offset) lets loop.c hoist the table and
// IwramClearWords addresses into r6 and r7, where the listing loads both inside
// the loop; work and result then move to r8 and sl, and most of the gap is that
// allocation. A label-and-goto loop stepping an offset closes it (differ 8, gap
// 2), and a local holding Flag_Test(0x303) so the counter's address loads after
// the call reaches differ 0, but both are devices, tagged FAKEMATCH in the same
// function in games/tla/src/scene/lobby/lobby.c; this draft stays plain. The
// waitCounter local is a one-address local, no device.
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "serial.h"
#include "text/msg_ids.h"

struct LinkPeer {
  s32 values[6];
};

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

extern struct LinkPeer gLinkPeerSignatures[4];
extern s32 gData_020022b8;
extern s32 gData_020022bc;

s32 Func_0200006c(s32 slot);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001ab4(s32 skip);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_ClearBit(flag);
}

s32 Func_02000134(void) {
  struct LobbyEventWork *work;
  s32 result;
  s32 i;
  s32 *waitCounter;

  work = Ram_HeapSlots->eventWork;
  result = 1;
  if (((struct Actor *)Actor_GetById(4))->z > 0xe00000) {
    Flag_Clear(0x304);
  }
  if (work->raisedTrigger != 2) {
    Func_0200006c(0);
    waitCounter = &gData_020022b8;
    if (!Flag_Test(0x303)) {
      if (++*waitCounter > 25) {
        for (i = 0; i < 4; i++) {
          Iwram_ClearWords(&gLinkPeerSignatures[i], 20);
        }
        gData_020022b8 = 0;
        LinkLobby_WriteSlotValue(4);
      }
    } else {
      *waitCounter = 0;
    }
    if (gData_020022b8 == 0) {
      if (Func_0200006c(0) && (Func_0200006c(1) || Func_0200006c(2))) {
        Flag_Set(0x201);
        if (Flag_Test(0x202)) {
          work->raisedTrigger = 1;
        }
        result = 1;
      } else {
        Flag_Clear(0x201);
        result = 0;
      }
    }
    if (Flag_Test(0x201) && Flag_Test(0x202) && !Flag_Test(0x200)) {
      work->raisedTrigger = 1;
    }
  }
  if ((Flag_Test(0x201) || Flag_Test(0x202)) && !Flag_Test(0x173) &&
      !Func_0200006c(0) && gData_020022b8 > 24) {
    work->raisedTrigger = 2;
    Flag_Set(0x205);
    Flag_Clear(0x201);
    Flag_Clear(0x202);
    LinkLobby_WriteSlotValue(4);
  }
  if (Flag_Test(0x205)) {
    work->raisedTrigger = 2;
  }
  return result;
}

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
  Func_02001ab4(0);
  Func_02001a64((s32)MsgIfYouWantToParticipate);
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
  Func_02001ab4(0);
  work->raisedTrigger = 0;
  Func_02001a64((s32)MsgTheLinkWas);
  FieldMessage_ShowAtActor(8, 0);
  GameFlag_ClearBit(0x205);
  Func_02001a1c();
}
