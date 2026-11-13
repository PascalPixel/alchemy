/// @file
/// The scene code of overlay 0x676, in ROM order.

#include "types.h"
#include "actor.h"
#include "field.h"
#include "module/scene/scene.h"
#include "module/scene/yallam.h"

void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

s32 Func_02000038(struct Actor *object) {
  struct TileSprite *sprite = object->sprite;

  sprite->tile = 0x8000;
  return 0;
}

s32 Func_02000044(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

#include "venus_lighthouse/distance.c"

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 MsgDoYouWant[];
extern const u8 MsgRunningAroundIn[];
extern const u8 MsgThereSALot[];
extern const u8 MsgThereSAGame[];
extern const u8 MsgIOnceHeard[];

void Func_020006bc(s32 id);
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

extern struct YallamSaved gData_02002fc0[5];
extern const struct YallamStop gData_02002ce4[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_02000088(struct Actor *object, struct Actor *target, s32 range,
                  s32 force);
void Func_02001688(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
#include "version.h"
#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "task.h"
#include "ram_buffer.h"
#include "macros.h"

s32 Func_02000088(struct Actor *object, struct Actor *target, s32 range,
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
s32 Func_0200014c(struct Actor *object) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 force = 0;

  leader = Actor_Get(gMainRoutineWork.currentOwner);

  if (SceneActor_CalculateFixedPointDistance(&leader->x, &object->x) <= 11) {
    u8 *state = &object->movementState;
    /* FAKEMATCH(temporary): the call's argument copied before the barrier
     * below; as object->movementState = force; Actor_SelectAnimation(object, 2), 4 of
     * 82 instructions differ in tla-en. */
    struct Actor *arg = object;

    /* FAKEMATCH(barrier): the state's address and the argument are both formed
     * before the state is stored, as the native does; as object->movementState
     * = force; Actor_SelectAnimation(object, 2), with the copy above, 4 of 82
     * instructions differ in tla-en. */
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
    if (Func_02000088(object, target, 24, 0) == 0) {
      leader = Actor_Get(gMainRoutineWork.currentOwner);
      if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
        force = 1;
      Func_02000088(object, leader, 24, force);
    }
  }
  return 0;
}
#else
s32 Func_0200014c(struct Actor *object) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 force = 0;
  struct Actor *target;
  u16 *flags = (u16 *)&object->timer;

  if (*flags & 1)
    target = Actor_Get(10);
  else
    target = Actor_Get(9);
  if (Func_02000088(object, target, 24, 0) == 0) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

    if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
      force = 1;
    Func_02000088(object, leader, 24, force);
  }
  return 0;
}
#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gYallam_Placements[];
extern const u8 gYallam_Room2Placements[];
extern const u8 gData_02002bfc[];

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1 &&
      gMainRoutineWork.entrance == 15)
    return gData_02002bfc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room2)
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
  Event_PlaySound(158);
  index--;
  MapGrid_AnimateCells(gData_02002ce4[index].a, gData_02002ce4[index].b,
                       gData_02002ce4[index].c);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
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

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Yallam_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                   s32 a5, s32 a6) {
  Func_02002598(a0, a1, a2, a3, a4, a5, a6);
}

void Func_02000328(void) {
  Func_02002598(67, 25, 5, 5, 67, 5, 1);
  MapGrid_CopyCellsTo(23, 29, 5, 6, 1, 1);
  MapGrid_CopyCellAttributes(23, 29, 1, 1, 5, 8);
}

void Func_0200036c(void) { Yallam_02002598(67, 25, 2, 1, 79, 22, 1); }

void Func_0200038c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Yallam_02002598(26, 95, 2, 1, 26, 76, 2);
  if (Event_GetFlag(0xf62) == 0) {
    Actor_SetPosition(66, PIXELS(424), PIXELS(232));
    if (leader->x >> 20 == 26 && leader->z >> 20 == 14) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_0200041c(void) { Yallam_02002598(67, 25, 3, 3, 106, 20, 1); }

void Func_0200043c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Yallam_02002598(67, 25, 3, 3, 106, 20, 1);
  if (Event_GetFlag(0xf60) == 0) {
    Actor_SetPosition(64, PIXELS(680), PIXELS(344));
    if (leader->x >> 20 == 42 && leader->z >> 20 == 21) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_020004cc(void) { Yallam_02002598(67, 25, 2, 2, 87, 8, 1); }

extern const u8 gYallam_Triggers[];
extern const u8 gYallam_RoomTwoTriggers[];
extern const u8 gYallam_EntranceFifteenTriggers[];

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1 &&
      gMainRoutineWork.entrance == 15)
    return gYallam_EntranceFifteenTriggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room2)
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

