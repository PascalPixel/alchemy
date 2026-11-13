/// @file
/// The scene code of overlay 0x658 that its scene tables belong to, in ROM
/// order; the rest of the overlay is in madra/.

// ---- games/tla/src/scene/madra.c ----

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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];

const u8 *Func_0200004c(void) {
  if (GameFlag_Test(0x8ff))
    return gData_02002ff0;
  return gData_02002d38;
}

struct MadraEventWork {
  u8 unk000[0x154];
  s16 value154;
};

struct MadraPlacement {
  const void *data;
  u16 x;
  u16 y;
};

/* VIEW(madra): variable gData_02003308 as extern struct MadraPlacement
 * gData_02003308[]; the definition has s32 gData_02003308[] = { (s32)
 * gData_020032fc, 0x30014, (s32) gData_020032f0, 0x17000d, (s32)
 * gData_020032f0, 0x110011, (s32) gData_020032f0...; tried as the definition's
 * extern s32 gData_02003308[], it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:77: request for member `x' in something not a
 * structure or union. */
extern struct MadraPlacement gData_02003308__madra[] __asm__("gData_02003308");

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002b28(void);
void Func_02002b30(void);

void Func_0200006c(void) {
  struct MadraEventWork *work;
  struct Actor *object;
  s16 *trigger;
  u32 id;
  s32 index;

  work = Ram_HeapSlots->eventWork;
  Func_02002b28();
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
  Func_02002b30();
}

void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe[];
extern u8 MsgTheArmorerAnd[];
extern u8 MsgAfterTheChampa[];
extern u8 MsgPeopleAreSaying[];
extern u8 MsgIKnewThere[];
extern u8 MsgOwOwOw[];
extern u8 MsgMyBoyfriendShin[];
extern u8 MsgAllMyInjuries[];
extern u8 MsgMySisterWas[];
extern u8 MsgIfYouDon[];
extern u8 MsgHaveYouBy[];
extern u8 MsgWhereCouldMenardi[];
extern u8 MsgEveryoneHereIn[];
extern u8 MsgYeahWellWe[];
extern u8 MsgYouCominThrough[];
extern u8 MsgYeahUhWe[];
extern u8 MsgOhUhYou[];
extern u8 MsgWasIJust[];
extern u8 MsgHeyMisterAre[];
extern u8 MsgDidYouCome[];
extern u8 MsgIsThereBy[];

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
    Func_080d47b4(33, a, b);
    FieldMessage_ShowAtActor(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      a = 0x103;
      b = 30;
      Func_080d47b4(33, a, b);
      Event_ShowMessageThenWait(33, 0, 10);
    } else {
      Func_02000294();
    }
  } else {
    Func_02000294();
  }
}

// ---- games/tla/src/scene/madra/events_b.c ----
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void Event_Wait(s32 frames);
struct Actor *Actor_GetById(u32 objectId);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
s32 FieldMessage_ShowAtActor(s32 localId, s32 unused);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Map_PlayMapEdit(const void *data, s32 a, s32 b);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 destX, s32 destY);
void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002b28(void);
void Func_02002b30(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 y,
                                                    s32 z);
void Event_SetMessageId(s32 message);
void Event_ShowMessageThenWait(s32 id, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 a, s32 b);
void Actor_ArmCallbackAndRefresh(s32 id, s32 a);
/* VIEW(events_b): routine Func_080d47b4 as void Func_080d47b4(s32 id, s32 a,
 * s32 b); event.h has s32 Func_080d47b4(s32 actorId, s32 emote, s32 frames);
 * tried as the definition's struct BattleEffectWatchedObject *Func_080d47b4(s32
 * resourceId, s32 flags, s32 state), it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:235: conflicting types for `Func_080d47b4'. */
extern void Func_080d47b4__events_b(s32 id, s32 a,
                                    s32 b) __asm__("Func_080d47b4");
void Import_EventSetExitRequest(s32 a);
void Event_ResolvePendingActions(s32 a);
s32 Party_GetLeaderActor(void);
void Func_02002c88(s32 a, s32 b);
void Sound_Play(s32 a);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];
extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe[];
extern u8 MsgTheArmorerAnd[];
extern u8 MsgAfterTheChampa[];
extern u8 MsgPeopleAreSaying[];
extern u8 MsgIKnewThere[];
extern u8 MsgOwOwOw[];
extern u8 MsgMyBoyfriendShin[];
extern u8 MsgAllMyInjuries[];
extern u8 MsgMySisterWas[];
extern u8 MsgIfYouDon[];
/* VIEW(events_b): variable MsgUsingPsynergyOn as extern u8 MsgUsingPsynergyOn;
 * msg_ids.h has extern u8 MsgUsingPsynergyOn[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja: games/tla/src/scene/madra.c:281:
 * `MsgUsingPsynergyOn__events_b' undeclared (first use in this function). */
extern u8 MsgUsingPsynergyOn__events_b __asm__("MsgUsingPsynergyOn");
extern u8 MsgWhereCouldMenardi[];
extern u8 MsgEveryoneHereIn[];
extern u8 MsgYeahWellWe[];
extern u8 MsgYouCominThrough[];
extern u8 MsgYeahUhWe[];
extern u8 MsgOhUhYou[];
extern u8 MsgWasIJust[];
extern u8 MsgHeyMisterAre[];
extern u8 MsgDidYouCome[];
extern u8 MsgIsThereBy[];

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
    Func_02002b28();
    Event_ResolvePendingActions(0);
    Func_080d47b4__events_b(33, 0x105, 30);
    Event_SetMessageId((s32)&MsgWhereCouldMenardi);
    Event_ShowMessageThenWait(33, 0, 10);
    Func_02002b30();
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

