@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a9cf8
	.thumb_func
Func_080a9cf8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	movs	r2, #168
	adds	r6, r3, #0
	sub	sp, #4
	adds	r7, r0, #0
	movs	r5, #0
	mov	r8, r2
	adds	r6, #200
.L0:
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r1, r5, #0
	movs	r0, #2
	adds	r2, r7, #0
	movs	r3, #248
	bl	Func_080150d0
	adds	r5, #1
	stmia	r6!, {r0}
	cmp	r5, #7
	ble.n	.L0
	movs	r0, #1
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
