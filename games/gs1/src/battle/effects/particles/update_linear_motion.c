#include "types.h"

s32 FixedPoint_Ratio(s32, s32);
void ParticleEffect_UpdateLinearMotion(void *particle)
{
  s32 velocity_x;
  s32 x;
  s32 velocity_z;
  s32 z;
  s32 velocity_y;
  s32 adjusted_velocity_z;
  velocity_x = *((s32 *) (((u8 *) particle) + 0x30));
  x = (*((s32 *) (((u8 *) particle) + 8))) + velocity_x;
  *((s32 *) (((u8 *) particle) + 8)) = x;
  *((s32 *) (((u8 *) particle) + 0x38)) = x;
  velocity_z = *((s32 *) (((u8 *) particle) + 0x34));
  z = (*((s32 *) (((u8 *) particle) + 0x10))) + velocity_z;
  *((s32 *) (((u8 *) particle) + 0x10)) = z;
  *((s32 *) (((u8 *) particle) + 0x40)) = z;
  velocity_y = (*((s32 *) (((u8 *) particle) + 0xC))) + 0x400;
  *((s32 *) (((u8 *) particle) + 0xC)) = velocity_y;
  *((s32 *) (((u8 *) particle) + 0x3C)) = velocity_y;
  *((s32 *) (((u8 *) particle) + 0x30)) =
      (s32)(velocity_x - FixedPoint_Ratio(velocity_x, 0x12));
 do {
   adjusted_velocity_z = velocity_z;
   if (velocity_z < 0) {
     adjusted_velocity_z += 0xF;
   }
   *((s32 *) (((u8 *) particle) + 0x34)) =
       (s32)(velocity_z - (adjusted_velocity_z >> 4));
 } while (0);
}
