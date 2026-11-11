#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "serial.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

void LinkLobby_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gLinkLobby_SlotValues[slot];
  dst[gLinkLobby_SlotColumns[slot]] = *src;
}

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

struct LinkPeerSignature {
  u8 bytes[24];
};

extern struct LinkPeerSignature gLinkPeerSignatures[];
extern s32 gData_020022b8;

s32 Func_0200006c(s32 slot);

/// The lobby's task each frame: counts frames without the opponent, clears
/// the peer signatures after 25, and raises the event trigger when both sides
/// are ready or the link is lost. Returns whether the peer answered.
// Ported from ☀️'s LinkLobby_PollPeerReady.
s32 Func_02000134(void) {
  struct LobbyEventWork *work;
  s32 result;
  s32 i;
  s32 set;
  s32 z;

  work = Ram_HeapSlots->eventWork;
  result = 1;
  z = Actor_GetById(4)->z;

  if (z > 0xe00000)
    Event_ClearFlag(0x304);
  if (work->raisedTrigger != 2) {
    Func_0200006c(0);
    set = Event_GetFlag(0x303);
    if (!set) {
      if (++gData_020022b8 > 25) {
        s32 offset = 0;

        // A goto loop counting down with the stride stepped by hand; as ☀️'s
        // for loop, loop.c also keeps the table and the routine's addresses
        // in registers (differ 76).
        i = 3;
      clear:
        Iwram_ClearWords((u8 *)gLinkPeerSignatures + offset, 20);
        i--;
        offset += 24;
        if (i >= 0)
          goto clear;
        gData_020022b8 = 0;
        LinkLobby_WriteSlotValue(4);
      }
    } else {
      gData_020022b8 = 0;
    }
    if (gData_020022b8 == 0) {
      if (Func_0200006c(0) && (Func_0200006c(1) || Func_0200006c(2))) {
        Event_SetFlag(0x201);
        if (Event_GetFlag(0x202))
          work->raisedTrigger = 1;
        result = 1;
      } else {
        Event_ClearFlag(0x201);
        result = 0;
      }
    }
    if (Event_GetFlag(0x201) && Event_GetFlag(0x202) && !Event_GetFlag(0x200))
      work->raisedTrigger = 1;
  }
  if ((Event_GetFlag(0x201) || Event_GetFlag(0x202)) && !Event_GetFlag(0x173) &&
      !Func_0200006c(0) && gData_020022b8 > 24) {
    work->raisedTrigger = 2;
    Event_SetFlag(0x205);
    Event_ClearFlag(0x201);
    Event_ClearFlag(0x202);
    LinkLobby_WriteSlotValue(4);
  }
  if (Event_GetFlag(0x205))
    work->raisedTrigger = 2;
  return result;
}
