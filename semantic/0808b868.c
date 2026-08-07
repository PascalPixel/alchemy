#include "types.h"

struct Entry_0808b868 {
    s16 id;
    s16 event;
    u8 padding04[4];
    s32 x;
    u8 padding0c[4];
    s32 y;
    u8 padding14[4];
};

extern u8 *Data_03001e70;
s32 Func_080770d0(s32);
s32 Func_080770c8(s32);

void Func_0808b868(struct Entry_0808b868 *entry) {
    u8 *state;
    s32 *minX;
    s32 *bounds;
    s32 inactive;

    state = Data_03001e70;
    Func_080770d0(0x164);
    Func_080770c8(0x165);

    if (entry->id == -1)
        return;

    minX = (s32 *)state;
    inactive = 0x165;
    do {
        bounds = minX + 59;
        if (entry->event == 0) {
                s32 x = entry->x;
                s32 y = entry->y;
                if (*bounds > x) {
                    entry->event = inactive;
                } else {
                    s32 maxX = *(s32 *)(state + 244);
                    if (x > maxX) {
                        entry->event = inactive;
                    } else {
                        s32 minY = *(s32 *)(state + 240);
                        if (minY > y) {
                            entry->event = inactive;
                        } else {
                            s32 maxY = *(s32 *)(state + 248);
                            if (y <= maxY)
                                entry->event = 0x164;
                            else
                                entry->event = inactive;
                        }
                    }
                }
        }
        entry++;
    } while (entry->id != -1);
}
