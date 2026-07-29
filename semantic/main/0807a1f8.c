typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;

struct OwnerState {
    u8 unknown_000[0xf8];
    u32 available[4];
    u32 used[4];
    u8 active[4];
    u8 counts[4];
};

struct ActiveEntry {
    u8 index;
    u8 bit;
    u8 owner;
    s8 status;
};

struct ActiveState {
    u8 unknown_000[8];
    struct ActiveEntry entries[64];
    s32 count;
};

struct OwnerState *Func_08077394(s32 owner);
struct ActiveState *Func_08077330(s32 group);

s32 Func_0807a1f8(s32 owner, s32 index, s32 bit)
{
    struct OwnerState *owner_state = Func_08077394(owner);
    struct ActiveState *active_state;
    s32 group;
    s32 entry_index;
    u32 mask;

    if (owner_state->active[index] == 0)
        return 0;
    if (owner_state->counts[index] > 9) {
        owner_state->counts[index] = 10;
        return 0;
    }

    mask = 1 << bit;
    if ((owner_state->available[index] & mask) == 0)
        return 0;
    if (owner_state->used[index] & mask)
        return 0;

    group = (u32)owner > 7;
    active_state = Func_08077330(group);
    for (entry_index = 0; entry_index < active_state->count; entry_index++) {
        struct ActiveEntry *entry = &active_state->entries[entry_index];
        if (entry->index == index && entry->bit == bit)
            break;
    }

    if (entry_index == active_state->count)
        return 1;
    if (active_state->entries[entry_index].status <= 0 &&
        active_state->entries[entry_index].status != -2)
        return 1;
    return 0;
}
