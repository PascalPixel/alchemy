typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_08078980 {
    u8 padding[216];
    u16 entries[15];
};

struct Entry_08078980 {
    u8 padding[3];
    u8 flags;
};

struct State_08078980 *Func_08077394(s32);
struct Entry_08078980 *Func_08078414(s32);

s32 Func_08078980(s32 owner, s32 index) {
    struct State_08078980 *state = Func_08077394(owner);
    s32 id = state->entries[index] & 0x1FF;
    struct Entry_08078980 *description = Func_08078414(id);

    if (id == 0) {
        return -1;
    }
    if ((description->flags & 8) != 0) {
        return -4;
    }
    if ((state->entries[index] & 0x200) != 0 && (description->flags & 2) != 0) {
        return -3;
    }
    return 0;
}
