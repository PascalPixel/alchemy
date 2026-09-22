.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080148e8, 0x080148e8
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_080cad84, 0x080cad84
	.global Overlay_080d3600
Overlay_080d3600:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	sub	sp, #4
	adds	r5, r1, #0
	cmp	r6, #0
	beq.n	.L_080d369a
	cmp	r5, #0
	beq.n	.L_080d369a
	ldr	r3, [r6, #16]
	ldr	r0, [r5, #16]
	ldr	r1, [r5, #8]
	subs	r0, r0, r3
	ldr	r3, [r6, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r7, r0, #16
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, r0, r7
	mov	r8, r0
	movs	r4, #0
.L_080d3632:
	ldrh	r2, [r6, #6]
	movs	r1, #2
	subs	r3, r7, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_080d365a
	movs	r0, #128
	lsls	r0, r0, #5
	cmp	r3, r0
	ble.n	.L_080d364c
	movs	r3, #128
	lsls	r3, r3, #5
.L_080d364c:
	ldr	r0, [pc, #84]
	cmp	r3, r0
	bge.n	.L_080d3654
	ldr	r3, [pc, #80]
.L_080d3654:
	adds	r3, r2, r3
	strh	r3, [r6, #6]
	b.n	.L_080d365c
.L_080d365a:
	movs	r1, #1
.L_080d365c:
	ldrh	r2, [r5, #6]
	mov	r0, r8
	subs	r3, r0, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_080d3684
	movs	r0, #128
	lsls	r0, r0, #5
	cmp	r3, r0
	ble.n	.L_080d3676
	movs	r3, #128
	lsls	r3, r3, #5
.L_080d3676:
	ldr	r0, [pc, #44]
	cmp	r3, r0
	bge.n	.L_080d367e
	ldr	r3, [pc, #36]
.L_080d367e:
	adds	r3, r2, r3
	strh	r3, [r5, #6]
	b.n	.L_080d3686
.L_080d3684:
	subs	r1, #1
.L_080d3686:
	cmp	r1, #0
	beq.n	.L_080d369a
	movs	r0, #1
	str	r4, [sp, #0]
	bl	sub_08013560
	ldr	r4, [sp, #0]
	adds	r4, #1
	cmp	r4, #59
	ble.n	.L_080d3632
.L_080d369a:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	sub_080cad84
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	cmp	r0, #0
	beq.n	.L_080d36c6
	bl	sub_080200c8
	lsls	r3, r5, #2
	adds	r3, #20
	movs	r2, #0
	str	r2, [r6, r3]
.L_080d36c6:
	pop	{r5, r6, pc}
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080d36ec
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080d36e4
	ldr	r3, [pc, #16]
	str	r3, [r0, #108]
	b.n	.L_080d36ec
.L_080d36e4:
	str	r3, [r0, #108]
	adds	r1, r5, #0
	bl	.L_080d3744
.L_080d36ec:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x36f5
	.2byte 0x080d
	push	{r5, r6, lr}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080d373a
	ldr	r3, [pc, #52]
	ldr	r5, [r0, #80]
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #52]
	lsrs	r3, r3, #1
	movs	r2, #3
	ands	r3, r2
	ldrb	r6, [r1, r3]
	ldrb	r1, [r5, #27]
	movs	r0, #0
	cmp	r0, r1
	bge.n	.L_080d3736
	adds	r4, r5, #0
	adds	r4, #40
.L_080d3720:
	ldmia	r4!, {r2}
	cmp	r2, #0
	beq.n	.L_080d3730
	ldr	r3, [r2, #16]
	cmp	r3, #0
	beq.n	.L_080d3730
	strb	r6, [r2, #5]
	ldrb	r1, [r5, #27]
.L_080d3730:
	adds	r0, #1
	cmp	r0, r1
	blt.n	.L_080d3720
.L_080d3736:
	movs	r3, #1
	strb	r3, [r5, #25]
.L_080d373a:
	pop	{r5, r6, pc}
	.4byte 0x0300122c
	.2byte 0x088c
	.2byte 0x080f
.L_080d3744:
	push	{r5, r6, lr}
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	adds	r6, r1, #0
	cmp	r3, #1
	bne.n	.L_080d377e
	ldr	r5, [r0, #80]
	movs	r0, #0
	ldrb	r4, [r5, #27]
	cmp	r0, r4
	bge.n	.L_080d377a
	adds	r1, r5, #0
	adds	r1, #40
.L_080d3764:
	ldmia	r1!, {r2}
	cmp	r2, #0
	beq.n	.L_080d3774
	ldr	r3, [r2, #16]
	cmp	r3, #0
	beq.n	.L_080d3774
	strb	r6, [r2, #5]
	ldrb	r4, [r5, #27]
.L_080d3774:
	adds	r0, #1
	cmp	r0, r4
	blt.n	.L_080d3764
.L_080d377a:
	movs	r3, #1
	strb	r3, [r5, #25]
.L_080d377e:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	movs	r0, #255
	ands	r0, r6
	bl	sub_080cad84
	cmp	r5, #0
	beq.n	.L_080d37d0
	cmp	r0, #0
	beq.n	.L_080d37d0
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r6
	str	r0, [r5, #104]
	cmp	r3, #0
	bne.n	.L_080d37c2
	adds	r2, r5, #0
	movs	r3, #40
	adds	r2, #100
	strh	r3, [r2, #0]
	ldr	r1, [pc, #24]
	ldr	r3, [r0, #52]
	lsls	r3, r3, #1
	str	r3, [r5, #52]
	ldr	r3, [r0, #48]
	str	r3, [r5, #48]
	adds	r3, r5, #0
	adds	r3, #89
	strb	r1, [r3, #0]
.L_080d37c2:
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_080200a8
	b.n	.L_080d37d0
	.2byte 0x0000
	.2byte 0x0000
.L_080d37d0:
	pop	{r5, r6, r7, pc}
