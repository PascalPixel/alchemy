.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014ca0, 0x08014ca0
	.set sub_08014d78, 0x08014d78
	.set sub_08014dac, 0x08014dac
	.set sub_08015778, 0x08015778
	.global Func_08196958
	.thumb_func
Func_08196958:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	mov	sl, r1
	mov	r9, r2
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #36]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	mov	r2, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x000000b0
	.2byte 0xb9a0
	.2byte 0x0813
	push	{r5, r6, r7, lr}
	sub	sp, #16
	adds	r7, r0, #0
	adds	r6, r1, #0
	cmp	r2, #0
	beq.n	.L_081969f4
	add	r5, sp, #4
.L_081969ba:
	movs	r3, #0
	ldrsb	r3, [r7, r3]
	adds	r0, r5, #0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #1
	ldrsb	r3, [r7, r3]
	adds	r1, r5, #0
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	movs	r3, #2
	ldrsb	r3, [r7, r3]
	str	r2, [sp, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	bl	sub_08015778
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #0]
	strh	r3, [r6, #0]
	ldr	r3, [r5, #4]
	subs	r2, #1
	strh	r3, [r6, #2]
	ldr	r3, [r5, #8]
	adds	r7, #4
	strh	r3, [r6, #4]
	adds	r6, #8
	cmp	r2, #0
	bne.n	.L_081969ba
.L_081969f4:
	add	sp, #16
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	lsls	r0, r5, #3
	subs	r0, r0, r5
	lsls	r0, r0, #2
	adds	r0, #28
	bl	sub_08014dac
	adds	r3, r5, #1
	adds	r1, r0, #0
	cmp	r3, #0
	ble.n	.L_08196a24
	movs	r3, #0
	adds	r2, r1, #0
	adds	r0, r5, #1
.L_08196a16:
	subs	r0, #1
	str	r3, [r2, #0]
	str	r3, [r2, #4]
	str	r3, [r2, #24]
	adds	r2, #28
	cmp	r0, #0
	bne.n	.L_08196a16
.L_08196a24:
	adds	r0, r1, #0
	pop	{r5, pc}
.L_08196a28:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldrb	r3, [r5, #0]
	movs	r6, #1
	lsls	r6, r3
	ldrb	r3, [r5, #1]
	adds	r7, r1, #0
	lsls	r6, r3
	bl	sub_08014ca0
	adds	r2, r6, #0
	adds	r2, #8
	cmp	r0, r2
	bcs.n	.L_08196a48
	movs	r0, #0
	b.n	.L_08196a74
.L_08196a48:
	ldrh	r3, [r5, #0]
	adds	r0, r2, #0
	strh	r3, [r7, #0]
	bl	sub_08014d78
	str	r0, [r7, #4]
	movs	r3, #4
	ldr	r1, [r5, #4]
	movs	r5, #3
	ands	r5, r1
	adds	r2, r6, r5
	negs	r3, r3
	adds	r2, #3
	ands	r2, r3
	subs	r1, r1, r5
	ldr	r3, [pc, #16]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x687b
	movs	r0, #1
	adds	r3, r3, r5
	str	r3, [r7, #4]
.L_08196a74:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #0
	sub	sp, #8
	adds	r6, r0, #0
	mov	r8, r3
	movs	r7, #0
	b.n	.L_08196d48
.L_08196a90:
	subs	r3, r1, #1
	cmp	r3, #10
	bls.n	.L_08196a98
	b.n	.L_08196d2c
.L_08196a98:
	ldr	r2, [pc, #724]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08196acc
	.4byte 0x08196b18
	.4byte 0x08196b36
	.4byte 0x08196c0e
	.4byte 0x08196c54
	.4byte 0x08196c9a
	.4byte 0x08196ce0
	.4byte 0x08196b54
	.4byte 0x08196b92
	.4byte 0x08196bd0
	.2byte 0x6afa
	.2byte 0x0819
	cmp	r1, r7
	beq.n	.L_08196af0
	ldr	r5, [pc, #672]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #656]
.L_08196ae8:
	mov	r1, r8
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196af0:
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x6831
	b.n	.L_08196d2c
	cmp	r1, r7
	beq.n	.L_08196af0
	ldr	r5, [pc, #636]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #616]
	b.n	.L_08196ae8
	cmp	r1, r7
	beq.n	.L_08196af0
	ldr	r5, [pc, #612]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #596]
	b.n	.L_08196ae8
	cmp	r1, r7
	beq.n	.L_08196af0
	ldr	r5, [pc, #592]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #572]
	b.n	.L_08196ae8
	cmp	r1, r7
	beq.n	.L_08196b70
	ldr	r0, [pc, #568]
	bl	sub_08014d78
	movs	r3, #128
	mov	r8, r0
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #560]
	mov	r1, r8
	ldr	r2, [pc, #560]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196b70:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196b8c
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196b8e
	str	r5, [r6, #16]
	b.n	.L_08196b8e
.L_08196b8c:
	mov	r5, sp
.L_08196b8e:
	adds	r0, r6, #0
	b.n	.L_08196d24
	cmp	r1, r7
	beq.n	.L_08196bae
	ldr	r0, [pc, #520]
	bl	sub_08014d78
	movs	r3, #128
	mov	r8, r0
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #508]
	mov	r1, r8
	ldr	r2, [pc, #508]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196bae:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196bca
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196bcc
	str	r5, [r6, #16]
	b.n	.L_08196bcc
.L_08196bca:
	mov	r5, sp
.L_08196bcc:
	adds	r0, r6, #0
	b.n	.L_08196d24
	cmp	r1, r7
	beq.n	.L_08196bec
	ldr	r0, [pc, #468]
	bl	sub_08014d78
	movs	r3, #128
	mov	r8, r0
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #460]
	mov	r1, r8
	ldr	r2, [pc, #460]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196bec:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196c08
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196c0a
	str	r5, [r6, #16]
	b.n	.L_08196c0a
.L_08196c08:
	mov	r5, sp
.L_08196c0a:
	adds	r0, r6, #0
	b.n	.L_08196d24
	cmp	r1, r7
	beq.n	.L_08196c32
	ldr	r5, [pc, #420]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #400]
	mov	r1, r8
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196c32:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196c4e
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196c50
	str	r5, [r6, #16]
	b.n	.L_08196c50
.L_08196c4e:
	mov	r5, sp
.L_08196c50:
	adds	r0, r6, #0
	b.n	.L_08196d24
	cmp	r1, r7
	beq.n	.L_08196c78
	ldr	r5, [pc, #356]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #340]
	mov	r1, r8
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196c78:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196c94
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196c96
	str	r5, [r6, #16]
	b.n	.L_08196c96
.L_08196c94:
	mov	r5, sp
.L_08196c96:
	adds	r0, r6, #0
	b.n	.L_08196d24
	cmp	r1, r7
	beq.n	.L_08196cbe
	ldr	r5, [pc, #296]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #276]
	mov	r1, r8
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196cbe:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196cda
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196cdc
	str	r5, [r6, #16]
	b.n	.L_08196cdc
.L_08196cda:
	mov	r5, sp
.L_08196cdc:
	adds	r0, r6, #0
	b.n	.L_08196d24
	cmp	r1, r7
	beq.n	.L_08196d04
	ldr	r5, [pc, #232]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	mov	r8, r0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #216]
	mov	r1, r8
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08196d04:
	ldr	r3, [r6, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08196d20
	mov	r5, sp
	mov	r0, sl
	adds	r1, r5, #0
	bl	.L_08196a28
	cmp	r0, #0
	beq.n	.L_08196d22
	str	r5, [r6, #16]
	b.n	.L_08196d22
.L_08196d20:
	mov	r5, sp
.L_08196d22:
	adds	r0, r6, #0
.L_08196d24:
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x6831
	b.n	.L_08196d2e
.L_08196d2c:
	mov	r5, sp
.L_08196d2e:
	ldr	r3, [r6, #16]
	adds	r7, r1, #0
	cmp	r3, r5
	bne.n	.L_08196d46
	ldr	r0, [r3, #4]
	movs	r3, #4
	negs	r3, r3
	ands	r0, r3
	bl	sub_08013164
	mov	r3, sl
	str	r3, [r6, #16]
.L_08196d46:
	adds	r6, #28
.L_08196d48:
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #0]
	mov	sl, r3
	cmp	r1, r7
	beq.n	.L_08196d60
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_08196d60
	mov	r0, r8
	bl	sub_08013164
	ldr	r1, [r6, #0]
.L_08196d60:
	cmp	r1, #0
	beq.n	.L_08196d66
	b.n	.L_08196a90
.L_08196d66:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x08196aa0
	.4byte 0x00000200
	.4byte 0x08138fc8
	.4byte 0x00000200
	.4byte 0x0813b7a0
	.4byte 0x00000210
	.4byte 0x081391c8
	.4byte 0x00000220
	.4byte 0x081393d8
	.4byte 0x000004b4
	.4byte 0x081395f8
	.4byte 0x8400012d
	.4byte 0x000004d8
	.4byte 0x0813ae14
	.4byte 0x84000136
	.4byte 0x000004b4
	.4byte 0x0813b2ec
	.4byte 0x8400012d
	.4byte 0x000004c4
	.4byte 0x08139aac
	.4byte 0x000004d4
	.4byte 0x08139f70
	.4byte 0x000004e0
	.4byte 0x0813a444
	.4byte 0x000004f0
	.4byte 0x0813a924
