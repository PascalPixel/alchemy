/* differ 2: the native loads the cursor before copying the column count; a load latency of 3 explains it, ARM9TDMI's 2 ties */
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "RUNTIME_MEM.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

#include "INPUT.H"
#include "text/MSG_IDS.H"

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
