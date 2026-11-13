/// @file
/// Yallam, the village by Taopo Swamp: its children's song game, run in a
/// circle round the village to the words of the songs of the moon, the stars
/// and the sun, the Scoop spots that hide its treasure, and the pillar Force
/// pushes down to reach the Masamune.

#include "types.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "field/field.h"
#include "module/scene/scene.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "version.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "event_work.h"
#include "system/task.h"
#include "macros.h"
#include "shared/tables.h"

void Yallam_SingMoonSong(void);

void Yallam_SingSunSong(void);
void Yallam_StartWatchers(void);

void Yallam_DigFirstHole(void);

void Yallam_DigSecondHole(void);

void Yallam_DigFourthHole(void);

void Yallam_DigSixthHole(void);

void Yallam_OfferYeppSong(void);

void Yallam_TalkAboutSongGame(void);

void Yallam_ForcePillarDown(void);

void Yallam_DoNothing(void);

extern s32 gYallam_PillarRaised[];
extern s32 gYallam_PillarSunk[];
extern u32 gYallam_FirstVillagerPaces[];
extern u32 gYallam_SecondVillagerPaces[];
extern s16 gYallam_DoorOpening[];
extern s16 gYallam_SecondDoorOpening[];
extern s16 gYallam_ThirdDoorOpening[];
extern s16 gYallam_FourthDoorOpening[];

void TileCopies_CopyTouchedCells(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5,
                                 s32 a6);

s32 Yallam_ClearSpriteTile(struct Actor *object) {
  struct TileSprite *sprite = object->sprite;

  sprite->tile = 0x8000;
  return 0;
}

s32 Yallam_HideSprite(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

#include "shared/distance.c"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Yallam_RunSongPath(s32 id);
void Yallam_ShowSongGame(void);
void Event_End(void);

struct YallamSaved {
  s32 action;
  s32 mode;
  s32 byte98;
  s32 byte99;
};

struct YallamStop {
  s32 a;
  u16 b;
  u16 c;
};

extern struct YallamSaved gYallam_SavedSongGame[5];
extern struct MapGridCellAnimation gYallam_Doors[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Yallam_FaceIfNear(struct Actor *object, struct Actor *target, s32 range,
                      s32 force);
void Yallam_UpdateSongGame(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);

/// Stops an actor and turns it to the target when it is within range (or
/// forced) and already faces near it; returns whether it did.
s32 Yallam_FaceIfNear(struct Actor *object, struct Actor *target, s32 range,
                      s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *objectPos;

  if (BYTE_AT(object, 99) != 0) {
    object->movementState = 1;
    return 1;
  }
  targetPos = &target->x;
  objectPos = &object->x;
  if (SceneActor_CalculateFixedPointDistance(targetPos, objectPos) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z - object->z, *targetPos - *objectPos);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = object->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      object->movementState = 1;
      Actor_SelectAnimation(object, 1);
      result = 1;
    }
  } else {
    object->movementState = 0;
    Actor_SelectAnimation(object, 2);
  }
  return result;
}

// Japanese has no stop beside the leader.
#if TLA_PATCH_1
/// Stops the object (mode 2) when the leader is within 11 of it; otherwise
/// lets it face object 10 or 9 by bit 0 of its action word and, failing that,
/// the leader, forced when bit 1 is set and event word 352 or tile byte 4 is.
s32 Yallam_WatchSongGame(struct Actor *object) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 force = 0;

  leader = Actor_Get(gMainRoutineWork.currentOwner);

  if (SceneActor_CalculateFixedPointDistance(&leader->x, &object->x) <= 11) {
    u8 *state = &object->movementState;
    /* FAKEMATCH(temporary): the call's argument copied before the barrier
     * below. */
    struct Actor *arg = object;

    /* FAKEMATCH(barrier): the state's address and the argument are both formed
     * before the state is stored. */
    asm("" : : "l"(arg), "l"(state));
    *state = force;
    Actor_SelectAnimation(arg, 2);
  } else {
    struct Actor *target;
    u16 *flags = (u16 *)&object->timer;

    if (*flags & 1)
      target = Actor_Get(10);
    else
      target = Actor_Get(9);
    if (Yallam_FaceIfNear(object, target, 24, 0) == 0) {
      leader = Actor_Get(gMainRoutineWork.currentOwner);
      if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
        force = 1;
      Yallam_FaceIfNear(object, leader, 24, force);
    }
  }
  return 0;
}
#else
s32 Yallam_WatchSongGame(struct Actor *object) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 force = 0;
  struct Actor *target;
  u16 *flags = (u16 *)&object->timer;

  if (*flags & 1)
    target = Actor_Get(10);
  else
    target = Actor_Get(9);
  if (Yallam_FaceIfNear(object, target, 24, 0) == 0) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

    if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
      force = 1;
    Yallam_FaceIfNear(object, leader, 24, force);
  }
  return 0;
}
#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s32 gYallam_Placements[];
extern struct ActorPlacement gYallam_Room2Placements[];
extern struct ActorPlacement gYallam_SongGamePlacements[];

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Yallam1 &&
      gMainRoutineWork.entrance == 15)
    return gYallam_SongGamePlacements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Yallam2)
    return gYallam_Room2Placements;
  return gYallam_Placements;
}

/// The party leader walks into one of Yallam's doors, which opens with its cell
/// animation, and leaves by the exit of the door's number.
void Yallam_EnterDoor(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  u32 i;
  s32 index;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = 8; i <= 63; i++) {
    u8 *actor = (u8 *)Actor_Get(i);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  index = *(s16 *)(event + 340);
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gYallam_Doors[index].steps, gYallam_Doors[index].x,
                       gYallam_Doors[index].y);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (index == 7)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(*(s16 *)(event + 340));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

static inline void Yallam_EditMap(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                  s32 a5, s32 a6) {
  TileCopies_CopyTouchedCells(a0, a1, a2, a3, a4, a5, a6);
}

/// Scoop at one of the village's six spots digs into the ground; the third and
/// fifth bring up a buried item the first time, throwing the leader back.
void Yallam_DigFirstHole(void) {
  TileCopies_CopyTouchedCells(67, 25, 5, 5, 67, 5, 1);
  MapGrid_CopyCellsTo(23, 29, 5, 6, 1, 1);
  MapGrid_CopyCellAttributes(23, 29, 1, 1, 5, 8);
}

void Yallam_DigSecondHole(void) { Yallam_EditMap(67, 25, 2, 1, 79, 22, 1); }

