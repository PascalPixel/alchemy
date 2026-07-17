@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a34c0
	.thumb_func
Func_080a34c0:
	push	{r5, lr}
	ldr	r3, [pc, #148]
	ldr	r5, [r3, #0]
	bl	Func_080a195c
	bl	Func_080a345c
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #190
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r0, #16
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #32
	movs	r1, #1
	bl	Func_080a1114
	movs	r3, #134
	lsls	r3, r3, #1
	adds	r0, r5, r3
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #36
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #40
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #44
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #48
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #52
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #56
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #60
	movs	r1, #1
	bl	Func_080a1114
	adds	r0, r5, #0
	adds	r0, #64
	movs	r1, #1
	bl	Func_080a1114
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
