#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"

extern struct Actor *Data_02004cec;
extern char MsgMarsStar02;
extern char MsgMarsStar06;
extern char MsgMarsStar08;

void Func_020042dc(void);
void Func_020042e4(void);
void Func_0200444c(s32 value);
void Actor_SetModeById(u32 id, s32 mode);
void Func_0200437c(s32 message);
void Func_02004394(s32 id, s32 value);
void Func_020043a4(s32 id, s32 angle);
void Func_02004204(struct Actor *object, s32 x, s32 y, s32 z);
void Func_0200420c(struct Actor *object);
void PartyInventory_Remove(s32 item);
s32 Inventory_AddItem(s32 owner, s32 item);
void FieldMessage_Show(s32 message, s32 position);
u8 *Unit_Get(s32 owner);
void Func_020042c4(s32 owner, s32 slot);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020041ec(struct Actor *object);
void Func_020043c4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020043d4(void);
void Func_0200440c(s32 a, s32 b);
void Func_02004404(s32 a, s32 b);
void Func_02004414(s32 frames);
void Func_020042a4(s32 message, s32 a, s32 b);

static __inline__ void Func_0200440c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 10 of 155
   * instructions differ in tla-en. */
  Func_0200440c(a, b);
}

void Func_02000bd8(void) {
  struct Actor *leader;
  struct Actor *object;
  s32 i;
  s32 offset;

  Func_020042dc();
  Func_0200444c(0);
  Actor_SetModeById(13, 5);
  Func_0200437c((s32)&MsgMarsStar02);
  Func_02004394(13, 0);
  Actor_SetModeById(13, 10);
  if (!GameFlag_Test(0xa3a)) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    Func_020043a4(gMainRoutineWork.currentOwner, 0x4000);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    object = Data_02004cec;
    object->motionFlags = 0;
    object->speedLimit = 0x40000;
    object->acceleration = 0x20000;
    Func_02004204(object, leader->x, leader->y + 0x200000, leader->z);
    Func_0200420c(Data_02004cec);
    Func_0200446c(83);
    PartyInventory_Remove(224);
    Inventory_AddItem(4, 222);
    FieldMessage_Show((s32)&MsgMarsStar08, 1);
    i = 0;
    offset = 216;
    for (; i <= 14; offset += 2, i++) {
      if ((*(u16 *)(Unit_Get(4) + offset) & 0x1ff) == 222) {
        Func_020042c4(4, i);
        break;
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    GameFlag_SetBit(0xa3a);
    Actor_SetPositionAndResetMotion(Data_02004cec, 0, 0, 0);
    wait(1);
    Func_020041ec(Data_02004cec);
    wait(1);
    Func_020043c4(0x4cccc, 0x9999);
    Camera_MoveWithinMapBounds(170 << 18, -1, 146 << 16, 1);
    Func_020043d4();
    Func_0200440c_(0x10000, 0);
    Func_02004404(0x10003, 0);
    Func_02004414(20);
    wait(20);
    {
      // A halfword in its own local, so the address takes the register first.
      struct {
        u16 value;
      } white;

      white.value = 0x7fff;
      *(u16 *)0x0500021e = white.value;
    }
    wait(1);
    Func_020042a4((s32)&MsgMarsStar06, 0, 0);
    Func_02004404(0x10000, 0);
    Func_02004414(20);
    wait(20);
  }
  Func_020042e4();
}
