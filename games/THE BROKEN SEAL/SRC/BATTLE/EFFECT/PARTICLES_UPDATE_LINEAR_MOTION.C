#include "TYPES.H"
#include "FIXED_MATH.H"

void ParticleEffect_UpdateLinearMotion(void *particle)
{
  s32 velocity_x;
  s32 x;
  s32 velocity_z;
  s32 z;
  s32 velocity_y;
  s32 vz;
  velocity_x = *((s32 *)(((u8 *)particle) + 0x30));
  x = (*((s32 *)(((u8 *)particle) + 8))) + velocity_x;
  *((s32 *)(((u8 *)particle) + 8)) = x;
  *((s32 *)(((u8 *)particle) + 0x38)) = x;
  velocity_z = *((s32 *)(((u8 *)particle) + 0x34));
  z = (*((s32 *)(((u8 *)particle) + 0x10))) + velocity_z;
  *((s32 *)(((u8 *)particle) + 0x10)) = z;
  *((s32 *)(((u8 *)particle) + 0x40)) = z;
  velocity_y = (*((s32 *)(((u8 *)particle) + 0xC))) + 0x400;
  *((s32 *)(((u8 *)particle) + 0xC)) = velocity_y;
  *((s32 *)(((u8 *)particle) + 0x3C)) = velocity_y;
  *((s32 *)(((u8 *)particle) + 0x30)) =
      (s32)(velocity_x - Math_Div(velocity_x, 0x12));
 do {
   vz = velocity_z;
   if (velocity_z < 0) {
     vz += 0xF;
   }
   *((s32 *)(((u8 *)particle) + 0x34)) =
       (s32)(velocity_z - (vz >> 4));
 } while (0);
}