void Yallam_DigThirdHole(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Yallam_EditMap(26, 95, 2, 1, 26, 76, 2);
  if (Event_GetFlag(0xf62) == 0) {
    Actor_SetPosition(66, 424, 232);
    if (leader->x >> 20 == 26 && leader->z >> 20 == 14) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Yallam_DigFourthHole(void) { Yallam_EditMap(67, 25, 3, 3, 106, 20, 1); }

void Yallam_DigFifthHole(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Yallam_EditMap(67, 25, 3, 3, 106, 20, 1);
  if (Event_GetFlag(0xf60) == 0) {
    Actor_SetPosition(64, 680, 344);
    if (leader->x >> 20 == 42 && leader->z >> 20 == 21) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Yallam_DigSixthHole(void) { Yallam_EditMap(67, 25, 2, 2, 87, 8, 1); }

extern struct Trigger gYallam_Triggers[];
extern struct Trigger gYallam_RoomTwoTriggers[];
extern struct Trigger gYallam_EntranceFifteenTriggers[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Yallam1 &&
      gMainRoutineWork.entrance == 15)
    return gYallam_EntranceFifteenTriggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Yallam2)
    return gYallam_RoomTwoTriggers;
  return gYallam_Triggers;
}

/// The child at the playground, spoken to from any side, offers to show one of
/// Yepp's songs and its game.
/// "Do you want to hear one of the songs Yepp wrote?"
void Yallam_OfferYeppSong(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgDoYouWant);
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(0, 0) == 0)
    Yallam_ShowSongGame();
  else
    Event_ShowMessage(8, 0);
  Event_End();
}

/// "Running around in circles here while singing one of Yepp's songs is pretty
/// tough."
void Yallam_TalkAboutSongCircles(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = (struct TalkActor *)Actor_Get(9);
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId(MsgRunningAroundIn);
  } else {
    FieldEvent_SetMessageId(MsgThereSALot);
  }
  Event_ShowMessage(9, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Event_End();
}

/// "There's a game that goes along with the song, but it's tough. I want to
/// play something else."
void Yallam_TalkAboutSongGame(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = (struct TalkActor *)Actor_Get(10);
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId(MsgThereSAGame);
  } else {
    FieldEvent_SetMessageId(MsgIOnceHeard);
  }
  Event_ShowMessage(10, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Event_End();
}

s32 Yallam_HasArrived(struct Actor *object) {
  s32 x = object->targetX;

  if (x == 0x80000000 && object->targetZ == 0x80000000)
    return 1;
  return 0;
}

