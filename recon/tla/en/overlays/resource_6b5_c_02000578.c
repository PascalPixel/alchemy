// Draft, differ 238 of 264, skeleton gap 90 (tla-en, 2026-11-01)
// Overlay 6b5's Func_02000578, the lobby's sending of the party records and the
// Djinn recovery table, after ☀️'s BattleArena_SendPartyRecords. The record loop
// and clear loop stay goto loops: as real loops loop.c strength-reduces them,
// which the listing does not. tries is three variables (r10, r5, r8 in the
// listing), and the shifted word is read as u32 so the store aliases count and
// the k loop rereads it. Nearly all of the gap is register class: the listing
// has heap in r7, timeout and the record loop's tries high, the result in fp
// and spills the member count; this ranks timeout above heap (global.c's
// weighted refs over live length) and spills the result, so every heap use
// reads mov for adds. Lowering timeout's refs moves heap low, so that is the
// lever, but no behaviour-preserving form found does it; a copy of timeout for
// the padding loop adds an instruction the listing lacks.
// Also left: the listing keeps slotMap's address set inside the clear loop;
// here gcse's PRE lifts it, and the entry-jump form that stops PRE leaves a
// branch the listing lacks and loses the spill slot. With PRE lifting it, the j
// loop reads peerSlots through slotMap's slot where the listing adds sp afresh.
// The k loop and its first test read count through a lifted address where the
// listing has [list, 0x120 in a register]; do-while and goto k loops are worse.
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

    tries = 0;
    Iwram_CopyWords(heap, Unit_Get(memberIds[i]), size);
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
    tries = 0;
    timeout = 600;
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
