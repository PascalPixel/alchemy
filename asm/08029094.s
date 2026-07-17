@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08029094
	.thumb_func
Func_08029094:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #100]
	adds	r7, r0, #0
	adds	r0, r3, #0
	ldr	r3, [r6, #0]
	adds	r4, r2, #0
	movs	r2, #1
	lsls	r1, r1, #16
	ands	r3, r2
	asrs	r5, r1, #16
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #2
	negs	r0, r0
	b.n	.L1
.L2:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	ldr	r1, [r6, #0]
	movs	r3, #64
	ands	r1, r3
	cmp	r1, #0
	beq.n	.L4
.L3:
	ldrh	r3, [r0, #0]
	ldr	r2, [pc, #28]
	eors	r3, r2
	strh	r3, [r0, #0]
	b.n	.L5
.L4:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L7
	adds	r3, r5, #1
	b.n	.L8
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001b04
.L7:
	ldrh	r3, [r4, #0]
	movs	r2, #198
	adds	r3, #1
	strh	r3, [r4, #0]
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	cmp	r3, r2
	ble.n	.L9
	strh	r1, [r4, #0]
	b.n	.L9
.L6:
	ldr	r3, [r6, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #0
	bne.n	.L11
	subs	r3, r5, #1
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L12
.L11:
	ldrh	r3, [r4, #0]
	subs	r3, #1
	strh	r3, [r4, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L12
	ldr	r3, [pc, #44]
	strh	r3, [r4, #0]
.L12:
	cmp	r5, #0
	bge.n	.L13
	movs	r5, #200
	b.n	.L13
.L10:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L15
	strh	r3, [r4, #0]
	adds	r3, r5, #0
	adds	r3, #10
.L8:
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L9
	movs	r0, r0
	.4byte 0x00000063
.L15:
	ldrh	r2, [r4, #0]
	adds	r3, r2, #0
	adds	r3, #10
	movs	r1, #198
	strh	r3, [r4, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L9
	adds	r3, r2, #0
	subs	r3, #89
	strh	r3, [r4, #0]
.L9:
	cmp	r5, #200
	ble.n	.L13
	movs	r5, #0
.L13:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	Func_08028ef0
	b.n	.L5
.L14:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L16
	strh	r3, [r4, #0]
	adds	r3, r5, #0
	subs	r3, #10
	lsls	r3, r3, #16
	asrs	r5, r3, #16
	b.n	.L17
.L16:
	ldrh	r2, [r4, #0]
	adds	r3, r2, #0
	subs	r3, #10
	strh	r3, [r4, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L17
	adds	r3, r2, #0
	adds	r3, #89
	strh	r3, [r4, #0]
.L17:
	cmp	r5, #0
	bge.n	.L18
	movs	r5, #200
.L18:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r4, #0
	bl	Func_08028ef0
.L5:
	adds	r0, r5, #0
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
