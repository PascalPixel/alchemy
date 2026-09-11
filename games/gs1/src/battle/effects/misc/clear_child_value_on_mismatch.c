#include "types.h"
#include "scene.h"
#include "abi/battle/effects/misc/clear_child_value_on_mismatch.h"

struct Child_08096ab0 {
    u8 padding[91];
    u8 value;
};

struct State_08096ab0 {
    u8 padding_00[20];
    struct Child_08096ab0 *child;
    u8 padding_18[2];
    s16 value;
    u8 padding_1c[2];
    s16 mode;
};

struct Global_08096ab0 {
    u8 padding[0x24a];
    s16 value;
};

extern struct State_08096ab0 *gIw;
extern struct Global_08096ab0 gCell;

void BattleFx_ClearChildValueOnMismatch(void)
{
    struct State_08096ab0 *state = gIw;

    if (state->mode == 2) {
        Battle_Run();
        if (gCell.value != state->value) {
            state->child->value = 0;
        }
    }
}
