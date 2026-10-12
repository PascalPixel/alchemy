#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

u8 *ResourceMetadata_RegisterFar(void *resource, s32 id);

void Func_080d5fd4(void)
{
    struct ObjectRuntime *object = ObjectTable_Get(gPartyState.current_owner);
    u8 *metadata = ResourceMetadata_RegisterFar(*(void **)((u8 *)object + 0x50), 268);

    metadata[5] = 15;
}
