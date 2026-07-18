@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ba978
	.thumb_func
Func_080ba978:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #568]
	adds	r7, r0, #0
	ldr	r5, [r3, #0]
	movs	r2, #128
	ldr	r3, [r7, #88]
	lsls	r2, r2, #11
	ands	r3, r2
	sub	sp, #88
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L0
	ldrb	r3, [r7, #0]
	ldr	r2, [pc, #548]
	cmp	r3, #7
	bls.n	.L1
	movs	r2, #160
	lsls	r2, r2, #7
.L1:
	movs	r3, #60
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	b.n	.L2
.L0:
	ldrb	r0, [r7, #0]
	bl	Func_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	bl	Func_080044d0
	ldrb	r4, [r7, #0]
	lsls	r0, r0, #16
	ldr	r2, [pc, #516]
	lsrs	r0, r0, #16
	adds	r3, r4, #0
	adds	r1, r0, r2
	cmp	r3, #7
	bls.n	.L3
	movs	r3, #192
	lsls	r3, r3, #5
	adds	r1, r0, r3
.L3:
	lsls	r3, r1, #16
	asrs	r1, r3, #16
	adds	r3, r4, #0
	cmp	r3, #7
	bhi.n	.L4
	movs	r3, #128
	lsls	r3, r3, #6
	b.n	.L5
.L4:
	ldr	r3, [pc, #476]
.L5:
	subs	r3, r3, r1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	cmp	r2, #0
	bge.n	.L6
	adds	r2, #3
.L6:
	asrs	r3, r2, #2
	adds	r1, r1, r3
	ldrb	r3, [r7, #2]
	adds	r2, r4, #0
	cmp	r3, #7
	bhi.n	.L7
	movs	r3, #0
	cmp	r2, #7
	bhi.n	.L8
	movs	r3, #1
.L8:
	cmp	r3, #0
	bne.n	.L9
	b.n	.L10
.L7:
	movs	r3, #0
	cmp	r2, #7
	bls.n	.L11
	movs	r3, #1
.L11:
	cmp	r3, #0
	beq.n	.L10
.L9:
	movs	r1, #144
	adds	r3, r4, #0
	lsls	r1, r1, #6
	cmp	r3, #7
	bls.n	.L10
	ldr	r1, [pc, #424]
.L10:
	ldr	r3, [r5, #0]
	cmp	r3, r1
	beq.n	.L2
	str	r1, [r5, #0]
.L2:
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	ldrb	r3, [r7, #0]
	ldr	r2, [pc, #392]
	cmp	r3, #7
	bls.n	.L13
	movs	r2, #128
	lsls	r2, r2, #6
.L13:
	movs	r3, #60
	str	r2, [r5, #0]
	str	r3, [r5, #4]
.L12:
	add	r5, sp, #4
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_080b9d34
	mov	r6, sl
	movs	r3, #1
	ands	r6, r3
	cmp	r6, #0
	beq.n	.L14
	str	r3, [r5, #28]
.L14:
	movs	r1, #0
	movs	r0, #0
	bl	Func_080c10e8
	ldr	r3, [pc, #356]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	Func_08015130
	ldr	r0, [r5, #8]
	bl	Func_080b7dd0
	ldr	r0, [r0, #0]
	movs	r1, #3
	mov	r8, r0
	bl	Func_08009080
	movs	r1, #16
	mov	r0, r8
	bl	Func_08009088
	movs	r0, #154
	bl	Func_080f9010
	movs	r3, #2
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L15
	ldr	r0, [r5, #8]
	ldr	r1, [r7, #80]
	movs	r2, #1
	movs	r3, #0
	bl	Func_080c1798
	b.n	.L16
.L15:
	cmp	r6, #0
	bne.n	.L16
	ldr	r0, [r5, #8]
	ldr	r1, [r7, #80]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080c1798
.L16:
	ldrb	r3, [r7, #2]
	cmp	r3, #7
	bhi.n	.L17
	movs	r3, #1
	b.n	.L18
.L17:
	movs	r3, #0
.L18:
	str	r3, [r5, #4]
	ldr	r3, [r5, #20]
	movs	r4, #0
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L19
	movs	r6, #0
.L22:
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #0]
	bl	Func_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r1, #0
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L20
	mov	ip, r3
	adds	r3, r6, r5
	adds	r2, r3, #0
	adds	r2, #52
	adds	r0, #40
.L21:
	ldmia	r0!, {r3}
	ldrb	r3, [r3, #5]
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L21
.L20:
	ldr	r3, [r5, #20]
	adds	r4, #1
	adds	r6, #4
	adds	r2, r5, #0
	cmp	r4, r3
	bne.n	.L22
.L19:
	ldr	r3, [r7, #92]
	cmp	r3, #0
	beq.n	.L23
	cmp	r3, #1
	bne.n	.L24
	ldrb	r1, [r7, #0]
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #156]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L25
.L24:
	ldr	r1, [pc, #152]
	movs	r0, #4
	bl	Func_080bbabc
.L25:
	bl	Func_080bb938
	bl	Func_080c1a14
	b.n	.L26
.L23:
	movs	r1, #200
	ldr	r0, [pc, #136]
	lsls	r1, r1, #4
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L27
	ldr	r3, [r7, #88]
	movs	r2, #128
	lsls	r2, r2, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L28
	adds	r0, r5, #0
	bl	Func_080c9008
	b.n	.L29
.L28:
	adds	r0, r5, #0
	bl	Func_080c9018
	b.n	.L29
.L27:
	bl	Func_080c1a14
.L29:
	bl	Func_080be02c
	adds	r6, r5, #0
	mov	r0, r8
	movs	r1, #1
	bl	Func_08009080
	ldr	r3, [r6, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L26
	movs	r7, #36
.L30:
	ldrsh	r0, [r6, r7]
	str	r4, [sp, #0]
	bl	Func_080b8000
	adds	r5, r6, #0
	ldr	r4, [sp, #0]
	ldr	r3, [r5, #20]
	adds	r4, #1
	adds	r7, #2
	cmp	r4, r3
	bne.n	.L30
.L26:
	movs	r0, #0
	add	sp, #88
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0xffffe800
	.4byte 0xffffdc00
	.4byte 0x03001e74
	.4byte 0x00000856
	.4byte 0x00000855
	.4byte 0x080bd899
