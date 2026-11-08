/// @file
/// Scatters count sparks (effect type 279) around a position: each starts at
/// the given scale, shrinks toward 0xcccc, turns to a random sixteenth of a
/// turn and flies off with a random velocity through the overlay's effect
/// spawner, falling under Func_020006d4.

#include "tundaria_tower.h"

void Func_02000708(struct TundariaTowerPosition *position, s32 count,
                   s32 scale) {
  struct EffectOptions options;
  s32 i;
  s32 velocityX;
  s32 velocityY;
  s32 x;

  options.priority = 1;
  options.type = 279;
  options.targetScaleY = 0xcccc;
  options.targetScaleX = 0xcccc;
  options.startScaleY = scale;
  options.startScaleX = scale;
  options.update = Func_020006d4;
  for (i = 0; i != count; i++) {
    s32 mask;

    options.rotation = (Random16Far() & 15) << 12;
    velocityX = Random16Far();
    // One constant for the loop's life: the listing keeps 15 in a register.
    mask = 15;
    velocityX &= mask;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16Far() & mask;
    velocityY += 8;
    velocityY <<= 14;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    // The overlay's spawner is declared with this scene's view of the options.
    Func_020000b8(x, position->y + ((Random16Far() & mask) << 16), position->z,
                  velocityX, velocityY, 0, 0x15e0000,
                  (struct TundariaTowerEffectParams *)&options);
  }
}
