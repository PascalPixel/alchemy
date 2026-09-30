@ Far-call stub table after the cartridge header at 0x080000c0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global WaitFramesFar
	.thumb_func
WaitFramesFar:
	overlay_veneer WaitFrames
	overlay_veneer Func_080144c0
	.global Func_080000d0
	.thumb_func
Func_080000d0:
	overlay_veneer Func_080145a8
	.global Func_080000d8
	.thumb_func
Func_080000d8:
	overlay_veneer Func_08014644
	.global Func_080000e0
	.thumb_func
Func_080000e0:
	overlay_veneer Func_08014694
	.global Func_080000e8
	.thumb_func
Func_080000e8:
	overlay_veneer Func_0801475c
	overlay_veneer Func_080147d8
	.global Func_080000f8
	.thumb_func
Func_080000f8:
	overlay_veneer Func_08014878
	.global Func_08000100
	.thumb_func
Func_08000100:
	overlay_veneer Func_080148e8
	overlay_veneer Func_080149b4
	overlay_veneer Func_080149e0
	.global Math_SineFar
	.thumb_func
Math_SineFar:
	overlay_veneer Math_Sine
	.global Math_CosineFar
	.thumb_func
Math_CosineFar:
	overlay_veneer Math_Cosine
	.global Func_08000128
	.thumb_func
Func_08000128:
	overlay_veneer Func_0801489c
	.global Func_08000130
	.thumb_func
Func_08000130:
	overlay_veneer Func_08013438
	overlay_veneer Func_08014c6c
	.global Func_08000140
	.thumb_func
Func_08000140:
	overlay_veneer Func_08014cc0
	.global Runtime_AllocateBlockFar
	.thumb_func
Runtime_AllocateBlockFar:
	overlay_veneer Runtime_AllocateBlock
	.global Runtime_ReleaseHeapBlockFar
	.thumb_func
Runtime_ReleaseHeapBlockFar:
	overlay_veneer Runtime_ReleaseHeapBlock
	overlay_veneer Func_08014ca0
	overlay_veneer Func_08014cb0
	.global Runtime_BumpAllocateFar
	.thumb_func
Runtime_BumpAllocateFar:
	overlay_veneer Runtime_BumpAllocate
	.global Func_08000170
	.thumb_func
Func_08000170:
	overlay_veneer Func_08014dac
	.global Sys_FreeFar
	.thumb_func
Sys_FreeFar:
	overlay_veneer Sys_Free
	overlay_veneer Func_08014ae0
	overlay_veneer Func_08014b30
	overlay_veneer Func_08014b50
	overlay_veneer Func_08014aa8
	.global Func_080001a0
	.thumb_func
Func_080001a0:
	overlay_veneer Func_0801591c
	.global Func_080001a8
	.thumb_func
Func_080001a8:
	overlay_veneer Func_0801587c
	overlay_veneer Func_08014368
	.global Func_080001b8
	.thumb_func
Func_080001b8:
	overlay_veneer Func_08014274
	.global Func_080001c0
	.thumb_func
Func_080001c0:
	overlay_veneer Func_080142ac
	.global VramBlock_LoadCachedFar
	.thumb_func
VramBlock_LoadCachedFar:
	overlay_veneer VramBlock_LoadCached
	.global Resource_FindFreeEntryFar
	.thumb_func
Resource_FindFreeEntryFar:
	overlay_veneer Resource_FindFreeEntry
	.global Func_080001d8
	.thumb_func
Func_080001d8:
	overlay_veneer Func_08014b70
	.global Func_080001e0
	.thumb_func
Func_080001e0:
	overlay_veneer Func_0801401c
	.global Func_080001e8
	.thumb_func
Func_080001e8:
	overlay_veneer Func_080140d8
	.global Func_080001f0
	.thumb_func
