#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"

extern char MsgGazedIntoThe[];
extern char MsgThereAreSome[];
extern char MsgTheBoatS[];

u32 Random16Far(void);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
u8 *Unit_Get(s32 owner);
void FieldMessage_Show(s32 message, s32 mode);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
struct Actor *Func_02004b30(s32 sprite, s32 x, s32 y, s32 z);
void Func_02004b78(s32 a, s32 b, s32 c);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004d70(s32 value);
void Func_02004db8(s32 value);
s32 Func_02004e10(void);
void Func_02004e18(s32 id, s32 amount);

void Func_02000be0(void) {
  u32 roll = (Random16Far() << 4) >> 16;

  Func_02004c20();
  Func_02004db8(0);
  FieldMessage_Show((s32)MsgGazedIntoThe, 1);
  if (roll == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    s32 slot = 79;
    struct Actor *leader = Actor_GetById(Func_02004e10());
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
      object = (struct Actor *)Value4((s32 (*)())Func_02004b30, 0x1e9, x,
                                      position[1], position[2]);
      if (object == NULL)
        return;
      work->objects[slot] = object;
    } else {
      object->x = x;
      object->y = position[1];
      object->z = position[2];
    }
    Func_02004e18(79, amount);
    /* FAKEMATCH(one-pass): the store and the call in a once-only block; as
     * plain statements, 6 of 129 instructions differ in tla-en. */
    do {
      *(u16 *)((u8 *)work + SPLIT_CONSTANT(0xd34)) = amount;
      Func_02004d70(0x1000 | slot);
    } while (0);
  } else if (roll == 1) {
    Func_02004b78(0x30000, 0x50000, 0x10000);
    Call3((void (*)())Func_02004b78, (s32)(-1), (s32)(-1), (s32)(SPLIT_CONSTANT(0xe666)));
    FieldEvent_Wait(30);
    FieldMessage_Show((s32)MsgTheBoatS, 1);
  } else {
    FieldMessage_Show((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Func_02004c28();
}
