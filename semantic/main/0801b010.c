#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_0801b36c(void *);
void *Func_080162d4(u32, u32, u32, u32, u32);
void Func_08016418(void *, s32);
void Func_08016478(void *);
void Func_0801e7c0(u32, void *, s32, s32);

/*
 * Ensure that the selection-description window has the required dimensions,
 * present it, and draw the message chosen by the active list entry or type.
 */
void Func_0801b010(u16 type, u32 width)
{
    u8 *state = *(u8 **)0x03001E98;
    u8 *entry = Func_0801b36c(state);
    void *window = M2C_FIELD(state, void **, 0x350);

    if (window == 0) {
        if (type == 6) {
            u32 left = M2C_FIELD(state, u16 *, 0x3B8) != 0
                ? 0x11 : 0;

            window = Func_080162d4(0x11, left, 5, 3, type);
            M2C_FIELD(state, void **, 0x350) = window;
            M2C_FIELD(state, u16 *, 0x3A0) = 0;
            M2C_FIELD(state, u16 *, 0x3B8) = 0x3E7;
        } else {
            window = Func_080162d4(
                ((9 - width) >> 1) + 0x13, 0x11, width + 2, 3, 6);
            M2C_FIELD(state, void **, 0x350) = window;
        }
        Func_08016478(window);
    } else {
        if (width != 0 &&
            M2C_FIELD(window, u16 *, 8) != width + 2) {
            Func_08016418(window, 2);
            window = Func_080162d4(
                ((9 - width) >> 1) + 0x13, 0x11, width + 2, 3, 6);
            M2C_FIELD(state, void **, 0x350) = window;
        }
        Func_08016478(M2C_FIELD(state, void **, 0x350));
    }

    if (M2C_FIELD(state, u16 *, 0x394) != 0) {
        Func_0801e7c0(
            M2C_FIELD(entry, u16 *, 0x20),
            M2C_FIELD(state, void **, 0x350), 0, 0);
    } else {
        switch (type) {
        case 2:
            Func_0801e7c0(
                0x50, M2C_FIELD(state, void **, 0x350), 0, 0);
            break;
        case 4:
            Func_0801e7c0(
                0x51, M2C_FIELD(state, void **, 0x350), 0, 0);
            break;
        }
    }
}
