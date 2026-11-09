#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#include "actor.h"
#include "system/vector3.h"
#include "system/io_reg.h"
#include "event_inline.h"

extern u8 gData_0200308c[];
extern u8 Data_02003098[];
extern u8 Data_02002fd7[];
extern u8 gData_02002fe8[];
extern u8 Data_02003090[];
extern u8 Data_02003024[];
extern u8 Data_02003060[];
extern u8 Data_02003030[];
s32 __modsi3();
s32 Func_020020d4();
s32 Func_0200210c();

void Func_020002b4(void) {
  struct Vector3 position;
  s8 mode = gData_0200308c[0];

  if (mode == 1) {
    s32 *time = (s32 *)Data_02003098;
    s32 t = *time;
    s32 level = Data_02002fd7[(t >> 3) & 15];

    {
      s32 value = level << 8;
      /* FAKEMATCH(one-pass): an empty block holds the shift first; with no
       * block, 2 of 96 instructions differ in tla-en. */
      do {
      } while (0);
      value |= 16 - level;

      REG_BLDALPHA = value;
    }
    /* FAKEMATCH(one-pass): the step in a once-only block holds it after the
     * store; as the plain statement, 2 of 96 instructions differ in tla-en. */
    do {
      t++;
    } while (0);
    *time = t;
  } else if (mode == 2) {
    s32 *time = (s32 *)Data_02003098;
    s32 step = (*time >> 2) % 28;

    {
      u16 *table = (u16 *)gData_02002fe8;
      /* FAKEMATCH(temporary): the offset in a local puts the table first in the
       * load; as table[step], 2 of 96 instructions differ in tla-en. */
      s32 at = step * 2;
      s32 value = *(u16 *)((u8 *)table + at);

      REG_BLDALPHA = value;
    }
    if (step == 0) {
      *(s32 *)Data_02003090 = 1;
      *(s32 *)Data_02003024 = step;
      Sound_Play(153);
    }
    if (step == 14) {
      *(s32 *)Data_02003090 = 0;
      *(s32 *)Data_02003024 = 1;
      Sound_Play(153);
    }
    (*time)++;
  }
  position.x = 0x3400000;
  position.y = 0;
  position.z = 0x1c00000;
  Func_020020d4(&position);
  if (*(s32 *)Data_02003090 != 0) {
    ((struct Actor *)Data_02003060)->y = position.x;
    ((struct Actor *)Data_02003060)->z = position.z;
    Func_0200210c(Data_02003060);
  }
  if (*(s32 *)Data_02003024 != 0) {
    ((struct Actor *)Data_02003030)->y = position.x;
    ((struct Actor *)Data_02003030)->z = position.z;
    Func_0200210c(Data_02003030);
  }
}
