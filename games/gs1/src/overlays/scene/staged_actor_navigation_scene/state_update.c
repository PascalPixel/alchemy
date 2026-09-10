#include "types.h"

#define SceneState_ApplyPlacementResult Func_02001544
#define SceneState_ClearWord24AndObjectByte62 Func_0200209c
typedef struct {
    s32 v[6];
} PlacementResult;

extern u8 *Data_03001ee0;

void Func_02004056(void);
s32 Func_02001caa(PlacementResult *out);
void Func_02001e56(PlacementResult out);
void Func_02004082(void);
u8 *Func_02004bbe();

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */

void SceneState_ApplyPlacementResult(void)
{
    PlacementResult out;

    Func_02004056();
    if (Func_02001caa(&out) != 0)
        Func_02001e56(out);
    Func_02004082();
}

/*
 * Clears Data_03001ee0[+24] and one flag byte on the object returned by
 * Func_02004bbe. The 28-byte owner at 0x0200209c includes its one pool
 * word, the Data_03001ee0 pointer.
 */
void SceneState_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Func_02004bbe(0);

    *(s32 *)(Data_03001ee0 + 24) = 0;
    obj[0x62] = 0;
}
