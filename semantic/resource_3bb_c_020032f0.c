#include "types.h"

u8 *Func_0808a080();
void Func_08009098();
void Func_08009150();
void Func_080f9010();

/*
 * resource_3bb owner at 0x020032f0, 84 bytes: move the indexed scene record
 * to the source object's position, install its callback, then consume the
 * source index.  The final callback word belongs to this owner.
 */
s32 Func_020032f0(u8 *object)
{
    s16 *index = (s16 *)(object + 100);
    u8 *record = Func_0808a080(*index);

    Func_08009150(record,
                  *(s32 *)(object + 8),
                  *(s32 *)(object + 12) + 0x02400000,
                  *(s32 *)(object + 16));
    record[0x55] = 0;
    Func_08009098(record, (void *)0x0200c288);
    Func_080f9010(0x53);
    *index = 0;
    return 0;
}
