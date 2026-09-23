.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080aec68, 0x080aec68
	.global Func_080aec04
	.thumb_func
Func_080aec04:
.L_080aec04:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	movs	r2, #44
	adds	r0, r3, #0
	muls	r0, r2
	ldr	r3, [pc, #4]
	adds	r0, r0, r3
	bx	lr
	.2byte 0x2364
	.2byte 0x080b
.L_080aec1c:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	.L_080aec04
	movs	r3, #165
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	ldrh	r0, [r0, #4]
	cmp	r3, #7
	bls.n	.L_080aec3e
	movs	r0, #0
	b.n	.L_080aec46
.L_080aec3e:
	ldrh	r3, [r2, #0]
	asrs	r0, r3
	movs	r3, #1
	ands	r0, r3
.L_080aec46:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080aec68
	cmp	r0, #0
	bne.n	.L_080aec5c
	movs	r0, #1
	b.n	.L_080aec64
.L_080aec5c:
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	.L_080aec1c
.L_080aec64:
	pop	{r5, r6, pc}
	.2byte 0x0000
