.syntax unified
	.thumb
	.global Func_081a81c0
	.thumb_func
Func_081a81c0:
	push {lr}
	ldr r0, .L_081a81d0
	bl Func_08014644
	movs r0, #128
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_081a81d0:
	.4byte Func_081a78c0