/// One step of a runner's path round the village in the song game, by its
/// step counter, stopping or going on by the game's mode.
void Yallam_RunSongPath(s32 actor) {
  /* FAKEMATCH(pin): the object in r5. */
  register struct YallamActor *object asm("r5") =
      ((struct YallamActor *)Actor_GetById(actor));
  u8 *state = &object->state;
  u8 s = *state;
  s16 *timer;

  if (s == 1)
    return;
  timer = &object->timer;
  switch (s) {
  case 0:
    Actor_SelectAnimation(object, 2);
    break;
  case 1:
    break;
  case 3:
    Actor_SelectAnimation(object, 1);
  case 2:
    Actor_SetMoveTarget(object, object->x, object->y, object->z);
    *timer &= ~1;
    *state = 1;
    return;
  }
  switch (*timer) {
  case 0:
    Actor_SetDestination(actor, 344, 824);
    (*timer)++;
    return;
  case 2:
    Actor_SetDestination(actor, 324, 820);
    (*timer)++;
    return;
  case 4:
    Actor_SetDestination(actor, 320, 808);
    (*timer)++;
    return;
  case 6:
    Actor_SetDestination(actor, 324, 796);
    (*timer)++;
    return;
  case 8:
    Actor_SetDestination(actor, 344, 792);
    (*timer)++;
    return;
  case 10:
    Actor_SetDestination(actor, 364, 796);
    (*timer)++;
    return;
  case 12:
    Actor_SetDestination(actor, 368, 808);
    (*timer)++;
    return;
  case 14:
    Actor_SetDestination(actor, 364, 820);
    (*timer)++;
    return;
  case 16:
    Actor_SetDestination(actor, 344, 824);
    (*timer)++;
    return;
  case 18:
    Actor_SetDestination(actor, 324, 820);
    (*timer)++;
    return;
  case 20:
    Actor_SetDestination(actor, 320, 808);
    (*timer)++;
    return;
  case 22:
    Actor_SetDestination(actor, 324, 796);
    (*timer)++;
    return;
  case 24:
    Actor_SetDestination(actor, 344, 792);
    (*timer)++;
    return;
  case 26:
    Actor_SetDestination(actor, 364, 796);
    (*timer)++;
    return;
  case 28:
    Actor_SetDestination(actor, 368, 808);
    (*timer)++;
    return;
  case 30:
    Actor_SetDestination(actor, 364, 820);
    (*timer)++;
    return;
  case 32:
    Actor_SetDestination(actor, 344, 824);
    (*timer)++;
    return;
  case 34:
    Actor_SetSpeedAndAcceleration(actor, 1.8, 1.8);
    Actor_SetDestination(actor, 364, 820);
    (*timer)++;
    return;
  case 36:
    Actor_SetDestination(actor, 368, 808);
    (*timer)++;
    return;
  case 38:
    Actor_SetDestination(actor, 392, 808);
    (*timer)++;
    return;
  case 40:
    Actor_SetDestination(actor, 392, 744);
    (*timer)++;
    return;
  case 42:
    Actor_SetSpeedAndAcceleration(actor, 1.2, 1.2);
    Actor_SetDestination(actor, 372, 740);
    (*timer)++;
    return;
  case 44:
    Actor_SetDestination(actor, 368, 728);
    (*timer)++;
    return;
  case 46:
    Actor_SetDestination(actor, 372, 716);
    (*timer)++;
    return;
  case 48:
    Actor_SetDestination(actor, 392, 712);
    (*timer)++;
    return;
  case 50:
    Actor_SetDestination(actor, 412, 716);
    (*timer)++;
    return;
  case 52:
    Actor_SetDestination(actor, 416, 728);
    (*timer)++;
    return;
  case 54:
    Actor_SetDestination(actor, 412, 740);
    (*timer)++;
    return;
  case 56:
    Actor_SetDestination(actor, 392, 744);
    (*timer)++;
    return;
  case 58:
    Actor_SetDestination(actor, 372, 740);
    (*timer)++;
    return;
  case 60:
    Actor_SetDestination(actor, 368, 728);
    (*timer)++;
    return;
  case 62:
    Actor_SetDestination(actor, 372, 716);
    (*timer)++;
    return;
  case 64:
    Actor_SetDestination(actor, 392, 712);
    (*timer)++;
    return;
  case 66:
    Actor_SetDestination(actor, 412, 716);
    (*timer)++;
    return;
  case 68:
    Actor_SetDestination(actor, 416, 728);
    (*timer)++;
    return;
  case 70:
    Actor_SetDestination(actor, 412, 740);
    (*timer)++;
    return;
  case 72:
    Actor_SetDestination(actor, 392, 744);
    (*timer)++;
    return;
  case 74:
    Actor_SetDestination(actor, 372, 740);
    (*timer)++;
    return;
  case 76:
    Actor_SetDestination(actor, 368, 728);
    (*timer)++;
    return;
  case 78:
    Actor_SetDestination(actor, 372, 716);
    (*timer)++;
    return;
  case 80:
    Actor_SetDestination(actor, 392, 712);
    (*timer)++;
    return;
  case 82:
    Actor_SetDestination(actor, 412, 716);
    (*timer)++;
    return;
  case 84:
    Actor_SetDestination(actor, 416, 728);
    (*timer)++;
    return;
  case 86:
    Actor_SetDestination(actor, 412, 740);
    (*timer)++;
    return;
  case 88:
    Actor_SetDestination(actor, 392, 744);
    (*timer)++;
    return;
  case 90:
    Actor_SetDestination(actor, 372, 740);
    (*timer)++;
    return;
  case 92:
    Actor_SetDestination(actor, 368, 728);
    (*timer)++;
    return;
  case 94:
    Actor_SetDestination(actor, 372, 716);
    (*timer)++;
    return;
  case 96:
    Actor_SetDestination(actor, 392, 712);
    (*timer)++;
    return;
  case 98:
    Actor_SetDestination(actor, 392, 696);
    (*timer)++;
    return;
  case 100:
    Actor_SetDestination(actor, 412, 692);
    (*timer)++;
    return;
  case 102:
    Actor_SetDestination(actor, 416, 680);
    (*timer)++;
    return;
  case 104:
    Actor_SetDestination(actor, 412, 668);
    (*timer)++;
    return;
  case 106:
    Actor_SetDestination(actor, 392, 664);
    (*timer)++;
    return;
  case 108:
    Actor_SetDestination(actor, 372, 668);
    (*timer)++;
    return;
  case 110:
    Actor_SetDestination(actor, 368, 680);
    (*timer)++;
    return;
  case 112:
    Actor_SetDestination(actor, 320, 680);
    (*timer)++;
    return;
  case 114:
    Actor_SetDestination(actor, 316, 692);
    (*timer)++;
    return;
  case 116:
    Actor_SetDestination(actor, 296, 696);
    (*timer)++;
    return;
  case 118:
    Actor_SetDestination(actor, 276, 692);
    (*timer)++;
    return;
  case 120:
    Actor_SetDestination(actor, 272, 680);
    (*timer)++;
    return;
  case 122:
    Actor_SetDestination(actor, 276, 668);
    (*timer)++;
    return;
  case 124:
    Actor_SetDestination(actor, 296, 664);
    (*timer)++;
    return;
  case 126:
    Actor_SetDestination(actor, 316, 668);
    (*timer)++;
    return;
  case 128:
    Actor_SetDestination(actor, 320, 680);
    (*timer)++;
    return;
  case 130:
    Actor_SetDestination(actor, 316, 692);
    (*timer)++;
    return;
  case 132:
    Actor_SetDestination(actor, 296, 696);
    (*timer)++;
    return;
  case 134:
    Actor_SetDestination(actor, 276, 692);
    (*timer)++;
    return;
  case 136:
    Actor_SetDestination(actor, 272, 680);
    (*timer)++;
    return;
  case 138:
    Actor_SetDestination(actor, 276, 668);
    (*timer)++;
    return;
  case 140:
    Actor_SetDestination(actor, 296, 664);
    (*timer)++;
    return;
  case 142:
    Actor_SetDestination(actor, 316, 668);
    (*timer)++;
    return;
  case 144:
    Actor_SetDestination(actor, 320, 680);
    (*timer)++;
    return;
  case 146:
    Actor_SetDestination(actor, 316, 692);
    (*timer)++;
    return;
  case 148:
    Actor_SetDestination(actor, 296, 696);
    (*timer)++;
    return;
  case 150:
    Actor_SetDestination(actor, 296, 728);
    (*timer)++;
    return;
  case 152:
    Actor_SetDestination(actor, 272, 728);
    (*timer)++;
    return;
  case 154:
    Actor_SetDestination(actor, 268, 740);
    (*timer)++;
    return;
  case 156:
    Actor_SetDestination(actor, 248, 744);
    (*timer)++;
    return;
  case 158:
    Actor_SetDestination(actor, 228, 740);
    (*timer)++;
    return;
  case 160:
    Actor_SetDestination(actor, 224, 728);
    (*timer)++;
    return;
  case 162:
    Actor_SetDestination(actor, 228, 716);
    (*timer)++;
    return;
  case 164:
    Actor_SetDestination(actor, 248, 712);
    (*timer)++;
    return;
  case 166:
    Actor_SetDestination(actor, 268, 716);
    (*timer)++;
    return;
  case 168:
    Actor_SetDestination(actor, 272, 728);
    (*timer)++;
    return;
  case 170:
    Actor_SetDestination(actor, 268, 740);
    (*timer)++;
    return;
  case 172:
    Actor_SetDestination(actor, 248, 744);
    (*timer)++;
    return;
  case 174:
    Actor_SetDestination(actor, 228, 740);
    (*timer)++;
    return;
  case 176:
    Actor_SetDestination(actor, 224, 728);
    (*timer)++;
    return;
  case 178:
    Actor_SetDestination(actor, 228, 716);
    (*timer)++;
    return;
  case 180:
    Actor_SetDestination(actor, 248, 712);
    (*timer)++;
    return;
  case 182:
    Actor_SetDestination(actor, 268, 716);
    (*timer)++;
    return;
  case 184:
    Actor_SetDestination(actor, 272, 728);
    (*timer)++;
    return;
  case 186:
    Actor_SetDestination(actor, 268, 740);
    (*timer)++;
    return;
  case 188:
    Actor_SetDestination(actor, 248, 744);
    (*timer)++;
    return;
  case 190:
    Actor_SetDestination(actor, 228, 740);
    (*timer)++;
    return;
  case 192:
    Actor_SetDestination(actor, 224, 728);
    (*timer)++;
    return;
  case 194:
    Actor_SetDestination(actor, 228, 716);
    (*timer)++;
    return;
  case 196:
    Actor_SetDestination(actor, 248, 712);
    (*timer)++;
    return;
  case 198:
    Actor_SetDestination(actor, 268, 716);
    (*timer)++;
    return;
  case 200:
    Actor_SetDestination(actor, 272, 728);
    (*timer)++;
    return;
  case 202:
    Actor_SetDestination(actor, 268, 740);
    (*timer)++;
    return;
  case 204:
    Actor_SetDestination(actor, 248, 744);
    (*timer)++;
    return;
  case 206:
    Actor_SetDestination(actor, 248, 760);
    (*timer)++;
    return;
  case 208:
    Actor_SetDestination(actor, 228, 764);
    (*timer)++;
    return;
  case 210:
    Actor_SetDestination(actor, 224, 776);
    (*timer)++;
    return;
  case 212:
    Actor_SetDestination(actor, 228, 788);
    (*timer)++;
    return;
  case 214:
    Actor_SetDestination(actor, 248, 792);
    (*timer)++;
    return;
  case 216:
    Actor_SetDestination(actor, 248, 840);
    (*timer)++;
    return;
  case 218:
    Actor_SetDestination(actor, 228, 844);
    (*timer)++;
    return;
  case 220:
    Actor_SetDestination(actor, 224, 856);
    (*timer)++;
    return;
  case 222:
    Actor_SetDestination(actor, 228, 868);
    (*timer)++;
    return;
  case 226:
    Actor_SetDestination(actor, 268, 868);
    (*timer)++;
    return;
  case 228:
    Actor_SetDestination(actor, 272, 856);
    (*timer)++;
    return;
  case 230:
    Actor_SetDestination(actor, 268, 844);
    (*timer)++;
    return;
  case 232:
    Actor_SetDestination(actor, 248, 840);
    (*timer)++;
    return;
  case 234:
    Actor_SetDestination(actor, 228, 844);
    (*timer)++;
    return;
  case 236:
    Actor_SetDestination(actor, 224, 856);
    (*timer)++;
    return;
  case 238:
    Actor_SetDestination(actor, 228, 868);
    (*timer)++;
    return;
  case 223:
  case 239:
    if (Yallam_HasArrived(object) != 0)
      (*timer)++;
  case 224:
  case 240:
    Actor_SetDestination(actor, 248, 872);
    {
      /* FAKEMATCH(pin): the timer's address rebuilt in r2 from the object. */
      register u8 *step asm("r2") = (u8 *)object;

      step += 100;
      (*(s16 *)step)++;
    }
    return;
  case 242:
    Actor_SetDestination(actor, 268, 868);
    (*timer)++;
    return;
  case 244:
    Actor_SetDestination(actor, 272, 856);
    (*timer)++;
    return;
  case 246:
    Actor_SetDestination(actor, 268, 844);
    (*timer)++;
    return;
  case 248:
    Actor_SetDestination(actor, 248, 840);
    (*timer)++;
    return;
  case 250:
    Actor_SetDestination(actor, 228, 844);
    (*timer)++;
    return;
  case 252:
    Actor_SetDestination(actor, 224, 856);
    (*timer)++;
    return;
  case 254:
    Actor_SetSpeedAndAcceleration(actor, 1.8, 1.8);
    Actor_SetDestination(actor, 200, 856);
    (*timer)++;
    return;
  case 256:
    Actor_SetDestination(actor, 200, 744);
    (*timer)++;
    return;
  case 258:
    Actor_SetSpeedAndAcceleration(actor, 1.2, 1.2);
    Actor_SetDestination(actor, 180, 740);
    (*timer)++;
    return;
  case 260:
    Actor_SetDestination(actor, 176, 728);
    (*timer)++;
    return;
  case 262:
    Actor_SetDestination(actor, 180, 716);
    (*timer)++;
    return;
  case 264:
    Actor_SetDestination(actor, 200, 712);
    (*timer)++;
    return;
  case 266:
    Actor_SetDestination(actor, 220, 716);
    (*timer)++;
    return;
  case 268:
    Actor_SetDestination(actor, 224, 728);
    (*timer)++;
    return;
  case 270:
    Actor_SetDestination(actor, 220, 740);
    (*timer)++;
    return;
  case 272:
    Actor_SetDestination(actor, 200, 744);
    (*timer)++;
    return;
  case 274:
    Actor_SetDestination(actor, 180, 740);
    (*timer)++;
    return;
  case 276:
    Actor_SetDestination(actor, 176, 728);
    (*timer)++;
    return;
  case 278:
    Actor_SetDestination(actor, 180, 716);
    (*timer)++;
    return;
  case 280:
    Actor_SetDestination(actor, 200, 712);
    (*timer)++;
    return;
  case 282:
    Actor_SetDestination(actor, 220, 716);
    (*timer)++;
    return;
  case 284:
    Actor_SetDestination(actor, 224, 728);
    (*timer)++;
    return;
  case 286:
    Actor_SetDestination(actor, 220, 740);
    (*timer)++;
    return;
  case 288:
    Actor_SetDestination(actor, 200, 744);
    (*timer)++;
    return;
  case 290:
    Actor_SetDestination(actor, 180, 740);
    (*timer)++;
    return;
  case 292:
    Actor_SetDestination(actor, 176, 728);
    (*timer)++;
    return;
  case 294:
    Actor_SetDestination(actor, 180, 716);
    (*timer)++;
    return;
  case 296:
    Actor_SetDestination(actor, 200, 712);
    (*timer)++;
    return;
  case 298:
    Actor_SetDestination(actor, 220, 716);
    (*timer)++;
    return;
  case 300:
    Actor_SetDestination(actor, 224, 728);
    (*timer)++;
    return;
  case 302:
    Actor_SetDestination(actor, 220, 740);
    (*timer)++;
    return;
  case 304:
    Actor_SetDestination(actor, 200, 744);
    (*timer)++;
    return;
  case 306:
    Actor_SetDestination(actor, 180, 740);
    (*timer)++;
    return;
  case 308:
    Actor_SetDestination(actor, 176, 728);
    (*timer)++;
    return;
  case 310:
    Actor_SetDestination(actor, 180, 716);
    (*timer)++;
    return;
  case 312:
    Actor_SetDestination(actor, 200, 712);
    (*timer)++;
    return;
  case 314:
    Actor_SetDestination(actor, 200, 680);
    (*timer)++;
    return;
  case 316:
    Actor_SetDestination(actor, 212, 676);
    (*timer)++;
    return;
  case 318:
    Actor_SetDestination(actor, 216, 664);
    (*timer)++;
    return;
  case 320:
    Actor_SetDestination(actor, 212, 652);
    (*timer)++;
    return;
  case 322:
    Actor_SetDestination(actor, 200, 648);
    (*timer)++;
    return;
  case 1:
  case 3:
  case 5:
  case 7:
  case 9:
  case 11:
  case 13:
  case 15:
  case 17:
  case 19:
  case 21:
  case 23:
  case 25:
  case 27:
  case 29:
  case 31:
  case 33:
  case 35:
  case 37:
  case 39:
  case 41:
  case 43:
  case 45:
  case 47:
  case 49:
  case 51:
  case 53:
  case 55:
  case 57:
  case 59:
  case 61:
  case 63:
  case 65:
  case 67:
  case 69:
  case 71:
  case 73:
  case 75:
  case 77:
  case 79:
  case 81:
  case 83:
  case 85:
  case 87:
  case 89:
  case 91:
  case 93:
  case 95:
  case 97:
  case 99:
  case 101:
  case 103:
  case 105:
  case 107:
  case 109:
  case 111:
  case 113:
  case 115:
  case 117:
  case 119:
  case 121:
  case 123:
  case 125:
  case 127:
  case 129:
  case 131:
  case 133:
  case 135:
  case 137:
  case 139:
  case 141:
  case 143:
  case 145:
  case 147:
  case 149:
  case 151:
  case 153:
  case 155:
  case 157:
  case 159:
  case 161:
  case 163:
  case 165:
  case 167:
  case 169:
  case 171:
  case 173:
  case 175:
  case 177:
  case 179:
  case 181:
  case 183:
  case 185:
  case 187:
  case 189:
  case 191:
  case 193:
  case 195:
  case 197:
  case 199:
  case 201:
  case 203:
  case 205:
  case 207:
  case 209:
  case 211:
  case 213:
  case 215:
  case 217:
  case 219:
  case 221:
  case 225:
  case 227:
  case 229:
  case 231:
  case 233:
  case 235:
  case 237:
  case 241:
  case 243:
  case 245:
  case 247:
  case 249:
  case 251:
  case 253:
  case 255:
  case 257:
  case 259:
  case 261:
  case 263:
  case 265:
  case 267:
  case 269:
  case 271:
  case 273:
  case 275:
  case 277:
  case 279:
  case 281:
  case 283:
  case 285:
  case 287:
  case 289:
  case 291:
  case 293:
  case 295:
  case 297:
  case 299:
  case 301:
  case 303:
  case 305:
  case 307:
  case 309:
  case 311:
  case 313:
  case 315:
  case 317:
  case 319:
  case 321:
  case 323:
    if (Yallam_HasArrived(object) == 0)
      return;
    (*timer)++;
    return;
  default:
    return;
  }
  (*timer)++;
}

