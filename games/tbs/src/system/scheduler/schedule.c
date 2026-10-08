#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "affine.h"
#include "resource.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "dma.h"
#include "mem.h"
#include "vram_block.h"
#include "scene.h"
#include "callback_scheduler.h"
#include "string.h"
#include "io_reg.h"
#include "low_runtime.h"
#include "oam.h"

extern u8 IwramClearWords[];
#define gObjAffineCount (*(u8 (*)[])&gObjAffineCount)

s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
    gObjAffineCount[0] = 0;
    _call_via_r3(((u32)&Data_03001400), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

#undef gObjAffineCount

#define Data_03001400 ((void *(*))&Data_03001400)

typedef void (*OamBuildRoutine)(void *work);

s32 AffineMatrix_BuildForEffect(struct AffineTransform *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    index = gObjAffineCount;
    x_scale = (s16)source->scale_x;
    y_scale = (s16)source->scale_y;
    angle = source->angle;
    if (index > 31)
        return 0;

    matrix = &gObjAffineMatrices[index];
    coefficient = matrix->coefficients;
    if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0) {
        s32 (*divide)(s32, s32);
        s32 reciprocal;
        s32 x_reciprocal;

        divide = Iwram_SignedDivide;
        reciprocal = divide(0x10000, y_scale);
        x_reciprocal = reciprocal;
        if (-x_scale == y_scale)
            x_reciprocal = -reciprocal;

        matrix->rows[0] = (u16)x_reciprocal;
        matrix->rows[1] = reciprocal << 16;
    } else {
        s32 sine;
        s32 cosine;

        sine = Trig_Sin(angle);
        cosine = Trig_Cos(angle);
        *coefficient = cosine / x_scale;
        coefficient++;
        *coefficient = sine / x_scale;
        coefficient++;
        *coefficient = (-sine) / y_scale;
        coefficient++;
        *coefficient = cosine / y_scale;
    }

    gObjAffineCount = index + 1;
    return index;
}

void Runtime_PushSlotEntry(void *slot_entry, s32 slot)
{
    void **head;
    void *previous_head;

    if (slot > 0xFF)
        slot = 0xFF;
    if (slot < 0)
        slot = 0;
    head = &Data_03001400[slot];
    previous_head = *head;
    *head = slot_entry;
    *(void **)slot_entry = previous_head;
}

void Render_BuildOamList(void *work)
{
    u32 size;
    OamBuildRoutine routine;

    /* FAKEMATCH: a loop that runs once around the size load. It is a scheduling barrier: without it the size literal is loaded before the output buffer is copied to r8. */
    do {
        size = (u32)LoadedRuntime_Size;
    } while (0);
    routine = (OamBuildRoutine)MemAlloc(size);
    Dma_Set((const void *)Render_BuildOamListKernel, (void *)routine,
            (size >> 2) | 0x84000000, REG_DMA3);
    routine(work);
    MemFree((void *)routine);
}
