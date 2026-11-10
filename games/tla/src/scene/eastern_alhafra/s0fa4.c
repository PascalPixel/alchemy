#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02004c84();

void Func_02000fa4(void) {
  s32 t1, t2, t8, t10, t12, t13, t18, t19, t24, v_r5;

  t1 = Event_GetFlag(0x8a6);
  if (t1 == 0)
    goto L_02009022;
  t2 = ((s32)Actor_Get(15));
  v_r5 = t2;
  Actor_SetPosition(15, PIXELS(336), PIXELS(536));
  Actor_SetAnimation(15, 4);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  FIELD_AT(v_r5, u8, 85) = 2;
  FIELD_AT(v_r5, s32, 40) = 0;
  FIELD_AT(v_r5, s32, 12) = -0x40000;
  FIELD_AT(v_r5, s32, 20) = -0x40000;
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 0, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x2100000, 0, 0);
L_02009022:
  t8 = Event_GetFlag(0x8a4);
  if (t8 == 0)
    goto L_0200903a;
  Actor_SetPosition(16, 0, 0);
L_0200903a:
  t10 = Event_GetFlag(0x8a5);
  if (t10 == 0)
    goto L_02009052;
  Actor_SetPosition(17, 0, 0);
L_02009052:
  t12 = Event_GetFlag(0x8a0);
  if (t12 == 0)
    goto L_020090a8;
  t13 = ((s32)Actor_Get(10));
  v_r5 = t13;
  Actor_SetPosition(10, PIXELS(504), PIXELS(520));
  Actor_SetAnimation(10, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  MapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          0);
  Func_02004c84(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0, 128);
L_020090a8:
  t18 = Event_GetFlag(0x8a1);
  if (t18 == 0)
    goto L_02009100;
  t19 = ((s32)Actor_Get(11));
  v_r5 = t19;
  Actor_SetPosition(11, PIXELS(408), PIXELS(536));
  Actor_SetAnimation(11, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  MapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          0);
  Func_02004c84(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0, 128);
L_02009100:
  t24 = Event_GetFlag(0x8a7);
  if (t24 == 0)
    goto L_02009122;
  MapGrid_CopyCells(32, 24, 2, 2, 30, 24);
L_02009122:
}
