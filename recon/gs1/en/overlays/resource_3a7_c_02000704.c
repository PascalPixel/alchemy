#include "types.h"
/*
 * resource_3a7 owner at 0x02000704, 48 bytes.
 * Sets bit 1 of the byte at +35 on six consecutive records, ids 16..21.
 */
u8 *Func_02001848();
void Func_02000704(void)
{
    s32 id;
    for (id = 16; id <= 21; id++) {
        u8 *rec = Func_02001848(id);
        rec[35] |= 2;
    }
}
