.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_08015830, 0x08015830
	.global Func_080cc9fc
	.thumb_func
Func_080cc9fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	movs	r2, #1
	mov	r9, r1
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #36]
	sub	sp, #12
	mov	sl, r2
	mov	r8, r2
	bl	sub_08015830
	movs	r3, #0
	str	r3, [sp, #8]
	mov	fp, r3
	b.n	.L_080cca48
.L_080cca26:
	mov	r1, r9
	mov	r2, r9
	lsls	r1, r1, #12
	lsls	r2, r2, #5
	str	r1, [sp, #8]
	mov	fp, r2
	b.n	.L_080cca48
	.4byte 0x080efd70
	.2byte 0x1a00
	.2byte 0x0600
.L_080cca3c:
	mov	r3, r9
	lsls	r3, r3, #12
	mov	r1, r9
	str	r3, [sp, #8]
	lsls	r1, r1, #5
.L_080cca46:
	mov	fp, r1
.L_080cca48:
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #72]
	add	r3, r9
	ldr	r0, [sp, #8]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	adds	r2, r1, #0
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	movs	r7, #160
	adds	r2, #64
	lsls	r7, r7, #19
	ldr	r5, [pc, #28]
	ldr	r4, [pc, #32]
	adds	r7, #2
	strh	r3, [r2, #0]
	movs	r2, #1
	adds	r0, #209
	add	r7, fp
	adds	r1, #2
	mov	ip, r2
	b.n	.L_080cca98
	.4byte 0x0000f052
	.4byte 0x0000f047
	.4byte 0x0000f042
	.4byte 0x0000001f
	.4byte 0x0000f0e0
	.4byte 0xfffff0e0
	.2byte 0x205a
	.2byte 0x0600
.L_080cca98:
	strh	r0, [r1, #0]
	adds	r2, r1, #0
	ldrh	r6, [r7, #0]
	adds	r2, #64
	adds	r3, r6, #0
	ands	r3, r5
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	lsrs	r3, r6, #5
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	lsrs	r3, r6, #10
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	movs	r3, #1
	add	ip, r3
	mov	r2, ip
	adds	r0, #1
	adds	r7, #2
	adds	r1, #2
	cmp	r2, #15
	bls.n	.L_080cca98
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #360]
	movs	r4, #31
.L_080ccad6:
	ldr	r2, [r1, #12]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccaee
	subs	r3, #65
	add	sl, r3
	mov	r2, sl
	cmp	r2, #0
	bgt.n	.L_080ccaee
	movs	r3, #3
	mov	sl, r3
.L_080ccaee:
	ldr	r2, [r1, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccb04
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #3
	ble.n	.L_080ccb04
	mov	sl, r2
.L_080ccb04:
	ldr	r2, [r1, #12]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccb1e
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bgt.n	.L_080ccb1e
	movs	r2, #15
	mov	r8, r2
.L_080ccb1e:
	ldr	r2, [r1, #12]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccb34
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	cmp	r2, #15
	ble.n	.L_080ccb34
	mov	r8, r3
.L_080ccb34:
	ldr	r2, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccb5e
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L_080ccb4e
	b.n	.L_080cca26
.L_080ccb4e:
	movs	r2, #13
	mov	r9, r2
	mov	r3, r9
	mov	r1, r9
	lsls	r3, r3, #12
	lsls	r1, r1, #5
	str	r3, [sp, #8]
	b.n	.L_080cca46
.L_080ccb5e:
	ldr	r2, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccb7e
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #13
	bgt.n	.L_080ccb76
	b.n	.L_080cca3c
.L_080ccb76:
	movs	r1, #0
	mov	r9, r1
	str	r1, [sp, #8]
	b.n	.L_080cca46
.L_080ccb7e:
	ldr	r2, [r1, #12]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccbca
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r1, #160
	lsls	r3, r3, #1
	lsls	r1, r1, #19
	adds	r7, r3, r1
	ldrh	r6, [r7, #0]
	mov	r1, sl
	adds	r5, r6, #0
	lsrs	r2, r6, #5
	lsrs	r3, r6, #10
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #1
	bne.n	.L_080ccbb0
	cmp	r5, #30
	bhi.n	.L_080ccbb0
	adds	r5, #1
.L_080ccbb0:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_080ccbbc
	cmp	r2, #30
	bhi.n	.L_080ccbbc
	adds	r2, #1
.L_080ccbbc:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_080ccc14
	cmp	r3, #30
	bhi.n	.L_080ccc14
	adds	r3, #1
	b.n	.L_080ccc14
.L_080ccbca:
	ldr	r2, [r1, #12]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccc20
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r1, #160
	lsls	r3, r3, #1
	lsls	r1, r1, #19
	adds	r7, r3, r1
	ldrh	r6, [r7, #0]
	mov	r1, sl
	adds	r5, r6, #0
	lsrs	r2, r6, #5
	lsrs	r3, r6, #10
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #1
	bne.n	.L_080ccbfc
	cmp	r5, #0
	beq.n	.L_080ccbfc
	subs	r5, #1
.L_080ccbfc:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_080ccc08
	cmp	r2, #0
	beq.n	.L_080ccc08
	subs	r2, #1
.L_080ccc08:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_080ccc14
	cmp	r3, #0
	beq.n	.L_080ccc14
	subs	r3, #1
.L_080ccc14:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r5
	strh	r3, [r7, #0]
	b.n	.L_080cca48
.L_080ccc20:
	ldr	r2, [r1, #12]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ccc88
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r2, #160
	lsls	r3, r3, #1
	lsls	r2, r2, #19
	adds	r7, r3, r2
	ldrh	r6, [r7, #0]
	b.n	.L_080ccc6c
	.2byte 0x1150
	.2byte 0x0300
.L_080ccc40:
	cmp	r5, #0
	bne.n	.L_080ccc48
	ldr	r3, [pc, #28]
	strh	r3, [r7, #0]
.L_080ccc48:
	cmp	r5, #10
	bne.n	.L_080ccc4e
	strh	r6, [r7, #0]
.L_080ccc4e:
	cmp	r5, #20
	bne.n	.L_080ccc56
	ldr	r3, [pc, #20]
	strh	r3, [r7, #0]
.L_080ccc56:
	cmp	r5, #30
	bne.n	.L_080ccc5c
	strh	r6, [r7, #0]
.L_080ccc5c:
	adds	r5, #1
	cmp	r5, #39
	bls.n	.L_080ccc6e
	b.n	.L_080ccc6c
	.4byte 0x00007fff
	.2byte 0x0000
	.2byte 0x0000
.L_080ccc6c:
	movs	r5, #0
.L_080ccc6e:
	movs	r0, #1
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08013560
	ldr	r1, [sp, #4]
	movs	r3, #8
	ldr	r2, [r1, #0]
	ldr	r4, [sp, #0]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ccc40
	strh	r6, [r7, #0]
.L_080ccc88:
	ldr	r2, [r1, #12]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ccca2
	movs	r0, #1
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08013560
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	b.n	.L_080ccad6
.L_080ccca2:
	bl	sub_08014bac
	bl	sub_08014b70
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #1
	ldr	r7, [r3, #108]
	movs	r1, #0
	negs	r2, r2
	adds	r4, r0, #0
	movs	r6, #8
	mov	ip, r1
	mov	lr, r2
.L_080cccce:
	ldmia	r7!, {r0}
	cmp	r0, #0
	beq.n	.L_080ccd2a
	cmp	r4, #7
	bgt.n	.L_080cccfc
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldrh	r2, [r0, #0]
	cmp	r3, lr
	beq.n	.L_080ccd2a
	movs	r1, #1
	negs	r1, r1
.L_080ccce6:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, r4
	beq.n	.L_080ccd36
	adds	r0, #24
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldrh	r2, [r0, #0]
	cmp	r3, r1
	bne.n	.L_080ccce6
	b.n	.L_080ccd2a
.L_080cccfc:
	ldrh	r2, [r0, #0]
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	adds	r1, r2, #0
	cmp	r3, lr
	beq.n	.L_080ccd2a
	movs	r5, #1
	negs	r5, r5
.L_080ccd0c:
	lsls	r3, r1, #16
	movs	r1, #224
	lsls	r1, r1, #11
	cmp	r3, r1
	ble.n	.L_080ccd1c
	cmp	r6, r4
	beq.n	.L_080ccd36
	adds	r6, #1
.L_080ccd1c:
	adds	r0, #24
	ldrh	r2, [r0, #0]
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	adds	r1, r2, #0
	cmp	r3, r5
	bne.n	.L_080ccd0c
.L_080ccd2a:
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	cmp	r3, #3
	ble.n	.L_080cccce
	ldrh	r2, [r0, #0]
.L_080ccd36:
	lsls	r3, r2, #16
	movs	r5, #1
	asrs	r3, r3, #16
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_080ccd44
	movs	r0, #0
.L_080ccd44:
	pop	{r5, r6, r7, pc}
	.align 2, 0
