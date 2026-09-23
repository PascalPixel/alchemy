.syntax unified
	.thumb
	.set sub_0801489c, 0x0801489c
	.set sub_080149e0, 0x080149e0
	.set sub_08016ce4, 0x08016ce4
	.set sub_08022b04, 0x08022b04
	.set sub_08023220, 0x08023220
	.set sub_080233a8, 0x080233a8
	.set sub_080233d0, 0x080233d0
	.set sub_08024738, 0x08024738
	.set sub_08026e60, 0x08026e60
	.set sub_0802dac0, 0x0802dac0
	.set sub_0802db64, 0x0802db64
	.global Func_080279b0
	.thumb_func
Func_080279b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	movs	r1, #0
	adds	r7, r0, #0
	ldr	r3, [pc, #844]
	movs	r0, #2
	str	r1, [sp, #16]
	str	r0, [sp, #4]
	ldr	r1, [pc, #844]
	movs	r2, #143
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080279f0
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #5
	str	r3, [sp, #4]
	b.n	.L_080279fc
.L_080279f0:
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
.L_080279fc:
	ldr	r5, [pc, #796]
	movs	r2, #15
	ldr	r3, [r5, #0]
	ldr	r1, [pc, #796]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r0, [r1, r3]
	lsls	r3, r0, #16
	lsrs	r3, r3, #16
	mov	r8, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	str	r0, [sp, #12]
	cmp	r8, r3
	bne.n	.L_08027a2c
	ldr	r0, [sp, #16]
	movs	r3, #4
	b.n	.L_08027bae
.L_08027a24:
	mov	r1, r9
	asrs	r1, r1, #16
	str	r1, [sp, #12]
	b.n	.L_08027bb2
.L_08027a2c:
	mov	r3, sp
	movs	r2, #0
	adds	r3, #88
	str	r3, [sp, #0]
	str	r2, [sp, #16]
	ldr	r3, [r7, #8]
	ldr	r0, [sp, #0]
	mov	r1, r8
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #12
	ldr	r2, [sp, #0]
	bl	sub_0801489c
	ldr	r3, [pc, #720]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08027a74
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027a66
	b.n	.L_08027bb2
.L_08027a66:
	movs	r0, #100
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08027a74
	b.n	.L_08027bb2
.L_08027a74:
	ldr	r3, [r7, #8]
	add	r1, sp, #76
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	mov	fp, r1
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	str	r3, [r1, #8]
	movs	r1, #168
	lsls	r1, r1, #5
	adds	r1, #85
	add	r1, r8
	lsls	r0, r0, #12
	mov	r2, fp
	bl	sub_0801489c
	ldr	r3, [r7, #8]
	add	r2, sp, #64
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #648]
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	movs	r0, #128
	str	r3, [r2, #8]
	add	r1, r8
	lsls	r0, r0, #12
	mov	r9, r2
	bl	sub_0801489c
	adds	r0, r7, #0
	ldr	r1, [sp, #0]
	bl	sub_0802db64
	mov	r1, fp
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	mov	r1, r9
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	orrs	r5, r6
	orrs	r5, r0
	cmp	r5, #0
	beq.n	.L_08027bb2
	add	r3, sp, #20
	ldr	r1, [sp, #16]
	mov	sl, r3
	movs	r3, #128
	lsls	r3, r3, #5
	add	r3, r8
	mov	r0, sl
	strh	r3, [r0, r1]
	ldr	r3, [pc, #580]
	mov	r2, sl
	add	r3, r8
	strh	r3, [r2, #2]
	movs	r3, #128
	lsls	r3, r3, #6
	add	r3, r8
	strh	r3, [r0, #4]
	ldr	r3, [pc, #568]
	mov	r1, sl
	add	r3, r8
	strh	r3, [r1, #6]
	movs	r3, #192
	lsls	r3, r3, #6
	add	r3, r8
	strh	r3, [r2, #8]
	ldr	r3, [pc, #556]
	movs	r1, #0
	add	r3, r8
	strh	r3, [r0, #10]
	str	r1, [sp, #8]
	mov	r8, r9
.L_08027b12:
	ldr	r2, [sp, #8]
	mov	r0, sl
	lsls	r3, r2, #1
	ldrsh	r2, [r0, r3]
	ldr	r3, [r7, #8]
	lsls	r2, r2, #16
	str	r3, [sp, #88]
	ldr	r3, [r7, #12]
	lsrs	r5, r2, #16
	str	r3, [sp, #92]
	ldr	r3, [r7, #16]
	movs	r0, #128
	str	r3, [sp, #96]
	add	r3, sp, #88
	adds	r1, r5, #0
	lsls	r0, r0, #12
	mov	r9, r2
	adds	r2, r3, #0
	bl	sub_0801489c
	ldr	r3, [r7, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r2, #168
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	lsls	r2, r2, #5
	str	r3, [r0, #8]
	adds	r2, #85
	movs	r0, #128
	adds	r1, r5, r2
	lsls	r0, r0, #12
	mov	r2, fp
	bl	sub_0801489c
	ldr	r3, [r7, #8]
	mov	r0, r8
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	ldr	r1, [pc, #452]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	adds	r5, r5, r1
	str	r3, [r0, #8]
	movs	r0, #128
	adds	r1, r5, #0
	lsls	r0, r0, #12
	mov	r2, r8
	bl	sub_0801489c
	add	r2, sp, #88
	adds	r1, r2, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	mov	r1, fp
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	mov	r1, r8
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_0802db64
	orrs	r5, r6
	orrs	r5, r0
	cmp	r5, #0
	bne.n	.L_08027ba0
	b.n	.L_08027a24
.L_08027ba0:
	ldr	r3, [sp, #8]
	adds	r3, #1
	str	r3, [sp, #8]
	cmp	r3, #6
	blt.n	.L_08027b12
	ldr	r0, [sp, #16]
	movs	r3, #1
.L_08027bae:
	orrs	r0, r3
	str	r0, [sp, #16]
.L_08027bb2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	cmp	r3, #0
	beq.n	.L_08027bdc
	ldr	r1, [sp, #16]
	movs	r2, #3
	ands	r2, r1
	cmp	r2, #0
	beq.n	.L_08027bd4
	movs	r0, #194
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_08027bdc
.L_08027bd4:
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r2, [r3, #0]
.L_08027bdc:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	beq.n	.L_08027bec
	adds	r0, r7, #0
	movs	r1, #9
	bl	sub_080233d0
	b.n	.L_08027bf4
.L_08027bec:
	adds	r0, r7, #0
	ldr	r1, [sp, #4]
	bl	sub_080233d0
.L_08027bf4:
	ldr	r3, [sp, #16]
	cmp	r3, #0
	beq.n	.L_08027c4e
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r0, [sp, #16]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08027c38
	ldr	r1, [sp, #12]
	ldrh	r2, [r7, #6]
	lsls	r3, r1, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L_08027c2c
	adds	r1, r3, #0
.L_08027c2c:
	ldr	r3, [pc, #252]
	cmp	r1, r3
	bge.n	.L_08027c34
	adds	r1, r3, #0
.L_08027c34:
	adds	r3, r2, r1
	strh	r3, [r7, #6]
.L_08027c38:
	movs	r2, #100
	adds	r2, r2, r7
	mov	r8, r2
	movs	r3, #0
	mov	r0, r8
	adds	r2, r7, #0
	strh	r3, [r0, #0]
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_08027ca2
.L_08027c4e:
	add	r3, sp, #88
	ldr	r2, [r3, #4]
	ldr	r1, [r3, #0]
	adds	r0, r7, #0
	ldr	r3, [r3, #8]
	bl	sub_08024738
	ldr	r1, [r7, #36]
	ldr	r6, [pc, #216]
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6af9
	adds	r5, r0, #0
	adds	r0, r1, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r1, [sp, #16]
	str	r1, [r7, #36]
	str	r1, [r7, #44]
	ldr	r2, [sp, #12]
	lsls	r1, r2, #16
	adds	r2, r7, #0
	adds	r2, #36
	lsrs	r1, r1, #16
	bl	sub_0801489c
	movs	r3, #100
	adds	r3, r3, r7
	mov	r8, r3
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_08027ca2
	subs	r3, r2, #1
	mov	r1, r8
	strh	r3, [r1, #0]
.L_08027ca2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #32]
	ldr	r3, [pc, #112]
	ldr	r1, [pc, #144]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	movs	r2, #143
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	ldr	r4, [r1, r3]
	adds	r1, r0, r2
	ldrh	r0, [r1, #0]
	subs	r3, r4, r0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_08027ccc
	adds	r3, #7
.L_08027ccc:
	asrs	r2, r3, #3
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r2, r3
	ble.n	.L_08027cd8
	adds	r2, r3, #0
.L_08027cd8:
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bge.n	.L_08027ce0
	adds	r2, r3, #0
.L_08027ce0:
	adds	r3, r2, #0
	adds	r3, #15
	cmp	r3, #30
	bhi.n	.L_08027cec
	ldrh	r3, [r1, #0]
	subs	r2, r4, r3
.L_08027cec:
	adds	r3, r0, r2
	strh	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #84
	ldrb	r6, [r3, #0]
	cmp	r6, #1
	bne.n	.L_08027dc0
	adds	r0, r7, #0
	adds	r0, #8
	ldr	r5, [r7, #80]
	bl	sub_0802dac0
	cmp	r0, #9
	bne.n	.L_08027d44
	ldr	r3, [r5, #44]
	strb	r6, [r3, #6]
	ldr	r3, [pc, #8]
	strb	r3, [r5, #26]
	b.n	.L_08027d4c
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000000
	.4byte 0x03001150
	.4byte 0x0802ec5c
	.4byte 0x03001238
	.4byte 0xffffeaab
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0x0300021c
	.4byte 0x0802eca0
	.2byte 0xfe00
	.2byte 0xffff
.L_08027d44:
	ldr	r2, [r5, #44]
	movs	r3, #9
	strb	r3, [r2, #6]
	strb	r6, [r5, #26]
.L_08027d4c:
	cmp	r0, #6
	bne.n	.L_08027dc0
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_08027dc0
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L_08027dc0
	movs	r0, #14
	adds	r0, #255
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	bl	sub_08023220
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08027dc0
	ldr	r1, [pc, #136]
	ldr	r6, [r5, #80]
	bl	sub_080233a8
	add	r0, sp, #16
	ldrb	r0, [r0, #0]
	adds	r3, r5, #0
	adds	r3, #85
	adds	r2, r5, #0
	strb	r0, [r3, #0]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	beq.n	.L_08027dba
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_08022b04
	add	r1, sp, #16
	ldrb	r1, [r1, #0]
	movs	r2, #13
	strb	r1, [r6, #26]
	ldrb	r1, [r6, #5]
	negs	r2, r2
	adds	r3, r2, #0
	ands	r3, r1
	movs	r1, #4
	orrs	r3, r1
	strb	r3, [r6, #5]
	ldrb	r3, [r6, #9]
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_08027dba:
	movs	r3, #10
	mov	r2, r8
	strh	r3, [r2, #0]
.L_08027dc0:
	bl	sub_08026e60
	ldr	r3, [pc, #60]
	ldr	r1, [pc, #64]
	movs	r0, #140
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08027de8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #251
	strh	r3, [r2, #0]
.L_08027de8:
	ldrh	r3, [r7, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r7, #4]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0802ec94
	.4byte 0x02000240
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_08026e60
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r5, #4]
	pop	{r5, pc}
	.align 2, 0
