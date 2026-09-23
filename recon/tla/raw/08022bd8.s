.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.global Func_08022bd8
	.thumb_func
Func_08022bd8:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	cmp	r0, #3
	bne.n	.L_08022bf8
	movs	r1, #224
	lsls	r1, r1, #4
	movs	r0, #16
	bl	sub_08014d00
	movs	r1, #192
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #12
	bl	sub_08014d00
	b.n	.L_08022c0e
.L_08022bf8:
	movs	r1, #224
	lsls	r1, r1, #4
	movs	r0, #16
	bl	sub_08014cc0
	movs	r1, #192
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #12
	bl	sub_08014cc0
.L_08022c0e:
	adds	r6, r0, #0
	bl	sub_08014c4c
	movs	r3, #128
	mov	r4, sp
	movs	r5, #0
	lsls	r3, r3, #19
	str	r5, [r4, #0]
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r7, #0
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r5, [r4, #0]
	adds	r0, r4, #0
	adds	r1, r6, #0
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #52]
	movs	r1, #128
	movs	r0, #93
	bl	sub_080142d4
	ldr	r5, [pc, #44]
	movs	r0, #84
	adds	r1, r5, #0
	bl	sub_08014cc0
	movs	r2, #132
	movs	r3, #128
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r1, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #24]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.4byte 0x85000380
	.4byte 0x85000180
	.4byte 0x0802e89c
	.4byte 0x00000080
	.2byte 0x17a4
	.2byte 0x0802
	push	{lr}
	movs	r0, #93
	bl	sub_08014274
	pop	{pc}
	.align 2, 0
