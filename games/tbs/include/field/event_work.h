#ifndef ALCHEMY_EVENT_WORK_H
#define ALCHEMY_EVENT_WORK_H

/// @file
/// EventWork, the running scene's event work.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

struct ObjectEventTemplate;
struct MapEvent;
struct ObjectEvent;
struct ObjectRuntime;

/// The running scene's event work: its placement and event tables, its
/// actors, the triggers and exit it has hit, encounters, transitions and the
/// message on show. It is struct FieldWork (heap slot 27) as the scenes see
/// it, through gEventWork; each field is the FieldWork field at its offset.
struct EventWork {
  struct ObjectEventTemplate *tables[4]; ///< the placement tables placed so far
  struct MapEvent *events;               ///< the scene's event table
  struct ObjectEvent *objectEvents[8];   ///< the party's characters, ids 0 to 7
  struct ObjectEvent *placedObjectEvents[58]; ///< the placed actors, from id 8
  u8 unk11c[0x50];    ///< FieldWork's markers; not named by TBS code
  s16 touchedTrigger; ///< the step event a touch or exit event answers
  u8 unk16e[2];       ///< FieldWork's pushTile request; not named by TBS code
  u16 requestedWarp;  ///< the exit kind taken, which ends the scene
  /// FieldWork's menu: 1 opens the main menu once the event ends, so that
  /// the item the place asks for can be used there.
  u16 requestedMenu;
  u8 unk174[6];            ///< FieldWork's examine, start and talk
  s16 facingTrigger;       ///< the collision code ahead, which a search answers
  u16 encounterEnemyGroup; ///< the next battle's enemy group, 0 for none
  s16 psynergyRequest;   ///< a field Psynergy to use, as owner << 10 | Psynergy
  u8 unk180[2];          ///< FieldWork's item request; not named by TBS code
  s16 raisedTrigger;     ///< a raised event for Map_RunRaisedEvent
  u8 unk184[0x18];       ///< FieldWork's fallen characters and item slot
  s16 blockedSteps;      ///< frames the leader has pushed against something
  s16 mode;              ///< an enum ObjectSystemMode
  u8 encounterGroups[8]; ///< encounter zone for each kind of ground
  s32 encounterBias;     ///< randomized distance adjustment between battles
  s32 encounterThreshold; ///< encounter steps before a battle
  u8 unk1b0[0x10];        ///< FieldWork's walking distance and tiles
  s32 startTransition;    ///< MAP_TRANSITION(style, variant)
  u8 unk1c4[4];           ///< FieldWork's unk1c4 and transitionActive
  s32 transitionFrames;   ///< frames
  u8 unk1cc[0x0c];        ///< FieldWork's fastForward and unk1d0
  u16 message;            ///< the next message, advanced as each window opens
  u8 unk1da[6];           ///< FieldWork's unk1da to unk1de
  struct ObjectEvent *viewCenter; ///< the object the map view follows
  u8 unk1e4[0x10];                ///< not named by TBS code through this view
  /// The actor speaking; the yes/no choice closes the portrait window of its
  /// sprite once answered (FieldScript_AskYesNoWithObjectEventAnimation).
  s32 speaker;
  u8 unk1f8[0xaca]; ///< not named by TBS code through this view
  /// FieldWork's nameMessage: the first of the two names the yes/no choice
  /// shows in place of Yes and No, 0 for those.
  u16 choiceMessage;
  u16 choiceWidth; ///< FieldWork's nameWidth: its name window's width, 0 for 3
};

STATIC_ASSERT(offsetof(struct EventWork, placedObjectEvents) == 0x34);
STATIC_ASSERT(offsetof(struct EventWork, touchedTrigger) == 0x16c);
STATIC_ASSERT(offsetof(struct EventWork, requestedWarp) == 0x170);
STATIC_ASSERT(offsetof(struct EventWork, requestedMenu) == 0x172);
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
STATIC_ASSERT(offsetof(struct EventWork, speaker) == 0x1f4);
STATIC_ASSERT(offsetof(struct EventWork, choiceMessage) == 0xcc2);
STATIC_ASSERT(offsetof(struct EventWork, choiceWidth) == 0xcc4);

#endif