s32 Func_020006a4(struct Actor *object) {
  s32 x = object->targetX;

  if (x == 0x80000000 && object->targetZ == 0x80000000)
    return 1;
  return 0;
}

void Func_020006bc(s32 actor) {
  /* FAKEMATCH(pin): the object in r5, as the native keeps it; as a plain local,
   * 372 of 1163 instructions differ in tla-en. */
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
    Actor_SetSpeed(actor, 0x1cccc, 0x1cccc);
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
    Actor_SetSpeed(actor, 0x13333, 0x13333);
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
    if (Func_020006a4(object) != 0)
      (*timer)++;
  case 224:
  case 240:
    Actor_SetDestination(actor, 248, 872);
    {
      /* FAKEMATCH(pin): the timer's address rebuilt in r2 from the object; as
       * object->timer++, 190 of 1163 instructions differ in tla-en. */
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
    Actor_SetSpeed(actor, 0x1cccc, 0x1cccc);
    Actor_SetDestination(actor, 200, 856);
    (*timer)++;
    return;
  case 256:
    Actor_SetDestination(actor, 200, 744);
    (*timer)++;
    return;
  case 258:
    Actor_SetSpeed(actor, 0x13333, 0x13333);
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
    if (Func_020006a4(object) == 0)
      return;
    (*timer)++;
    return;
  default:
    return;
  }
  (*timer)++;
}

void Func_02001688(void) {
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
  Func_020006bc(8);
  Func_020006bc(10);
  Func_020006bc(9);
  Func_020006bc(11);
  Func_020006bc(gMainRoutineWork.currentOwner);
}

void Func_02001730(void) {
  struct Actor *object;

  object = Actor_Get(8);
  gData_02002fc0[0].action = object->timer;
  gData_02002fc0[0].mode = (s16)object->unk66;
  gData_02002fc0[0].byte98 = BYTE_AT(object, 98);
  gData_02002fc0[0].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(10);
  gData_02002fc0[1].action = object->timer;
  gData_02002fc0[1].mode = (s16)object->unk66;
  gData_02002fc0[1].byte98 = BYTE_AT(object, 98);
  gData_02002fc0[1].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(9);
  gData_02002fc0[2].action = object->timer;
  gData_02002fc0[2].mode = (s16)object->unk66;
  gData_02002fc0[2].byte98 = BYTE_AT(object, 98);
  gData_02002fc0[2].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(11);
  gData_02002fc0[3].action = object->timer;
  gData_02002fc0[3].mode = (s16)object->unk66;
  gData_02002fc0[3].byte98 = BYTE_AT(object, 98);
  gData_02002fc0[3].byte99 = BYTE_AT(object, 99);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  gData_02002fc0[4].action = object->timer;
  gData_02002fc0[4].mode = (s16)object->unk66;
  gData_02002fc0[4].byte98 = BYTE_AT(object, 98);
  gData_02002fc0[4].byte99 = BYTE_AT(object, 99);
  FieldTask_Remove(Func_02001688);
}

void Func_0200182c(void) {
  struct Actor *object;

  object = Actor_Get(8);
  object->timer = gData_02002fc0[0].action;
  object->unk66 = gData_02002fc0[0].mode;
  BYTE_AT(object, 98) = gData_02002fc0[0].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[0].byte99;
  object = Actor_Get(10);
  object->timer = gData_02002fc0[1].action;
  object->unk66 = gData_02002fc0[1].mode;
  BYTE_AT(object, 98) = gData_02002fc0[1].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[1].byte99;
  object = Actor_Get(9);
  object->timer = gData_02002fc0[2].action;
  object->unk66 = gData_02002fc0[2].mode;
  BYTE_AT(object, 98) = gData_02002fc0[2].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[2].byte99;
  object = Actor_Get(11);
  object->timer = gData_02002fc0[3].action;
  object->unk66 = gData_02002fc0[3].mode;
  BYTE_AT(object, 98) = gData_02002fc0[3].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[3].byte99;
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->timer = gData_02002fc0[4].action;
  object->unk66 = gData_02002fc0[4].mode;
  BYTE_AT(object, 98) = gData_02002fc0[4].byte98;
  BYTE_AT(object, 99) = gData_02002fc0[4].byte99;
  Task_AddFrame(Func_02001688, TASK_PASS_VBLANK);
}

