#include "types.h"
#include "call.h"
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
  for (i = 0; i <= 7; i++)
    peerSlots[i] = 0;
  i = 0;
  if (i >= memberCount)
    goto next;
  {
  send:
    {
      s32 tries = 0;

      Iwram_CopyWords(heap, Unit_Get(memberIds[i]), size);
      heap[0x12a] = 2;
      peerSlots[memberIds[i]] = i - 128;
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
      /* FAKEMATCH(one-pass): an empty once-only block between the test and the
       * jump, which stops cross-jumping joining the test to the guard; as no
       * block, 14 of 264 instructions differ in tla-en. */
      do {
      } while (0);
      goto next;
    }
  }
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
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 6 of 264 instructions differ
   * in tla-en. */
  do {
    Iwram_CopyWords(heap, Djinn_GetRecoveryTable(0), size);
  } while (0);
  {
    s32 timeout;
    s32 tries;

    list = &((struct OfferTable *)heap)->list;
    tries = 0;
    timeout = 600;
    for (j = 0; j < list->count; j++) {
      list->offers[j].owner = peerSlots[list->offers[j].owner];
      if (list->offers[j].owner == 0) {
        for (k = j; k < list->count - 1; k++)
          list->offers[k] = list->offers[k + 1];
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
