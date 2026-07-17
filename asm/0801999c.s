@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801999c
	.thumb_func
Func_0801999c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #60]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	bl	Func_080f9048
	cmp	r0, #0
	bne.n	.L0
	movs	r5, #1
.L0:
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r5, #1
.L1:
	cmp	r5, #0
	beq.n	.L2
	movs	r3, #0
	strh	r3, [r6, #20]
	movs	r0, #1
	b.n	.L3
.L2:
	movs	r0, #0
.L3:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x000012f9
	.4byte 0x03001ae8
	.4byte 0x00000303
