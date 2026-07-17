@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080f9ba4
	.thumb_func
Func_080f9ba4:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #18]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
