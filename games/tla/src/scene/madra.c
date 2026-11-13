/// @file
/// The scene code of overlay 0x658, in ROM order.

// ---- games/tla/src/scene/madra.c ----

#include "shared/routines.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "event.h"
#include "event_work.h"
#include "field_event.h"
#include "flag_call.h"
#include "module/game/game.h"
#include "module/scene/madra.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "module/window/window.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02002ff0[];

const u8 *Scene_GetPlacements(void) {
  if (GameFlag_Test(0x8ff))
    return gData_02002ff0;
  return gData_02002d38;
}

/* VIEW(madra): variable gData_02003308 as extern struct MadraPlacement
 * gData_02003308[]; the definition has s32 gData_02003308[] = { (s32)
 * gData_020032fc, 0x30014, (s32) gData_020032f0, 0x17000d, (s32)
 * gData_020032f0, 0x110011, (s32) gData_020032f0...; tried as the definition's
 * extern s32 gData_02003308[], it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:71: request for member `x' in something not a
 * structure or union. */
extern struct MadraPlacement gData_02003308__madra[] __asm__("gData_02003308");

void Func_0200006c(void) {
  struct MadraEventWork *work;
  struct Actor *object;
  s16 *trigger;
  u32 id;
  s32 index;

  work = Ram_HeapSlots->eventWork;
  Event_Start();
  Event_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_GetById(id);
  if (object != NULL) {
    object->motionFlags = 0;
  }
  id++;
  if (id < 64)
    goto next;
  trigger = &work->value154;
  index = *trigger - 1;
  Sound_Play(158);
  {
    s32 x = gData_02003308__madra[index].x;
    s32 y = gData_02003308__madra[index].y;

    Map_PlayMapEdit(gData_02003308__madra[index].data, x, y);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02000108(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(4, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgItLooksLikeThe);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgTheArmorerAnd);
  else
    Event_SetMessageId((s32)&MsgAfterTheChampa);
  Event_ShowMessageAndWait(29, 0);
}

void Func_0200018c(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(5, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgPeopleAreSaying);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgIKnewThere);
  else
    Event_SetMessageId((s32)&MsgOwOwOw);
  Event_ShowMessageAndWait(30, 0);
}

void Func_02000210(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(6, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgMyBoyfriendShin);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgAllMyInjuries);
  else
    Event_SetMessageId((s32)&MsgMySisterWas);
  Event_ShowMessageAndWait(arg, 0);
}

void Func_02000294(void) {
  Event_SetMessageId((s32)&MsgIfYouDon);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
}

