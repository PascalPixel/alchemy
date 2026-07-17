@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c9048
	.thumb_func
Func_080c9048:
	push	{lr}
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #48]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #48]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #32]
	ldr	r1, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	ldrh	r3, [r0, #0]
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0000100e
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x00003537
	.4byte 0x00003f21
	.4byte 0x04000050
	.4byte 0x04000040
	.4byte 0x04000048
	.4byte 0x02002090
	.4byte 0x04000208
.L0:
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #28]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L1:
	strh	r4, [r0, #0]
	movs	r0, #1
	bl	Func_080030f8
	pop	{r0}
	bx	r0
	.4byte 0x00007741
