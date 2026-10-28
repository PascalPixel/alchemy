// Draft of overlay 6b5's Func_02000578, the lobby's sending of the party
// records and the Djinn recovery table, after ☀️'s BattleArena_SendPartyRecords
// with the record loops as the listing shows them: differ 258 of 264. Left:
// register allocation (the native keeps the result in fp and spills the member
// count, where this spills the result), and the slot map's address stored
// afresh in each turn of the clearing loop and read from its register.
#include "types.h"
#include "iwram_call.h"
#include "mem.h"
#include "owner_state.h"
#include "serial_runtime.h"

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
  struct TradeOfferState *list;

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
  send:
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

    list = (struct TradeOfferState *)heap;
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