void Func_02000404(void);
void Func_02000490(void);
void Func_02002b28(void);
void Func_02002b30(void);

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
    Func_02002b28();
    Event_ResolvePendingActions(0);
    PLACE(26, PIXELS(744), PIXELS(257), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(0x280);
    Func_02000404();
    Func_02002b30();
  }
}

void Func_020005b8(void) {
  if (!Event_GetFlag(0x280)) {
    Func_02002b28();
    Event_ResolvePendingActions(0);
    PLACE(26, PIXELS(744), PIXELS(256), FACING_SOUTH);
    Actor_GetById(4)->velocityY = PIXELS(5);
    ObjSpeed(4, 0x20000, 0x10000);
    ObjSetPos(4, 720, 244);
    ObjCall2bf8(4, FACING_SOUTHEAST);
    PLACE(4, PIXELS(720), PIXELS(244), FACING_SOUTHEAST);
    Event_SetFlag(0x280);
    Func_02000490();
    Func_02002b30();
  }
}

void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];
extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe[];
extern u8 MsgTheArmorerAnd[];
extern u8 MsgAfterTheChampa[];
extern u8 MsgPeopleAreSaying[];
extern u8 MsgIKnewThere[];
extern u8 MsgOwOwOw[];
extern u8 MsgMyBoyfriendShin[];
extern u8 MsgAllMyInjuries[];
extern u8 MsgMySisterWas[];
extern u8 MsgIfYouDon[];
extern u8 MsgWhereCouldMenardi[];
extern u8 MsgEveryoneHereIn[];
extern u8 MsgYeahWellWe[];
extern u8 MsgYouCominThrough[];
extern u8 MsgYeahUhWe[];
extern u8 MsgOhUhYou[];
extern u8 MsgWasIJust[];
extern u8 MsgHeyMisterAre[];
extern u8 MsgDidYouCome[];
extern u8 MsgIsThereBy[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

const u8 *Func_02000638(void) {
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
s32 Func_020006ac(void) {
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

  Func_02002b28();
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
  Func_02002b30();
}

void Func_02000894(void) {
  s32 msg;

  Func_02002b28();
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
  Func_02002b30();
}

void Func_02000968(void) {
  s32 msg, x, z;

  Func_02002b28();
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
      Func_080d47b4(14, x, z);
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
  Func_02002b30();
}

void Func_02000a48(void) {
  s32 flags;

  Func_02002b28();
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
  Func_02002b30();
}
#undef PLACE

// ---- games/tla/src/scene/madra/s0ac4.c ----
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Event_Wait(s32 frames);
struct Actor *Actor_GetById(u32 objectId);
void Actor_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_OffsetPositionAndReset(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
s32 FieldMessage_ShowAtActor(s32 localId, s32 unused);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002b28(void);
void Func_02002b30(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
/* VIEW(s0ac4): routine Event_SetMessageId as void Event_SetMessageId(const u8
 * *message); event_work.h has void Event_SetMessageId(s32 message). */
extern void
Event_SetMessageId__s0ac4(const u8 *message) __asm__("Event_SetMessageId");
void Event_ShowMessageThenWait(s32 id, s32 a, s32 b);
void Actor_ArmCallbackAndRefresh(s32 id, s32 a);
/* VIEW(s0ac4): routine Func_080d47b4 as struct Actor *Func_080d47b4(s32 id, s32
 * emote, s32 frames); event.h has s32 Func_080d47b4(s32 actorId, s32 emote, s32
 * frames). */
extern struct Actor *Func_080d47b4__s0ac4(s32 id, s32 emote,
                                          s32 frames) __asm__("Func_080d47b4");
void FieldEffect_SetAttached(s32 id, s32 a);
void Func_02002c28(void);
void Event_ResolvePendingActions(s32 a);
void Event_ShowMessageAndWait(s32 id, s32 a);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_LaunchFromLeader(s32 id, s32 a, s32 b, s32 c);
/* VIEW(s0ac4): routine Map_PlayMapEdit as s32 Map_PlayMapEdit(const void *edit,
 * s32 a, s32 b); events_b has void Map_PlayMapEdit(const void *data, s32 a, s32
 * b); tried as the definition's void Map_PlayMapEdit(u16 *steps, s32
 * destinationX, u32 destinationY), it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:695: conflicting types for `Map_PlayMapEdit'. */
extern s32 Map_PlayMapEdit__s0ac4(const void *edit, s32 a,
                                  s32 b) __asm__("Map_PlayMapEdit");
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
void Func_02002b10(s32 item, s32 a);
s32 Func_02002b18(s32 item);
void Func_02002c50(s32 item, s32 a);
void Func_02002c60(void);
void Sound_Play(s32 cue);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void FieldMessage_Show(s32 message, s32 position);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);

/* VIEW(s0ac4): variable MsgYouThereHold as extern const u8 MsgYouThereHold[];
 * msg_ids.h has extern u8 MsgYouThereHold[]. */
extern const u8 MsgYouThereHold__s0ac4[] __asm__("MsgYouThereHold");
/* VIEW(s0ac4): variable MsgHasEscaped2 as extern const u8 MsgHasEscaped2[];
 * msg_ids.h has extern u8 MsgHasEscaped2[]. */
extern const u8 MsgHasEscaped2__s0ac4[] __asm__("MsgHasEscaped2");
/* VIEW(s0ac4): variable MsgHeyYouLot as extern const u8 MsgHeyYouLot[];
 * msg_ids.h has extern u8 MsgHeyYouLot[]. */
extern const u8 MsgHeyYouLot__s0ac4[] __asm__("MsgHeyYouLot");
/* VIEW(s0ac4): variable MsgItWasThem as extern const u8 MsgItWasThem[];
 * msg_ids.h has extern u8 MsgItWasThem[]. */
extern const u8 MsgItWasThem__s0ac4[] __asm__("MsgItWasThem");
/* VIEW(s0ac4): variable MsgWasAnythingStolen as extern const u8
 * MsgWasAnythingStolen[]; msg_ids.h has extern u8 MsgWasAnythingStolen[]. */
extern const u8 MsgWasAnythingStolen__s0ac4[] __asm__("MsgWasAnythingStolen");
/* VIEW(s0ac4): variable MsgWait3 as extern const u8 MsgWait3[]; msg_ids.h has
 * extern u8 MsgWait3[]. */
extern const u8 MsgWait3__s0ac4[] __asm__("MsgWait3");
/* VIEW(s0ac4): variable MsgTheMayorTried as extern const u8 MsgTheMayorTried[];
 * msg_ids.h has extern u8 MsgTheMayorTried[]. */
extern const u8 MsgTheMayorTried__s0ac4[] __asm__("MsgTheMayorTried");
/* VIEW(s0ac4): variable MsgICanTBelieveIt as extern const u8
 * MsgICanTBelieveIt[]; msg_ids.h has extern u8 MsgICanTBelieveIt[]. */
extern const u8 MsgICanTBelieveIt__s0ac4[] __asm__("MsgICanTBelieveIt");
/* VIEW(s0ac4): variable MsgWhoIsThis as extern const u8 MsgWhoIsThis[];
 * msg_ids.h has extern u8 MsgWhoIsThis[]. */
extern const u8 MsgWhoIsThis__s0ac4[] __asm__("MsgWhoIsThis");
extern u8 MsgYouThereHold[];
extern u8 MsgHasEscaped2[];
extern u8 MsgHeyYouLot[];
extern u8 MsgItWasThem[];
extern u8 MsgWasAnythingStolen[];
extern u8 MsgWait3[];
extern u8 MsgTheMayorTried[];
extern u8 MsgICanTBelieveIt[];
extern u8 MsgWhoIsThis[];

static __inline__ void ArmCallback(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 72 of 475
   * instructions of Func_02000ac4 differ in tla-en, 36 of 549 of Func_02000f80
   * and 64 of 490 of Func_020014fc. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void OffsetPos(u32 id, s32 dx, s32 dz) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 10 of 475
   * instructions of Func_02000ac4 differ in tla-en and 12 of 490 of
   * Func_020014fc. */
  Actor_OffsetPositionAndReset(id, dx, dz);
}

static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 31 of 475
   * instructions of Func_02000ac4 differ in tla-en and 2 of 490 of
   * Func_020014fc. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 24 of 475
   * instructions of Func_02000ac4 differ in tla-en, 30 of 549 of Func_02000f80
   * and 28 of 490 of Func_020014fc. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 51 of 475
   * instructions of Func_02000ac4 differ in tla-en and 20 of 549 of
   * Func_02000f80. */
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 d) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 33 of 475
   * instructions of Func_02000ac4 differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, d);
}

