#include "owner_state.h"
#include "types.h"

struct Owner_080792c4 {
    u8 unknown_000[0x0f];
    u8 level;
    u8 unknown_010[0x114];
    u32 value_124;
};

u32 Func_08079008(s32 owner, s32 level);
s32 Func_0807905c(s32, s32);

s32 Func_080792c4(s32 owner_no, s32 value)
{
    struct Owner_080792c4 *owner;

    owner = Func_08077394(owner_no);
    if ((owner->value_124 >= Func_08079008(owner_no, owner->level + 1)) &&
        (Func_0807905c(owner_no, value) != 0)) {
        return value;
    }
    return 0;
}
