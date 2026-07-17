@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080fa1d4
	.thumb_func
Func_080fa1d4:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L0
	bl	Func_080fa1ac
.L0:
	bx	ip
