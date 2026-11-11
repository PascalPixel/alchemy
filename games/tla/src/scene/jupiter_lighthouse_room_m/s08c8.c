#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as a direct call, 12 of 61
   * instructions of Func_020008c8 differ in tla-en. */
  Func_0802b450(a, b, c, d, e, f);
}

void Func_020008c8(void) {
  if (Event_GetFlag(0xa1f) != 0)
    return;
  Event_ClearFlag(0x202);
  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(125);
  FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(208), 1);
  Func_020048f4();
  FieldEvent_Wait(6);
  Func_0200474c_(49, 103, 4, 3, 49, 76);
  FieldEvent_Wait(6);
  Event_PlaySound(133);
  MapGrid_CopyCellsLayered(49, 100, 4, 3, 49, 76);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}
