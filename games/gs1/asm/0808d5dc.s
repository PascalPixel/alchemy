@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808d5dc
	.thumb_func
Func_0808d5dc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r7, r0, #0
	bl	Func_0808d394
	movs	r1, #1
	negs	r1, r1
	mov	fp, r0
	adds	r0, r7, #0
	mov	r9, r1
	bl	Func_08092054
	movs	r2, #0
	ldr	r3, [pc, #444]
	ldr	r1, [pc, #444]
	str	r2, [sp, #0]
	adds	r3, r3, r1
	mov	sl, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	mov	r8, r0
	cmp	r3, r7
	bne.n	.L0
	movs	r3, #1
	movs	r0, #7
	adds	r1, r7, #0
	mov	sl, r3
	bl	Func_0808d48c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L1
	adds	r1, r7, #0
	movs	r0, #0
	bl	Func_0808d48c
	movs	r1, #1
	adds	r6, r0, #0
	str	r1, [sp, #0]
	mov	r0, r9
	cmp	r6, #0
	bne.n	.L2
	b.n	.L3
.L2:
	movs	r2, #128
	ldr	r3, [r6, #8]
	lsls	r2, r2, #9
	cmp	r3, r2
	blt.n	.L4
	adds	r0, r7, #0
	bl	Func_080915dc
	adds	r5, r0, #0
	bl	Func_08004458
	lsls	r0, r0, #1
	ldr	r3, [pc, #368]
	lsrs	r0, r0, #16
	lsls	r5, r5, #1
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	Func_080916b0
	adds	r0, r5, #0
	bl	Func_08092b94
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_08092f84
	bl	Func_08091750
	b.n	.L5
.L0:
	movs	r0, #0
	adds	r1, r7, #0
	bl	Func_0808d48c
	adds	r6, r0, #0
.L4:
	cmp	r6, #0
	bne.n	.L1
	b.n	.L5
.L1:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	bne.n	.L6
	b.n	.L5
.L6:
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L7
	mov	r2, r8
	adds	r2, #91
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r0, r8
	bl	Func_08009088
	mov	r2, r8
	ldrh	r2, [r2, #6]
	str	r2, [sp, #4]
	mov	r1, fp
	ldrb	r3, [r1, #22]
	movs	r2, #128
	lsls	r3, r3, #24
	lsls	r2, r2, #17
	cmp	r3, r2
	bls.n	.L8
	movs	r1, #192
	lsls	r1, r1, #18
	cmp	r3, r1
	bne.n	.L9
.L8:
	ldr	r5, [pc, #252]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	bl	Func_08092054
	ldr	r3, [r0, #8]
	str	r3, [r0, #56]
	ldr	r3, [r0, #12]
	str	r3, [r0, #60]
	ldr	r3, [r0, #16]
	str	r3, [r0, #64]
	mov	r3, sl
	str	r3, [r0, #36]
	str	r3, [r0, #40]
	str	r3, [r0, #44]
	movs	r2, #0
	ldr	r1, [r5, #0]
	adds	r0, r7, #0
	bl	Func_08092848
.L9:
	ldr	r3, [r6, #8]
.L7:
	movs	r1, #128
	lsls	r1, r1, #9
	cmp	r3, r1
	bge.n	.L10
	ldr	r0, [sp, #0]
	bl	Func_08015058
	bl	Func_080916b0
	ldr	r0, [r6, #8]
	bl	Func_08092b94
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_08092f84
	bl	Func_08091750
	b.n	.L11
.L10:
	bl	Func_08091660
	ldr	r3, [r6, #8]
	adds	r0, r7, #0
	bl	Func_080072f0
.L11:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L12
	mov	r2, r8
	movs	r1, #4
	ldrsh	r3, [r2, r1]
	ldr	r2, [r2, #0]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	cmp	r3, #16
	bne.n	.L13
	mov	r3, fp
	movs	r0, #22
	ldrsb	r0, [r3, r0]
	cmp	r0, #3
	bne.n	.L14
	ldr	r3, [pc, #124]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	Func_08092054
	mov	r2, r8
	str	r0, [r2, #104]
	mov	r1, r8
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	mov	r0, r8
	ldr	r1, [pc, #104]
	bl	Func_08093a6c
	b.n	.L13
.L14:
	cmp	r0, #1
	bne.n	.L13
	add	r1, sp, #4
	ldrh	r1, [r1, #0]
	mov	r3, r8
	adds	r3, #100
	strh	r1, [r3, #0]
	mov	r0, r8
	ldr	r1, [pc, #84]
	bl	Func_08009098
.L13:
	mov	r2, r8
	adds	r2, #91
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r0, r8
	movs	r1, #16
	bl	Func_08009088
.L12:
	movs	r2, #0
	mov	r9, r2
.L5:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L15
	ldr	r5, [pc, #36]
	ldr	r1, [pc, #36]
	adds	r5, r5, r1
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl	Func_0809ade8
	ldr	r3, [pc, #40]
	strh	r3, [r5, #0]
.L15:
	mov	r0, r9
.L3:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x0000024a
	.4byte 0x00000e0b
	.4byte 0x0809ff40
	.4byte 0x0809fc1c
	.4byte 0x0000ffff
