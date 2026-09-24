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
 * differing_halfwords=50, 79% similar.  Every block, the vowel chain, both
 * loops and all three literal pools line up.  The ring codes are HImode pool
 * constants (zero-extended u16 of a Value_ symbol, 60-byte reach); the inline
 * helper reproduces that but loads each code before its index is computed,
 * so every code store swaps r2/r3 against the reference, and the first pool
 * lands after the mode block instead of after the article test.  The
 * reference computes the ring index first, as a direct store would, but a
 * direct store folds the constant to SImode (1020-byte reach).  The else
 * path of the article test reloads *name twice in the reference (a HImode
 * copy and a zero-extended compare); this draft loads it once.
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

static __inline__ u32 UiText_PutEntry(u16 *entry, u32 pos, s32 ch)
{
    entry[pos] = ch;
    return (pos + 1) & 0x1ff;
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
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_00000020);
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_0000000a);
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_0000000a);
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
            pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_00000065);
        }
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_00000073);
    }
    if (mode != 0) {
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_0000000a);
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_00000008);
        pos = UiText_PutEntry(entry, pos, (u16)(s32)&Value_00000020);
    }
    return pos;
}
