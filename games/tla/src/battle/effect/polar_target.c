#include "types.h"
#include "actor.h"
#include "animspr.h"
#include "frame.h"
#include "fx_scene.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "macros.h"

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

struct CompositeAnimation {
  u8 unk00[0x11];
  u8 displayFlags;
  u8 unk12[7];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

void Func_080dd950(void);
void Func_080debd8(void);
void FieldEvent_ShowStatusMessage(void);
void Func_080e4730(void);
void Func_080e9db4(void);
void Func_080e87c4(void);

void BattleEffect_FinishSceneAndReleaseHeapBlock(void);

/// unk26a from main_routine_work.h.
struct PartyPauseView {
  u8 unk000[0x26a];
  s16 pausedObject;
};

void BattleEffect_ClearChildValueOnMismatch(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  if (scene->fieldKind == 2) {
    struct PartyPauseView *party;

    BattleEffect_FinishSceneAndReleaseHeapBlock();
    party = (struct PartyPauseView *)&gMainRoutineWork;
    /* FAKEMATCH: ⚓️ loads gMainRoutineWork's address before it builds the field
     * offset; read as a plain field the offset's first half is scheduled ahead
     * of the load (differ 2) */
    asm volatile("");
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 0;
  }
}

/// Additional processing for each type.
void FieldEvent_RunTypeHandler(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  switch (scene->fieldKind) {
  case 8:
    Func_080dd950();
    break;
  case 10:
    Func_080debd8();
    break;
  case 16:
    FieldEvent_ShowStatusMessage();
    break;
  case 23:
    Func_080e4730();
    break;
  case 24:
    Func_080e9db4();
    break;
  case 28:
    Func_080e87c4();
    break;
  }
}

void Actor_ApplyRandomChildValues(struct Actor *owner) {
  s32 state;
  struct CompositeAnimation *target;
  s32 initialCount;
  s32 count;
  struct AnimationEntry **entry;
  struct AnimationEntry *current;
  u32 frame;
  s32 value;

  state = owner->animationKind;
  if (state == 1) {
    target = owner->animation;
    if (target != 0 && (target->displayFlags & state) == 0) {
      initialCount = target->count;
      if (initialCount != 0) {
        entry = target->entries;
        frame = gFrameCount;
        count = initialCount;
        do {
          current = *entry++;
          value = frame % 6;
          count--;
          current->param = value;
        } while (count != 0);
      }
      target->dirty = 1;
    }
  }
}

void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle) {
  s32 values[3];

  if (object != 0) {
    SET_VECTOR(values, object->x, object->y, object->z);
    Vector_AddPolarOffset(magnitude, angle, values);
    Actor_SetTargetPosition(object, values[0], values[1], values[2]);
  }
}
