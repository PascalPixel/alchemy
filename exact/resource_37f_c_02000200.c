#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02001db0(void);  /* import veneer -> Func_0808a018 */
extern s32 Func_02001d96();       /* import veneer -> Func_080770c0, 1st site */
extern void Func_02001d9a();      /* import veneer -> Func_08015040, 1st site */
extern void Func_02001da4();      /* import veneer -> Func_08015040, 2nd site */
extern s32 Func_02001db2();       /* import veneer -> Func_080770c0, 2nd site */
extern void Func_02001df0(void);  /* import veneer -> Func_0808a020 */

void Func_02000200(void)
{
    Func_02001db0();

    if (Func_02001d96(0x81a) != 0) {
        Func_02001d9a(0x1034, 1);
    } else {
        Func_02001da4(0x1031, 1);
        if (Func_02001db2(0xf01) != 0) {
            u16 *p = (u16 *)(Data_03001ebc + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Func_02001df0();
}
