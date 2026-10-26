#include "fixed_point_position.h"
#include "types.h"
#include "iwram_call.h"
#include "battle_display.h"
#include "battle_work.h"
#include "ram_buffer.h"
#include "serial_runtime.h"

#define REG_SIOCNT (*(volatile u32 *)0x04000128)

void SceneTransform_RewindStack(void);
void SceneTransform_SetPosition(s32 *position);
void Func_080150ac(s32 yaw);
void SceneTransform_ApplyPitch(s32);

void Func_08118adc(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct FixedPointPosition eyeOffset;
  s32 *focus;
  s16 delta;
  u32 linkId;

  if (Ram_HeapSlots->battleWork->twoSided != 0) {
    if ((gLinkStatus & 3) != 3) {
      Ram_HeapSlots->battleWork->unk051++;
      if (Ram_HeapSlots->battleWork->unk051 > 24) {
        Ram_HeapSlots->battleWork->linkPaused = 1;
      }
    } else {
      linkId = (REG_SIOCNT << 26) >> 30;
      if (Ram_HeapSlots->battleWork->linkSide != linkId) {
        Ram_HeapSlots->battleWork->linkPaused = 1;
      }
      Ram_HeapSlots->battleWork->unk051 = 0;
    }
  }

  if (transition->frames != 0) {
    delta = transition->targetYaw - camera->yaw;
    delta /= 16;
    camera->yaw += delta;
    transition->frames--;
  }

  focus = camera->pos;
  if (camera->followPos != 0) {
    focus = camera->followPos;
  }

  SceneTransform_RewindStack();
  Func_080150ac((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);
  SceneTransform_SetPosition(focus);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset.x, (s32 *)camera);

  if (transition->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}