Func_080001f0:
	overlay_veneer Func_08014bac
	overlay_veneer Func_08014c4c
	overlay_veneer Func_08014de4
	overlay_veneer Func_08014ee0
	overlay_veneer Func_08014e38
	overlay_veneer Func_08014ea8
	overlay_veneer Func_08014efc
	overlay_veneer Func_08015024
	overlay_veneer Func_08015068
	overlay_veneer Func_080150e4
	overlay_veneer Func_08015128
	overlay_veneer Func_080151ac
	overlay_veneer Func_0801524c
	overlay_veneer Func_08015384
	overlay_veneer Func_080156e8
	.global Func_08000268
	.thumb_func
Func_08000268:
	overlay_veneer Func_08015778
	overlay_veneer Func_08015714
	overlay_veneer Camera_StoreSceneParameters
	overlay_veneer Func_080132d0
	overlay_veneer Runtime_ConstantZeroResult
	.global Resource_GetTableEntryFar
	.thumb_func
Resource_GetTableEntryFar:
	overlay_veneer Resource_GetTableEntry
	.global Func_08000298
	.thumb_func
Func_08000298:
	overlay_veneer Func_080132fc
	overlay_veneer Func_0801336c
	.global Func_080002a8
	.thumb_func
Func_080002a8:
	overlay_veneer Func_08013e70
	overlay_veneer Func_08013eb4
	.global Func_080002b8
	.thumb_func
Func_080002b8:
	overlay_veneer Func_08013ef8
	.global Func_080002c0
	.thumb_func
Func_080002c0:
	overlay_veneer Func_08013f3c
	overlay_veneer Func_08013f80
	.global Func_080002d0
	.thumb_func
Func_080002d0:
	overlay_veneer Func_08013fdc
	overlay_veneer Func_080132b8
	overlay_veneer Func_080132bc
	overlay_veneer Func_080132c0
	overlay_veneer Func_080132c4
	overlay_veneer Func_080132c8
	.global Func_08000300
	.thumb_func
Func_08000300:
	overlay_veneer Func_08016180
	overlay_veneer Func_08016348
	overlay_veneer Func_08013b68
	.global Func_08000318
	.thumb_func
Func_08000318:
	overlay_veneer Func_08013ba4
	overlay_veneer Func_08013be0
	overlay_veneer Func_08013c1c
	overlay_veneer Func_08013c58
	overlay_veneer Func_08013c94
	overlay_veneer Func_08013cd0
	overlay_veneer Func_08013d0c
	overlay_veneer Func_08013d48
	overlay_veneer Func_080138a8
	overlay_veneer Func_080156f8
	overlay_veneer Func_080167d8
	.global Func_08000370
	.thumb_func
Func_08000370:
	overlay_veneer Func_080167ac
	.global Func_08000378
	.thumb_func
Func_08000378:
	overlay_veneer Func_08013b30
	.global Func_08000380
	.thumb_func
Func_08000380:
	overlay_veneer Func_0801680c
	.global Func_08000388
	.thumb_func
Func_08000388:
	overlay_veneer Func_08016854
	overlay_veneer Func_080168a0
	overlay_veneer Func_080168cc
	overlay_veneer Func_080168f8
	.global Func_080003a8
	.thumb_func
Func_080003a8:
	overlay_veneer Func_0801692c
	overlay_veneer Func_08016990
	.global Func_080003b8
	.thumb_func
Func_080003b8:
	overlay_veneer Func_0801471c
	.global Owner_GetStateFar
	.thumb_func
Owner_GetStateFar:
	overlay_veneer Owner_GetState
	.global Func_080003c8
	.thumb_func
Func_080003c8:
	overlay_veneer GameFlag_TestFar
	.global Func_080003d0
	.thumb_func
Func_080003d0:
	overlay_veneer GameFlag_SetBitFar
	.global Func_080003d8
	.thumb_func
Func_080003d8:
	overlay_veneer GameFlag_ClearBitFar
	overlay_veneer Func_08016d34
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer GameFlag_GetByte
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer GameFlag_SetByte
	overlay_veneer Func_08016d7c
	overlay_veneer Func_08016d98
	overlay_veneer GameFlag_GetNibble
	overlay_veneer Func_08016dd0
	.global Func_08000418
	.thumb_func
Func_08000418:
	overlay_veneer Func_0801456c
	overlay_veneer Func_08014220
