@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801de5c
	.thumb_func
Func_0801de5c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	mov	r8, r3
	ldr	r3, [pc, #724]
	movs	r5, #128
	ldr	r3, [r3, #0]
	lsls	r5, r5, #4
	adds	r6, r0, #0
	adds	r0, r5, #0
	mov	fp, r3
	bl	Func_08004938
	str	r0, [sp, #8]
	ldr	r0, [pc, #708]
	bl	Func_08002f40
	ldr	r3, [pc, #704]
	str	r0, [sp, #4]
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #12
	str	r3, [sp, #0]
	movs	r1, #16
	ldr	r3, [pc, #696]
	add	r0, sp, #20
	bl	Func_080072f0
	movs	r2, #240
	ldr	r1, [sp, #0]
	lsls	r2, r2, #8
	cmp	r1, r2
	bne.n	.L0
	add	r3, sp, #20
	mov	sl, r3
	ldr	r3, [pc, #676]
	add	r3, fp
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #676]
	movs	r3, #15
	ands	r3, r2
	ldrb	r3, [r1, r3]
	mov	r4, sl
	strb	r3, [r4, #1]
	movs	r3, #3
	strb	r3, [r4, #3]
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #660]
	adds	r1, r5, #0
	ldr	r2, [pc, #660]
	bl	Func_080072f0
	b.n	.L1
.L0:
	ldr	r3, [pc, #640]
	add	r3, fp
	ldrb	r2, [r3, #0]
	movs	r7, #20
	movs	r3, #15
	add	r7, sp
	ands	r3, r2
	strb	r3, [r7, #1]
	movs	r3, #1
	strb	r3, [r7, #3]
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #628]
	adds	r1, r5, #0
	ldr	r2, [pc, #632]
	mov	sl, r7
	bl	Func_080072f0
.L1:
	cmp	r6, #0
	bne.n	.L2
	b.n	.L3
.L2:
	b.n	.L4
.L16:
	cmp	r1, #30
	bhi.n	.L5
	subs	r3, r1, #3
	cmp	r3, #26
	bls.n	.L6
	b.n	.L4
.L6:
	ldr	r2, [pc, #608]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801dfaa
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801df98
	.4byte 0x0801df80
	.4byte 0x0801df98
	.4byte 0x0801df98
	.4byte 0x0801dfb2
	.4byte 0x0801dfb2
	.4byte 0x0801e036
	.4byte 0x0801dfb0
	.4byte 0x0801dfb0
	.4byte 0x0801e036
	.4byte 0x0801dfb2
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801dfb0
	.4byte 0x0801dfb2
	ldr	r3, [pc, #468]
	ldrh	r1, [r6, #0]
	add	r3, fp
	strh	r1, [r3, #0]
	ldr	r2, [pc, #464]
	movs	r3, #15
	ands	r3, r1
	ldrb	r3, [r2, r3]
	mov	r1, sl
	adds	r6, #2
	strb	r3, [r1, #1]
	b.n	.L4
	ldr	r3, [pc, #444]
	movs	r2, #15
	add	r3, fp
	strh	r2, [r3, #0]
	ldr	r3, [pc, #440]
	ldrb	r3, [r3, r2]
	mov	r2, sl
	strb	r3, [r2, #1]
	b.n	.L4
	ldr	r3, [pc, #452]
	ldrb	r3, [r3, #0]
	b.n	.L7
	adds	r6, #2
	adds	r6, #2
	b.n	.L4
.L5:
	movs	r3, #255
	ands	r1, r3
	ldr	r4, [sp, #4]
	ldr	r0, [sp, #8]
	lsls	r3, r1, #5
	movs	r7, #0
	adds	r5, r4, r3
	add	r0, r8
	mov	r9, r7
	mov	lr, sl
.L11:
	ldmia	r5!, {r2}
	movs	r4, #3
.L10:
	movs	r7, #15
	adds	r3, r2, #0
	ands	r3, r7
	mov	r7, lr
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L8
	strb	r3, [r0, #0]
.L8:
	lsrs	r2, r2, #4
	movs	r7, #15
	adds	r3, r2, #0
	ands	r3, r7
	mov	r7, sl
	ldrb	r3, [r7, r3]
	adds	r0, #1
	cmp	r3, #0
	beq.n	.L9
	strb	r3, [r0, #0]
.L9:
	subs	r4, #1
	adds	r0, #1
	lsrs	r2, r2, #4
	cmp	r4, #0
	bge.n	.L10
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r0, #248
	cmp	r3, #7
	ble.n	.L11
	ldr	r4, [pc, #360]
	cmp	ip, r4
	beq.n	.L12
	ldr	r7, [pc, #360]
	cmp	ip, r7
	bne.n	.L13
.L12:
	movs	r1, #8
	add	r8, r1
	b.n	.L4
.L13:
	ldr	r2, [pc, #352]
	cmp	ip, r2
	bne.n	.L14
	movs	r3, #3
	b.n	.L7
.L14:
	cmp	r1, #31
	bls.n	.L15
	ldr	r2, [pc, #324]
	adds	r3, r1, #0
	subs	r3, #32
	ldrb	r3, [r2, r3]
	b.n	.L7
.L15:
	movs	r3, #1
.L7:
	add	r8, r3
.L4:
	ldrh	r1, [r6, #0]
	adds	r6, #2
	mov	ip, r1
	cmp	r1, #0
	beq.n	.L3
	b.n	.L16
.L3:
	mov	r3, r8
	adds	r3, #7
	lsrs	r6, r3, #3
	ldr	r5, [sp, #8]
	movs	r1, #128
	lsls	r4, r6, #2
	lsls	r7, r6, #3
	lsls	r1, r1, #1
	movs	r2, #7
	adds	r0, r5, #0
	mov	sl, r4
	mov	r8, r7
	mov	lr, r1
	mov	r9, r2
.L19:
	cmp	r6, #0
	beq.n	.L17
	ldr	r3, [pc, #284]
	ldr	r7, [pc, #284]
	mov	ip, r3
	adds	r4, r6, #0
.L18:
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	lsls	r3, r1, #4
	orrs	r1, r3
	lsrs	r3, r2, #4
	orrs	r2, r3
	mov	r3, ip
	ands	r1, r3
	ands	r2, r7
	lsls	r3, r1, #8
	orrs	r1, r3
	lsrs	r3, r2, #8
	orrs	r2, r3
	lsls	r3, r1, #4
	lsrs	r3, r3, #16
	lsls	r2, r2, #16
	orrs	r3, r2
	subs	r4, #1
	adds	r5, #8
	stmia	r0!, {r3}
	cmp	r4, #0
	bne.n	.L18
.L17:
	mov	r4, sl
	movs	r2, #1
	subs	r3, r0, r4
	mov	r7, lr
	mov	r1, r8
	negs	r2, r2
	adds	r0, r3, r7
	add	r9, r2
	subs	r3, r5, r1
	adds	r5, r3, r7
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L19
	cmp	r6, #0
	bne.n	.L20
	b.n	.L21
.L20:
	ldr	r4, [pc, #208]
	movs	r7, #234
	movs	r1, #218
	ldr	r2, [pc, #204]
	add	r4, fp
	lsls	r7, r7, #4
	lsls	r1, r1, #4
	ldr	r0, [sp, #8]
	mov	lr, r4
	add	r7, fp
	mov	ip, r1
	mov	r8, r2
	mov	r9, r6
.L29:
	mov	r3, lr
	ldrb	r2, [r3, #0]
	movs	r5, #127
	cmp	r2, #0
	beq.n	.L22
	movs	r5, #255
.L22:
	ldr	r4, [sp, #16]
	ldrh	r3, [r4, #0]
	mov	r1, r8
	ands	r1, r3
	adds	r3, r1, #0
	subs	r3, #128
	cmp	r3, #127
	bls.n	.L23
	cmp	r2, #0
	beq.n	.L24
	movs	r2, #128
	lsls	r2, r2, #2
	cmp	r1, r2
	bcc.n	.L24
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r1, r3
	bcc.n	.L23
.L24:
	ldrh	r1, [r7, #0]
	mov	r2, ip
	ands	r1, r5
	adds	r3, r1, r2
	mov	r2, fp
	ldrb	r3, [r2, r3]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L25
.L26:
	adds	r1, #1
	adds	r4, #1
	ands	r1, r5
	cmp	r4, r5
	bhi.n	.L25
	movs	r2, #218
	lsls	r2, r2, #4
	adds	r3, r1, r2
	mov	r2, fp
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L26
.L25:
	adds	r3, r1, #1
	ands	r3, r5
	strh	r3, [r7, #0]
	mov	r3, ip
	adds	r2, r1, r3
	mov	r4, fp
	movs	r3, #1
	strb	r3, [r4, r2]
	cmp	r1, #127
	bls.n	.L27
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r1, r1, r2
	b.n	.L28
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000013
	.4byte 0x00000ea7
	.4byte 0x03000164
	.4byte 0x00000eae
	.4byte 0x080371b4
	.4byte 0x03000168
	.4byte 0x04040404
	.4byte 0x0e0e0e0e
	.4byte 0x0801df14
	.4byte 0x080370d4
	.4byte 0x0000f01d
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.4byte 0x0ff00ff0
	.4byte 0x00ff00ff
	.4byte 0x00000ea2
	.4byte 0x000003ff
.L27:
	movs	r3, #128
	orrs	r1, r3
.L28:
	ldr	r3, [sp, #0]
	ldr	r4, [sp, #16]
	orrs	r3, r1
	strh	r3, [r4, #0]
	ldr	r2, [sp, #12]
	strh	r3, [r2, #0]
.L23:
	movs	r3, #192
	lsls	r2, r1, #5
	lsls	r3, r3, #19
	adds	r1, r2, r3
	ldr	r4, [pc, #152]
	ldr	r3, [r0, #0]
	str	r3, [r1, #0]
	adds	r1, r2, r4
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	lsls	r4, r4, #2
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #0]
	ldr	r3, [pc, #128]
	lsls	r4, r4, #2
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r1, #0]
	ldr	r3, [pc, #116]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r1, #0]
	ldr	r3, [pc, #108]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #0]
	ldr	r3, [pc, #96]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #224
	str	r3, [r1, #0]
	ldr	r3, [pc, #88]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	str	r3, [r1, #0]
	movs	r3, #1
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	negs	r3, r3
	add	r9, r3
	adds	r1, #2
	adds	r2, #2
	mov	r4, r9
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	adds	r0, #4
	cmp	r4, #0
	beq.n	.L21
	b.n	.L29
.L21:
	ldr	r0, [sp, #8]
	bl	Func_08002df0
	adds	r0, r6, #0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x06000004
	.4byte 0x06000008
	.4byte 0x0600000c
	.4byte 0x06000010
	.4byte 0x06000014
	.4byte 0x06000018
	.4byte 0x0600001c
