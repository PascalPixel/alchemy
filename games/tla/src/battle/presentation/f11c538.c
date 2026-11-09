#include "types.h"
#include "scene.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_display.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_side.h"
#include "system.h"
#include "ram_buffer.h"
#include "input.h"
void MessageArgument_ClearAllFar(void);
s32 AutoBattle_ShowDefend(s16 *);
s32 BattleStaging_RunUnitAction(s16 *);
s32 BattleStaging_RunPairedUnitTransition(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

void BattleCamera_AdjustByShoulderKeys(void) {
  struct BattleCamera *cam = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *trans = Ram_HeapSlots->transitionWork;

  if ((gInput.held & 512) != 0) {
    cam->yaw += 512;
  }
  // The held keys reread through a volatile pointer, a load apart from the L
  // test's; as gInput.held, 18 of 41 instructions differ in tla-en.
  if ((*(volatile u32 *)&gInput.held & 256) != 0) {
    cam->yaw -= 512;
  }
  if (trans->flag == 0) {
    BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
  }
}
