@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a35f8
	.thumb_func
Func_080a35f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #28]
	ldr	r3, [pc, #276]
	ldr	r3, [r3, #0]
	movs	r1, #28
	ldrsb	r1, [r3, r1]
	movs	r2, #30
	ldrsb	r2, [r3, r2]
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #24]
	str	r1, [sp, #20]
	str	r1, [sp, #12]
	str	r1, [sp, #8]
	mov	r2, r8
	lsls	r7, r2, #1
	mov	sl, r3
	ldrh	r0, [r7, r0]
	movs	r3, #1
	mov	fp, r3
	bl	Func_08077008
	mov	r5, sl
	movs	r3, #10
	adds	r5, #32
	str	r0, [sp, #16]
	str	r3, [sp, #0]
	movs	r7, #2
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	str	r7, [sp, #4]
	bl	Func_080a10d0
	cmp	r0, #0
	beq.n	.L0
	ldr	r1, [r5, #0]
	mov	r0, sl
	bl	Func_080a33d4
.L0:
	mov	r6, sl
	movs	r3, #4
	adds	r6, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #13
	movs	r2, #13
	movs	r3, #17
	str	r7, [sp, #4]
	bl	Func_080a10d0
	cmp	r0, #0
	beq.n	.L1
	ldr	r3, [sp, #20]
	ldr	r2, [r6, #0]
	movs	r0, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #0
	bl	Func_080150d0
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, sl
	str	r0, [r3, #0]
	movs	r3, #13
	strb	r3, [r0, #5]
.L1:
	ldr	r5, [pc, #148]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	Func_08015080
	ldr	r1, [r6, #0]
	movs	r2, #0
	movs	r3, #8
	adds	r0, r5, #0
	bl	Func_08015080
	mov	r1, sl
	ldr	r3, [r1, #20]
	mov	r2, fp
	strb	r2, [r3, #5]
	b.n	.L2
.L18:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #24]
	add	r0, r8
	bl	Func_080022fc
	lsls	r7, r0, #1
	mov	r8, r0
	adds	r0, r7, r0
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	Func_080a1a40
	mov	r3, fp
	mov	r9, r7
	cmp	r3, #0
	beq.n	.L3
	movs	r1, #0
	ldr	r3, [sp, #28]
	str	r1, [sp, #12]
	adds	r5, r7, r3
	mov	r2, sl
	ldrh	r0, [r5, #0]
	mov	fp, r1
	ldr	r6, [r2, #36]
	bl	Func_08077008
	ldr	r1, [sp, #8]
	str	r0, [sp, #16]
	cmp	r1, #0
	beq.n	.L4
	ldrh	r0, [r5, #0]
	bl	Func_08077008
	movs	r1, #228
	lsls	r1, r1, #1
	add	r1, sl
	movs	r2, #0
	bl	Func_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, sl
	strb	r0, [r3, #0]
	ldrh	r0, [r5, #0]
	bl	Func_080a38a8
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_080a112c
	b.n	.L5
	.4byte 0x03001f2c
	.4byte 0x00000b87
.L4:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	bl	Func_080a3e88
	ldrh	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080a112c
.L5:
	movs	r3, #165
	lsls	r3, r3, #1
	ldr	r1, [pc, #48]
	movs	r2, #3
	add	r3, sl
.L6:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L6
	movs	r2, #162
	lsls	r2, r2, #1
	ldr	r3, [pc, #32]
	add	r2, r9
	mov	r1, sl
	strh	r3, [r1, r2]
.L3:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r6, [pc, #24]
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	ldr	r3, [pc, #16]
	movs	r2, #128
	b.n	.L8
	.4byte 0x0000001e
	.4byte 0x0000001a
	.4byte 0x03001c94
	.4byte 0x03001ae8
.L8:
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L9
	ldr	r2, [sp, #12]
	adds	r2, #4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L10
	ldr	r3, [sp, #12]
	adds	r3, #7
.L10:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	ldr	r0, [sp, #16]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	adds	r1, r3, #0
	adds	r0, #216
	str	r3, [sp, #12]
	bl	Func_080a1e38
	ldr	r3, [sp, #12]
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	str	r3, [sp, #12]
	ldr	r3, [sp, #28]
	mov	r2, r9
	ldrh	r0, [r2, r3]
	movs	r1, #0
	bl	Func_080a3e88
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L7
.L9:
	ldr	r5, [sp, #28]
	add	r5, r9
	ldrh	r0, [r5, #0]
	bl	Func_080a3d6c
	cmp	r0, #0
	beq.n	.L11
	movs	r0, #112
	bl	Func_080f9010
	ldrh	r5, [r5, #0]
	str	r5, [sp, #20]
	b.n	.L12
.L11:
	movs	r0, #114
	bl	Func_080f9010
.L7:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #113
	bl	Func_080f9010
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #20]
	b.n	.L12
.L13:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	movs	r3, #1
	str	r3, [sp, #8]
	mov	fp, r3
.L14:
	ldr	r3, [pc, #128]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L15
	ldr	r1, [sp, #8]
	cmp	r1, #1
	bne.n	.L15
	movs	r2, #0
	movs	r3, #1
	str	r2, [sp, #8]
	mov	fp, r3
.L15:
	ldr	r5, [pc, #108]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L16
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	add	r8, r1
	mov	fp, r2
.L16:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	add	r8, r3
	mov	fp, r3
.L2:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L17
	b.n	.L18
.L17:
	mov	r1, r8
	lsls	r7, r1, #1
.L12:
	mov	r3, sl
	mov	r2, r8
	strb	r2, [r3, #28]
	ldr	r1, [sp, #28]
	ldrh	r2, [r7, r1]
	str	r2, [r3, #8]
	ldr	r3, [pc, #32]
	add	r3, sl
	strb	r2, [r3, #0]
	ldr	r0, [sp, #20]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x0000021a
