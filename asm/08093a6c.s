@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08093a6c
	.thumb_func
Func_08093a6c:
	push	{r5, lr}
	subs	r3, r1, #1
	adds	r5, r0, #0
	cmp	r3, #6
	bhi.n	.L0
	ldr	r2, [pc, #92]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08093a9c
	.4byte 0x08093aa0
	.4byte 0x08093aa4
	.4byte 0x08093aa8
	.4byte 0x08093aac
	.4byte 0x08093ab0
	.4byte 0x08093ac4
	ldr	r1, [pc, #56]
	b.n	.L0
	ldr	r1, [pc, #56]
	b.n	.L0
	ldr	r1, [pc, #56]
	b.n	.L0
	ldr	r1, [pc, #56]
	b.n	.L0
	ldr	r1, [pc, #56]
	b.n	.L0
	ldr	r3, [pc, #56]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_08092054
	ldr	r1, [pc, #48]
	str	r0, [r5, #104]
	b.n	.L0
	ldr	r1, [pc, #44]
.L0:
	adds	r0, r5, #0
	bl	Func_08009098
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08093a80
	.4byte 0x0809fe00
	.4byte 0x0809fd44
	.4byte 0x0809fe10
	.4byte 0x0809fecc
	.4byte 0x0809ff18
	.4byte 0x02000240
	.4byte 0x0809ff2c
	.4byte 0x0809fe04
