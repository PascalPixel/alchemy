#include "types.h"

extern u8 *Data_03001f30;
extern void Func_0808a428(s32 value, s32 mode);
extern void Func_0808a430(s32 left, s32 right);
extern void Func_0808a448(void);
extern void Func_0808a420(s32 enabled);
extern void Func_0808a438(void);

/* Configure the presentation layer and expose workspace feature 0x71c. */
void Func_02000d68(void)
{
    Func_0808a428(78, 1);
    Func_0808a430(2, 15);
    Data_03001f30[0x71c] |= 8;
    Func_0808a448();
    Func_0808a420(1);
    Func_0808a438();
}
