#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TRANSFORM.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"

s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
extern u8 gProjection[];
void Graphics_PrepareTransfer(void *src, void *dst, void *work);

/* camera/scene/set_angle_parameters.c */
struct CameraWork {
    s32 result;
    s32 param1;
    s32 param2;
};

struct Projection {
    s32 focal;
    s32 near;
    s32 far;
    s32 center_x;
    s32 center_y;
};

extern const u8 Resource_DecodeHalfwordLz[];

/* Linker sizes determine the stack allocation for the copied ARM entries. */
extern u8 Resource_DecompressHalfwordsCodeSize[];
extern const u8 Func_08002544[];
extern const u8 Resource_Decode2[];
extern const u8 Resource_DecodeByteLzArm[];

/* Each stream decoder is ARM code that runs from a heap copy of itself; the
   copy lengths are link-time symbols. */
extern u8 Resource_DecodeType01CodeSize[];
extern u8 Resource_Decode2CodeSize[];
extern u8 Resource_DecodeByteLzCodeSize[];

extern const u8 ColorBuffer_BrightenKernel[];
extern const u8 ColorBuffer_DarkenKernel[];
extern const u8 ColorBuffer_ScaleThreeQuartersKernel[];
extern const u8 ColorBuffer_HalveKernel[];
extern const u8 ColorBuffer_BrightenPartialKernel[];
extern const u8 ColorBuffer_DarkenPartialKernel[];
extern const u8 ColorBuffer_ScaleNonzeroThreeQuartersKernel[];
extern const u8 ColorBuffer_HalveNonzeroKernel[];

/* Linker sizes determine the stack allocation for the copied ARM entries. */
extern u8 ColorBuffer_BackupAndBrightenCodeSize[];
extern u8 ColorBuffer_BackupAndDarkenCodeSize[];
extern u8 ColorBuffer_BackupAndScaleThreeQuartersCodeSize[];
extern u8 ColorBuffer_BackupAndHalveCodeSize[];
extern u8 ColorBuffer_BackupAndBrightenPartialCodeSize[];
extern u8 ColorBuffer_BackupAndDarkenPartialCodeSize[];
extern u8 ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize[];
extern u8 ColorBuffer_BackupAndHalveNonzeroCodeSize[];

/* graphics/prepare_transfer_in_iwram_work.c */
/* graphics/prepare_transfer_in_iwram_work.c */
void Graphics_PrepareTransferInIwramWork(s32 src, s32 dst)
{
    Graphics_PrepareTransfer((void *)src, (void *)dst, gTransform);
}

/* graphics/prepare_transfer_and_run.c */
void Graphics_PrepareTransferAndRun(void *src, void *dst)
{
    u8 work[48];

    Graphics_PrepareTransfer(src, dst, work);
    Iwram_TransformMatrix((s32 *)work);
}

/* camera/scene/set_angle_parameters.c */
void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(value + (value >> 31)) >> 1;
    first = Trig_Sin(half);
    result = Iwram_RatioMulQ14(
        first,
        Trig_Cos(half)* 0x50
    );
    ((struct CameraWork *)((u32)&gProjection))->param1 = param1;
    ((struct CameraWork *)((u32)&gProjection))->result = result;
    ((struct CameraWork *)((u32)&gProjection))->param2 = param2;
}

/* camera/scene/store_parameters.c */
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = (u32 *)((u32)&gProjection);

    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}

/* Transforms point through the IWRAM matrix routine and projects it to
   screen x, y and depth; returns the perspective scale, or 0 when the depth
   lies outside the near and far planes. */
s32 Render_ProjectPoint(s32 *point, s32 *screen)
{
    struct Projection *projection;
    s32 depth;
    s32 scale;
    s32 result;

    Iwram_TransformVector(point, screen);
    projection = &gProjection;
    depth = -screen[2];
    result = 0;
    if (depth >= projection->near && depth <= projection->far) {
        screen[2] = depth >> 16;
        if (projection->focal != 0) {
            u32 shifted = (u32)depth >> 11;

            scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
        } else {
            scale = 0x151eb;
        }
        screen[0] = projection->center_x + Iwram_MulQ16(screen[0], scale) / 0x10000;
        screen[1] = projection->center_y - Iwram_MulQ16(screen[1], scale) / 0x10000;
        result = scale;
    }
    return result;
}

void Resource_DecompressHalfwords(const void *source, void *destination)
{
    u32 words = (u32)Resource_DecompressHalfwordsCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)Resource_DecodeHalfwordLz, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(const void *, void *))routine)(source, destination);
}

/* Decodes stream types 0 and 1 (DECODE_01.S). */
s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeType01CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Func_08002544, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

/* Decodes stream type 2 (DECODE_2.S). */
s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_Decode2CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Resource_Decode2, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

/* Decodes byte LZ streams (DECODE_BYTE_LZ.S). */
s32 Resource_DecodeByteLz(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeByteLzCodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Resource_DecodeByteLzArm, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleThreeQuartersKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

/* Words containing four maximum-intensity bytes are left untouched. */
void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenPartialKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

/* Zero words are left untouched. */
void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenPartialKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleNonzeroThreeQuartersKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveNonzeroKernel, routine, words | 0x84000000, (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}
