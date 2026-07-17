@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080fa1ac
	.thumb_func
Func_080fa1ac:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L0
	movs	r2, #12
	b.n	.L1
.L0:
	movs	r2, #3
.L1:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
