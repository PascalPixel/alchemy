@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800479c
	.thumb_func
Func_0800479c:
	push	{lr}
	ldr	r0, [pc, #96]
	bl	Func_08002f40
	movs	r1, #192
	ldr	r3, [pc, #92]
	lsls	r1, r1, #19
	ldr	r2, [pc, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #88]
	ldr	r1, [pc, #92]
	ldr	r2, [pc, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	movs	r2, #131
	ldr	r3, [pc, #80]
	lsls	r2, r2, #7
	strh	r2, [r3, #0]
	ldr	r2, [pc, #80]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #197
	lsls	r2, r2, #6
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #68]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #68]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #64]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #64]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r3, [pc, #16]
	ldr	r0, [pc, #60]
	adds	r1, #32
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r0}
	bx	r0
	.4byte 0x00000013
	.4byte 0x040000d4
	.4byte 0x84000800
	.4byte 0x0800777c
	.4byte 0x050001e0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x00003960
	.4byte 0x00002920
	.4byte 0x000049a0
	.4byte 0x000051c0
	.4byte 0x000059e0
	.4byte 0x0800779c
	.4byte 0x800000e0
