// Draft, differ 20 of 247 (tla-en, 2026-11-05).
// Draft, differ 30 of 247 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 39 of 247 (tla-en); older figures below predate it.
// Draft, differ 56 of 247, skeleton gap 5/7 (tla-en, 2026-11-01)
// Receiving clear data over the link cable: waits for a peer with a matching
// signature, then the transfer, and copies the received flags and units in.
// Every loop is built from labels and goto: real loops let loop.c move the
// signature and retry blocks out of line (their exit jumps), copy the first
// test to the top, and hoist gInput's address. A real for also lets flow merge
// the receive block into the wait loop. The signature stores go through a
// pointer because the array store expands as a bit-field store and folds to
// SImode constants (movs); the HImode constant is what loads from the pool, at
// range 64, which places the mid-function pool. The masks are locals assigned
// before each loop.
// Left, the skeleton: the native reloads 0x02013040 from the pool for the third
// copy; here the argument's pseudo is reused from the first, kept in r8. Only a
// CSE block end between them would stop that, and a do { } while (0) macro
// emits none. Also r2/r3 swapped at the zero stores, and one scheduling pair at
// the counters' setup. Declaration order changes nothing; idle's place in the
// setup gives the native's i, window and idle allocation.
// Before adoption: linked names for the Ram_ addresses, and the two window
// macros for French and Spanish; 0x55 in tla-en is listed as MsgFieldRepelEvil
// only by coincidence of value.
#include "types.h"
#include "call.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "edition.h"

extern volatile u16 gLinkStatus;
extern u16 gLinkPeerSignatures[][12];
extern u16 gSerialReceivedSize;
extern s32 gSerialReceiveDest;
extern u32 gInput[];
extern u16 gData_02003a74[4];
extern u8 gGameFlagBytes[];
#define Ram_MapCellBackup FIXED_ADDRESS((u8 *), 0x02013000, gMapCellBackup)
#define Ram_ReceivedFlags FIXED_ADDRESS((u8 *), 0x02013040, gReceivedFlags)
#define Ram_ReceivedUnits FIXED_ADDRESS((u8 *), 0x02013240, gReceivedUnits)
extern char MsgFieldPreparingToReceive[];
extern char MsgFieldTransferringData[];
extern char MsgFieldDataTransferWasInterrupted[];

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);
void Window_Close(void *window, s32 release);
void WindowSprite_PrepareForRedraw(void *window);
void wait(s32 frames);
s32 Serial_StartReceive(void *destination);
void Unit_Calculate(s32 id);
void Func_02002220(void);
void Func_02002228(void);
void Func_02002310(s32 hue, s32 brightness);
void Func_02002408(s32 cue);

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define REQUEST_EDITION 'C'
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define REQUEST_EDITION 'E'
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define REQUEST_EDITION 'S'
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define REQUEST_EDITION 'F'
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define REQUEST_EDITION 'I'
#else
#define REQUEST_EDITION 'U'
#endif

#define MSGNO(name) ((s32)(name))

#if (LANGUAGE == LANGUAGE_GERMAN)
#define RECEIVE_WINDOW_X 5
#define RECEIVE_WINDOW_WIDTH 20
#else
#define RECEIVE_WINDOW_X 6
#define RECEIVE_WINDOW_WIDTH 18
#endif

s32 Func_02000b24(void) {
  void *window;
  s32 i, idle, ok, shown;
  s32 mask, linkMask;
  s32 message;
  u8 *flags;
  s32 (*copyWords)(void *destination, const void *source, s32 size);

  window = Window_Create(RECEIVE_WINDOW_X, 5, RECEIVE_WINDOW_WIDTH, 8, 2);
  message = MSGNO(MsgFieldPreparingToReceive);
  Text_DrawMessage(message, window, 0, 4);
  Text_DrawMessage(message + 1, window, 0, 16);
  Text_DrawMessage(message + 3, window, 0, 36);
  Func_02002220();
  wait(10);
restart:
  mask = 3;
  goto poll;
scan:
  if ((gLinkStatus & mask) == mask) {
    u16 *signature = gLinkPeerSignatures[((REG_SIOCNT << 26) >> 30) ^ 1];

    if (signature[0] == 48 && signature[1] == 48 && signature[2] == 48 &&
        signature[3] == 48) {
      u16 *request = gData_02003a74;

      request[0] = REQUEST_EDITION;
      request[1] = 'V';
      request[2] = 'T';
      request[3] = 'S';
      goto receive;
    }
  }
  wait(1);
poll:
  if (!(gInput[1] & 2))
    goto scan;
  Func_02002408(113);
  goto close;
receive:
  wait(10);
  Serial_StartReceive(Ram_MapCellBackup);
  wait(10);
  linkMask = 3;
  idle = 0;
  ok = 1;
  shown = 0;
  i = 0;
transfer:
  if (!shown && gSerialReceivedSize != 0) {
    shown = 1;
    Window_Close(window, 2);
    window = Window_Create(5, 10, 20, 4, 2);
    Text_DrawMessage(MSGNO(MsgFieldTransferringData), window, 0, 4);
  }
  if (i == 9 && gSerialReceivedSize == 0) {
    u16 *request = gData_02003a74;

    request[0] = 0;
    request[1] = 0;
    request[2] = 0;
    request[3] = 0;
    Func_02002228();
    wait(10);
    Func_02002220();
    wait(10);
    goto restart;
  }
  idle++;
  if ((gLinkStatus & linkMask) == linkMask)
    idle = 0;
  if (idle == 10) {
    ok = 0;
    goto done;
  }
  if (gSerialReceiveDest != 0) {
    wait(1);
    i++;
    if (i < 600000)
      goto transfer;
  }
done:
  if (!ok) {
    WindowSprite_PrepareForRedraw(window);
    Text_DrawMessage(MSGNO(MsgFieldDataTransferWasInterrupted), window, 0, 4);
  interrupted:
    wait(1);
    if (!(gInput[1] & 1))
      goto interrupted;
  close:
    Window_Close(window, 2);
    return -1;
  }
  wait(10);
  Func_02002228();
  wait(10);
  Window_Close(window, 2);
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 32 of 247 instructions differ
   * in tla-en. */
  do {
    flags = gGameFlagBytes;
  } while (0);
  copyWords = Iwram_CopyWords;
  Call3((void (*)())copyWords, flags, Ram_ReceivedFlags, 0x214);
  copyWords(flags + 0x220, Ram_ReceivedUnits, 0xd20);
  Unit_Calculate(0);
  Unit_Calculate(1);
  Unit_Calculate(2);
  Unit_Calculate(3);
  Call3((void (*)())copyWords, Ram_ReceivedFlags, flags, 0xf40);
  Func_02002310(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  return 1;
}
