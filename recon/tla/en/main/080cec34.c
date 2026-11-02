// Matches: differ 0 (tla-en, 2026-11-01). From the listing: spawns the current
// scene's item objects (chests, jars, buried items) from the rows
// Func_080ceba8 finds, at most 16. The loop is an if and a do-while with the
// row tests at its foot, as the sibling Func_080cf554 has it; s16 type makes
// the foot's 0x4000 a pool load, which keeps it apart from the entry's test.
// motionFlags is written through a local holding its address for its whole
// life (S2: no device): as the field store, the mask's zero stays in a register
// and cell->collisionCode = 0 reuses it (8 of 241 differ). Not yet checked
// in the other editions.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"


struct SceneItemWork {
  u8 unk000[0x114];
  struct Actor *objects[16];
  u8 unk154[0x18a - 0x154];
  s8 mode;
};

extern u8 gMapCellBuffer[];
s16 *Func_080ceba8(void);
s32 Func_080cec24(s32 value, s32 *out);
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 GameFlag_Test(s32 flag);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           u32 flags);
void Func_080d3b28(s32 id, s32 item);

void Func_080cec34(void) {
  s16 *rows = Func_080ceba8();
  struct SceneItemWork *work = Ram_HeapSlots->eventWork;
  s32 i;
  s16 type;
  s32 id;
  s32 flag;
  s32 item;
  s32 position[3];
  s32 kind;
  s32 buried;
  struct Actor *object;

  if (rows == NULL)
    return;
  i = 0;
  type = *rows++;
  if (type == 0 || (type & 0x4000))
    return;
  do {
    id = rows[0];
    flag = rows[1];
    item = rows[2];
    rows += 3;
    if ((type == 128 || flag == -1 || !GameFlag_Test(flag)) &&
        !Func_080cec24(id, position)) {
      buried = 0;
      if ((u16)(type - 128) <= 1)
        kind = 487;
      else if (type == 130)
        kind = 265;
      else if (type == 132)
        kind = 322;
      else if (type == 131)
        kind = 489;
      else {
        buried = 1;
        kind = 489;
      }
      object = Func_080200c0(kind, position[0], position[1], position[2]);
      if (object != NULL) {
        if (work->mode == 3) {
          object->motionFlags = 2;
          object->terrainHeight = 0;
          object->y = 0;
        } else {
          object->terrainHeight =
              Map_GetTerrainHeightFar(0, object->x, object->z);
          object->y += object->terrainHeight;
          if (buried) {
            u8 *motionFlags = &object->motionFlags;
            struct MapWork *map;
            struct MapCell *cells;
            s32 x;
            s32 z;

            *motionFlags = 2;
            object->terrainHeight += 0xfff00000;
            object->y = object->terrainHeight;
            map = Ram_HeapSlots->mapWork;
            x = object->x >> 20;
            z = object->z >> 20;
            cells = (struct MapCell *)gMapCellBuffer;
            if (map != NULL)
              cells = map->layers[0].cells;
            cells += x + (z << 7);
            if ((u32)(cells->collisionCode - 242) <= 5)
              cells->collisionCode = 0;
          }
        }
        Actor_SetMode(object, 1);
        Actor_SetSingleChildFlagsFar(object, 0);
        if (GameFlag_Test(flag))
          Actor_SetMode(object, 2);
        Actor_ResetMotion(object);
        object->action = object->x / 0x10000;
        object->unk66 = object->z / 0x10000;
        object->priorityFlags = 1;
        object->collisionFlags = 1;
        work->objects[i] = object;
        if (type == 131)
          Func_080d3b28(i + 64, item);
        if (type == 133)
          object->collisionFlags = 0;
      }
    }
    i++;
    if (i > 15)
      break;
    type = *rows++;
    if (type == 0)
      break;
  } while (!(type & 0x4000));
}
