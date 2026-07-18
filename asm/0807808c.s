@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807808c
	.thumb_func
Func_0807808c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	bl	Func_080795fc
	movs	r6, #0
	adds	r7, r0, #0
	cmp	r6, r7
	bge.n	.L0
.L6:
	movs	r1, #252
	ldr	r2, [pc, #152]
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r0, [r2, r3]
	bl	Func_08077394
	adds	r5, r0, #0
	ldrh	r1, [r5, #52]
	ldrh	r3, [r5, #54]
	strh	r1, [r5, #56]
	strh	r3, [r5, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L1
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L1
	adds	r3, r0, #0
.L1:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L2
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #1
	strh	r3, [r5, #20]
.L2:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L3
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L3
	adds	r3, r0, #0
.L3:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L4
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #1
	strh	r3, [r5, #22]
.L4:
	mov	r2, r8
	cmp	r2, #1
	bne.n	.L5
	ldr	r1, [pc, #32]
	movs	r2, #0
	adds	r3, r5, r1
	adds	r1, #15
	strb	r2, [r3, #0]
	adds	r3, r5, r1
	strb	r2, [r3, #0]
.L5:
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L6
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000131
