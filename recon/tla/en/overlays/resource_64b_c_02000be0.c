// Draft, differ 53 of 129: gazing over the side of the boat: one roll in
// sixteen finds coins, the next shakes the camera, the rest show one of the
// MsgTheBoatS messages. Left: the native keeps the found object in r6 and
// reads the offset x once for both paths, and builds the shake's -1s apart.
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "event_work.h"

extern char MsgGazedIntoThe[];
extern char MsgThereAreSome[];
extern char MsgTheBoatS[];

u32 Random16Far(void);
void FieldEvent_WaitFrames(s32 frames);
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
    u8 *unit = Unit_Get(gAdventure.currentOwner);
    s32 amount = (((unit[15] * Random16Far()) << 1) >> 16) + 0x8001;
    s32 position[3];
    s32 x;
    struct Actor *object;

    FieldMessage_Show((s32)MsgThereAreSome, 1);
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffsetFar(0x100000, leader->angle, position);
    object = Actor_GetById(79);
    x = position[0];
    if (object == NULL) {
      object = Func_02004b30(0x1e9, x, position[1], position[2]);
      if (object == NULL)
        return;
      work->objects[slot] = object;
    } else {
      object->x = x;
      object->y = position[1];
      object->z = position[2];
    }
    Func_02004e18(79, amount);
    *(u16 *)((u8 *)work + 0xd34) = amount;
    Func_02004d70(0x1000 | slot);
  } else if (roll == 1) {
    Func_02004b78(0x30000, 0x50000, 0x10000);
    Func_02004b78(-1, -1, 0xe666);
    FieldEvent_WaitFrames(30);
    FieldMessage_Show((s32)MsgTheBoatS, 1);
  } else {
    FieldMessage_Show((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Func_02004c28();
}
