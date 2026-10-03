#include "AFFINE.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "VRAM_BLOCK.H"
#include "SCENE.H"
#include "CALLBACK_SCHEDULER.H"
#include "STRING.H"
#include "IO_REG.H"
#include "LOW_RUNTIME.H"


/* Each of the 256 render priorities owns a linked-list head. */
extern void *Data_03001400[256];
extern const u8 Render_BuildOamList[];
typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size of the routine copied into the heap. */
extern u8 LoadedRuntime_Size[];

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
        matrix->rows[1] = (u32)reciprocal << 16;
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

/* Each priority has a head; an entry contributes only its first-word link. */
void Runtime_PushSlotEntry(void *slot_entry, s32 slot)
{
    /* FAKEMATCH: the ordinary named-bank index reduces this
       complete native list push from 36 to 32 bytes. Retain its existing
       word-cell address transport; every entry contributes only its link. */
    void *previous_head;
    s32 slot_offset;
    s32 clamped_slot;

    clamped_slot = slot;
    if (clamped_slot > 0xFF) {
        clamped_slot = 0xFF;
    }
    if (clamped_slot < 0) {
        clamped_slot = 0;
    }
    slot_offset = clamped_slot * sizeof(void *);
    previous_head = *(void **)((u8 *)slot_offset + (u32)Data_03001400);
    *(void **)((u8 *)slot_offset + (u32)Data_03001400) = slot_entry;
    *(s32 *)slot_entry = (s32)previous_head;
}

void Runtime_CopyAndCallRoutine(void *argument)
{
    u32 size;
    LoadedRoutine routine;

    /*
     * FAKEMATCH: a loop that runs once around the size load. It is a
     * scheduling barrier: without it the size literal is loaded before the
     * argument is copied to r8.
     */
    do {
        size = (u32)LoadedRuntime_Size;
    } while (0);
    routine = (LoadedRoutine)Runtime_BumpAllocate(size);
    Dma_Set((const void *)Render_BuildOamList, (void *)routine,
            (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    routine(argument);
    Sys_Free((void *)routine);
}
