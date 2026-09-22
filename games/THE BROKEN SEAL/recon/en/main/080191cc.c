#include "types.h"

#define Function Func_080191cc

s32 Func_08002304();
void Func_08003dec();
s32 Func_08003fa4();
void Func_08003d28();
s32 Func_08002322();
s32 Func_0800231c();
s32 Func_08004458();
void Func_0801908c();

static u8 *g_state(void)
{
    return (u8 *)(*(u32 *)0x03001e8c);
}

static u32 g_clock(void)
{
    return *(u32 *)0x03001800;
}

void Function(s32 unused)
{
    u8 *state = g_state();
    u8 *slot = state + 0x500;
    u32 slot_index;

    for (slot_index = 0; slot_index != 8; ++slot_index, slot += 36) {
        u8 *item;
        u8 *entry;
        u32 kind;
        u32 clock_slot;

        if ((*(u16 *)(slot + 22) & 1) == 0) {
            continue;
        }
        item = *(u8 **)slot;
        while (item != 0) {
            entry = item + 16;
            if (*(u16 *)(slot + 18) == 4) {
                *(u16 *)(item + 12) = 2;
                item[5] = 8;
            }
            kind = (u32)(item[5] - 2);
            clock_slot = (g_clock() >> 2) & 7;
            switch (kind) {
            case 0:
                if (*(u16 *)(state + 0x12b6) != 0x60) {
                    u32 loaded = Func_08003fa4(*(u16 *)(state + 0x12b6), 0x80,
                                               (clock_slot << 7) + 0x080368d4);
                    entry[8] = (u8)((entry[8] & 0xfc) | (loaded & 0x3ff));
                    *(u16 *)(entry + 8) = (*(u16 *)(entry + 8) & 0xfc00) | (loaded & 0x3ff);
                    item[14] = entry[8];
                    item[21] = (u8)(((item[21] & 0xf3) | 0x20) & 0x3f) | 0x80;
                    entry[7] &= 0x3f;
                    entry[4] = (u8)(item[8] + ((const u8 *)0x08033e60)[Func_08002304(*(u32 *)0x03001800, 80)] + 2);
                    item[21] &= 0xfc;
                    entry[7] &= 0xc1;
                }
                break;
            case 2:
                if (*(u32 *)0x03001800 & 1) {
                    s32 a = Func_08004458();
                    s32 b = Func_08004458();
                    s32 delta = ((((a * 3) >> 16) + ((b * 3) >> 16)) >> 1);
                    *(u16 *)(entry + 6) = (*(u16 *)(entry + 6) & 0xfe00) |
                                           ((*(u16 *)(item + 6) + delta - 1) & 0x1ff);
                    a = Func_08004458();
                    b = Func_08004458();
                    delta = ((((a * 3) >> 16) + ((b * 3) >> 16)) >> 1);
                    entry[4] = (u8)(item[8] + delta - 1);
                }
                break;
            case 3: {
                u16 scratch[3];
                if (*(u16 *)(item + 12) == 0) {
                    break;
                }
                scratch[0] = (u16)(*(u32 *)(state + 0x10) & 0x3fff) | 0x200;
                scratch[1] = (u16)(*(u32 *)(state + 0x10) & 0xffff);
                scratch[2] = (u16)(*(u32 *)(state + 0x10) & 0xffff);
                Func_08003d28(scratch);
                entry[7] = (u8)((entry[7] & 0xc0) | ((scratch[0] & 31) << 1));
                item[5] |= 3;
                *(u16 *)(entry + 6) = (*(u16 *)(entry + 6) & 0xfe00) |
                                       ((*(u16 *)(item + 6) + 0x1ff) & 0x1ff);
                entry[4] = (u8)(item[8] - 5);
                break;
            }
            case 4:
                if (*(u16 *)(item + 12) != 0) {
                    *(u16 *)(item + 12) += 0x80;
                    entry[7] = (u8)((entry[7] & 0xc0) | ((Func_08002304(*(u16 *)(item + 12), 0x50) & 31) << 1));
                    item[5] |= 3;
                    *(u16 *)(entry + 6) = (*(u16 *)(entry + 6) & 0xfe00) |
                                           ((*(u16 *)(item + 6) + 0x1ff) & 0x1ff);
                    entry[4] = (u8)(item[8] - 8);
                }
                break;
            case 5:
                *(u16 *)(entry + 6) = (*(u16 *)(entry + 6) & 0xfe00) |
                                       ((*(u16 *)(item + 6) + 0x100) & 0x1ff);
                entry[4] = (u8)(item[8] - 8);
                break;
            case 6:
                if (*(u16 *)(item + 12) != 0) {
                    ++*(u16 *)(item + 12);
                    entry[7] = (u8)((entry[7] & 0xc0) |
                                    ((Func_08002304(*(u16 *)(item + 12), 20) & 31) << 1));
                    *(u16 *)(entry + 6) = (*(u16 *)(entry + 6) & 0xfe00) |
                                           ((*(u16 *)(item + 6) + 0x1ff) & 0x1ff);
                    entry[4] = (u8)(item[8] - 2);
                }
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                Func_0801908c(item);
                break;
            case 12:
            case 13:
            case 14:
                ++*(u16 *)(item + 12);
                entry[4] = (u8)(item[8] + (s8)((const s8 *)0x08033eb0)[(*(u16 *)(item + 12)) & 15]);
                break;
            case 15:
                if (*(u16 *)(item + 12) != 0) {
                    ++*(u16 *)(item + 12);
                    entry[4] = (u8)(item[8] + (s8)((const s8 *)0x08033eb0)[(*(u16 *)(item + 12)) & 15]);
                }
                break;
            case 16:
                if (*(u16 *)(item + 12) != 0) {
                    ++*(u16 *)(item + 12);
                    entry[4] = (u8)(item[8] + (s8)((const s8 *)0x08033eb0)[(*(u16 *)(item + 12)) & 15]);
                }
                break;
            default:
                break;
            }
            if (item[5] == 2) {
                if (*(u16 *)(state + 0x12b6) != 0x60)
                    Func_08003dec(entry, item[15]);
            } else if (item[5] != 13) {
                Func_08003dec(entry, item[15]);
            }
            item = *(u8 **)item;
        }
    }
}
