#include "types.h"
/* Actor-8 tile-triggered presentation owner, 0x02000380-0x020003e3. */
  
 
extern u8 * Func_02000aac();
extern void Func_02000892();
extern u8 * Func_02000ac8();
extern void Func_02000aa6();
extern u8 * Func_02000aec();
extern void Func_02000aba();
extern void Func_02000ae8();
void Func_02000380(void)
{
    u8 *actor = Func_02000aac(8); s32 x = *(s32 *)(actor + 8);
    if (x < 0) x += 0xfffff;
    if ((x >> 20) != 24) return;
    Func_02000892(8);
    actor = Func_02000ac8(8); actor[35] |= 2;
    Func_02000aa6(19, 74, 9, 3, 19, 17);
    actor = Func_02000aec(8); Func_02000aba(actor, 0);
    Func_02000ae8(0x864);
}
