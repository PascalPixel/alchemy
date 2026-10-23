#ifndef ALCHEMY_EVENT_WORK_H
#define ALCHEMY_EVENT_WORK_H

/// @file
/// EventWork, the running map's event work.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

struct ObjectEventTemplate;
struct MapEvent;
struct ObjectEvent;
struct Object;
struct MapGridCell;

/// The running map's event work: its object event template and map event
/// tables, its object events, the triggers and warp it has hit, encounters,
/// transitions and the message on show. It is struct FieldWork (heap slot 27)
/// as the maps see it, through gFieldWork; each field is the FieldWork field at
/// its offset.
struct EventWork {
  struct ObjectEventTemplate
      *tables[4];          ///< the object event template tables placed so far
  struct MapEvent *events; ///< the map's event table
  struct ObjectEvent *objectEvents[8]; ///< the party's characters, ids 0 to 7
  struct ObjectEvent
      *placedObjectEvents[58]; ///< the placed object events, from local id 8
  u8 treasureSpots[0x50];      ///< FieldWork's; not named through this view
  s16 coordEvent;              ///< the step event a coord event or warp answers
  s16 pushCell;                ///< FieldWork's; not named through this view
  u16 warp;                    ///< the warp kind taken, which ends the map
  /// FieldWork's menu: 1 opens the main menu once the script ends, so that
  /// the item the place asks for can be used there.
  u16 menu;
  /// FieldWork's examine, start and talk requests; not named through this view.
  ///@{
  s16 examine;
  s16 start;
  u16 talk;
  ///@}
  s16 searchCell;  ///< the metatile behavior ahead, which a bg event answers
  u16 battle;      ///< the next battle's formation, 0 for none
  s16 psynergy;    ///< a field Psynergy to use, as unit << 10 | Psynergy
  s16 item;        ///< FieldWork's; not named through this view
  s16 raisedEvent; ///< a raised event for Map_RunRaisedEvent
  /// FieldWork's fallen characters and item slot; not named through this view.
  ///@{
  s16 fallenCount;
  s16 survivorCount;
  s16 fallenOwners[9];
  s16 itemArgument;
  ///@}
  s16 blockedSteps;       ///< frames the leader has pushed against something
  s16 mode;               ///< an enum ObjectSystemMode
  u8 encounterZones[8];   ///< encounter zone for each kind of ground
  s32 encounterBias;      ///< randomized distance adjustment between battles
  s32 encounterThreshold; ///< encounter steps before a battle
  /// FieldWork's walking distance and tiles; not named through this view.
  ///@{
  s32 walkingDistanceScale;
  s32 walkingDistance;
  struct MapGridCell *currentCell;
  struct MapGridCell *previousCell;
  ///@}
  s32 transition;               ///< MAP_TRANSITION(style, variant)
  u16 unused;                   ///< FieldWork's; no code names it
  u16 transitionActive;         ///< FieldWork's; not named through this view
  s32 transitionFrames;         ///< frames
  s32 fastForward;              ///< FieldWork's; not named through this view
  u8 unusedAfterFastForward[8]; ///< FieldWork's; no code names it
  u16 message;  ///< the next message, advanced as each window opens
  u8 unk1da[6]; ///< XXX: FieldWork's unk1da to unk1de (see the field README)
  struct ObjectEvent *camera; ///< the object the map view follows
  u8 unusedAfterCamera[0x10]; ///< FieldWork's; no code names it
  /// The object event speaking; the yes/no choice closes the portrait window of
  /// its sprite once answered (FieldScript_AskYesNoWithObjectEventAnimation).
  s32 speaker;
  /// FieldWork's windows, the leader's templates and the Reveal state; not
  /// named through this view.
  ///@{
  s32 messageWindow;
  s32 sideWindow;
  u8 leaderTemplates[0x30];
  u8 unusedAfterTemplates[0xa86];
  u16 busy;
  s16 revealActive;
  s16 revealFrames;
  s16 revealCenterX;
  s16 revealCenterY;
  s16 effectTimerRunning;
  ///@}
  /// FieldWork's nameMessage: the first of the two names the yes/no choice
  /// shows in place of Yes and No, 0 for those.
  u16 choiceMessage;
  u16 choiceWidth; ///< FieldWork's nameWidth: its name window's width, 0 for 3
};

STATIC_ASSERT(offsetof(struct EventWork, placedObjectEvents) == 0x34);
STATIC_ASSERT(offsetof(struct EventWork, coordEvent) == 0x16c);
STATIC_ASSERT(offsetof(struct EventWork, warp) == 0x170);
STATIC_ASSERT(offsetof(struct EventWork, menu) == 0x172);
STATIC_ASSERT(offsetof(struct EventWork, searchCell) == 0x17a);
STATIC_ASSERT(offsetof(struct EventWork, battle) == 0x17c);
STATIC_ASSERT(offsetof(struct EventWork, psynergy) == 0x17e);
STATIC_ASSERT(offsetof(struct EventWork, raisedEvent) == 0x182);
STATIC_ASSERT(offsetof(struct EventWork, blockedSteps) == 0x19c);
STATIC_ASSERT(offsetof(struct EventWork, mode) == 0x19e);
STATIC_ASSERT(offsetof(struct EventWork, encounterZones) == 0x1a0);
STATIC_ASSERT(offsetof(struct EventWork, encounterBias) == 0x1a8);
STATIC_ASSERT(offsetof(struct EventWork, encounterThreshold) == 0x1ac);
STATIC_ASSERT(offsetof(struct EventWork, transition) == 0x1c0);
STATIC_ASSERT(offsetof(struct EventWork, transitionFrames) == 0x1c8);
STATIC_ASSERT(offsetof(struct EventWork, message) == 0x1d8);
STATIC_ASSERT(offsetof(struct EventWork, camera) == 0x1e0);
STATIC_ASSERT(offsetof(struct EventWork, speaker) == 0x1f4);
STATIC_ASSERT(offsetof(struct EventWork, choiceMessage) == 0xcc2);
STATIC_ASSERT(offsetof(struct EventWork, choiceWidth) == 0xcc4);

#endif
