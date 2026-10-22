#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "battle_system.h"
#include "field_event.h"
#include "edition.h"

struct FieldEffectState {
  u8 padding0[0x1AC];
  s32 effect;
  u8 padding1B0[4];
  s32 delay;
};

struct FieldObject {
  u8 padding0[0x34];
  u16 savedX;
  u16 savedY;
  s16 x;
  u16 y;
};

void Event_WaitTransitionFramesFar(void);
void Event_StartScreenTransitionFar(void);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#endif

void Inn_PlaySleep(s32 roomPrice) {
  s16 objects[8];
  s32 count;
  s32 index;
  struct FieldObject *object;
  struct FieldEffectState *state;

  count = Party_ListActiveOwnersFar(objects);
  Party_AdjustCoinsFar(-roomPrice);

  for (index = 0; index < count; index++) {
    object = UnitGet(objects[index]);
    if (object->x != 0) {
      object->x = object->savedX;
      object->y = object->savedY;
      UnitRatioFar(objects[index]);
    }
  }

  state = Ram_HeapSlots->eventWork;
  state->effect = 0x209;
  state->delay = 60;
  WaitFrames(20);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  BattleEffect_SetPrimaryBufferValueFar();
#else
  Event_FinishScreenTransitionFar();
#endif
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  Event_DelayEffectFramesFar();
#else
  Event_WaitTransitionFramesFar();
#endif
  Audio_PlayCue(86);
  AudioCommand_WaitForStateByteClear();
  WaitFrames(10);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  BattlePalette_StopBlendAndReleaseBufferFar();
#else
  Event_StartScreenTransitionFar();
#endif
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
  Event_DelayEffectFramesFar();
#else
  Event_WaitTransitionFramesFar();
#endif
  WaitFrames(30);
  ((struct FieldEffectState *)Ram_HeapSlots->eventWork)->delay = 16;
}
