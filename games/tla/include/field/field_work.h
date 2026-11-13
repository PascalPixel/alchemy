#ifndef ALCHEMY_FIELD_WORK_H
#define ALCHEMY_FIELD_WORK_H

/// @file
/// What games/tla/src/field/field_sequence/ declares for the files that use it,
/// written once.

#include "types.h"

struct FieldWork {
  u8 unk000[16];
  const void *events;
  u8 unk014[0x140];
  s16 coordEvent;
  s16 pushCell;
  s16 warp;
  s16 menu;
  s16 examine;
  s16 start;
  s16 talk;
  s16 searchCell;
  s16 battle;
  s16 psynergy;
  s16 item;
  s16 raisedEvent;
  s16 fallenCount;
  s16 survivorCount;
  s16 fallenOwners[9];
  s16 itemArgument;
  u8 unk184[2];
  u16 keys;
  u8 unk188[2];
  s8 eventMode;
  u8 unk18b[9];
  s32 unk194;
  u8 unk198[4];
  s32 walkingScale;
  s32 walkingDistance;
  u8 unk1a4[4];
  void *previousCell;
  s32 transition;
  u8 unk1b0[2];
  u16 transitionActive;
  s32 transitionFrames;
  u8 unk1b8[0xaea];
  s16 busy;
  u8 unkca4;
  u8 effectTimerRunning;
  u8 unkca6[0x12];
  s16 queuedSound;
  u8 unkcba[0x7c];
  u8 sceneAttributeLow;
  u8 sceneAttributeHigh;
  u8 unkd38[0x18];
};

#endif
