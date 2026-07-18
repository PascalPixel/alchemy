@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08090584
	.thumb_func
Func_08090584:
	push	{lr}
	ldr	r3, [pc, #104]
	ldrh	r3, [r3, #0]
	adds	r4, r3, #0
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #0]
	mov	ip, r3
.L7:
	movs	r0, #132
	lsls	r0, r0, #1
	add	r0, ip
	ldrh	r3, [r0, #0]
	cmp	r3, #1
	beq.n	.L0
	cmp	r3, #1
	bgt.n	.L1
	cmp	r3, #0
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r3, #2
	beq.n	.L4
	cmp	r3, #3
	bne.n	.L3
	movs	r3, #130
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L3
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #52]
	ands	r3, r2
	ldr	r2, [pc, #36]
	orrs	r3, r2
	b.n	.L5
.L4:
	movs	r3, #131
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L3
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #24]
	ands	r3, r2
.L5:
	strh	r3, [r1, #0]
	movs	r3, #9
	strh	r3, [r0, #0]
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000002
	.4byte 0x04000006
	.4byte 0x03001e70
	.4byte 0x0000fff8
.L0:
	movs	r3, #130
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L6
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #52]
	ands	r3, r2
	ldr	r2, [pc, #44]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	b.n	.L7
.L6:
	movs	r3, #131
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L3
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #16]
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #3
	strh	r3, [r0, #0]
	b.n	.L7
	movs	r0, r0
	.4byte 0x00000002
	.4byte 0x0000fff8
.L2:
	cmp	r4, #158
	bhi.n	.L3
	movs	r3, #1
	strh	r3, [r0, #0]
	b.n	.L7
.L3:
	pop	{r0}
	bx	r0
