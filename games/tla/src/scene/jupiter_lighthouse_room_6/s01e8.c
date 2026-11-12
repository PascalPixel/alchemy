/// @file
/// Scene code of overlay 0x69f, in ROM order, from s01e8's offset up to the next object the overlay links.

#include "jupiter_lighthouse_room_6.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "scene_tables.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;
extern s32 gData_020023c0;
extern u16 gData_020023e8[];

s32 __modsi3();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

s32 Func_020001e8(void) {
  struct JupiterLighthouseRoom6EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  if (Event_GetFlag(0xa19)) {
    Actor_SetPosition(12, PIXELS(72), PIXELS(600));
    Func_020000b4(0);
  }
  if (Event_GetFlag(0x200))
    Func_020000b4(0);
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    Func_02001250(33, 0x280, 0x380, 20, 31);
  } else {
    Func_02001250(43, 0x280, 0x380, 21, 31);
    Func_0200196c(21, -3, 0x204);
  }
  return 0;
}

extern s16 gData_0200242a;
extern struct Actor *gData_0200242c[];
extern u16 gData_0200252e;
s32 Func_020018bc(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

s32 Func_02000288(void) {
  Scene_SetArrivalFlags(0);
  Func_020003a4(0);
  if (!Event_GetFlag(265))
    Event_ClearFlag(0xa19);
  return 0;
}
