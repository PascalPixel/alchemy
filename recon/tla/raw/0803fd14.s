.syntax unified
	.thumb
	.global Func_0803fd14
	.thumb_func
Func_0803fd14:
	push {lr}
	ldr r0, .L_0803fd24
	bl Func_08014644
	movs r0, #208
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_0803fd24:
	.4byte Func_0803fb60
