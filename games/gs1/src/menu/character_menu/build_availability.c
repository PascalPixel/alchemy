#include "types.h"
#include "menu_result.h"
#include "gs1_edition.h"
#include "scene.h"

/* menu/character_menu/build_availability.c */
u8 *Runtime_GetObject(s32 owner);

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    u8 *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = Runtime_GetObject(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (*(s16 *)(state + 56) == 0 && requested == 1) {
        output[0] = requested;
        count = 1;
    }

    mode = *(s8 *)(state + 0x131);
    if (mode != 0) {
        if (mode == 1)
            output[1] = mode;
        else
            output[2] = 1;
        count++;
    }
    if (*(s8 *)(state + 304) != 0) {
        output[3] = 1;
        count++;
    }
    if (state[320] != 0) {
        output[4] = 1;
        count++;
    }
    return count;
}

/* menu/core/build_page_result.c */
#if defined(GS1_EDITION_JA)
#define GROUP_LEN 6
#else
#define GROUP_LEN 5
#endif

s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)ADDR_03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = (s32)Runtime_GetObject(LoadByte(entries, offset));
    limit = LoadByte(base, 0x218);
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = FixedPoint_Ratio(value, GROUP_LEN);
    remainder = Modulo(value, GROUP_LEN);
    groups = FixedPoint_Ratio(limit, GROUP_LEN);
    if (Modulo(limit, GROUP_LEN) != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}
