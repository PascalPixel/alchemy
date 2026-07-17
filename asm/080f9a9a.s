@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080f9a9a
	.thumb_func
Func_080f9a9a:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L0
	ldr	r0, [pc, #12]
	cmp	r2, r0
	bcc.n	.L1
	lsrs	r0, r2, #14
	beq.n	.L0
.L1:
	movs	r3, #0
.L0:
	pop	{r0}
	bx	lr
	.4byte 0x080fb7a0
