// Measured under -Os, 5 November 2026: differ 8 (tla-en); notes below may predate it.
// Draft, differ 24 of 150, skeleton gap 10 (tla-en, 2026-11-01)
// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs; flag + 1 set means the actor is parked at (0, 0), otherwise
// its cell gets attributeB 128 and its shape-grid byte is rewritten, and flag
// set makes it copy actor 9's visual attributes. Same shape as
// resource_691_c_02002d08.c and 68f's 020025e0.
// Measured: the native's [sp,#4] word is the high half of an 8-byte local, with
// sp+0 its unwritten low half (frame 16; flag at 8, map at 12). A separate
// pointer took r6 (t tied to it, so list lost fp; differ 96), a plain volatile
// local sits above the spills and hoists its address (differ 64), and only a DI
// pseudo gives this layout. ldr r6, [sp,#4] is a local read before
// Func_02003b3c that crosses that call, which is why it is r6 and not a reload.
// Left: the first set, `pair.cell = cells`, exists only to keep the DI pseudo;
// with one set CSE replaces the read and the pseudo disappears (differ 62). It
// costs one str and puts t in r6, the else block's 16 differing lines. The native
// has one store and t in r3. Reassigning the temporary after the store keeps the
// pseudo with one store (`target = (u8 *)(cells + index); pair.cell = target;
// target = shapes + index;`, differ 41, gap 10/9), but the add then writes r3
// and the read stays above the call. That is the better lead: what is missing
// there is adds r6, r6, r3 and the read landing after the bl.
// Also left: `adds r2, r2, r3` for shapes, where the native's first operand is
// the grid (here it is the offset; a grid local, s32 casts, operand swap and
// &gMapShapeGrid[] all leave it), and two scheduling pairs, ldr r5 / movs r2 at
// entry and adds r7 / str at the loop top.
// Tried, no gain: goto loop entered at the bottom (95), do { } while (0) around
// Func_02003b3c (64), a volatile pair word (73), reading the word earlier (92),
// no local for the read (68), reading it back into cells (66), cells stepped in
// place (64), an owner pointer local (29).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02003b14(s32 id, s32 owner);
void Func_020030a4(s32 id);
void Func_02003b0c(s32 id, s32 x, s32 z);
s32 Func_02003c04(struct Actor *object);
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003b3c(s32 id);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void wait(s32 frames);

void Func_020030bc(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Func_02003b14(8, gMainRoutineWork.currentOwner);
  Func_02003b14(9, gMainRoutineWork.currentOwner);
  Func_02003b14(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_020030a4(8);
  Func_020030a4(9);
  Func_020030a4(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02003b0c(8, 0, 0);
  Func_02003b0c(9, 0, 0);
  Func_02003b0c(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  {
    s32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    s32 nextFlag = flag + 1;
    u8 *layer = &object->terrainId;
    struct MapCell *cells = map->layers[*layer].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(nextFlag)) {
      Func_02003b0c(id, 0, 0);
    } else {
      s32 index = Func_02003c04(object);
      u8 *cell = (u8 *)&cells[index];
      u8 *slot = shapes + index;
      s32 shape = Func_02003adc(*layer, object->x, object->z);
      s32 ground = Map_GetTerrainHeight(*layer, object->x, object->z);

      Func_02003b3c(id);
      cell[3] = 128;
      *slot = Func_02003c24(*layer, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}
