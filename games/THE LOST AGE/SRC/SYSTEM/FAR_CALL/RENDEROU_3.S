@ Far-call stub table at 0x080382b8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080382b8
	.thumb_func
Func_080382b8:
	overlay_veneer RenderOutput_CreateLoaded
	.global Func_080382c0
	.thumb_func
Func_080382c0:
	overlay_veneer Func_08043690
	.global Func_080382c8
	.thumb_func
Func_080382c8:
	overlay_veneer Func_08043774
	.global Func_080382d0
	.thumb_func
Func_080382d0:
	overlay_veneer Func_0804380c
	.global Func_080382d8
	.thumb_func
Func_080382d8:
	overlay_veneer Func_080438c8
	.global Func_080382e0
	.thumb_func
Func_080382e0:
	overlay_veneer Func_08043534
	overlay_veneer Func_080434ec
	.global Func_080382f0
	.thumb_func
Func_080382f0:
	overlay_veneer Func_080432a4
	.global Func_080382f8
	.thumb_func
Func_080382f8:
	overlay_veneer Func_08044558
	.global Func_08038300
	.thumb_func
Func_08038300:
	overlay_veneer Func_08044478
	.global Func_08038308
	.thumb_func
Func_08038308:
	overlay_veneer Func_08044488
	overlay_veneer Func_0803e774
	.global Func_08038318
	.thumb_func
Func_08038318:
	overlay_veneer Func_0804e1d8
	.global Func_08038320
	.thumb_func
Func_08038320:
	overlay_veneer Func_0804e3f4
