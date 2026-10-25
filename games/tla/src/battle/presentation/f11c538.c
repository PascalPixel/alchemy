#include "types.h"
#include "scene.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_display.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "system.h"
#include "ram_buffer.h"
#include "input.h"
void UiWork_ClearValueNameTablesFar(void);
s32 BattlePres_ShowDefendMessage(s16 *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

void BattlePres_AdjustCameraByShoulderKeys(void) {
  struct BattleCamera *cam = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *trans = Ram_HeapSlots->transitionWork;
  /* FAKEMATCH(volatile): the keys read through a volatile pointer, so each
   * test reads them afresh; as a plain u32 pointer, 18 of 41 instructions
   * differ in tla-en. */
  volatile u32 *keys = (volatile u32 *)((u32)&gInput);

  if ((*keys & 512) != 0) {
    cam->yaw += 512;
  }
  if ((*keys & 256) != 0) {
    cam->yaw -= 512;
  }
  if (trans->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}
