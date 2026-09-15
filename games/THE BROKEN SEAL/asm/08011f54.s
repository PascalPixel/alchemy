@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011f54
	.thumb_func
Func_08011f54:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #108]
	adds	r5, r1, #0
	ldr	r1, [r3, #0]
	adds	r6, r2, #0
	asrs	r5, r5, #16
	asrs	r6, r6, #16
	ldr	r2, [pc, #100]
	cmp	r1, #0
	beq.n	.L0
	movs	r2, #3
	ands	r2, r0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #1
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r1, r3]
.L0:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L1
	adds	r3, #15
.L1:
	asrs	r1, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L2
	adds	r3, #15
.L2:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r1, [r2, #3]
	ldr	r3, [pc, #48]
	lsls	r1, r1, #2
	adds	r0, r1, r3
	movs	r2, #15
	ldrb	r0, [r0, #0]
	adds	r3, r2, #0
	ldr	r4, [pc, #40]
	ands	r3, r0
	ldr	r7, [pc, #40]
	ands	r5, r2
	ands	r6, r2
	lsls	r3, r3, #2
	adds	r0, r1, r7
	ldr	r3, [r4, r3]
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_080072f0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x02010000
	.4byte 0x0202c000
	.4byte 0x080134fc
	.4byte 0x0202c001
