/// @file
/// Places the scene's markers from its scene item rows (a type, an event id, a
/// flag and a pad): for each of the first 16 rows whose flag is clear and whose
/// event has not run, a marker object goes at its event object, or at the map
/// cell of a tile event, when that point is on screen.
#include "types.h"
#include "field_sprite.h"
#include "flag_call.h"
#include "actor.h"
#include "ram_buffer.h"

struct ActorView;

s16 *Func_080ceba8(void);
s32 Func_080cec24(s32 value, s32 *out);
struct Actor *ObjectEvent_GetObject(u32 objectId);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScriptFar(struct ActorView *object, const s32 *script);
void Actor_SetSingleChildFlagsFar(struct ActorView *object, u32 flags);

extern const s32 Data_080f01b8[];

/// Places a marker at each waiting scene item event that is on screen.
void Func_080cf554(void) {
  // The row type is s16, as the rows are: with it the copied head test stays
  // apart from the bottom one.
  s16 *entry = Func_080ceba8();
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0] & 0xffff0000;
  s32 originZ = origin[1] & 0xffff0000;
  s32 result[3];
  // volatile: headerWord lives at sp+4, stored once; as a plain s32, 86 of
  // 193 instructions differ in tla-en.
  volatile s32 headerWord;
  struct Actor *object;
  s16 type;
  s32 id;
  s32 flag;
  s32 i;
  s32 x;
  s32 y;
  s32 z;
  s32 dx;
  s32 dz;

  // DEAD: the map header's second word is never read; the ROM keeps its load
  // and store.
  headerWord = (*(s32 **)map)[1];
  if (entry == NULL)
    return;
  for (i = 0; i <= 15 && (type = *entry++) != 0 && !(type & 0x4000); i++) {
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
    // A tile event's object holds its map cell, marked by this value.
    if (object->unk4c == 0x31415927) {
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
    // Both differences are taken before either is shifted; shifted as each is
    // taken, 2 of 193 instructions differ in tla-en.
    dx = x - originX;
    dz = z - originZ - y;
    dx >>= 16;
    dz >>= 16;
    if ((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
      continue;
    if (flag == -1 || GameFlag_Test(flag))
      continue;
    object = Func_080200c0(673, x, y, z);
    if (object != NULL) {
      object->motionFlags = 0;
      Actor_StartScriptFar((struct ActorView *)object, Data_080f01b8);
      Actor_SetSingleChildFlagsFar((struct ActorView *)object, 0);
      Actor_SetMode(object, 1);
      ((struct FieldSprite *)object->animation)->priority = 1;
    }
  }
}
