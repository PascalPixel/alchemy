#include "types.h"

typedef struct {
    s32 v[6];
} PlacementResult;

extern void Func_02004056(void);
extern s32 Func_02001caa(PlacementResult *result);
extern void Func_02001e56(PlacementResult result);
extern void Func_02004082(void);

void Func_02001544(void)
{
    PlacementResult result;

    Func_02004056();
    if (Func_02001caa(&result) != 0)
        Func_02001e56(result);
    Func_02004082();
}
