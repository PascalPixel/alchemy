@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a6794
	.thumb_func
Func_080a6794:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #200]
	ldr	r3, [r3, #0]
	mov	sl, r3
	mov	r0, sl
	sub	sp, #8
	bl	Func_080a1814
	movs	r5, #0
	movs	r1, #2
	movs	r2, #2
	movs	r3, #8
	str	r5, [sp, #0]
	bl	Func_080a1870
	movs	r6, #2
	movs	r1, #5
	movs	r2, #30
	movs	r3, #15
	movs	r0, #0
	str	r6, [sp, #0]
	bl	Func_08015010
	movs	r3, #136
	lsls	r3, r3, #1
	mov	r2, sl
	add	r3, sl
	str	r0, [r2, #32]
	strb	r5, [r3, #0]
	ldr	r3, [pc, #148]
	movs	r2, #137
	add	r3, sl
	lsls	r2, r2, #1
	strb	r5, [r3, #0]
	add	r2, sl
	movs	r3, #8
	strb	r3, [r2, #0]
	ldr	r3, [pc, #136]
	add	r3, sl
	strb	r6, [r3, #0]
	movs	r1, #0
	movs	r2, #4
	mov	r8, r0
	bl	Func_080a1778
	movs	r3, #13
	strb	r3, [r0, #5]
	mov	r3, sl
	str	r0, [r3, #68]
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	mov	r3, r8
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	mov	r6, sl
	bl	Func_080150d8
	movs	r2, #8
	mov	r9, r2
	adds	r6, #72
	movs	r7, #96
.L0:
	mov	r3, r9
	str	r3, [sp, #0]
	adds	r1, r5, #0
	adds	r3, r7, #0
	movs	r0, #4
	mov	r2, r8
	bl	Func_080150d0
	adds	r5, #1
	stmia	r6!, {r0}
	adds	r7, #16
	cmp	r5, #7
	ble.n	.L0
	movs	r2, #24
	mov	r6, sl
	movs	r5, #8
	mov	r9, r2
	adds	r6, #104
	movs	r7, #96
.L1:
	mov	r3, r9
	str	r3, [sp, #0]
	adds	r1, r5, #0
	adds	r3, r7, #0
	movs	r0, #4
	mov	r2, r8
	bl	Func_080150d0
	adds	r5, #1
	stmia	r6!, {r0}
	adds	r7, #16
	cmp	r5, #15
	ble.n	.L1
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000111
	.4byte 0x00000113
