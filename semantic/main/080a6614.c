#include "layout_guard.h"
#include "types.h"

typedef struct SharedState_080a6614 {
    u8 padding_000[0x220];
    u16 first_value_220;
    u16 second_value_222;
} SharedState_080a6614;

LAYOUT_OFFSET_GUARD(
    SharedState080a6614_FirstValue,
    SharedState_080a6614,
    first_value_220,
    0x220);
LAYOUT_OFFSET_GUARD(
    SharedState080a6614_SecondValue,
    SharedState_080a6614,
    second_value_222,
    0x222);

extern SharedState_080a6614 Data_02000240;

void Func_08015080(s32, void *, s32, s32);
void Func_08015090(void *, void *, s32, s32);
void Func_080150b8(s32);
s32 Func_08015108(s32, s32 *, s32 *, s32 *, s32 *);
void Func_08015120(u32, u32);
void *Func_08077008(s32);

/*
 * Render the two packed shared values and, when their labels are narrow
 * enough, add the associated owner portrait beside each row.
 */
s32 Func_080a6614(void *window)
{
    volatile SharedState_080a6614 *state = &Data_02000240;
    u16 first;
    u16 second;
    s32 output20;
    s32 output16;
    s32 output12;
    s32 output8;
    s32 wide;

    if (state->first_value_220 != 0 &&
        state->second_value_222 != 0)
        Func_08015080(0x0ae4, window, 0, -8);
    else
        Func_08015080(0x0ae0, window, 0, -8);

    first = state->first_value_220;
    Func_08015108(
        (first & 0x03ff) + 0x0333,
        &output20,
        &output16,
        &output12,
        &output8);
    wide = output12 > 10;

    first = state->first_value_220;
    if (first != 0) {
        Func_08015120(first & 0x03ff, 4);
        Func_08015080(0x0ae7, window, 0, 0);
        if (!wide) {
            void *owner = Func_08077008(first >> 10);

            Func_08015090(owner, window, 80, 0);
        }
    } else {
        Func_08015080(0x0ae5, window, 0, 0);
    }

    second = state->second_value_222;
    Func_08015108(
        (second & 0x03ff) + 0x0333,
        &output20,
        &output16,
        &output12,
        &output8);
    wide = output12 > 10;

    second = state->second_value_222;
    if (second != 0) {
        Func_08015120(second & 0x03ff, 4);
        Func_08015080(0x0ae8, window, 0, 8);
        if (!wide) {
            void *owner = Func_08077008(second >> 10);

            Func_08015090(owner, window, 80, 8);
        }
        Func_080150b8(15);
    } else {
        Func_08015080(0x0ae6, window, 0, 8);
    }

    return 1;
}
