/* differ 2: the native loads the cursor before copying the column count; a load latency of 3 explains it, ARM9TDMI's 2 ties */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "dma.h"
#include "resource.h"
#include "render_input.h"
#include "mem.h"
#include "ram_buffer.h"
#include "io_reg.h"

#include "input.h"
#include "text/msg_ids.h"

struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

struct ShopRuntime {
    u8 unknown_000[0x4dc];
    struct RenderOutput *anchor;
    u8 unknown_4e0[0x1a];
    u16 keeper_resource;
    u8 unknown_4fc[0xa];
    s8 shop_type;
};

void WaitFrames(s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePendingCoreFar(void);

s32 Func_08108690(s32 *cursor, s32 count, s32 columns)
{
    s32 changed = 0;
    s32 old = *cursor;
    s32 pos = old;

    if (gInput.repeat & 0x20) {
        pos--;
        pos = (pos + count) % count;
    }
    if (gInput.repeat & 0x10) {
        pos++;
        pos = (pos + count) % count;
    }
    if (gInput.repeat & 0x40) {
        if (pos - columns >= 0)
            pos -= columns;
    }
    if (gInput.repeat & 0x80) {
        if (pos + columns < (columns + count - 1) / columns * columns)
            pos += columns;
        if (pos > count - 1)
            pos = count - 1;
    }
    if (old != pos) {
        Audio_PlayCue(111);
        changed = 1;
    }
    if (old / columns * columns != pos / columns * columns)
        changed = 2;
    *cursor = pos;
    return changed;
}
