// Draft, differ 134 of 185, skeleton gap 30 (tla-en, 2026-11-01)
// Overlay 6b5's Func_0200039c, the lobby's receipt of the partner's three member
// records and Djinn recovery table; ☀️'s BattleArena_ReceivePartyRecords is
// confirmed as its twin, but this is not the same source: that C compiled for
// tla-en stays at differ 182. What moved it here:
// - The prefix scan and the copy are goto loops (the native's index stays and
//   is shifted in the body); the shift stays a real for, which loop.c reduces
//   as the native does. The copy walks `destination` by hand: h1 to h3, a
//   for, while or do of the same, are 142 to 166.
// - `received` is set straight after each Func_0200193c: the native loads the
//   address before `cmp r0, #0`, for both arms. No hoisting pass gives that.
// - `failure` is one label here. The native's single -1 block is more likely
//   jump2's cross-jump of separate `result = -1` sets, which merge only once
//   result is in a low register (the Broken Seal's, with result high, do not).
//   Written that way, or with Heap_Release and return at each site, the gap
//   grows until the allocation below is right; retry them then.
// Left is allocation. The native's order is result r7, slot r8, timeout r10,
// heap r9, size r11; the draft's is timeout r7, slot r8, result r10, size r9,
// heap r11. global.c's priorities, floor_log2(refs) * refs / live from the lreg
// dump, are timeout 0.40, slot 0.178, result 0.10, size 0.09, heap 0.07.
// Repeating the failure sites lifts result to 0.255 and heap to 0.214, but
// nothing tried brings timeout under slot, which needs live 494 against 218 or
// about 10 refs against 22, and its 22 are set by the final CFG. So timeout
// probably loses r7 another way, a hard register preference through a copy with
// a local-alloc pseudo or similar, not priority; find_reg's regs_someone_prefers
// is the place to read. Unfound.
// The frame address: the native has `mov r1, sp` for the argument, `mov r2, sp`
// for the scan and `mov r3, sp` inside the copy's body, so nothing crosses the
// call or the shift. Here one pseudo in r5 serves all three (PRE unifies them).
// Tried: a do { } while (0) round the call (skeleton 28, differ unchanged, so
// taken back out as an untagged one-pass device), a local set in the copy's
// body, *(namePrefix + n), a byte-offset spelling, the scan without `prefix`,
// `prefix` in an inner block: none better.
#include "types.h"
#include "edition.h"
#include "serial.h"

void *Unit_Get(s32 owner);
void *Djinn_GetRecoveryTable(s32 which);
s32 Func_0200193c(void);
void Message_DecodeRawUnbounded(s32 message, u16 *buffer);
void wait(s32 frames);

extern const u8 MsgEnemyLabel[];

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LOBBY_PREFIX_LIMIT 7
#else
#define LOBBY_PREFIX_LIMIT 4
#endif

s32 Func_0200039c(void) {
  u16 namePrefix[24];
  u32 size;
  void *heap;
  s32 result;
  s32 slot;
  s32 timeout;
  s32 tries;
  u8 *record;
  s32 transferResult;
  s32 prefixLength;
  s32 nameIndex;
  u16 *prefix;
  u8 *destination;
  s32 active;
  volatile u16 *received;

  size = 0x154;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = 900;
  slot = 0;
  goto next;
waitRecord:
  if (*received > size) {
    goto failure;
  }
  wait(1);
  if (--timeout < 0 || (gLinkStatus & 3) != 3) {
    if (++tries > 24) {
      goto failure;
    }
  }
testRecord:
  active = Func_0200193c();
  received = &gSerialReceivedSize;
  if (active != 0) {
    goto waitRecord;
  }
  if (*received != size) {
    goto failure;
  }
  if (record[0x12a] != 0) {
    result++;
  }
  wait(2);
  Message_DecodeRawUnbounded((s32)MsgEnemyLabel, namePrefix);
  prefix = namePrefix;
  nameIndex = 0;
  if (prefix[nameIndex] != 0) {
  scan:
    nameIndex++;
    if (nameIndex <= LOBBY_PREFIX_LIMIT && prefix[nameIndex] != 0)
      goto scan;
  }
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  namePrefix[nameIndex] = ' ';
  prefixLength = nameIndex + 1;
#else
  prefixLength = nameIndex;
#endif
  for (nameIndex = 14; nameIndex >= prefixLength; nameIndex--) {
    record[nameIndex] = record[nameIndex - prefixLength];
  }
  nameIndex = 0;
  if (nameIndex < prefixLength) {
    destination = record;
  copy:
    *destination = namePrefix[nameIndex];
    nameIndex++;
    destination++;
    if (nameIndex < prefixLength)
      goto copy;
  }
  record[14] = 0;
  slot++;
next:
  if (slot > 2) {
    goto second;
  }
  record = (u8 *)Unit_Get(slot + 128);
  tries = 0;
  if ((transferResult = Serial_StartReceive(record)) == -1) {
    goto failed;
  }
  goto testRecord;
waitRecovery:
  transferResult = *received;
  if (transferResult > 0x154) {
    goto failure;
  }
  wait(1);
  if (--timeout < 0 || (gLinkStatus & 3) != 3) {
    if (++tries > 24) {
      goto failure;
    }
  }
testRecovery:
  active = Func_0200193c();
  received = &gSerialReceivedSize;
  if (active != 0) {
    goto waitRecovery;
  }
  if (*received != 0x154) {
  failure:
    result = -1;
  } else {
    wait(2);
  }
  goto done;
second:
  Heap_Release(heap);
  size = 0x154;
  heap = Heap_AllocEwram(size);
  tries = 0;
  if ((transferResult =
           Serial_StartReceive(Djinn_GetRecoveryTable(1))) != -1) {
    goto testRecovery;
  }
failed:
  result = transferResult;
done:
  Heap_Release(heap);
  return result;
}
