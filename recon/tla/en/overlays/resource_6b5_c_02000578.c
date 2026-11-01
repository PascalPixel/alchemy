// Draft, differ 216 of 264, skeleton gap 82 (tla-en, 2026-11-01)
// Overlay 6b5's Func_02000578, the lobby's sending of the party records and the
// Djinn recovery table, after ☀️'s BattleArena_SendPartyRecords. Confirmed twin
// by statements, but not by allocation: the twin has heap r8, i r7, timeout r6.
// The record and clear loops stay goto loops: real loops strength-reduce them
// (twin's for loops measure differ 290), which the listing does not. tries is
// three variables (r10, r5, r8 in the listing); the shifted word is read as u32
// so the store aliases count and the k loop rereads it.
// This round: tries = 0 after the copy and timeout before tries in the last
// block took differ from 238. The listing shows tries = 0 above Unit_Get, but
// that is a callee-saved set sched2 may move; written there it measures 238.
// Left, register class. Listing order is i, heap, memberIds' address, tries,
// timeout, result, then memberCount and the slot address spilled (sp+4, sp+0).
// Here (--rtl): timeout 25 refs p.61, tries p.446, heap p.431, address p.226,
// memberCount p.157, slot address p.111, result p.07, so result is spilled and
// memberCount takes fp. timeout's refs come from each wait loop's two back edges
// to test, each adding a depth (flow.c). Measured: one latch per wait loop drops
// timeout to 19 refs p.452 and the last block's timeout below its tries, as the
// listing has, but heap still ranks third; that needed an empty asm at each
// join, a barrier that gained only 2, so it came back out. Weights that count
// each loop once fit the listing better than this compiler's do.
// Left, hoisting. The listing keeps sp+8 in the clear loop and the j loop, and
// reads [list, 0x120 in a register] in the k loop; here gcse's PRE lifts both,
// (plus sfp 8) and (plus list 0x120), and loop.c moves the rest. A goto k loop
// loses (differ 227, 232); the guarded do-while gains 2 of differ and loses 5
// of skeleton (k < list->count - 1 under if, differ 214, gap 51/41). No source
// form found stops PRE without a branch the listing lacks. Same family as the
// frame-address notes in other drafts; a compiler question, out of scope here.
// result above memberCount needs result undoubled (local-alloc doubles L for
// its constant first set) or a longer memberCount; neither has a plain form.
#include "types.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "owner_state.h"
#include "serial.h"

/// The list inside a sent recovery table, as ☀️'s DjinnRecoveryList.
struct OfferList {
  struct TradeOffer offers[72];
  s32 count;
};

/// The recovery table as sent: the list from its eighth byte.
struct OfferTable {
  u8 unk00[8];
  struct OfferList list;
};

s32 Func_02000538(u16 *memberIds);
s32 Func_0200192c(void *data, s32 size);
s32 Func_0200193c(void);
void wait(s32 frames);

s32 Func_02000578(void) {
  s32 result;
  u8 *slotMap;
  u16 memberIds[8];
  u8 peerSlots[8];
  s32 transferResult;
  u8 *heap;
  s32 memberCount;
  s32 timeout;
  s32 tries;
  s32 i;
  s32 j;
  s32 k;
  u32 size;
  struct OfferList *list;

  size = 0x154;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = 600;
  memberCount = Func_02000538(memberIds);
  i = 0;
clear:
  slotMap = peerSlots;
  slotMap[i] = 0;
  i++;
  if (i <= 7)
    goto clear;
  i = 0;
  if (i < memberCount) {
  send: {
    s32 tries;

    Iwram_CopyWords(heap, Unit_Get(memberIds[i]), size);
    tries = 0;
    heap[0x12a] = 2;
    slotMap[memberIds[i]] = i - 128;
    if ((transferResult = Func_0200192c(heap, size)) == -1) {
      result = transferResult;
      goto done;
    }
    goto testMember;
  waitMember:
    wait(1);
    if (--timeout < 0 || (gLinkStatus & 3) != 3) {
      if (++tries > 24) {
        result = -1;
        goto done;
      }
    }
  testMember:
    if (Func_0200193c() != 0)
      goto waitMember;
    wait(2);
    i++;
    if (i < memberCount)
      goto send;
  }
  }
  goto next;
wait:
  wait(1);
  if (--timeout < 0 || (gLinkStatus & 3) != 3) {
    if (++tries > 24) {
      result = -1;
      goto done;
    }
  }
test:
  if (Func_0200193c() != 0)
    goto wait;
  wait(2);
  i++;
next:
  if (i <= 2) {
    heap[0x12a] = tries = 0;
    if ((transferResult = Func_0200192c(heap, size)) != -1)
      goto test;
    result = transferResult;
    goto done;
  }
  Heap_Release(heap);
  size = 0x154;
  heap = Heap_AllocEwram(size);
  Iwram_CopyWords(heap, Djinn_GetRecoveryTable(0), size);
  {
    s32 timeout;
    s32 tries;

    list = &((struct OfferTable *)heap)->list;
    timeout = 600;
    tries = 0;
    for (j = 0; j < list->count; j++) {
      list->offers[j].owner = *(peerSlots + list->offers[j].owner);
      if (list->offers[j].owner == 0) {
        for (k = j; k < list->count - 1; k++)
          ((u32 *)list->offers)[k] = ((u32 *)list->offers)[k + 1];
        list->count--;
        j--;
      }
    }
    if ((transferResult = Func_0200192c(heap, size)) == -1) {
      result = transferResult;
      goto done;
    }
    goto testOffers;
  waitOffers:
    wait(1);
    if (--timeout < 0 || (gLinkStatus & 3) != 3) {
      if (++tries > 24) {
        result = -1;
        goto done;
      }
    }
  testOffers:
    if (Func_0200193c() != 0)
      goto waitOffers;
    wait(1);
    wait(2);
  }
done:
  Heap_Release(heap);
  return result;
}
