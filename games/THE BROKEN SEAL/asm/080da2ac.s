@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080da2ac
	.thumb_func
Func_080da2ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #116
	str	r3, [sp, #72]
	subs	r2, #108
	ldr	r3, [pc, #84]
	mov	r9, r1
	ldr	r2, [r2, #0]
	add	r3, r9
	str	r2, [sp, #56]
	str	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #52]
	movs	r1, #240
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #48
	lsls	r1, r1, #5
	strh	r3, [r2, #0]
	ldr	r0, [pc, #60]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #128
	ldr	r3, [pc, #48]
	lsls	r2, r2, #8
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #52]
	bl	Func_080072f0
	ldr	r3, [pc, #40]
	movs	r2, #7
	movs	r5, #0
	mov	ip, r2
	movs	r7, #0
	mov	lr, r3
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000008b
	.4byte 0x03001388
	.4byte 0x02010000
	.4byte 0x06008000
.L0:
	adds	r4, r5, #0
	adds	r4, #96
	adds	r3, r4, #0
	mov	r6, ip
	ands	r3, r6
	lsls	r3, r3, #3
	mov	r8, r3
	adds	r3, r7, r5
	lsls	r3, r3, #3
	mov	r2, r9
	movs	r0, #0
	adds	r1, r3, r2
.L3:
	adds	r3, r0, #0
	adds	r3, #32
	adds	r2, r3, #0
	mov	r6, ip
	ands	r2, r6
	cmp	r3, #0
	bge.n	.L1
	adds	r3, #7
.L1:
	asrs	r3, r3, #3
	lsls	r3, r3, #6
	adds	r3, r2, r3
	mov	r6, r8
	adds	r2, r3, r6
	adds	r3, r4, #0
	cmp	r3, #0
	bge.n	.L2
	adds	r3, #7
.L2:
	asrs	r3, r3, #3
	lsls	r3, r3, #11
	adds	r3, r2, r3
	mov	r2, lr
	ldrb	r3, [r3, r2]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r0, #40
	bne.n	.L3
	adds	r5, #1
	adds	r7, #4
	cmp	r5, #16
	bne.n	.L0
	ldr	r3, [pc, #780]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L4
	ldr	r3, [pc, #772]
	ldr	r2, [pc, #772]
	str	r3, [r2, #0]
	movs	r3, #112
	negs	r3, r3
	str	r3, [sp, #52]
	b.n	.L5
.L4:
	movs	r4, #0
	str	r4, [sp, #52]
.L5:
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	bl	Func_080ed408
	ldr	r3, [pc, #744]
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #60]
	ldr	r3, [pc, #724]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L6
	ldr	r3, [pc, #712]
	ldr	r2, [pc, #724]
	add	r3, r9
	ldr	r1, [r3, #0]
	movs	r0, #0
	add	r2, r9
.L7:
	str	r0, [r2, #0]
	ldr	r3, [r1, #20]
	adds	r4, #1
	adds	r2, #28
	cmp	r4, r3
	bne.n	.L7
.L6:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #696]
	movs	r3, #0
	add	r2, r9
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #688]
	bl	Func_080041d8
	mov	r6, sp
	adds	r6, #100
	str	r6, [sp, #48]
	ldr	r3, [pc, #680]
	adds	r2, r6, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	ldr	r2, [pc, #672]
	movs	r3, #128
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #141
	bl	Func_080f9010
	movs	r6, #0
	ldr	r3, [pc, #624]
	str	r6, [sp, #68]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #652]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L8
	b.n	.L9
.L8:
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #600]
	adds	r0, #12
	add	r1, r9
	str	r0, [sp, #28]
	str	r1, [sp, #44]
.L23:
	bl	Func_080049ac
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #28]
	bl	Func_080051d8
	ldr	r2, [sp, #44]
	ldr	r3, [r2, #0]
	ldr	r2, [r3, #24]
	ldr	r4, [pc, #608]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r4, r3]
	ldr	r6, [sp, #68]
	subs	r3, #16
	cmp	r6, r3
	bne.n	.L10
	movs	r0, #133
	bl	Func_080b50e8
.L10:
	movs	r0, #0
	movs	r1, #8
	movs	r2, #16
	str	r0, [sp, #64]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r0, [sp, #12]
.L19:
	ldr	r4, [sp, #68]
	movs	r3, #31
	ands	r3, r4
	ldr	r6, [sp, #16]
	adds	r3, #32
	cmp	r3, r6
	bne.n	.L11
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #12]
	ldr	r3, [r0, r1]
	adds	r3, #32
	str	r3, [r0, r1]
.L11:
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bge.n	.L12
	b.n	.L13
.L12:
	ldr	r6, [pc, #500]
	add	r6, r9
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r4, [pc, #524]
	adds	r3, #1
	ldrb	r3, [r4, r3]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #68]
	adds	r3, r0, r3
	cmp	r1, r3
	bge.n	.L13
	subs	r0, r1, r0
	lsls	r0, r0, #10
	bl	Func_08002322
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #48]
	ldr	r3, [r2, r4]
	muls	r3, r0
	asrs	r5, r3, #16
	cmp	r5, #0
	bge.n	.L14
	negs	r5, r5
.L14:
	movs	r3, #112
	movs	r1, #240
	subs	r3, r3, r5
	movs	r0, #40
	lsls	r1, r1, #5
	str	r3, [sp, #40]
	str	r0, [sp, #0]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #60]
	str	r5, [sp, #4]
	ldr	r0, [sp, #72]
	add	r1, r9
	bl	Func_080072f4
	movs	r0, #40
	movs	r2, #16
	movs	r3, #96
	subs	r3, r3, r5
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #72]
	mov	r1, r9
	ldr	r2, [sp, #24]
	bl	Func_080072f4
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L13
	ldr	r2, [sp, #24]
	movs	r0, #76
	ldr	r1, [sp, #20]
	add	r0, sp
	adds	r2, #40
	mov	sl, r0
	movs	r3, #36
	str	r1, [sp, #36]
	str	r2, [sp, #32]
	adds	r7, r6, #0
	mov	r8, r3
	add	r6, sp, #88
	mov	fp, sl
.L17:
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	str	r4, [sp, #8]
	bl	Func_080b5098
	ldr	r5, [r0, #0]
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	mov	r1, fp
	str	r3, [r6, #8]
	adds	r0, r6, #0
	bl	Func_080e3944
	mov	r0, fp
	ldr	r3, [r0, #0]
	ldr	r1, [sp, #52]
	adds	r2, r3, r1
	str	r2, [r0, #0]
	ldr	r3, [sp, #36]
	adds	r3, #8
	ldr	r4, [sp, #8]
	cmp	r2, r3
	blt.n	.L15
	ldr	r3, [sp, #32]
	cmp	r2, r3
	bgt.n	.L15
	mov	r0, sl
	ldr	r3, [r0, #4]
	ldr	r1, [sp, #40]
	cmp	r3, r1
	blt.n	.L15
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #40]
	ldr	r3, [pc, #324]
	str	r3, [r5, #72]
.L15:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bge.n	.L16
	ldr	r3, [r7, #0]
	mov	r2, r8
	ldrsh	r0, [r3, r2]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L16:
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #20]
	movs	r2, #2
	adds	r4, #1
	add	r8, r2
	cmp	r4, r3
	bne.n	.L17
.L13:
	ldr	r3, [sp, #20]
	ldr	r4, [sp, #24]
	ldr	r6, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #64]
	adds	r3, #40
	adds	r4, #40
	adds	r6, #4
	adds	r0, #4
	adds	r1, #1
	str	r3, [sp, #20]
	str	r4, [sp, #24]
	str	r6, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #64]
	cmp	r1, #3
	beq.n	.L18
	b.n	.L19
.L18:
	ldr	r2, [pc, #200]
	mov	r6, r9
	ldr	r3, [r6, r2]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L20
	movs	r6, #225
	lsls	r6, r6, #7
	add	r6, r9
	movs	r7, #36
.L22:
	mov	r0, r9
	adds	r5, r0, r2
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	str	r4, [sp, #8]
	bl	Func_080b5098
	ldr	r3, [r6, #24]
	ldr	r0, [r0, #0]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L21
	ldr	r3, [r0, #12]
	cmp	r3, #0
	bgt.n	.L21
	ldr	r3, [r0, #40]
	cmp	r3, #0
	bge.n	.L21
	movs	r3, #1
	str	r3, [r6, #24]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L21:
	ldr	r2, [pc, #116]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #28
	adds	r7, #2
	cmp	r4, r3
	bne.n	.L22
.L20:
	ldr	r1, [sp, #44]
	ldr	r3, [r1, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #124]
	adds	r3, #2
	ldrb	r1, [r2, r3]
	adds	r0, r1, #0
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [sp, #68]
	adds	r3, #1
	str	r3, [sp, #68]
	ldr	r4, [sp, #44]
	ldr	r3, [r4, #0]
	ldr	r2, [r3, #24]
	ldr	r6, [pc, #84]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #68]
	cmp	r0, r3
	beq.n	.L9
	b.n	.L23
.L9:
	ldr	r0, [pc, #56]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007828
	.4byte 0xffff9000
	.4byte 0x04000028
	.4byte 0x03001e50
	.4byte 0x00007098
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080eda88
	.4byte 0x000077a8
	.4byte 0x080eea38
	.4byte 0x0000ab85
	.4byte 0x00007824
