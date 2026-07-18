@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080108e4
	.thumb_func
Func_080108e4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r3, #0
	ldr	r3, [pc, #216]
	mov	sl, r1
	mov	r8, r2
	movs	r1, #136
	ldr	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldr	r6, [r3, #0]
	mov	r1, sl
	movs	r3, #1
	ands	r1, r3
	mov	sl, r1
	lsls	r0, r0, #1
	mov	r1, r8
	ands	r1, r3
	mov	r9, r0
	mov	r8, r1
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r1, [sp, #28]
	movs	r3, #206
	lsls	r3, r3, #2
	adds	r2, r2, r3
	cmp	r1, #0
	bne.n	.L0
	ldrh	r3, [r2, #0]
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L1
.L0:
	movs	r1, #128
	strh	r5, [r2, #0]
	lsls	r1, r1, #3
	movs	r0, #14
	bl	Func_080048b0
	lsls	r3, r5, #2
	adds	r7, r0, #0
	ldr	r0, [r3, r6]
	adds	r1, r7, #0
	adds	r0, r6, r0
	bl	Func_080053e8
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #5
	add	r3, sl
	ldr	r2, [pc, #120]
	lsls	r3, r3, #6
	adds	r4, r7, #0
	adds	r5, r3, r2
	movs	r6, #0
.L2:
	ldr	r3, [pc, #116]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #1
	adds	r4, #64
	adds	r5, #128
	cmp	r6, #15
	bls.n	.L2
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L3
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #6
	add	r3, sl
	ldr	r2, [pc, #88]
	lsls	r3, r3, #5
	ldr	r5, [pc, #88]
	adds	r1, r3, r2
	adds	r4, r7, #0
	movs	r6, #0
.L5:
	movs	r0, #0
.L4:
	ldrh	r3, [r4, #0]
	lsls	r3, r3, #2
	ldrh	r2, [r3, r5]
	strh	r2, [r1, #0]
	ldr	r2, [pc, #72]
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	adds	r4, #4
	cmp	r0, #15
	bls.n	.L4
	adds	r6, #1
	adds	r1, #96
	cmp	r6, #15
	bls.n	.L5
.L3:
	movs	r0, #14
	bl	Func_08002dd8
	movs	r0, #1
.L1:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e70
	.4byte 0x02020000
	.4byte 0x040000d4
	.4byte 0x84000010
	.4byte 0x06004000
	.4byte 0x02010000
	.4byte 0x02010002