/// A task: sets every runner's mode from the lead child's and moves them on.
void Yallam_UpdateSongGame(void) {
  struct Actor *a = Actor_Get(8);
  struct Actor *b = Actor_Get(10);
  struct Actor *c = Actor_Get(9);
  struct Actor *d = Actor_Get(11);
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 value = (s16)a->unk66;

  switch (value) {
  case 1:
    BYTE_AT(a, 98) = 0;
    BYTE_AT(b, 98) = 0;
    BYTE_AT(c, 98) = 0;
    BYTE_AT(d, 98) = 0;
    BYTE_AT(leader, 98) = 0;
    break;
  case 2:
    BYTE_AT(a, 98) = 2;
    BYTE_AT(b, 98) = 2;
    BYTE_AT(c, 98) = 2;
    BYTE_AT(d, 98) = 2;
    BYTE_AT(leader, 98) = 2;
    break;
  case 3:
    BYTE_AT(a, 98) = 3;
    BYTE_AT(b, 98) = 3;
    BYTE_AT(c, 98) = 3;
    BYTE_AT(d, 98) = 3;
    BYTE_AT(leader, 98) = 3;
    break;
  }
  a->unk66 = 0;
  Yallam_RunSongPath(8);
  Yallam_RunSongPath(10);
  Yallam_RunSongPath(9);
  Yallam_RunSongPath(11);
  Yallam_RunSongPath(gMainRoutineWork.currentOwner);
}

