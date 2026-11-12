/// @file
/// Scene code of overlay 0x64b, in ROM order, from s2e78's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "input.h"
#include "scene_ids.h"
#include "event_inline.h"
#include "frame.h"
#include "iwram_call.h"

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004d38(s32 scene, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004d08_(s32 actor, s32 value, s32 frames) {
  Func_080d47b4(actor, value, frames);
}

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_Ov667.
void Func_02002e00(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  {
    /* FAKEMATCH(pin): the mode pinned to r3 starts right after the 1 negated
     * for y, before x loads; as a plain constant, 2 of 39 instructions differ
     * in tla-en. */
    register s32 mode asm("r3") = 1;

    Camera_MoveWithinMapBounds(11890 << 16, -1, 10090 << 16, mode);
  }
  // Entered at its test: as a while loop the test is copied ahead of it and
  // gInput hoisted (differ 17).
  goto wait;
  do {
    wait(1);
  wait:;
  } while (gInput.held == 0);
  Func_02004d38((s32)&SceneId_Ov667, 80);
}

void Func_02002e78(void) {
  s32 scale;
  s32 seen = Event_GetFlag(0x8de);

  if (seen == 0) {
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    scale = Actor_GetById(gMainRoutineWork.currentOwner)->scaleX;
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
    Event_Wait(40);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(4, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 264, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldMessage_ShowAtActor(7, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(8, 265, 30);
      FieldEvent_ShowMessageThenWait(9, 0, 10);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Actor_ShowEmote(8, 265, 30);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
      FieldEvent_ShowMessageThenWait(9, 0, 10);
    }
    Func_02004d08_(8, 256, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Func_02004d08_(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    Func_02004d08_(8, 264, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    Actor_ShowEmote(8, 258, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 264, 30);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
    gMainRoutineWork.renderMode = 7;
    gMainRoutineWork.ownerOverride = 1;
    Func_02004db0();
    Event_SetFlag(0x8de);
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(6, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(5, 0, 0);
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = scale;
    Func_02004c28();
  }
}

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_02004c20(void);

void Func_0200310c(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToAndWait(8, 10720, 9075);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(66);
}

extern s32 Data_02006be8[8];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_0200050c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004dc0(void);

void Func_02003144(void) {
  s32 x, z, i;

  if (GameFlag_Test(0x8df) == 0)
    return;
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa78);
  Actor_MoveToAndWait(8, 0x1bf6, 0x2154);
  Func_0200050c();
  x = Actor_Get(8)->x / 0x10000;
  z = Actor_Get(8)->z / 0x10000;
  i = 0;
  {
    do {
      if (Actor_Get(i) != NULL) {
        Data_02006be8[i] = i;
        Actor_Get(i)->scaleX = 0;
        Actor_SetSingleChildFlags(Actor_Get(i), 0);
        Actor_SetPosition(i, x << 16, z << 16);
      } else {
        Data_02006be8[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_Get(23)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_Get(23), 0);
  Actor_SetPosition(23, x << 16, z << 16);
  Func_02004da8();
  FieldEvent_SetMessageId((s32)MsgWeDidIt);
  Event_PlaySound(390);
  Event_ShowMessage(2, 0);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  Event_PlaySound(78);
  FieldEvent_Wait(50);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(38);
  Event_ShowMessage(6, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(3, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(2, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(1, 0);
  FieldEvent_Wait(40);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(60);
  Event_ShowMessage(6, 0);
  Event_PlaySound(78);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(40);
  Event_ShowMessage(23, 0);
  Func_02004dc0();
  Func_02004db0();
  i = 0;
  {
  restore:
    if (Data_02006be8[i] >= 0) {
      Actor_SetSingleChildFlags(Actor_Get(Data_02006be8[i]), 1);
      Actor_Get(Data_02006be8[i])->scaleX = 0x10000;
      Actor_SetPosition(Data_02006be8[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Actor_SetPosition(23, 0, 0);
  Func_02004c28();
}

extern u8 gData_02006c08[];
extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
u32 Random16Far(void);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02003654(s32 a, s32 b, s32 c);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020036a8(void);
void Func_020048f4(void);
void Func_02003bb8(struct Actor *object, s32 value);
void Func_02004aa8(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
u32 __umodsi3(u32 a, u32 b);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

s32 Func_02003418(struct Actor *object) {
  if (Random16Far() < 0x4000)
    Animation_ApplyChildValues(object, Random16Far() * 8 >> 16);
  return 0;
}

extern const u8 gData_02004f70[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004b18(struct Actor *object, s32 a);
void Func_02004b20(struct Actor *object, const u8 *data);

void Func_0200343c(struct Actor *parent) {
  if ((gFrameCount & 7) == 0) {
    s32 x = parent->x;
    s32 y = parent->y;
    s32 z = parent->z;
    struct Actor *object = (FieldActor_Create(673, x, y, z));

    if (object != NULL) {
      Func_02004b20(object, gData_02004f70);
      Func_02004b18(object, 1);
      Actor_SetSingleChildFlags(object, 0);
      object->motionFlags = 0;
      object->scaleX = 0x20000;
      object->scaleY = 0x20000;
    }
  }
}

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02004c20(void);
void Func_02004c28(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  Actor_SetPositionAndCommit(actor, x, z);
}

void Func_02003490(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_0200343c;
  Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x2000;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit = 0x20000;
  System_Wait(5);
  FieldEffect_ScheduleRatioTransition(0xc000, 60);
  Event_StartScreenTransition();
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14537, 8577);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14465, 8662);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14452, 8766);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14387, 8828);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14331, 8888);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14284, 8904);
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner, 14255, 8870);
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner, 14255, 8831);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14255, 8772);
  FieldEffect_ScheduleRatioTransition(0x14000, 120);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14275, 8736);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c28();
  FieldEvent_SetExitRequest(78);
}

extern u8 gData_02006c08[];
extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
u32 Random16Far(void);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02003654(s32 a, s32 b, s32 c);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020036a8(void);
void Func_020048f4(void);
void Func_02003bb8(struct Actor *object, s32 value);
void Func_02004aa8(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
u32 __umodsi3(u32 a, u32 b);

s32 Func_020035e0(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}
