#include "types.h"
#include "scene.h"
#include "abi/map/shared/select_group_entry.h"

/*
 * Walks the word table returned by the 0x02008000 service at +0x14 and
 * replaces the current 16-bit id at 0x02000240+0x1c0 with the first entry
 * of the matching group that passes the kind and flag checks.
 *
 * Table layout: a group header word has its upper 20 bits clear and a
 * 12-bit id in the low bits (0x1ff terminates the table); every other word
 * is an entry: bits 0-11 id (0x1ff ends the group), bits 12-19 sub value,
 * bits 20-27 kind (0 ends the group, 0xff matches any kind), bit 28 marks
 * that a flag id follows in the next word.
 */

struct Work_0808a5f8 {
    u8 padding000[0x1c0];
    s16 current;
    s16 sub;
};

typedef u32 *(*TableProvider_0808a5f8)(void);

struct Services_0808a5f8 {
    u8 padding000[0x14];
    TableProvider_0808a5f8 table_provider;
};

extern struct Work_0808a5f8 gCell;
extern struct Services_0808a5f8 gOv;

void Map_Run(s32 kind)
{
    s16 cur;
    u32 *p;
    u32 *start;
    u32 word;
    s32 id;
    s32 result;
    s32 sub;
    s32 entry_sub;
    s32 entry_kind;
    s32 flag;

    cur = gCell.current;
    p = gOv.table_provider();
    result = 999;
    sub = 0;
    if (kind == 999)
        return;

    for (;;) {
        id = *p++;
        if (id & 0xfffff000)
            continue;
        id &= 0xfff;
        if (id == 0x1ff)
            break;
        if (id != cur)
            continue;
        start = p;
        for (;;) {
            word = *p++;
            id = word & 0xfff;
            entry_sub = (word & 0xff000) >> 12;
            entry_kind = (word & 0x0ff00000) >> 20;
            flag = word & 0x10000000;
            if (flag)
                flag = *p++;
            if (id == 0x1ff || entry_kind == 0) {
                result = *start & 0x1ff;
                break;
            }
            if (entry_kind == 0xff || entry_kind == kind) {
                if (flag == 0 || GameFlag_IsSet(flag) == 0) {
                    result = id;
                    sub = entry_sub;
                    break;
                }
            }
        }
        break;
    }

    if (result != 999) {
        gCell.current = result;
        gCell.sub = sub;
    }
}
