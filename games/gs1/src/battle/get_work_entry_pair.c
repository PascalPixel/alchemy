#include "types.h"

struct Entry_080b7eb4 {
    s32 first;
    s32 second;
    u8 filler8[36];
};

struct State_080b7eb4 {
    u8 filler0[0x80];
    struct Entry_080b7eb4 entries[1];
};

struct Output_080b7eb4 {
    s32 first;
    s32 middle;
    s32 second;
};

extern struct State_080b7eb4 * volatile Data_03001e74;

s32 Battle_GetWorkEntryPair(s32 no, struct Output_080b7eb4 *out)
{
    struct State_080b7eb4 *state = Data_03001e74;

    out->first = state->entries[no].first;
    out->middle = 0;
    out->second = state->entries[no].second;
    return 0;
}