/// Saves every runner's state and stops the game's task.
void Yallam_PauseSongGame(void) {
  struct Actor *object;

  object = Actor_Get(8);
  gYallam_SavedSongGame[0].action = object->timer;
  gYallam_SavedSongGame[0].mode = (s16)object->unk66;
  gYallam_SavedSongGame[0].byte98 = BYTE_AT(object, 98);
  gYallam_SavedSongGame[0].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(10);
  gYallam_SavedSongGame[1].action = object->timer;
  gYallam_SavedSongGame[1].mode = (s16)object->unk66;
  gYallam_SavedSongGame[1].byte98 = BYTE_AT(object, 98);
  gYallam_SavedSongGame[1].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(9);
  gYallam_SavedSongGame[2].action = object->timer;
  gYallam_SavedSongGame[2].mode = (s16)object->unk66;
  gYallam_SavedSongGame[2].byte98 = BYTE_AT(object, 98);
  gYallam_SavedSongGame[2].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(11);
  gYallam_SavedSongGame[3].action = object->timer;
  gYallam_SavedSongGame[3].mode = (s16)object->unk66;
  gYallam_SavedSongGame[3].byte98 = BYTE_AT(object, 98);
  gYallam_SavedSongGame[3].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  gYallam_SavedSongGame[4].action = object->timer;
  gYallam_SavedSongGame[4].mode = (s16)object->unk66;
  gYallam_SavedSongGame[4].byte98 = BYTE_AT(object, 98);
  gYallam_SavedSongGame[4].byte99 = BYTE_AT(object, 99);
  FieldTask_Remove(Yallam_UpdateSongGame);
}

/// Restores every runner's state and starts the game's task again.
void Yallam_ResumeSongGame(void) {
  struct Actor *object;

  object = Actor_Get(8);
  object->timer = gYallam_SavedSongGame[0].action;
  object->unk66 = gYallam_SavedSongGame[0].mode;
  BYTE_AT(object, 98) = gYallam_SavedSongGame[0].byte98;
  BYTE_AT(object, 99) = gYallam_SavedSongGame[0].byte99;
  object = Actor_Get(10);
  object->timer = gYallam_SavedSongGame[1].action;
  object->unk66 = gYallam_SavedSongGame[1].mode;
  BYTE_AT(object, 98) = gYallam_SavedSongGame[1].byte98;
  BYTE_AT(object, 99) = gYallam_SavedSongGame[1].byte99;
  object = Actor_Get(9);
  object->timer = gYallam_SavedSongGame[2].action;
  object->unk66 = gYallam_SavedSongGame[2].mode;
  BYTE_AT(object, 98) = gYallam_SavedSongGame[2].byte98;
  BYTE_AT(object, 99) = gYallam_SavedSongGame[2].byte99;
  object = Actor_Get(11);
  object->timer = gYallam_SavedSongGame[3].action;
  object->unk66 = gYallam_SavedSongGame[3].mode;
  BYTE_AT(object, 98) = gYallam_SavedSongGame[3].byte98;
  BYTE_AT(object, 99) = gYallam_SavedSongGame[3].byte99;
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->timer = gYallam_SavedSongGame[4].action;
  object->unk66 = gYallam_SavedSongGame[4].mode;
  BYTE_AT(object, 98) = gYallam_SavedSongGame[4].byte98;
  BYTE_AT(object, 99) = gYallam_SavedSongGame[4].byte99;
  Task_AddFrame(Yallam_UpdateSongGame, TASK_PASS_VBLANK);
}

