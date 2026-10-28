.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global RenderOutput_CreateLoadedFar
	.thumb_func
RenderOutput_CreateLoadedFar:
	overlay_veneer RenderOutput_CreateLoaded
	.global Func_080382c0
	.thumb_func
Func_080382c0:
	overlay_veneer Func_08043690
	.global Func_080382c8
	.thumb_func
Func_080382c8:
	overlay_veneer Save_ChooseAndLoadFile
	.global Func_080382d0
	.thumb_func
Func_080382d0:
	overlay_veneer Save_CopyFile
	.global Func_080382d8
	.thumb_func
Func_080382d8:
	overlay_veneer Save_EraseSelectedFile
	.global Func_080382e0
	.thumb_func
Func_080382e0:
	overlay_veneer Func_08043534
	.global Func_080382e8
	.thumb_func
Func_080382e8:
	overlay_veneer Save_WriteFile
	.global Func_080382f0
	.thumb_func
Func_080382f0:
	overlay_veneer Func_080432a4
	.global Func_080382f8
	.thumb_func
Func_080382f8:
	overlay_veneer Func_08044558
	.global Resource_CopyFixedBlockBFar
	.thumb_func
Resource_CopyFixedBlockBFar:
	overlay_veneer Resource_CopyFixedBlockB
	.global Resource_CopyFixedBlockAFar
	.thumb_func
Resource_CopyFixedBlockAFar:
	overlay_veneer Resource_CopyFixedBlockA
	.global Func_08038310
	.thumb_func
Func_08038310:
	overlay_veneer Menu_SetNodeCoordinates
	.global Func_08038318
	.thumb_func
Func_08038318:
	overlay_veneer Func_0804e1d8
	.global Func_08038320
	.thumb_func
Func_08038320:
	overlay_veneer Func_0804e3f4
