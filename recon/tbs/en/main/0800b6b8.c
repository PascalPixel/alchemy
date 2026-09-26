/* Draft, not exact (2026-09-26): 208 of 224 bytes, 105 differing halfwords.
   Complete loader [0x0800b6b8, 0x0800b798), including its literal pool.
   The signed table-number field is read then narrowed to u16; separate
   walking field cursors recover the table scan. Remaining: saved-register
   allocation, argument spill, slot store order, and conversion-loop setup.
   Struct-cursor and separate-field-cursor trials retain those differences. */
#include "TYPES.H"

struct ResourceNoEntry {
    u16 res;
    s16 no;
};

struct BufferSlot {
    s32 tag;
    void *buf;
};

struct BufferControl {
    u8 unknown_00[0x1c];
    struct BufferSlot slots[8];
};

extern struct BufferControl *Data_03001e68;
extern struct ResourceNoEntry Data_08012fa0[];
extern u8 Data_080092b8[][256];

void *Func_08185000(s32 no);
void *Func_08002f40(s32 resource_id);
u32 Func_08005340(const void *source, void *destination);

s32 Func_0800b6b8(u32 slot, u32 *buf, s32 no, u32 kind)
{
    struct BufferSlot *rec;
    u8 *meta;
    u32 *p;
    u8 *s;
    u8 *end;
    u8 *conv;
    u32 size;
    u32 cnt;
    u32 k;
    u16 entry_no;
    s32 res;
    u16 *respos;
    s16 *nopos;

    if (slot > 7)
        return 0;

    rec = &Data_03001e68->slots[slot];
    meta = Func_08185000(no);
    rec->tag = (slot << 12) | no;
    rec->buf = buf;

    cnt = 0;
    respos = &Data_08012fa0[0].res;
    nopos = &Data_08012fa0[0].no;
    entry_no = (u16)*nopos;
    res = *respos;
    nopos += 2;
    do {
        respos += 2;
        if (entry_no == 0)
            return 0;
        if (entry_no == no)
            break;
        cnt++;
        if (cnt > 255)
            break;
        entry_no = (u16)*nopos;
        res = *respos;
        nopos += 2;
    } while (1);

    size = Func_08005340(Func_08002f40(res), buf);

    p = buf;
    cnt = 0;
    while (*p != 0) {
        *p += (u32)buf;
        p++;
        cnt++;
        if (cnt > 255)
            break;
    }

    if (kind != 0) {
        k = kind - 1;
        if (k > 4)
            k = 0;
        conv = Data_080092b8[k];
        s = (u8 *)(p + 1);
        end = (u8 *)buf + size;
        while (s < end) {
            if (*s <= 0xdf)
                *s = conv[*s];
            s++;
        }
    }

    return meta[0] * meta[1];
}
