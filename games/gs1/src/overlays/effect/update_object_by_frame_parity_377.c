#include "types.h"

#define SceneEffect_UpdateObjectByFrameParity Func_02001544

extern s32 Data_03001e40;

extern void Func_02002e24(u8 *, s32);
extern void Func_02002e2e(u8 *, s32);
extern void Func_02002c00(u8 *);

void SceneEffect_UpdateObjectByFrameParity(u8 *obj)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_02002e24(obj, 7);
    } else {
        Func_02002e2e(obj, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c00(obj);
    }
}
