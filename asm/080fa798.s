@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa798
	.thumb_func
Func_080fa798:
	push	{r4, r5, r6, lr}
	adds	r2, r0, #0
	ldr	r0, [pc, #120]
	ldr	r4, [r0, #0]
	movs	r0, #240
	lsls	r0, r0, #12
	ands	r0, r2
	lsrs	r2, r0, #16
	movs	r6, #0
	strb	r2, [r4, #8]
	ldr	r1, [pc, #108]
	subs	r0, r2, #1
	lsls	r0, r0, #1
	adds	r0, r0, r1
	ldrh	r5, [r0, #0]
	str	r5, [r4, #16]
	movs	r0, #198
	lsls	r0, r0, #3
	adds	r1, r5, #0
	bl	Func_080022ec
	strb	r0, [r4, #11]
	ldr	r0, [pc, #88]
	muls	r0, r5
	ldr	r1, [pc, #88]
	adds	r0, r0, r1
	ldr	r1, [pc, #88]
	bl	Func_080022ec
	adds	r1, r0, #0
	str	r1, [r4, #20]
	movs	r0, #128
	lsls	r0, r0, #17
	bl	Func_080022ec
	adds	r0, #1
	asrs	r0, r0, #1
	str	r0, [r4, #24]
	ldr	r0, [pc, #68]
	strh	r6, [r0, #0]
	ldr	r4, [pc, #68]
	ldr	r0, [pc, #72]
	adds	r1, r5, #0
	bl	Func_080022ec
	negs	r0, r0
	strh	r0, [r4, #0]
	bl	Func_080fa9a4
	ldr	r1, [pc, #60]
.L0:
	ldrb	r0, [r1, #0]
	cmp	r0, #159
	beq.n	.L0
	ldr	r1, [pc, #52]
.L1:
	ldrb	r0, [r1, #0]
	cmp	r0, #159
	bne.n	.L1
	ldr	r1, [pc, #32]
	movs	r0, #128
	strh	r0, [r1, #0]
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x080fb914
	.4byte 0x00091d1b
	.4byte 0x00001388
	.4byte 0x00002710
	.4byte 0x04000102
	.4byte 0x04000100
	.4byte 0x00044940
	.4byte 0x04000006
