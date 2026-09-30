#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RESOURCE.H"

extern u8 RomBytes_080308a0[];

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
extern void UiGlyph_DecodeWithHeapRoutines(FontTransfer *work, s32 slot);
extern s32 VramBlock_LoadCached(s32 index, s32 size, u8 *destination);
extern s32 RomBytes_08029a10[];
extern s32 UiIcon_PsynergyIconPointers[];

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gGlyphWork;
extern u32 UiIcon_MiscIconPointers[];

/* ui/icon/build_ability_icon_tiles.c */
/* ui/icon/icon_build_ability_icon_tiles.c */
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    slot = 0;

    if (glyph >= Ui_CountSecondTableEntries())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = RomBytes_08029a10[2];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->f604 = UiIcon_PsynergyIconPointers[glyph];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, &work->f400);
    Runtime_ReleaseHeapBlock(0x11);
}

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    struct State_0801a4c0 *state = gGlyphWork;

    state->value = UiIcon_MiscIconPointers[index];
    state->first = 2;
    state->second = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}
