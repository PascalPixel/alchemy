.syntax unified
	.thumb
	.set sub_080c9f2c, 0x080c9f2c
	.set sub_080ca18c, 0x080ca18c
	.set sub_080ca5d8, 0x080ca5d8
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.global Func_080d2b0c
	.thumb_func
Func_080d2b0c:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	adds	r6, r0, #0
	adds	r7, r1, #0
	bl	sub_080ca18c
	movs	r2, #178
	lsls	r2, r2, #1
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	adds	r2, #38
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d2b40
	bl	sub_080cdf5c
	bl	sub_080cad84
	adds	r0, #8
	bl	sub_080c9f2c
.L_080d2b40:
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080ca5d8
	pop	{r5, r6, r7, pc}
	.align 2, 0
