@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08021848
	.thumb_func
Func_08021848:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #120]
	ldr	r3, [pc, #124]
	movs	r7, #0
	mov	r8, r2
	mov	sl, r3
.L5:
	lsls	r3, r7, #1
	adds	r3, r3, r7
	ldr	r2, [pc, #116]
	lsls	r3, r3, #7
	movs	r6, #0
	adds	r5, r3, r2
.L4:
	adds	r0, r5, #0
	movs	r1, #64
	ldr	r2, [pc, #108]
	bl	Func_0800730c
	movs	r4, #1
	adds	r0, r5, #4
.L3:
	adds	r1, r6, #0
	cmp	r7, #1
	bne.n	.L0
	cmp	r4, #1
	ble.n	.L1
.L0:
	cmp	r7, #0
	bne.n	.L2
	subs	r3, r4, #2
	cmp	r6, r3
	ble.n	.L2
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L2
	movs	r1, #0
.L2:
	lsls	r1, r1, #3
	mov	r3, r8
	ldr	r3, [r3, r1]
	ldr	r2, [r0, #0]
	eors	r2, r3
	str	r2, [r0, #0]
	adds	r1, #4
	mov	r3, r8
	ldr	r2, [r0, #32]
	ldr	r1, [r3, r1]
	eors	r2, r1
	str	r2, [r0, #32]
.L1:
	adds	r4, #1
	adds	r0, #4
	cmp	r4, #7
	ble.n	.L3
	adds	r6, #1
	adds	r5, #64
	cmp	r6, #5
	ble.n	.L4
	adds	r7, #1
	cmp	r7, #1
	ble.n	.L5
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08037250
	.4byte 0x03000168
	.4byte 0x06006280
	.4byte 0x44444444
