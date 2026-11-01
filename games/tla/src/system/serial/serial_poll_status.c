#include "serial.h"
#include "io_write.h"

s32 Serial_PollAndTransfer(void *send, void *receive) {
  volatile u32 *siocnt;
  u32 control;
  s32 status;
  s32 result;

  siocnt = &REG_SIOCNT;
  control = *siocnt;
  switch (gSerialWork.phase) {
  case 0: {
    u8 multiplayerId = control & 0x30;
    u8 mode;

    if (multiplayerId == 0 && (mode = control & 0x88) == 8) {
      u8 serialError = control & 4;

      if (serialError == 0 && gSerialWork.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~0x80) | 0x40, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = 0xc0;
        REG_TM3CNT = 0xc963;
        gSerialWork.mode = mode;
      }
    } else if (multiplayerId == 0) {
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

      if (serialError == 0 && gSerialWork.sendIndex == -1) {
        u32 ie;

        Io_Write16(0, &REG_IME);
        ie = REG_IE;
        Io_Write16((ie & ~0x80) | 0x40, &REG_IE);
        Io_Write16(1, &REG_IME);
        *((volatile s8 *)siocnt + 1) &= ~0x40;
        REG_IF = 0xc0;
        REG_TM3CNT = 0xc963;
        gSerialWork.mode = mode;
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
