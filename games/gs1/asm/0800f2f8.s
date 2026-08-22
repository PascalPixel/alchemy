@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800f2f8
	.thumb_func
Func_0800f2f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	movs	r1, #0
	adds	r6, r0, #0
	ldr	r3, [pc, #796]
	movs	r0, #2
	movs	r2, #135
	str	r1, [sp, #8]
	str	r0, [sp, #0]
	lsls	r2, r2, #2
	ldr	r1, [pc, #788]
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #48]
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r6, #52]
	movs	r3, #5
	str	r3, [sp, #0]
	b.n	.L1
.L0:
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r6, #52]
.L1:
	ldr	r3, [pc, #748]
	movs	r0, #128
	ldr	r3, [r3, #0]
	lsls	r0, r0, #2
	ands	r3, r0
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #48]
.L2:
	ldr	r5, [pc, #724]
	ldr	r2, [r5, #0]
	movs	r1, #15
	lsrs	r2, r2, #4
	ands	r2, r1
	ldr	r3, [pc, #720]
	lsls	r2, r2, #1
	ldrsh	r1, [r3, r2]
	str	r1, [sp, #4]
	lsls	r1, r1, #16
	mov	fp, r1
	lsrs	r7, r1, #16
	ldr	r1, [pc, #712]
	cmp	r7, r1
	bne.n	.L3
	ldr	r2, [sp, #8]
	movs	r3, #4
	orrs	r2, r3
	str	r2, [sp, #8]
	b.n	.L4
.L8:
	mov	r3, fp
	asrs	r3, r3, #16
	str	r3, [sp, #4]
	b.n	.L4
.L3:
	ldr	r3, [r6, #8]
	add	r1, sp, #80
	movs	r0, #0
	str	r0, [sp, #8]
	str	r3, [r1, #0]
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	movs	r2, #224
	ldr	r3, [r6, #16]
	lsls	r2, r2, #11
	mov	r8, r1
	mov	sl, r2
	str	r3, [r1, #8]
	mov	r0, sl
	adds	r1, r7, #0
	mov	r2, r8
	bl	Func_0800447c
	ldr	r3, [pc, #656]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
	ldr	r3, [r5, #0]
	mov	r0, r9
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L5
	b.n	.L4
.L5:
	adds	r0, r6, #0
	mov	r1, r8
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L6
	ldr	r3, [r6, #8]
	add	r5, sp, #68
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r2, #128
	ldr	r3, [r6, #16]
	lsls	r2, r2, #5
	adds	r1, r7, r2
	mov	r0, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L6
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #576]
	mov	r0, sl
	adds	r1, r7, r3
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L6
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r0, #128
	ldr	r3, [r6, #16]
	lsls	r0, r0, #6
	adds	r1, r7, r0
	str	r3, [r5, #8]
	mov	r0, sl
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L6
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r2, [pc, #508]
	ldr	r3, [r6, #16]
	adds	r1, r7, r2
	mov	r0, sl
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L6
	b.n	.L4
.L6:
	add	r3, sp, #12
	mov	r0, fp
	movs	r1, #128
	mov	sl, r3
	lsls	r1, r1, #5
	lsrs	r3, r0, #16
	adds	r2, r3, r1
	ldr	r1, [pc, #460]
	mov	r0, sl
	strh	r2, [r0, #0]
	adds	r2, r3, r1
	movs	r1, #128
	lsls	r1, r1, #6
	strh	r2, [r0, #2]
	adds	r2, r3, r1
	ldr	r1, [pc, #448]
	strh	r2, [r0, #4]
	adds	r2, r3, r1
	movs	r1, #192
	lsls	r1, r1, #6
	strh	r2, [r0, #6]
	adds	r2, r3, r1
	ldr	r1, [pc, #440]
	strh	r2, [r0, #8]
	adds	r2, r3, r1
	movs	r1, #128
	lsls	r1, r1, #7
	strh	r2, [r0, #10]
	adds	r2, r3, r1
	ldr	r1, [pc, #428]
	strh	r2, [r0, #12]
	adds	r3, r3, r1
	mov	r2, sl
	strh	r3, [r2, #14]
	movs	r3, #0
	mov	r9, r3
.L9:
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	ldrsh	r2, [r1, r3]
	ldr	r3, [r6, #8]
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [r6, #12]
	str	r3, [r1, #4]
	lsls	r2, r2, #16
	ldr	r3, [r6, #16]
	lsrs	r7, r2, #16
	movs	r0, #224
	str	r3, [r1, #8]
	lsls	r0, r0, #11
	adds	r1, r7, #0
	mov	fp, r2
	mov	r2, r8
	bl	Func_0800447c
	adds	r0, r6, #0
	mov	r1, r8
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L7
	ldr	r3, [r6, #8]
	add	r5, sp, #68
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r2, #128
	ldr	r3, [r6, #16]
	lsls	r2, r2, #5
	movs	r0, #224
	adds	r1, r7, r2
	lsls	r0, r0, #11
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L7
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r3, [pc, #296]
	movs	r0, #224
	adds	r1, r7, r3
	lsls	r0, r0, #11
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L7
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	movs	r0, #128
	ldr	r3, [r6, #16]
	lsls	r0, r0, #6
	adds	r1, r7, r0
	movs	r0, #224
	lsls	r0, r0, #11
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L7
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r2, [pc, #228]
	ldr	r3, [r6, #16]
	movs	r0, #224
	adds	r1, r7, r2
	lsls	r0, r0, #11
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080122ac
	cmp	r0, #0
	bne.n	.L7
	b.n	.L8
.L7:
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #8
	blt.n	.L9
	ldr	r1, [sp, #8]
	orrs	r1, r3
	str	r1, [sp, #8]
.L4:
	ldr	r3, [pc, #192]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L10
	ldr	r0, [sp, #8]
	movs	r2, #3
	ands	r2, r0
	cmp	r2, #0
	beq.n	.L11
	movs	r1, #206
	lsls	r1, r1, #1
	adds	r2, r3, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L10
.L11:
	movs	r0, #206
	lsls	r0, r0, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
.L10:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	beq.n	.L12
	adds	r0, r6, #0
	movs	r1, #9
	bl	Func_0800c300
	b.n	.L13
.L12:
	adds	r0, r6, #0
	ldr	r1, [sp, #0]
	bl	Func_0800c300
.L13:
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L14
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	ldr	r0, [sp, #4]
	ldrh	r2, [r6, #6]
	lsls	r3, r0, #16
	lsrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r1, r3
	ble.n	.L16
	adds	r1, r3, #0
.L16:
	ldr	r3, [pc, #56]
	cmp	r1, r3
	bge.n	.L17
	adds	r1, r3, #0
.L17:
	adds	r3, r2, r1
	strh	r3, [r6, #6]
.L15:
	movs	r1, #100
	adds	r1, r1, r6
	mov	r8, r1
	movs	r3, #0
	mov	r2, r8
	strh	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #102
	movs	r3, #2
	strh	r3, [r2, #0]
	b.n	.L18
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x08013254
	.4byte 0x0000ffff
	.4byte 0x03001f54
	.4byte 0xfffff000
	.4byte 0xffffe000
	.4byte 0xffffd000
	.4byte 0xffffc000
	.4byte 0x03001ebc
.L14:
	add	r3, sp, #80
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r6, #0
	ldr	r3, [r3, #8]
	bl	Func_0800d14c
	ldr	r1, [r6, #36]
	ldr	r4, [pc, #184]
	adds	r0, r1, #0
	mov	ip, pc
	bx	r4
	ldr	r1, [r6, #44]
	adds	r3, r0, #0
	adds	r0, r1, #0
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #8]
	lsls	r1, r2, #16
	adds	r2, r6, #0
	adds	r2, #36
	lsrs	r1, r1, #16
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	bl	Func_0800447c
	movs	r3, #100
	adds	r3, r3, r6
	mov	r8, r3
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L18
	subs	r3, r2, #1
	mov	r1, r8
	strh	r3, [r1, #0]
.L18:
	ldr	r3, [pc, #120]
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #120]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ldr	r1, [pc, #116]
	ands	r3, r2
	movs	r2, #141
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	ldr	r4, [r1, r3]
	adds	r1, r0, r2
	ldrh	r0, [r1, #0]
	subs	r3, r4, r0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L19
	adds	r3, #7
.L19:
	asrs	r2, r3, #3
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r2, r3
	ble.n	.L20
	adds	r2, r3, #0
.L20:
	ldr	r3, [pc, #80]
	cmp	r2, r3
	bge.n	.L21
	adds	r2, r3, #0
.L21:
	adds	r3, r2, #0
	adds	r3, #15
	cmp	r3, #30
	bhi.n	.L22
	ldrh	r3, [r1, #0]
	subs	r2, r4, r3
.L22:
	adds	r3, r0, r2
	strh	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r7, [r3, #0]
	cmp	r7, #1
	bne.n	.L23
	adds	r0, r6, #0
	adds	r0, #8
	ldr	r5, [r6, #80]
	bl	Func_08012204
	cmp	r0, #9
	bne.n	.L24
	ldr	r3, [r5, #44]
	adds	r2, r5, #0
	strb	r7, [r3, #6]
	ldr	r3, [pc, #4]
	adds	r2, #38
	strb	r3, [r2, #0]
	b.n	.L25
	.4byte 0x00000000
	.4byte 0x03000118
	.4byte 0x03001e70
	.4byte 0x03001ae8
	.4byte 0x0801328c
	.4byte 0xfffffe00
.L24:
	ldr	r3, [r5, #44]
	movs	r2, #9
	strb	r2, [r3, #6]
	adds	r3, r5, #0
	adds	r3, #38
	strb	r7, [r3, #0]
.L25:
	cmp	r0, #6
	bne.n	.L23
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L23
	ldr	r2, [sp, #8]
	cmp	r2, #0
	bne.n	.L23
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #24
	bl	Func_0800c150
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L23
	ldr	r1, [pc, #108]
	ldr	r7, [r5, #80]
	bl	Func_0800c2d8
	add	r0, sp, #8
	adds	r3, r5, #0
	ldrb	r0, [r0, #0]
	adds	r3, #85
	adds	r2, r5, #0
	strb	r0, [r3, #0]
	adds	r2, #34
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r7, #0
	beq.n	.L26
	movs	r1, #1
	adds	r0, r7, #0
	bl	Func_0800ba30
	add	r1, sp, #8
	ldrb	r1, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #38
	strb	r1, [r3, #0]
	movs	r2, #13
	ldrb	r1, [r7, #5]
	negs	r2, r2
	adds	r3, r2, #0
	ands	r3, r1
	movs	r1, #4
	orrs	r3, r1
	strb	r3, [r7, #5]
	ldrb	r3, [r7, #9]
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r2, [r7, #9]
.L26:
	movs	r3, #10
	mov	r2, r8
	strh	r3, [r2, #0]
.L23:
	bl	Func_0800eaf8
	ldrh	r3, [r6, #4]
	adds	r3, #1
	movs	r0, #1
	strh	r3, [r6, #4]
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x08013280
