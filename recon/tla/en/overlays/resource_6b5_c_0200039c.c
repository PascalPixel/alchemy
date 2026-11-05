// Draft, differ 50 (tla-en, 2026-11-05). Func_0200039c of overlay 6b5.
// The whole layout and the frame now match: the receive loop's top is moved
// to its bottom by the two break exits, and the same in the one-shot recovery
// loop. Direct result = -1 sites (cross-jumped back to one) rank result above
// slot, as natively; Call2 on the decoder stops the sp address being shared.
// Left: scratch r2 and r3 swapped, from the hoisted gSerialReceivedSize
// address being in r2 where the native has r3 and the head's order.
#include "types.h"
#include "edition.h"
#include "serial.h"
#include "call.h"

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
  s32 result;
  u32 size;
  void *heap;
  s32 timeout;
  s32 slot;
  s32 tries;
  s32 transfer;
  s32 i;
  s32 length;
  u8 *record;

  size = 0x154;
  heap = Heap_AllocEwram(size);
  timeout = 900;
  result = 0;
  for (slot = 0;; slot++) {
    if (slot > 2)
      goto second;
    record = Unit_Get(slot + 128);
    transfer = Serial_StartReceive(record);
    tries = 0;
    if (transfer == -1)
      break;
    while (Func_0200193c() != 0) {
      if (gSerialReceivedSize > size)
        {
        result = -1;
        goto done;
      }
      wait(1);
      if (--timeout < 0 || (gLinkStatus & 3) != 3) {
        if (++tries > 24)
          {
        result = -1;
        goto done;
      }
      }
    }
    if (gSerialReceivedSize != size)
      {
        result = -1;
        goto done;
      }
    if (record[0x12a] != 0)
      result++;
    wait(2);
    Call2((void (*)())Message_DecodeRawUnbounded, (s32)MsgEnemyLabel, namePrefix);
    for (i = 0; i <= LOBBY_PREFIX_LIMIT && namePrefix[i] != 0; i++)
      ;
#if LOBBY_PREFIX_LIMIT == 7
    namePrefix[i] = 32;
    length = i + 1;
#else
    length = i;
#endif
    for (i = 14; i >= length; i--)
      record[i] = record[i - length];
    for (i = 0; i < length; i++)
      record[i] = namePrefix[i];
    record[14] = 0;
  }
  goto failed;
  for (;;) {
  second:
    Heap_Release(heap);
    size = 0x154;
    heap = Heap_AllocEwram(size);
    transfer = Serial_StartReceive(Djinn_GetRecoveryTable(1));
    tries = 0;
    if (transfer == -1)
      break;
    while (Func_0200193c() != 0) {
      if (gSerialReceivedSize > 0x154)
        {
        result = -1;
        goto done;
      }
      wait(1);
      if (--timeout < 0 || (gLinkStatus & 3) != 3) {
        if (++tries > 24)
          {
        result = -1;
        goto done;
      }
      }
    }
    if (gSerialReceivedSize != 0x154)
      result = -1;
    else
      wait(2);
    goto done;
  }
failed:
  result = transfer;
done:
  Heap_Release(heap);
  return result;
}
