#ifndef ALCHEMY_EVENT_WORK_H
#define ALCHEMY_EVENT_WORK_H

/// @file
/// EventWork, the running scene's event work.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

struct ScenePlacement;
struct SceneEvent;
struct FieldActor;
struct ObjectRuntime;

/// The running scene's event work: its placement and event tables, its
/// actors, the triggers and exit it has hit, encounters, transitions and the
/// message on show.
struct EventWork {
  struct ScenePlacement *tables[4];
  struct SceneEvent *events;
  struct FieldActor *actors[8];
  struct FieldActor *placedActors[58];
  u8 unk11c[0x50];
  s16 touchedTrigger;
  u8 unk16e[2];
  u16 requestedExit;
  u16 unk172;
  u8 unk174[6];
  s16 facingTrigger;
  u16 encounterEnemyGroup; ///< the next battle's enemy group, 0 for none
  s16 psynergyRequest;
  u8 unk180[2];
  s16 raisedTrigger;
  u8 unk184[0x18];
  s16 blockedSteps;
  s16 mode;
  u8 encounterGroups[8];
  s32 encounterBias;
  s32 encounterThreshold;
  u8 unk1b0[0x10];
  s32 startTransition;
  u8 unk1c4[4];
  s32 transitionFrames;
  u8 unk1cc[0x0c];
  u16 message;
  u8 unk1da[6];
  struct FieldActor *viewCenter;
  u8 unk1e4[0xade];
  u16 choiceMessage; ///< Message containing the choices.
  u16 unkcc4;
};

STATIC_ASSERT(offsetof(struct EventWork, placedActors) == 0x34);
STATIC_ASSERT(offsetof(struct EventWork, touchedTrigger) == 0x16c);
STATIC_ASSERT(offsetof(struct EventWork, requestedExit) == 0x170);
STATIC_ASSERT(offsetof(struct EventWork, facingTrigger) == 0x17a);
STATIC_ASSERT(offsetof(struct EventWork, encounterEnemyGroup) == 0x17c);
STATIC_ASSERT(offsetof(struct EventWork, psynergyRequest) == 0x17e);
STATIC_ASSERT(offsetof(struct EventWork, raisedTrigger) == 0x182);
STATIC_ASSERT(offsetof(struct EventWork, blockedSteps) == 0x19c);
STATIC_ASSERT(offsetof(struct EventWork, mode) == 0x19e);
STATIC_ASSERT(offsetof(struct EventWork, encounterGroups) == 0x1a0);
STATIC_ASSERT(offsetof(struct EventWork, encounterBias) == 0x1a8);
STATIC_ASSERT(offsetof(struct EventWork, encounterThreshold) == 0x1ac);
STATIC_ASSERT(offsetof(struct EventWork, startTransition) == 0x1c0);
STATIC_ASSERT(offsetof(struct EventWork, transitionFrames) == 0x1c8);
STATIC_ASSERT(offsetof(struct EventWork, message) == 0x1d8);
STATIC_ASSERT(offsetof(struct EventWork, viewCenter) == 0x1e0);
STATIC_ASSERT(offsetof(struct EventWork, choiceMessage) == 0xcc2);

#endif
