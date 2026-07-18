@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b362c
	.thumb_func
Func_080b362c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	sub	sp, #24
	str	r3, [sp, #20]
	mov	fp, r0
	bl	Func_08077008
	movs	r1, #1
	movs	r2, #0
	str	r0, [sp, #8]
	str	r2, [sp, #4]
	movs	r5, #2
	movs	r2, #16
	movs	r3, #4
	mov	sl, r1
	mov	r9, r1
	movs	r0, #14
	movs	r1, #8
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r1, #5
	str	r0, [sp, #12]
	movs	r2, #30
	movs	r3, #3
	movs	r0, #0
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r2, #224
	ldr	r1, [sp, #20]
	str	r0, [sp, #16]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r7, #0
.L10:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L0
	movs	r2, #0
	mov	r0, fp
	mov	r9, r2
	bl	Func_08077248
	mov	sl, r0
	mov	r3, sl
	subs	r3, #1
	cmp	r7, r3
	ble.n	.L1
	adds	r7, r3, #0
.L1:
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #20]
	lsls	r3, r7, #1
	adds	r3, #216
	ldrh	r3, [r1, r3]
	ldr	r2, [r2, #32]
	ldr	r6, [pc, #56]
	movs	r1, #5
	adds	r0, r7, #0
	ands	r6, r3
	mov	r8, r2
	bl	Func_080022fc
	movs	r1, #5
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080022ec
	adds	r2, r0, #0
	lsls	r5, r5, #4
	lsls	r2, r2, #4
	adds	r2, #8
	mov	r0, r8
	adds	r1, r5, #0
	bl	Func_080b0a6c
	movs	r1, #234
	ldr	r3, [sp, #20]
	lsls	r1, r1, #2
	adds	r2, r3, r1
	movs	r3, #3
	strb	r3, [r2, #0]
	ldr	r0, [sp, #12]
	mov	r1, fp
	adds	r2, r7, #0
	b.n	.L2
	.4byte 0x000001ff
	.4byte 0x03001f2c
.L2:
	bl	Func_080b386c
	ldr	r3, [pc, #340]
	adds	r6, r6, r3
	ldr	r0, [sp, #16]
	adds	r1, r6, #0
	bl	Func_080b11a4
.L0:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #328]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L3
	mov	r0, fp
	adds	r1, r7, #0
	bl	Func_080772a8
	cmp	r0, #0
	bne.n	.L4
	movs	r0, #112
	bl	Func_080f9010
	str	r7, [sp, #4]
	b.n	.L5
.L4:
	movs	r2, #4
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L6
	ldr	r0, [pc, #288]
	movs	r1, #8
	movs	r2, #1
	movs	r3, #2
	bl	Func_08015038
	b.n	.L7
.L6:
	movs	r1, #3
	ldr	r3, [sp, #4]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L7
	ldr	r0, [pc, #268]
	movs	r1, #8
	movs	r2, #1
	movs	r3, #2
	bl	Func_08015038
.L7:
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L8
.L9:
	movs	r0, #1
	bl	Func_080030f8
.L8:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L9
	bl	Func_08015140
	b.n	.L10
.L3:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	movs	r0, #113
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #4]
	b.n	.L5
.L11:
	ldr	r5, [pc, #208]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	movs	r0, #111
	bl	Func_080f9010
	subs	r7, #1
	mov	r3, sl
	adds	r0, r7, r3
	mov	r1, sl
	bl	Func_080022fc
	movs	r1, #1
	adds	r7, r0, #0
	mov	r9, r1
.L12:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #111
	bl	Func_080f9010
	adds	r7, #1
	mov	r2, sl
	adds	r0, r7, r2
	mov	r1, sl
	bl	Func_080022fc
	movs	r3, #1
	adds	r7, r0, #0
	mov	r9, r3
.L13:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	subs	r7, #5
	cmp	r7, #0
	bge.n	.L15
	adds	r7, #15
.L15:
	cmp	r7, sl
	blt.n	.L16
.L17:
	subs	r7, #5
	cmp	r7, sl
	bge.n	.L17
.L16:
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	mov	r9, r1
.L14:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L18
	b.n	.L10
.L18:
	adds	r7, #5
	cmp	r7, sl
	blt.n	.L19
	subs	r7, #15
.L19:
	cmp	r7, #0
	bge.n	.L20
.L21:
	adds	r7, #5
	cmp	r7, #0
	blt.n	.L21
.L20:
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	r9, r2
	b.n	.L10
.L5:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	Func_08015018
	movs	r1, #2
	ldr	r0, [sp, #12]
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #4]
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
	.4byte 0x00000c96
	.4byte 0x00000c97
	.4byte 0x03001b04
