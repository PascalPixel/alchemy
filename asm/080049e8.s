@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080049e8
	.thumb_func
Func_080049e8:
	push	{r5, lr}
	ldr	r5, [pc, #40]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bgt.n	.L0
	ldr	r4, [pc, #36]
	ldr	r3, [pc, #36]
	ldr	r0, [pc, #40]
	ldr	r1, [r4, #0]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r4, #0]
	adds	r3, #48
	str	r3, [r4, #0]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001cc4
	.4byte 0x03001d2c
	.4byte 0x040000d4
	.4byte 0x03000350
	.4byte 0x8400000c
