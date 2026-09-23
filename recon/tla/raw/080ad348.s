.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_080afdbc, 0x080afdbc
	.global Func_080ad348
	.thumb_func
Func_080ad348:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080ad356
	movs	r0, #131
	bl	sub_08016ca4
	b.n	.L_080ad358
.L_080ad356:
	ldr	r0, [pc, #4]
.L_080ad358:
	pop	{pc}
	movs	r0, r0
	.2byte 0x024c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	sub	sp, #4
	bl	sub_080afdbc
	adds	r7, r0, #0
	movs	r6, #0
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L_080ad3a0
	cmp	r6, r7
	bge.n	.L_080ad396
	ldr	r3, [pc, #44]
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r2, r3, r1
	adds	r5, r7, #0
.L_080ad380:
	ldrb	r0, [r2, #0]
	adds	r2, #1
	str	r2, [sp, #0]
	bl	sub_08016ca4
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, r6, r3
	ldr	r2, [sp, #0]
	cmp	r5, #0
	bne.n	.L_080ad380
.L_080ad396:
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08002054
	adds	r6, r0, #0
.L_080ad3a0:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r3, #250
	subs	r0, #8
	lsls	r3, r3, #2
	cmp	r0, r3
	bcc.n	.L_080ad3b6
	movs	r0, #0
.L_080ad3b6:
	movs	r3, #76
	muls	r0, r3
	ldr	r3, [pc, #4]
	adds	r0, r0, r3
	pop	{pc}
	.4byte 0x080b9e7c
