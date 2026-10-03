.syntax unified
	.thumb
	.global Func_08045794
	.thumb_func
Func_08045794:
	push {lr}
	movs r2, #128
	lsls r2, r2, #19
	movs r3, #0
	adds r2, #18
	strh r3, [r2]
	ldr r2, .L_080457ac
	movs r0, #2
	movs r1, #136
	bl Runtime_SetIrqHandler
	pop {pc}
.L_080457ac:
	.4byte Func_08045780
