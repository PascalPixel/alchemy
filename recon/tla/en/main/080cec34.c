// Draft, differ 142 (150 before statements reordered 2026-10-26) of 241 (167 before, 2026-10-25), from the listing: spawns
// the current scene's item objects (chests, jars, buried items) from the rows
// Func_080ceba8 finds, at most 16. The loop is a for whose condition reads
// the next row's type, as the native's test at its foot shows (its i > 15
// test folded away on entry), with the row's three halfwords read by index.
// Left: the loop's entry. alchemy-gcc's jump pass copies the exit test in
// front of the loop as the native has it, but jump2 cross-jumps the copy
// into the test at the foot (the entry becomes a branch to it), which the
// native never does; the registers after it follow from that. The kind
// chain matches as an else-if chain.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

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
  s32 type;
  s32 id;
  s32 flag;
  s32 item;
  s32 position[3];
  s32 kind;
  s32 buried;
  struct Actor *object;

  if (rows == NULL)
    return;
  for (i = 0; i < 16 && (type = *rows++) != 0 && !(type & 0x4000); i++) {
    flag = rows[1];
    id = rows[0];
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
            struct MapWork *map;
            struct MapCell *cells;
            struct MapCell *cell;
            s32 x;
            s32 z;

            object->motionFlags = 2;
            object->terrainHeight += 0xfff00000;
            object->y = object->terrainHeight;
            map = Ram_HeapSlots->mapWork;
            cells = (struct MapCell *)gMapCellBuffer;
            z = object->z >> 20;
            x = object->x >> 20;
            if (map != NULL)
              cells = map->layers[0].cells;
            cell = &cells[x + (z << 7)];
            if ((u32)(cell->collisionCode - 242) <= 5)
              cell->collisionCode = 0;
          }
        }
        Actor_SetMode(object, 1);
        Actor_SetSingleChildFlagsFar(object, 0);
        if (GameFlag_Test(flag))
          Actor_SetMode(object, 2);
        Actor_ResetMotion(object);
        S16_AT(object, 100) = object->x / 0x10000;
        S16_AT(object, 102) = object->z / 0x10000;
        BYTE_AT(object, 35) = 1;
        BYTE_AT(object, 89) = 1;
        work->objects[i] = object;
        if (type == 131)
          Func_080d3b28(i + 64, item);
        if (type == 133)
          BYTE_AT(object, 89) = 0;
      }
    }
  }
}
