#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

#include "flag_call.h"
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_020020ac(s32 a);
void Func_020020d4(s32 a);
void Func_0200207c(s32 a);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
s32 ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void Battle_WaitMode0(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void Func_02001f2c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Func_02001f2c(a, b, c, d, e, f);
}

void Func_02000078(void) {
  if (Flag_IsSet(0x985)) {
    u8 *work = Ram_HeapSlots->eventWork;
    u32 i;

    Func_02001f5c();
    Func_020020ac(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop
     * notes: the bytes rebuild the zero and the height on every pass, and a for
     * loop hoists both (differ 39). */
    i = 8;
  loop:
    {
      struct ObjectRuntime *object = Object_GetById(i);

      if (object != 0) {
        object->motionFlags = 0;
        object->y = 0x200000;
      }
    }
    i++;
    if (i <= 63)
      goto loop;
    if (!Flag_IsSet(0x201)) {
      Func_020020d4(158);
      Func_02001f2c_(51, 38, 70, 22, 1, 2);
    }
    Object_SetModeById(gPartyState.currentOwner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
    ObjectMotion_CenterOnColumn(gPartyState.currentOwner, 2, -4);
    Battle_WaitMode0(8);
    Func_020020d4(123);
    Func_0200207c(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001f64();
  }
}
