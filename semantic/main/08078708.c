typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

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
    u16 entry = state->entries[index];
    struct Description *description;
    u8 category;
    s32 other;

    if (Func_0807842c(owner, entry) == 0)
        return -1;
    if (entry & 0x200)
        return 0;

    description = Func_08078414(entry);
    category = description->category;
    if (category != 6) {
        for (other = 0; other <= 14; other++) {
            u16 other_entry = state->entries[other];

            if (!(other_entry & 0x200))
                continue;
            if (Func_08078414(other_entry)->category == category)
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
