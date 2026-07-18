@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3ef0
	.thumb_func
Func_080a3ef0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	ldr	r3, [pc, #404]
	mov	r9, r1
	movs	r1, #0
	sub	sp, #4
	adds	r5, r2, #0
	ldr	r7, [r3, #0]
	mov	r8, r0
	mov	sl, r1
	bl	Func_08077008
	mov	r2, r9
	lsls	r3, r2, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	mov	fp, r0
	str	r3, [sp, #0]
	cmp	r5, #1
	bne.n	.L0
	movs	r3, #128
	lsls	r3, r3, #1
	mov	sl, r3
.L0:
	ldr	r1, [sp, #0]
	ldr	r0, [pc, #364]
	ands	r0, r1
	bl	Func_08077018
	ldrb	r3, [r0, #2]
	cmp	r3, #9
	bls.n	.L1
	b.n	.L2
.L1:
	ldr	r2, [pc, #352]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080a4000
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3ff4
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	.4byte 0x080a3f6c
	cmp	r8, r6
	bne.n	.L3
	movs	r3, #2
	b.n	.L4
.L3:
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r5, #166
	lsls	r5, r5, #1
	mov	fp, r0
	adds	r0, r5, #0
	bl	Func_08004938
	adds	r2, r5, #0
	ldr	r3, [pc, #280]
	mov	r1, fp
	mov	r8, r0
	bl	Func_080072f0
	adds	r0, r6, #0
	bl	Func_080a40ac
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L5
	ldr	r3, [pc, #264]
	ldr	r1, [sp, #0]
	adds	r0, r6, #0
	ands	r1, r3
	str	r1, [sp, #0]
	bl	Func_08077028
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L6
	movs	r3, #2
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	b.n	.L7
.L6:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
.L7:
	mov	r3, sl
	bl	Func_080a112c
	b.n	.L8
.L5:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	Func_080a112c
.L8:
	movs	r2, #166
	ldr	r3, [pc, #192]
	mov	r0, fp
	mov	r1, r8
	lsls	r2, r2, #1
	bl	Func_080072f0
	mov	r0, r8
	bl	Func_08002df0
	b.n	.L2
	cmp	r6, r8
	bne.n	.L9
	movs	r3, #4
.L4:
	mov	r2, sl
	orrs	r2, r3
	mov	sl, r2
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	Func_080a112c
	b.n	.L2
.L9:
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r5, #166
	lsls	r5, r5, #1
	mov	fp, r0
	adds	r0, r5, #0
	bl	Func_08004938
	adds	r2, r5, #0
	ldr	r3, [pc, #128]
	mov	r1, fp
	mov	r8, r0
	bl	Func_080072f0
	adds	r0, r6, #0
	bl	Func_080a40ac
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L10
	adds	r0, r6, #0
	ldr	r1, [sp, #0]
	bl	Func_08077028
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L11
	movs	r3, #4
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	b.n	.L12
.L11:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
.L12:
	mov	r3, sl
	bl	Func_080a112c
	b.n	.L13
.L10:
	ldr	r0, [r7, #36]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	Func_080a112c
.L13:
	movs	r2, #166
	ldr	r3, [pc, #44]
	mov	r0, fp
	mov	r1, r8
	lsls	r2, r2, #1
	bl	Func_080072f0
	mov	r0, r8
	bl	Func_08002df0
.L2:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x080a3f44
	.4byte 0x03001388
	.4byte 0xfffffdff
