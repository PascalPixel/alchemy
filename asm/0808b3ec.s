@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808b3ec
	.thumb_func
Func_0808b3ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #572]
	ldr	r3, [r3, #0]
	mov	r9, r3
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	mov	fp, r1
	sub	sp, #8
	movs	r1, #0
	cmp	r3, r7
	beq.n	.L0
	cmp	r3, #0
	bne.n	.L1
	mov	r0, r9
	str	r7, [r0, #0]
	b.n	.L0
.L1:
	adds	r1, #1
	cmp	r1, #3
	bgt.n	.L0
	lsls	r2, r1, #2
	mov	r0, r9
	ldr	r3, [r0, r2]
	cmp	r3, r7
	beq.n	.L0
	cmp	r3, #0
	bne.n	.L1
	str	r7, [r0, r2]
.L0:
	movs	r0, #1
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	negs	r0, r0
	ldrh	r2, [r7, #0]
	b.n	.L2
.L19:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, #7
	bgt.n	.L3
	str	r3, [sp, #4]
	b.n	.L4
.L3:
	ldr	r2, [pc, #500]
	cmp	r3, r2
	bgt.n	.L4
	mov	r3, fp
	movs	r0, #1
	str	r3, [sp, #4]
	add	fp, r0
.L4:
	movs	r1, #2
	ldrsh	r5, [r7, r1]
	adds	r0, r5, #0
	bl	Func_0808d428
	cmp	r0, #0
	bne.n	.L5
	b.n	.L6
.L5:
	adds	r3, r5, #0
	subs	r3, #48
	cmp	r3, #79
	bhi.n	.L7
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r9
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	beq.n	.L7
	adds	r0, r5, #0
	adds	r0, #80
	bl	Func_0808d428
	cmp	r0, #0
	bne.n	.L7
	b.n	.L6
.L7:
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	bl	Func_0808b398
	mov	sl, r0
	ldr	r0, [sp, #4]
	bl	Func_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L8
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	mov	r0, sl
	bl	Func_080090c8
	ldrb	r2, [r7, #23]
	movs	r3, #1
	ands	r3, r2
	adds	r6, r0, #0
	movs	r1, #1
	cmp	r3, #0
	beq.n	.L9
	ldr	r0, [sp, #4]
	subs	r0, #1
	str	r1, [sp, #0]
	bl	Func_0808ba1c
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #0]
	cmp	r3, #1
	bne.n	.L9
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L9
	ldr	r0, [r0, #80]
	ldrb	r3, [r0, #29]
	orrs	r3, r1
	strb	r3, [r0, #29]
	ldrb	r5, [r0, #28]
	ldr	r0, [r6, #80]
	ldrb	r3, [r0, #29]
	orrs	r3, r1
	mov	r8, r0
	strb	r3, [r0, #29]
	ldrb	r0, [r0, #28]
	bl	Func_08003f3c
	mov	r1, r8
	strb	r5, [r1, #28]
.L9:
	movs	r0, #33
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L10
	mov	r3, sl
	subs	r3, #18
	cmp	r3, #1
	bhi.n	.L10
	adds	r0, r6, #0
	movs	r1, #226
	bl	Func_08009228
	b.n	.L10
.L8:
	ldr	r0, [pc, #300]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L10
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	adds	r0, r6, #0
	bl	Func_080090f0
.L10:
	cmp	r6, #0
	beq.n	.L11
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L12
	ldr	r2, [r6, #80]
	mov	r8, r2
	cmp	r2, #0
	beq.n	.L12
	bl	Func_08004458
	movs	r1, #30
	bl	Func_08002304
	mov	r3, r8
	adds	r3, #36
	strb	r0, [r3, #0]
.L12:
	ldrh	r3, [r7, #20]
	adds	r2, r6, #0
	strh	r3, [r6, #6]
	adds	r2, #89
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl	Func_08093a6c
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
	ldr	r2, [r6, #8]
	cmp	r2, #0
	bge.n	.L13
	ldr	r3, [pc, #200]
	adds	r2, r2, r3
.L13:
	adds	r3, r6, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L14
	ldr	r0, [pc, #184]
	adds	r3, r3, r0
.L14:
	adds	r2, r6, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq.n	.L15
	movs	r3, #4
	subs	r2, #17
	strb	r3, [r2, #0]
	movs	r1, #128
	ldr	r3, [r6, #12]
	lsls	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r6, #12]
.L15:
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r9
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L16
	adds	r1, r6, #0
	adds	r1, #85
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	movs	r0, #33
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L17
	mov	r1, r8
	ldr	r0, [r1, #24]
	movs	r1, #192
	ldr	r3, [pc, #112]
	lsls	r1, r1, #8
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r2, r8
	str	r0, [r2, #24]
	b.n	.L17
.L16:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #0
	bl	Func_080091a8
	ldr	r3, [r6, #12]
	adds	r3, r3, r0
	str	r0, [r6, #20]
	str	r3, [r6, #12]
.L17:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #1
	strb	r3, [r2, #0]
.L11:
	ldr	r0, [sp, #4]
	lsls	r3, r0, #2
	adds	r3, #20
	mov	r1, r9
	str	r6, [r1, r3]
.L6:
	adds	r7, #24
	ldrh	r3, [r7, #0]
	adds	r2, r3, #0
	lsls	r3, r2, #16
	movs	r0, #1
	asrs	r3, r3, #16
	negs	r0, r0
.L2:
	cmp	r3, r0
	beq.n	.L18
	mov	r1, fp
	cmp	r1, #65
	bgt.n	.L18
	b.n	.L19
.L18:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x00002705
	.4byte 0x00000109
	.4byte 0x0000ffff
	.4byte 0x03000118
