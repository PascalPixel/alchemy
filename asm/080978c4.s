@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080978c4
	.thumb_func
Func_080978c4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #116]
	ldr	r2, [pc, #116]
	ldr	r5, [r3, #0]
	adds	r6, r5, r2
	ldrh	r0, [r6, #0]
	sub	sp, #12
	movs	r7, #0
	lsls	r0, r0, #16
	add	r1, sp, #8
	add	r2, sp, #4
	mov	r3, sp
	str	r7, [sp, #8]
	str	r7, [sp, #4]
	str	r7, [sp, #0]
	bl	Func_08097948
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #88]
	asrs	r3, r3, #18
	adds	r4, r5, r2
	adds	r3, #4
	strb	r3, [r4, #0]
	ldr	r3, [sp, #4]
	adds	r2, #1
	asrs	r3, r3, #18
	adds	r0, r5, r2
	adds	r3, #4
	strb	r3, [r0, #0]
	ldr	r3, [sp, #0]
	adds	r2, #1
	asrs	r3, r3, #18
	adds	r5, r5, r2
	adds	r3, #4
	strb	r3, [r5, #0]
	ldrh	r3, [r6, #0]
	adds	r3, #4
	strh	r3, [r6, #0]
	movs	r2, #31
	ldrb	r1, [r4, #0]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r4, #0]
	ldrb	r1, [r0, #0]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r0, #0]
	ldrb	r3, [r5, #0]
	ands	r2, r3
	strb	r2, [r5, #0]
	movs	r2, #180
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #1
	cmp	r3, r2
	bcc.n	.L0
	strh	r7, [r6, #0]
.L0:
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ea8
	.4byte 0x0000028e
	.4byte 0x0000028b
