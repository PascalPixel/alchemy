@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08094380
	.thumb_func
Func_08094380:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #148]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	mov	r8, r2
	mov	r0, r8
	movs	r1, #27
	bl	Func_08009048
	movs	r3, #0
	mov	sl, r3
	mov	r7, r8
	adds	r7, #38
	mov	r1, sl
	strb	r1, [r7, #0]
	movs	r3, #15
	ldr	r2, [pc, #108]
	strb	r3, [r0, #5]
	movs	r1, #128
	ldr	r3, [r5, #8]
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r3, r9
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r1, r6, #0
	str	r3, [r5, #16]
	adds	r0, r5, #0
	bl	Func_08009080
	movs	r0, #30
	bl	Func_080030f8
	mov	r3, r8
	adds	r3, #39
	movs	r6, #1
	strb	r6, [r3, #0]
	mov	r2, r8
	ldr	r0, [r2, #44]
	bl	Func_08009060
	mov	r3, sl
	mov	r1, r8
	str	r3, [r1, #44]
	strb	r6, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	str	r3, [r5, #48]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	add	r3, r9
	bl	Func_08009150
	adds	r0, r5, #0
	bl	Func_08009158
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0xfff00000
