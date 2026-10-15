#include "types.h"

struct CanvasJob {
    s32 unk00;
    s32 side;
};

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_0815585c(struct CanvasJob *job, s32 flags, s32 *b, s32 *a);
void Func_08143bb8(void);

void Func_08163c00(struct CanvasJob *job)
{
    s32 b[3];
    s32 a[3];

    BattleFx_BeginCanvasLayer(0);
    Func_0815585c(job, ((job->side ^ 1) << 4) | 7, b, a);
    Func_08143bb8();
}
