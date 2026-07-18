@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f60a0
	.thumb_func
Func_080f60a0:
	push	{r5, lr}
	ldr	r0, [pc, #132]
	ldr	r2, [pc, #132]
	ldr	r5, [r0, #0]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	subs	r2, #164
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldr	r4, [r0, #4]
	cmp	r3, #1
	beq.n	.L1
	cmp	r3, #2
	beq.n	.L2
	b.n	.L3
.L1:
	ldr	r3, [pc, #108]
	adds	r0, r4, #0
	ldr	r1, [pc, #108]
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #108]
	movs	r1, #128
	adds	r3, r5, r2
	ldr	r2, [r3, #0]
	adds	r0, r4, #0
	ldr	r3, [pc, #100]
	lsls	r1, r1, #8
	bl	Func_080072f0
	b.n	.L3
.L2:
	ldr	r2, [pc, #88]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	cmp	r3, #50
	bne.n	.L4
	movs	r2, #128
	ldr	r1, [pc, #68]
	lsls	r2, r2, #8
	adds	r0, r4, #0
	bl	Func_0800567c
	b.n	.L3
.L4:
	movs	r2, #128
	ldr	r1, [pc, #52]
	lsls	r2, r2, #8
	adds	r0, r4, #0
	bl	Func_0800562c
.L3:
	ldr	r3, [pc, #36]
	adds	r2, r5, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, r5, r3
	movs	r3, #1
	b.n	.L5
.L0:
	ldr	r3, [pc, #44]
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
.L5:
	str	r3, [r2, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007824
	.4byte 0x040000d4
	.4byte 0x06003500
	.4byte 0x84002000
	.4byte 0x00007784
	.4byte 0x03000168
	.4byte 0x00007820
