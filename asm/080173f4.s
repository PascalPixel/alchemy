@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080173f4
	.thumb_func
Func_080173f4:
	push	{r5, lr}
	ldr	r3, [pc, #76]
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #95
	ldr	r5, [r3, #0]
	bl	Func_08003fa4
	ldr	r2, [pc, #64]
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	adds	r2, r5, r3
	movs	r3, #9
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, r5, r3
	movs	r3, #10
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r1, #0
	adds	r3, r5, r2
	strh	r1, [r3, #0]
	ldr	r3, [pc, #48]
	adds	r2, r5, r3
	movs	r3, #15
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	adds	r5, r5, r2
	strh	r1, [r5, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #40]
	bl	Func_080041d8
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012b8
	.4byte 0x000012b0
	.4byte 0x00000ea8
	.4byte 0x00000eac
	.4byte 0x00000eae
	.4byte 0x000012b2
	.4byte 0x0801789d
