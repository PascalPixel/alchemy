#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "flag_call.h"
#include "object_runtime.h"

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

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_0200006c(s32 slot);

// The lobby's task each frame, ported from ☀️'s LinkLobby_PollPeerReady:
// counts frames without the opponent, clears the peer signatures after 25,
// and raises the event trigger when both sides are ready or the link is
// lost. Returns whether the peer answered.
s32 Func_02000134(void) {
  struct LobbyEventWork *work;
  s32 result;
  s32 i;
  s32 set;
  s32 z;
  s32 *wait;

  work = Ram_HeapSlots->eventWork;
  result = 1;
  z = Object_GetById(4)->z;
  /* FAKEMATCH: the one-pass block loads z before the 0xe00000 bound is built;
   * tested directly the bound comes first (differ 2). */
  do {
  } while (0);
  if (z > 0xe00000)
    Flag_Clear(0x304);
  if (work->raisedTrigger != 2) {
    Func_0200006c(0);
    /* FAKEMATCH: the wait counter's address taken once before the test, where
     * the native loads it for both branches; written in each branch it is
     * loaded in each (differ 11). */
    set = Flag_IsSet(0x303);
    wait = &gData_020022b8;
    if (!set) {
      if (++*wait > 25) {
        s32 offset = 0;

        /* FAKEMATCH: a goto loop counting down with the stride stepped by hand;
         * as ☀️'s for loop, loop.c also keeps the table and the routine's
         * addresses in registers (differ 76). */
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
      *wait = 0;
    }
    if (gData_020022b8 == 0) {
      if (Func_0200006c(0) && (Func_0200006c(1) || Func_0200006c(2))) {
        Flag_Set(0x201);
        if (Flag_IsSet(0x202))
          work->raisedTrigger = 1;
        result = 1;
      } else {
        Flag_Clear(0x201);
        result = 0;
      }
    }
    if (Flag_IsSet(0x201) && Flag_IsSet(0x202) && !Flag_IsSet(0x200))
      work->raisedTrigger = 1;
  }
  if ((Flag_IsSet(0x201) || Flag_IsSet(0x202)) && !Flag_IsSet(0x173) &&
      !Func_0200006c(0) && gData_020022b8 > 24) {
    work->raisedTrigger = 2;
    Flag_Set(0x205);
    Flag_Clear(0x201);
    Flag_Clear(0x202);
    LinkLobby_WriteSlotValue(4);
  }
  if (Flag_IsSet(0x205))
    work->raisedTrigger = 2;
  return result;
}
