#include "types.h"
#include "scene.h"
#include "abi/field/check_configured_keys_and_count.h"


s32 Field_CheckConfiguredKeysAndCount(void *work)
{
    Field_Check();
    FIELD_AT_OFFSET(work, u16 *, 4) = (u16)(FIELD_AT_OFFSET(work, u16 *, 4) + 1);
    return 1;
}
