#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_0200012c();
void Func_02000168();
void Func_02004c80();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

void Func_020001f8(s32 a0) {
  s32 t1, t2, t3, t4, v10, v11, v5, v6, v7, v8, v9;

  v5 = a0;
  v9 = (FIELD_AT(v5, s32, 8) >> 20);
  v10 = (FIELD_AT(v5, s32, 16) >> 20);
  t1 = Func_0200012c(0, v9, v10);
  v8 = t1;
  t2 = Func_0200012c(2, v9, v10);
  v6 = t2;
  v11 = (34 + v5);
  t3 = Map_GetTerrainHeight(FIELD_AT((34 + v5), u8, 0), FIELD_AT(v5, s32, 8),
                            FIELD_AT(v5, s32, 16));
  v7 = (t3 >> 19);
  t4 = Event_GetFlag(FIELD_AT(v5, s16, 100));
  if (t4 == 0)
    goto L_02008268;
  v6 = (v6 & -0xff01);
  v8 = (v8 & -129);
  if ((FIELD_AT(v5, s32, 20) >> 19) == v7)
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
  Func_02004c80((v9 << 20), (v10 << 20), FIELD_AT(v11, u8, 0), v7);
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