static __inline__ void ArmCallbackAndRefresh(s32 id, s32 a) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 44 of 475
   * instructions of Func_02000ac4 differ in tla-en, 4 of 549 of Func_02000f80
   * and 48 of 490 of Func_020014fc. */
  Actor_ArmCallbackAndRefresh(id, a);
}

static __inline__ struct Actor *Emote(s32 id, s32 emote, s32 frames) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 8 of 475 instructions
   * of Func_02000ac4 differ in tla-en, 25 of 549 of Func_02000f80 and 27 of 490
   * of Func_020014fc. */
  return Func_080d47b4__s0ac4(id, emote, frames);
}

static __inline__ void LaunchFromFocused(s32 id, s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 36 of 475
   * instructions of Func_02000ac4 differ in tla-en. */
  Actor_LaunchFromLeader(id, a, b, c);
}

static __inline__ void SetPosInMode2(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 48 of 549
   * instructions of Func_02000f80 differ in tla-en. */
  Actor_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void SetHorizontalPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 50 of 549
   * instructions of Func_02000f80 differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Madra_Func_02002c10(s32 id, s32 a) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 31 of 549
   * instructions of Func_02000f80 differ in tla-en. */
  FieldEffect_SetAttached(id, a);
}

/// Actors 24 and 25 talk with 32 and 4, 25 is moved about with its priority
/// bit cleared and set, and 32 is placed at the current owner before flag
/// 0x910 is set. It opens with "You there, hold".
void Func_02000ac4(void) {
  struct Actor *object;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  Event_SetMessageId__s0ac4(MsgYouThereHold__s0ac4);
  Event_ShowMessageThenWait(24, 0, 2);
  CamBounds(238 << 17, -1, 210 << 17, 1);
  OffsetPos(25, 3, 8);
  CommitPos(24, -3, 8);
  Actor_WaitForMoveById(25);
  SetPos(4, 472, 426);
  Event_Wait(10);
  ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  LaunchFromFocused(32, 16, 1, FACING_NORTH);
  Func_02002c28();
  Actor_WaitForMoveById(32);
  ArmCallback(4, FACING_EAST, 0);
  ArmCallback(32, FACING_WEST, 0);
  Event_Wait(30);
  Event_Wait(10);
  ArmCallback(4, FACING_NORTH, 0);
  ArmCallback(32, FACING_NORTH, 0);
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
  ArmCallback(24, FACING_SOUTH + FACING_STEP, 0);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  SetPos(25, 457, 402);
  CommitPos(25, 16, 0);
  SetPos(25, 472, 420);
  Event_Wait(30);
  Actor_SetPositionAndReset(25, 484, 420);
  Actor_GetById(25)->priorityFlags |= 1;
  ArmCallback(25, FACING_SOUTH, 0);
  Event_Wait(60);
  Event_Wait(10);
  ArmCallback(25, FACING_NORTH, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 4);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_ShowMessageThenWait(24, 0, 2);
  ArmCallbackAndRefresh(25, FACING_SOUTH);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(5);
  Event_ShowMessageThenWait(25, 0, 2);
  Actor_GetById(25)->priorityFlags &= 0xfe;
  Actor_SetSpritePriority(25, 3);
  CommitPos(25, 0, -16);
  SetPos(25, 475, 400);
  CommitPos(25, -16, 0);
  Actor_GetById(25)->priorityFlags |= 1;
  CamBounds(238 << 17, -1, 198 << 17, 1);
  SetPos(25, 459, 416);
  ArmCallback(25, FACING_SOUTHEAST, 0);
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
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(4, 32, 0);
  Event_Wait(20);
  Event_Wait(10);
  ArmCallback(4, FACING_NORTH, 0);
  ArmCallback(32, FACING_NORTH, 0);
  Emote(25, 0x109, 50);
  Event_ShowMessageThenWait(24, 0, 2);
  Emote(25, 0x103, 20);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessageThenWait(25, 0, 2);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(30);
  ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessageThenWait(24, 0, 2);
  Speed(24, 0x4ccc, 0x2666);
  Speed(25, 0x4ccc, 0x2666);
  OffsetPos(24, 3, -8);
  Actor_OffsetPositionAndReset(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  ArmCallback(25, FACING_SOUTHEAST + FACING_STEP, 0);
  ArmCallback(24, FACING_SOUTH + FACING_STEP, 0);
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
  Func_02002b30();
}

/// Once flag 0x911 is set: actors 33 and 34 talk with 4, the prompt's answer
/// is counted in the event work either way, and flag 0x913 is set. The talk
/// includes "... has escaped".
void Func_02000f80(void) {
  if (!GameFlag_Test(0x911))
    return;
  Func_02002b28();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(124), -1, PIXELS(420), 1);
  Func_02002c28();
  Speed(33, 0xcccc, 0x6666);
  Speed(34, 0xcccc, 0x6666);
  Event_StartScreenTransition();
  SetHorizontalPos(33, PIXELS(172), PIXELS(332));
  Actor_SetHorizontalPositionWithTerrain(34, PIXELS(172), PIXELS(324));
  Event_SetMessageId__s0ac4(MsgHasEscaped2__s0ac4);
  Event_WaitTransitionFrames();
  Event_ShowMessageThenWait(33, 0, 10);
  SetPosInMode2(33, 156, 372);
  Event_Wait(20);
  SetPosInMode2(34, 156, 372);
  SetPosInMode2(4, 76, 388);
  Actor_WaitForMoveById(33);
  SetPosInMode2(33, 108, 380);
  Actor_WaitForMoveById(34);
  SetPosInMode2(34, 124, 380);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  Actor_WaitForMoveById(33);
  Actor_WaitForMoveById(34);
  ArmCallback(33, FACING_NORTH, 0);
  ArmCallback(34, FACING_NORTH, 0);
  Event_Wait(20);
  ArmCallback(33, FACING_NORTH, 0);
  ArmCallback(34, FACING_NORTH, 0);
  Madra_Func_02002c10(34, 258);
  Madra_Func_02002c10(33, 258);
  Event_Wait(50);
  Motion_SetModeAndWaitAnimation(34, 3);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  SetPosInMode2(33, 102, 340);
  Event_Wait(20);
  SetPosInMode2(34, 102, 340);
  Actor_WaitForMoveById(33);
  SetHorizontalPos(33, PIXELS(4), PIXELS(4));
  Actor_WaitForMoveById(34);
  SetHorizontalPos(34, PIXELS(4), PIXELS(4));
  Actor_ArmCallback(4, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Emote(4, 258, 30);
  Event_Wait(40);
  Event_Wait(120);
  SetHorizontalPos(33, PIXELS(102), PIXELS(340));
  SetHorizontalPos(34, PIXELS(102), PIXELS(340));
  SetPos(34, 100, 364);
  SetPosInMode2(34, 124, 380);
  SetPos(33, 100, 380);
  Actor_WaitForMoveById(34);
  SetPosInMode2(34, 132, 396);
  SetPos(33, 108, 396);
  Actor_WaitForMoveById(34);
  Actor_SetAngleToward(4, 33, 0);
  ArmCallback(34, FACING_NORTH, 0);
  ArmCallback(33, FACING_NORTH, 0);
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
  Func_080d47b4__s0ac4(33, 258, 30);
  Event_ShowMessageThenWait(33, 0, 10);
  ArmCallback(33, FACING_NORTH, 0);
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
  Func_080d47b4__s0ac4(33, 257, 30);
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
  Func_080d47b4__s0ac4(4, 261, 30);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 10);
  Event_Wait(20);
  Func_080d47b4__s0ac4(33, 262, 30);
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
  Func_080d47b4__s0ac4(34, 258, 30);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(60);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_CommitPositionAndActivate(34, 6, 0);
  SetPos(34, 164, 316);
  Event_Wait(10);
  ArmCallbackAndRefresh(33, FACING_WEST);
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
  SetPos(33, 164, 316);
  Event_Wait(20);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Event_Wait(20);
  Func_02002b30();
}

