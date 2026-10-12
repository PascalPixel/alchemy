#include "types.h"
#include "ram_buffer.h"

struct InnWork {
    u8 unknown_000[0x4ca];
    s16 members[29];
    s8 member_count;
};

void *UnitGet(s32 unit_id);
extern const s8 Data_0810cf2c[];

s32 Func_0810b378(s32 mode)
{
    struct InnWork *work = Ram_HeapSlots->menu_runtime;
    s32 active = 0;
    s32 factor = Data_0810cf2c[mode];
    s32 index;

    for (index = 0; index < work->member_count; index++) {
        if (*(s16 *)((u8 *)UnitGet(work->members[index]) + 56) != 0)
            active++;
    }

    return factor * active;
}
