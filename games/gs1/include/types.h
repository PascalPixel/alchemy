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
#define GameFlag_IsSet Func_080770c0
#define GameFlag_Set Func_080770c8
#define GameFlag_Clear Func_080770d0
#define FixedPoint_Ratio Func_080022ec
#define Object_ResetMotion Func_08009140
#define Object_SetPosition Func_08009150
#define Object_CommitPosition Func_08009158
#define Object_Destroy Func_080090d0
#define Object_SetAction Func_08009088
#define UiWindow_Close Func_08015018
#define Ability_GetData Func_08077080
#define Owner_GetState Func_08077394
#define Resource_FindFreeSlot Func_08004080
#define Runtime_AllocateBlock Func_080048f4
#define Resource_CopyData Func_08003fa4
#define ScheduleCallback Func_08004278
#define Object_InitializeMode Func_08009020
#define Scene_GetRecord Func_0808a080
#define Resource_ResetEntry Func_08003f3c
#define Runtime_AllocateHeapBlock Func_080048b0
#define Resource_GetBuffer Func_080040d0
#define UiPalette_SetColor Func_080150b8
#define Menu_AppendResourceEntry Func_080287a8
#define Object_CheckMovementCollision Func_080091d8
#define Ability_GetMaximum Func_08077240
#define Ability_GetAvailability Func_08077248
#define Shop_GetSelectionState Func_08077020
#define UiText_DrawMessage Func_08015040

#endif
