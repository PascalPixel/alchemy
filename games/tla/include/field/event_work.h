#ifndef ALCHEMY_EVENT_WORK_H
#define ALCHEMY_EVENT_WORK_H

#include "types.h"
#include "edition.h"
#include "static_assert.h"

struct ActorPlacement;
struct Trigger;
struct FieldActor;
struct Actor;

struct EventWork {
  u8 unk000[0x14];
  struct Actor *objects[80];
  s16 exit;
  u8 unk156[2];
  s16 countdown;
  u16 unk15a;
  u8 unk15c[8];
  u16 encounterEnemyGroup; ///< the next battle's enemy group, 0 for none
  s16 unk166;
  u8 unk168[2];
  u16 unk16a;
  u8 unk16c[0x18];
  s16 blockedSteps; ///< refused key moves in a row
  u16 heldKeys;
  u8 unk188[2];
  s8 controlMode;
  u8 unk18b[0x21];
  s32 screenEffect;
  u8 unk1b0[2];
  s16 screenStatus;
  s32 delay;
  s32 waitMode;
  u8 unk1bc[8];
  u16 messageId;
  u16 messageActor;
  s16 choice;
  s16 messageState;
  struct Actor *viewCenter;
  u8 unk1d0[0x10];
  s32 currentOwner;
  s32 objectStatus;
  u8 unk1e8[0xaba];
  s16 actionCounter;
  s8 pendingFirst;
  u8 unkca5[7];
  s8 pendingSecond;
  u8 unkcad[3];
  s16 unkcb0;
  s16 unkcb2;
  s16 unkcb4;
  u8 unkcb6[2];
  s16 cueState;
};

STATIC_ASSERT(offsetof(struct EventWork, objects) == 0x14);
STATIC_ASSERT(offsetof(struct EventWork, exit) == 0x154);
STATIC_ASSERT(offsetof(struct EventWork, countdown) == 0x158);
STATIC_ASSERT(offsetof(struct EventWork, encounterEnemyGroup) == 0x164);
STATIC_ASSERT(offsetof(struct EventWork, controlMode) == 0x18a);
STATIC_ASSERT(offsetof(struct EventWork, screenEffect) == 0x1ac);
STATIC_ASSERT(offsetof(struct EventWork, delay) == 0x1b4);
STATIC_ASSERT(offsetof(struct EventWork, messageId) == 0x1c4);
STATIC_ASSERT(offsetof(struct EventWork, currentOwner) == 0x1e0);
STATIC_ASSERT(offsetof(struct EventWork, actionCounter) == 0xca2);
STATIC_ASSERT(offsetof(struct EventWork, pendingSecond) == 0xcac);
STATIC_ASSERT(offsetof(struct EventWork, cueState) == 0xcb8);

#endif
