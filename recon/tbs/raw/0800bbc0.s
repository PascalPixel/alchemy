.syntax unified
	.thumb
	.global	AnimationObject_Allocate
	.thumb_func
AnimationObject_Allocate:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r4, #0
	str	r4, [sp, #0]
	adds	r7, r0, #0
	bl	Resource_GetMetadataRecordFar
	ldr	r3, [pc, #20]
	adds	r6, r0, #0
	ldr	r2, [r3]
	ldrb	r3, [r6]
	movs	r5, #0
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq	.L0_0
	ldrb	r3, [r2, #4]
	movs	r1, #0
	b	.L0_1
	.4byte	0x03001e5c
.L0_3:
	adds	r1, #1
	adds	r2, #24
	cmp	r1, #63
	bgt	.L0_2
	ldrb	r3, [r2, #4]
.L0_1:
	cmp	r3, #0
	bne	.L0_3
	adds	r4, r2, #0
.L0_2:
	cmp	r4, #0
	beq	.L0_0
	ldr	r3, [pc, #48]
	ldr	r0, [r6, #12]
	adds	r5, r4, #0
	mov	r8, r3
	strh	r7, [r5]
	cmp	r0, #0
	bne	.L0_4
	adds	r0, r7, #0
	bl	Animation_LookupValueByKey
.L0_4:
	ldr	r2, [r6, #16]
	str	r0, [r5, #8]
	str	r2, [r5, #12]
	ldrb	r3, [r6, #10]
	strb	r3, [r5, #7]
	movs	r3, #255
	strb	r3, [r5, #22]
	ldr	r3, [r2]
	str	r3, [r5, #16]
	mov	r3, r8
	strb	r3, [r5, #20]
	ldrb	r3, [r6, #4]
	strb	r3, [r5, #4]
	mov	r3, r8
	strb	r3, [r5, #5]
	b	.L0_0
	.4byte	0x00000000
.L0_0:
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
