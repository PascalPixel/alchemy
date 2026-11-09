#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

extern char MsgGazedIntoThe[];
extern char MsgThereAreSome[];
extern char MsgTheBoatS[];

u32 Random16Far(void);
u8 *Unit_Get(s32 owner);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004d70(s32 value);

void Func_02000be0(void) {
  u32 roll = (Random16Far() << 4) >> 16;

  Func_02004c20();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGazedIntoThe, 1);
  if (roll == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    s32 slot = 79;
    struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
    u8 *unit = Unit_Get(gMainRoutineWork.currentOwner);
    s32 amount = (((unit[15] * Random16Far()) << 1) >> 16) + 0x8001;
    s32 position[3];
    s32 x;
    /* FAKEMATCH(pin): object in r6, which leaves r7 to the amount; as a plain
     * local, 24 of 129 instructions differ in tla-en. */
    register struct Actor *object asm("r6");

    FieldMessage_Show((s32)MsgThereAreSome, 1);
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffsetFar(0x100000, leader->angle, position);
    object = Actor_GetById(79);
    x = position[0];
    if (object == NULL) {
      object = FieldActor_Create(0x1e9, x, position[1], position[2]);
      if (object == NULL)
        return;
      work->objects[slot] = object;
    } else {
      object->x = x;
      object->y = position[1];
      object->z = position[2];
    }
    Func_080d3b28(79, amount);
    /* FAKEMATCH(one-pass): the store and the call in a once-only block; as
     * plain statements, 6 of 129 instructions differ in tla-en. */
    do {
      *(u16 *)((u8 *)work + 0xd34) = amount;
      Func_02004d70(0x1000 | slot);
    } while (0);
  } else if (roll == 1) {
    Map_SetScreenShake(0x30000, 0x50000, 0x10000);
    MapGrid_SetShake((s32)(-1), (s32)(-1), (s32)(0xe666));
    Event_Wait(30);
    FieldMessage_Show((s32)MsgTheBoatS, 1);
  } else {
    FieldMessage_Show((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Func_02004c28();
}
