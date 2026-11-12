#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "actor.h"

s32 Func_0200012c();
s32 Func_02000168(u32 layer, s32 x, s32 y, s32 value);
void Func_02004c80();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

void Func_020001f8(struct Actor *object) {
  s32 t1, t2, t3, t4, v10, v6, v7, v8, v9;
  struct Actor *v5;
  u8 *v11;

  v5 = object;
  v9 = (v5->x >> 20);
  v10 = (v5->z >> 20);
  t1 = Func_0200012c(0, v9, v10);
  v8 = t1;
  t2 = Func_0200012c(2, v9, v10);
  v6 = t2;
  v11 = &v5->terrainId;
  t3 = Map_GetTerrainHeight(v5->terrainId, v5->x, v5->z);
  v7 = (t3 >> 19);
  t4 = Event_GetFlag(v5->timer);
  if (t4 == 0)
    goto L_02008268;
  v6 = (v6 & -0xff01);
  v8 = (v8 & -129);
  if ((v5->terrainHeight >> 19) == v7)
    goto L_02008282;
  v7 = (v7 + -4);
  goto L_02008282;
L_02008268:
  v6 = (v6 & 255);
  v6 = (v6 | 0xff00);
  v8 = (v8 | 128);
  v7 = (v7 + 4);
  Scene_SetActorSpritePriority(v5, 2);
L_02008282:
  Func_02000168(0, v9, v10, v8);
  Func_02000168(2, v9, v10, v6);
  Func_02004c80((v9 << 20), (v10 << 20), *v11, v7);
}

#include "field_event.h"
#include "field.h"

void Func_02004bc0();

void Func_020002bc(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = (struct FieldActor *)Actor_Get(row[0]);
  animation = row[1];
  flag = row[2];

  Actor_SetSingleChildFlags(object, 0);
  Func_02004bc0(object, (u16)animation + Event_GetFlag((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  Func_020001f8(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

#include "room.h"
#include "field_effect.h"

struct EffectHolder {
  u8 unk00[0x1a];
  s16 objectId;
};

void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

/// While index is at most 7, spawns effect type 0x117 index steps below the
/// object the effect work names and shrinks the object a little; mode 1
/// restores its size, sets its flag, hands it to Func_020001f8 and clears its
/// position.
void Func_02000338(s32 mode, s32 index) {
  struct EffectHolder *holder = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_Get(holder->objectId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = 0x117;
    options.startScaleX = options.startScaleY = 0x10000;
    options.targetScaleX = options.targetScaleY = 0x8000;
    options.rotation = (Random16Far() & 15) << 12;
    Func_02003b30(object->x, object->y - index * 3 * 0x10000 + 0x100000,
                  object->z, ((Random16Far() & 15) - 8) << 13, 0, 0, 0x5a0000,
                  &options);
    object->scaleY -= 0x1f00;
  }
  if (mode == 1) {
    object->scaleY = 0x10000;
    Event_SetFlag(object->timer);
    Func_020001f8(Actor_Get(holder->objectId));
    object->x = object->y = object->z = 0;
  }
}
