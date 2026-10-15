/* Near miss, differ 2 of 62: one reordered pair at the start. The native
 * loads gPartyState's address (ldr r1) right after it starts Ram_HeapSlots
 * (movs r3, #192), ahead of the copy of the mode (adds r6, r0); here the
 * copy, the gPartyState load and the next shift tie at priority 6 in sched2
 * and the copy goes first by insn order. Measured: target as one function
 * local (differ 26: it is allocated r2), the table chosen by an inline
 * (43), a party pointer, the default table set first, the scene read into
 * a local (18), s16/u8/u16/s8 modes (9 to 37), alchemy permute (4
 * minutes, differ 17 at best). */
#include "../../../../games/tla/src/scene/magma_rock/magma_rock.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "object_runtime.h"

extern s16 Data_02003d9e[];
extern s16 Data_02003db2[];
extern s16 Data_02003dc4[];
extern s16 Data_02003dd6[];
extern s16 Data_02003dee[];
extern s16 Data_02003e02[];

s32 Func_02001084(s16 *table);
void Func_02000fcc(struct ObjectRuntime *object);

void Func_02001110(s32 mode)
{
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
    s16 *table = Data_02003d9e;

    if (gPartyState.scene == (s32)&SceneId_Ov691Room2) {
        if (gPartyState.entrance <= 3)
            table = Data_02003db2;
        else if (gPartyState.entrance <= 8)
            table = Data_02003dc4;
        else if (gPartyState.entrance <= 12)
            table = Data_02003dd6;
        else
            table = Data_02003dee;
    } else if (gPartyState.scene == (s32)&SceneId_Ov691Room3) {
        table = Data_02003e02;
    }
    if (mode == 2) {
        struct ObjectRuntime *target;

        Func_02001084(table);
        target = work->target;
        ((u8 *)target)[99] = 4;
        target->update = (void (*)(void))Func_02000fcc;
    }
    if (mode == 3) {
        struct ObjectRuntime *target;

        Func_02001084(table);
        target = work->target;
        ((u8 *)target)[99] = 2;
        target->update = (void (*)(void))Func_02000fcc;
    }
}
