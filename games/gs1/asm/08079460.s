@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079460
	.thumb_func
Func_08079460:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r1
	mov	r5, fp
	sub	sp, #36
	mov	r9, r0
	mov	r8, r2
	subs	r5, #8
	cmp	r0, #127
	bgt.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	mov	r2, r9
	movs	r0, #0
	cmp	r2, #134
	ble.n	.L2
	b.n	.L1
.L2:
	cmp	r5, #242
	bls.n	.L3
	b.n	.L1
.L3:
	mov	r0, r9
	bl	Func_08077394
	movs	r1, #166
	ldr	r3, [pc, #336]
	lsls	r1, r1, #1
	adds	r6, r0, #0
	bl	Func_080072f0
	cmp	r5, #164
	bls.n	.L4
	movs	r5, #0
.L4:
	movs	r3, #84
	adds	r2, r5, #0
	muls	r2, r3
	ldr	r3, [pc, #316]
	adds	r7, r2, r3
	ldrb	r3, [r7, #15]
	strb	r3, [r6, #15]
	ldrh	r3, [r7, #16]
	strh	r3, [r6, #16]
	strh	r3, [r6, #56]
	strh	r3, [r6, #52]
	ldrh	r3, [r7, #18]
	strh	r3, [r6, #18]
	strh	r3, [r6, #58]
	strh	r3, [r6, #54]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r6, #20]
	strh	r3, [r6, #22]
	ldrh	r3, [r7, #20]
	strh	r3, [r6, #24]
	ldrh	r3, [r7, #22]
	strh	r3, [r6, #26]
	ldrh	r3, [r7, #24]
	strh	r3, [r6, #28]
	ldrb	r3, [r7, #26]
	strb	r3, [r6, #30]
	ldrb	r3, [r7, #27]
	ldrb	r2, [r7, #28]
	strb	r3, [r6, #31]
	adds	r3, r6, #0
	adds	r3, #32
	strb	r2, [r3, #0]
	ldr	r0, [pc, #260]
	ldrb	r3, [r7, #29]
	adds	r2, r6, #0
	add	r4, sp, #4
	adds	r2, #33
	strb	r3, [r2, #0]
	adds	r0, r5, r0
	adds	r1, r4, #0
	movs	r2, #15
	str	r4, [sp, #0]
	bl	Func_08015028
	ldr	r4, [sp, #0]
	movs	r5, #0
	ldrh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L5
	adds	r0, r4, #0
	adds	r1, r6, #0
	movs	r2, #0
.L6:
	ldrh	r3, [r2, r0]
	adds	r5, #1
	strb	r3, [r1, #0]
	adds	r2, #2
	adds	r1, #1
	cmp	r5, #13
	bgt.n	.L5
	ldrh	r3, [r2, r4]
	cmp	r3, #0
	bne.n	.L6
.L5:
	mov	r3, r8
	cmp	r3, #8
	bgt.n	.L7
	adds	r3, #49
	strb	r3, [r6, r5]
	adds	r5, #1
.L7:
	movs	r3, #0
	strb	r3, [r6, r5]
	movs	r2, #40
	movs	r3, #0
	strb	r3, [r6, #14]
	mov	r8, r3
	mov	sl, r2
	movs	r3, #40
	movs	r2, #48
	adds	r3, r3, r7
	adds	r2, r2, r7
	mov	lr, r3
	movs	r5, #3
	mov	ip, r2
	adds	r0, r6, #0
.L11:
	mov	r2, lr
	ldrh	r3, [r2, #0]
	movs	r2, #2
	add	lr, r2
	cmp	r3, #0
	beq.n	.L8
	mov	r2, ip
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L8
	adds	r2, r0, #0
	mov	r4, sl
	adds	r1, r3, #0
	adds	r2, #216
.L10:
	mov	r3, r8
	cmp	r3, #14
	bgt.n	.L9
	ldrh	r3, [r7, r4]
	strh	r3, [r2, #0]
	movs	r3, #1
	adds	r2, #2
	adds	r0, #2
	add	r8, r3
.L9:
	subs	r1, #1
	cmp	r1, #0
	bne.n	.L10
.L8:
	movs	r2, #2
	movs	r3, #1
	subs	r5, #1
	add	sl, r2
	add	ip, r3
	cmp	r5, #0
	bge.n	.L11
	movs	r3, #144
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [r7, #32]
	str	r3, [r2, #0]
	ldr	r3, [pc, #88]
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r5, r6, r2
	mov	r3, fp
	adds	r1, r6, #0
	strb	r3, [r5, #0]
	adds	r1, #36
	mov	r0, r9
	bl	Func_080798e0
	mov	r0, r9
	bl	Func_08077428
	movs	r3, #149
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrb	r3, [r5, #0]
	cmp	r3, #171
	bgt.n	.L12
	cmp	r3, #158
	blt.n	.L12
	movs	r3, #2
	strb	r3, [r2, #0]
.L12:
	movs	r0, #1
.L1:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000164
	.4byte 0x08080ec8
	.4byte 0x0000028f
	.4byte 0x00000129
