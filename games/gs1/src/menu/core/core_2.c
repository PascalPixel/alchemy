#include "menu_result.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

/* menu/core/build_page_result.c */
#if defined(GS1_EDITION_JA)
#define GROUP_LEN 6
#else
#define GROUP_LEN 5
#endif

extern u8 *gIw;
s32 Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)ADDR_03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = Runtime_GetObject(LoadByte(entries, offset));
    limit = LoadByte(base, 0x218);
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = FixedPoint_Ratio(value, GROUP_LEN);
    remainder = Modulo(value, GROUP_LEN);
    groups = FixedPoint_Ratio(limit, GROUP_LEN);
    if (Modulo(limit, GROUP_LEN) != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}

/* menu/core/build_pattern_tiles.c */
struct TileMask {
    u32 word0;
    u32 word1;
};

typedef void (*FillProc)(void *dst, s32 size, u32 word);

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask MaskTbl[];

#define FillWords ((FillProc)0x03000168)

void Menu_BuildPatternTiles(void)
{
    u32 *vram = (u32 *)0x06005000;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], MaskTbl[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], MaskTbl[mi].word1);
            }
        }
    }
}
