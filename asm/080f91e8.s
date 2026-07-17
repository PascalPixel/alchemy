@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f91e8
	.thumb_func
Func_080f91e8:
	push	{r5, r6, lr}
	ldr	r1, [pc, #72]
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	beq.n	.L0
	cmp	r2, #1
	bne.n	.L1
	ldr	r3, [pc, #60]
	ldrb	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L0
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	b.n	.L0
.L1:
	adds	r3, #255
	strb	r3, [r1, #0]
.L0:
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #44]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	ldrh	r4, [r1, #0]
	cmp	r2, r3
	beq.n	.L2
	subs	r0, r2, r3
	cmp	r0, #0
	ble.n	.L3
	ldr	r3, [pc, #28]
	ldrh	r3, [r3, #0]
	adds	r3, r4, r3
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x02003000
	.4byte 0x02004210
	.4byte 0x02003034
	.4byte 0x02003008
	.4byte 0x02003010
.L3:
	ldr	r3, [pc, #148]
	ldrh	r3, [r3, #0]
	subs	r3, r4, r3
.L4:
	strh	r3, [r1, #0]
	ldr	r3, [pc, #144]
	ldr	r1, [pc, #148]
	ldrh	r4, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	subs	r3, r3, r2
	eors	r3, r0
	cmp	r3, #0
	bge.n	.L5
	strh	r4, [r1, #0]
.L5:
	ldrh	r2, [r1, #0]
	ldr	r0, [pc, #128]
	movs	r1, #255
	bl	Func_080fb2cc
.L2:
	ldr	r3, [pc, #124]
	ldr	r1, [pc, #124]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	ldrh	r4, [r1, #0]
	cmp	r2, r3
	beq.n	.L6
	subs	r0, r2, r3
	cmp	r0, #0
	ble.n	.L7
	ldr	r3, [pc, #108]
	ldrh	r3, [r3, #0]
	adds	r3, r4, r3
	b.n	.L8
.L7:
	ldr	r3, [pc, #100]
	ldrh	r3, [r3, #0]
	subs	r3, r4, r3
.L8:
	strh	r3, [r1, #0]
	ldr	r3, [pc, #84]
	ldr	r6, [pc, #84]
	ldrh	r1, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r4, #0
	ldrsh	r2, [r6, r4]
	subs	r3, r3, r2
	eors	r3, r0
	cmp	r3, #0
	bge.n	.L9
	strh	r1, [r6, #0]
.L9:
	ldr	r5, [pc, #56]
	ldrh	r1, [r6, #0]
	adds	r0, r5, #0
	bl	Func_080fb2a4
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r3, #244
	lsls	r2, r2, #18
	lsls	r3, r3, #24
	adds	r2, r2, r3
	asrs	r2, r2, #16
	adds	r0, r5, #0
	movs	r1, #255
	bl	Func_080fb334
.L6:
	bl	Func_080f9c44
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02003010
	.4byte 0x02003034
	.4byte 0x02003008
	.4byte 0x02004290
	.4byte 0x02003030
	.4byte 0x02003038
	.4byte 0x0200300c
