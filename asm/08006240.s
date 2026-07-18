@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006240
	.thumb_func
Func_08006240:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #112]
	sub	sp, #8
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	lr, sp
	ldr	r0, [pc, #104]
	mov	r2, lr
	str	r3, [r2, #0]
	str	r4, [r2, #4]
	ldr	r3, [r0, #0]
	ldr	r1, [pc, #100]
	lsls	r3, r3, #25
	lsrs	r3, r3, #31
	strb	r3, [r1, #9]
	ldr	r2, [r1, #20]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L0
	ldr	r3, [pc, #84]
	ldr	r2, [r1, #44]
	strh	r3, [r0, #2]
	ldr	r3, [r1, #40]
	str	r2, [r1, #40]
	str	r3, [r1, #44]
	b.n	.L1
.L0:
	cmp	r2, #0
	blt.n	.L1
	ldr	r3, [r1, #44]
	lsls	r2, r2, #1
	ldrh	r3, [r2, r3]
	strh	r3, [r0, #2]
.L1:
	ldr	r7, [pc, #56]
	ldr	r3, [r7, #20]
	cmp	r3, #14
	bgt.n	.L2
	adds	r3, #1
	str	r3, [r7, #20]
.L2:
	movs	r6, #0
.L8:
	mov	r2, lr
	lsls	r5, r6, #1
	ldrh	r3, [r2, r5]
	ldr	r2, [pc, #40]
	cmp	r3, r2
	bne.n	.L3
	lsls	r3, r6, #2
	mov	ip, r3
	mov	r2, ip
	adds	r2, #24
	ldr	r3, [r7, r2]
	cmp	r3, #13
	ble.n	.L4
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, r2]
	b.n	.L5
	movs	r0, r0
	.4byte 0x04000120
	.4byte 0x04000128
	.4byte 0x02002240
	.4byte 0x0000fefe
.L3:
	lsls	r2, r6, #2
	mov	ip, r2
.L4:
	mov	r3, ip
	adds	r3, #24
	mov	r4, ip
	ldr	r1, [r7, r3]
	adds	r4, #48
	mov	r3, lr
	ldr	r0, [r7, r4]
	ldrh	r5, [r3, r5]
	lsls	r2, r1, #1
	strh	r5, [r2, r0]
	cmp	r1, #13
	bne.n	.L5
	mov	r3, ip
	adds	r3, #64
	ldr	r2, [r7, r3]
	adds	r1, r6, #4
	str	r0, [r7, r3]
	str	r2, [r7, r4]
	ldrb	r3, [r7, r1]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r7, r1]
.L5:
	ldr	r0, [pc, #76]
	ldrb	r3, [r0, #9]
	cmp	r3, #0
	beq.n	.L6
	adds	r3, r6, #4
	ldrb	r2, [r0, r3]
	movs	r1, #2
	orrs	r2, r1
	strb	r2, [r0, r3]
.L6:
	mov	r2, ip
	adds	r2, #24
	ldr	r3, [r0, r2]
	cmp	r3, #14
	bgt.n	.L7
	adds	r3, #1
	str	r3, [r0, r2]
.L7:
	adds	r6, #1
	adds	r7, r0, #0
	cmp	r6, #1
	ble.n	.L8
	ldrb	r3, [r0, #0]
	cmp	r3, #8
	bne.n	.L9
	ldr	r0, [pc, #36]
	ldr	r3, [pc, #16]
	ldr	r1, [pc, #36]
	strh	r3, [r0, #0]
	ldr	r2, [pc, #16]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #12]
	strh	r3, [r0, #0]
	b.n	.L9
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x000000c0
	.4byte 0x02002240
	.4byte 0x0400010e
	.4byte 0x04000128
.L9:
	add	sp, #8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
