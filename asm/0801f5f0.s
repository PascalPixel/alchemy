@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801f5f0
	.thumb_func
Func_0801f5f0:
	push	{r5, r6, r7, lr}
	adds	r6, r3, #0
	ldr	r3, [pc, #124]
	ldr	r3, [r3, #0]
	mov	ip, r3
	ldrh	r3, [r0, #12]
	adds	r3, r1, r3
	adds	r1, r3, #1
	ldrh	r3, [r0, #14]
	ldr	r7, [sp, #20]
	adds	r3, r2, r3
	adds	r2, r3, #1
	movs	r3, #1
	ands	r7, r3
	ldr	r5, [sp, #16]
	lsls	r7, r7, #12
	cmp	r1, #0
	bge.n	.L0
	adds	r6, r6, r1
	movs	r1, #0
.L0:
	adds	r3, r1, r6
	cmp	r3, #29
	ble.n	.L1
	movs	r3, #30
	subs	r6, r3, r1
.L1:
	cmp	r2, #0
	bge.n	.L2
	adds	r5, r5, r2
	movs	r2, #0
.L2:
	adds	r3, r2, r5
	cmp	r3, #29
	ble.n	.L3
	movs	r3, #20
	subs	r5, r3, r2
.L3:
	cmp	r6, #0
	ble.n	.L4
	cmp	r5, #0
	ble.n	.L4
	lsls	r2, r2, #6
	lsls	r3, r1, #1
	adds	r1, r2, r3
.L7:
	mov	r3, ip
	adds	r0, r6, #0
	adds	r4, r1, r3
	cmp	r0, #0
	beq.n	.L5
	ldr	r2, [pc, #40]
.L6:
	ldrh	r3, [r4, #0]
	ands	r3, r2
	orrs	r3, r7
	subs	r0, #1
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r0, #0
	bne.n	.L6
.L5:
	subs	r5, #1
	adds	r1, #64
	cmp	r5, #0
	bne.n	.L7
	ldr	r2, [pc, #20]
	movs	r3, #1
	add	r2, ip
	strb	r3, [r2, #0]
.L4:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0xffffefff
	.4byte 0x00000ea3
