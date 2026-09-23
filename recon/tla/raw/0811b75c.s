.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020000, 0x08020000
	.set sub_08020040, 0x08020040
	.set sub_08020058, 0x08020058
	.set sub_08020080, 0x08020080
	.set sub_0811b4ac, 0x0811b4ac
	.set sub_0811b4d8, 0x0811b4d8
	.set sub_0811b6a4, 0x0811b6a4
	.set sub_0811b724, 0x0811b724
	.set sub_0811be3c, 0x0811be3c
	.global Func_0811b75c
	.thumb_func
Func_0811b75c:
.L_0811b75c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	cmp	r2, #0
	beq.n	.L_0811b79e
	movs	r1, #0
	mov	fp, r1
.L_0811b778:
	ldr	r0, [sp, #16]
	mov	r1, fp
	bl	sub_0811b4ac
	cmp	r0, #0
	bne.n	.L_0811b794
	mov	r0, fp
	mov	r2, fp
	adds	r0, #120
	cmp	r2, #7
	bgt.n	.L_0811b790
	mov	r0, fp
.L_0811b790:
	bl	sub_0811b724
.L_0811b794:
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	cmp	r1, #13
	ble.n	.L_0811b778
.L_0811b79e:
	ldr	r3, [pc, #148]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0811b7ac
	movs	r0, #1
	bl	sub_08013560
.L_0811b7ac:
	ldr	r3, [sp, #16]
	movs	r2, #0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	mov	fp, r2
	str	r3, [sp, #8]
	cmp	r3, #255
	bne.n	.L_0811b7be
	b.n	.L_0811b970
.L_0811b7be:
	ldr	r1, [sp, #16]
	str	r1, [sp, #4]
.L_0811b7c2:
	ldr	r2, [sp, #8]
	cmp	r2, #254
	bne.n	.L_0811b7ca
	b.n	.L_0811b954
.L_0811b7ca:
	adds	r0, r2, #0
	bl	sub_0811be3c
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_0811b7d8
	b.n	.L_0811b954
.L_0811b7d8:
	adds	r3, r7, #0
	adds	r3, #42
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b7e4
	b.n	.L_0811b954
.L_0811b7e4:
	ldr	r0, [sp, #8]
	adds	r1, r7, #0
	bl	sub_0811b4d8
	ldr	r3, [r7, #0]
	mov	r8, r3
	cmp	r3, #0
	bne.n	.L_0811b7f6
	b.n	.L_0811b954
.L_0811b7f6:
	mov	r1, r8
	adds	r1, #84
	ldrb	r2, [r1, #0]
	mov	r9, r2
	cmp	r2, #0
	beq.n	.L_0811b804
	b.n	.L_0811b954
.L_0811b804:
	ldrh	r2, [r7, #4]
	ldr	r3, [pc, #40]
	ands	r3, r2
	movs	r2, #153
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0811b844
	adds	r2, #70
	cmp	r3, r2
	beq.n	.L_0811b844
	adds	r2, #7
	cmp	r3, r2
	beq.n	.L_0811b844
	subs	r2, #53
	cmp	r3, r2
	beq.n	.L_0811b844
	subs	r2, #6
	cmp	r3, r2
	beq.n	.L_0811b844
	b.n	.L_0811b838
	movs	r0, r0
	.4byte 0x00000fff
	.2byte 0x1108
	.2byte 0x0300
.L_0811b838:
	adds	r2, #2
	cmp	r3, r2
	beq.n	.L_0811b844
	adds	r2, #2
	cmp	r3, r2
	bne.n	.L_0811b8c8
.L_0811b844:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #24]
	ldrh	r5, [r7, #4]
	ldr	r3, [r2, #24]
	lsls	r3, r3, #2
	adds	r2, r2, r3
	mov	r9, r2
	movs	r3, #8
	add	r3, r9
	mov	sl, r3
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r2, r8
	mov	r1, sl
	str	r1, [r2, #80]
	ldr	r3, [pc, #356]
	movs	r1, #16
	mov	r0, sl
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c28
	bl	sub_08020040
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0811b89e
	ldr	r1, [r7, #24]
	ldr	r3, [pc, #336]
	ldr	r0, [r6, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x60f0
	adds	r0, r5, #0
	bl	sub_08020000
	ldrb	r3, [r0, #9]
	mov	r2, sl
	lsrs	r3, r3, #1
	mov	r1, r8
	strh	r3, [r1, #32]
	str	r6, [r2, #0]
	movs	r3, #12
	add	r3, r9
	mov	sl, r3
.L_0811b89e:
	movs	r2, #128
	lsls	r2, r2, #6
	movs	r1, #0
	adds	r2, #1
	strb	r1, [r6, #26]
	adds	r0, r5, r2
	bl	sub_08020040
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0811b8c4
	ldr	r1, [r7, #24]
	ldr	r0, [r6, #12]
	ldr	r3, [pc, #276]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	str	r0, [r6, #12]
	str	r6, [r1, #0]
.L_0811b8c4:
	movs	r2, #0
	b.n	.L_0811b94c
.L_0811b8c8:
	ldrh	r0, [r7, #4]
	str	r1, [sp, #0]
	bl	sub_08020040
	adds	r6, r0, #0
	ldr	r1, [sp, #0]
	cmp	r6, #0
	beq.n	.L_0811b94e
	movs	r3, #1
	mov	sl, r3
	mov	r2, sl
	strb	r2, [r1, #0]
	mov	r3, r8
	str	r6, [r3, #80]
	ldr	r0, [r6, #12]
	ldr	r1, [r7, #24]
	ldr	r2, [pc, #228]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x60f0
	ldr	r5, [r6, #40]
	ldr	r3, [r7, #20]
	strb	r3, [r5, #5]
	mov	r3, sl
	strb	r3, [r5, #6]
	ldrh	r5, [r7, #6]
	cmp	r5, #0
	beq.n	.L_0811b90e
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08020058
	mov	r1, sl
	adds	r5, r0, #0
	strb	r1, [r5, #6]
.L_0811b90e:
	ldrh	r5, [r7, #8]
	cmp	r5, #0
	beq.n	.L_0811b92a
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08020058
	adds	r5, r0, #0
	str	r5, [r7, #32]
	movs	r1, #0
	bl	sub_08020080
	movs	r3, #3
	strb	r3, [r5, #6]
.L_0811b92a:
	ldrh	r5, [r7, #10]
	cmp	r5, #0
	beq.n	.L_0811b94e
	ldrb	r3, [r6, #20]
	cmp	r3, #32
	bne.n	.L_0811b93c
	movs	r5, #216
	lsls	r5, r5, #1
	adds	r5, #255
.L_0811b93c:
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_08020058
	mov	r2, r9
	adds	r5, r0, #0
	str	r5, [r7, #36]
	strb	r2, [r5, #6]
.L_0811b94c:
	strb	r2, [r6, #26]
.L_0811b94e:
	ldr	r0, [sp, #8]
	bl	sub_0811b6a4
.L_0811b954:
	ldr	r3, [sp, #4]
	movs	r1, #1
	add	fp, r1
	adds	r3, #2
	mov	r2, fp
	str	r3, [sp, #4]
	cmp	r2, #13
	bgt.n	.L_0811b970
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	str	r3, [sp, #8]
	cmp	r3, #255
	beq.n	.L_0811b970
	b.n	.L_0811b7c2
.L_0811b970:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_0811b9bc
	ldr	r2, [sp, #16]
	movs	r3, #0
	movs	r1, #0
	ldrsh	r5, [r2, r1]
	mov	fp, r3
	cmp	r5, #255
	beq.n	.L_0811b9bc
.L_0811b984:
	ldr	r1, [sp, #16]
	ldrsh	r3, [r3, r1]
	cmp	r3, #254
	beq.n	.L_0811b9a8
	adds	r0, r5, #0
	bl	sub_0811be3c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0811b9a8
	ldr	r7, [r7, #0]
	mov	r8, r7
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0811b9a8
	adds	r0, r5, #0
	bl	sub_0811b6a4
.L_0811b9a8:
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #13
	bgt.n	.L_0811b9bc
	ldr	r1, [sp, #16]
	lsls	r3, r2, #1
	ldrsh	r5, [r3, r1]
	cmp	r5, #255
	bne.n	.L_0811b984
.L_0811b9bc:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000258
	.2byte 0x021c
	.2byte 0x0300
	push	{lr}
	movs	r2, #1
	bl	.L_0811b75c
	pop	{pc}
	.2byte 0x0000
