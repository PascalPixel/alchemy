@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b211c
	.thumb_func
Func_080b211c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r0, [sp, #20]
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	mov	fp, r3
	bl	Func_08077008
	movs	r2, #1
	movs	r3, #2
	str	r0, [sp, #8]
	str	r3, [sp, #0]
	mov	r9, r2
	movs	r0, #15
	movs	r1, #8
	movs	r2, #15
	movs	r3, #4
	bl	Func_08015010
	str	r0, [sp, #12]
	movs	r7, #0
.L18:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #3
	movs	r0, #0
	movs	r1, #5
	bl	Func_08015010
	movs	r3, #224
	str	r0, [sp, #16]
	lsls	r3, r3, #2
	add	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #234
	lsls	r2, r2, #2
	movs	r3, #12
	add	r2, fp
	strb	r3, [r2, #0]
	movs	r3, #1
	mov	sl, r3
.L16:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L0
	movs	r3, #0
	ldr	r0, [sp, #20]
	mov	sl, r3
	bl	Func_08077248
	mov	r9, r0
	mov	r3, r9
	subs	r3, #1
	cmp	r7, r3
	ble.n	.L1
	adds	r7, r3, #0
.L1:
	ldr	r2, [sp, #8]
	lsls	r4, r7, #1
	adds	r4, #216
	ldrh	r3, [r2, r4]
	ldr	r6, [pc, #60]
	ands	r6, r3
	mov	r3, fp
	ldr	r3, [r3, #32]
	movs	r1, #5
	adds	r0, r7, #0
	str	r4, [sp, #4]
	mov	r8, r3
	bl	Func_080022fc
	movs	r1, #5
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080022ec
	adds	r2, r0, #0
	lsls	r5, r5, #4
	lsls	r2, r2, #4
	adds	r1, r5, #0
	adds	r2, #8
	mov	r0, r8
	bl	Func_080b0a6c
	movs	r2, #234
	lsls	r2, r2, #2
	movs	r3, #3
	add	r2, fp
	strb	r3, [r2, #0]
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #4]
	b.n	.L2
	.4byte 0x000001ff
	.4byte 0x03001f2c
.L2:
	ldrh	r0, [r2, r4]
	bl	Func_080b20e8
	adds	r1, r6, #0
	adds	r2, r0, #0
	movs	r3, #2
	ldr	r0, [sp, #12]
	bl	Func_080b110c
	ldr	r3, [pc, #284]
	adds	r6, r6, r3
	ldr	r0, [sp, #16]
	adds	r1, r6, #0
	bl	Func_080b11a4
.L0:
	ldr	r1, [pc, #276]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #112
	bl	Func_080f9010
	movs	r5, #0
	b.n	.L4
.L3:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r0, #113
	movs	r5, #1
	bl	Func_080f9010
	negs	r5, r5
	b.n	.L4
.L5:
	ldr	r5, [pc, #236]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r0, #111
	bl	Func_080f9010
	subs	r7, #1
	mov	r3, r9
	adds	r0, r7, r3
	mov	r1, r9
	bl	Func_080022fc
	movs	r2, #1
	adds	r7, r0, #0
	mov	sl, r2
.L6:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #111
	bl	Func_080f9010
	adds	r7, #1
	mov	r3, r9
	adds	r0, r7, r3
	mov	r1, r9
	bl	Func_080022fc
	movs	r2, #1
	adds	r7, r0, #0
	mov	sl, r2
.L7:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	subs	r7, #5
	cmp	r7, #0
	bge.n	.L9
	adds	r7, #15
.L9:
	cmp	r7, r9
	blt.n	.L10
.L11:
	subs	r7, #5
	cmp	r7, r9
	bge.n	.L11
.L10:
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	mov	sl, r3
.L8:
	ldr	r3, [pc, #128]
	ldr	r3, [r3, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	adds	r7, #5
	cmp	r7, r9
	blt.n	.L13
	subs	r7, #15
.L13:
	cmp	r7, #0
	bge.n	.L14
.L15:
	adds	r7, #5
	cmp	r7, #0
	blt.n	.L15
.L14:
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	sl, r2
.L12:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L16
.L4:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	cmp	r5, #0
	bne.n	.L17
	ldr	r0, [sp, #20]
	adds	r1, r7, #0
	bl	Func_080b2328
	ldr	r0, [pc, #56]
	bl	Func_080b04dc
	ldr	r0, [sp, #20]
	bl	Func_08077248
	cmp	r0, #0
	beq.n	.L17
	b.n	.L18
.L17:
	ldr	r0, [sp, #12]
	movs	r1, #2
	bl	Func_08015018
	adds	r0, r5, #0
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000075
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x00000cc2
