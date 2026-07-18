@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1ebc
	.thumb_func
Func_080c1ebc:
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	ldr	r6, [r3, #0]
	adds	r3, r6, #0
	adds	r3, #64
	ldrb	r5, [r3, #0]
	bl	Func_08077008
	ldr	r3, [pc, #124]
	mov	ip, r0
	add	r3, ip
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, ip
	movs	r1, #0
	ldrb	r0, [r3, #0]
	cmp	r1, r5
	bge.n	.L1
	ldrh	r3, [r6, #16]
	cmp	r3, r0
	beq.n	.L1
	adds	r2, r6, #0
	adds	r2, #16
.L2:
	adds	r1, #1
	cmp	r1, r5
	bge.n	.L1
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L2
.L1:
	cmp	r1, r5
	beq.n	.L0
	lsls	r1, r1, #2
	adds	r3, r1, #0
	adds	r3, #28
	ldr	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L0
	mov	r2, ip
	ldrb	r3, [r2, #0]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L3
.L4:
	adds	r4, #1
	cmp	r4, #13
	bgt.n	.L3
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L4
.L3:
	movs	r0, #32
	cmp	r4, #0
	ble.n	.L5
	subs	r3, r4, #1
	mov	r2, ip
	ldrb	r3, [r2, r3]
	adds	r0, r3, #0
	subs	r0, #49
.L5:
	adds	r1, #28
	ldr	r3, [r6, r1]
	movs	r2, #1
	lsls	r2, r0
	bics	r3, r2
	str	r3, [r6, r1]
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x00000129
