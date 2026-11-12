#ifndef TLA_LEMURIA_DOCK_ROOM_H
#define TLA_LEMURIA_DOCK_ROOM_H

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "event_inline.h"
#include "frame.h"

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct SceneWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0x44];
  s32 unk1ac;
  u8 unk1b0[0x14];
  u16 unk1c4;
};

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->eventWork)

extern u8 MsgAreWeSetting[];
extern s32 gData_0200057c[];
extern s32 gData_0200059c[];
extern struct WarpTarget gData_02000610;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000254(const struct WarpTarget *warp, s32 door, s32 mode);
void Func_020002f8(void);
void Func_0200046c(void);
void Func_02000474(void);

#endif
