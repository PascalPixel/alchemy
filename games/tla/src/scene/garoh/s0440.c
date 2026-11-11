#include "garoh.h"
#include "main_routine_work.h"
#include "event_inline.h"

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

void Event_FinishScreenTransition(void);

/// Return the table for the current room.
const void *Func_02000440(void) {
  s16 scene = gMainRoutineWork.scene;

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
  /* FAKEMATCH(wrapper): the call through this inline, so each call keeps its
   * own argument order; as a direct call to Map_PlayMapEdit, 12 of 72
   * instructions of Func_020004cc differ in tla-en. */
  Map_PlayMapEdit(data, a, b);
}

static __inline__ void OffsetPosition_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH(wrapper): the call through this inline, so the second argument is
   * set before the third's split constant is finished; as a direct call to
   * Actor_OffsetPositionAndResetMotion, 2 of 72 instructions of Func_020004cc
   * differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void CenterOnColumn_(s32 id, s32 column, s32 dz) {
  /* FAKEMATCH(wrapper): the call through this inline, for the same order as
   * OffsetPosition_; as a direct call to Actor_CenterOnColumn, 2 of 72
   * instructions of Func_020004cc differ in tla-en. */
  Actor_CenterOnColumn(id, column, dz);
}

/// 6A6's Func_02000e54 for this overlay: clears the motion of every object
/// from 8 to 63, shows the picture the event work's index names and moves the
/// party's current owner to it.
void Func_020004cc(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  s32 i;

  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop: the bytes rebuild the zero on every pass, which a for
  // loop hoists (differ 12).
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  index = *(s16 *)(work + 0x154);
  Event_PlaySound(158);
  index--;
  Func_0200517c_(gData_02005de0[index].data, gData_02005de0[index].a,
                 gData_02005de0[index].b);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (index == 5)
      OffsetPosition_(owner, 0, -4);
    else
      CenterOnColumn_(owner, 2, -4);
  }
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020051b4();
}
