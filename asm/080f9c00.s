@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080f9c00
	.thumb_func
Func_080f9c00:
	mov	ip, lr
	bl	Func_080f9ab4
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L0
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L0:
	bx	ip
