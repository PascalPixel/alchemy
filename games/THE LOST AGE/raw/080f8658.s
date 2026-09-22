.syntax unified
	.thumb
	.set sub_080143ac, 0x080143ac
	.set sub_08020018, 0x08020018
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_08038098, 0x08038098
	.set sub_080380a8, 0x080380a8
	.set sub_080380b8, 0x080380b8
	.set sub_080380c8, 0x080380c8
	.set sub_080380e0, 0x080380e0
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080f811c, 0x080f811c
	.set sub_080f8610, 0x080f8610
	.set sub_08104b58, 0x08104b58
	.set sub_08108088, 0x08108088
	.global Overlay_080f8658
Overlay_080f8658:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #152]
	movs	r3, #40
	adds	r6, r1, #0
	bl	sub_08038090
	ldr	r3, [pc, #144]
	adds	r1, r6, #0
	mov	r8, r3
	mov	r0, r8
	movs	r3, #40
	movs	r2, #48
	bl	sub_08038098
	movs	r3, #52
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	movs	r3, #40
	adds	r0, r5, #0
	movs	r2, #88
	bl	sub_080f8610
	movs	r3, #56
	ldrsh	r5, [r7, r3]
	ldrh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r3, r3, #18
	cmp	r5, r3
	bge.n	.L_080f86a0
	movs	r0, #4
	bl	sub_080380b8
.L_080f86a0:
	cmp	r5, #0
	bne.n	.L_080f86aa
	movs	r0, #2
	bl	sub_080380b8
.L_080f86aa:
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #40
	bl	sub_080f8610
	movs	r0, #15
	bl	sub_080380b8
	adds	r1, r6, #0
	ldr	r0, [pc, #68]
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038090
	mov	r0, r8
	adds	r1, r6, #0
	movs	r3, #48
	movs	r2, #48
	bl	sub_08038098
	movs	r3, #58
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r3, #48
	movs	r2, #48
	bl	sub_080f8610
	movs	r3, #54
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #48
	bl	sub_080f8610
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0810593c
	.4byte 0x08105940
	.2byte 0x5944
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r2, #0
	adds	r5, r1, #0
	mov	r8, r0
	adds	r1, r6, #0
	ldr	r0, [pc, #280]
	movs	r2, #0
	movs	r3, #32
	sub	sp, #4
	bl	sub_08038080
	movs	r7, #40
	ldrh	r0, [r5, #60]
	adds	r2, r6, #0
	movs	r3, #16
	movs	r1, #3
	str	r7, [sp, #0]
	bl	sub_080380a8
	mov	r3, r8
	ldrh	r2, [r3, #60]
	ldrh	r3, [r5, #60]
	cmp	r2, r3
	beq.n	.L_080f8770
	adds	r0, r2, #0
	movs	r3, #64
	adds	r2, r6, #0
	movs	r1, #3
	str	r7, [sp, #0]
	bl	sub_080380a8
	mov	r3, r8
	ldrh	r2, [r3, #60]
	ldrh	r3, [r5, #60]
	cmp	r2, r3
	bls.n	.L_080f8764
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #36
	movs	r3, #0
	bl	sub_08104b58
	b.n	.L_080f8770
.L_080f8764:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #36
	movs	r3, #1
	bl	sub_08104b58
.L_080f8770:
	ldr	r0, [pc, #196]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08038080
	movs	r7, #56
	ldrh	r0, [r5, #62]
	adds	r2, r6, #0
	movs	r3, #16
	movs	r1, #3
	str	r7, [sp, #0]
	bl	sub_080380a8
	mov	r3, r8
	ldrh	r2, [r3, #62]
	ldrh	r3, [r5, #62]
	cmp	r2, r3
	beq.n	.L_080f87c8
	adds	r0, r2, #0
	movs	r3, #64
	adds	r2, r6, #0
	movs	r1, #3
	str	r7, [sp, #0]
	bl	sub_080380a8
	mov	r3, r8
	ldrh	r2, [r3, #62]
	ldrh	r3, [r5, #62]
	cmp	r2, r3
	bls.n	.L_080f87bc
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #52
	movs	r3, #0
	bl	sub_08104b58
	b.n	.L_080f87c8
.L_080f87bc:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #52
	movs	r3, #1
	bl	sub_08104b58
.L_080f87c8:
	ldr	r0, [pc, #112]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #64
	bl	sub_08038080
	adds	r7, r5, #0
	movs	r3, #72
	adds	r7, #64
	mov	r5, r8
	ldrh	r0, [r7, #0]
	adds	r2, r6, #0
	str	r3, [sp, #0]
	mov	sl, r3
	movs	r1, #3
	movs	r3, #16
	adds	r5, #64
	bl	sub_080380a8
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	beq.n	.L_080f8828
	mov	r3, sl
	adds	r0, r2, #0
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #64
	movs	r1, #3
	bl	sub_080380a8
	ldrh	r2, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r2, r3
	bls.n	.L_080f881c
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #68
	movs	r3, #0
	bl	sub_08104b58
	b.n	.L_080f8828
.L_080f881c:
	adds	r0, r6, #0
	movs	r1, #44
	movs	r2, #68
	movs	r3, #1
	bl	sub_08104b58
.L_080f8828:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000104b
	.4byte 0x0000104c
	.2byte 0x104f
	.2byte 0x0000
.L_080f8840:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	mov	r8, r0
	mov	sl, r1
	adds	r7, r2, #0
	bl	sub_080143ac
	adds	r5, r0, #0
	movs	r6, #0
	cmp	r5, #0
	beq.n	.L_080f8876
	ldr	r2, [pc, #36]
	movs	r1, #128
	bl	sub_08108088
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	sub_080380c8
	adds	r6, r0, #0
.L_080f8876:
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x01fa
	.2byte 0x0000
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080f88be
	movs	r3, #1
	strb	r3, [r0, #5]
	movs	r2, #128
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	ldrh	r1, [r0, #22]
	ldr	r3, [pc, #32]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #22]
	ldrh	r3, [r0, #8]
	ldrb	r2, [r0, #23]
	strb	r3, [r0, #20]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r0, #21]
	strb	r3, [r0, #23]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #21]
.L_080f88be:
	pop	{pc}
	.2byte 0xfe00
	.2byte 0xffff
	push	{lr}
	movs	r0, #0
	movs	r2, #0
	bl	sub_080380e0
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	sub	sp, #8
	movs	r6, #0
	movs	r3, #5
	adds	r5, r7, #0
	str	r6, [r7, #16]
	adds	r5, #16
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #13
	bl	sub_080f811c
	ldr	r5, [r5, #0]
	movs	r3, #3
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r3, #12
	movs	r1, #0
	movs	r2, #3
	bl	sub_08038070
	movs	r1, #8
	negs	r1, r1
	movs	r2, #11
	adds	r0, r5, #0
	bl	.L_080f8840
	movs	r2, #13
	mov	r8, r2
	mov	r3, r8
	strb	r3, [r0, #5]
	movs	r3, #255
	strb	r3, [r7, #28]
	movs	r3, #254
	str	r0, [r7, #20]
	strb	r3, [r0, #15]
	ldr	r2, [r7, #24]
	subs	r3, #255
	strb	r6, [r7, #29]
	strb	r3, [r2, #15]
	bl	sub_080143ac
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L_080f8960
	ldr	r2, [pc, #96]
	movs	r1, #128
	bl	sub_08108088
	movs	r3, #4
	negs	r3, r3
	movs	r1, #128
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r3, #84
	lsls	r1, r1, #23
	adds	r0, r6, #0
	bl	sub_080380c8
	mov	r2, r8
	strb	r2, [r0, #5]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
.L_080f8960:
	bl	sub_080143ac
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L_080f8990
	ldr	r2, [pc, #52]
	movs	r1, #128
	bl	sub_08108088
	movs	r3, #8
	movs	r1, #128
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r3, #84
	lsls	r1, r1, #23
	adds	r0, r6, #0
	bl	sub_080380c8
	movs	r2, #190
	mov	r3, r8
	lsls	r2, r2, #1
	strb	r3, [r0, #5]
	adds	r3, r7, r2
	str	r0, [r3, #0]
.L_080f8990:
	adds	r0, r5, #0
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000001fe
	.2byte 0x01ff
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	sub	sp, #32
	ldr	r5, [r3, #0]
	bl	sub_080ad0f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	sl, r0
	movs	r4, #0
	cmp	r4, sl
	bge.n	.L_080f8a38
	movs	r2, #8
	movs	r3, #140
	add	r2, sp
	lsls	r3, r3, #1
	adds	r7, r5, r3
	mov	r8, r2
	add	r6, sp, #16
	adds	r5, #248
.L_080f89d6:
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	ldr	r0, [r5, #0]
	movs	r2, #229
	lsls	r3, r3, #15
	lsls	r2, r2, #15
	subs	r1, r2, r3
	cmp	r0, #0
	beq.n	.L_080f8a2e
	ldrb	r3, [r0, #9]
	str	r4, [sp, #4]
	mov	ip, r3
	movs	r3, #13
	negs	r3, r3
	adds	r2, r3, #0
	mov	r3, ip
	ands	r3, r2
	strb	r3, [r0, #9]
	mov	r2, r8
	ldr	r3, [r5, #64]
	str	r3, [sp, #8]
	ldr	r3, [r5, #64]
	str	r3, [r2, #4]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	str	r1, [r6, #4]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	mov	r2, r8
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #250
	str	r3, [sp, #0]
	movs	r3, #128
	adds	r1, r6, #0
	lsls	r3, r3, #7
	bl	sub_08020018
	ldr	r4, [sp, #4]
.L_080f8a2e:
	adds	r4, #1
	adds	r7, #2
	adds	r5, #4
	cmp	r4, sl
	blt.n	.L_080f89d6
.L_080f8a38:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
