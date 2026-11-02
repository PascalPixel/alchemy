// Draft, differ 216 of 264, skeleton gap 84 (tla-en, 2026-11-01)
// Overlay 6b5's Func_02000578, the lobby's sending of the party records and the
// Djinn recovery table, after ☀️'s BattleArena_SendPartyRecords. Confirmed twin
// by statements, but not by allocation: the twin has heap r8, i r7, timeout r6.
// The record and clear loops stay goto loops: real loops strength-reduce them
// (twin's for loops measure differ 290), which the listing does not. The j and
// k loops are real: the listing steps r0 and stmia r2! there. tries is three
// variables (r10, r5, r8 in the listing).
// Left, nearly all of it allocation. Most of the skeleton gap follows from heap
// in a high register and result on the stack: adds against mov, add r2, sl,
// adds r1, #42 (move2add needs heap low), and the shared tails that a stacked
// result or a registered memberCount lets cross-jumping make.
// Listing's order is tries2 (r5), i (r6), heap (r7), memberIds' address (r8),
// tries (r10), timeout (r9), result (r11), then memberCount and the slot address
// spilled (sp+4, sp+0). Here (--rtl, global.c's floor_log2(refs) * refs / live):
// i .69, tries .61, timeout .61, heap .43, address .226, memberCount .16 (4/50),
// result .07. So the listing wants tries and timeout below .226, about half
// their weight here. timeout's 25 refs come from each wait loop's two back edges
// to the test, each adding a depth in flow.c; those edges are in the listing
// too, so no source form lowers them. One latch per wait loop gives 19 refs, .45,
// still above heap, and needs an empty asm. Weights that count each loop once
// fit the listing better than this compiler's do: a compiler question, out of
// scope here.
// result above memberCount: update_equiv_regs doubles result's live length for
// its constant first set (344), and memberCount needs 3 refs for log 1, with
// set, guard and the loop's test weighing 4. Neither has a plain form.
// Left, hoisting. The listing keeps sp+8 in the clear loop and the j loop, and
// reads [list, 0x120 in a register] in the k loop; here gcse's PRE lifts (plus
// sfp 8) onto edge (0,1) and (plus list 0x120) out of the k guard. Entering the
// clear loop at its bottom test does stop PRE there, and keeps the address in
// the loop, but leaves a b the listing lacks, and cse2 gives slotMap a REG_EQUIV
// from its copy, so it rematerializes instead of taking sp+0 (differ 231,
// gap 83). In the j loop, sp+8 rebuilt beside sp+0 read in the record loop means
// two pseudos; peerSlots[] there measures 222, slotMap[] 216. A goto k loop
// loses (differ 227, 232); the guarded do-while gains 2 of differ and loses 5
// of skeleton (k < list->count - 1 under if, differ 214, gap 51/41).
// To retire: the u32 cast in the k loop is a cast to a type the object is not.
// It keeps the store aliasing count, so the loop rereads count as the listing
// does and counts up; list->offers[k] = list->offers[k + 1] hoists count and
// counts down (differ 237, gap 58/43), and a u32 count loses 2 and its compares
// are unsigned. No plain form found; devices in disguise may not be added, so
// this one stays only while this is a draft, and needs an answer before adoption.
// Also measured, no gain: wait loops as real while (224 to 237), one tries for
// the first two loops (234), a for record loop (254), a struct-wrapped peerSlots
// (216, unchanged), result after timeout (222), memberIds after peerSlots (220).
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
      list->offers[j].owner = slotMap[list->offers[j].owner];
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
