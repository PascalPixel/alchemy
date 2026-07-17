@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080216b4
	.thumb_func
Func_080216b4:
	push	{r5, lr}
	ldr	r4, [pc, #40]
	ldr	r3, [r4, #0]
	ldr	r5, [pc, #40]
	movs	r1, #7
	lsrs	r3, r3, #2
	ands	r3, r1
	ldrb	r2, [r0, #8]
	ldrb	r3, [r5, r3]
	adds	r2, r2, r3
	ldr	r3, [r4, #0]
	strb	r2, [r0, #20]
	lsrs	r3, r3, #2
	ldr	r0, [r0, #0]
	ands	r3, r1
	ldrb	r2, [r0, #8]
	ldrb	r3, [r5, r3]
	adds	r2, r2, r3
	strb	r2, [r0, #20]
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001800
	.4byte 0x08037226
