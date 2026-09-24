#include "OBJECT_FACTORY.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Menu_CreateEightEntryObjects(s32 resource)
{
    void **slot;
    void *obj;
    s32 i;
    u8 *state;
    s32 param;

    state = *(u8 **)ADDR_03001F2C;
    i = 0;
    param = 0xA8;
    slot = (void **)(state + 0xC8);
    do {
        obj = RenderOutput_CreateFromResourceFar(2, i, resource, 0xF8, param);
        i += 1;
        *slot = obj;
        slot += 1;
    } while (i <= 7);
    return 1;
}
