@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080982dc
	.thumb_func
Func_080982dc:
	push	{r5, r6, lr}
	ldr	r3, [pc, #156]
	movs	r0, #250
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #152]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	movs	r1, #204
	lsls	r1, r1, #4
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r4, r0, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #128]
	adds	r2, r5, r3
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
	subs	r3, r1, #1
	strh	r3, [r2, #0]
.L0:
	ldr	r1, [pc, #116]
	adds	r3, r5, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r0, [r4, #8]
	cmp	r0, #0
	bge.n	.L1
	ldr	r1, [pc, #104]
	adds	r0, r0, r1
.L1:
	asrs	r0, r0, #16
	ldr	r3, [pc, #104]
	subs	r0, r2, r0
	ldr	r1, [pc, #104]
	mov	ip, pc
	bx	r3
	ldr	r2, [pc, #100]
	adds	r3, r5, r2
	adds	r1, r0, #0
	ldr	r2, [r4, #16]
	movs	r0, #0
	ldrsh	r6, [r3, r0]
	ldr	r3, [r4, #12]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L2
	ldr	r2, [pc, #68]
	adds	r0, r0, r2
.L2:
	asrs	r3, r0, #16
	subs	r3, r6, r3
	adds	r0, r3, #0
	muls	r0, r3
	adds	r2, r1, #0
	muls	r2, r1
	adds	r3, r0, #0
	movs	r1, #225
	adds	r2, r2, r3
	lsls	r1, r1, #4
	cmp	r2, r1
	bge.n	.L3
	ldr	r2, [pc, #32]
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L4
.L3:
	movs	r1, #191
	lsls	r1, r1, #1
	ldr	r3, [pc, #40]
	adds	r2, r5, r1
	strh	r3, [r2, #0]
.L4:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000cba
	.4byte 0x00000cbc
	.4byte 0x0000ffff
	.4byte 0x03000118
	.4byte 0x0000d105
	.4byte 0x00000cbe
	.4byte 0x00002090
