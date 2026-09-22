.syntax unified
	.thumb
	.set sub_08016e0c, 0x08016e0c
	.global Overlay_08016ea4
Overlay_08016ea4:
	push	{r4, lr}
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	bl	sub_08016e0c
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	ldr	r2, [pc, #12]
	movs	r4, #1
	b.n	.L_08016ed2
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.2byte 0x7d64
	.2byte 0x0801
.L_08016ed0:
	adds	r2, #4
.L_08016ed2:
	ldr	r1, [r2, #0]
	adds	r0, r1, #0
	adds	r0, #40
	ldrb	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L_08016ee6
	ldrh	r0, [r1, #40]
	cmp	r3, r0
	bne.n	.L_08016ed0
	movs	r4, #0
.L_08016ee6:
	ldr	r1, [pc, #56]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #0]
	str	r0, [r1, #0]
	ldr	r1, [pc, #52]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #4]
	str	r0, [r1, #0]
	ldr	r1, [pc, #48]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #8]
	str	r0, [r1, #0]
	ldr	r1, [pc, #44]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #12]
	str	r0, [r1, #0]
	ldr	r1, [pc, #40]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #16]
	str	r0, [r1, #0]
	ldr	r1, [pc, #36]
	ldr	r0, [r2, #0]
	adds	r0, #20
	str	r0, [r1, #0]
	adds	r0, r4, #0
	pop	{r4}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02007404
	.4byte 0x02007410
	.4byte 0x02007414
	.4byte 0x02007400
	.4byte 0x02007418
	.4byte 0x02007408
