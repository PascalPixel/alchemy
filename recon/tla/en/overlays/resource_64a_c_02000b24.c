// Draft, differ 200 of 247 (tla-en): receiving clear data over the link
// cable: waits for a peer with a matching signature, then the transfer, and
// copies the received flags and units in. Left: the native keeps the link
// mask in r5 and orders the signature stores inside the wait loop.
#include "types.h"
#include "party_state.h"
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

void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void UiText_DrawResource(s32 message, void *window, s32 x, s32 y);
void UiWork_Finalize(void *window, s32 release);
void RenderOutput_PrepareForRedraw(void *window);
void WaitTicks(s32 ticks);
s32 SerialRuntime_StartReceive(void *destination);
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

  window = UiWindow_Create(RECEIVE_WINDOW_X, 5, RECEIVE_WINDOW_WIDTH, 8, 2);
  UiText_DrawResource(message, window, 0, 4);
  UiText_DrawResource(message + 1, window, 0, 16);
  UiText_DrawResource(message + 3, window, 0, 36);
  Func_02002220();
  WaitTicks(10);
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
    WaitTicks(1);
  }
  Func_02002408(113);
  goto close;
receive:
  WaitTicks(10);
  SerialRuntime_StartReceive(Data_02012f94 + 0x6c);
  WaitTicks(10);
  ok = 1;
  shown = 0;
  idle = 0;
  for (i = 0; i <= 0x927bf; i++) {
    if (!shown && gSerialReceivedSize != 0) {
      shown = 1;
      UiWork_Finalize(window, 2);
      window = UiWindow_Create(5, 10, 20, 4, 2);
      UiText_DrawResource(MSGNO(MsgFieldTransferringData), window, 0, 4);
    }
    if (i == 9 && gSerialReceivedSize == 0) {
      gData_02003a74[0] = 0;
      gData_02003a74[1] = 0;
      gData_02003a74[2] = 0;
      gData_02003a74[3] = 0;
      Func_02002228();
      WaitTicks(10);
      Func_02002220();
      WaitTicks(10);
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
    WaitTicks(1);
  }
  if (!ok) {
    RenderOutput_PrepareForRedraw(window);
    UiText_DrawResource(MSGNO(MsgFieldDataTransferWasInterrupted), window, 0,
                        4);
    do
      WaitTicks(1);
    while (!(gInput[1] & 1));
  close:
    UiWork_Finalize(window, 2);
    return -1;
  }
  WaitTicks(10);
  Func_02002228();
  WaitTicks(10);
  UiWork_Finalize(window, 2);
  Iwram_CopyWords(gGameFlagBytes, Data_02012f94 + 0xac, 0x214);
  Iwram_CopyWords(gGameFlagBytes + 0x220, Data_02012f94 + 0x2ac, 0xd20);
  Unit_Calculate(0);
  Unit_Calculate(1);
  Unit_Calculate(2);
  Unit_Calculate(3);
  Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
  Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  return 1;
}
