@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a370
	.thumb_func
Func_0801a370:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldr	r0, [pc, #68]
	ldr	r3, [pc, #72]
	ands	r0, r5
	ldr	r6, [r3, #0]
	bl	Func_08077018
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [pc, #60]
	ldrh	r3, [r0, #6]
	adds	r1, r6, r2
	ldr	r2, [pc, #60]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	str	r3, [r1, #0]
	b.n	.L1
.L0:
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #44]
	ldr	r2, [r2, #0]
	adds	r3, r6, r1
	str	r2, [r3, #0]
.L1:
	movs	r2, #192
	lsls	r2, r2, #3
	ldr	r1, [pc, #40]
	adds	r3, r6, r2
	movs	r2, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_0801a5a4
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x000001ff
	.4byte 0x03001e94
	.4byte 0x00000604
	.4byte 0x08029ee4
	.4byte 0x00000602
