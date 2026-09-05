#include "types.h"

#define SceneState_ApplyPlacementResult Func_02001544

typedef struct {
    s32 v[6];
} PlacementResult;

extern void Func_02004056(void);
extern s32 Func_02001caa(PlacementResult *out);
extern void Func_02001e56(PlacementResult out);
extern void Func_02004082(void);

void SceneState_ApplyPlacementResult(void)
{
    PlacementResult out;

    Func_02004056();
    if (Func_02001caa(&out) != 0)
        Func_02001e56(out);
    Func_02004082();
}
