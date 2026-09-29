/* Draft, not exact (2026-09-26): 208 of 224 bytes, 105 differing halfwords.
   Complete loader [0x0800b6b8, 0x0800b798), including its literal pool.
   The signed table-number field is read then narrowed to u16; separate
   walking field cursors recover the table scan. Remaining: saved-register
   allocation, argument spill, slot store order, and conversion-loop setup.
   Struct-cursor and separate-field-cursor trials retain those differences.
   2026-09-29 (alchemy permute scorer): the draft scored 3770 (19
   register-only, 12 operand, 7 reordered, 11 inserted, 19 deleted). The
   callees now have their build names (Resource_GetMetadataRecordFar,
   Resource_GetTableEntry, Resource_DecodeType01) and the two tables their
   own scaffold labels, ResourceSlot_NumberTable (08012fa0) and
   ResourceSlot_ConversionTables (080092b8, five 256-byte tables). A
   300-second search (31,000 candidates) then found this body: the
   relocation walk tests the first word before a loop that tests at its
   bottom, and the conversion bounds come before the kind test. It scores
   535 (11 register-only, 2 operand, 4 reordered, 2 deleted). Remaining: the
   number table scan zero-extends at the loop head where the reference
   extends after each ldrsh, and picks r0 for the ldrsh index constants
   (r1 and r6 in the reference, whose res load is live across them);
   reading res first gives those registers but loses the extension order
   (795); and the conversion pointer stays in the relocation cursor's
   register (r4) where the reference moves it to r5. Two more searches
   (160,000 candidates) found nothing below 535.
 */
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
extern struct ResourceNoEntry ResourceSlot_NumberTable[];
extern u8 ResourceSlot_ConversionTables[][256];

void *Resource_GetMetadataRecordFar(s32 no);
void *Resource_GetTableEntry(s32 resource_id);
u32 Resource_DecodeType01(const void *source, void *destination);

s32 ResourceSlot_Load(u32 slot, u32 *buf, s32 no, u32 kind)
{
    u8 *meta;
    struct BufferSlot *rec;
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
    struct BufferControl *tmp;

    if (slot > 7)
        return 0;
    tmp = Data_03001e68;
    rec = &tmp->slots[slot];
    meta = Resource_GetMetadataRecordFar(no);
    rec->tag = (slot << 12) | no;
    rec->buf = buf;
    respos = &ResourceSlot_NumberTable[0].res;
    cnt = 0;
    nopos = &ResourceSlot_NumberTable[0].no;
    entry_no = *nopos;
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
    size = Resource_DecodeType01(Resource_GetTableEntry(res), buf);
    p = buf;
    cnt = 0;
    if (*p != 0) {
        while (1) {
            *p += (u32)buf;
            ++p;
            cnt++;
            if (cnt > 255)
                break;
            if (*p == 0)
                break;
        }
    }
    if (kind != 0) {
        s = (u8 *)(p + 1);
        end = size + (u8 *)buf;
        k = kind - 1;
        if (k > 4)
            k = 0;
        conv = ResourceSlot_ConversionTables[k];
        for (; s < end; s++) {
            if (*s <= 0xdf)
                *s = conv[*s];
        }
    }
    return meta[0] * meta[1];
}
