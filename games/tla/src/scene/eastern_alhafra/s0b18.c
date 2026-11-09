#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "event_inline.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) (Actor_Get((id)))

extern const u8 gData_02004d0c[];
void Func_02000af8(void);
void Func_02004c5c();
void Actor_SetSingleChildFlags();
void Func_02003274(void);
void Func_02000fa4(void);
void Func_020040ec();
void Func_02004c14(void);
void Func_02004c2c();
void Func_02004c1c(void);
void Func_02004c24();
void Func_020047fc();
void Func_02004c94();

s32 Func_02000b18(void) {
  struct Actor *object;
  s32 flag;

  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Func_02000af8();
  OBJECT(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(0x8a7) != 0) {
    if (Event_GetFlag(0x905) != 0 &&
        Event_GetFlag(0x90f) == 0) {
      Actor_SetPosition(30, PIXELS(184), PIXELS(616));
      Actor_SetPosition(31, PIXELS(136), PIXELS(600));
      Actor_SetPosition(28, PIXELS(168), PIXELS(584));
      Actor_FaceDirection(28, FACING_SOUTH, 0);
      Call3(Func_02004c5c, 30, 0, 13);
      Call3(Func_02004c5c, 31, 0x8000, 13);
      Call3(Func_02004c5c, 28, 0, 13);
      Actor_SetPosition(25, PIXELS(112), PIXELS(432));
      Actor_SetPosition(27, PIXELS(176), PIXELS(408));
      Actor_SetPosition(29, PIXELS(152), PIXELS(376));
      Actor_FaceDirection(29, FACING_SOUTH, 0);
      Actor_SetSpriteRotationById(25, 0x4000);
      Call2(Actor_SetSingleChildFlags, (s32)Actor_Get(25), 0);
      Actor_SetSpriteRotationById(27, 0xc000);
      Call2(Actor_SetSingleChildFlags, (s32)Actor_Get(27), 0);
      Actor_SetSpriteRotationById(29, 0x4000);
      Call2(Actor_SetSingleChildFlags, (s32)Actor_Get(29), 0);
      Actor_SetAnimation(25, 13);
      Actor_SetAnimation(27, 13);
      Actor_SetAnimation(29, 13);
    }
    if (Event_GetFlag(0x905) != 0) {
      MapGrid_CopyCellsLayered(40, 0, 24, 31, 11, 6);
      MapGrid_CopyCellsLayered(84, 15, 10, 10, 75, 15);
      MapGrid_CopyCellAttributes(10, 24, 1, 1, 11, 24);
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(8, 0, 0);
      Actor_SetPosition(10, 0, 0);
      Actor_SetPosition(11, 0, 0);
      Actor_SetPosition(15, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.secondary = 1;
    gMainRoutineWork.entrance = 1;
    Actor_SetPosition(4, PIXELS(136), PIXELS(552));
    Actor_SetPosition(26, PIXELS(120), PIXELS(552));
    Actor_SetPosition(30, PIXELS(184), PIXELS(616));
    Actor_SetPosition(31, PIXELS(136), PIXELS(600));
    Actor_SetPosition(28, PIXELS(160), PIXELS(584));
    Actor_SetSpriteRotationById(30, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(30), 0);
    Actor_SetSpriteRotationById(31, 0xc000);
    Actor_SetSingleChildFlags(Actor_Get(31), 0);
    Actor_SetSpriteRotationById(28, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(28), 0);
    FieldActor_SetSpritePriority(27, 2);
    FieldActor_SetSpritePriority(25, 2);
    FieldActor_SetSpritePriority(29, 2);
    Actor_SetPosition(25, PIXELS(160), PIXELS(392));
    Actor_SetPosition(27, PIXELS(176), PIXELS(408));
    Actor_SetPosition(29, PIXELS(152), PIXELS(376));
    Actor_SetSpriteRotationById(27, 0xc000);
    Actor_SetSingleChildFlags(Actor_Get(27), 0);
    Actor_SetSpriteRotationById(29, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(29), 0);
    Actor_FaceDirection(25, 0xd000, 0);
    Actor_FaceDirection(29, FACING_SOUTH, 0);
    Actor_SetAnimation(27, 13);
    Actor_SetAnimation(29, 13);
    Actor_SetAnimation(30, 13);
    Actor_SetAnimation(31, 13);
    Actor_SetAnimation(28, 13);
    Func_02003274();
  }
  Func_02000fa4();
  if (Event_GetFlag(0x8a6) == 0)
    Func_020040ec(15);
  Func_02004c14();
  Func_02004c2c(0, 0x8a3, 8, 9);
  OBJECT(8)->priorityFlags |= 32;
  OBJECT(9)->priorityFlags |= 32;
  Func_02004c1c();
  Func_02004c24(0, 12, 13);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  Actor_SetSingleChildFlags(Actor_Get(13), 0);
  Func_020047fc(gData_02004d0c);
  Actor_SetSingleChildFlags(Actor_Get(14), 0);
  flag = Event_GetFlag(0x8a2);
  if (flag == 0) {
    object = OBJECT(18);
    /* FAKEMATCH(one-pass): the solid bit set in a once-only block, which lays
     * out the branch over this block as the native does; as a plain
     * statement, 18 of 467 instructions differ in tla-en. */
    do {
      object->collisionFlags |= 8;
    } while (0);
    object = OBJECT(19);
    object->motionFlags = flag;
    object->y = 0x100000;
    Func_02004c24(1, 18, 19);
    Func_02004c94(1);
    object = OBJECT(20);
    object->collisionFlags |= 8;
    object = OBJECT(21);
    object->motionFlags = flag;
    object->y = 0x100000;
    Func_02004c24(2, 20, 21);
  }
  Actor_SetSingleChildFlags(Actor_Get(16), 0);
  Actor_SetSingleChildFlags(Actor_Get(17), 0);
  FieldActor_SetSpritePriority(10, 2);
  ((u8 *)OBJECT(23))[0x62] = 1;
  if (Event_GetFlag(0x8ab) != 0 && Event_GetFlag(0x8a7) == 0) {
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
  }
  return 0;
}
