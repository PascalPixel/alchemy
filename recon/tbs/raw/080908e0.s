.syntax unified
	.thumb
	.set sub_080072f0, 0x080072f0
	.set sub_080770c0, 0x080770c0
	.global Func_080908e0
	.thumb_func
Func_080908e0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	movs	r1, #196
	ldr	r6, [r3, #0]
	movs	r0, #169
	lsls	r1, r1, #5
	lsls	r0, r0, #1
	adds	r5, r6, r1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080908fa
	b.n	.L_08090a44
.L_080908fa:
	ldr	r2, [pc, #68]
	adds	r1, r6, r2
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_08090908
	b.n	.L_08090a44
.L_08090908:
	adds	r2, #1
	adds	r3, r6, r2
	ldrb	r2, [r3, #0]
	adds	r2, #1
	strb	r2, [r3, #0]
	lsls	r2, r2, #24
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	asrs	r2, r2, #24
	cmp	r2, r3
	bge.n	.L_08090948
	movs	r3, #224
	lsls	r3, r3, #2
	ldr	r4, [pc, #32]
	adds	r1, r6, r3
	movs	r0, #0
.L_08090928:
	ldrh	r3, [r1, #0]
	ldrh	r2, [r5, #0]
	adds	r0, #1
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	adds	r5, #2
	adds	r1, #2
	cmp	r0, r4
	ble.n	.L_08090928
	b.n	.L_08090966
	.4byte 0x03001ed0
	.4byte 0x00002a01
	.2byte 0x053f
	.2byte 0x0000
.L_08090948:
	movs	r1, #224
	movs	r2, #224
	lsls	r1, r1, #2
	lsls	r2, r2, #4
	adds	r0, r6, r1
	adds	r1, r6, r2
	movs	r2, #168
	ldr	r3, [pc, #72]
	lsls	r2, r2, #4
	bl	sub_080072f0
	ldr	r3, [pc, #68]
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_08090966:
	movs	r1, #168
	lsls	r1, r1, #6
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #7
	movs	r2, #140
	adds	r3, r6, r3
	lsls	r2, r2, #6
	adds	r4, r3, r2
	movs	r3, #248
	movs	r2, #224
	lsls	r3, r3, #7
	movs	r0, #224
	lsls	r2, r2, #2
	ldr	r7, [pc, #12]
	ldr	r5, [pc, #12]
	mov	ip, r3
	lsls	r0, r0, #1
	adds	r1, r6, r2
	b.n	.L_080909a8
	movs	r0, r0
	.4byte 0x000003e0
	.4byte 0x0000001f
	.4byte 0x03001388
	.2byte 0x2a01
	.2byte 0x0000
.L_080909a8:
	ldrh	r3, [r1, #0]
	mov	r2, ip
	ands	r2, r3
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #16
	asrs	r3, r3, #21
	ands	r3, r7
	orrs	r2, r3
	ldrh	r3, [r1, #4]
	lsls	r3, r3, #16
	asrs	r3, r3, #26
	ands	r3, r5
	orrs	r2, r3
	subs	r0, #1
	strh	r2, [r4, #0]
	adds	r1, #6
	adds	r4, #2
	cmp	r0, #0
	bne.n	.L_080909a8
	movs	r3, #168
	lsls	r3, r3, #6
	adds	r1, r6, r3
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	ldrb	r2, [r1, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #7
	movs	r1, #140
	adds	r0, r6, r3
	lsls	r1, r1, #6
	ldr	r5, [pc, #96]
	adds	r6, r0, r1
	ldr	r4, [pc, #96]
	ldrh	r3, [r4, #0]
	adds	r1, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_08090a16
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r5, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	ldr	r2, [pc, #64]
	str	r2, [r3, #0]
.L_08090a16:
	strh	r1, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_08090a42
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r1, #147
	adds	r2, #1
	adds	r3, r3, r5
	lsls	r1, r1, #6
	adds	r3, #4
	strh	r2, [r5, #0]
	adds	r2, r0, r1
	stmia	r3!, {r2}
	ldr	r2, [pc, #28]
	stmia	r3!, {r2}
	ldr	r2, [pc, #20]
	str	r2, [r3, #0]
.L_08090a42:
	strh	r6, [r4, #0]
.L_08090a44:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x84000070
	.4byte 0x05000200
