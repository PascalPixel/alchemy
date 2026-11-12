/// @file
/// Places the listed actors on their cells.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

void Func_02003b14(s32 id, s32 owner);
void Func_020030a4(s32 id);
s32 Func_02003c04(struct Actor *object);
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003b3c(s32 id);
u8 Func_02003c24(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
void Func_020030bc(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Func_02003b14(8, gMainRoutineWork.currentOwner);
  Func_02003b14(9, gMainRoutineWork.currentOwner);
  Func_02003b14(10, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Func_020030a4(8);
  Func_020030a4(9);
  Func_020030a4(10);
  Actor_SetAnimation(9, 0);
  System_Wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  System_Wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_Get(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (Event_GetFlag(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02003c04(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02003adc(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02003b3c(id);
    cell->attributeB = 128;
    *slot = Func_02003c24(object->terrainId, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  System_Wait(10);
}

extern const u8 gData_02003dba[];
void Func_02002e30(const u8 *data);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02003224(struct Actor *object, struct Actor *source) {
  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  FieldActor_SetPositionAndResetMotion(source, 0, 0, 0);
}

s32 Func_02003b8c(void);
struct Actor *Func_02003c2c(s32 *position, s32 flags);
void Func_02003aec(void);
s32 Func_02003af4(void);
void Func_02003a54(struct Actor *object, s32 mode);
void Func_020034ac(s32 mode);
void Func_02003784(s32 level, s32 layer);
void Func_020035e0(void);

void Func_02003240(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target == 0)
    return;
  /* FAKEMATCH(dead): a test no u16 can pass enters the loop a second way, so it
   * is no natural loop and its references are not weighted; as if (target != 0)
   * goto test; return;, 75 of 191 instructions differ in tla-en. */
  if (list[0] & 0x10000)
    goto body;
  goto test;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Func_02003aec();
      if (!GameFlag_Test(flag)) {
        Func_02003a54(actor8, 1);
        Func_02003224(actor8, target);
        wait(1);
        Sound_Play(125);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(2);
        Func_02003a54(actor8, 0);
        ObjectVisual_CopyAttributes(id, 9);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        wait(2);
        Actor_SetPartAttribute(Actor_GetById(8), 7);
        wait(4);
        Actor_SetPartAttribute(Actor_GetById(8), 0);
        Func_020034ac(0);
        Func_02003224(target, actor8);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02003a54(actor8, 0);
        Func_02003224(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(185);
        /* FAKEMATCH(wrapper): forwarded through Call3, which builds each
         * 0x10000 apart; as MapGrid_SetShake(0x10000, 0x40000, 0x10000), 21
         * of 191 instructions differ in tla-en. */
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(actor8, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        /* FAKEMATCH(wrapper): forwarded through Call3, which builds each -1
         * apart; as MapGrid_SetShake(-1, -1, 0xe666), 23 of 191 instructions
         * differ in tla-en. */
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02003af4();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}
