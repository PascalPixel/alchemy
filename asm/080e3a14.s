@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080e3a14
	.thumb_func
Func_080e3a14:
	push	{lr}
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #24]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r3, [r3, r2]
	cmp	r3, #127
	ble.n	.L0
	ldr	r3, [pc, #12]
	ldr	r3, [r3, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001ae8
