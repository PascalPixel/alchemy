#include "types.h"

struct WindowWork_08016f2c {
    s32 field00;
    s32 field04;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 field10;
    u16 field12;
    u16 field14;
    u16 flags;
    s16 frame;
    u16 duration;
    u16 previousX;
    u16 previousY;
    u16 previousWidth;
    u16 previousHeight;
};

struct State_08016f2c {
    u8 padding000[0x500];
    struct WindowWork_08016f2c windows[8];
    u8 padding620[0x883];
    u8 needsRefresh;
};

extern struct State_08016f2c *Data_03001e8c;

void Func_08016178(s16 x, s16 y, s16 width, s16 height);
void Func_08016230(struct WindowWork_08016f2c *window);
void Func_08017004(struct WindowWork_08016f2c *window, s32 savePosition);

void Func_08016f2c(void)
{
    struct State_08016f2c *state;
    struct WindowWork_08016f2c *window;
    s32 index;

    state = Data_03001e8c;
    window = state->windows;
    index = 0;
    do {
        u16 flags;

        flags = window->flags;
        if (flags != 0) {
            if ((s16)window->frame != 0) {
                Func_08017004(window, 0);
                window->frame--;
            } else if ((s16)window->duration != 0) {
                Func_08016230(window);
            }
        } else if ((s16)window->duration != 0) {
            if ((s16)window->frame != (s16)window->duration) {
                Func_08016178((s16)window->previousX,
                              (s16)window->previousY,
                              (s16)window->previousWidth,
                              (s16)window->previousHeight);
                Func_08017004(window, 1);
                window->frame++;
            } else {
                s16 previousY;
                s16 previousHeight;

                previousY = (s16)window->previousY;
                previousHeight = (s16)window->previousHeight;
                Func_08016178((s16)window->previousX, previousY,
                              (s16)window->previousWidth, previousHeight);
                window->field00 = flags;
                window->field04 = flags;
                window->width = flags;
                window->height = flags;
                window->x = flags;
                window->y = flags;
                window->field10 = flags;
                window->field12 = flags;
                window->field14 = flags;
                window->flags = flags;
                window->frame = 0;
                window->duration = flags;
                window->previousX = flags;
                window->previousY = flags;
                window->previousWidth = flags;
                window->previousHeight = flags;
            }
            state->needsRefresh = 1;
        }
        index++;
        window++;
    } while (index != 8);
}
