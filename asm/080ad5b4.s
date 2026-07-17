@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ad5b4
	.thumb_func
Func_080ad5b4:
	push	{r5, r6, lr}
	adds	r5, r3, #0
	ldr	r3, [pc, #52]
	movs	r6, #137
	ldr	r4, [r3, #0]
	lsls	r6, r6, #2
	lsls	r3, r0, #2
	adds	r3, r3, r6
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L0
	lsls	r0, r0, #1
	adds	r6, #16
	adds	r3, r0, r6
	strh	r1, [r4, r3]
	movs	r3, #143
	lsls	r3, r3, #2
	adds	r0, r0, r3
	adds	r3, r2, #0
	cmp	r5, #0
	beq.n	.L1
	ldr	r3, [pc, #12]
	orrs	r3, r2
.L1:
	strh	r3, [r4, r0]
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0xffff8000
	.4byte 0x03001f2c