void Func_02001904(s32 id) {
  struct YallamActor *actor = ((struct YallamActor *)Actor_Get(id));

  Actor_SetSpeed(id, 0x13333, 0x13333);
  Actor_SetAnimation(id, 2);
  actor->timer = 0;
  actor->count = 0;
  actor->state = 1;
  actor->kind = 0;
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 MsgHereLetMe[];
extern const u8 gData_0200277c[];
extern const u8 gData_020027c0[];
extern const u8 gData_02002804[];
extern const u8 gData_02002870[];
extern const u8 gData_020028dc[];
extern const u8 gData_02002948[];
extern const u8 gData_02002998[];
extern const u8 gData_020029c0[];
extern const u8 gData_02002a04[];

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
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  Actor_SetScript(9, gData_0200277c);
  Actor_SetScript(10, gData_020027c0);
  (FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x1300000, -1, 0x36e0000, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 872);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
  Event_PlaySound(78);
  Actor_FaceDirection(8, 0xb000, 20);
  Actor_FaceDirectionAndWait(8, 0xd000);
  FieldActor_Jump(8, 2, 10);
  FieldActor_Jump(8, 4, 10);
  Actor_SetAnimationAndWait(8, 3);
  Actor_SetSpeed(8, 0x19999, 0xcccc);
  Actor_SetSpeed(11, 0x19999, 0xcccc);
  Actor_SetScript(8, gData_02002804);
  Actor_SetScript(9, gData_02002870);
  Actor_SetScript(10, gData_020028dc);
  Actor_RunScriptAndWait(11, gData_02002948);
  Actor_SetPosition(5, 0x1280000, 0x3680000);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_WalkToAndWait(5, 312, 854);
  Actor_FaceDirectionAndWait(5, 0x6000);
  Actor_SetAnimationAndWait(5, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0) {
    Actor_SetDestination(5, ((s16 *)(&leader->x))[1], ((s16 *)(&leader->z))[1]);
  }
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 316, 0x380);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 344, 0x390);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0xc000);
  Event_PlaySound(17);
  Actor_FaceDirectionAndWait(8, 0xc000);
  Event_ShowMessage(8, 0);
  EventCamera_FollowActor(8, 1);
  Actor_SetScript(8, gData_02002998);
  Actor_SetScript(10, gData_020029c0);
  Actor_RunScriptAndWait(9, gData_02002a04);
  Actor_WalkToAndWait(11, 344, 890);
  Actor_FaceDirectionAndWait(11, 0xc000);
  Func_02001904(8);
  Func_02001904(10);
  Func_02001904(9);
  Func_02001904(11);
  Func_02001904(gMainRoutineWork.currentOwner);
  {
    s32 priority = 0x480;
    Task_AddFrame(Func_02001688, priority);
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
  Func_02001730();
  FieldActor_Jump(8, 4, 0);
  FieldActor_Jump(10, 4, 0);
  FieldActor_Jump(9, 4, 0);
  FieldActor_Jump(11, 4, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(9, 0x8000, 0);
  Actor_FaceDirectionAndWait(11, 0x8000);
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Yallam_SingMoonSong();
  } else {
    FieldEvent_ShowMessageThenWait(8, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(123);
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
  s32 t1, t33, v5, v6, v7;

  t1 = ((s32)Actor_Get(8));
  v7 = t1;
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_FaceDirection(11, 0xb000, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)&MsgIfYouWant);
  v5 = v7;
  Event_ShowMessage(8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009d78:
  System_Wait(1);
  v6 = v7;
  v6 = (v6 + 100);
  if (FIELD_AT(v6, s16, 0) != 114)
    goto L_02009d78;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009da0:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 150)
    goto L_02009da0;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009dc4:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 154)
    goto L_02009dc4;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009de8:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 216)
    goto L_02009de8;
  FIELD_AT(v5, s16, 0) = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Func_02001730();
  FieldActor_Jump(8, 4, 0);
  FieldActor_Jump(10, 4, 0);
  FieldActor_Jump(9, 4, 0);
  FieldActor_Jump(11, 4, 40);
  v5 = (s32)&gMainRoutineWork;
  v5 = (v5 + 0x214);
  Actor_FaceDirection(FIELD_AT(v5, s32, 0), FACING_SOUTH, 0);
  Actor_FaceDirection(8, 0xd000, 0);
  Actor_FaceDirection(10, 0xd000, 0);
  Actor_FaceDirection(9, 0xd000, 0);
  Actor_FaceDirectionAndWait(11, 0xd000);
  Event_OpenMessage(8, 0);
  t33 = FieldEvent_AskYesNoWithActorAnimation(FIELD_AT(v5, s32, 0), 0);
  if (t33 != 0)
    goto L_02009e90;
  Yallam_SingSunSong();
  goto L_02009eb4;
L_02009e90:
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(123);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(15);
L_02009eb4:
}

extern u8 MsgIfYouWantTo[];

