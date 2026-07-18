@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080994d0
	.thumb_func
Func_080994d0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #388]
	ldr	r3, [r3, #0]
	mov	sl, r3
	ldr	r6, [r3, #16]
	bl	Func_08097384
	movs	r2, #0
	mov	r8, r2
	movs	r7, #0
.L1:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r0, #233
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r3, [pc, #348]
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldr	r3, [pc, #348]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #120
	strh	r3, [r2, #0]
	lsls	r3, r7, #13
	adds	r2, #2
	strh	r3, [r2, #0]
	subs	r2, #17
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r1, r8
	ldr	r0, [r5, #80]
	bl	Func_08096c48
	mov	r8, r0
.L0:
	movs	r0, #1
	adds	r7, #1
	bl	Func_080030f8
	cmp	r7, #7
	ble.n	.L1
	mov	r2, r8
	ldrb	r2, [r2, #28]
	movs	r0, #130
	mov	r9, r2
	bl	Func_080f9010
	movs	r0, #110
	bl	Func_080030f8
	movs	r0, #233
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08096c80
	adds	r6, r0, #0
	adds	r5, r6, #0
	cmp	r6, #0
	beq.n	.L2
	ldr	r3, [pc, #260]
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	mov	r2, sl
	ldr	r3, [r2, #4]
	str	r3, [r6, #8]
	ldr	r3, [r2, #8]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r6, #12]
	mov	r2, sl
	ldr	r3, [r2, #12]
	adds	r2, r6, #0
	str	r3, [r6, #16]
	adds	r2, #85
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #7
	bl	Func_08009240
.L2:
	movs	r0, #131
	bl	Func_080f9010
	movs	r0, #12
	bl	Func_080030f8
	cmp	r6, #0
	beq.n	.L3
	movs	r3, #3
	movs	r7, #0
	mov	r8, r3
.L6:
	adds	r3, r7, #0
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	adds	r0, r5, #0
	movs	r1, #9
	bl	Func_08009240
	b.n	.L5
.L4:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_08009240
.L5:
	movs	r0, #2
	adds	r7, #1
	bl	Func_080030f8
	cmp	r7, #29
	ble.n	.L6
.L3:
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009240
	movs	r0, #84
	bl	Func_080f9010
	cmp	r5, #0
	beq.n	.L7
	ldr	r3, [pc, #144]
	adds	r2, r6, #0
	str	r3, [r6, #108]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L8
	movs	r0, #128
	bl	Func_080030f8
	b.n	.L7
.L8:
	movs	r0, #192
	bl	Func_080030f8
.L7:
	cmp	r6, #0
	beq.n	.L9
	ldr	r3, [pc, #104]
	adds	r2, r6, #0
	adds	r2, #100
	strh	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r6, #48]
	ldr	r3, [pc, #96]
	subs	r2, #10
	str	r3, [r6, #52]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #192
	movs	r2, #232
	lsls	r1, r1, #16
	lsls	r2, r2, #8
	adds	r0, r6, #0
	bl	Func_08096bec
	adds	r0, r6, #0
	bl	Func_08009158
	adds	r0, r6, #0
	bl	Func_080090d0
.L9:
	mov	r3, r9
	cmp	r3, #96
	beq.n	.L10
	mov	r0, r9
	bl	Func_08003f3c
.L10:
	mov	r2, sl
	ldr	r3, [r2, #36]
	cmp	r3, #0
	beq.n	.L11
	bl	Func_080072f0
.L11:
	bl	Func_0809748c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x0000b333
	.4byte 0x08099341
	.4byte 0x080993b1
	.4byte 0x0000ffff
	.4byte 0x00006666
