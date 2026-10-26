// Under -mthumb-slow-loads (26 October 2026): differ 2 of 108 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it, five minutes of permute; none reaches 0. Left: the
// native copies the camera into r1 for the vector call before it loads the
// distance; a pinned copy in r1 at either place keeps the load first.
// Near miss, differ 4 of 108 (two reordered rows): The Broken Seal's
// BattlePresentation_UpdateCamera (games/tbs/src/battle/battle/setup.c) with
// this game's heap slots, the battle work read from its slot at each use,
// and this game's view calls (rewind the transform stack, yaw, pitch, then
// the position). Left: the native reads the link side (ldrb r2) before the
// SIO shift (lsls r3, #26), and copies the camera into r1 for the vector
// call before it loads the distance. Tried: the side read first (10), the
// distance through a word index (4), z set first (10), four minutes of
// permute (no gain).
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

void BattlePresentation_UpdateCamera(void) {
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
