@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080f9a30
	.thumb_func
Func_080f9a30:
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq.n	.L0
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #48]
	cmp	r2, #0
	beq.n	.L1
	str	r1, [r2, #52]
	b.n	.L2
.L1:
	str	r1, [r3, #32]
.L2:
	cmp	r1, #0
	beq.n	.L3
	str	r2, [r1, #48]
.L3:
	movs	r1, #0
	str	r1, [r0, #44]
.L0:
	bx	lr
