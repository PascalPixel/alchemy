@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080119cc
	.thumb_func
Func_080119cc:
	push	{lr}
	ldr	r3, [pc, #156]
	ldr	r3, [r3, #0]
	mov	ip, r3
	mov	r4, ip
	adds	r4, #216
	ldr	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L0
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L0
.L3:
	ldrh	r2, [r4, #8]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L1
	ldr	r0, [r4, #4]
	ldr	r2, [pc, #128]
	ldrh	r1, [r0, #0]
	adds	r0, #2
	cmp	r1, r2
	bne.n	.L2
	ldr	r3, [r4, #0]
	str	r3, [r4, #4]
	b.n	.L3
.L2:
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r2, #254
	ands	r3, r1
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L4
	movs	r2, #255
	ands	r2, r1
	cmp	r2, #255
	beq.n	.L0
	ldr	r3, [r4, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r4, #4]
	b.n	.L3
.L4:
	movs	r3, #240
	lsls	r3, r3, #8
	movs	r2, #192
	ands	r3, r1
	lsls	r2, r2, #6
	cmp	r3, r2
	bne.n	.L5
	ldr	r3, [pc, #68]
	strh	r1, [r3, #0]
	ldr	r3, [pc, #68]
	add	r3, ip
	strb	r1, [r3, #0]
	ldr	r3, [r4, #4]
	adds	r3, #2
	str	r3, [r4, #4]
	b.n	.L3
.L5:
	ldr	r3, [pc, #56]
	add	r3, ip
	ldrb	r2, [r3, #0]
	movs	r3, #192
	ands	r3, r2
	cmp	r3, #64
	bne.n	.L6
	ldr	r3, [pc, #44]
	b.n	.L7
.L6:
	ldr	r3, [pc, #44]
.L7:
	strh	r1, [r3, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r4, #8]
	ldr	r3, [r4, #4]
	adds	r3, #4
	str	r3, [r4, #4]
	b.n	.L3
.L1:
	ldr	r1, [pc, #12]
	adds	r3, r2, r1
	strh	r3, [r4, #8]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x0000ffff
	.4byte 0x04000050
	.4byte 0x00000103
	.4byte 0x04000052
	.4byte 0x04000054
