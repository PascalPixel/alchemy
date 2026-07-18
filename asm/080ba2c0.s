@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ba2c0
	.thumb_func
Func_080ba2c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r0, [sp, #12]
	ldr	r3, [pc, #268]
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #12]
	str	r3, [sp, #4]
	ldrb	r0, [r1, #0]
	bl	Func_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	bl	Func_080044d0
	ldr	r3, [pc, #248]
	lsls	r0, r0, #16
	ldr	r1, [sp, #12]
	lsrs	r0, r0, #16
	adds	r2, r0, r3
	ldrb	r3, [r1, #0]
	cmp	r3, #7
	bls.n	.L0
	movs	r3, #192
	lsls	r3, r3, #7
	adds	r2, r0, r3
.L0:
	ldr	r3, [pc, #228]
	ldr	r1, [pc, #224]
	ands	r2, r3
	adds	r3, r2, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #128
	asrs	r3, r3, #1
	lsls	r1, r1, #6
	adds	r2, r3, r1
	ldr	r1, [sp, #4]
	ldr	r3, [r1, #0]
	cmp	r3, r2
	bne.n	.L1
	str	r2, [r1, #0]
	movs	r0, #5
	bl	Func_080030f8
	b.n	.L2
.L1:
	ldr	r3, [sp, #4]
	movs	r0, #10
	str	r2, [r3, #0]
	bl	Func_080030f8
.L2:
	movs	r0, #0
	movs	r1, #0
	bl	Func_080c10e8
	add	r7, sp, #24
	ldr	r0, [sp, #12]
	adds	r1, r7, #0
	bl	Func_080b9d34
	ldr	r3, [r7, #0]
	cmp	r3, #135
	bne.n	.L3
	ldr	r3, [pc, #160]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	Func_08015130
.L3:
	ldr	r0, [r7, #8]
	bl	Func_08077008
	adds	r5, r0, #0
	movs	r1, #36
	ldrsh	r0, [r7, r1]
	bl	Func_08077008
	ldr	r3, [sp, #12]
	adds	r3, #44
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [sp, #12]
	str	r3, [sp, #8]
	movs	r3, #30
	ldrsb	r3, [r2, r3]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L4
	movs	r6, #1
.L4:
	ldr	r3, [sp, #12]
	ldrb	r0, [r3, #0]
	bl	Func_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	ldr	r3, [r0, #40]
	movs	r2, #1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #2
	bl	Func_08009260
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r3, r5, r2
	mov	r9, r0
	ldrb	r0, [r3, #0]
	bl	Func_080c2410
	adds	r3, r0, #0
	lsls	r3, r3, #16
	movs	r2, #36
	ldrsh	r1, [r7, r2]
	ldr	r0, [r7, #8]
	mov	r2, r9
	bl	Func_080b82c4
	ldr	r0, [r7, #8]
	bl	Func_080b7dd0
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	Func_08009088
	movs	r3, #36
	ldrsh	r0, [r7, r3]
	bl	Func_080b7dd0
	ldrh	r3, [r7, #36]
	cmp	r3, #7
	bhi.n	.L5
	movs	r3, #1
	b.n	.L6
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x00007fff
	.4byte 0x03001e74
.L5:
	movs	r3, #0
.L6:
	str	r3, [r7, #4]
	ldr	r1, [pc, #44]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #44]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #24]
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L7
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x00006000
	.4byte 0x04000040
	.4byte 0x04000048
.L7:
	cmp	r6, #0
	beq.n	.L8
	movs	r0, #10
	bl	Func_080030f8
	movs	r1, #36
	ldrsh	r0, [r7, r1]
	bl	Func_080b8178
	movs	r0, #2
	bl	Func_080030f8
	movs	r0, #4
	bl	Func_080030f8
	movs	r0, #10
	bl	Func_080030f8
	ldr	r2, [sp, #12]
	movs	r0, #0
	ldrb	r1, [r2, #2]
	bl	Func_080bbabc
	ldr	r1, [pc, #268]
	movs	r0, #4
	bl	Func_080bbabc
	bl	Func_080bb938
	movs	r3, #36
	ldrsh	r0, [r7, r3]
	bl	Func_080b8000
	b.n	.L9
.L8:
	movs	r1, #0
	str	r1, [sp, #0]
	str	r1, [r7, #28]
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #88]
	cmp	r3, #0
	beq.n	.L10
	movs	r3, #1
	str	r3, [r7, #28]
.L10:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L11
	ldr	r3, [r7, #0]
	adds	r3, #200
	str	r3, [r7, #0]
	ldr	r2, [sp, #4]
	movs	r1, #1
	str	r1, [sp, #0]
	str	r1, [r2, #20]
	ldr	r3, [r7, #8]
	add	r0, sp, #16
	strh	r3, [r0, #0]
	ldr	r3, [r7, #12]
	strh	r3, [r0, #2]
	movs	r3, #255
	strh	r3, [r0, #4]
	movs	r1, #0
	bl	Func_080b7b6c
.L11:
	movs	r3, #8
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	bgt.n	.L12
	movs	r2, #1
	mov	r9, r2
.L12:
	movs	r3, #0
	mov	r1, r9
	mov	fp, r3
	cmp	r1, #0
	beq.n	.L13
	mov	r8, r7
	mov	sl, r3
.L15:
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L14
	mov	r3, r8
	mov	r1, r9
	mov	r0, sl
	ldr	r5, [r3, #8]
	ldr	r6, [r3, #12]
	bl	Func_080022ec
	adds	r2, r0, #0
	adds	r2, #100
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_080c0df4
.L14:
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	movs	r1, #30
	add	fp, r2
	add	sl, r1
	cmp	fp, r9
	bne.n	.L15
.L13:
	movs	r1, #200
	ldr	r0, [pc, #108]
	lsls	r1, r1, #4
	bl	Func_080041d8
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L16
	ldr	r1, [sp, #12]
	movs	r2, #128
	ldr	r3, [r1, #88]
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L17
	adds	r0, r7, #0
	bl	Func_080c9008
	b.n	.L16
.L17:
	adds	r0, r7, #0
	bl	Func_080c9018
.L16:
	bl	Func_080be02c
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L18
	ldr	r1, [sp, #4]
	movs	r3, #0
	str	r3, [r1, #20]
	bl	Func_080b6cb0
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	Func_080c0cec
.L18:
	movs	r2, #36
	ldrsh	r0, [r7, r2]
	bl	Func_080b8000
.L9:
	ldr	r0, [r7, #8]
	bl	Func_080b8000
	movs	r0, #0
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000853
	.4byte 0x080bd899
