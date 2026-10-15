#ifndef ALCHEMY_EVENT_RUNTIME_H
#define ALCHEMY_EVENT_RUNTIME_H

#include "types.h"

struct EventPairWork1d6 {
  u8 unk000[0x1d6];
  u16 special;
  u8 unk1d8[0x1c];
  u32 objectId;
};

struct EventRuntime {
  u8 unk000[0x170];
  u16 value170;
  s16 unk172[5];
  u16 value17c;
  u8 unk17e[0x20];
  s16 mode19e;
  u8 unk1a0[0x20];
  s32 value1c0;
  u16 value1c4;
  u16 status1c6;
  s32 value1c8;
  s32 messageBusy;
  u8 unk1d0[0x08];
  s16 message;
  u8 unk1da[0x1a];
  s32 speaker;
  s32 messageWindow;
  s32 sideWindow;
};

struct WorkPointers {
  u8 *window;
  u8 unk04[0x2c];
  struct EventRuntime *event;
};

#endif
