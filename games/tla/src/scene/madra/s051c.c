/// @file
/// Scene code of overlay 0x658, in ROM order, from s051c's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
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

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];
extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe;
extern u8 MsgTheArmorerAnd;
extern u8 MsgAfterTheChampa;
extern u8 MsgPeopleAreSaying;
extern u8 MsgIKnewThere;
extern u8 MsgOwOwOw;
extern u8 MsgMyBoyfriendShin;
extern u8 MsgAllMyInjuries;
extern u8 MsgMySisterWas;
extern u8 MsgIfYouDon;
extern u8 MsgHaveYouBy;
extern u8 MsgUsingPsynergyOn;
extern u8 MsgWhereCouldMenardi;
extern u8 MsgEveryoneHereIn;
extern u8 MsgYeahWellWe;
extern u8 MsgYouCominThrough;
extern u8 MsgYeahUhWe;
extern u8 MsgOhUhYou;
extern u8 MsgWasIJust;
extern u8 MsgHeyMisterAre;
extern u8 MsgDidYouCome;
extern u8 MsgIsThereBy;

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

void Func_02000670(void);

void Func_0200068c(void) {
  MapGrid_CopyCellsLayered(51, 10, 3, 3, 45, 10);
  Func_02000670();
}

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_0200068c(void);
void Func_02000f80(void);
void Func_020019d8(void);

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
    ((struct FieldSprite *)Actor_GetById(26)->animation)->nextFrame |= 8;
    {
      struct FieldSprite *sprite = Actor_GetById(26)->animation;

      sprite->unk11 = (sprite->unk11 & 3) | 32;
    }
    ((struct FieldSprite *)Actor_GetById(26)->animation)->unk19 = 1;
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

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];
extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe;
extern u8 MsgTheArmorerAnd;
extern u8 MsgAfterTheChampa;
extern u8 MsgPeopleAreSaying;
extern u8 MsgIKnewThere;
extern u8 MsgOwOwOw;
extern u8 MsgMyBoyfriendShin;
extern u8 MsgAllMyInjuries;
extern u8 MsgMySisterWas;
extern u8 MsgIfYouDon;
extern u8 MsgHaveYouBy;
extern u8 MsgUsingPsynergyOn;
extern u8 MsgWhereCouldMenardi;
extern u8 MsgEveryoneHereIn;
extern u8 MsgYeahWellWe;
extern u8 MsgYouCominThrough;
extern u8 MsgYeahUhWe;
extern u8 MsgOhUhYou;
extern u8 MsgWasIJust;
extern u8 MsgHeyMisterAre;
extern u8 MsgDidYouCome;
extern u8 MsgIsThereBy;

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
