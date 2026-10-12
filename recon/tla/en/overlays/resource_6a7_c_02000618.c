/* Near miss: Func_02000618 differs 14: the party state address is loaded one row late and the owner override
 * store swaps r2 and r3. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "flag_call.h"

struct SceneWork {
    u8 unknown_000[0x1ac];
    s32 unknown_1ac;
};

extern char SceneId_Ov6a7Room1;

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000064(void);
void Func_02000af8(s32 scene, s32 entrance);
void Func_02000b08(void);

s32 Func_02000618(void)
{
    ((struct SceneWork *)Ram_HeapSlots->event_work)->unknown_1ac = 256;
    Object_GetById(gPartyState.current_owner)->priority_flags |= 32;
    if (gPartyState.entrance == 90) {
        GameFlag_SetBit(0x90a);
        gPartyState.owner_override = 1;
        Func_02000af8((s32)&SceneId_Ov6a7Room1, 4);
    }
    Func_02000b08();
    return 0;
}

