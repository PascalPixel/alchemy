@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091890
	.thumb_func
Func_08091890:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	bl	Func_08077168
	bl	Func_08091858
	adds	r0, r5, #0
	bl	Func_08077008
	adds	r6, r0, #0
	ldrh	r1, [r6, #52]
	ldrh	r3, [r6, #54]
	strh	r1, [r6, #56]
	strh	r3, [r6, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L0
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L0
	adds	r3, r0, #0
.L0:
	strh	r3, [r6, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L1
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	strh	r3, [r6, #20]
.L1:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L2
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L2
	adds	r3, r0, #0
.L2:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L3
	movs	r2, #58
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #1
	strh	r3, [r6, #22]
.L3:
	ldr	r3, [pc, #188]
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r8, r3
	bl	Func_08077148
	cmp	r8, r0
	bge.n	.L4
	ldr	r3, [pc, #176]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r5, r0, #0
.L6:
	ldrb	r0, [r7, #0]
	bl	Func_08077008
	adds	r6, r0, #0
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	adds	r7, #1
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #1
	add	r8, r3
.L5:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L6
.L4:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L7
	ldr	r3, [pc, #132]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_08077008
	movs	r5, #1
	adds	r6, r0, #0
	strh	r5, [r6, #56]
	lsls	r5, r5, #14
	movs	r3, #52
	ldrsh	r1, [r6, r3]
	adds	r0, r5, #0
	bl	Func_080022ec
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r0, r2
	bgt.n	.L8
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L8
	adds	r5, r0, #0
.L8:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L9
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L9
	movs	r3, #1
	strh	r3, [r6, #20]
.L9:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L10
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L10
	adds	r3, r0, #0
.L10:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L7
	movs	r2, #58
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L7
	movs	r3, #1
	strh	r3, [r6, #22]
.L7:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000131
	.4byte 0x02000240
