@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801173c
	.thumb_func
Func_0801173c:
	push	{lr}
	ldr	r3, [pc, #68]
	ldr	r2, [pc, #68]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #68]
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #159
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #44]
	bl	Func_08002f40
	ldr	r1, [pc, #40]
	bl	Func_08005340
	bl	Func_080113e4
	ldr	r0, [pc, #36]
	bl	Func_0800439c
	movs	r0, #1
	bl	Func_080030f8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x03001cfc
	.4byte 0x0801161d
	.4byte 0x000000d5
	.4byte 0x02010000
	.4byte 0x0801179d
