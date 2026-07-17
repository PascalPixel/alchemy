@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080199ec
	.thumb_func
Func_080199ec:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #76]
	ldr	r1, [pc, #76]
	ldr	r5, [r3, #0]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	adds	r7, r0, #0
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L0
	bl	Func_080f9048
	cmp	r0, #0
	bne.n	.L0
	movs	r6, #1
.L0:
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #56]
	ldr	r2, [r3, #0]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	ldr	r3, [pc, #48]
	ldr	r2, [r3, #0]
.L1:
	ldr	r3, [pc, #48]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r6, #1
.L2:
	cmp	r6, #0
	beq.n	.L3
	movs	r3, #0
	strh	r3, [r7, #20]
	movs	r0, #1
	b.n	.L4
.L3:
	movs	r0, #0
.L4:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012f9
	.4byte 0x03001c94
	.4byte 0x00000ea4
	.4byte 0x03001af8
	.4byte 0x00000303
