#include "TYPES.H"
#include "FIXED_MATH.H"

u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width)
{
    s32 offset;
    s32 value;
    s32 negative;
    s32 space;
    s32 minus;
    u8 *leading;
    u8 *trim;

    value = input;
    negative = 0;
    if (value < 0) {
        if (width == 0)
            negative = 1;
        value = (s32)(0U - (u32)value);
    }

    buffer[0] = ' ';
    for (offset = 12; offset != 0; offset--) {
        buffer[offset] = Math_Mod(value, 10) + '0';
        value = Math_Div(value, 10);
    }

    offset = 0;
    buffer[13] = offset;
    space = ' ';
    offset = 1;
    minus = '-';
    for (leading = buffer; offset != 13; leading++, offset++) {
        if (leading[1] == '0') {
            if (offset != 12)
                leading[1] = space;
        } else {
            if (negative != 0)
                leading[0] = minus;
            break;
        }
    }

    if (width == 0) {
        offset = 0;
        if (buffer[0] == ' ') {
            trim = buffer;
            do {
                offset++;
                if (offset == 12)
                    break;
                trim++;
            } while (*trim == ' ');
        }
        return buffer + offset;
    }

    if ((u32)width > 12)
        width = 12;
    return buffer + 13 - width;
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

/* The English articles by kind: "a ", "an ", "some ", "the ". */
struct ArticleTable {
    s8 *text[8];
};

extern const struct ArticleTable Data_08033e40;

/* Append a name to the 512-entry render ring, optionally preceded by an
   article (code 29 and a kind at its head selects one; otherwise a vowel
   takes "an ") and followed by a plural "s" or "es". A name ending in S or
   s asks for "es". With mode set, the name is wrapped in a space, two
   line breaks and a closing sequence. */
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
        entry[pos] = ' ';
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x0a;
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
    } else {
        head = name[0];
        if (head == 29)
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
            entry[pos] = 'e';
            pos = (pos + 1) & 0x1ff;
        }
        entry[pos] = 's';
        pos = (pos + 1) & 0x1ff;
    }
    if (mode != 0) {
        entry[pos] = 0x0a;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = 0x08;
        pos = (pos + 1) & 0x1ff;
        entry[pos] = ' ';
        pos = (pos + 1) & 0x1ff;
    }
    return pos;
}
#endif
