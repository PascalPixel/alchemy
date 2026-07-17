@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c10e8
	.thumb_func
Func_080c10e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #224]
	sub	sp, #28
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [r3, #0]
	cmp	r1, #0
	bne.n	.L0
	ldr	r0, [pc, #212]
	bl	Func_08004278
	ldr	r3, [pc, #212]
	mov	r1, sl
	strh	r1, [r3, #0]
	bl	Func_080c1054
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #200]
	movs	r1, #0
	bl	Func_0800387c
.L0:
	cmp	r5, #0
	beq.n	.L1
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L1
	movs	r1, #202
	lsls	r1, r1, #3
	adds	r3, r5, r1
	mov	r1, sl
	strh	r1, [r3, #0]
	ldr	r1, [pc, #176]
	movs	r2, #0
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	ldr	r3, [pc, #160]
	strh	r2, [r3, #0]
	movs	r2, #16
	subs	r3, #2
	strh	r2, [r3, #0]
	mov	r5, sp
	adds	r1, r5, #0
	movs	r0, #3
	bl	Func_080b6c08
	movs	r6, #0
	adds	r7, r0, #0
	cmp	r6, r7
	bcs.n	.L2
	movs	r2, #1
	mov	fp, r5
	mov	r9, r2
	movs	r5, #0
.L3:
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	mov	r2, r9
	mov	r1, sl
	ands	r1, r2
	adds	r6, #1
	bl	Func_080c0f98
	adds	r5, #2
	cmp	r6, r7
	bcc.n	.L3
.L2:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L4
	ldrh	r0, [r3, #0]
	movs	r1, #2
	movs	r6, #0
	add	r8, r1
	cmp	r0, #255
	beq.n	.L4
	movs	r7, #1
	adds	r5, r7, #0
	mov	r2, sl
	ands	r5, r2
.L5:
	adds	r1, r5, #0
	eors	r1, r7
	adds	r6, #1
	bl	Func_080c0f98
	cmp	r6, #13
	bhi.n	.L4
	mov	r3, r8
	ldrh	r0, [r3, #0]
	movs	r1, #2
	add	r8, r1
	cmp	r0, #255
	bne.n	.L5
.L4:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #48]
	movs	r1, #0
	bl	Func_0800387c
	movs	r1, #144
	ldr	r0, [pc, #28]
	lsls	r1, r1, #3
	bl	Func_080041d8
.L1:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x080c1085
	.4byte 0x04000054
	.4byte 0x04000050
	.4byte 0x0000064e
