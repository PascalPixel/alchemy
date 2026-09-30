.syntax unified
	.thumb
	.global Func_0802b700
	.thumb_func
Func_0802b700:
	push {r5, lr}
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r0, r5, #0
	movs r1, #0
	bl Func_0802b738
	pop {r5, pc}
