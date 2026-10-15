#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "mem.h"
#include "iwram_call.h"
#include "resource.h"
#include "callback_scheduler.h"

struct DepthScene {
    u16 mode;
    u8 unk02[2];
    s32 x;
    u8 unk08[8];
    struct ObjectRuntime *mainObject;
    u8 unk14[0x7dc];
    s16 vramSlot;
    s16 active;
};

void Func_080dba44(void);

void Func_080dbb40(void)
{
    struct DepthScene *scene = Ram_HeapSlots->effectWork;

    /* FAKEMATCH: ⚓️ loads the scene before it builds the flag offset */
    asm volatile("");
    if (scene->active != 0) {
        scene->active = 0;
        Scheduler_RemoveCallback(Func_080dba44);
        Resource_ResetEntry(scene->vramSlot);
    }
}
