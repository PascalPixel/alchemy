@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08029554
	.thumb_func
Func_08029554:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	movs	r2, #1
	movs	r1, #0
	str	r2, [sp, #4]
	ldr	r3, [pc, #500]
	mov	sl, r1
	mov	fp, r1
	mov	r9, r1
	add	r1, sp, #4
	ldr	r3, [r3, #0]
	ldrh	r1, [r1, #0]
	movs	r0, #1
	strh	r1, [r3, #4]
	bl	Func_080030f8
.L20:
	ldr	r2, [pc, #480]
	ldr	r3, [r2, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #1
	movs	r1, #1
	negs	r3, r3
	str	r1, [sp, #4]
	add	fp, r3
.L0:
	ldr	r2, [pc, #460]
	ldr	r3, [r2, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	str	r3, [sp, #4]
	add	fp, r3
.L1:
	ldr	r1, [pc, #440]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r2, #1
	movs	r3, #1
	negs	r2, r2
	str	r3, [sp, #4]
	add	r9, r2
.L2:
	ldr	r1, [pc, #416]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r2, #1
	str	r2, [sp, #4]
	add	r9, r2
.L3:
	ldr	r1, [pc, #396]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	b.n	.L5
.L4:
	ldr	r3, [r1, #0]
	movs	r5, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L6
	b.n	.L5
.L6:
	ldr	r2, [sp, #4]
	cmp	r2, #0
	bne.n	.L7
	b.n	.L8
.L7:
	mov	r2, fp
	movs	r3, #0
	adds	r2, #8
	str	r3, [sp, #4]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L9
	mov	r3, fp
	adds	r3, #15
.L9:
	asrs	r3, r3, #3
	lsls	r3, r3, #3
	mov	r0, r9
	subs	r2, r2, r3
	movs	r1, #3
	adds	r0, #3
	mov	fp, r2
	bl	Func_080022fc
	movs	r1, #2
	mov	r9, r0
	mov	r0, sl
	bl	Func_08016418
	movs	r1, #0
	movs	r0, #10
	movs	r2, #18
	movs	r3, #12
	str	r5, [sp, #0]
	bl	Func_080162d4
	mov	r1, r9
	mov	sl, r0
	cmp	r1, #0
	bne.n	.L10
	ldr	r0, [pc, #300]
	b.n	.L11
.L10:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L12
	ldr	r0, [pc, #292]
.L11:
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e940
	b.n	.L13
.L12:
	ldr	r0, [pc, #284]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e940
.L13:
	ldr	r0, [pc, #276]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #8
	bl	Func_0801e940
	movs	r3, #8
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	mov	r2, sl
	movs	r3, #40
	bl	Func_0801ea08
	mov	r1, fp
	lsls	r1, r1, #5
	mov	r8, r1
	movs	r2, #8
	str	r2, [sp, #0]
	mov	r0, r8
	movs	r1, #3
	mov	r2, sl
	movs	r3, #64
	bl	Func_0801ea08
	ldr	r0, [pc, #228]
	mov	r1, sl
	movs	r2, #88
	movs	r3, #8
	bl	Func_0801e940
	movs	r3, #8
	mov	r0, r8
	str	r3, [sp, #0]
	adds	r0, #31
	movs	r1, #3
	mov	r2, sl
	movs	r3, #96
	bl	Func_0801ea08
	movs	r5, #0
.L19:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	adds	r2, r5, #0
	cmp	r5, #0
	bge.n	.L14
	adds	r2, r5, #7
.L14:
	asrs	r2, r2, #3
	lsls	r3, r2, #3
	lsls	r2, r2, #4
	subs	r3, r5, r3
	adds	r6, r2, #0
	mov	r1, r9
	lsls	r7, r3, #4
	adds	r6, #16
	cmp	r1, #0
	bne.n	.L15
	mov	r2, r8
	adds	r0, r2, r5
	str	r1, [sp, #0]
	add	r2, sp, #12
	movs	r1, #1
	add	r3, sp, #8
	bl	Func_08019fcc
	b.n	.L16
.L15:
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L17
	mov	r1, r8
	movs	r3, #0
	adds	r0, r1, r5
	str	r3, [sp, #0]
	movs	r1, #1
	add	r2, sp, #12
	add	r3, sp, #8
	bl	Func_0801a404
.L16:
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #23
	mov	r2, sl
	adds	r3, r7, #0
	str	r6, [sp, #0]
	bl	Func_0801eadc
	b.n	.L18
.L17:
	bl	Func_08004080
	movs	r1, #0
	adds	r2, r0, #0
	adds	r0, r5, #0
	str	r2, [sp, #12]
	bl	Func_0801a2ec
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #23
	mov	r2, sl
	adds	r3, r7, #0
	str	r6, [sp, #0]
	bl	Func_0801eadc
.L18:
	adds	r5, #1
	cmp	r5, #31
	ble.n	.L19
.L8:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L20
.L5:
	mov	r0, sl
	movs	r1, #2
	bl	Func_08016418
	ldr	r3, [pc, #24]
	ldr	r2, [r3, #0]
	movs	r3, #0
	movs	r0, #0
	strh	r3, [r2, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e68
	.4byte 0x03001b04
	.4byte 0x08037440
	.4byte 0x08037448
	.4byte 0x08037450
	.4byte 0x08037458
	.4byte 0x08037460
