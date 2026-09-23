.syntax unified
	.thumb
	.global Runtime_RunBusyWaitDelay
	.global Func_080a8904
	.thumb_func
Runtime_RunBusyWaitDelay:
Func_080a8904:
	push	{lr}
	movs	r3, #255
.L_080a8908:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L_080a8908
	pop	{r0}
	bx	r0
	.2byte 0x0000
