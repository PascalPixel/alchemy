#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

/* The fixed-width spellings every reconstructed translation unit uses.
   Before this header each of them carried its own copy: 7,751 typedef lines
   across 1,224 of 1,249 files, which is the same eight declarations restated
   once per function. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/* m2c spells a truth value this way and the drafts kept it; every source that
   uses it agreed on the same definition. */
typedef int bool;

#define NULL ((void *)0)

/* Shared runtime names: the address symbols remain the ABI, while callers
   can describe the operation they are performing. */
#define WaitFrames Func_080030f8
#define Audio_PlayCue Func_080f9010
#define Object_SetMode Func_08009080
#define Object_SetCallback Func_08009098
#define Object_Spawn Func_08096c80
#define Object_SetVelocity Func_08096bec
#define Runtime_GetObject Func_08077008
#define Item_GetData Func_08077018
#define Item_FindSlot Func_08077038
#define RotateVectorByMagnitude Func_0800447c
#define NormalizeVector Func_080974d8
#define ScheduleCallbackAfterFrames Func_080041d8
#define Modulo Func_080022fc
#define UiWindow_Clear Func_08015060
#define UiText_DrawMessageAt Func_08015078
#define UiText_DrawAt Func_08015080
#define UiNumber_DrawAt Func_080150b0
#define UiIcon_DrawWithFlags Func_080150c8
#define UiIcon_Draw Func_080152d0
#define UiWindow_Commit Func_08015270
#define UiText_DrawQuantity Func_08015120

#endif
