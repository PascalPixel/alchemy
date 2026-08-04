#include "types.h"

struct State {
    u8 unknown_000[0xD8];
    u16 entries[15];
};

struct Description {
    u8 unknown_00[2];
    u8 category;
    u8 flags;
};

struct State *Func_08077394(s32 owner);
s32 Func_0807842c(s32 owner, s32 entry);
struct Description *Func_08078414(s32 entry);
void Func_08078bf0(s32 owner);
void Func_08077428(s32 owner);

s32 Func_08078708(s32 owner, s32 index)
{
    struct State *state = Func_08077394(owner);
    unsigned int mask;
    unsigned int entry = state->entries[index];
    struct Description *description;
    u8 category;
    s32 other;

    if (Func_0807842c(owner, entry) == 0)
        return -1;
    mask = 0x200;
    if (entry & mask)
        return 0;

    description = (u8 *) Func_08078414(entry);
    category = description->category;
    if (category != 6) {
        for (other = 0, entry = 0xD8; other <= 14; entry += 2, other++) {
            unsigned int checkMask = mask;
            unsigned int bits = *(u16 *)(entry + (unsigned int)state);

            bits &= checkMask;
            if (bits == 0)
                continue;
            if (Func_08078414(*(volatile u16 *)(entry + (unsigned int)state))->category == category)
                break;
        }

        if (other != 15) {
            description = Func_08078414(state->entries[other]);
            if (description->flags & 2)
                return -2;
            state->entries[other] &= 0xFDFF;
        }
    }

    state->entries[index] |= 0x200;
    Func_08078bf0(owner);
    Func_08077428(owner);
    return 0;
}
