#include "types.h"





/* Complete 36-byte actor-0x800b dialogue owner through its two-word pool. */
extern void Func_02001438(void);
extern void Func_020014e6(s32 message);
extern void Func_020014fe(s32 actor, s32 mode);
extern void Func_02001452(void);
void Func_02000298(void)
{
    Func_02001438();
    Func_020014e6(0x1c40);
    Func_020014fe(0x800b, 0);
    Func_02001452();
}
