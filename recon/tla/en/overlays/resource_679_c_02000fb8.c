/* Draft of Func_02000fb8 (overlay 679, 61 instructions), differ 12: the overlay's
 * entry, written from the listing (scene ids 0x9a to 0x9c are the linker symbols
 * SceneId_Ov679Room1 to Room3). Scheduling only: the native runs its statements
 * more nearly in order: it loads the event work ([r0, #108]) before it starts 428,
 * stores 0x202 before it starts 480 for the scene's address, and stores 0x100
 * before it starts 482; here each constant is started first (sched2: equal
 * priority, and the constant into a free register has no dependence on the
 * store just scheduled).
 * Measured: the map byte as a mask (& ~12: movs #243, differ 15); the event work
 * through a struct instead of an s32 pointer (no change). */
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "scene_ids.h"

struct MapFlags {
    u8 unknown_00[23];
    u8 low : 2;
    u8 mode : 2;
    u8 high : 4;
};

s32 GameFlag_Test(s32 flag);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_02000540(void);
void Func_02000974(void);
void Func_02001054(void);
void Func_020010b4(void);

s32 Func_02000fb8(void)
{
    s32 *transition;
    s16 scene;

    ((struct MapFlags *)Ram_HeapSlots->map_work)->mode = 1;
    transition = (s32 *)((u8 *)Ram_HeapSlots->event_work + 428);
    *transition = 0x202;
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Ov679Room2) {
        Func_02001054();
    } else if (scene == (s32)&SceneId_Ov679Room1) {
        Scheduler_AddOrUpdateCallback(Func_02000540, 0x480);
        if (GameFlag_Test(0x95e))
            Func_02000974();
    } else if (scene == (s32)&SceneId_Ov679Room3) {
        *transition = 0x100;
        if (gPartyState.entrance == 6)
            Func_020010b4();
    }
    return 0;
}
