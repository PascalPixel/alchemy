@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a8578
	.thumb_func
Func_080a8578:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #120]
	mov	r8, r0
	adds	r7, r1, #0
	ldr	r3, [r3, #0]
	cmp	r2, #0
	bne.n	.L0
	cmp	r7, #3
	ble.n	.L0
	adds	r7, #1
.L0:
	cmp	r7, #1
	bne.n	.L1
	ldr	r2, [pc, #100]
	adds	r6, r3, r2
	ldrb	r0, [r6, #0]
	bl	Func_08077008
	adds	r5, r0, #0
	ldrb	r3, [r5, #15]
	cmp	r3, #99
	bne.n	.L2
	movs	r7, #8
	b.n	.L1
.L2:
	ldrb	r1, [r5, #15]
	ldrb	r0, [r6, #0]
	adds	r1, #1
	bl	Func_08077258
	movs	r2, #146
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	movs	r1, #5
	subs	r0, r0, r3
	bl	Func_08015120
.L1:
	movs	r0, #128
	lsls	r0, r0, #1
	bl	Func_08004938
	adds	r5, r0, #0
	ldr	r0, [pc, #48]
	adds	r1, r5, #0
	adds	r0, r7, r0
	movs	r2, #128
	bl	Func_08015030
	movs	r3, #1
	adds	r0, r5, #0
	negs	r3, r3
	mov	r1, r8
	movs	r2, #0
	bl	Func_08015258
	adds	r0, r5, #0
	bl	Func_08002df0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x00000be6
