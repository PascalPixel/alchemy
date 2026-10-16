#include "garoh.h"
#include "party_state.h"

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;

extern u32 gData_020056dc[];
extern u32 gData_020056f4[];
extern s16 gData_02005874[];
extern u32 gData_020059dc[];
extern s32 gData_02005a54[];
extern u32 gData_02005b5c[];
extern s32 gData_02005c1c[];

struct GarohPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const struct GarohPicture gData_02005de0[];

void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 dz);
void Event_FinishScreenTransition(void);
void Func_0200517c(const void *data, s32 a, s32 b);
void Func_020052c4(s32 value);

// Return the table for the current room.
const void *Func_02000440(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov65FRoom1) {
    if (Flag_IsSet(0x91b))
      return gData_02005874;
    return gData_020056f4;
  }
  if (scene == (s32)&SceneId_Ov65FRoom2)
    return gData_02005b5c;
  if (scene == (s32)&SceneId_Ov65FRoom3)
    return gData_020059dc;
  if (scene == (s32)&SceneId_Ov65FRoom4)
    return gData_02005a54;
  if (scene == (s32)&SceneId_Ov65FRoom5)
    return gData_02005c1c;
  return gData_020056dc;
}

static __inline__ void Func_0200517c_(const void *data, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own argument
   * order, as ☀️'s scene layer does (called directly, differ 12). */
  Func_0200517c(data, a, b);
}

static __inline__ void OffsetPosition_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarding through the inline sets the second argument before
   * the third's split constant is finished, as the native orders them (called
   * directly, the negs comes first: differ 2). */
  ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void CenterOnColumn_(s32 id, s32 column, s32 dz) {
  /* FAKEMATCH: the same forwarding as OffsetPosition_, for the same order
   * (differ 2). */
  ObjectMotion_CenterOnColumn(id, column, dz);
}

// 6A6's Func_02000e54 for this overlay: clears the motion of every object
// from 8 to 63, shows the picture the event work's index names and moves the
// party's current owner to it.
void Func_020004cc(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  s32 i;

  Func_020051ac();
  Func_02005324(0);
  // A goto loop: the bytes rebuild the zero on every pass, which a for
  // loop hoists (differ 12).
  i = 8;
loop:
  {
    struct ObjectRuntime *object = Object_GetById(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  index = *(s16 *)(work + 0x154);
  /* FAKEMATCH: a one-pass block keeps the index's decrement after the sound
   * call, where plainly sched2 ranks it above the call (differ 4 with the
   * forwarding inlines). */
  do {
    Func_02005394(158);
    index--;
  } while (0);
  Func_0200517c_(gData_02005de0[index].data, gData_02005de0[index].a,
                 gData_02005de0[index].b);
  Object_SetModeById(gPartyState.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gPartyState.currentOwner;

    if (index == 5)
      OffsetPosition_(owner, 0, -4);
    else
      CenterOnColumn_(owner, 2, -4);
  }
  FieldEvent_WaitFrames(6);
  Func_020052c4(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020051b4();
}
