@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fab3c
	.thumb_func
Func_080fab3c:
	push	{r4, r5, r6, lr}
	adds	r6, r0, #0
	ldr	r1, [r6, #52]
	ldr	r0, [pc, #52]
	cmp	r1, r0
	bne.n	.L0
	adds	r0, r1, #1
	str	r0, [r6, #52]
	ldr	r0, [r6, #4]
	movs	r1, #128
	lsls	r1, r1, #24
	orrs	r0, r1
	str	r0, [r6, #4]
	ldrb	r4, [r6, #8]
	ldr	r5, [r6, #44]
	cmp	r4, #0
	ble.n	.L1
.L2:
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080f9ef8
	subs	r4, #1
	adds	r5, #80
	cmp	r4, #0
	bgt.n	.L2
.L1:
	ldr	r0, [pc, #8]
	str	r0, [r6, #52]
.L0:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x68736d53
