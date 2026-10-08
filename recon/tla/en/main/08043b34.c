#include "types.h"
#include "scene.h"
extern struct InnState *Data_03001f2c;

void Scheduler_RemoveCallback(s32);
void Menu_UpdateFirstObjectRowPositions(void);
void ResourceObject_ReleaseFar(void *);

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    u8 *base = (u8 *)Data_03001f2c;
    s32 offset = 138;
    s32 zero;
    s32 count;

    Scheduler_RemoveCallback((s32)Menu_UpdateFirstObjectRowPositions);
    zero = 0;
    offset *= 2;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            ResourceObject_ReleaseFar(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}
