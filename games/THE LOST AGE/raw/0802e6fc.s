.syntax unified
	.thumb
	.set sub_08021918, 0x08021918
	.global Overlay_0802e6fc
Overlay_0802e6fc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #16]
	movs	r3, #3
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r3, #40
	sub	sp, #8
	movs	r7, #0
	mov	r8, r3
	movs	r4, #0
.L_0802e718:
	mov	r3, r8
	ldr	r5, [r6, r3]
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_0802e75a
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08021918
	ldrb	r3, [r0, #5]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r1, r3
	bge.n	.L_0802e754
	ldrb	r3, [r0, #4]
	ldr	r2, [r5, #12]
	strb	r3, [r5, #4]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	strb	r4, [r5, #20]
	str	r3, [r5, #16]
	lsls	r3, r7, #4
	strh	r3, [r5, #2]
	movs	r3, #16
	strb	r3, [r5, #21]
	movs	r3, #255
	strb	r4, [r5, #23]
	strb	r3, [r5, #22]
.L_0802e754:
	ldrb	r3, [r0, #7]
	strh	r4, [r6, #18]
	strb	r3, [r6, #23]
.L_0802e75a:
	adds	r7, #1
	adds	r6, #56
	cmp	r7, #9
	ble.n	.L_0802e718
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
