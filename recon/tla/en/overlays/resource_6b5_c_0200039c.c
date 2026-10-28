// Draft of overlay 6b5's Func_0200039c, the lobby's receipt of the partner's
// three member records and Djinn recovery table, ported from ☀️'s
// BattleArena_ReceivePartyRecords with the prefix loops as goto loops: differ
// 184 of 185 (German to Italian 195 of 191, with the space after the prefix).
// Left: register allocation throughout (the native keeps the record size in fp,
// the heap block in r9, the timeout in sl and the result in r7), and the prefix
// scan indexes namePrefix where this walks a pointer.
#include "types.h"
#include "edition.h"
#include "serial_runtime.h"

void *Unit_Get(s32 owner);
void *Djinn_GetRecoveryTable(s32 which);
s32 Func_0200193c(void);
void Message_DecodeRawUnbounded(s32 message, u16 *buffer);
void WaitFrames(s32 frames);

extern const u8 MsgEnemyLabel[];

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

  size = 0x154;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = 900;
  slot = 0;
  goto next;
waitRecord:
  if (gSerialReceivedSize > size) {
    result = -1;
    goto done;
  }
  WaitFrames(1);
  if (--timeout < 0 || (gLinkStatus & 3) != 3) {
    if (++tries > 24) {
      result = -1;
      goto done;
    }
  }
testRecord:
  if (Func_0200193c() != 0) {
    goto waitRecord;
  }
  if (gSerialReceivedSize != size) {
    result = -1;
    goto done;
  }
  if (record[0x12a] != 0) {
    result++;
  }
  WaitFrames(2);
  Message_DecodeRawUnbounded((s32)MsgEnemyLabel, namePrefix);
  nameIndex = 0;
  if (namePrefix[nameIndex] != 0) {
  scan:
    nameIndex++;
    if (nameIndex <= 4 && namePrefix[nameIndex] != 0)
      goto scan;
  }
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  namePrefix[nameIndex] = ' ';
  prefixLength = nameIndex + 1;
#else
  prefixLength = nameIndex;
#endif
  nameIndex = 14;
  if (nameIndex >= prefixLength) {
  shift:
    record[nameIndex] = record[nameIndex - prefixLength];
    nameIndex--;
    if (nameIndex >= prefixLength)
      goto shift;
  }
  nameIndex = 0;
  if (nameIndex < prefixLength) {
  copy:
    record[nameIndex] = namePrefix[nameIndex];
    nameIndex++;
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
  transferResult = gSerialReceivedSize;
  if (transferResult > 0x154) {
    result = -1;
    goto done;
  }
  WaitFrames(1);
  if (--timeout < 0 || (gLinkStatus & 3) != 3) {
    if (++tries > 24) {
      result = -1;
      goto done;
    }
  }
testRecovery:
  if (Func_0200193c() != 0) {
    goto waitRecovery;
  }
  if (gSerialReceivedSize != 0x154) {
    result = -1;
  } else {
    WaitFrames(2);
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
