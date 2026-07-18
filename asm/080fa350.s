@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa350
	.thumb_func
Func_080fa350:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #36]
	ldr	r1, [pc, #40]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r1, [r1, #0]
	ldr	r3, [r1, #0]
	ldr	r2, [r0, #0]
	cmp	r3, r2
	beq.n	.L0
	adds	r0, r1, #0
	adds	r1, r2, #0
	bl	Func_080faa58
	b.n	.L1
	movs	r0, r0
	.4byte 0x080fc624
	.4byte 0x080fc684
.L0:
	ldr	r2, [r1, #4]
	ldrh	r0, [r1, #4]
	cmp	r0, #0
	beq.n	.L2
	cmp	r2, #0
	bge.n	.L1
.L2:
	adds	r0, r1, #0
	adds	r1, r3, #0
	bl	Func_080faa58
.L1:
	pop	{r0}
	bx	r0
