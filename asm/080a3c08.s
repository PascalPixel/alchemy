@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3c08
	.thumb_func
Func_080a3c08:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #80]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #0]
	movs	r2, #31
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L0
	ldr	r1, [pc, #72]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r7, #0
.L3:
	asrs	r5, r7, #24
	movs	r2, #130
	lsls	r3, r5, #1
	lsls	r2, r2, #2
	movs	r1, #188
	adds	r3, r3, r2
	lsls	r1, r1, #1
	ldrh	r0, [r6, r3]
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #28]
	ands	r1, r3
	bl	Func_08077218
	cmp	r0, #0
	beq.n	.L1
	movs	r2, #138
	lsls	r3, r5, #2
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r6, r3]
	movs	r1, #3
	bl	Func_08009020
	b.n	.L2
	.4byte 0x000001ff
	.4byte 0x03001f2c
	.4byte 0x03001e40
	.4byte 0x00000219
.L1:
	movs	r1, #138
	lsls	r1, r1, #1
	lsls	r3, r5, #2
	adds	r3, r3, r1
	ldr	r0, [r6, r3]
	movs	r1, #1
	bl	Func_08009020
.L2:
	movs	r2, #128
	lsls	r2, r2, #17
	ldr	r1, [pc, #20]
	adds	r3, r7, r2
	adds	r7, r3, #0
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	asrs	r2, r7, #24
	cmp	r2, r3
	blt.n	.L3
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000219
