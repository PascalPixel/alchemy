/* Near miss, differ 2 of 104: the same opening as Func_02001110. The native
 * loads gPartyState's address (ldr r1) right after movs r3, #192, before the
 * shift that finishes Ram_HeapSlots; in sched2 the shift and the load tie at
 * priority 6 and the shift wins on its three dependents (the event and effect
 * work loads and the block end) to the load's two. Measured: the party
 * pointer declared first (its load then comes first in insn order, still
 * differ 2), pinned to r1 (26), held by a "+l" barrier (2: the asm's free
 * link lowers the load's priority), the effect work read first (23), alchemy
 * permute for 4 minutes (2). The three single calls go through Call6, as
 * copy.c's do; the first two share 78 in r5 as direct calls. */
#include "../../../../games/tla/src/scene/magma_rock/magma_rock.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "object_runtime.h"
#include "call.h"

extern s16 Data_02003d98[];
extern s16 Data_02003dac[];
extern s16 Data_02003dd2[];
extern s16 Data_02003de4[];
extern s16 Data_02003de8[];

void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c14(s16 *table);
void Func_0200074c(struct ObjectRuntime *object);

void Func_02000940(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct MagmaRockWork *work = Ram_HeapSlots->effect_work;

    if (gPartyState.scene == (s32)&SceneId_Ov691Room1) {
        Func_02003aa4(98, 18, 4, 3, 78, 21);
        Func_02003aa4(98, 24, 4, 3, 78, 24);
        Func_02003c14(Data_02003d98);
    } else if (gPartyState.scene == (s32)&SceneId_Ov691Room2) {
        if (gPartyState.entrance <= 3) {
            Func_02003c14(Data_02003dac);
        } else if (gPartyState.entrance <= 7) {
            Func_02003c14(Data_02003dd2);
        } else if (gPartyState.entrance <= 11) {
            Call6((void (*)())Func_02003aa4, 79, 46, 2, 1, 79, 36);
            Func_02003c14(Data_02003de4);
        } else {
            if (((s16 *)&work->target->z)[1] == 600)
                Call6((void (*)())Func_02003aa4, 104, 38, 1, 1, 103, 38);
            else
                Call6((void (*)())Func_02003aa4, 94, 44, 5, 1, 99, 38);
            Func_02003c14(Data_02003de8);
        }
    }
    Func_0200074c(*(struct ObjectRuntime **)(event + 0xcbc));
}
