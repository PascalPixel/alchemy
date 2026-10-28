#include "serial_runtime.h"

s32 Serial_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialWork.phase) {
  case 0: {
    u8 mask30 = control & 0x30;
    u8 mode;

    if (mask30 == 0 && (mode = control & 0x88) == 8) {
      u8 serialError = control & 4;
      s32 index;

      if (serialError == 0) {
        index = gSerialWork.sendIndex;
        /* FAKEMATCH: keeps the send-index load ahead of the -1, as in
         * Serial_PollStatus; without the barrier the -1 is started first
         * (differ 2). */
        asm("");
        if (index == -1) {
          u32 ie;

          /* FAKEMATCH: the do/while blocks and the separate enable
           * variable are meaningless, as in Serial_PollStatus. */
          do {
            REG_IME = serialError;
          } while (0);
          ie = REG_IE;
          do {
            ie &= ~0x80;
            ie |= 0x40;
            REG_IE = ie;
            {
              u8 enable = 1;
              REG_IME = enable;
            }
            *((volatile s8 *)siocnt + 1) &= ~0x40;
            REG_IF = 0xc0;
            REG_TM3CNT = 0xc963;
          } while (0);
          gSerialWork.mode = mode;
        }
      }
    } else if (mask30 == 0) {
      break;
    }
    gSerialWork.phase = 1;
  }
  case 1:
    Serial_CollectReceivedPayloads(receive);
    Serial_PrepareSendPacket(send);
    break;
  }

  gSerialWork.packetNumber++;
  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == 8)
    status |= 0x80;
  result = Serial_AddErrorFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}

s32 Serial_PollStatus(void) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  if (gSerialWork.phase == 0) {
    u8 mode = control & 0x88;

    if (mode == 8) {
      u8 serialError = control & 4;

      if (serialError == 0) {
        s32 index = gSerialWork.sendIndex;

        /* FAKEMATCH: keeps the send-index load ahead of the -1. */
        asm("");
        if (index == -1) {
          u32 ie;

          /* FAKEMATCH: one-pass blocks preserve IE/IME load order; the enable
           * local derives the mask from r3. */
          do {
            REG_IME = serialError;
          } while (0);
          ie = REG_IE;
          do {
            ie &= ~0x80;
            ie |= 0x40;
            REG_IE = ie;
            {
              u8 enable = 1;
              REG_IME = enable;
            }
            *((volatile s8 *)siocnt + 1) &= ~0x40;
            REG_IF = 0xc0;
            REG_TM3CNT = 0xc963;
          } while (0);
          gSerialWork.mode = mode;
        }
      }
      gSerialWork.phase = 1;
    }
    gSerialWork.packetNumber++;
  }

  status = gSerialWork.currentMask | (gSerialWork.receivedMask << 8);
  if (gSerialWork.mode == 8)
    status |= 0x80;
  result = Serial_AddErrorFlag(status);
  if (((control << 26) >> 30) > 1)
    result |= 0x2000;
  return result;
}
