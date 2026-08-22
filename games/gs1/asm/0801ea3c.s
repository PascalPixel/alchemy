@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ea3c
	.thumb_func
Func_0801ea3c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r3
	ldr	r3, [pc, #128]
	sub	sp, #32
	adds	r4, r0, #0
	ldr	r5, [sp, #56]
	ldr	r3, [r3, #0]
	adds	r6, r1, #0
	adds	r7, r2, #0
	add	r0, sp, #16
	adds	r1, r4, #0
	movs	r2, #4
	mov	sl, r3
	bl	Func_08017dd4
	cmp	r5, #0
	bne.n	.L0
	ldr	r3, [pc, #100]
	b.n	.L1
.L0:
	ldr	r3, [pc, #100]
.L1:
	mov	r4, sp
	strh	r3, [r4, #0]
	ldr	r3, [pc, #100]
	strh	r3, [r4, #2]
	adds	r2, r4, #4
	movs	r1, #4
.L2:
	ldrb	r3, [r0, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L2
	movs	r3, #0
	mov	r1, r8
	strh	r3, [r4, #12]
	ldrh	r3, [r6, #14]
	lsrs	r2, r1, #3
	adds	r3, r3, r2
	ldrh	r2, [r6, #12]
	lsrs	r1, r7, #3
	adds	r3, #1
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	adds	r1, r3, #1
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r1, r3
	bcs.n	.L3
	ldr	r3, [pc, #48]
	lsls	r1, r1, #1
	adds	r2, r1, r3
	movs	r3, #7
	add	r1, sl
	ands	r3, r7
	adds	r0, r4, #0
	bl	Func_0801de5c
.L3:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x0000f01d
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.4byte 0x06002000
