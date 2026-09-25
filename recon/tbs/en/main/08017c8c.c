/* Draft, not exact: 62 differing halfwords, 320-byte candidate for the
   328-byte owner (2026-09-24). The terminator block, switch table, call
   and kerning test match. Residual: the loop. The reference loads each
   character with ldrsh, keeps it zero-extended as (c << 16) in r1 for the
   exit test and derives the u16 in r5 with lsrs in the body; this
   candidate tests the sign-extended value directly and keeps c in r5.
   Tried u16/s32/s16 locals, a separate u16 copy, an explicit duplicated
   load, a comma test and a shifted test. */

#include "TYPES.H"

struct TextWork {
    u8 unknown_000[0xea3];
    u8 dirty;
    u8 unknown_ea4[0xc];
    u16 entries[0x200];
    u16 unknown_12b0;
    u16 count;
};

extern struct TextWork *gWindowWork;

s32 Func_08018efc(void *window, u32 c, s32 x, s32 y, s32 flags);

void UiText_RenderWideStringInWindow(s16 *text, void *window, s32 x, s32 y)
{
    struct TextWork *work;
    s16 c;
    s16 start;

    work = gWindowWork;
    start = x;
    if (text == NULL) {
        text = (s16 *)work->entries;
        work->entries[work->count] = 0;
        work->count = (work->count + 1) & 0x1ff;
    }
    while ((c = *text++) != 0) {
        if ((u16)c <= 30) {
            switch ((u16)c) {
            case 3:
                x = start;
                y++;
                break;
            case 14:
            case 15:
            case 28:
                text++;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 17:
            case 29:
                text++;
                break;
            }
        } else {
            Func_08018efc(window, (u16)c, x, y, 0);
            if ((u16)((u16)c - 0xde) > 1)
                x++;
        }
    }
    work->dirty = 1;
}
