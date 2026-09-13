#include "types.h"
#include "scene.h"

/* ui/icon/build_item_icon_tiles.c */
typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern void FunctionHead_0801a5a4(FontTransfer *work, s32 slot);

extern FontTransfer *Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern s32 Func_08004080(void);
extern s32 Resource_CopyData(s32 index, s32 size, u8 *destination);

extern s32 gRom[];
extern s32 gRom2[];

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    slot = 0;
    work = Runtime_AllocateHeapBlock(0x11, 0x608);

    if (glyph >= FunctionHead_08019ebc())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = gRom[2];
        work->f600 = 2;
        work->f602 = 2;
        FunctionHead_0801a5a4(work, 0);
        slot = 1;
    }

    work->f604 = gRom2[glyph];
    work->f600 = 2;
    work->f602 = 2;
    FunctionHead_0801a5a4(work, slot);

    if (reuse == 0)
        *src = Func_08004080();

    *dst = Resource_CopyData(*src, 0x80, &work->f400);
    FunctionHead_08002dd8(0x11);
}
