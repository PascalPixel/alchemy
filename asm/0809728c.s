@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809728c
	.thumb_func
Func_0809728c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #212]
	ldr	r6, [r3, #0]
	subs	r3, #116
	ldr	r5, [r6, #16]
	ldr	r3, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #20
	mov	sl, r3
	movs	r2, #28
	ldrsh	r7, [r6, r2]
	bl	Func_08009080
	ldr	r3, [r5, #8]
	str	r3, [r5, #56]
	ldr	r3, [r5, #12]
	str	r3, [r5, #60]
	ldr	r3, [r5, #16]
	str	r3, [r5, #64]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	movs	r3, #34
	adds	r3, r3, r6
	mov	r8, r3
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #212
	bl	Func_080f9010
	ldr	r3, [pc, #152]
	str	r3, [r5, #108]
.L0:
	adds	r3, r6, #0
	adds	r3, #35
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r5, #0
	movs	r1, #1
	movs	r2, #0
	bl	Func_08096cdc
	adds	r0, r7, #0
	movs	r1, #4
	bl	Func_08015120
	adds	r3, r6, #0
	adds	r3, #33
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L2
	ldr	r2, [pc, #104]
	adds	r3, r6, r2
	ldr	r0, [pc, #104]
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	bl	Func_08015040
	b.n	.L3
.L2:
	ldr	r2, [pc, #88]
	adds	r3, r6, r2
	ldr	r0, [pc, #88]
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	bl	Func_08015040
.L3:
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #16
	bl	Func_08096cdc
.L1:
	movs	r0, #160
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L4
	mov	r2, r8
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L5
	ldr	r3, [pc, #52]
	str	r3, [r5, #108]
.L5:
	adds	r0, r5, #0
	movs	r1, #21
	bl	Func_08009080
	b.n	.L6
.L4:
	bl	Func_08097174
.L6:
	ldr	r2, [pc, #36]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x08096f15
	.4byte 0x0000071c
	.4byte 0x00000926
	.4byte 0x08096f51
	.4byte 0x00000cc7
