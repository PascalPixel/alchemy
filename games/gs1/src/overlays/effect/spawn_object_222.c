#include "types.h"

extern s32 Data_03001e40;
extern s32 Data_0200d240[3];   /* image offset 0x5240 */

extern u8 *Func_02006652(s32 arg0, s32 arg1, s32 arg2, s32 arg3);  /* site veneer -> Func_080090c8 */
extern void Func_0200559e(u8 *obj);                             /* site veneer -> Func_02002a10 */
extern void Func_0200666a(u8 *obj, s32 arg1);                   /* site veneer -> Object_SetMode */

void SceneEffect_SpawnObject222(void)
{
    u8 *obj;

    if ((Data_03001e40 & 3) != 0) {
        return;
    }
    obj = Func_02006652(222, Data_0200d240[0], Data_0200d240[1], Data_0200d240[2]);
    if (obj == 0) {
        return;
    }
    {
        u16 *p = (u16 *)(obj + 100);
        u16 v = 30;
        *p = v;
    }
    {
        u16 *q = (u16 *)(obj + 102);
        u16 w = 1;
        *q = w;
    }
    *(s32 *)(obj + 104) = 20;
    Func_0200559e(obj);
    *(s32 *)(obj + 108) = 0x0200aa49;
    Func_0200666a(obj, 1);
}
