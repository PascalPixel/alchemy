@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0184
	.thumb_func
Func_080c0184:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #0]
	subs	r0, r3, #1
	cmp	r0, #31
	bhi.n	.L0
	ldr	r2, [pc, #24]
	lsrs	r0, r0, #2
	lsls	r0, r0, #5
	adds	r0, r0, r2
	ldr	r3, [pc, #20]
	ldr	r1, [pc, #20]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ef8
	.4byte 0x080c5a30
	.4byte 0x040000d4
	.4byte 0x06005000
	.4byte 0x84000008
