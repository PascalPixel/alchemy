#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"

extern struct Actor *Data_02004cec;
extern char MsgMarsStar02;
extern char MsgMarsStar06;
extern char MsgMarsStar08;

void Func_020042dc(void);
void Func_020042e4(void);
void PartyInventory_Remove(s32 item);
s32 Inventory_AddItem(s32 owner, s32 item);
u8 *Unit_Get(s32 owner);
void Func_020042c4(s32 owner, s32 slot);
void Func_020041ec(struct Actor *object);
void Func_020043d4(void);

static __inline__ void Func_0200440c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 10 of 155
   * instructions differ in tla-en. */
  BattleEffect_ApplyColorToSourceBuffer(a, b);
}

void Func_02000bd8(void) {
  struct Actor *leader;
  struct Actor *object;
  s32 i;
  s32 offset;

  Func_020042dc();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(13, 5);
  Event_SetMessageId((s32)&MsgMarsStar02);
  Event_ShowMessageAndWait(13, 0);
  Actor_SetModeById(13, 10);
  if (!GameFlag_Test(0xa3a)) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    object = Data_02004cec;
    object->motionFlags = 0;
    object->speedLimit = 0x40000;
    object->acceleration = 0x20000;
    FieldActor_SetMoveTarget(object, leader->x, leader->y + 0x200000, leader->z);
    Actor_WaitForMove(Data_02004cec);
    Event_PlaySound(83);
    PartyInventory_Remove(224);
    Inventory_AddItem(4, 222);
    Message_ShowCentered((s32)&MsgMarsStar08, 1);
    i = 0;
    offset = 216;
    for (; i <= 14; offset += 2, i++) {
      if ((*(u16 *)(Unit_Get(4) + offset) & 0x1ff) == 222) {
        Func_020042c4(4, i);
        break;
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Event_SetFlag(0xa3a);
    FieldActor_SetPositionAndResetMotion(Data_02004cec, 0, 0, 0);
    System_Wait(1);
    Func_020041ec(Data_02004cec);
    System_Wait(1);
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(170 << 18, -1, 146 << 16, 1);
    Func_020043d4();
    Func_0200440c_(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 0);
    PaletteFade_Interpolate(20);
    System_Wait(20);
    {
      // A halfword in its own local, so the address takes the register first.
      struct {
        u16 value;
      } white;

      white.value = 0x7fff;
      *(u16 *)0x0500021e = white.value;
    }
    System_Wait(1);
    FieldMessage_ShowCentered((s32)&MsgMarsStar06, 0, 0);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(20);
    System_Wait(20);
  }
  Func_020042e4();
}
