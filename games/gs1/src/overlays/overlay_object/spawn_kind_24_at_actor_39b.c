#include "types.h"

#define OverlayObject_SpawnKind24AtActor Func_02001bc8

extern u8 *Func_02004020();
extern void Func_02004026();
extern void Func_02004038();

/* Spawn and configure the compact companion object at a source position. */
void OverlayObject_SpawnKind24AtActor(u8 *src)
{
    u8 *obj = Func_02004020(24, *(int *)(src + 8),
                              *(int *)(src + 12), *(int *)(src + 16));
    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        Func_02004026(obj, (void *)0x0200a7b8);
        obj[85] = 0;
        obj[34] = 1;
        obj[35] = 2;
        if (rec != 0) {
            Func_02004038(rec, 2);
            rec[38] = 0;
            rec[9] |= 0x0c;
        }
    }
}
