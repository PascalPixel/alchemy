typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08077150(u16);
u8 Func_08077158(u16 *);
void Func_08077168(u16);

/*
 * Move one entry by a single position in the active identifier list. Rebuild
 * the registry in the new order, compact the stored list, and publish its
 * resulting count.
 */
s32 Func_080a7f44(s32 selected, s32 move_down) {
    u8 *state;
    u16 *identifiers;
    u16 reordered[14];
    u8 count;
    s32 adjacent;
    s32 index;
    u16 swap;

    state = *(u8 **)0x03001F2C;
    identifiers = (u16 *)(state + 0x208);
    count = state[0x219];
    if (count <= 1) {
        return 0;
    }
    if (move_down == 1) {
        if (selected == count - 1) {
            return 0;
        }
        adjacent = selected + 1;
    } else {
        if (selected == 0) {
            return 0;
        }
        adjacent = selected - 1;
    }

    for (index = 0; index < 14; index++) {
        reordered[index] = 0;
    }
    for (index = 0; index < count; index++) {
        reordered[index] = identifiers[index];
    }

    swap = reordered[selected];
    reordered[selected] = reordered[adjacent];
    reordered[adjacent] = swap;

    for (index = 0; index < count; index++) {
        Func_08077168(identifiers[index]);
    }
    for (index = 0; index < count; index++) {
        Func_08077150(reordered[index]);
    }
    state[0x219] = Func_08077158(identifiers);
    return 1;
}
