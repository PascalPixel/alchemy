@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08095214
	.thumb_func
Func_08095214:
	push	{lr}
	ldr	r1, [pc, #28]
	movs	r0, #30
	bl	Func_080048f4
	movs	r3, #252
	lsls	r3, r3, #5
	adds	r2, r0, r3
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	movs	r1, #0
	adds	r0, r0, r3
	strh	r1, [r0, #0]
	pop	{r0}
	bx	r0
	.4byte 0x00001f88
	.4byte 0x00007fff
	.4byte 0x00001f82
