#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/initialize_state_interaction.h"

extern s16 gCell[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;

void State_Run(s32 a, s32 b)
{
    s32 v;
    s32 id;

    State_Apply(b, 5);
    v = gCell[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    State_Do(id + 1);
    State_Apply2(a, 0);
}