/// Readies an actor to run the song game's path from its start.
void Yallam_StartRunner(s32 id) {
  struct YallamActor *actor = ((struct YallamActor *)Actor_Get(id));

  Actor_SetSpeedAndAcceleration(id, 1.2, 1.2);
  Actor_SetAnimation(id, 2);
  actor->timer = 0;
  actor->count = 0;
  actor->state = 1;
  actor->kind = 0;
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern u32 gYallam_SongGameGather9[];
extern u32 gYallam_SongGameGather10[];
extern u32 gYallam_SongGameForm8[];
extern u32 gYallam_SongGameForm9[];
extern u32 gYallam_SongGameForm10[];
extern u32 gYallam_SongGameForm11[];
extern u32 gYallam_SongGameStart8[];
extern u32 gYallam_SongGameStart10[];
extern u32 gYallam_SongGameStart9[];

/// The child shows the party the song's game.
/// "Here, let me show you!"
void Yallam_ShowSongGame(void) {
  struct Actor *actor = Actor_Get(8);
  struct Actor *leader;
  FieldEvent_SetMessageId(MsgHereLetMe);
  Event_ShowMessage(8, 0);
  FieldActor_Stop(9);
  FieldActor_Stop(10);
  (Actor_Get(9))->update = 0;
  (Actor_Get(10))->update = 0;
  System_Wait(1);
  Actor_SetSpeed(9, 1.6);
  Actor_SetSpeed(10, 1.6);
  Actor_SetScript(9, gYallam_SongGameGather9);
  Actor_SetScript(10, gYallam_SongGameGather10);
  (FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 878, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 872);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_FaceDirection(8, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHEAST);
  FieldActor_Jump(8, 2, 10);
  FieldActor_Jump(8, 4, 10);
  Actor_SetAnimationAndWait(8, 3);
  Actor_SetSpeed(8, 1.6);
  Actor_SetSpeed(11, 1.6);
  Actor_SetScript(8, gYallam_SongGameForm8);
  Actor_SetScript(9, gYallam_SongGameForm9);
  Actor_SetScript(10, gYallam_SongGameForm10);
  Actor_RunScriptAndWait(11, gYallam_SongGameForm11);
  Actor_SetPosition(5, 296, 872);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_SetSpeed(5, 1.2);
  Actor_WalkToAndWait(5, 312, 854);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(5, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0) {
    Actor_SetDestination(5, ((s16 *)(&leader->x))[1], ((s16 *)(&leader->z))[1]);
  }
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 316, 896);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 344, 912);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Event_PlaySound(MUS_017);
  Actor_FaceDirectionAndWait(8, FACING_NORTH);
  Event_ShowMessage(8, 0);
  EventCamera_FollowActor(8, 1);
  Actor_SetScript(8, gYallam_SongGameStart8);
  Actor_SetScript(10, gYallam_SongGameStart10);
  Actor_RunScriptAndWait(9, gYallam_SongGameStart9);
  Actor_WalkToAndWait(11, 344, 890);
  Actor_FaceDirectionAndWait(11, FACING_NORTH);
  Yallam_StartRunner(8);
  Yallam_StartRunner(10);
  Yallam_StartRunner(9);
  Yallam_StartRunner(11);
  Yallam_StartRunner(gMainRoutineWork.currentOwner);
  {
    s32 priority = 0x480;
    Task_AddFrame(Yallam_UpdateSongGame, priority);
  }
  Event_ShowMessage(8, 0);
  actor->unk66 = 1;
wait1:
  {
    s16 *count;
    System_Wait(1);
    if ((*(&actor->timer)) != 34) {
      goto wait1;
    }
  }

  actor->unk66 = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  actor->unk66 = 1;
wait2:
  System_Wait(1);

  if (actor->timer != 42) {
    goto wait2;
  }
  actor->unk66 = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  actor->unk66 = 1;
wait3:
  System_Wait(1);

  if (actor->timer != 108) {
    goto wait3;
  }
  actor->unk66 = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Yallam_PauseSongGame();
  FieldActor_Jump(8, 4, 0);
  FieldActor_Jump(10, 4, 0);
  FieldActor_Jump(9, 4, 0);
  FieldActor_Jump(11, 4, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Yallam_SingMoonSong();
  } else {
    FieldEvent_ShowMessageThenWait(8, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_Wait(40);
    FieldEvent_SetExitRequest(15);
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 MsgIfYouWant[];

/// "If you want to go to the moon, if you want to go to the moon."
void Yallam_SingMoonSong(void) {
  s32 v5, v6;
  struct Actor *object;

  object = Actor_Get(8);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_EAST);
  FieldEvent_SetMessageId((s32)&MsgIfYouWant);
  v5 = object;
  Event_ShowMessage(8, 0);
  v5 = (v5 + 102);
  Yallam_ResumeSongGame();
  FIELD_AT(v5, s16, 0) = 1;
waitFirstVerse:
  System_Wait(1);
  v6 = object;
  v6 = (v6 + 100);
  if (FIELD_AT(v6, s16, 0) != 114)
    goto waitFirstVerse;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
waitSecondVerse:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 150)
    goto waitSecondVerse;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
waitThirdVerse:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 154)
    goto waitThirdVerse;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
waitFourthVerse:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 216)
    goto waitFourthVerse;
  FIELD_AT(v5, s16, 0) = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Yallam_PauseSongGame();
  FieldActor_Jump(8, 4, 0);
  FieldActor_Jump(10, 4, 0);
  FieldActor_Jump(9, 4, 0);
  FieldActor_Jump(11, 4, 40);
  v5 = (s32)&gMainRoutineWork;
  v5 = (v5 + 0x214);
  Actor_FaceDirection(FIELD_AT(v5, s32, 0), FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(10, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHEAST);
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FIELD_AT(v5, s32, 0), 0) != 0)
    goto declined;
  Yallam_SingSunSong();
  goto done;
declined:
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(15);
done:
}

extern u8 MsgIfYouWantTo[];

/// "If you want to go to the sun, if you want to go to the sun."
void Yallam_SingSunSong(void) {
  s32 t1, t24, t25, t33, t34, t42, t43, t51, t52, t63, t64, t65, t66, v5, v6,
      v7, v8;

  t1 = ((s32)Actor_Get(8));
  v6 = t1;
  Actor_FaceDirection(8, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  FieldEvent_SetMessageId((s32)&MsgIfYouWantTo);
  v5 = v6;
  Event_ShowMessage(8, 0);
  v5 = (v5 + 102);
  Yallam_ResumeSongGame();
  FIELD_AT(v5, s16, 0) = 1;
waitFirstVerse:
  System_Wait(1);
  v7 = v6;
  v7 = (v7 + 100);
  if (FIELD_AT(v7, s16, 0) != 218)
    goto waitFirstVerse;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
waitSecondVerse:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 254)
    goto waitSecondVerse;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
waitThirdVerse:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x102)
    goto waitThirdVerse;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
