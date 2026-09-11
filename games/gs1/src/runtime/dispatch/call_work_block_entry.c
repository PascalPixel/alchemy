#include "types.h"
#include "scene.h"

#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

typedef s32 (*WorkEntryFn)(void);

void Runtime_CallWorkBlockEntry(void)
{
    s32 base = 0x02008000;
    ((WorkEntryFn)FIELD_AT_OFFSET((void *)base, s32 *, 4))();
}
