#include "palette_scene.h"

struct PaletteEffectFrame {
    s32 pad00[6];
    s32 progress;
    s32 pad1c[7];
    s32 limit;
    s32 second_limit;
    s32 third_limit;
};

extern void Func_02002b52(struct PaletteEffectFrame *frame);

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Func_02002b52(frame);
            }
        }
    }
    return 1;
}