waitFourthVerse:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x144)
    goto waitFourthVerse;
  FIELD_AT(v5, s16, 0) = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Yallam_PauseSongGame();
  Actor_FaceDirectionAndWait(8, FACING_SOUTH_SOUTHWEST);
  t24 = ((s32)Actor_Get(8));
  FIELD_AT(t24, u8, 90) = (254 & FIELD_AT(t24, u8, 90));
  t25 = ((s32)Actor_Get(8));
  v6 = t25;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(8, 2.4);
  FieldActor_Jump(8, 6, 0);
  Actor_MoveToAndWait(8, 200, 670);
  FieldActor_Jump(8, 4, 0);
  FieldActor_SetMoveTargetFixed(v6, v7, 0, v8);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  t33 = ((s32)Actor_Get(10));
  FIELD_AT(t33, u8, 90) = (254 & FIELD_AT(t33, u8, 90));
  t34 = ((s32)Actor_Get(10));
  v6 = t34;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(10, 2.4);
  FieldActor_Jump(10, 6, 0);
  Actor_MoveToAndWait(10, 200, 670);
  FieldActor_Jump(10, 4, 0);
  FieldActor_SetMoveTargetFixed(v6, v7, 0, v8);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  t42 = ((s32)Actor_Get(9));
  FIELD_AT(t42, u8, 90) = (254 & FIELD_AT(t42, u8, 90));
  t43 = ((s32)Actor_Get(9));
  v6 = t43;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(9, 2.4);
  FieldActor_Jump(9, 6, 0);
  Actor_MoveToAndWait(9, 200, 670);
  FieldActor_Jump(9, 4, 0);
  FieldActor_SetMoveTargetFixed(v6, v7, 0, v8);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
  t51 = ((s32)Actor_Get(11));
  v5 = (254 & FIELD_AT(t51, u8, 90));
  FIELD_AT(t51, u8, 90) = v5;
  t52 = ((s32)Actor_Get(11));
  v6 = t52;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(11, 2.4);
  FieldActor_Jump(11, 6, 0);
  Actor_MoveToAndWait(11, 200, 670);
  FieldActor_Jump(11, 4, 0);
  FieldActor_SetMoveTargetFixed(v6, v7, 0, v8);
  FieldActor_WaitForMoveById(11);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 184, 686);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 186, 674);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  t63 = ((s32)Actor_Get(8));
  FIELD_AT(t63, u8, 90) = (FIELD_AT(t63, u8, 90) | 1);
  t64 = ((s32)Actor_Get(10));
  FIELD_AT(t64, u8, 90) = (FIELD_AT(t64, u8, 90) | 1);
  t65 = ((s32)Actor_Get(9));
  FIELD_AT(t65, u8, 90) = (FIELD_AT(t65, u8, 90) | 1);
  t66 = ((s32)Actor_Get(11));
  v5 = (1 | FIELD_AT(t66, u8, 90));
  FIELD_AT(t66, u8, 90) = v5;
  Actor_FaceDirection(8, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(11, FACING_WEST, 20);
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(15);
}

/// Once Force has pushed it (flag 0x300), the pillar lies where it fell.
void Yallam_PlaceFallenPillar(void) {
  if (Event_GetFlag(FLAG_AREA_TEMP_00)) {
    SET_POSITION(Actor_Get(9), 0xa80000, 0, 0xc00000);
  }
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Yallam1) {
    if (gMainRoutineWork.entrance == 15)
      Yallam_StartWatchers();
    else
      Yallam_PlaceFallenPillar();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Yallam1 &&
      Event_GetFlag(FLAG_AREA_TEMP_00))
    MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
  return 0;
}

/// The Force trigger's first phase, which does nothing here.
void Yallam_DoNothing(void) {}

/// Force pushes the pillar off its ledge; it tumbles down and lands as a step.
void Yallam_ForcePillarDown(void) {
  s32 step;
  s32 i;
  struct Actor *object = Actor_Get(9);
  struct TileSprite *sprite = object->sprite;

  Event_SetFlag(FLAG_AREA_TEMP_00);
  object->acceleration = 0x18000;
  object->speedLimit = 0x80000;
  Event_PlaySound(SE_TOPPLE);
  FieldActor_SetMoveTarget(object, 176, 40, 184);
  step = 0;
  for (i = 15; i >= 0; i--) {
    sprite->tile -= step;
    System_Wait(1);
    step += 40;
  }
  FieldActor_SetMoveTarget(object, 168, 16, 184);
  step = 400;
  for (i = 25; i >= 0; i--) {
    sprite->tile -= step;
    System_Wait(1);
    step += 40;
  }
  FieldActor_WaitForMove(object);
  FieldEvent_Wait(2);
  Event_PlaySound(SE_LOG_FALL);
  i = 0;
  sprite->tile = i;
  object->x = 0xa80000;
  object->z = 0xc00000;
  object->y = i;
  object->velocityY = i;
  object->velocityX = i;
  MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
}

/// Yallam's view of the two objects Yallam_StartWatchers restarts.

/// Hands objects 9 and 10 to Yallam_WatchSongGame's update, object 9 with its
/// timer at 1 and object 10 at 0.
void Yallam_StartWatchers(void) {
  struct YallamActor *actor = ((struct YallamActor *)Actor_Get(9));
  struct YallamActor *other;

  actor->timer = 1;
  actor->kind = 0;
  actor->update = Yallam_WatchSongGame;
  other = ((struct YallamActor *)Actor_Get(10));
  other->timer = 0;
  other->kind = 0;
  other->update = Yallam_WatchSongGame;
}

#undef BYTE_AT
#undef EVENT_WORK
#undef FIELD_AT
#include "text/msg_ids.h"

void Yallam_EnterDoor(void);
void Yallam_DigThirdHole(void);
void Yallam_DigFifthHole(void);
void Yallam_TalkAboutSongCircles(void);

extern s32 gYallam_PillarShadowSunk[];

s32 gYallam_PillarRaised[] = {
    39, 3, 22, 26, 0, 23, 6, 0x20000, 46, (s32)Yallam_HideSprite, 17,
};

s32 gYallam_PillarSunk[] = {
    22, 26, 0, 23, 6, -0x100000, 46, (s32)Yallam_HideSprite, 17,
};

s32 gYallam_PillarShadowSunk[] = {
    22, 9, 0x9999, 22, 10,        0x9999, 22,
    26, 0, 23,     6,  -0x100000, 46,     (s32)Yallam_HideSprite,
    17,
};

// clang-format off
DEFINE_G_DATA_02002634(gYallam_FirstVillagerPaces, 0x1480000, 0x2f00000, 0x1680000,
    0x1780000, 0x3080000, 0x1680000, 0x3180000, 0x1480000, 0x1380000)

DEFINE_G_DATA_02002634(gYallam_SecondVillagerPaces, 0x1680000, 0x3180000, 0x1480000,
    0x1380000, 0x3080000, 0x1480000, 0x2f00000, 0x1680000, 0x1780000)

