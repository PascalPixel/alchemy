#include "scene.h"

void Func_02002010(void) {
  struct FieldActor *leader = Object_GetById(gAdventure.currentOwner);
  s32 x;
  s32 y;
  s32 z;

  y = leader->y;
  asm volatile(
      ""); /* FAKEMATCH: ⚓️ reads the height before it builds 0x100000 */
  if (y > 0x100000)
    gAdventure.unk2c3 = 1;
  else
    gAdventure.unk2c3 = 0;
  if (gAdventure.scene == (s32)&SceneId_Ov661Room3) {
    ObjectMotion_SetActionVariant(18, leader->sprite->priority);
    x = leader->x;
    asm volatile(""); /* FAKEMATCH: ⚓️ reads x before it builds 0x1d80000 */
    if (x > 0x1d80000) {
      z = leader->z;
      asm volatile(""); /* FAKEMATCH: ⚓️ reads z before it builds 0x1e80000 */
      if (z <= 0x1e80000)
        return;
      if (leader->y > (s32)0xfff00000)
        gAdventure.unk2c3 = 1;
      else
        gAdventure.unk2c3 = 0;
    }
  }
}
