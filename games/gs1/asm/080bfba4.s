@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bfba4
	.thumb_func
Func_080bfba4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #12]
	ldrb	r0, [r0, #0]
	movs	r1, #0
	mov	r8, r0
	str	r1, [sp, #4]
	bl	Func_08077008
	mov	r2, r8
	str	r0, [sp, #8]
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L0
	movs	r0, #1
.L0:
	bl	Func_08077000
	adds	r3, r0, #0
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r6, r3, #0
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #4]
	adds	r6, #8
	movs	r7, #0
	cmp	r1, r3
	bge.n	.L1
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	adds	r5, r6, #0
.L3:
	ldrb	r3, [r5, #2]
	cmp	r3, r8
	bne.n	.L2
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, sl
	bne.n	.L2
	ldrb	r1, [r5, #0]
	ldrb	r2, [r5, #1]
	mov	r0, r8
	bl	Func_080771b8
.L2:
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	adds	r7, #1
	adds	r5, #4
	cmp	r7, r3
	blt.n	.L3
.L1:
	movs	r0, #1
	movs	r1, #0
	bl	Func_080b6b40
	cmp	r0, #0
	beq.n	.L4
	movs	r0, #2
	movs	r1, #0
	bl	Func_080b6b40
	cmp	r0, #0
	beq.n	.L4
	movs	r1, #1
	str	r1, [sp, #4]
.L4:
	mov	r2, r8
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L5
	movs	r0, #1
.L5:
	bl	Func_08077000
	mov	r3, sp
	adds	r3, #16
	str	r3, [sp, #0]
	adds	r6, r0, #0
	ldr	r0, [sp, #0]
	adds	r6, #8
	movs	r2, #0
	add	r3, sp, #28
	mov	ip, r0
.L6:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L6
	movs	r3, #128
	lsls	r3, r3, #1
	movs	r1, #2
	adds	r7, r6, r3
	negs	r1, r1
	mov	r9, r1
	mov	fp, r7
.L14:
	movs	r2, #1
	ldr	r3, [r7, #0]
	negs	r2, r2
	movs	r4, #0
	mov	ip, r2
	cmp	r4, r3
	bge.n	.L7
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, r9
	bne.n	.L8
	ldrb	r3, [r6, #2]
	mov	ip, r3
	b.n	.L7
.L8:
	ldr	r3, [r7, #0]
	adds	r4, #1
	cmp	r4, r3
	bge.n	.L7
	lsls	r3, r4, #2
	adds	r2, r6, r3
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r9
	bne.n	.L8
	ldrb	r2, [r2, #2]
	mov	ip, r2
.L7:
	movs	r3, #1
	negs	r3, r3
	cmp	ip, r3
	beq.n	.L9
	adds	r5, r3, #0
	ldr	r3, [r7, #0]
	cmp	r3, #0
	ble.n	.L10
	mov	r0, fp
	ldr	r4, [r0, #0]
	adds	r2, r6, #0
.L12:
	ldrb	r3, [r2, #2]
	cmp	r3, ip
	bne.n	.L11
	movs	r3, #3
	ldrsb	r3, [r2, r3]
	cmp	r3, r5
	ble.n	.L11
	adds	r5, r3, #0
.L11:
	subs	r4, #1
	adds	r2, #4
	cmp	r4, #0
	bne.n	.L12
.L10:
	adds	r5, #1
	cmp	r5, #1
	bgt.n	.L13
	movs	r5, #2
.L13:
	ldr	r3, [r7, #0]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L14
	movs	r2, #128
	movs	r1, #2
	lsls	r2, r2, #1
	negs	r1, r1
	adds	r2, r2, r6
	ldr	r0, [sp, #0]
	mov	sl, r1
	mov	lr, r2
	adds	r1, r6, #0
.L16:
	ldrb	r3, [r1, #2]
	cmp	r3, ip
	bne.n	.L15
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, sl
	bne.n	.L15
	ldrb	r2, [r1, #0]
	strb	r5, [r1, #3]
	lsls	r2, r2, #2
	ldr	r3, [r0, r2]
	adds	r3, #1
	str	r3, [r0, r2]
	adds	r5, #1
.L15:
	mov	r2, lr
	ldr	r3, [r2, #0]
	adds	r4, #1
	adds	r1, #4
	cmp	r4, r3
	blt.n	.L16
	b.n	.L14
.L9:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L17
	b.n	.L18
.L17:
	movs	r5, #166
	movs	r0, #0
	lsls	r5, r5, #1
	mov	sl, r0
	adds	r0, r5, #0
	bl	Func_08004970
	adds	r2, r5, #0
	ldr	r3, [pc, #604]
	ldr	r1, [sp, #8]
	mov	r9, r0
	bl	Func_080072f0
	movs	r7, #1
	ldr	r2, [sp, #0]
	negs	r7, r7
	movs	r6, #0
.L20:
	ldmia	r2!, {r3}
	cmp	r3, sl
	ble.n	.L19
	mov	sl, r3
	adds	r7, r6, #0
.L19:
	adds	r6, #1
	cmp	r6, #3
	ble.n	.L20
	cmp	r7, #0
	blt.n	.L21
	movs	r1, #150
	lsls	r1, r1, #1
	ldr	r0, [sp, #8]
	adds	r2, r7, r1
	ldrsb	r3, [r0, r2]
	cmp	r3, sl
	bge.n	.L21
	mov	r1, sl
	strb	r1, [r0, r2]
.L21:
	mov	r0, r8
	bl	Func_08077010
	movs	r6, #0
	movs	r7, #72
.L23:
	ldr	r3, [sp, #8]
	mov	r1, r9
	ldrsh	r2, [r7, r3]
	ldrsh	r3, [r7, r1]
	subs	r5, r2, r3
	cmp	r5, #0
	ble.n	.L22
	bl	Func_080bdfec
	movs	r0, #25
	bl	Func_080bd808
	mov	r1, r8
	movs	r0, #0
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	movs	r1, #175
	movs	r0, #14
	bl	Func_080bbabc
	ldr	r1, [pc, #496]
	movs	r0, #4
	adds	r1, r6, r1
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #11
	bl	Func_080bbabc
	movs	r0, #212
	bl	Func_080f9010
	mov	r0, r8
	bl	Func_080b7dd0
	movs	r1, #3
	ldr	r0, [r0, #0]
	bl	Func_08009080
	mov	r0, r8
	bl	Func_080b7dd0
	movs	r1, #32
	ldr	r0, [r0, #0]
	bl	Func_08009088
	mov	r3, sl
	adds	r1, r6, #0
	movs	r2, #2
	subs	r3, #1
	mov	r0, r8
	bl	Func_080c1798
	bl	Func_080be02c
.L22:
	adds	r6, #1
	adds	r7, #4
	cmp	r6, #3
	ble.n	.L23
	mov	r0, r9
	bl	Func_08002df0
	ldr	r1, [sp, #4]
	cmp	r1, #0
	bne.n	.L24
	b.n	.L18
.L24:
	bl	Func_080bdfec
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #96]
	cmp	r3, #0
	beq.n	.L25
	mov	r1, r8
	movs	r0, #8
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r3, [sp, #12]
	movs	r0, #1
	ldr	r1, [r3, #96]
	bl	Func_080bbabc
	ldr	r1, [pc, #368]
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r0, [sp, #12]
	ldr	r1, [r0, #96]
	mov	r0, r8
	negs	r1, r1
	bl	Func_08077118
	cmp	r0, #0
	bne.n	.L26
	mov	r1, r8
	movs	r0, #9
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, r8
	cmp	r1, #7
	bhi.n	.L27
	ldr	r1, [pc, #328]
	b.n	.L28
.L27:
	ldr	r1, [pc, #328]
.L28:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L25
.L26:
	movs	r0, #11
	mov	r1, r8
	bl	Func_080bbabc
.L25:
	bl	Func_080bb938
	bl	Func_080bdfec
	ldr	r3, [pc, #304]
	ldr	r2, [sp, #8]
	adds	r6, r2, r3
	movs	r0, #0
	ldrsb	r0, [r6, r0]
	cmp	r0, #0
	beq.n	.L29
	movs	r1, #52
	ldrsh	r3, [r2, r1]
	movs	r1, #10
	muls	r0, r3
	bl	Func_080022ec
	ldr	r3, [pc, #284]
	adds	r7, r0, #0
	mov	r1, r8
	movs	r0, #8
	ldr	r5, [r3, #0]
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	Func_080bbabc
	adds	r1, r7, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #256]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L30
	movs	r3, #130
	lsls	r3, r3, #4
	adds	r2, r5, r3
	movs	r3, #134
	b.n	.L31
.L30:
	movs	r0, #130
	lsls	r0, r0, #4
	adds	r2, r5, r0
	movs	r3, #133
.L31:
	str	r3, [r2, #0]
	negs	r1, r7
	mov	r0, r8
	bl	Func_08077118
	cmp	r0, #0
	bne.n	.L32
	mov	r1, r8
	movs	r0, #9
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, r8
	cmp	r1, #7
	bhi.n	.L33
	ldr	r1, [pc, #172]
	b.n	.L34
.L33:
	ldr	r1, [pc, #172]
.L34:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L29
.L32:
	movs	r0, #11
	mov	r1, r8
	bl	Func_080bbabc
.L29:
	bl	Func_080bb938
	bl	Func_080bdfec
	ldr	r3, [pc, #160]
	ldr	r2, [sp, #8]
	adds	r1, r2, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L35
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	bne.n	.L35
	movs	r1, #192
	lsls	r1, r1, #24
	mov	r0, r8
	bl	Func_08077118
	cmp	r0, #0
	bne.n	.L35
	mov	r1, r8
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r5, [pc, #120]
	movs	r0, #4
	adds	r1, r5, #0
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #8
	bl	Func_080bbabc
	mov	r1, r8
	movs	r0, #9
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, r8
	bl	Func_080bbabc
	mov	r0, r8
	cmp	r0, #7
	bhi.n	.L36
	subs	r1, r5, #3
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L35
.L36:
	adds	r1, r5, #3
	movs	r0, #4
	bl	Func_080bbabc
.L35:
	bl	Func_080bb938
.L18:
	mov	r0, r8
	bl	Func_08077010
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001388
	.4byte 0x00000879
	.4byte 0x0000084b
	.4byte 0x00000825
	.4byte 0x0000082b
	.4byte 0x00000131
	.4byte 0x03001e74
	.4byte 0x00000851
	.4byte 0x00000141
	.4byte 0x00000828
