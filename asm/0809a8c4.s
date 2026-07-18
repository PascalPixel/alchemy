@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809a8c4
	.thumb_func
Func_0809a8c4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #420]
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #16]
	mov	sl, r3
	ldr	r3, [r3, #20]
	mov	fp, r3
	movs	r3, #0
	sub	sp, #36
	mov	r8, r3
	ldr	r3, [r2, #8]
	add	r5, sp, #12
	str	r3, [r5, #0]
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	mov	r2, sl
	ldr	r3, [r2, #4]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r2, #8]
	ldr	r2, [pc, #380]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	mov	r2, sl
	ldr	r3, [r2, #12]
	movs	r0, #218
	str	r3, [r6, #8]
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08096c80
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_08097384
	adds	r0, r7, #0
	movs	r1, #2
	bl	Func_08009080
	mov	r9, r5
.L2:
	mov	r2, r9
	ldr	r5, [r2, #0]
	ldr	r3, [r6, #0]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r2, r9
	ldr	r5, [r2, #4]
	ldr	r3, [r6, #4]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r2, r9
	ldr	r5, [r2, #8]
	ldr	r3, [r6, #8]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	ldr	r3, [pc, #276]
	adds	r5, r5, r0
	movs	r1, #10
	mov	r0, r8
	muls	r0, r3
	str	r5, [r7, #16]
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r0, r3
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #11
	blt.n	.L2
	ldr	r3, [pc, #240]
	str	r3, [r7, #24]
	ldr	r3, [pc, #240]
	movs	r0, #163
	str	r3, [r7, #28]
	bl	Func_080f9010
	movs	r0, #20
	bl	Func_080030f8
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L3
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L4
	ldr	r3, [pc, #208]
	str	r3, [r2, #108]
.L4:
	movs	r3, #0
	mov	r8, r3
	add	r6, sp, #24
.L6:
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [pc, #196]
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	movs	r3, #192
	lsls	r5, r0, #2
	lsls	r3, r3, #10
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	movs	r0, #249
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L5
	ldr	r3, [pc, #132]
	str	r3, [r5, #108]
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #100
	str	r7, [r5, #104]
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	bl	Func_08004458
	strh	r0, [r5, #6]
.L5:
	movs	r0, #6
	bl	Func_080030f8
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #15
	ble.n	.L6
	movs	r0, #20
	bl	Func_080030f8
	movs	r0, #120
	bl	Func_080030f8
.L3:
	movs	r1, #1
	adds	r0, r7, #0
	bl	Func_08009080
	movs	r0, #30
	bl	Func_080030f8
	movs	r0, #136
	bl	Func_080f9010
	movs	r0, #20
	bl	Func_080030f8
	adds	r0, r7, #0
	bl	Func_080090d0
	bl	Func_0809748c
.L1:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0xfffc0000
	.4byte 0x00010ccc
	.4byte 0x0001b333
	.4byte 0x00014ccc
	.4byte 0x0809a891
	.4byte 0x0000cccc
	.4byte 0x0809a7f5
