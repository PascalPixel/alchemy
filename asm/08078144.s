@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078144
	.thumb_func
Func_08078144:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	bl	Func_080795fc
	movs	r7, #0
	mov	r8, r0
	cmp	r7, r8
	bge.n	.L0
.L9:
	movs	r1, #252
	ldr	r2, [pc, #188]
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrb	r6, [r2, r3]
	ldr	r3, [pc, #184]
	ldrb	r3, [r3, r6]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L1
	movs	r0, #136
	lsls	r0, r0, #1
	bl	Func_08079338
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #137
	lsls	r0, r0, #1
	b.n	.L3
.L1:
	ldr	r0, [pc, #160]
	bl	Func_08079338
	cmp	r0, #0
	bne.n	.L2
	ldr	r0, [pc, #156]
.L3:
	bl	Func_08079338
	cmp	r0, #0
	beq.n	.L4
.L2:
	movs	r5, #1
.L4:
	cmp	r5, #0
	beq.n	.L5
	adds	r0, r6, #0
	bl	Func_08077394
	adds	r5, r0, #0
	ldrh	r3, [r5, #54]
	strh	r3, [r5, #58]
	movs	r2, #56
	ldrsh	r0, [r5, r2]
	movs	r3, #52
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L6
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L6
	adds	r3, r0, #0
.L6:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L7
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L7
	movs	r3, #1
	strh	r3, [r5, #20]
.L7:
	movs	r2, #58
	ldrsh	r0, [r5, r2]
	movs	r3, #54
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L8
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L8
	adds	r3, r0, #0
.L8:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L5
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #1
	strh	r3, [r5, #22]
.L5:
	adds	r7, #1
	cmp	r7, r8
	blt.n	.L9
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0807a828
	.4byte 0x00000111
	.4byte 0x00000113
	bx	lr
