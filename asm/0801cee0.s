@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801cee0
	.thumb_func
Func_0801cee0:
	push	{lr}
	ldr	r2, [pc, #80]
	adds	r0, r0, r2
	ldrh	r3, [r0, #0]
	cmp	r3, #1
	beq.n	.L0
	cmp	r3, #1
	bgt.n	.L1
	cmp	r3, #0
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r3, #2
	beq.n	.L4
	b.n	.L3
.L2:
	ldr	r3, [pc, #56]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r1, r3, r2
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #1
	bhi.n	.L3
	b.n	.L5
.L0:
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	adds	r1, r3, r2
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #23
	bhi.n	.L3
	b.n	.L5
.L4:
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #28]
	adds	r1, r3, r2
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #14
	bhi.n	.L3
.L5:
	adds	r3, r2, #1
	strb	r3, [r1, #0]
.L3:
	pop	{r0}
	bx	r0
	.4byte 0x00000574
	.4byte 0x02000240
	.4byte 0x00000205
	.4byte 0x00000206
	bx	lr
