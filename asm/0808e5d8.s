@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808e5d8
	.thumb_func
Func_0808e5d8:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #136]
	adds	r5, r0, #0
	ands	r3, r5
	mov	r8, r3
	mov	r0, r8
	movs	r3, #15
	lsrs	r5, r5, #10
	sub	sp, #4
	ands	r5, r3
	bl	Func_08077080
	ldr	r6, [pc, #116]
	movs	r3, #250
	lsls	r3, r3, #1
	ldrb	r0, [r0, #12]
	adds	r6, r6, r3
	mov	r9, r0
	ldr	r0, [r6, #0]
	bl	Func_0808ba1c
	mov	sl, sp
	mov	r1, r9
	mov	r2, sl
	ldr	r0, [pc, #96]
	bl	Func_0808e4b4
	mov	r2, sl
	mov	fp, r0
	mov	r1, r9
	ldr	r0, [pc, #88]
	bl	Func_0808e4b4
	movs	r1, #0
	mov	sl, r0
	mov	r0, r8
	bl	Func_08096fb0
	ldr	r0, [r6, #0]
	ldr	r1, [sp, #0]
	bl	Func_080970f8
	adds	r1, r5, #0
	ldr	r2, [sp, #0]
	mov	r0, fp
	bl	Func_08096b28
	bl	Func_08096af0
	bl	Func_08097174
	adds	r1, r5, #0
	ldr	r2, [sp, #0]
	mov	r0, sl
	bl	Func_08096b28
	bl	Func_08097194
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x02000240
	.4byte 0x30000005
	.4byte 0x20000005
