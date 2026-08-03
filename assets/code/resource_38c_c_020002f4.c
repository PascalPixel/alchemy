#include "resource_38c.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_02000832(s32);
extern void Func_02000818(void);
extern void Func_020008a2(s32, s32);
extern s32 Func_0200081a(s32);
extern void Func_02000874(s32);
extern void Func_02000884(s32);
/* Both arms reach this one name from sites 0x10 apart, so it stands for two
 * different veneers -- 0x570 from the first arm and 0x560 from the second. */
extern void Func_0200089c(s32, s32);
extern void Func_02000860(void);

void Func_020002f4(void)
{
    u32 place;

    place = *(u16 *) (Func_02000832(0) + 6);
    Func_02000818();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_020008a2(2, 19);
    } else if (Func_0200081a(0x845) != 0) {
        Func_02000874(0x16FB);
        Func_0200089c(19, 0);
    } else {
        Func_02000884(0x13EB);
        Func_0200089c(19, 0);
    }

    Func_02000860();
}
