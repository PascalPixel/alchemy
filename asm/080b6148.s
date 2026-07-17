@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b6148
	.thumb_func
Func_080b6148:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #92]
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	sub	sp, #20
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	adds	r3, r1, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #60]
	lsls	r2, r2, #3
	adds	r5, r2, r3
	adds	r3, r1, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	ldr	r6, [pc, #52]
	cmp	r3, #0
	beq.n	.L2
	b.n	.L3
.L2:
	ldr	r3, [pc, #20]
	strh	r3, [r6, #0]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #2]
	ldr	r3, [pc, #20]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #20]
	movs	r0, #1
	strh	r3, [r6, #10]
	bl	Func_080030f8
	b.n	.L4
	.4byte 0x00000065
	.4byte 0x00000078
	.4byte 0x00000054
	.4byte 0x00000055
	.4byte 0x03001e74
	.4byte 0x02002024
	.4byte 0x02002224
.L6:
	movs	r0, #1
	bl	Func_080030f8
.L4:
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L5
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L6
	b.n	.L3
.L5:
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	movs	r7, #0
	cmp	r2, r3
	beq.n	.L7
	b.n	.L3
.L7:
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	beq.n	.L8
	b.n	.L3
.L8:
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L6
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L6
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	cmp	r2, r3
	bne.n	.L6
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	bne.n	.L6
	ldr	r3, [pc, #32]
	strh	r3, [r6, #12]
	ldr	r3, [pc, #32]
	strh	r3, [r6, #14]
	b.n	.L9
.L11:
	movs	r0, #1
	bl	Func_080030f8
.L9:
	ldr	r3, [pc, #24]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L10
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L11
	b.n	.L3
	.4byte 0x00000072
	.4byte 0x0000006e
	.4byte 0x03001f64
.L10:
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	movs	r7, #0
	cmp	r2, r3
	beq.n	.L12
	b.n	.L3
.L12:
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	beq.n	.L13
	b.n	.L3
.L13:
	ldrh	r2, [r6, #12]
	ldrh	r3, [r5, #12]
	cmp	r2, r3
	bne.n	.L11
	ldrh	r2, [r6, #14]
	ldrh	r3, [r5, #14]
	cmp	r2, r3
	bne.n	.L11
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	strh	r3, [r6, #0]
	strh	r3, [r6, #4]
	ldr	r3, [pc, #40]
	strh	r2, [r6, #2]
	strh	r3, [r6, #6]
	b.n	.L14
.L16:
	movs	r0, #1
	bl	Func_080030f8
.L14:
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L15
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L16
	b.n	.L3
	.4byte 0x00000045
	.4byte 0x00000058
	.4byte 0x00000043
	.4byte 0x03001f64
.L15:
	ldrh	r2, [r6, #12]
	ldrh	r3, [r5, #12]
	movs	r7, #0
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r6, #14]
	ldrh	r3, [r5, #14]
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	cmp	r2, r3
	bne.n	.L16
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L16
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L16
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	bne.n	.L16
	ldr	r3, [pc, #36]
	strh	r3, [r6, #8]
	ldr	r3, [pc, #36]
	strh	r3, [r6, #10]
	b.n	.L17
.L19:
	movs	r0, #1
	bl	Func_080030f8
.L17:
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L18
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L19
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000074
	.4byte 0x00000075
	.4byte 0x03001f64
.L18:
	ldrh	r2, [r6, #0]
	ldrh	r3, [r5, #0]
	movs	r7, #0
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r6, #2]
	ldrh	r3, [r5, #2]
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r6, #4]
	ldrh	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r6, #6]
	ldrh	r3, [r5, #6]
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r6, #8]
	ldrh	r3, [r5, #8]
	cmp	r2, r3
	bne.n	.L19
	ldrh	r2, [r6, #10]
	ldrh	r3, [r5, #10]
	cmp	r2, r3
	bne.n	.L19
	ldr	r3, [pc, #40]
	strh	r3, [r6, #12]
	ldr	r3, [pc, #40]
	strh	r3, [r6, #14]
	b.n	.L20
.L22:
	movs	r0, #1
	bl	Func_080030f8
.L20:
	ldr	r3, [pc, #32]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L21
	adds	r7, #1
	cmp	r7, #24
	ble.n	.L22
.L3:
	movs	r0, #1
	negs	r0, r0
	b.n	.L23
	movs	r0, r0
	.4byte 0x00000052
	.4byte 0x0000004e
	.4byte 0x03001f64
.L21:
	ldrh	r3, [r5, #12]
	movs	r7, #0
	cmp	r3, #114
	bne.n	.L1
	ldrh	r3, [r5, #14]
	cmp	r3, #110
	beq.n	.L22
.L1:
	movs	r0, #0
.L23:
	add	sp, #20
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
