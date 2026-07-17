@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801965c
	.thumb_func
Func_0801965c:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #92]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #92]
	adds	r5, r2, #0
	adds	r2, r6, r3
	movs	r3, #0
	adds	r7, r1, #0
	strh	r3, [r2, #0]
	movs	r1, #1
	bl	Func_08018038
	subs	r5, #1
	movs	r0, #0
	cmp	r0, r5
	bcs.n	.L0
	movs	r2, #235
	lsls	r2, r2, #4
	ldrh	r3, [r6, r2]
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L0
	mov	ip, r5
	adds	r2, r6, r2
	movs	r4, #0
.L2:
	adds	r0, #1
	adds	r4, #2
	cmp	r0, ip
	bcs.n	.L1
	adds	r2, #2
	ldrh	r3, [r2, #0]
	adds	r1, r4, #0
	strh	r3, [r1, r7]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L2
	b.n	.L3
.L0:
	movs	r1, #0
	b.n	.L3
.L1:
	lsls	r1, r0, #1
.L3:
	ldr	r3, [pc, #8]
	strh	r3, [r1, r7]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x000012b2
