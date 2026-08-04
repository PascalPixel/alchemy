#include "types.h"

struct Window {
    u16 unknown_00[6];
    u16 tile_x;
    u16 tile_y;
};

extern u16 *Data_03001e8c;

void Func_0801e260(u32 x, u32 y, u32 width, u32 height);

void Func_0801e41c(struct Window *window,
                   u32 x1, u32 y1, u32 x2, u32 y2)
{
    u16 *tile;
    u32 position;

    if (x1 == x2) {
        if (y1 == y2)
            return;
        if (y1 > y2) {
            u32 swap = y1;
            y1 = y2;
            y2 = swap;
        }

        Func_0801e260(
            window->tile_x + x1,
            window->tile_y + y1,
            1,
            y2 - y1);
        tile = Data_03001e8c +
            (window->tile_y + y1) * 32 + window->tile_x + x1;

        for (position = y1; position <= y2; position++, tile += 32) {
            u16 value = *tile;

            if (position == y1) {
                switch (value) {
                case 0xf009:
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                case 0xf018:
                    break;
                case 0xf011:
                    value = 0xf018;
                    break;
                case 0xf00e:
                    value = 0xf009;
                    break;
                }
            } else if (position == y2) {
                switch (value) {
                case 0xf00a:
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                case 0xf019:
                    break;
                case 0xf014:
                    value = 0xf019;
                    break;
                case 0xf00e:
                    value = 0xf00a;
                    break;
                }
            } else {
                switch (value) {
                case 0xf00b:
                case 0xf00c:
                case 0xf00d:
                    break;
                case 0xf009:
                case 0xf00a:
                case 0xf00e:
                    value = 0xf00d;
                    break;
                default:
                    value = 0xf00f;
                    break;
                }
            }
            *tile = value;
        }
        return;
    }

    if (y1 != y2)
        return;
    if (x1 > x2) {
        u32 swap = x1;
        x1 = x2;
        x2 = swap;
    }

    Func_0801e260(
        window->tile_x + x1,
        window->tile_y + y1,
        x2 - x1,
        1);
    tile = Data_03001e8c +
        (window->tile_y + y1) * 32 + window->tile_x + x1;

    for (position = x1; position <= x2; position++, tile++) {
        u16 value = *tile;

        if (position == x1) {
            switch (value) {
            case 0xf009:
            case 0xf00a:
            case 0xf00b:
            case 0xf00d:
            case 0xf01a:
                break;
            case 0xf016:
                value = 0xf01a;
                break;
            case 0xf00f:
                value = 0xf00b;
                break;
            }
        } else if (position == x2) {
            switch (value) {
            case 0xf009:
            case 0xf00a:
            case 0xf00c:
            case 0xf00d:
            case 0xf01b:
                break;
            case 0xf017:
                value = 0xf01b;
                break;
            case 0xf00f:
                value = 0xf00c;
                break;
            }
        } else {
            switch (value) {
            case 0xf009:
            case 0xf00a:
            case 0xf00d:
                break;
            case 0xf00b:
            case 0xf00c:
            case 0xf00f:
                value = 0xf00d;
                break;
            default:
                value = 0xf00e;
                break;
            }
        }
        *tile = value;
    }
}
