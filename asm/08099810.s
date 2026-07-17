@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08099810
	.thumb_func
Func_08099810:
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r2, #147
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L0
	movs	r1, #200
	ldr	r0, [pc, #12]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x08099679
