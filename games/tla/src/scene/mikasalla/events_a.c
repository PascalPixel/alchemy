#include "local.h"

struct WarpEntry {
  s32 map;
  u16 x;
  u16 z;
};

extern struct WarpEntry gData_0200073c[];

void Func_02000610(s32 map, s32 x, s32 z);
void Func_020006b8(s32 value);
void Func_020006f0(s32 value);
void ObjectMotion_CenterOnColumn(s32 id, s32 a, s32 b);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void Func_02000610_(s32 map, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline loads the warp's fields as the
   * native does, the map last (differ 12 as a direct call). */
  Func_02000610(map, x, z);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarding through the inline sets the arguments up in their own
   * order; the direct call negates the offset last (differ 2). */
  ObjectMotion_CenterOnColumn(id, column, offset);
}

void Func_0200009c(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 id;
  s16 *choice;
  s32 index;

  Func_02000648();
  Func_020006d0(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Object_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      /* FAKEMATCH: the zero pinned to r3 is rebuilt on every pass as the native
       * does; loop.c hoists a plain zero out of the loop into a saved register
       * (differ 24). */
      register s32 zero asm("r3") = 0;

      *flags = zero;
    }
  }
  choice = (s16 *)(work + 340);
  index = *choice;
  /* FAKEMATCH: the empty asms on each side of the call keep the choice's
   * decrement after it, as in overlay 66d's Func_02000368 (differ 2 without
   * either). */
  asm("");
  Func_020006f0(158);
  asm("");
  index--;
  Func_02000610_(gData_0200073c[index].map, gData_0200073c[index].x,
                 gData_0200073c[index].z);
  /* FAKEMATCH: the empty asm ends the shift's output dependences on the later
   * calls (r4 is call-used), so sched2 takes the table address first as the
   * native does (differ 2). */
  asm("");
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
  {
    s32 mode = 2;

    /* FAKEMATCH: the mode passes through an asm so that it is set before the
     * owner id is loaded (differ 2 without it). */
    __asm__("" : "+l"(mode));
    Object_SetModeById(gPartyState.currentOwner, mode);
  }
  if (index != 4) {
    ObjectMotion_CenterOnColumn_(gPartyState.currentOwner, 2, -8);
    FieldEvent_WaitFrames(10);
  }
  Func_020006b8(*choice);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000650();
}

void Func_0200014c(void) { Func_020006d8(18, 2, 8); }

#include "scene_ids.h"

extern s32 gData_020012ac[];
extern s32 gData_020010d8[];

s32 *Func_0200015c(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_020012ac;
  return gData_020010d8;
}
