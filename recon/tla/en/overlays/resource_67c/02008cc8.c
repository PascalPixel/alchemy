// Draft of Func_02000cc8 (overlay 67C, 284 bytes), differ 66 of 120. The two
// room tests and the slide loop are right; left: the native takes the address
// of the position on every pass (mov r1, sp) where this form keeps it in r6,
// which moves the id from r6 to r7, and it builds each clamp bound twice
// (cmp r3, r1; ble; movs r3, #128; lsls r3, r3, #9) where this form copies the
// bound it compared with. A for loop with breaks is differ 91.
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"
#include "scene_ids.h"

void Func_02000ca8(s32 id);
s32 Func_02003d2c(struct Actor *object, s32 *position);

void Func_02000cc8(s32 id) {
  s32 slide = 1;

  if (gAdventure.scene == (s32)&SceneId_Ov67cRoom5) {
    s32 column = Actor_GetById(8)->x >> 20;

    if (id == 77 && column == 17)
      slide = 0;
    if (id == 78 && column == 19)
      slide = 0;
  }
  /* FAKEMATCH: the native reads the scene again here; without the barrier the
   * read is moved up both paths. */
  __asm__("" : : : "memory");
  if (gAdventure.scene == (s32)&SceneId_Ov67cRoom6) {
    if (id == 77 && Actor_GetById(10)->x >> 20 == 9)
      slide = 0;
    if (id == 78 && Actor_GetById(11)->x >> 20 == 13)
      slide = 0;
  }
  if (slide) {
    struct Actor *object = Actor_GetById(gAdventure.currentOwner);
    s32 position[3];
    s32 result;

    Func_02003d7c();
    Func_02003eac(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop
     * notes: the bytes take the position address on every pass, and a for loop
     * hoists it. */
    goto test;
  body:
    if (result < 0) {
      object->angle = 0x4000;
      Func_02000ca8(id);
      return;
    }
    object->z += 0x20000;
    {
      s32 x = object->x;
      s32 offset = (x & 0xfffff) - 0x80000;

      if (offset > 0x10000)
        offset = 0x10000;
      if (offset < -0x10000)
        offset = -0x10000;
      object->x = x - offset;
    }
    object->angle += 0x1000;
    wait(1);
  test:
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z + 0x80000;
    result = Func_02003d2c(object, position);
    if (result <= 0)
      goto body;
  }
}
