// Draft, differ 200 of 247 (tla-en): receiving clear data over the link
// cable: waits for a peer with a matching signature, then the transfer, and
// copies the received flags and units in. Left: the native keeps the link
// mask in r5 and orders the signature stores inside the wait loop.
#include "types.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "game/edition.h"

extern volatile u16 gLinkStatus;
extern u16 gLinkPeerSignatures[][12];
extern volatile u16 gSerialReceivedSize;
extern s32 gSerialReceiveDest;
extern u32 gInput[];
extern u16 gData_02003a74[4];
extern u8 gGameFlagBytes[];
extern u8 Data_02012f94[];
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
  s32 message = MSGNO(MsgFieldPreparingToReceive);

  window = Window_Create(RECEIVE_WINDOW_X, 5, RECEIVE_WINDOW_WIDTH, 8, 2);
  Text_DrawMessage(message, window, 0, 4);
  Text_DrawMessage(message + 1, window, 0, 16);
  Text_DrawMessage(message + 3, window, 0, 36);
  Func_02002220();
  wait(10);
wait:
  while (!(gInput[1] & 2)) {
    if ((gLinkStatus & 3) == 3) {
      u16 *signature = gLinkPeerSignatures[((REG_SIOCNT << 26) >> 30) ^ 1];

      if (signature[0] == 48 && signature[1] == 48 && signature[2] == 48 &&
          signature[3] == 48) {
        gData_02003a74[0] = 'S';
        gData_02003a74[1] = 'V';
        gData_02003a74[2] = 'T';
        gData_02003a74[3] = 'S';
        goto receive;
      }
    }
    wait(1);
  }
  Func_02002408(113);
  goto close;
receive:
  wait(10);
  Serial_StartReceive(Data_02012f94 + 0x6c);
  wait(10);
  ok = 1;
  shown = 0;
  idle = 0;
  for (i = 0; i <= 0x927bf; i++) {
    if (!shown && gSerialReceivedSize != 0) {
      shown = 1;
      Window_Close(window, 2);
      window = Window_Create(5, 10, 20, 4, 2);
      Text_DrawMessage(MSGNO(MsgFieldTransferringData), window, 0, 4);
    }
    if (i == 9 && gSerialReceivedSize == 0) {
      gData_02003a74[0] = 0;
      gData_02003a74[1] = 0;
      gData_02003a74[2] = 0;
      gData_02003a74[3] = 0;
      Func_02002228();
      wait(10);
      Func_02002220();
      wait(10);
      goto wait;
    }
    idle++;
    if ((gLinkStatus & 3) == 3)
      idle = 0;
    if (idle == 10) {
      ok = 0;
      break;
    }
    if (gSerialReceiveDest == 0)
      break;
    wait(1);
  }
  if (!ok) {
    WindowSprite_PrepareForRedraw(window);
    Text_DrawMessage(MSGNO(MsgFieldDataTransferWasInterrupted), window, 0,
                        4);
    do
      wait(1);
    while (!(gInput[1] & 1));
  close:
    Window_Close(window, 2);
    return -1;
  }
  wait(10);
  Func_02002228();
  wait(10);
  Window_Close(window, 2);
  Iwram_CopyWords(gGameFlagBytes, Data_02012f94 + 0xac, 0x214);
  Iwram_CopyWords(gGameFlagBytes + 0x220, Data_02012f94 + 0x2ac, 0xd20);
  Unit_Calculate(0);
  Unit_Calculate(1);
  Unit_Calculate(2);
  Unit_Calculate(3);
  Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
  Func_02002310(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  return 1;
}
