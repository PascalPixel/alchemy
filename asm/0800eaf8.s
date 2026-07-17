@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800eaf8
	.thumb_func
Func_0800eaf8:
	push	{r5, lr}
	ldr	r3, [pc, #140]
	ldr	r1, [r3, #0]
	movs	r5, #0
	movs	r0, #0
	cmp	r1, #0
	beq.n	.L0
	ldr	r4, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	ldr	r0, [pc, #128]
	adds	r3, r4, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #185
	b.n	.L2
.L1:
	movs	r2, #132
	lsls	r2, r2, #2
	adds	r3, r4, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #186
	b.n	.L2
.L3:
	ldr	r2, [pc, #96]
	adds	r3, r4, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #187
.L2:
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r5, #1
	b.n	.L5
.L4:
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r4, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r2, #136
	lsls	r2, r2, #2
	b.n	.L7
.L6:
	ldr	r2, [pc, #48]
	adds	r3, r4, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	ldr	r2, [pc, #40]
.L7:
	adds	r3, r4, r2
	ldrh	r0, [r3, #0]
	bl	Func_0800ea60
	adds	r5, r0, #0
.L5:
	adds	r0, r5, #0
.L0:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x03001c94
	.4byte 0x00000216
	.4byte 0x0000021a
	.4byte 0x00000222
