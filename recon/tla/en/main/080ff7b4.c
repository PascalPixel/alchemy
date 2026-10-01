#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
extern u8 Data_03001f2c[];

/* menu/character_menu/build_availability.c */
u8 *Owner_GetStateFar(s32 owner);

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)((u32)&Data_03001f2c);
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = (s32)Owner_GetStateFar(LoadByte(entries, offset));
    limit = LoadByte(base, 0x218);
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = __divsi3(value, GROUP_LEN);
    remainder = __modsi3(value, GROUP_LEN);
    groups = __divsi3(limit, GROUP_LEN);
    if (__modsi3(limit, GROUP_LEN) != 0) {
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
