// Draft, differ 68 of 193: places the scene's markers from its Func_080ceba8
// rows (a type, an event id, a flag and a pad): for each of the first 16 rows
// whose flag is clear and whose event has not run, a marker object goes at its
// event object, or at the map cell of a tile event, when that point is on
// screen. No ☀️ twin found. Left: the native reads the map header word (*map)[1]
// into a dead stack slot (sp + 4, frame 20); the draft drops it (frame 16) and
// its result buffer moves to sp + 4. The loop entry: the native repeats the
// first row's type tests before the loop, where this do-while shares them with
// its condition and jumps in (a for loop with the read inside differs 102, a
// while with the tests at the end 106). Elsewhere register choice. Computing
// both screen offsets before the bounds test matches the native interleaving.
#include "types.h"
#include "field_sprite.h"
#include "flag_call.h"
#include "object_runtime.h"
#include "ram_buffer.h"

s16 *Func_080ceba8(void);
s32 Func_080cec24(s32 id, void *result);
struct ObjectRuntime *ObjectEvent_GetObject(s32 id);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
struct ObjectRuntime *Func_080200c0(s32 sprite, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const void *script);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           u32 flags);

extern s32 Data_080f01b8[];

#define TILE_EVENT_MAGIC 0x31415927

void Func_080cf554(void) {
  s16 *entry = Func_080ceba8();
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0] & 0xffff0000;
  s32 originZ = origin[1] & 0xffff0000;
  u8 result[12];
  struct ObjectRuntime *object;
  s32 type;
  s32 id;
  s32 flag;
  s32 i;
  s32 x;
  s32 y;
  s32 z;
  s32 dx;
  s32 dz;

  if (entry == NULL)
    return;
  type = *entry++;
  i = 0;
  if (type == 0 || (type & 0x4000))
    return;
  do {
    id = entry[0];
    flag = entry[1];
    entry += 3;
    if (type != 128 && flag != -1 && GameFlag_Test(flag))
      continue;
    if (Func_080cec24(id, result))
      continue;
    object = ObjectEvent_GetObject(i + 64);
    if (object == NULL)
      continue;
    if (object->unk4c == TILE_EVENT_MAGIC) {
      x = object->action << 16;
      z = (s16)object->unk66 << 16;
      y = Map_GetTerrainHeightFar(0, x, z);
    } else {
      if ((u16)(type - 128) <= 1)
        continue;
      if (type == 130 || type == 132 || type == 133 || type == 131)
        continue;
      x = object->x;
      y = object->y;
      z = object->z;
    }
    dx = (x - originX) >> 16;
    dz = (z - originZ - y) >> 16;
    if ((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
      continue;
    if (flag == -1 || GameFlag_Test(flag))
      continue;
    object = Func_080200c0(673, x, y, z);
    if (object != NULL) {
      object->motionFlags = 0;
      ObjectDispatch_InitializeFar(object, Data_080f01b8);
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      Object_SetMode(object, 1);
      ((struct FieldSprite *)object->animation)->priority = 1;
    }
  } while (++i <= 15 && (type = *entry++) != 0 && !(type & 0x4000));
}