/// Actors 24, 25 and 32 question 4, whose answers are counted in the event
/// work; after flag 0x98f 32 asks again. It opens with "Hey, you lot"; 32's
/// second question is "It was them". Then 32 is placed at the current owner and
/// flag 0x914 is set.
void Func_020014fc(void) {
  struct Actor *object;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  Event_SetMessageId__s0ac4(MsgHeyYouLot__s0ac4);
  Event_ShowMessageThenWait(24, 0, 2);
  Camera_MoveWithinMapBounds(PIXELS(476), -1, PIXELS(400), 1);
  CommitPos(24, -3, 8);
  Actor_SetPositionAndReset(4, 472, 426);
  Event_Wait(10);
  ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
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
  Func_080d47b4__s0ac4(32, 0x101, 30);
  Event_ShowMessageAndWait(32, 0);
  Motion_SetModeAndWaitAnimation(4, 4);
  ArmCallbackAndRefresh(24, FACING_SOUTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_ShowMessageThenWait(24, 0, 10);
  ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
  Func_080d47b4__s0ac4(32, 0x103, 0);
  Actor_Jump(32, 6, 15);
  Actor_Jump(32, 6, 23);
  Event_ShowMessageAndWait(32, 0);
  Emote(4, 0x100, 30);
  Actor_SetAngleToward(4, 24, 0);
  Event_Wait(20);
  Func_080d47b4__s0ac4(32, 0x100, 30);
  ArmCallbackAndRefresh(32, FACING_NORTH + FACING_STEP);
  Event_ShowMessageAndWait(32, 0);
  ArmCallbackAndRefresh(25, FACING_SOUTHEAST);
  Motion_SetModeAndWaitAnimation(25, 3);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(10);
  FieldMessage_ShowAtActor(25, 0);
  ArmCallbackAndRefresh(4, FACING_NORTHWEST + FACING_STEP);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(25);
    ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
    Event_ShowMessageThenWait(25, 0, 10);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Event_Wait(25);
    ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(25, 0, 10);
  }
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  ArmCallbackAndRefresh(32, FACING_NORTHWEST + FACING_STEP);
  Event_ShowMessageAndWait(32, 0);
  Func_080d47b4__s0ac4(25, 0x102, 30);
  Event_ShowMessageThenWait(25, 0, 10);
  Motion_SetVarCbAndRefresh(24, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(24, 0, 10);
  if (GameFlag_Test(0x98f)) {
    Event_SetMessageId__s0ac4(MsgItWasThem__s0ac4);
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
  Event_SetMessageId__s0ac4(MsgWasAnythingStolen__s0ac4);
  ArmCallback(32, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(4, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Event_Wait(40);
  Emote(24, 0x101, 30);
  ArmCallback(24, FACING_WEST, 0);
  Event_Wait(20);
  ArmCallback(32, FACING_NORTHWEST + FACING_STEP, 0);
  ArmCallback(4, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4__s0ac4(25, 0x102, 30);
  Motion_SetModeAndWaitAnimation(25, 4);
  Event_Wait(10);
  ArmCallback(25, FACING_SOUTHEAST, 0);
  Event_ShowMessageThenWait(25, 0, 10);
  Emote(32, 0x101, 0);
  Func_080d47b4__s0ac4(4, 0x101, 30);
  Event_ShowMessageAndWait(32, 0);
  ArmCallbackAndRefresh(24, FACING_SOUTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_Wait(10);
  Event_ShowMessageThenWait(24, 0, 10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(25, 0, 10);
  Speed(24, 0x4ccc, 0x2666);
  Speed(25, 0x4ccc, 0x2666);
  OffsetPos(24, 3, -8);
  Actor_OffsetPositionAndReset(25, -3, -8);
  Actor_WaitForMoveById(24);
  Actor_WaitForMoveById(25);
  ArmCallback(25, FACING_SOUTHEAST + FACING_STEP, 0);
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
  ArmCallbackAndRefresh(4, FACING_NORTH);
  Func_02002b30();
}

/// A map edit and where it applies (Map_PlayMapEdit forwards
/// to Map_PlayMapEdit).
struct CellPictureCopyEntry {
  const s16 *steps;
  u16 unk04;
  u16 unk06;
};

/* VIEW(s0ac4): variable gData_02003308 as extern const struct
 * CellPictureCopyEntry gData_02003308[]; madra has extern struct MadraPlacement
 * gData_02003308[]; tried as the definition's extern s32 gData_02003308[], it
 * does not compile in tla-ja: games/tla/src/scene/madra.c:1293: request for
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
  s32 x;
  s32 z;

  /* FAKEMATCH(temporary): x and z held in locals set first, so the place call
   * builds x before the actor id; as constants passed to the call, 4 of 1565
   * instructions differ in tla-en (with z alone a local, 2). */
  x = 153 << 16;
  z = 0x01030000;
  Func_02002b28();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  if (GameFlag_Test(0x825) || GameFlag_Test(0x826))
    GameFlag_SetBit(0xa76);
  Event_SetMessageId__s0ac4(MsgWait3__s0ac4);
  Event_ShowMessageThenWait(34, 0, 0);
  SetPos(4, 150, 280);
  Actor_OffsetPositionAndReset(4, 16, 0);
  LaunchFromFocused(5, 8, 16, FACING_NORTH);
  LaunchFromFocused(32, 0, 0, FACING_NORTH);
  LaunchFromFocused(6, -8, 16, FACING_NORTH);
  LaunchFromFocused(7, -16, 0, FACING_NORTH);
  Actor_WaitForMoveById(4);
  ArmCallback(4, FACING_NORTH, 0);
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
    ArmCallback(32, FACING_NORTH, 0);
    ArmCallback(5, FACING_NORTH, 0);
    ArmCallback(7, FACING_NORTH, 0);
    ArmCallback(6, FACING_NORTH, 0);
    Actor_ArmCallback(34, FACING_SOUTH, 0);
    Event_Wait(20);
    SET_MODE(34, 4);
    Event_Wait(33);
    Event_Wait(20);
    Event_ShowMessageAndWait(34, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    ArmCallback(32, FACING_NORTH, 0);
    ArmCallback(5, FACING_NORTH, 0);
    ArmCallback(7, FACING_NORTH, 0);
    ArmCallback(6, FACING_NORTH, 0);
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
  Func_080d47b4__s0ac4(6, 0x102, 30);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(5, 6, 0);
  Event_Wait(20);
  SET_MODE(5, 4);
  Event_Wait(33);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4__s0ac4(34, 0x100, 30);
  Event_Wait(20);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_CommitPositionAndActivate(34, 16, 8);
  ArmCallback(34, FACING_SOUTH, 0);
  if (Func_02002b18(210) >= 0) {
    Func_02002c50(210, 3);
    PartyInventory_GiveItem(210, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Actor_ArmCallback(4, FACING_NORTH, 0);
    GameFlag_SetBit(0x918);
  } else {
    s32 count = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) + 1;

    Event_SetMessageId__s0ac4(MsgTheMayorTried__s0ac4);
    Func_02002b10(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    Motion_SetVarCbAndRefresh(34, 2);
    Event_ShowMessageAndWait(34, 0);
    Actor_CommitPositionAndActivate(34, 16, 0);
    Func_02002b10(210, 2);
    FieldMessage_Show((*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++, 5);
    CommitPos(34, -16, 0);
    ArmCallbackAndRefresh(34, FACING_SOUTH);
    SetHorizontalPos(66, PIXELS(192), PIXELS(256));
    *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) = count;
  }
  CommitPos(34, -16, -8);
  ArmCallbackAndRefresh(34, FACING_SOUTH);
  Func_080d47b4__s0ac4(34, 0x105, 30);
  Event_ShowMessageThenWait(34, 0, 0);
  SET_MODE(34, 3);
  Event_Wait(20);
  Event_Wait(20);
  ArmCallback(5, FACING_NORTH, 0);
  ArmCallback(6, FACING_NORTH, 0);
  Event_ShowMessageThenWait(34, 0, 0);
  Actor_SetModeById(32, 3);
  SET_MODE(4, 3);
  SET_MODE(5, 3);
  SET_MODE(6, 3);
  SET_MODE(7, 3);
  Event_Wait(20);
  Event_Wait(20);
  ArmCallbackAndRefresh(34, FACING_NORTH);
  SetPos(34, 153, 259);
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
  Func_080d47b4__s0ac4(32, 0x101, 30);
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
  ArmCallback(33, FACING_WEST, 0);
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
  ArmCallbackAndRefresh(33, FACING_WEST);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  Event_ShowMessageAndWait(33, 0);
  Emote(6, 0x102, 30);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Func_080d47b4__s0ac4(5, 0x100, 30);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4__s0ac4(33, 0x107, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_SetAngleToward(33, 4, 0);
  Event_Wait(20);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
    Event_ShowMessageAndWait(33, 0);
    Func_080d47b4__s0ac4(4, 0x102, 30);
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
    Func_080d47b4__s0ac4(33, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  } else {
    Event_SetMessageId__s0ac4(MsgICanTBelieveIt__s0ac4);
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
    Func_080d47b4__s0ac4(6, 0x109, 30);
    Event_ShowMessageAndWait(33, 0);
  }
  Event_SetMessageId__s0ac4(MsgWhoIsThis__s0ac4);
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
  Func_080d47b4__s0ac4(32, 0x109, 30);
  Event_Wait(60);
  Func_080d47b4__s0ac4(33, 0x106, 30);
  Event_ShowMessageThenWait(33, 0, 0);
  Emote(4, 0x102, 0);
  Emote(5, 0x102, 0);
  Emote(6, 0x102, 0);
  Func_080d47b4__s0ac4(32, 0x102, 30);
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
  ArmCallback(33, FACING_WEST, 0);
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
  Func_080d47b4__s0ac4(4, 0x100, 30);
  Func_080d47b4__s0ac4(33, 0x101, 30);
  Event_ShowMessageAndWait(33, 0);
  Actor_ArmCallbackAndRefresh(33, FACING_EAST);
  Event_Wait(20);
  ArmCallbackAndRefresh(33, FACING_WEST);
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
    Func_080d47b4__s0ac4(33, 0x108, 30);
    Event_ShowMessageThenWait(33, 0, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Func_080d47b4__s0ac4(33, 0x103, 30);
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
  CamBounds(-1, -1, -1, 0);
  Actor_CommitPositionAndActivate(33, 24, 24);
  Actor_CommitPositionAndActivate(33, 132, 0);
  Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  Func_02002c60();
  Camera_FollowActor(4, 1);
  Func_080d47b4__s0ac4(6, 0x102, 30);
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
  ArmCallback(32, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(32, 0);
  Func_080d47b4__s0ac4(7, 0x101, 20);
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
  ArmCallback(5, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(5, 0);
  Actor_TurnToFaceEachOther(4, 7, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(32, 2);
  Event_Wait(20);
  ArmCallbackAndRefresh(32, FACING_SOUTH);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetPartAttribute(Actor_GetById(5), 0);
  Event_Wait(20);
  Func_080d47b4__s0ac4(5, 0x100, 30);
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
  Func_080d47b4__s0ac4(32, 0x108, 30);
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
  Speed(32, 0x00013333, 0x9999);
  SET_MODE(32, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(32, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  Speed(5, 0x00013333, 0x9999);
  SET_MODE(5, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(5, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Speed(6, 0x00013333, 0x9999);
  SET_MODE(6, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(6, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Speed(7, 0x00013333, 0x9999);
  SET_MODE(7, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(7, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_SetBit(0x980);
  GameFlag_SetBit(0x98e);
  Func_02002b30();
}
#undef SET_MODE

// ---- games/tla/src/scene/madra/events_d.c ----
#include "text/msg_ids.h"

void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

/* VIEW(events_d): variable gData_02002d38 as extern s16 gData_02002d38[]; s051c
 * has extern const u8 gData_02002d38[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja: games/tla/src/scene/madra.c:1888:
 * conflicting types for `gData_02002d38'. */
extern s16 gData_02002d38__events_d[] __asm__("gData_02002d38");
/* VIEW(events_d): variable gData_02002ff0 as extern struct ActorPlacement
 * gData_02002ff0[]; s051c has extern const u8 gData_02002ff0[]; tried as the
 * definition's extern u32 gData_02002ff0[], it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:1768: conflicting types for `gData_02002ff0'. */
extern struct ActorPlacement
    gData_02002ff0__events_d[] __asm__("gData_02002ff0");
/* VIEW(events_d): variable gData_02003358 as extern struct Trigger
 * gData_02003358[]; s051c has extern const u8 gData_02003358[]; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:1987: conflicting types for `gData_02003358'. */
extern struct Trigger gData_02003358__events_d[] __asm__("gData_02003358");
/* VIEW(events_d): variable gData_0200367c as extern struct Trigger
 * gData_0200367c[]; s051c has extern const u8 gData_0200367c[]; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:2057: conflicting types for `gData_0200367c'. */
extern struct Trigger gData_0200367c__events_d[] __asm__("gData_0200367c");
/* VIEW(events_d): variable gData_0200394c as extern struct Trigger
 * gData_0200394c[]; s051c has extern const u8 gData_0200394c[]; tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/madra.c:2120: conflicting types for `gData_0200394c'. */
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
  Func_02002b30();
}

void Func_02002a58(void) {
  s32 id;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  CommitPos(4, 0, -14);
  id = 4;
  Actor_CommitPositionAndActivate(id, 16, 0);
  Func_020029c8();
}

void Func_02002a80(void) {
  s32 id, x, z;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  CommitPos(4, -2, -6);
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

void Func_02002ab0(void) {
  s32 id, a;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * angle byte and its shift; as no asm, 2 of 11 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_ArmCallback(id, a << 8, 0);
  Func_020029c8();
}

void Func_0200006c(void);
void Func_020002bc(void);
void Func_02000320(void);
void Func_0200036c(void);
void Func_02000404(void);
void Func_02000490(void);
void Func_0200051c(void);
void Func_020005b8(void);
void Func_02000894(void);
void Func_02000968(void);
void Func_02000a48(void);
void Func_02000ac4(void);
void Func_020014fc(void);

extern s16 gData_020032f0[];
extern s16 gData_020032fc[];

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
    {1, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 15, CONDITION_ALWAYS, 15},
    {2, 16, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {2, 19, 0x910, EVENT(Func_020029ac)},
    {2, 20, 0x910, EVENT(Func_02000ac4)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgTheTownElder)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgYouPickedThe)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgThatBigWave)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgLetMeGuess)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgMaybeWeLl)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgIfICould)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_02000894)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgItSLike)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgTheChampaWere)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgThoseChampaCurs)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgIDoubtThe)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgIGotSomeone)},
    {0, 20, CONDITION_ALWAYS, MSGNO(MsgIDonTCare)},
    {0, 21, CONDITION_ALWAYS, MSGNO(MsgWeTookThat)},
    {0, 22, CONDITION_ALWAYS, MSGNO(MsgMyFriendWas)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgMyPartnerWas)},
    {0, 24, CONDITION_ALWAYS, MSGNO(MsgThereSNoWay)},
    {0, 25, CONDITION_ALWAYS, MSGNO(MsgAllOfIndra)},
    {0, 26, CONDITION_ALWAYS, EVENT(Func_02000404)},
    {0, 27, CONDITION_ALWAYS, EVENT(Func_0200083c)},
    {0, 28, CONDITION_ALWAYS, MSGNO(MsgEvenIfWe)},
    {0, 29, CONDITION_ALWAYS, EVENT(Func_02000108)},
    {0, 30, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {0, 31, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {0, 35, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgEveryoneThinksBriggs)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgTheTownElderIs)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgIfABig)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgItSABit)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgThatChampaIn)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgIVeAlways)},
    {0x8d15, 1038, 0x300, EVENT(Func_02000968)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgWhoWasIt)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgNowMadraIs)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgWhenISaw)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgTheChampaMust)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgItSNot)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgIMNotSure)},
    {0x8d15, 20, CONDITION_ALWAYS, MSGNO(MsgILlNever)},
    {0x8d15, 21, CONDITION_ALWAYS, MSGNO(MsgILookedInto)},
    {0x8d15, 22, CONDITION_ALWAYS, MSGNO(MsgIWonderHowMany)},
    {0x8d15, 23, CONDITION_ALWAYS, MSGNO(MsgIMissMy)},
    {0x8d15, 24, CONDITION_ALWAYS, MSGNO(MsgItSSoWeird)},
    {0x8d15, 25, CONDITION_ALWAYS, MSGNO(MsgTheseAreRough)},
    {0x8d15, 26, CONDITION_ALWAYS, MSGNO(MsgTheChampaDug)},
    {0x8d15, 27, CONDITION_ALWAYS, MSGNO(MsgItWasAll)},
    {0x8d15, 28, CONDITION_ALWAYS, MSGNO(MsgMySoreBottom)},
    {0x8d15, 29, CONDITION_ALWAYS, MSGNO(MsgTheItemVendor)},
    {0x8d15, 30, CONDITION_ALWAYS, MSGNO(MsgThoseLousyChampa)},
    {0x8d15, 31, CONDITION_ALWAYS, MSGNO(MsgBriggsAndHis)},
    {0x8d15, 35, CONDITION_ALWAYS, MSGNO(MsgBriggsAndHis)},
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
    {1, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 15, CONDITION_ALWAYS, 15},
    {2, 16, CONDITION_ALWAYS, EVENT(Func_020005b8)},
    {2, 19, 0x914, EVENT(Func_020029b8)},
    {2, 20, 0x914, EVENT(Func_020014fc)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgWeThinkThe)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgSoTheKibombo)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgTheKibomboCame)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgIHeardThatThe)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgIGuessThat)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgGondowanSoundsLike)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_02000894)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgICanTBelieveSomeone)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgNowThatWe)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgThereAreStill)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgBriggsDidnT)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgTheseCavesWere)},
    {0, 20, CONDITION_ALWAYS, MSGNO(MsgThatGuyWasn)},
    {0, 21, CONDITION_ALWAYS, MSGNO(MsgOhWeReleased)},
    {0, 22, CONDITION_ALWAYS, MSGNO(MsgEveryoneYelledAt)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgYouShouldaSeen)},
    {0, 24, CONDITION_ALWAYS, MSGNO(MsgFirstPiratesRaid)},
    {0, 25, CONDITION_ALWAYS, MSGNO(MsgMyGuessIs)},
    {0, 26, CONDITION_ALWAYS, EVENT(Func_02000490)},
    {0, 29, CONDITION_ALWAYS, EVENT(Func_02000108)},
    {0, 30, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {0, 31, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgIfItWerenT)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgIfGondowanIs)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgItSStrangeIt)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgTheMayorIs)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgHeToldUs)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgInGondowanThey)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgIHeardThe)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgIfTheyWere)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgItWasSo)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgIWonderWhoDug)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgWaitIfBriggs)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgIVeNeverSeenAny)},
    {0x8d15, 20, CONDITION_ALWAYS, MSGNO(MsgDidnTSeem)},
    {0x8d15, 21, CONDITION_ALWAYS, MSGNO(MsgWasSoHappy)},
    {0x8d15, 22, CONDITION_ALWAYS, MSGNO(MsgIDidnT)},
    {0x8d15, 23, CONDITION_ALWAYS, MSGNO(MsgICanTBelieveNobody)},
    {0x8d15, 24, CONDITION_ALWAYS, MSGNO(MsgTheWayThings)},
    {0x8d15, 25, CONDITION_ALWAYS, MSGNO(MsgIVeBeenStanding)},
    {0x8d15, 26, CONDITION_ALWAYS, MSGNO(MsgWeSealedUp)},
    {0x8d15, 29, CONDITION_ALWAYS, MSGNO(MsgTheChampaWeapons)},
    {0x8d15, 30, CONDITION_ALWAYS, MSGNO(MsgTheGroupThat)},
    {0x8d15, 31, CONDITION_ALWAYS, MSGNO(MsgThatShinIs)},
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
    {1, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 15, CONDITION_ALWAYS, 15},
    {2, 21, 0x98e, EVENT(Func_0200036c)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgIDonTThink)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgWeGoTo)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgIHeardTheBoat)},
    {0, 11, 0x301, EVENT(Func_02000a48)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgThoseTravelersLooked)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgIWonderIfThe)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhatWill)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgAGroupOfSharp)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgIfYouHaveA)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgIWasAt)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgItSNotThat)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgTheMayorAsked)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgItSeemsNo)},
    {0, 20, CONDITION_ALWAYS, MSGNO(MsgSayThereWas)},
    {0, 21, CONDITION_ALWAYS, MSGNO(MsgIThoughtShin)},
    {0, 22, CONDITION_ALWAYS, MSGNO(MsgWhenAndHis)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgUhhhnnnUhhhnnn2)},
    {0, 24, CONDITION_ALWAYS, MSGNO(MsgTheMayorFinally)},
    {0, 25, CONDITION_ALWAYS, MSGNO(MsgHeyItS)},
    {0, 26, CONDITION_ALWAYS, MSGNO(MsgYeahWellWe)},
    {0, 29, CONDITION_ALWAYS, EVENT(Func_02000108)},
    {0, 30, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {0, 31, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {0, 33, CONDITION_ALWAYS, EVENT(Func_020002bc)},
    {0, 35, CONDITION_ALWAYS, EVENT(Func_020003b8)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgWhyWouldAnyoneBe)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgTheyVeBeenHaving)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgEveryoneGetsThe)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgIWonderWhy)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgWhatWasThat)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgThePeopleOf)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgThatIslandThat)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgHowBigWould)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgTheseNewBoats)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgIfABuilding)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgSailorsAreSupposed)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgOurMayorS)},
    {0x8d15, 20, CONDITION_ALWAYS, MSGNO(MsgIsTurningWater)},
    {0x8d15, 21, CONDITION_ALWAYS, MSGNO(MsgMadraIsAlways)},
    {0x8d15, 22, CONDITION_ALWAYS, MSGNO(MsgSoThisFellow)},
    {0x8d15, 23, CONDITION_ALWAYS, MSGNO(MsgIKnewI)},
    {0x8d15, 24, CONDITION_ALWAYS, MSGNO(MsgHeWasGone)},
    {0x8d15, 25, CONDITION_ALWAYS, MSGNO(MsgHowMuchLongerDo)},
    {0x8d15, 29, CONDITION_ALWAYS, MSGNO(MsgThePiratesAnd)},
    {0x8d15, 30, CONDITION_ALWAYS, MSGNO(MsgThatNightThe)},
    {0x8d15, 31, CONDITION_ALWAYS, MSGNO(MsgShinSeemsTerrified)},
    {0x8d15, 33, CONDITION_ALWAYS, EVENT(Func_02000320)},
    {0x8d15, 35, CONDITION_ALWAYS, MSGNO(MsgWeReStill)},
    {TRIGGERS_END},
};
