.syntax unified
	.thumb
	.global Func_08040614
	.thumb_func
Func_08040614:
	push {lr}
	ldr r0, .L_08040624
	bl Func_08014644
	movs r0, #208
	bl Runtime_ReleaseHeapBlock
	pop {pc}
.L_08040624:
	.4byte Func_080405ac
