#include "TYPES.H"
#include "SCENE.H"

s32 Field_CheckConfiguredKeysAndCount(void *work)
{
    Field_Check();
    FIELD_AT_OFFSET(work, u16 *, 4) = (u16)(FIELD_AT_OFFSET(work, u16 *, 4) + 1);
    return 1;
}
