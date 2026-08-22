#include "types.h"
/* resource_37b owner at 0x020015d4, 40 bytes. Setup, then a guarded call. */
void Func_02003a08();
s32 Func_020031d6();
void Func_0200383a();
void Func_020015d4(void)
{
    Func_02003a08(2, 0x00d00000, 0x00700000, 0);
    if (Func_020031d6(10, 14, 7) != 0) {
        Func_0200383a();
    }
}
