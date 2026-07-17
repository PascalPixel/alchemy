@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801776c
	.thumb_func
Func_0801776c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #268]
	ldr	r3, [r3, #0]
	sub	sp, #28
	mov	r8, r3
	movs	r3, #0
	movs	r4, #0
	adds	r6, r1, #0
	str	r3, [sp, #4]
	str	r4, [sp, #8]
	movs	r3, #2
	movs	r5, #0
	movs	r2, #1
	adds	r7, r6, #0
	ands	r3, r6
	mov	sl, r0
	str	r5, [sp, #24]
	str	r5, [sp, #20]
	ands	r7, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #232]
	add	r3, r8
	strb	r2, [r3, #0]
.L0:
	add	r0, sp, #12
	add	r2, sp, #20
	add	r3, sp, #16
	str	r0, [sp, #0]
	add	r1, sp, #24
	mov	r0, sl
	bl	Func_080187ac
	ldr	r2, [sp, #16]
	movs	r3, #30
	subs	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r2, [sp, #12]
	str	r3, [sp, #24]
	movs	r3, #12
	subs	r3, r3, r2
	asrs	r2, r3, #1
	movs	r3, #8
	ands	r3, r6
	str	r2, [sp, #20]
	cmp	r3, #0
	beq.n	.L1
	adds	r3, r2, #4
	b.n	.L2
.L1:
	movs	r3, #64
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L3
	adds	r3, r2, #0
	adds	r3, #12
	b.n	.L2
.L3:
	ldr	r3, [pc, #168]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	add	r5, sp, #4
	ldr	r0, [r3, #0]
	adds	r1, r5, #0
	bl	Func_0808a278
	ldr	r3, [r5, #4]
	asrs	r3, r3, #3
	cmp	r3, #9
	ble.n	.L4
	subs	r3, #5
	b.n	.L2
.L4:
	adds	r3, #4
.L2:
	str	r3, [sp, #20]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	mov	r0, sl
	adds	r3, r7, #0
	bl	Func_08017658
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L5
	b.n	.L6
.L7:
	movs	r0, #1
	bl	Func_080030f8
.L6:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L7
	movs	r3, #32
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L8
	ldr	r3, [pc, #84]
	ldr	r2, [pc, #96]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
.L8:
	movs	r3, #4
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L5
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	Func_08016418
	b.n	.L9
.L10:
	movs	r0, #1
	bl	Func_080030f8
.L9:
	adds	r0, r5, #0
	bl	Func_08017394
	cmp	r0, #0
	beq.n	.L10
.L5:
	ldr	r3, [pc, #44]
	movs	r2, #0
	add	r3, r8
	strb	r2, [r3, #0]
	ldr	r3, [pc, #48]
	add	r3, r8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r0, #3
	bl	Func_080030f8
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012f9
	.4byte 0x02000240
	.4byte 0x00000ea6
	.4byte 0x000012f4
	.4byte 0x000012f6
