@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801fd34
	.thumb_func
Func_0801fd34:
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #64]
	ldr	r6, [pc, #64]
	movs	r5, #0
.L1:
	ldr	r3, [r7, #0]
	lsls	r2, r5, #3
	adds	r3, r3, r2
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #8
	bl	Func_08002322
	cmp	r0, #0
	bge.n	.L0
	ldr	r3, [pc, #44]
	adds	r0, r0, r3
.L0:
	asrs	r3, r0, #14
	lsls	r1, r3, #1
	adds	r2, r3, #0
	adds	r1, #22
	adds	r2, #16
	adds	r3, #20
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #0]
	adds	r6, #2
	adds	r5, #1
	cmp	r5, #3
	ble.n	.L1
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001800
	.4byte 0x050001d0
	.4byte 0x00003fff
