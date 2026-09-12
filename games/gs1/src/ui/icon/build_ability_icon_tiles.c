#include "types.h"
#include "scene.h"
#include "global_cells.h"

extern u8 RomBytes_080308a0[];

/* ui/icon/build_ability_icon_tiles.c */
/* ui/icon/icon_build_ability_icon_tiles.c */
/* ui/icon/build_ability_icon_tiles.c */
typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern FontTransfer *Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData(s32 index, s32 size, u8 *destination);

extern s32 RomBytes_08029a10[];
extern s32 gRom2[];

void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    slot = 0;

    if (glyph >= Ui_Check())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = RomBytes_08029a10[2];
        work->f600 = 2;
        work->f602 = 2;
        Ui_Apply(work, 0);
        slot = 1;
    }

    work->f604 = gRom2[glyph];
    work->f600 = 2;
    work->f602 = 2;
    Ui_Apply(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeSlot();

    *dst = Resource_CopyData(*src, 0x80, &work->f400);
    Ui_Do(0x11);
}
