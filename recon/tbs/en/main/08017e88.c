/*
 * UiText_AppendArticleName (main:08017e88, 432 bytes)
 *
 * Appends a name to the 512-entry render ring, optionally preceded by an
 * English article ("a ", "an ", "some ", "the " from the Data_08033e40
 * table) and followed by a plural "s" or "es".  A leading code 29 selects
 * the article kind explicitly; otherwise a name starting with a vowel takes
 * "an ".  A name ending in S or s asks for "es" next time.  With mode set,
 * the name is wrapped in a space, two newlines and a closing sequence.
 *
 * Draft, not exact (2026-09-24): candidate=432 reference=432
 * differing_halfwords=38, 85% similar.  Every block, register and store
 * lines up; the ring codes are HImode pool constants (a Value_ symbol
 * zero-extended from u16, 60-byte reach) stored through the ring slot
 * address computed first, as the helper does.  Remaining: (1) the first
 * literal pool lands after the mode block, where the reference places it
 * after the article test, and the third pool two instructions early; GCC's
 * pool placement estimate is four bytes longer here than in the reference.
 * (2) The else path of the article test loads *name once and compares a
 * copy; the reference loads it twice (a HImode copy for the loop and a
 * zero-extended load for the == 29 test).  Loop spellings (while/for,
 * post-increment placement) and the helper argument order did not move
 * either.
 */
#include "TYPES.H"

struct ArticleTable {
    s8 *text[8];
};

extern const struct ArticleTable Data_08033e40;
extern u8 Value_00000020;
extern u8 Value_0000000a;
extern u8 Value_00000008;
extern u8 Value_00000065;
extern u8 Value_00000073;

static __inline__ void UiText_PutEntry(u16 *dst, u8 *code)
{
    s32 c = (u16)(s32)code;

    *dst = c;
}

u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry,
                             s32 no, s32 plural, s32 *suffix)
{
    struct ArticleTable tbl;
    s32 kind;
    u16 head;
    u16 c;
    u8 *p;
    s8 c8;
    s32 cnt;

    if (mode != 0) {
        UiText_PutEntry(&entry[pos], &Value_00000020);
        pos = (pos + 1) & 0x1ff;
        UiText_PutEntry(&entry[pos], &Value_0000000a);
        pos = (pos + 1) & 0x1ff;
        UiText_PutEntry(&entry[pos], &Value_0000000a);
        pos = (pos + 1) & 0x1ff;
    }
    if (no == 1 || (no == 3 && plural == 0)) {
        kind = 0;
        tbl = Data_08033e40;
        head = name[0];
        if (head == 29) {
            kind = name[1] - 1;
            name += 2;
        }
        if (kind == 0) {
            if (head == 'A' || head == 'I' || head == 'U' || head == 'E' || head == 'O')
                kind = 2;
            else
                kind = 1;
        }
        p = tbl.text[kind & 7];
        for (cnt = 0; cnt < 8; cnt++) {
            if ((c8 = *p++) == 0)
                break;
            entry[pos] = c8;
            pos = (pos + 1) & 0x1ff;
        }
    } else if (name[0] == 29) {
        name += 2;
    }
    while (*name != 0) {
        c = *name++;
        entry[pos] = (s16)c;
        pos = (pos + 1) & 0x1ff;
        if (c == 'S' || c == 's')
            *suffix = 1;
        else
            *suffix = 0;
    }
    if (no == 2 || (no == 3 && plural != 0)) {
        if (*suffix != 0) {
            UiText_PutEntry(&entry[pos], &Value_00000065);
        pos = (pos + 1) & 0x1ff;
        }
        UiText_PutEntry(&entry[pos], &Value_00000073);
        pos = (pos + 1) & 0x1ff;
    }
    if (mode != 0) {
        UiText_PutEntry(&entry[pos], &Value_0000000a);
        pos = (pos + 1) & 0x1ff;
        UiText_PutEntry(&entry[pos], &Value_00000008);
        pos = (pos + 1) & 0x1ff;
        UiText_PutEntry(&entry[pos], &Value_00000020);
        pos = (pos + 1) & 0x1ff;
    }
    return pos;
}
