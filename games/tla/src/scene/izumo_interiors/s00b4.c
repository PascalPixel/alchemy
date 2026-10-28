#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02002b40[];
extern const u8 gData_02002cf0[];
extern const u8 gData_02002d80[];
extern const u8 gData_02002f78[];
extern const u8 gData_02003164[];
extern const u8 gData_0200323c[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Func_020021c4(void);
void Func_020021cc(void);
void Func_020022bc(s32 skip);
void Func_02002244(s32 message);
void Func_02002254(s32 speaker, s32 flags);
void Func_0200226c(s32 actor, s32 animation, s32 frames);
s32 Func_020022dc(void);
void Func_02002304(s32 slot, s32 actor);
void Func_0200230c(s32 actor);
void Func_02002314(s32 slot, s32 actor);
void Func_0200061c(s32 mode);
void Actor_StartRepeatedMotion(s32 actor, s32 variant);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_OffsetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
s32 PartyInventory_FindOwner(s32 item);
void FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);

static inline s32 FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void ActorTurn(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

static inline void ActorAnimate(s32 actor, s32 animation, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200226c(actor, animation, frames);
}

static inline void LeaderAnimate(s32 animation, s32 frames) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; called as
   * value-returning, the animation and frames are set before the owner is
   * loaded (differ 4 as a void call). */
  ((s32 (*)(s32, s32, s32))Func_0200226c)(gAdventure.currentOwner, animation,
                                          frames);
}

const u8 *Func_020000b4(void) {
  if (FlagCheck(0x927))
    return gData_02002d80;
  if (gAdventure.entrance == 7)
    return gData_02002cf0;
  return gData_02002b40;
}

const u8 *Func_020000f0(void) {
  if (FlagCheck(0x927))
    return gData_0200323c;
  if (gAdventure.entrance == 7)
    return gData_02003164;
  return gData_02002f78;
}

void Func_0200012c(s32 actor) {
  u16 facing = Actor_GetById(gAdventure.currentOwner)->angle;

  /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing
   * load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2).
   */
  do {
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
      Func_0200230c(actor);
    } else {
      Func_020021c4();
      Func_020022bc(0);
      if (FlagCheck(0x927)) {
        Func_02002244((s32)MsgNowThatPeace);
        Func_02002254(12, 0);
      } else {
        Func_02002244((s32)MsgWhyWouldOur);
        Func_02002254(22, 0);
      }
      Func_020021cc();
    }
  } while (0);
}

void Func_020001a8(s32 actor) {
  u16 facing = Actor_GetById(gAdventure.currentOwner)->angle;

  /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing
   * load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2).
   */
  do {
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
      Func_02002314(9, actor);
    } else {
      Func_020021c4();
      Func_020022bc(0);
      if (FlagCheck(0x927))
        Func_02002244((s32)MsgBriggsFromChampa);
      else
        Func_02002244((s32)MsgOfCourseI);
      Func_02002254(actor, 0);
      Func_020021cc();
    }
  } while (0);
}

void Func_02000220(s32 actor) {
  u16 facing = Actor_GetById(gAdventure.currentOwner)->angle;

  /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing
   * load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2).
   */
  do {
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
      Func_02002304(24, actor);
    } else {
      Func_020021c4();
      Func_020022bc(0);
      if (FlagCheck(0x927))
        Func_02002244((s32)MsgYouMustSee);
      else
        Func_02002244((s32)MsgIzumoSLeaders);
      Func_02002254(actor, 0);
      Func_020021cc();
    }
  } while (0);
}

void Func_02000298(void) {
  Func_020021c4();
  Func_020022bc(0);
  if (!FlagCheck(0x922)) {
    Func_02002244((s32)MsgSusaSNot);
    Func_02002254(8, 0);
    FieldEvent_Wait(30);
    ActorAnimate(8, 265, 0);
    FieldEvent_Wait(60);
    Func_02002254(8, 0);
    FieldEvent_Wait(30);
    ActorAnimate(8, 261, 0);
    FieldEvent_Wait(60);
    Func_02002254(8, 0);
    FieldEvent_Wait(30);
    Actor_StartRepeatedMotion(8, 1);
    FieldEvent_Wait(60);
    Func_02002254(8, 0);
    FieldEvent_Wait(30);
    FlagSet(0x922);
  }
  Actor_SetModeById(8, 4);
  FieldEvent_Wait(60);
  Func_02002244((s32)MsgIfTheSerpent);
  Func_02002254(8, 0);
  Func_020021cc();
}

void Func_02000358(void) {
  FlagSet(0x923);
  Func_02002244((s32)MsgIPrayThat);
  Func_02002254(8, 0);
}

void Func_02000378(void) {
  void *work = Ram_HeapSlots->eventWork;

  Func_020021c4();
  Func_020022bc(0);
  Actor_SetAngleToward(15, gAdventure.currentOwner, 0);
  if (FlagCheck(0x92c)) {
    Func_02002244((s32)MsgWithThisDancing);
    Func_02002254(15, 0);
  } else {
    Func_02002244((s32)MsgIfYouHave);
    Func_02002254(15, 0);
    if (PartyInventory_FindOwner(451) != -1)
      FIELD_AT(work, u16, 346) = 1;
  }
  ActorTurn(15, 0x4000, 0);
  Func_020021cc();
}

void Func_02000400(void) {
  s32 msg;

  Func_020021c4();
  Func_020022bc(0);
  msg = (s32)MsgTheyVeBeen;
  Func_02002244(msg);
  FieldMessage_ShowAtActor(16, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020022dc(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02002244(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02002244(msg + 2);
  }
  Func_02002254(16, 0);
  Func_020021cc();
}

void Func_02000458(void) {
  Func_020021c4();
  Func_020022bc(0);
  Actor_SetAngleToward(15, gAdventure.currentOwner, 0);
  if (FlagCheck(0x92b)) {
    Func_02002244((s32)MsgTryingAgainWhat);
    Func_02002254(15, 0);
  } else {
    Func_02002244((s32)MsgAndWhatWere);
    Func_02002254(15, 0);
    if (FlagCheck(0x923)) {
      LeaderAnimate(256, 30);
      Func_0200226c(15, 259, 30);
      Func_02002254(15, 0);
      LeaderAnimate(258, 30);
      Actor_SetModeById(15, 4);
      FieldEvent_Wait(30);
      Func_02002254(15, 0);
    }
    FlagSet(0x92b);
  }
  ActorTurn(15, 0x4000, 0);
  Func_020021cc();
}
