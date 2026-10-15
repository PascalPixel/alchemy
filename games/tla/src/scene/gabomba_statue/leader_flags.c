#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "mem.h"
#include "resource.h"

extern u8             gSceneState[];
extern const u8       gField_CachedTilesA[];
struct ObjectRuntime *Object_GetById(s32 id);
void                  GameFlag_SetBit(s32 flag);
void                  GameFlag_ClearBit(s32 flag);
void                  Func_020055cc(const u8 *data, void *buffer);

void Func_02003868(void)
{
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    if (gPartyState.entrance == 1)
    {
        GameFlag_SetBit(0xa27);
        GameFlag_ClearBit(0xa28);
    }
}
