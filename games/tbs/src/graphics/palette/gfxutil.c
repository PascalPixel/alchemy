#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "project.h"
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "global_cells.h"
#include "transform.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"
#include "fixed_math.h"






void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target)
{
    Graphics_PrepareTransfer(eye, target, gTransform);
}

void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target)
{
    s32 work[12];

    Graphics_PrepareTransfer(eye, target, work);
    Iwram_TransformMatrix(work);
}

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
    gProjection.near = param1;
    gProjection.focal = result;
    gProjection.far = param2;
}

void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    struct Projection *work = &gProjection;

    work->focal = value0;
    work->near = value1;
    work->far = value2;
}

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
    Dma_Set((const void *)Resource_DecodeHalfwordLz, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(const void *, void *))routine)(source, destination);
}

s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: needed to match: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeType01CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))MemAlloc(size);
    Dma_Set((const void *)Resource_DecodeType01Arm, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: needed to match: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: needed to match: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_Decode2CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))MemAlloc(size);
    Dma_Set((const void *)Resource_Decode2, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: needed to match: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeByteLz(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: needed to match: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeByteLzCodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))MemAlloc(size);
    Dma_Set((const void *)Resource_DecodeByteLzArm, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: needed to match: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    MemFree(routine);
    return result;
}

void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleThreeQuartersKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndBrightenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_BrightenPartialKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndDarkenPartialCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_DarkenPartialKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u32, u8 *, u32))routine)(buffer, amount, backup, bytes);
}

void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_ScaleNonzeroThreeQuartersKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}

void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes)
{
    u32 words = (u32)ColorBuffer_BackupAndHalveNonzeroCodeSize >> 2;
    u32 routine[words];
    Dma_Set((const void *)ColorBuffer_HalveNonzeroKernel, routine, words | 0x84000000, REG_DMA3);
    ((void (*)(u8 *, u8 *, u32))routine)(buffer, backup, bytes);
}
