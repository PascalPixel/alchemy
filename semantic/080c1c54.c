#include "layout_guard.h"
#include "types.h"

struct Owner_080c1c54 {
    u8 unknown_00[0x0f];
    u8 level;
    s16 primary;
    s16 secondary;
    u8 unknown_14[4];
    u16 capacity[3];
    u8 unknown_1e[6];
    s16 resistance[4][2];
};

struct Snapshot_080c1c54 {
    s16 primary;
    s16 secondary;
    u8 unknown_04[4];
    u16 capacity[3];
    u8 unknown_0e[6];
    s16 resistance[4][2];
};

typedef void (*CopySnapshot_080c1c54)(void *, const void *, s32);

LAYOUT_OFFSET_GUARD(
    Owner080c1c54_Primary,
    struct Owner_080c1c54,
    primary,
    0x10);
LAYOUT_OFFSET_GUARD(
    Owner080c1c54_Capacity,
    struct Owner_080c1c54,
    capacity,
    0x18);
LAYOUT_OFFSET_GUARD(
    Owner080c1c54_Resistance,
    struct Owner_080c1c54,
    resistance,
    0x24);
LAYOUT_SIZE_GUARD(Snapshot080c1c54, struct Snapshot_080c1c54, 0x24);

s32 Func_080022ec(s32, s32);
void Func_08002df0(void *);
void *Func_08004970(s32);
struct Owner_080c1c54 *Func_08077008(s32);
void Func_08077010(s32);

static s32 Maximum_080c1c54(s32 value, s32 minimum, s32 maximum)
{
    if (value < minimum)
        value = minimum;
    if (value > maximum)
        value = maximum;
    return value;
}

void Func_080c1c54(s32 owner_id, s32 increase)
{
    CopySnapshot_080c1c54 copy_snapshot =
        (CopySnapshot_080c1c54)0x03001388;
    struct Snapshot_080c1c54 *original = Func_08004970(0x24);
    struct Owner_080c1c54 *owner = Func_08077008(owner_id);
    u32 index;
    s32 value;

    copy_snapshot(original, &owner->primary, 0x24);

    value = owner->primary + Func_080022ec(increase * 97, 10);
    owner->primary = Maximum_080c1c54(
        value,
        Func_080022ec(original->primary * 7, 10),
        9999);

    value = owner->secondary + Func_080022ec(increase * 15, 10);
    owner->secondary = Maximum_080c1c54(
        value,
        Func_080022ec(original->secondary * 7, 10),
        9999);

    value = owner->capacity[0] + Func_080022ec(increase * 123, 10);
    owner->capacity[0] = Maximum_080c1c54(
        value,
        Func_080022ec(original->capacity[0] * 7, 10),
        999);

    value = owner->capacity[1] + Func_080022ec(increase * 33, 10);
    owner->capacity[1] = Maximum_080c1c54(
        value,
        Func_080022ec(original->capacity[1] * 7, 10),
        999);

    value = owner->capacity[2] + Func_080022ec(increase * 51, 10);
    owner->capacity[2] = Maximum_080c1c54(
        value,
        Func_080022ec(original->capacity[2] * 7, 10),
        999);

    for (index = 0; index < 4; index++) {
        value =
            owner->resistance[index][0] + Func_080022ec(increase * 15, 10);
        owner->resistance[index][0] = Maximum_080c1c54(
            value,
            Func_080022ec(original->resistance[index][0] * 7, 10),
            200);
    }

    owner->level += increase;
    Func_08077010(owner_id);
    Func_08002df0(original);
}
