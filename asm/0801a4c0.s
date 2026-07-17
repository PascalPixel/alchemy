@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801a4c0
	.thumb_func
Func_0801a4c0:
	push	{lr}
	ldr	r3, [pc, #44]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r2, r1, r3
	ldr	r3, [pc, #44]
	lsls	r0, r0, #2
	ldr	r3, [r3, r0]
	movs	r0, #192
	lsls	r0, r0, #3
	str	r3, [r2, #0]
	adds	r3, r1, r0
	movs	r2, #2
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	adds	r0, r1, #0
	movs	r1, #0
	bl	Func_0801a5a4
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e94
	.4byte 0x00000604
	.4byte 0x080308a0
