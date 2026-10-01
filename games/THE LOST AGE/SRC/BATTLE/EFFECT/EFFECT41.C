#include "TYPES.H"
#include "OBJECT_EFX.H"

u32 Random16(void);
void Object_SetCallback(void *, void *);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleFx_UpdateDriftingFallObject(void *obj)
{
    s32 r;

    FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0xC) + 0xFFFFB334);
    r = Random16();
    FIELD_AT_OFFSET(obj, s32 *, 8) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 8) + (r - Random16()));
    if ((s32)FIELD_AT_OFFSET(obj, s32 *, 0xC) <= (s32)FIELD_AT_OFFSET(obj, s32 *, 0x14)) {
        Object_SetCallback(obj, BattleFx_CommonParticleScript);
    }
}
