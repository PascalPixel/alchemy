@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c3e8
	.thumb_func
Func_0801c3e8:
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r0, #141
	ldr	r1, [r3, #0]
	lsls	r0, r0, #2
	adds	r2, r1, r0
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #40]
	adds	r3, r3, r0
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #140
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	Func_08016418
	ldr	r0, [pc, #16]
	bl	Func_08004278
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x0000ffff
	.4byte 0x0801c3e9
