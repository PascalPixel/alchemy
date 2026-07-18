@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080aa460
	.thumb_func
Func_080aa460:
	push	{lr}
	bl	Func_08077080
	ldrb	r3, [r0, #1]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L0
	cmp	r2, #11
	beq.n	.L1
	b.n	.L2
.L0:
	movs	r0, #126
	bl	Func_080a2438
.L1:
	movs	r0, #126
	bl	Func_080a2438
	b.n	.L3
.L2:
	ldrb	r3, [r0, #3]
	subs	r0, r3, #1
	cmp	r0, #31
	bhi.n	.L4
	ldr	r2, [pc, #160]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	add	r5, pc, #168
	lsrs	r2, r1, #32
	add	r5, pc, #168
	lsrs	r2, r1, #32
	add	r5, pc, #112
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #80
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #144
	lsrs	r2, r1, #32
	add	r5, pc, #168
	lsrs	r2, r1, #32
	add	r5, pc, #168
	lsrs	r2, r1, #32
	movs	r0, #82
	bl	Func_080a2438
	b.n	.L3
	movs	r0, #84
	bl	Func_080a2438
	b.n	.L3
.L4:
	movs	r0, #91
	bl	Func_080a2438
.L3:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080aa494
	movs	r0, #0
	bx	lr
