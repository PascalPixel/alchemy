#include "types.h"
extern s32 Func_080770c0(s32 flagId);
extern void Func_0808a038(const void *table);

/* Complete 68-byte two-flag table selector through its five-word pool. */
void *Func_020000fc(void)
{
    void *table;

    if (Func_080770c0(0x87a) != 0)
        table = (void *)0x020098b8;
    else if (Func_080770c0(0x815) != 0)
        table = (void *)0x02009738;
    else
        table = (void *)0x020095b8;
    Func_0808a038(table);
    return table;
}
