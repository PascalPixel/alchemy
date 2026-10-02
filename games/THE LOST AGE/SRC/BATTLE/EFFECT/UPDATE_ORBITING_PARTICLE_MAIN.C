#include "TYPES.H"
#include "OBJDISP.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"

struct OrbitingParticle;

void BattleFx_UpdateOrbitingParticleFade(void *object)
{
  s32 primary_fade;
  u8 *object_bytes;
  if (object != ((void *) 0))
  {
    primary_fade = *((s32 *)(((u8 *)object) + 0x18));
    primary_fade = primary_fade + 0xFFFFF000;
    *((s32 *)((object_bytes = (u8 *)object) + 0x1C)) = (s32)((*((s32 *)(object_bytes + 0x1C))) + 0xFFFFF000);
    *((s32 *)(object_bytes + 0x18)) = primary_fade;
    if (primary_fade <= 0x1000)
    {
      ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }
  }
}
