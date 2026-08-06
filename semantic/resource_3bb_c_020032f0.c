#include "types.h"






/*
 * resource_3bb owner at 0x020032f0, 84 bytes: move the indexed scene record
 * to the source object's position, install its callback, then consume the
 * source index.  The final callback word belongs to this owner.
 */
extern u8 * Func_02007254();
extern void Func_0200718e();
extern void Func_02007176();
extern void Func_020073dc();
s32 Func_020032f0(u8 *object)
{
    s16 *index = (s16 *)(object + 100);
    u8 *record = Func_02007254(*index);

    Func_0200718e(record,
                  *(s32 *)(object + 8),
                  *(s32 *)(object + 12) + 0x02400000,
                  *(s32 *)(object + 16));
    Func_02007176(record, (void *)0x0200c288);
    record[0x55] = 0;
    Func_020073dc(0x53);
    *index = 0;
    return 0;
}
