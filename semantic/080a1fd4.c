#include "types.h"

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_0800352c(void);
void Func_08015418(s32);
void Func_080f9010(s32);

s32 Func_080a1fd4(
    s32 orientation,
    s32 item_count,
    s32 page_size,
    s32 *cursor,
    s32 *page)
{
    s32 page_count;
    s32 increment;
    s32 decrement;
    s32 previous;
    s32 next;
    s32 base;
    s32 remaining;

    if (item_count == 0)
        return -1;

    Func_08015418(0x06002500);
    page_count = Func_080022ec(item_count, page_size);
    if (Func_080022fc(item_count, page_size) != 0)
        page_count++;

    if (orientation != 0) {
        increment = *(u32 *)0x03001B04 & 0x10;
        decrement = *(u32 *)0x03001B04 & 0x20;
        previous = *(u32 *)0x03001B04 & 0x40;
        next = *(u32 *)0x03001B04 & 0x80;
    } else {
        increment = *(u32 *)0x03001B04 & 0x80;
        decrement = *(u32 *)0x03001B04 & 0x40;
        previous = *(u32 *)0x03001B04 & 0x20;
        next = *(u32 *)0x03001B04 & 0x10;
    }

    if (previous != 0) {
        Func_080f9010(0x6F);
        (*page)--;
        if (*page < 0)
            *page = page_count - 1;
        goto page_changed;
    }

    if (next != 0) {
        Func_080f9010(0x6F);
        (*page)++;
        if (*page > page_count - 1)
            *page = 0;

page_changed:
        base = page_size * *page;
        if (*cursor + base > item_count - 1) {
            *cursor = item_count - base - 1;
            if (*cursor > page_size - 1)
                *cursor = page_size - 1;
        }
        Func_0800352c();
        return 1;
    }

    if (decrement != 0) {
        Func_080f9010(0x6F);
        (*cursor)--;
        if (*cursor < 0) {
            remaining = item_count - (*page * page_size) - 1;
            *cursor = remaining;
            if (*cursor > page_size - 1)
                *cursor = page_size - 1;
        }
        return 0;
    }

    if (increment != 0) {
        Func_080f9010(0x6F);
        (*cursor)++;
        remaining = item_count - (*page * page_size);
        if (*cursor == remaining)
            *cursor = 0;
        if (*cursor > page_size - 1)
            *cursor = 0;
        return 0;
    }

    return -1;
}
