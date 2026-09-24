#include "TYPES.H"
#include "SCENE.H"
#include "TBS_EDITION.H"

struct State08093304 {
    u8 padding[0x12F4];
    s16 ret;
    s16 value;
};

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 ObjectTable_ReadActiveValue(s32 key);

extern u8 gGameState[];
extern u8 gRom[];
extern struct State08093304 *gWindowWork;

void Ui_SetRenderResultFromObject(s32 arg0)
{
    struct State08093304 *state = gWindowWork;
    u8 value;
    s32 ret;

    if (arg0 == (s32)0x80000000) {
        FIELD(state, s16, RENDER_RESULT_OFS) = (value = 0);
        FIELD(state, s16, RENDER_RESULT_OFS + 2) = value;
    } else {
        ret = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(arg0));
        value = gRom[gGameState[0x20C]];
        FIELD(state, s16, RENDER_RESULT_OFS) = ret;
        FIELD(state, s16, RENDER_RESULT_OFS + 2) = value;
    }
}
