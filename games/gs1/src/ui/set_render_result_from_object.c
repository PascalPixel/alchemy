#include "types.h"
#include "scene.h"
#include "gs1_edition.h"

struct State08093304 {
    u8 padding[0x12F4];
    s16 ret;
    s16 value;
};

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 ObjectTable_ReadActiveValue(s32 key);

extern u8 gCell[];
extern u8 gRom[];
extern struct State08093304 *gIw;

void Ui_SetRenderResultFromObject(s32 arg0)
{
    struct State08093304 *state = gIw;
    u8 value;
    s32 ret;

    if (arg0 == (s32)0x80000000) {
        FIELD(state, s16, RENDER_RESULT_OFS) = (value = 0);
        FIELD(state, s16, RENDER_RESULT_OFS + 2) = value;
    } else {
        ret = Ui_Check(ObjectTable_ReadActiveValue(arg0));
        value = gRom[gCell[0x20C]];
        FIELD(state, s16, RENDER_RESULT_OFS) = ret;
        FIELD(state, s16, RENDER_RESULT_OFS + 2) = value;
    }
}