u32 gYallam_SongGameGather9[] = {
    39, 2, 4, 0x1500000, 0, 0x3180000,
    1, 39, 1, 0, 1, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gYallam_SongGameGather10[] = {
    39, 2, 4, 0x1600000, 0, 0x3180000,
    1, 39, 1, 0, 1, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gYallam_SongGameForm8[] = {
    39, 2, 4, 0x13c0000, 0, 0x3780000,
    1, 4, 0x1500000, 0, 0x3780000, 1,
    4, 0x1580000, 0, 0x3680000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gYallam_SongGameForm9[] = {
    39, 2, 4, 0x16a0000, 0, 0x3180000,
    1, 4, 0x1800000, 0, 0x3500000, 1,
    4, 0x17e0000, 0, 0x3780000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gYallam_SongGameForm10[] = {
    39, 2, 4, 0x16c0000, 0, 0x3280000,
    1, 4, 0x1760000, 0, 0x36e0000, 1,
    4, 0x1680000, 0, 0x3780000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gYallam_SongGameForm11[] = {
    39, 2, 4, 0x1120000, 0, 0x3780000,
    1, 4, 0x1480000, 0, 0x3780000, 1,
    39, 1, 22, 3, 0x8000, 0,
    10, 17,
};

u32 gYallam_SongGameStart8[] = {
    39, 2, 4, 0x1580000, 0, 0x3480000,
    1, 39, 1, 17,
};

u32 gYallam_SongGameStart10[] = {
    39, 2, 4, 0x1580000, 0, 0x3780000,
    1, 4, 0x1580000, 0, 0x3580000, 1,
    39, 1, 0, 1, 17,
};

u32 gYallam_SongGameStart9[] = {
    39, 2, 4, 0x1580000, 0, 0x3780000,
    1, 4, 0x1580000, 0, 0x36a0000, 1,
    39, 1, 0, 1, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Yallam1),
    SCENE_EXIT(1, YallamInteriors1, 1),
    SCENE_EXIT(2, YallamInteriors1, 2),
    SCENE_EXIT(3, YallamInteriors2, 1),
    SCENE_EXIT(4, YallamInteriors1, 4),
    SCENE_EXIT(5, YallamInteriors2, 3),
    SCENE_EXIT(6, YallamInteriors2, 2),
    SCENE_EXIT(7, YallamInteriors1, 3),
    SCENE_EXIT(8, MikasallaInteriors2, 4),
    SCENE_EXIT(9, Yallam2, 6),
    SCENE_EXIT(10, WorldMap, 34),
    SCENE_EXIT(11, WorldMap, 33),
    SCENE_EXIT(12, Yallam1, 15),
    SCENE_EXIT(13, Yallam2, 7),
    SCENE_EXIT(15, Yallam1, 12),
    SCENE_EXITS(Yallam2),
    SCENE_EXIT(6, Yallam1, 9),
    SCENE_EXIT(7, Yallam1, 13),
    SCENE_EXITS_END,
};

s32 gYallam_Placements[] = {
    (s32)0xffff0146,
    (s32)gYallam_PillarSunk,
    0xb80000,
    0,
    0xc80000,
    0x8000,
    (s32)0xffff0146,
    (s32)gYallam_PillarRaised,
    0xb80000,
    0,
    0xc80000,
    0x8000,
    (s32)0xffff0147,
    (s32)gYallam_PillarShadowSunk,
    0x80000,
    0,
    0x80000,
    0x8000,
    (s32)0xffff0073,
    1,
    0x2680000,
    0,
    0xd80000,
    0x32000,
    (s32)0xffff0074,
    1,
    0x2c80000,
    0,
    0xe80000,
    0x14000,
    (s32)0xffff0072,
#if !TLA_PATCH_1
    2,
    0x2280000,
    0,
    0xe80000,
#else
    3,
    0x2280000,
    0,
    0xf80000,
#endif
    0x16000,
    (s32)0xffff0080,
    2,
    0x1680000,
    0,
    0x1680000,
    0x1a000,
    (s32)0xffff0070,
    1,
    0x1c80000,
    0,
    0x1180000,
    0x12000,
    (s32)0xffff0071,
    1,
    0xd80000,
    0,
    0x1780000,
    0x1e000,
    (s32)0xffff00e2,
    1,
    0x1d00000,
    0,
    0x880000,
    0x16000,
    (s32)0xffff00e2,
    1,
    0x1880000,
    0,
    0x1080000,
    0x14000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct ActorPlacement gYallam_Room2Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gYallam_SongGamePlacements[] = {
    {136, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(872), FACING_WEST, 1, 0},
    {136, CONDITION_ALWAYS, (s32)gYallam_FirstVillagerPaces, PIXELS(312), 0, PIXELS(776),
     FACING_NORTH, 2, 0},
    {138, CONDITION_ALWAYS, (s32)gYallam_SecondVillagerPaces, PIXELS(376), 0, PIXELS(776),
     FACING_SOUTH, 2, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(272), 0, PIXELS(800), 0xb000, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gYallam_DoorOpening[] = {
    93, 28, 1, 2, 6, 92, 28, 1,
    2, 6, -1,
};

s16 gYallam_SecondDoorOpening[] = {
    95, 28, 1, 2, 6, 94, 28, 1,
    2, 6, -1,
};

s16 gYallam_ThirdDoorOpening[] = {
    89, 28, 1, 2, 6, 90, 28, 1,
    2, 6, -1,
};

s16 gYallam_FourthDoorOpening[] = {
    92, 30, 2, 2, 6, 94, 30, 2,
    2, 6, -1,
};

struct MapGridCellAnimation gYallam_Doors[] = {
    {gYallam_DoorOpening, 98, 19},
    {gYallam_SecondDoorOpening, 92, 20},
    {gYallam_ThirdDoorOpening, 84, 18},
    {gYallam_DoorOpening, 79, 7},
    {gYallam_ThirdDoorOpening, 80, 16},
    {gYallam_ThirdDoorOpening, 77, 17},
    {gYallam_SecondDoorOpening, 99, 8},
    {gYallam_FourthDoorOpening, 90, 6},
};

struct Trigger gYallam_Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 13, CONDITION_ALWAYS, 13},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(Yallam_DigFirstHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21, CONDITION_ALWAYS, EVENT(Yallam_DigSecondHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22, CONDITION_ALWAYS, EVENT(Yallam_DigThirdHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23, CONDITION_ALWAYS, EVENT(Yallam_DigFourthHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 24, CONDITION_ALWAYS, EVENT(Yallam_DigFifthHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 25, CONDITION_ALWAYS, EVENT(Yallam_DigSixthHole)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgYallamDoesnT)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgOurMasterBoatsman)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIfYeppWere)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgYeppFoundThis)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgThereUsedTo)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgWhenWeLost)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgYeppWasThe)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgYeppWasTheOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSingingWithYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgThosePearlsYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgYeppNeverTold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgYeppWasAmazing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgSunshineIsA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgIVeSearched)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgYeppAlwaysSang)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x300, (s32)(Yallam_DoNothing)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 8, 0x300, EVENT(Yallam_ForcePillarDown)},
    {TRIGGER_TABLE_END},
};

struct Trigger gYallam_RoomTwoTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gYallam_EntranceFifteenTriggers[] = {
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TALK_TRIGGER_FACING(FACING_EAST), 8, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_SOUTHEAST), 8, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_NORTHEAST), 8, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), 8, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgYeppSSongs)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Yallam_TalkAboutSongCircles)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongCircles)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Yallam_TalkAboutSongGame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongGame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgYeppWroteAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgTheStoryIs)},
    {TRIGGER_TABLE_END},
};
