#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"
#include "RESOURCE.H"
#include "CALLBACK_SCHEDULER.H"

struct DepthScene {
    u16 mode;
    u8 unknown_02[2];
    s32 x;
    u8 unknown_08[8];
    struct ObjectRuntime *main_object;
    u8 unknown_14[0x7dc];
    s16 vram_slot;
    s16 active;
};

void Func_080dba44(void);

void Func_080dbb40(void)
{
    struct DepthScene *scene = Ram_HeapSlots->effect_work;

    /* FAKEMATCH: ⚓️ loads the scene before it builds the flag offset */
    asm volatile("");
    if (scene->active != 0) {
        scene->active = 0;
        Scheduler_RemoveCallback(Func_080dba44);
        Resource_ResetEntry(scene->vram_slot);
    }
}
