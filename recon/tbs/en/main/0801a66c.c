/* Draft: whole 268-byte UiGlyph_ResetWorkState, including 12-byte pool.
 * Candidate 264 bytes, 95 differing halfwords (16 aligned edits).
 * Remaining: initial zero shares the count register, row-pair stores use
 * +374/+426 instead of +372/+424 with offset 2, and two pad bytes absent.
 * Scoped initialization and cursor helper did not close it.
 */
#include "TYPES.H"

struct GlyphRow {
    u8 unknown_00[8];
    struct { u16 unused; u16 cursor; } index;
    u8 unknown_0c[6];
    u16 state;
    u8 unknown_14[32];
};


struct GlyphVisual {
    u8 unknown_00[5];
    u8 flags05_0 : 2;
    u8 flags05_2 : 2;
    u8 flags05_4 : 1;
    u8 flags05_5 : 1;
    u8 flags05_6 : 2;
    u8 unknown_06;
    u8 flags07_0 : 1;
    u8 flags07_1 : 5;
    u8 flags07_6 : 2;
    u8 unknown_08;
    u8 flags09_0 : 2;
    u8 flags09_2 : 2;
    u8 flags09_4 : 4;
    u8 unknown_0a[2];
};

struct GlyphWork {
    struct GlyphRow rows[14];
    u8 unknown_2d8[10];
    u16 field_2e2;
    u16 slot;
    u16 tile;
    u8 unknown_2e8[18];
    u16 field_2fa;
    u8 unknown_2fc[4];
    struct GlyphVisual visual;
    u8 unknown_30c[10];
    u16 field_316;
    u8 unknown_318[48];
    s32 field_348;
    s32 field_34c;
    s32 field_350;
    u8 unknown_354[64];
    u16 field_394;
    u8 unknown_396[4];
    u16 field_39a;
    u16 field_39c;
    u16 field_39e;
    u16 field_3a0;
    u8 unknown_3a2[22];
    u16 field_3b8;
    u8 unknown_3ba[42];
};

extern const u8 Data_080346f8[];
struct GlyphWork *Runtime_AllocateBlock(s32 kind, s32 size);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);

void UiGlyph_ResetWorkState(void)
{
    struct GlyphWork *work;
    struct GlyphVisual *visual;
    const u8 *tbl;
    s32 i;

    work = Runtime_AllocateBlock(18, 996);
    work->field_348 = 0;
    work->field_34c = 0;
    work->field_350 = 0;
    work->field_39a = 0;
    work->field_39c = 0;
    work->field_39e = 128;
    work->field_3a0 = 32;
    work->field_394 = 0;
    work->field_3b8 = 999;
    i = 0;
    do {
        work->rows[i + 2].index.cursor = 0;
        work->rows[i + 9].index.cursor = 0;
        i++;
    } while (i != 5);
    work->rows[7].index.cursor = 0;
    work->rows[8].index.cursor = 0;
    work->rows[0].index.cursor = 0;
    work->rows[1].index.cursor = 0;
    work->rows[0].state = 0;
    work->rows[1].state = 0;
    tbl = Data_080346f8;
    work->slot = Resource_FindFreeEntry();
    work->tile = VramBlock_LoadCached(work->slot, 256, tbl);
    work->field_2e2 = 0;
    work->field_2fa = 0;
    work->field_316 = 0;
    visual = &work->visual;
    visual->flags05_2 = 0;
    visual->flags05_4 = 0;
    visual->flags05_5 = 1;
    visual->flags05_0 = 0;
    visual->flags07_1 = 0;
    visual->flags07_6 = 1;
    visual->flags05_6 = 0;
    visual->flags09_2 = 0;
}
