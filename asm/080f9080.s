@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f9080
	.thumb_func
Func_080f9080:
	push	{r5, r6, r7, lr}
	movs	r5, #240
	ldr	r3, [pc, #172]
	adds	r6, r0, #0
	lsls	r5, r5, #8
	ands	r5, r6
	ands	r6, r3
	cmp	r6, #17
	bne.n	.L0
	ldr	r5, [pc, #164]
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L1
	b.n	.L2
.L1:
	ldr	r0, [pc, #156]
	movs	r1, #7
	bl	Func_080fa4bc
	ldrb	r3, [r5, #0]
	ldr	r2, [pc, #152]
	adds	r3, #1
	strb	r3, [r5, #0]
	movs	r3, #19
	b.n	.L3
.L0:
	ldr	r3, [pc, #144]
	cmp	r6, r3
	bne.n	.L4
	ldr	r3, [pc, #144]
	movs	r2, #0
	strh	r2, [r3, #6]
	ldr	r0, [pc, #140]
	movs	r1, #3
	bl	Func_080fa4bc
	b.n	.L2
.L4:
	cmp	r6, #99
	ble.n	.L5
	ldr	r7, [pc, #132]
	lsls	r4, r6, #3
	adds	r3, r4, #4
	ldrh	r2, [r7, r3]
	cmp	r2, #7
	bne.n	.L6
	ldr	r1, [pc, #124]
.L8:
	lsls	r5, r2, #1
	adds	r3, r5, r2
	lsls	r3, r3, #2
	ldr	r3, [r1, r3]
	ldrb	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L7
	subs	r2, #1
	cmp	r2, #3
	bgt.n	.L8
	movs	r2, #7
	movs	r5, #14
	b.n	.L7
.L6:
	ldr	r1, [pc, #96]
	lsls	r5, r2, #1
.L7:
	adds	r3, r5, r2
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
	ldr	r1, [r7, r4]
	bl	Func_080faa58
	ldr	r3, [pc, #68]
	strh	r6, [r3, r5]
	b.n	.L2
.L5:
	cmp	r6, #79
	ble.n	.L9
	ldr	r0, [pc, #44]
	movs	r1, #255
	movs	r2, #0
	bl	Func_080fb2cc
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #60]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #60]
	lsls	r0, r6, #16
	strh	r2, [r3, #0]
	lsrs	r0, r0, #16
	bl	Func_080fa324
	ldr	r2, [pc, #52]
	movs	r3, #10
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000fff
	.4byte 0x02003014
	.4byte 0x02004290
	.4byte 0x0200303c
	.4byte 0x00000121
	.4byte 0x02003020
	.4byte 0x02004360
	.4byte 0x080fc684
	.4byte 0x080fc624
	.4byte 0x02003034
	.4byte 0x02003008
	.4byte 0x02003000
.L9:
	cmp	r6, #18
	beq.n	.L2
	ldr	r2, [pc, #56]
	ldrb	r3, [r2, #0]
	cmp	r6, r3
	beq.n	.L2
	strb	r6, [r2, #0]
	cmp	r6, #70
	beq.n	.L10
	cmp	r6, #75
	beq.n	.L10
	movs	r0, #2
	cmp	r6, #67
	bne.n	.L11
.L10:
	movs	r0, #3
.L11:
	bl	Func_080037d4
	lsls	r0, r6, #16
	lsrs	r0, r0, #16
	bl	Func_080fa324
	movs	r3, #128
	lsls	r3, r3, #5
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L12
	ldr	r2, [pc, #12]
	ldr	r3, [pc, #4]
	b.n	.L13
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0200303c
	.4byte 0x02003008
.L12:
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #24]
.L13:
	strh	r3, [r2, #0]
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #28]
	movs	r3, #0
.L3:
	strb	r3, [r2, #0]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000004
	.4byte 0x02003008
	.4byte 0x02003034
	.4byte 0x02003010
	.4byte 0x02003014
.L2:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
