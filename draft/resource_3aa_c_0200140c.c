#include "types.h"
/* resource_3aa owner at 0x0200140c, 68 bytes. Four channel setups, then an
 * optional handoff when the caller passed a non-null argument. */
void Func_02002f4e();
void Func_02002f5a();
void Func_02002f66();
void Func_02002f72();
void Func_02002ec4();
void Func_0200140c(s32 arg0)
{
    Func_02002f4e(0, 0x6000, 0);
    Func_02002f5a(1, 0xe000, 0);
    Func_02002f66(2, 0x2000, 0);
    Func_02002f72(3, 0xa000, 0);
    if (arg0 != 0) {
        Func_02002ec4(arg0);
    }
}