void Func_020002bc(void) {
  s32 a, b;

  Event_SetMessageId(MsgHaveYouBy);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    a = 0x103;
    b = 30;
    /* FAKEMATCH(barrier): a and b held at an empty asm, so 0x103 and the frame
     * count are built before the id, and again for the second call (with no
     * asm the calls share 0x103 in r5); as no asm, 14 of 38 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+r"(a), "+r"(b));
    Actor_ShowEmoteById(33, a, b);
    FieldMessage_ShowAtActor(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      a = 0x103;
      b = 30;
      Actor_ShowEmoteById(33, a, b);
      Event_ShowMessageThenWait(33, 0, 10);
    } else {
      Func_02000294();
    }
  } else {
    Func_02000294();
  }
}

struct Actor *Actor_GetById(u32 objectId);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Map_PlayMapEdit(const void *data, s32 a, s32 b);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 y,
                                                    s32 z);
/* VIEW(events_b): routine Actor_ShowEmoteById as void Actor_ShowEmoteById(s32
 * id, s32 a, s32 b); event.h has s32 Actor_ShowEmoteById(s32 actorId, s32
 * emote, s32 frames); tried as the definition's struct
 * BattleEffectWatchedObject *Actor_ShowEmoteById(s32 resourceId, s32 flags, s32
 * state), it does not compile in tla-ja: games/tla/src/scene/madra.c:176:
 * conflicting types for `Actor_ShowEmoteById'. */
extern void Func_080d47b4__events_b(s32 id, s32 a,
                                    s32 b) __asm__("Actor_ShowEmoteById");

extern const u8 gData_02002ff0[];

/* VIEW(events_b): variable MsgUsingPsynergyOn as extern u8 MsgUsingPsynergyOn;
 * msg_ids.h has extern u8 MsgUsingPsynergyOn[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja: games/tla/src/scene/madra.c:191:
 * `MsgUsingPsynergyOn__events_b' undeclared (first use in this function). */
extern u8 MsgUsingPsynergyOn__events_b __asm__("MsgUsingPsynergyOn");

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

void Func_02000320(void) {
  s32 x, z;

  Event_SetMessageId((s32)&MsgUsingPsynergyOn__events_b);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  x = 0x108;
  z = 30;
  /* FAKEMATCH(barrier): x and z held at an empty asm, so both distances load
   * before the actor id, as the native has them; as no asm, 2 of 27
   * instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(x), "+r"(z));
  Func_080d47b4__events_b(33, x, z);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(10);
  Event_ShowMessageThenWait(33, 0, 10);
}

void Func_0200036c(void) {
  if (Flag_Test(0x20f) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Func_080d47b4__events_b(33, 0x105, 30);
    Event_SetMessageId((s32)&MsgWhereCouldMenardi);
    Event_ShowMessageThenWait(33, 0, 10);
    Event_End();
    GameFlag_SetBit(0x20f);
  }
}

void Func_020003b8(s32 actor) {
  s32 msg = (s32)&MsgEveryoneHereIn;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_02000404(void) {
  s32 msg;

  Actor_SetAngleToward(26, 4, 0);
  if (Flag_Test(0x281)) {
    Event_SetMessageId((s32)&MsgYeahWellWe);
  } else {
    msg = (s32)&MsgYouCominThrough;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(26, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      GameFlag_SetBit(0x281);
      Event_Wait(10);
      Func_080d47b4__events_b(26, 0x102, 0);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(26, 3);
      Event_SetMessageId(msg + 2);
    }
  }
  Event_ShowMessageAndWait(26, 0);
}

void Func_02000490(void) {
  s32 msg;

  Actor_SetAngleToward(26, 4, 0);
  if (Flag_Test(0x281)) {
    Event_SetMessageId((s32)&MsgYeahUhWe);
  } else {
    msg = (s32)&MsgOhUhYou;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(26, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      GameFlag_SetBit(0x281);
      Event_Wait(10);
      Func_080d47b4__events_b(26, 0x102, 0);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(26, 3);
      Event_SetMessageId(msg + 2);
    }
  }
  Event_ShowMessageAndWait(26, 0);
}

// ---- games/tla/src/scene/madra/s051c.c ----
// Scene code of overlay 0x658, in ROM order, from s051c's offset up to the next
// object the overlay links.

#include "event_work.h"
#include "field_sprite.h"

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle held in a block local, as overlay       \
     * 6a3's PLACE has it, so it is built first and shifted before the         \
     * positions; as the angle passed straight to the call, 8 of 60            \
     * instructions of Func_0200051c differ in tla-en, and 8 of 51 of          \
     * Func_020005b8. */                                                       \
    s32 angle_ = (angle);                                                      \
    Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle_);          \
  }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjSetPos(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void ObjCall2bf8(s32 id, s32 a) {
  Actor_ArmCallbackAndRefresh(id, a);
}

void Func_0200051c(void) {
  if (Event_GetFlag(0x910)) {
    Sound_Play(123);
    Import_EventSetExitRequest(16);
  } else if (!Event_GetFlag(0x280)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    PLACE(26, PIXELS(744), PIXELS(257), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(0x280);
    Func_02000404();
    Event_End();
  }
}

void Func_020005b8(void) {
  if (!Event_GetFlag(0x280)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    PLACE(26, PIXELS(744), PIXELS(256), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(0x280);
    Func_02000490();
    Event_End();
  }
}

extern const u8 gData_02002ff0[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

const u8 *Scene_GetEvents(void) {
  if (GameFlag_Test(0x8ff))
    return gData_0200394c;
  if (GameFlag_Test(0x8ab))
    return gData_0200367c;
  return gData_02003358;
}

void Func_02000670(void) { CopyCells(109, 14, 3, 3, 45, 14); }

void Func_0200068c(void) {
  MapGrid_CopyCellsLayered(51, 10, 3, 3, 45, 10);
  Func_02000670();
}

/// Madra's prepare step: sets up actor 26 (larger, its sprite flags set)
/// and places actors 26 to 28, 33 and 66 by the story flags, then runs
/// Func_02000f80 on entrance 9 and Func_020019d8 once on entrance 7.
s32 Scene_Initialize(void) {
  struct Actor *object;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (Event_GetFlag(0x8ff)) {
    Func_0200068c();
  } else {
    Actor_SetSingleChildFlags(Actor_GetById(26), 0);
    object = Actor_GetById(26);
    object->radius = Actor_GetById(26)->radius * 4 / 3;
    ((struct FieldSprite *)Actor_GetById(26)->sprite)->nextFrame |= 8;
    {
      struct FieldSprite *sprite = Actor_GetById(26)->sprite;

      sprite->unk11 = (sprite->unk11 & 3) | 32;
    }
    ((struct FieldSprite *)Actor_GetById(26)->sprite)->unk19 = 1;
    if (Event_GetFlag(0x8ff) == 0 && Event_GetFlag(0x8ab) != 0) {
      Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
      if (Event_GetFlag(0x280) == 0)
        Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    } else if (Event_GetFlag(0x8ff) != 0 || Event_GetFlag(0x910) != 0 ||
               Event_GetFlag(0x280) == 0) {
      Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 9 && Event_GetFlag(0x913) == 0)
    Func_02000f80();
  if (Event_GetFlag(0x8ff) != 0 && Event_GetFlag(0x989) != 0) {
    if (gMainRoutineWork.entrance == 7 && Event_GetFlag(0x98e) == 0) {
      Func_020019d8();
      Event_SetFlag(0x98e);
    }
    if (Event_GetFlag(0x98e))
      Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  } else {
    Actor_SetHorizontalPositionWithTerrain(66, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200083c(s32 actor) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgWasIJust;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

void Func_02000894(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x8ab)) {
    msg = (s32)&MsgHeyMisterAre;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  } else {
    if (GameFlag_Test(0x300) == 0)
      Actor_SetAngleToward(14, 4, 0);
    msg = (s32)&MsgDidYouCome;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  }
  Actor_ArmCallback(14, FACING_SOUTH, 0);
  GameFlag_SetBit(0x300);
  Event_End();
}

void Func_02000968(void) {
  s32 msg, x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x8ab)) {
    msg = (s32)&MsgHeyMisterAre;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  } else {
    if (GameFlag_Test(0x300) == 0) {
      x = 0x100;
      z = 30;
      Actor_ShowEmoteById(14, x, z);
      Actor_SetAngleToward(14, 4, 0);
    }
    msg = (s32)&MsgDidYouCome;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  }
  Actor_ArmCallback(14, FACING_SOUTH, 0);
  GameFlag_SetBit(0x300);
  Event_End();
}

void Func_02000a48(void) {
  s32 flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(11, 4, 0);
  Event_SetMessageId((s32)&MsgIsThereBy);
  flags = 0;
  FieldMessage_ShowAtActor(11, flags);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    GameFlag_SetBit(0x301);
    Event_ShowMessageAndWait(11, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageAndWait(11, 0);
  }
  Event_End();
}
#undef PLACE

struct Actor *Actor_GetById(u32 objectId);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_LaunchFromLeader(s32 id, s32 a, s32 b, s32 c);
/* VIEW(s0ac4): routine Map_PlayMapEdit as s32 Map_PlayMapEdit(const void *edit,
 * s32 a, s32 b); events_b has void Map_PlayMapEdit(const void *data, s32 a, s32
 * b); tried as the definition's void Map_PlayMapEdit(u16 *steps, s32
 * destinationX, u32 destinationY), it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:547: conflicting types for `Map_PlayMapEdit'. */
extern s32 Map_PlayMapEdit__s0ac4(const void *edit, s32 a,
                                  s32 b) __asm__("Map_PlayMapEdit");
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
void Func_02002c50(s32 item, s32 a);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void FieldMessage_Show(s32 message, s32 position);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);

static __inline__ struct Actor *Emote(s32 id, s32 emote, s32 frames) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 8 of 475 instructions
   * of Func_02000ac4 differ in tla-en, 25 of 549 of Func_02000f80 and 27 of 490
   * of Func_020014fc. */
  return Actor_ShowEmoteById(id, emote, frames);
}

/// Actors 24 and 25 talk with 32 and 4, 25 is moved about with its priority
/// bit cleared and set, and 32 is placed at the current owner before flag
/// 0x910 is set. It opens with "You there, hold".
void Func_02000ac4(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgYouThereHold);
  Event_ShowMessageThenWait(24, 0, 2);
  FieldCamera_MoveTo(238 << 17, -1, 210 << 17, 1);
  Actor_WalkBy(25, 3, 8);
  Actor_WalkByAndWait(24, -3, 8);
  Actor_WaitForMoveById(25);
  Actor_WalkToAndWait(4, 472, 426);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  FieldActor_LaunchFromLeader(32, 16, 1, FACING_NORTH);
  Func_02002c28();
  Actor_WaitForMoveById(32);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(32, FACING_WEST, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(32, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(32, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_SetAngleToward(24, 25, 0);
  Event_Wait(10);
  Actor_SetAngleToward(25, 24, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(30);
  Actor_SetAngleToward(25, 24, 0);
  Event_Wait(10);
  Actor_SetAngleToward(25, 32, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(30);
  Actor_FaceDirection(24, FACING_SOUTH + FACING_STEP, 0);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  Actor_WalkToAndWait(25, 457, 402);
  Actor_WalkByAndWait(25, 16, 0);
  Actor_WalkToAndWait(25, 472, 420);
  Event_Wait(30);
  Actor_SetPositionAndReset(25, 484, 420);
  Actor_GetById(25)->priorityFlags |= 1;
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 4);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_ShowMessageThenWait(24, 0, 2);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(5);
  Event_ShowMessageThenWait(25, 0, 2);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  Actor_WalkByAndWait(25, 0, -16);
  Actor_WalkToAndWait(25, 475, 400);
  Actor_WalkByAndWait(25, -16, 0);
  Actor_GetById(25)->priorityFlags |= 1;
  FieldCamera_MoveTo(238 << 17, -1, 198 << 17, 1);
  Actor_WalkToAndWait(25, 459, 416);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Emote(32, 0x101, 50);
  Event_ShowMessageThenWait(32, 0, 2);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 2);
  Event_Wait(10);
  Emote(32, 0x100, 0);
  Emote(4, 0x100, 50);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(25, 4);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(4, 32, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(32, FACING_NORTH, 0);
  Emote(25, 0x109, 50);
  Event_ShowMessageThenWait(24, 0, 2);
  Emote(25, 0x103, 20);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(30);
  Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessageThenWait(24, 0, 2);
  Actor_SetSpeed(24, 0x4ccc, 0x2666);
  Actor_SetSpeed(25, 0x4ccc, 0x2666);
  Actor_WalkBy(24, 3, -8);
  Actor_OffsetPositionAndReset(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(24, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetModeById(32, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Actor_SetModeById(32, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  GameFlag_SetBit(0x910);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  Event_End();
}

/// Once flag 0x911 is set: actors 33 and 34 talk with 4, the prompt's answer
/// is counted in the event work either way, and flag 0x913 is set. The talk
/// includes "... has escaped".
void Func_02000f80(void) {
  if (!GameFlag_Test(0x911))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(124), -1, PIXELS(420), 1);
  Func_02002c28();
  Actor_SetSpeed(33, 0xcccc, 0x6666);
  Actor_SetSpeed(34, 0xcccc, 0x6666);
  Event_StartScreenTransition();
  Actor_SetPosition(33, PIXELS(172), PIXELS(332));
  Actor_SetHorizontalPositionWithTerrain(34, PIXELS(172), PIXELS(324));
  Event_SetMessageId(MsgHasEscaped2);
  Event_WaitTransitionFrames();
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_WalkTo(33, 156, 372);
  Event_Wait(20);
  Actor_WalkTo(34, 156, 372);
  Actor_WalkTo(4, 76, 388);
  Actor_WaitForMoveById(33);
  Actor_WalkTo(33, 108, 380);
  Actor_WaitForMoveById(34);
  Actor_WalkTo(34, 124, 380);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  Actor_WaitForMoveById(33);
  Actor_WaitForMoveById(34);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Actor_FaceDirection(34, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Actor_FaceDirection(34, FACING_NORTH, 0);
  Actor_SetAttachedEffect(34, 258);
  Actor_SetAttachedEffect(33, 258);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(34, 3);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Actor_WalkTo(33, 102, 340);
  Event_Wait(20);
  Actor_WalkTo(34, 102, 340);
  Actor_WaitForMoveById(33);
  Actor_SetPosition(33, PIXELS(4), PIXELS(4));
  Actor_WaitForMoveById(34);
  Actor_SetPosition(34, PIXELS(4), PIXELS(4));
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Emote(4, 258, 30);
  Event_Wait(40);
  Event_Wait(120);
  Actor_SetPosition(33, PIXELS(102), PIXELS(340));
  Actor_SetPosition(34, PIXELS(102), PIXELS(340));
  Actor_WalkToAndWait(34, 100, 364);
  Actor_WalkTo(34, 124, 380);
  Actor_WalkToAndWait(33, 100, 380);
  Actor_WaitForMoveById(34);
  Actor_WalkTo(34, 132, 396);
  Actor_WalkToAndWait(33, 108, 396);
  Actor_WaitForMoveById(34);
  Actor_SetAngleToward(4, 33, 0);
  Actor_FaceDirection(34, FACING_NORTH, 0);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Emote(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetAngleToward(34, 33, 0);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_SetAngleToward(33, 34, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_ShowEmoteById(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_FaceDirection(33, FACING_NORTH, 0);
  Actor_ArmCallback(34, FACING_NORTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Event_Wait(20);
  Actor_ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  Actor_ShowEmoteById(33, 257, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Actor_SetAngleToward(34, 33, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Actor_SetAngleToward(33, 34, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetAngleToward(33, 4, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_ShowEmoteById(4, 261, 30);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Event_Wait(20);
  Actor_ShowEmoteById(33, 262, 30);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_ShowMessageThenWait(33, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(33, 0, 10);
  }
  Actor_SetAngleToward(33, 34, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_ShowEmoteById(34, 258, 30);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(60);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_CommitPositionAndActivate(34, 6, 0);
  Actor_WalkToAndWait(34, 164, 316);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(33, FACING_WEST);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Actor_SetHorizontalPositionWithTerrain(34, 0, 0);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  GameFlag_SetBit(0x913);
  Actor_CommitPositionAndActivate(33, 30, 0);
  Actor_WalkToAndWait(33, 164, 316);
  Event_Wait(20);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Event_Wait(20);
  Event_End();
}

/// Actors 24, 25 and 32 question 4, whose answers are counted in the event
/// work; after flag 0x98f 32 asks again. It opens with "Hey, you lot"; 32's
/// second question is "It was them". Then 32 is placed at the current owner and
/// flag 0x914 is set.
void Func_020014fc(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgHeyYouLot);
  Event_ShowMessageThenWait(24, 0, 2);
  Camera_MoveWithinMapBounds(PIXELS(476), -1, PIXELS(400), 1);
  Actor_WalkByAndWait(24, -3, 8);
  Actor_SetPositionAndReset(4, 472, 426);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_LaunchFromLeader(32, 16, 1, FACING_NORTH);
  Func_02002c28();
  Actor_WaitForMoveById(32);
  Actor_CommitPositionAndActivate(25, 3, 8);
  Actor_ArmCallbackAndRefresh(25, FACING_EAST);
  Actor_SetModeById(25, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(25, 0, 10);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(32, FACING_WEST, 0);
  Event_Wait(20);
  Actor_ShowEmoteById(32, 0x101, 30);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetModeAndWaitAnimation(4, 4);
  Actor_FaceDirectionAndWait(24, FACING_SOUTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmoteById(32, 0x103, 0);
  Actor_Jump(32, 6, 15);
  Actor_Jump(32, 6, 23);
  Event_ShowMessageAndWait(32, 0);
  Emote(4, 0x100, 30);
  Actor_SetAngleToward(4, 24, 0);
  Event_Wait(20);
  Actor_ShowEmoteById(32, 0x100, 30);
  Actor_FaceDirectionAndWait(32, FACING_NORTH + FACING_STEP);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTHEAST);
  Motion_SetModeAndWaitAnimation(25, 3);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(10);
  FieldMessage_ShowAtActor(25, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTHWEST + FACING_STEP);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(25);
    Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
    Event_ShowMessageThenWait(25, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Event_Wait(25);
    Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(25, 0, 10);
  }
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(32, FACING_NORTHWEST + FACING_STEP);
  Event_ShowMessageAndWait(32, 0);
  Actor_ShowEmoteById(25, 0x102, 30);
  Event_ShowMessageThenWait(25, 0, 10);
  Motion_SetVarCbAndRefresh(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 10);
  if (GameFlag_Test(0x98f)) {
    Event_SetMessageId(MsgItWasThem);
    Actor_ArmCallback(32, FACING_WEST, 0);
    Event_Wait(20);
    FieldMessage_ShowAtActor(32, 0);
    Actor_ArmCallback(4, FACING_EAST, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(32, 3);
      Event_ShowMessageAndWait(32, 0);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Event_Wait(20);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Motion_SetModeAndWaitAnimation(32, 4);
      Event_ShowMessageAndWait(32, 0);
    }
  }
  Event_SetMessageId(MsgWasAnythingStolen);
  Actor_FaceDirection(32, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(4, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Event_Wait(40);
  Emote(24, 0x101, 30);
  Actor_FaceDirection(24, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmoteById(25, 0x102, 30);
  Motion_SetModeAndWaitAnimation(25, 4);
  Event_Wait(10);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  Event_ShowMessageThenWait(25, 0, 10);
  Emote(32, 0x101, 0);
  Actor_ShowEmoteById(4, 0x101, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceDirectionAndWait(24, FACING_SOUTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_Wait(10);
  Event_ShowMessageThenWait(24, 0, 10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(25, 0, 10);
  Actor_SetSpeed(24, 0x4ccc, 0x2666);
  Actor_SetSpeed(25, 0x4ccc, 0x2666);
  Actor_WalkBy(24, 3, -8);
  Actor_OffsetPositionAndReset(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  Actor_FaceDirection(25, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(24, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(32, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetModeById(32, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(10);
  GameFlag_SetBit(0x914);
  Actor_SetModeById(32, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_End();
}

/* VIEW(s0ac4): variable gData_02003308 as extern const struct
 * CellPictureCopyEntry gData_02003308[]; madra has extern struct MadraPlacement
 * gData_02003308[]; tried as the definition's extern s32 gData_02003308[], it
 * does not compile in tla-ja: games/tla/src/scene/madra.c:1085: request for
 * member `steps' in something not a structure or union. */
extern const struct CellPictureCopyEntry
    gData_02003308__s0ac4[] __asm__("gData_02003308");

// The macro's do-while starts each mode change's own scheduling region; as
// plain calls, 88 of 1565 instructions differ in tla-en.
#define SET_MODE(id, mode)                                                     \
  do {                                                                         \
    Actor_SetModeById(id, mode);                                               \
  } while (0)

/// Actor 34 questions 4 and the others; the answers are counted in the event
/// work. It opens with "Wait". With room for item 210 it is given and flag
/// 0x918 is set, otherwise the mayor's two lines are shown. 33 then talks with
/// the group, and they all gather at 4 before flags 0x980 and 0x98e are set.
void Func_020019d8(void) {
  struct Actor *object;
  s32 x = 153 << 16;
  s32 z;

  z = 0x01030000;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  if (GameFlag_Test(0x825) || GameFlag_Test(0x826))
    GameFlag_SetBit(0xa76);
  Event_SetMessageId(MsgWait3);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_WalkToAndWait(4, 150, 280);
  Actor_OffsetPositionAndReset(4, 16, 0);
  FieldActor_LaunchFromLeader(5, 8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(32, 0, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, -8, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(7, -16, 0, FACING_NORTH);
  Actor_WaitForMoveById(4);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(20);
  Map_PlayMapEdit__s0ac4(gData_02003308__s0ac4[6].steps,
                         gData_02003308__s0ac4[6].unk04,
                         gData_02003308__s0ac4[6].unk06);
  Actor_SetHorizontalPositionAndAngleWithTerrain(34, x, z, FACING_SOUTH);
  Actor_CommitPositionAndActivate(34, 0, 4);
  SET_MODE(34, 3);
  /* FAKEMATCH(dead): this wait in a once-only block, which starts its own
   * scheduling region; as a plain call, whose message call then loads its
   * arguments in the other order, 2 of 1565 instructions differ in tla-en. */
  do {
    Event_Wait(20);
  } while (0);
  FieldMessage_ShowAtActor(34, 0);
  Actor_SetAngleToward(32, 4, 0);
  Actor_SetAngleToward(5, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Actor_ArmCallback(34, FACING_SOUTH, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_FaceDirection(32, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    Actor_FaceDirection(7, FACING_NORTH, 0);
    Actor_FaceDirection(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    Event_ShowMessageAndWait(34, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Actor_FaceDirection(32, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    Actor_FaceDirection(7, FACING_NORTH, 0);
    Actor_FaceDirection(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(34, 0, 0);
  }
  Emote(34, 0x102, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_ShowEmoteById(6, 0x102, 30);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(34, 0x100, 30);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_CommitPositionAndActivate(34, 16, 8);
  Actor_FaceDirection(34, FACING_SOUTH, 0);
  if (PartyInventory_FindRoom(210) >= 0) {
    Func_02002c50(210, 3);
    PartyInventory_GiveItem(210, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Actor_ArmCallback(4, FACING_NORTH, 0);
    GameFlag_SetBit(0x918);
  } else {
    s32 count = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) + 1;

    Event_SetMessageId(MsgTheMayorTried);
    MessageArgument_Push(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Motion_SetVarCbAndRefresh(34, 2);
    Event_ShowMessageAndWait(34, 0);
    Actor_CommitPositionAndActivate(34, 16, 0);
    MessageArgument_Push(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Actor_WalkByAndWait(34, -16, 0);
    Actor_FaceDirectionAndWait(34, FACING_SOUTH);
    Actor_SetPosition(66, PIXELS(192), PIXELS(256));
    *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) = count;
  }
  Actor_WalkByAndWait(34, -16, -8);
  Actor_FaceDirectionAndWait(34, FACING_SOUTH);
  Actor_ShowEmoteById(34, 0x105, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_SetModeById(32, 3);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(34, FACING_NORTH);
  Actor_WalkToAndWait(34, 153, 259);
  Actor_SetHorizontalPositionWithTerrain(34, 0, 0);
  SET_MODE(32, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Event_ShowMessageAndWait(33, 0);
  Sound_Play(78);
  Emote(4, 0x101, 0);
  Emote(5, 0x101, 0);
  Emote(7, 0x101, 0);
  Emote(6, 0x101, 0);
  Actor_ShowEmoteById(32, 0x101, 30);
  Actor_SetAngleToward(4, 33, 0);
  Actor_SetAngleToward(7, 33, 0);
  Actor_SetAngleToward(6, 33, 0);
  Actor_SetAngleToward(5, 33, 0);
  Actor_SetAngleToward(32, 33, 0);
  Event_Wait(20);
  Camera_FollowActor(33, 1);
  Event_Wait(60);
  Sound_Play(36);
  Emote(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_FaceDirection(33, FACING_WEST, 0);
  Actor_TurnToFaceEachOther(6, 5, 0);
  Actor_TurnToFaceEachOther(7, 32, 0);
  Event_Wait(40);
  Emote(4, 0x102, 30);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_SetAngleToward(4, 33, 0);
  Actor_SetAngleToward(7, 33, 0);
  Actor_SetAngleToward(6, 33, 0);
  Actor_SetAngleToward(5, 33, 0);
  Actor_SetAngleToward(32, 33, 0);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(33, FACING_WEST);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(6, 0x102, 30);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_ShowEmoteById(5, 0x100, 30);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_SetAngleToward(33, 4, 0);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
    Event_ShowMessageAndWait(33, 0);
    Actor_ShowEmoteById(4, 0x102, 30);
    Event_Wait(40);
    SET_MODE(33, 3);
    Event_Wait(20);
    Event_Wait(20);
    Event_ShowMessageAndWait(33, 0);
    Actor_TurnToFaceEachOther(6, 5, 0);
    Event_Wait(40);
    Actor_SetAngleToward(6, 33, 0);
    Actor_SetAngleToward(5, 33, 0);
    Event_Wait(40);
    Actor_SetModeById(5, 4);
    SET_MODE(6, 4);
    Event_Wait(33);
    Event_Wait(20);
    Actor_ShowEmoteById(33, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  } else {
    Event_SetMessageId(MsgICanTBelieveIt);
    Event_ShowMessageAndWait(33, 0);
    Actor_TurnToFaceEachOther(6, 5, 0);
    Event_Wait(40);
    Actor_SetAngleToward(6, 33, 0);
    Actor_SetAngleToward(5, 33, 0);
    Event_Wait(40);
    Actor_SetModeById(5, 4);
    SET_MODE(6, 4);
    Event_Wait(33);
    Event_Wait(20);
    Actor_ShowEmoteById(6, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  }
  Event_SetMessageId(MsgWhoIsThis);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(32, 4);
  SET_MODE(4, 4);
  SET_MODE(7, 4);
  SET_MODE(6, 4);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(4, 0x109, 0);
  Emote(5, 0x109, 0);
  Emote(6, 0x109, 0);
  Actor_ShowEmoteById(32, 0x109, 30);
  Event_Wait(60);
  Actor_ShowEmoteById(33, 0x106, 30);
  Event_ShowMessageThenWait(33, 0, 0);
  Emote(4, 0x102, 0);
  Emote(5, 0x102, 0);
  Emote(6, 0x102, 0);
  Actor_ShowEmoteById(32, 0x102, 30);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(32, 0x100, 30);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(4, 0x103, 0);
  Emote(6, 0x103, 0);
  Emote(5, 0x103, 0);
  Emote(7, 0x103, 30);
  Actor_FaceDirection(33, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(33, 0x108, 30);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  Event_Wait(20);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(4, 0x100, 0);
  Emote(7, 0x100, 0);
  Emote(5, 0x100, 0);
  Emote(6, 0x100, 0);
  Actor_ShowEmoteById(4, 0x100, 30);
  Actor_ShowEmoteById(33, 0x101, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(33, FACING_WEST);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_ShowEmoteById(33, 0x108, 30);
    Event_ShowMessageThenWait(33, 0, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Actor_ShowEmoteById(33, 0x103, 30);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(33, 0, 0);
  }
  Emote(32, 0x102, 30);
  SET_MODE(32, 4);
  Event_Wait(33);
  SET_MODE(32, 4);
  Event_Wait(33);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(33, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallback(33, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Sound_Play(78);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_CommitPositionAndActivate(33, 24, 24);
  Actor_CommitPositionAndActivate(33, 132, 0);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Party_PlaySavedMusic();
  Camera_FollowActor(4, 1);
  Actor_ShowEmoteById(6, 0x102, 30);
  Event_ShowMessageAndWait(6, 0);
  SET_MODE(5, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Emote(7, 0x101, 30);
  Actor_SetAngleToward(7, 4, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 0);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_FaceDirection(32, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_ShowEmoteById(7, 0x101, 20);
  Actor_SetAngleToward(7, 32, 0);
  Actor_SetAngleToward(4, 32, 0);
  Actor_SetAngleToward(6, 32, 0);
  Actor_SetAngleToward(5, 32, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 0);
  Actor_SetAngleToward(32, 7, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 10);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Actor_TurnToFaceEachOther(4, 7, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(32, FACING_SOUTH);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 0);
  Event_Wait(20);
  Actor_ShowEmoteById(5, 0x100, 30);
  Actor_SetAngleToward(5, 32, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Emote(6, 0x108, 30);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 10);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 0);
  Actor_SetAngleToward(4, 32, 0);
  Actor_SetAngleToward(5, 32, 0);
  Actor_SetAngleToward(6, 32, 0);
  Actor_SetAngleToward(7, 32, 0);
  Event_Wait(40);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(5, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_ShowEmoteById(32, 0x108, 30);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(32, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(32, FACING_EAST);
  Event_ShowMessageAndWait(32, 0);
  SET_MODE(4, 3);
  Event_Wait(20);
  Event_Wait(20);
  SET_MODE(6, 3);
  SET_MODE(5, 3);
  Event_Wait(20);
  SET_MODE(4, 3);
  Event_Wait(20);
  SET_MODE(32, 3);
  /* FAKEMATCH(dead): this wait in a once-only block, which starts its own
   * scheduling region; as a plain call, whose speed call then loads its
   * arguments in the other order, 2 of 1565 instructions differ in tla-en. */
  do {
    Event_Wait(20);
  } while (0);
  Actor_SetSpeed(32, 0x00013333, 0x9999);
  SET_MODE(32, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  Actor_SetSpeed(5, 0x00013333, 0x9999);
  SET_MODE(5, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(5, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Actor_SetSpeed(6, 0x00013333, 0x9999);
  SET_MODE(6, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(6, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Actor_SetSpeed(7, 0x00013333, 0x9999);
  SET_MODE(7, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(7, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_SetBit(0x980);
  GameFlag_SetBit(0x98e);
  Event_End();
}
#undef SET_MODE

// ---- games/tla/src/scene/madra/events_d.c ----
#include "text/msg_ids.h"

/* VIEW(events_d): variable gData_02002d38 as extern s16 gData_02002d38[]; s051c
 * has extern const u8 gData_02002d38[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja: games/tla/src/scene/madra.c:1663:
 * conflicting types for `gData_02002d38'. */
extern s16 gData_02002d38__events_d[] __asm__("gData_02002d38");
/* VIEW(events_d): variable gData_02002ff0 as extern struct ActorPlacement
 * gData_02002ff0[]; s051c has extern const u8 gData_02002ff0[]; tried as the
 * definition's extern u32 gData_02002ff0[], it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:1554: conflicting types for `gData_02002ff0'. */
extern struct ActorPlacement
    gData_02002ff0__events_d[] __asm__("gData_02002ff0");
/* VIEW(events_d): variable gData_02003358 as extern struct Trigger
 * gData_02003358[]; s051c has extern const u8 gData_02003358[]; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:1762: conflicting types for `gData_02003358'. */
extern struct Trigger gData_02003358__events_d[] __asm__("gData_02003358");
/* VIEW(events_d): variable gData_0200367c as extern struct Trigger
 * gData_0200367c[]; s051c has extern const u8 gData_0200367c[]; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:1832: conflicting types for `gData_0200367c'. */
extern struct Trigger gData_0200367c__events_d[] __asm__("gData_0200367c");
/* VIEW(events_d): variable gData_0200394c as extern struct Trigger
 * gData_0200394c[]; s051c has extern const u8 gData_0200394c[]; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:1895: conflicting types for `gData_0200394c'. */
extern struct Trigger gData_0200394c__events_d[] __asm__("gData_0200394c");

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_020029ac(void) { GameFlag_SetBit(0x910); }

void Func_020029b8(void) { GameFlag_SetBit(0x914); }

void Func_020029c8(void) {
  s32 next;

  next = 16;
  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154) > 34)
    next = 15;
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(10);
  Sound_Play(123);
  Import_EventSetExitRequest(next);
  Event_End();
}

void Func_02002a58(void) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  id = 4;
  Actor_CommitPositionAndActivate(id, 16, 0);
  Func_020029c8();
}

void Func_02002a80(void) {
  s32 id, x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * distances and their negation; as no asm, 2 of 18 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_CommitPositionAndActivate(id, -x, -z);
  Func_020029c8();
}

DEFINE_FUNC_02000544(Func_02002ab0, Func_020029c8)

void Func_0200006c(void);
void Func_020002bc(void);
void Func_0200051c(void);
void Func_02000a48(void);
void Func_020014fc(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(44),
    SCENE_EXIT_IF(1, 46, 1, -1),
    SCENE_EXIT_IF(2, 45, 1, -1),
    SCENE_EXIT_IF(3, 45, 2, -1),
    SCENE_EXIT_IF(4, 45, 8, -1),
    SCENE_EXIT_IF(5, 45, 10, -1),
    SCENE_EXIT_IF(6, 45, 15, -1),
    SCENE_EXIT_IF(7, 45, 5, -1),
    SCENE_EXIT_IF(8, 45, 9, -1),
    SCENE_EXIT_IF(9, 45, 20, -1),
    SCENE_EXIT_IF(10, 46, 2, -1),
    SCENE_EXIT_IF(11, 2, 9, -1),
    SCENE_EXIT_IF(15, 48, 1, -1),
    SCENE_EXIT_IF(16, 48, 3, -1),
    SCENE_EXITS_END,
};

// clang-format off
s16 gData_02002d38__events_d[] = {
    71, -1, 1, 0, 0, 136, 0, 0,
    0, 296, 0x3000, 1, 72, -1, 1, 0,
    0, 744, 0, 0, 0, 392, 0xc000, 1,
    73, -1, 1, 0, 0, 312, 0, 0,
    0, 360, 0x4000, 1, 74, -1, 1, 0,
    0, 184, 0, 0, 0, 440, 0xa000, 1,
    75, -1, 1, 0, 0, 344, 0, 0,
    0, 280, 0x4000, 1, 76, -1, 1, 0,
    0, 408, 0, 0, 0, 384, 0xc000, 1,
    77, -1, 1, 0, 0, 274, 0, 0,
    0, 510, 0x4000, 1, 78, -1, 1, 0,
    0, 312, 0, 0, 0, 376, 0xc000, 1,
    75, -1, 1, 0, 0, 472, 0, 0,
    0, 264, 0x2000, 1, 73, -1, 1, 0,
    0, 504, 0, 0, 0, 232, 0x4000, 0,
    224, -1, 1, 0, 0, 472, 0, 0,
    0, 296, 0, 1, 78, -1, 1, 0,
    0, 552, 0, 0, 0, 259, 0x4000, 1,
    71, -1, 1, 0, 0, 72, 0, 0,
    0, 472, 0xe000, 1, 72, -1, 1, 0,
    0, 136, 0, 0, 0, 440, 0xc000, 1,
    187, -1, 1, 0, 0, 88, 0, 0,
    0, 360, 0x5000, 1, 187, -1, 1, 0,
    0, 128, 0, 0, 0, 360, 0x5000, 1,
    71, -1, 1, 0, 0, 504, 0, 0,
    0, 408, 0x4000, 1, 71, -1, 1, 0,
    0, 456, 0, 0, 0, 408, 0x4000, 1,
    71, -1, 1, 0, 0, 744, 0, 0,
    0, 248, 0xc000, 0, 187, 0x1911, 1, 0,
    0, 112, 0, 0, 0, 440, 0xc000, 1,
    192, 0x1911, 1, 0, 0, 88, 0, 0,
    0, 440, 0xc000, 1, 205, -1, 1, 0,
    0, 376, 0, 0, 0, 272, 0x4000, 1,
    206, -1, 1, 0, 0, 328, 0, 0,
    0, 256, 0x4000, 1, 198, 0x18ab, 1, 0,
    0, 280, 0, 0, 0, 256, 0x4000, 1,
    56, -1, 1, 0, 0, 8, 0, 0,
    0, 8, 0xc000, 1, 200, -1, 1, 0,
    0, 8, 0, 0, 0, 8, 0xc000, 1,
    88, -1, 1, 0, 0, 8, 0, 0,
    0, 8, 0xc000, 1, 186, 0x8ab, 1, 0,
    0, 280, 0, 0, 0, 256, 0x4000, 1,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

struct ActorPlacement gData_02002ff0__events_d[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(150), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(392), FACING_NORTH, 1, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(360), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(440), FACING_NORTHWEST, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(280), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(392), FACING_NORTH, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(274), 0, PIXELS(510), FACING_SOUTH, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(376), FACING_NORTH, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(264), FACING_SOUTH, 1, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(232), FACING_SOUTH, 0, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(296), FACING_SOUTH + FACING_STEP, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(259), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(472), FACING_NORTHEAST, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(440), FACING_NORTHWEST, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {187, 0x1911, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {192, 0x1911, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {205, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(272), FACING_SOUTH, 1, 0},
    {206, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(256), FACING_SOUTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {23, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(276), FACING_SOUTHEAST, 1, 0},
    {20, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 1, 0},
    {238, CONDITION_ALWAYS, 2, PIXELS(112), 0, PIXELS(393), FACING_SOUTH, 1, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
    {SCENE_TABLE_END},
};

s16 gData_020032f0[] = {
    1, 39, 1, 2, 6, -1,
};

s16 gData_020032fc[] = {
    3, 39, 1, 2, 6, -1,
};
// clang-format on

s32 gData_02003308[] = {
    (s32)gData_020032fc, 0x30014,  (s32)gData_020032f0, 0x17000d,
    (s32)gData_020032f0, 0x110011, (s32)gData_020032f0, 0x120017,
    (s32)gData_020032f0, 0xa001c,  (s32)gData_020032f0, 0x6001c,
    (s32)gData_020032f0, 0x60009,  (s32)gData_020032f0, 0x180013,
    (s32)gData_020032f0, 0,        (s32)gData_020032fc, 0x30011,
};

struct Trigger gData_02003358__events_d[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {TRIGGER_STEP, 19, 0x910, EVENT(Func_020029ac)},
    {TRIGGER_STEP, 20, 0x910, EVENT(Func_02000ac4)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTheTownElder)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYouPickedThe)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgThatBigWave)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLetMeGuess)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgMaybeWeLl)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIfICould)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02000894)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgItSLike)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheChampaWere)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgThoseChampaCurs)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgIDoubtThe)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgIGotSomeone)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgIDonTCare)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeTookThat)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgMyFriendWas)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgMyPartnerWas)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgThereSNoWay)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAllOfIndra)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Func_02000404)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, EVENT(Func_0200083c)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgEvenIfWe)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(Func_02000108)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {TRIGGER_TALK, 35, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneThinksBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgTheTownElderIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIfABig)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgItSABit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgThatChampaIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgIVeAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1038, 0x300, EVENT(Func_02000968)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgWhoWasIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgNowMadraIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWhenISaw)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgTheChampaMust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgItSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIMNotSure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgILlNever)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgILookedInto)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIWonderHowMany)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIMissMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgItSSoWeird)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgTheseAreRough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgTheChampaDug)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgItWasAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgMySoreBottom)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgTheItemVendor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgThoseLousyChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgBriggsAndHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS,
     MSGNO(MsgBriggsAndHis)},
    {TRIGGERS_END},
};

struct Trigger gData_0200367c__events_d[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_020005b8)},
    {TRIGGER_STEP, 19, 0x914, EVENT(Func_020029b8)},
    {TRIGGER_STEP, 20, 0x914, EVENT(Func_020014fc)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgWeThinkThe)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSoTheKibombo)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTheKibomboCame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgIHeardThatThe)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgIGuessThat)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgGondowanSoundsLike)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02000894)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgICanTBelieveSomeone)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgNowThatWe)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgThereAreStill)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgBriggsDidnT)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheseCavesWere)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgThatGuyWasn)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgOhWeReleased)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgEveryoneYelledAt)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgYouShouldaSeen)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgFirstPiratesRaid)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgMyGuessIs)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Func_02000490)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(Func_02000108)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIfItWerenT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIfGondowanIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgItSStrangeIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgTheMayorIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgHeToldUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgInGondowanThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIHeardThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIfTheyWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgItWasSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhoDug)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgWaitIfBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIVeNeverSeenAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgDidnTSeem)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgWasSoHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIDidnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgICanTBelieveNobody)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgTheWayThings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgIVeBeenStanding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgWeSealedUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgTheChampaWeapons)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgTheGroupThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgThatShinIs)},
    {TRIGGERS_END},
};

struct Trigger gData_0200394c__events_d[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 21, 0x98e, EVENT(Func_0200036c)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIDonTThink)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgWeGoTo)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIHeardTheBoat)},
    {TRIGGER_TALK, 11, 0x301, EVENT(Func_02000a48)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgThoseTravelersLooked)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgIWonderIfThe)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhatWill)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAGroupOfSharp)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIfYouHaveA)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgIWasAt)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgItSNotThat)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTheMayorAsked)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgItSeemsNo)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSayThereWas)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIThoughtShin)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgUhhhnnnUhhhnnn2)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTheMayorFinally)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgHeyItS)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgYeahWellWe)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(Func_02000108)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {TRIGGER_TALK, 33, CONDITION_ALWAYS, EVENT(Func_020002bc)},
    {TRIGGER_TALK, 35, CONDITION_ALWAYS, EVENT(Func_020003b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgWhyWouldAnyoneBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgTheyVeBeenHaving)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneGetsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgWhatWasThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgThePeopleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgThatIslandThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgHowBigWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgTheseNewBoats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgIfABuilding)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgSailorsAreSupposed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgOurMayorS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgIsTurningWater)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgMadraIsAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIKnewI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgHeWasGone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgHowMuchLongerDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgThePiratesAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgThatNightThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgShinSeemsTerrified)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS,
     EVENT(Func_02000320)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS,
     MSGNO(MsgWeReStill)},
    {TRIGGERS_END},
};
