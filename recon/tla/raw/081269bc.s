.syntax unified
	.thumb
	.global Func_081269bc
	.thumb_func
Func_081269bc:
	push	{lr}
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r6, [r5, #0]
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	ldr	r3, [r5, #0]
	adds	r6, r6, r3
	str	r6, [r5, #4]
	pop	{r5, r6, pc}