/// "If you want to go to the sun, if you want to go to the sun."
void Yallam_SingSunSong(void) {
  s32 t1, t24, t25, t33, t34, t42, t43, t51, t52, t63, t64, t65, t66, v5, v6,
      v7, v8;

  t1 = ((s32)Actor_Get(8));
  v6 = t1;
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(10, 0, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  FieldEvent_SetMessageId((s32)&MsgIfYouWantTo);
  v5 = v6;
  Event_ShowMessage(8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009f28:
  System_Wait(1);
  v7 = v6;
  v7 = (v7 + 100);
  if (FIELD_AT(v7, s16, 0) != 218)
    goto L_02009f28;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f50:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 254)
    goto L_02009f50;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f74:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x102)
    goto L_02009f74;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f9c:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x144)
    goto L_02009f9c;
  FIELD_AT(v5, s16, 0) = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Func_02001730();
  Actor_FaceDirectionAndWait(8, 0x5000);
  t24 = ((s32)Actor_Get(8));
  FIELD_AT(t24, u8, 90) = (254 & FIELD_AT(t24, u8, 90));
  t25 = ((s32)Actor_Get(8));
  v6 = t25;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  FieldActor_Jump(8, 6, 0);
  Actor_MoveToAndWait(8, 200, 0x29e);
  FieldActor_Jump(8, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(10, 0x5000);
  t33 = ((s32)Actor_Get(10));
  FIELD_AT(t33, u8, 90) = (254 & FIELD_AT(t33, u8, 90));
  t34 = ((s32)Actor_Get(10));
  v6 = t34;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(10, 0x26666, 0x13333);
  FieldActor_Jump(10, 6, 0);
  Actor_MoveToAndWait(10, 200, 0x29e);
  FieldActor_Jump(10, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  t42 = ((s32)Actor_Get(9));
  FIELD_AT(t42, u8, 90) = (254 & FIELD_AT(t42, u8, 90));
  t43 = ((s32)Actor_Get(9));
  v6 = t43;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(9, 0x26666, 0x13333);
  FieldActor_Jump(9, 6, 0);
  Actor_MoveToAndWait(9, 200, 0x29e);
  FieldActor_Jump(9, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, 0xb000);
  t51 = ((s32)Actor_Get(11));
  v5 = (254 & FIELD_AT(t51, u8, 90));
  FIELD_AT(t51, u8, 90) = v5;
  t52 = ((s32)Actor_Get(11));
  v6 = t52;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(11, 0x26666, 0x13333);
  FieldActor_Jump(11, 6, 0);
  Actor_MoveToAndWait(11, 200, 0x29e);
  FieldActor_Jump(11, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldActor_WaitForMoveById(11);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
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
  Actor_FaceDirection(8, 0x5000, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(11, FACING_WEST, 20);
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(123);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(15);
}

void Func_02002244(void) {
  if (Event_GetFlag(0x300)) {
    SET_POSITION(Actor_Get(9), 0xa80000, 0, 0xc00000);
  }
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1) {
    if (gMainRoutineWork.entrance == 15)
      Func_020023c8();
    else
      Func_02002244();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1 &&
      Event_GetFlag(0x300))
    MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
  return 0;
}

void Func_020022f4(void) {}

void Func_020022f8(void) {
  s32 step;
  s32 i;
  struct Actor *object = Actor_Get(9);
  struct TileSprite *sprite = object->sprite;

  Event_SetFlag(0x300);
  object->acceleration = 0x18000;
  object->speedLimit = 0x80000;
  Event_PlaySound(227);
  FieldActor_SetMoveTarget(object, 0xb00000, 0x280000, 0xb80000);
  step = 0;
  for (i = 15; i >= 0; i--) {
    sprite->tile -= step;
    System_Wait(1);
    step += 40;
  }
  FieldActor_SetMoveTarget(object, 0xa80000, 0x100000, 0xb80000);
  step = 400;
  for (i = 25; i >= 0; i--) {
    sprite->tile -= step;
    System_Wait(1);
    step += 40;
  }
  FieldActor_WaitForMove(object);
  FieldEvent_Wait(2);
  Event_PlaySound(240);
  i = 0;
  sprite->tile = i;
  object->x = 0xa80000;
  object->z = 0xc00000;
  object->y = i;
  object->velocityY = i;
  object->velocityX = i;
  MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
}

/// Yallam's view of the two objects Func_020023c8 restarts.

/// Hands objects 9 and 10 to Func_0200014c's update, object 9 with its timer
/// at 1 and object 10 at 0.
void Func_020023c8(void) {
  struct YallamActor *actor = ((struct YallamActor *)Actor_Get(9));
  struct YallamActor *other;

  actor->timer = 1;
  actor->kind = 0;
  actor->update = Func_0200014c;
  other = ((struct YallamActor *)Actor_Get(10));
  other->timer = 0;
  other->kind = 0;
  other->update = Func_0200014c;
}
