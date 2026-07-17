@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08004a5c
	.thumb_func
Func_08004a5c:
	push	{lr}
	ldr	r2, [pc, #32]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L0
	subs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #24]
	ldr	r0, [r3, #0]
	subs	r0, #48
	str	r0, [r3, #0]
	ldr	r1, [pc, #20]
	ldr	r3, [pc, #20]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001cc4
	.4byte 0x03001d2c
	.4byte 0x03000350
	.4byte 0x040000d4
	.4byte 0x8400000c
