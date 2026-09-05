#include "types.h"

extern s32 Data_03001e40;

extern void Func_02008180(u8 *, s32);
extern void Func_0200818a(u8 *, s32);
extern void Func_02007f98(u8 *);

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_02008180(obj, 7);
    } else {
        Func_0200818a(obj, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02007f98(obj);
    }
}
