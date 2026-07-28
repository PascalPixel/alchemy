#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct LookupEntry {
    s32 key;
    s32 value;
};

s32 Func_0800b798(s32 key)
{
    u32 no;
    struct LookupEntry *entry;

    entry = (struct LookupEntry *)(*(u32 *)0x03001e68 + 0x1c);
    no = 0;
loop_1:
    if (entry->key == key) {
        return entry->value;
    }
    no++;
    entry++;
    /* 表は8要素で終わる。 */
    if (no > 7) {
        return 0;
    }
    goto loop_1;
}
