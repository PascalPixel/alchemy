#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "TYPES.H"

/* Resident IWRAM routines use fixed entries beyond a direct ROM call's reach. */
#define Iwram_ClearWords ((s32 (*)(void *destination, s32 size))0x03000258) /* IwramClearWords */
#define Iwram_FillWords ((s32 (*)(void *destination, s32 size, u32 value))0x03000260) /* IwramFillWords */
#define Iwram_MulQ16 ((s32 (*)(s32 a, s32 b))0x0300021c) /* IwramMulQ16 */
#define Iwram_RatioMulQ14 ((s32 (*)(s32 divisor, s32 value))0x03000230) /* IwramRatioMulQ14 */
#define Iwram_Sqrt ((s32 (*)(s32 value))0x030002d4) /* IwramSqrt */
#define Iwram_TransformMatrix ((void (*)(const s32 *matrix))0x030003c8) /* IwramTransformMatrix */
#define Iwram_TransformVector ((void (*)(const s32 *vector, s32 *result))0x03000354) /* IwramTransformVector */
#define Iwram_SignedDivide ((s32 (*)(s32 dividend, s32 divisor))0x03000528) /* IwramSignedDivide */
#define Iwram_UnsignedDivide ((u32 (*)(u32 dividend, u32 divisor))0x03000534) /* IwramUnsignedDivide */
#define Iwram_CopyWords ((s32 (*)(void *destination, const void *source, s32 size))0x03000730) /* IwramCopyWords */

#endif
