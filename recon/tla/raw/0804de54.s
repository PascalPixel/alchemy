.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_080393fc, 0x080393fc
	.set sub_08042188, 0x08042188
	.global Func_0804de54
	.thumb_func
Func_0804de54:
.L_0804de54:
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r5, r2, #0
	cmp	r1, #5
	bls.n	.L_0804de60
	movs	r1, #5
.L_0804de60:
	movs	r2, #0
	cmp	r1, #0
	beq.n	.L_0804de84
	movs	r6, #15
	mov	r4, sp
.L_0804de6a:
	adds	r3, r0, #0
	ands	r3, r6
	cmp	r3, #9
	bhi.n	.L_0804de76
	adds	r3, #48
	b.n	.L_0804de78
.L_0804de76:
	adds	r3, #55
.L_0804de78:
	strb	r3, [r4, #0]
	adds	r2, #1
	lsrs	r0, r0, #4
	adds	r4, #1
	cmp	r2, r1
	bne.n	.L_0804de6a
.L_0804de84:
	subs	r2, r1, #1
	cmp	r2, #0
	blt.n	.L_0804de9c
	mov	r3, sp
	adds	r1, r2, r3
	mov	ip, r3
.L_0804de90:
	ldrb	r3, [r1, #0]
	subs	r1, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	cmp	r1, ip
	bge.n	.L_0804de90
.L_0804de9c:
	add	sp, #8
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r0
	sub	sp, #36
	adds	r5, r1, #0
	bl	sub_080393fc
	mov	r1, sl
	movs	r2, #48
	movs	r3, #0
	ldr	r0, [pc, #156]
	bl	sub_08042188
	add	r2, sp, #8
	mov	r8, r2
	mov	r2, sp
	adds	r2, #33
	movs	r1, #0
	movs	r3, #28
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	add	r3, sp
	movs	r1, #16
	lsls	r7, r5, #8
	mov	fp, r3
	mov	r9, r1
.L_0804dede:
	mov	r3, fp
.L_0804dee0:
	movs	r1, #0
	strb	r1, [r3, #0]
	ldr	r2, [sp, #0]
	adds	r3, #1
	cmp	r3, r2
	bne.n	.L_0804dee0
	adds	r0, r7, #0
	movs	r1, #3
	mov	r2, fp
	bl	.L_0804de54
	mov	r0, fp
	mov	r1, sl
	movs	r2, #0
	mov	r3, r9
	bl	sub_08042188
	ldr	r0, [pc, #92]
	mov	r1, sl
	movs	r2, #32
	mov	r3, r9
	bl	sub_08042188
	mov	r6, r8
	mov	r5, r8
	adds	r6, #15
.L_0804df14:
	adds	r0, r7, #0
	bl	sub_08016ce4
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	adds	r3, #48
	strb	r3, [r5, #0]
	adds	r5, #1
	adds	r7, #1
	cmp	r5, r6
	ble.n	.L_0804df14
	movs	r3, #16
	movs	r2, #0
	mov	r1, r8
	strb	r2, [r1, r3]
	mov	r0, r8
	mov	r3, r9
	mov	r1, sl
	movs	r2, #48
	bl	sub_08042188
	ldr	r1, [sp, #4]
	movs	r3, #8
	adds	r1, #1
	add	r9, r3
	str	r1, [sp, #4]
	cmp	r1, #16
	bne.n	.L_0804dede
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0805f8dc
	.2byte 0xf8d8
	.2byte 0x0805
	push	{r5, r6, lr}
	ldr	r6, [pc, #308]
	adds	r5, r2, #0
	ldr	r3, [r6, #12]
	movs	r2, #1
	ands	r3, r2
	adds	r4, r5, #4
	cmp	r3, #0
	beq.n	.L_0804df9e
	ldr	r3, [r1, #0]
	ldr	r2, [r4, #0]
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	lsls	r3, r3, #4
	adds	r5, r3, r2
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804df96
	adds	r0, r5, #0
	bl	sub_08016d18
	b.n	.L_0804e092
.L_0804df96:
	adds	r0, r5, #0
	bl	sub_08016cfc
	b.n	.L_0804e092
.L_0804df9e:
	ldr	r3, [r6, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804dfb2
	ldr	r3, [r6, #12]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804dfb8
.L_0804dfb2:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0804e098
.L_0804dfb8:
	ldr	r0, [r6, #12]
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0804dfd2
	ldr	r3, [r4, #0]
	subs	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_0804e096
	movs	r3, #15
	str	r3, [r4, #0]
	b.n	.L_0804e096
.L_0804dfd2:
	ldr	r3, [r6, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804dfea
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #15
	ble.n	.L_0804e096
	str	r0, [r4, #0]
	b.n	.L_0804e096
.L_0804dfea:
	ldr	r0, [r6, #12]
	movs	r3, #32
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0804e004
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_0804e096
	movs	r3, #15
	str	r3, [r5, #0]
	b.n	.L_0804e096
.L_0804e004:
	ldr	r3, [r6, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e01c
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, #15
	ble.n	.L_0804e096
	str	r0, [r5, #0]
	b.n	.L_0804e096
.L_0804e01c:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e038
	ldr	r3, [r6, #12]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e038
	ldr	r3, [r1, #0]
	subs	r3, #10
	b.n	.L_0804e06e
.L_0804e038:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e05e
	ldr	r3, [r6, #12]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e05e
	ldr	r3, [r1, #0]
	adds	r3, #10
	str	r3, [r1, #0]
	cmp	r3, #15
	ble.n	.L_0804e092
	movs	r3, #0
	str	r3, [r1, #0]
	b.n	.L_0804e092
.L_0804e05e:
	ldr	r0, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0804e07a
	ldr	r3, [r1, #0]
	subs	r3, #1
.L_0804e06e:
	str	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_0804e092
	movs	r3, #15
	str	r3, [r1, #0]
	b.n	.L_0804e092
.L_0804e07a:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e096
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #15
	ble.n	.L_0804e092
	str	r0, [r1, #0]
.L_0804e092:
	movs	r0, #1
	b.n	.L_0804e098
.L_0804e096:
	movs	r0, #0
.L_0804e098:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03001150
