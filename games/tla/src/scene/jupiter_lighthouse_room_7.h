#ifndef TLA_JUPITER_LIGHTHOUSE_ROOM_7_H
#define TLA_JUPITER_LIGHTHOUSE_ROOM_7_H

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "input.h"
#include "text/msg_ids.h"
#include "frame.h"

struct JupiterLighthouseRoom7Cell {
  u8 unk00[6];
  s16 x;
  s16 z;
};

extern s16 gData_02002c64[][2];

void Func_02000060(s32 mode);
void Func_020005e8(s32 mode);
void Func_020009f8(s32 id, s32 x, s32 z, s32 dx, s32 dz);
void Func_020024ac(void);

#endif
