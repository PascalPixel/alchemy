@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f4028
	.thumb_func
Func_080f4028:
	push	{r5, r6, lr}
	ldr	r3, [pc, #116]
	ldr	r5, [r3, #0]
	movs	r3, #152
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	movs	r3, #255
	lsls	r3, r3, #17
	str	r3, [r5, #32]
	ldr	r3, [pc, #104]
	movs	r6, #0
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #20]
	strh	r6, [r5, #54]
	str	r6, [r5, #28]
	str	r6, [r3, #12]
	str	r6, [r3, #16]
	str	r6, [r5, #24]
	sub	sp, #12
	bl	Func_080049ac
	adds	r0, r5, #0
	adds	r0, #12
	bl	Func_08004cb4
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	Func_08004c1c
	movs	r3, #52
	ldrsh	r0, [r5, r3]
	bl	Func_08004bd4
	mov	r0, sp
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	ldr	r3, [r5, #32]
	adds	r1, r5, #0
	str	r3, [r0, #8]
	ldr	r3, [pc, #44]
	bl	Func_080072f0
	movs	r0, #250
	movs	r1, #192
	ldr	r3, [pc, #40]
	lsls	r1, r1, #8
	lsls	r0, r0, #16
	bl	Func_080072f0
	adds	r1, r0, #0
	movs	r0, #250
	lsls	r0, r0, #16
	ldr	r2, [pc, #28]
	bl	Func_08005258
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x03001ce0
	.4byte 0x03000250
	.4byte 0x0300013c
	.4byte 0x7fff0000
