@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021cb8
	.thumb_func
Func_08021cb8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r1, #0
	movs	r1, #193
	lsls	r1, r1, #3
	adds	r7, r0, #0
	movs	r0, #17
	mov	r8, r2
	bl	Func_080048b0
	adds	r6, r0, #0
	ldr	r0, [pc, #156]
	bl	Func_08002f40
	ldr	r3, [pc, #152]
	lsls	r5, r5, #1
	adds	r2, r6, r3
	ldrh	r3, [r5, r0]
	adds	r0, r0, r3
	str	r0, [r2, #0]
	adds	r1, r6, #0
	bl	Func_080053e8
	movs	r0, #128
	lsls	r0, r0, #3
	bl	Func_08004938
	movs	r2, #0
	mov	lr, r0
	mov	r5, lr
	mov	ip, r2
.L2:
	ldrb	r4, [r6, #0]
	ldrb	r2, [r7, r4]
	adds	r3, r2, #0
	adds	r6, #1
	cmp	r3, #255
	bne.n	.L0
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r0, r7, r3
	ldr	r3, [r0, #0]
	strb	r3, [r7, r4]
	ldr	r1, [r0, #0]
	cmp	r1, #63
	bgt.n	.L1
	movs	r3, #160
	lsls	r2, r1, #1
	lsls	r3, r3, #19
	adds	r3, r3, r2
	ldr	r2, [pc, #88]
	mov	sl, r3
	lsls	r3, r4, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	mov	r2, sl
	strh	r3, [r2, #0]
	adds	r3, r1, #1
	str	r3, [r0, #0]
	ldrb	r2, [r7, r4]
	b.n	.L0
.L1:
	adds	r2, r3, #0
.L0:
	strb	r2, [r5, #0]
	movs	r3, #1
	movs	r2, #128
	add	ip, r3
	lsls	r2, r2, #3
	adds	r5, #1
	cmp	ip, r2
	blt.n	.L2
	mov	r3, r8
	ldr	r2, [pc, #48]
	lsls	r1, r3, #6
	adds	r1, r1, r2
	ldr	r3, [pc, #48]
	mov	r0, lr
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, lr
	bl	Func_08002df0
	movs	r0, #17
	bl	Func_08002dd8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000000f1
	.4byte 0x00000604
	.4byte 0x05000200
	.4byte 0x06004000
	.4byte 0x040000d4
	.4byte 0x84000100
