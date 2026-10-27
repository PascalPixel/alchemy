#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct AnemosInnerSanctumPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const s16 gData_020043e4[];
extern const struct AnemosInnerSanctumPicture gData_02004410[];

void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003320(const void *data, s32 a, s32 b);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003450(s32 value);
void Func_02003488(s32 skip);
void Func_020034b8(s32 cue);

static __inline__ void Func_02003320_(const void *data, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003320(data, a, b);
}

/// The scene opening: clears the actors' motion flags, plays the entrance's
/// map edit and steps the leader in.
void Func_02001c94(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Func_02003398();
  Func_02003488(0);
  // A goto loop, as in overlay 6a6's Func_02000e54 (its twin): it carries no
  // loop notes.
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
  Func_020034b8(158);
  if (gPartyState.scene == (s32)&SceneId_Ov6B0Room1) {
    Func_02003320_(gData_020043e4, 33, 7);
  } else {
    s32 index = work->value / 2 - 1;

    Func_02003320_(gData_02004410[index].data, gData_02004410[index].a,
                   gData_02004410[index].b);
  }
  Object_SetModeById(gPartyState.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 0, -4);
  FieldEvent_WaitFrames(6);
  Func_02003450(work->value);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020033a0();
}
