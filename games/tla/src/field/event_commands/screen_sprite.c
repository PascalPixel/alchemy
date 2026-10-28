#include "types.h"

struct ScreenSprite {
  u8 unk00[0xc];
  s32 x;
  s32 y;
  u8 unk14[0x14];
};

void Camera_WorldToScreen(s32 *position);
void Func_080eb01c(struct ScreenSprite *sprite);

/// Places sprite at the screen position of world position and draws it.
void EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite, s32 *position) {
  s32 screen[3];

  if (sprite != 0) {
    s32 *p = screen;
    s32 *arg;

    p[0] = position[0];
    arg = p;
    /* FAKEMATCH: ⚓️ sets the argument before it copies the second word */
    asm("" : "+r"(arg) : : "memory");
    p[1] = position[1];
    p[2] = position[2];
    Camera_WorldToScreen(arg);
    sprite->x = screen[0];
    sprite->y = screen[2];
    Func_080eb01c(sprite);
  }
}
