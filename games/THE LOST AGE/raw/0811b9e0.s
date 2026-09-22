.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_0811a31c, 0x0811a31c
	.set sub_0811b598, 0x0811b598
	.set sub_0811bdb0, 0x0811bdb0
	.global Overlay_0811b9e0
Overlay_0811b9e0:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_0811b9fa
	movs	r1, #0
	adds	r0, #40
	movs	r2, #3
.L_0811b9ec:
	ldmia	r0!, {r3}
	cmp	r3, #0
	beq.n	.L_0811b9f4
	str	r1, [r3, #16]
.L_0811b9f4:
	subs	r2, #1
	cmp	r2, #0
	bge.n	.L_0811b9ec
.L_0811b9fa:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #100
	add	r1, sp, #72
	movs	r0, #1
	mov	r8, r1
	mov	r9, r3
	bl	sub_0811a31c
	lsls	r1, r0, #1
	mov	fp, r0
	add	r1, r8
	movs	r0, #2
	bl	sub_0811a31c
	str	r0, [sp, #12]
	movs	r7, #0
	mov	r2, r8
	ldrh	r3, [r2, r7]
	cmp	r3, #255
	beq.n	.L_0811bab2
	add	r3, sp, #16
	movs	r2, #0
	mov	sl, r3
.L_0811ba3c:
	movs	r3, #192
	mov	r1, r8
	lsls	r3, r3, #18
	ldrh	r2, [r1, r2]
	ldr	r1, [r3, #36]
	movs	r6, #0
	adds	r0, r1, #0
	adds	r0, #132
	cmp	r2, #7
	ble.n	.L_0811ba52
	subs	r2, #120
.L_0811ba52:
	adds	r2, #116
	ldrb	r3, [r1, r2]
	movs	r5, #0
	cmp	r3, #255
	beq.n	.L_0811ba64
	ldrb	r3, [r1, r2]
	movs	r2, #44
	muls	r3, r2
	adds	r5, r0, r3
.L_0811ba64:
	cmp	r5, #0
	beq.n	.L_0811ba9a
	adds	r0, r5, #0
	ldr	r6, [r5, #0]
	bl	sub_0811b598
	ldr	r3, [r5, #36]
	cmp	r3, #0
	beq.n	.L_0811ba9a
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0811bdb0
	cmp	r0, #0
	beq.n	.L_0811ba9a
	ldr	r3, [r6, #12]
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L_0811ba8c
	movs	r1, #9
.L_0811ba8c:
	ldr	r2, [r5, #36]
	ldrb	r3, [r2, #6]
	cmp	r3, r1
	beq.n	.L_0811ba9a
	movs	r3, #1
	strb	r1, [r2, #6]
	strb	r3, [r0, #25]
.L_0811ba9a:
	lsls	r3, r7, #2
	mov	r2, sl
	adds	r7, #1
	str	r6, [r2, r3]
	cmp	r7, #13
	bgt.n	.L_0811bab2
	lsls	r3, r7, #1
	adds	r2, r3, #0
	mov	r1, r8
	ldrh	r3, [r1, r2]
	cmp	r3, #255
	bne.n	.L_0811ba3c
.L_0811bab2:
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #102
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811bac2
	b.n	.L_0811bc1c
.L_0811bac2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	movs	r2, #54
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	blt.n	.L_0811bad8
	movs	r1, #2
	movs	r3, #1
	str	r1, [sp, #8]
	b.n	.L_0811bade
.L_0811bad8:
	movs	r2, #1
	str	r2, [sp, #8]
	movs	r3, #2
.L_0811bade:
	movs	r7, #0
	cmp	r7, fp
	bge.n	.L_0811bb78
	mov	r9, r3
	mov	r2, r9
	lsls	r2, r2, #2
	movs	r1, #16
	str	r2, [sp, #4]
	add	r1, sp
	mov	sl, r1
.L_0811baf2:
	lsls	r3, r7, #2
	mov	r1, sl
	ldr	r6, [r1, r3]
	cmp	r6, #0
	beq.n	.L_0811bb72
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_0811bb10
	cmp	r2, #2
	beq.n	.L_0811bb4c
	b.n	.L_0811bb72
.L_0811bb10:
	ldr	r5, [r6, #80]
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	ldr	r2, [sp, #4]
	mov	r1, r8
	orrs	r3, r2
	strb	r3, [r5, #9]
	lsls	r3, r7, #1
	ldrh	r0, [r1, r3]
	bl	sub_08016ca4
	movs	r2, #44
	adds	r2, #255
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #4
	bne.n	.L_0811bb42
	ldrb	r2, [r5, #26]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r5, #26]
	b.n	.L_0811bb72
.L_0811bb42:
	ldrb	r2, [r5, #26]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r5, #26]
	b.n	.L_0811bb72
.L_0811bb4c:
	movs	r2, #13
	mov	r3, r9
	negs	r2, r2
	ldr	r1, [r6, #80]
	mov	ip, r2
	lsls	r6, r3, #2
	movs	r4, #3
.L_0811bb5a:
	ldmia	r1!, {r0}
	cmp	r0, #0
	beq.n	.L_0811bb6c
	adds	r5, r0, #0
	ldrb	r2, [r5, #9]
	mov	r3, ip
	ands	r3, r2
	orrs	r3, r6
	strb	r3, [r5, #9]
.L_0811bb6c:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_0811bb5a
.L_0811bb72:
	adds	r7, #1
	cmp	r7, fp
	blt.n	.L_0811baf2
.L_0811bb78:
	ldr	r3, [sp, #12]
	mov	r7, fp
	adds	r0, r7, r3
	cmp	r7, r0
	bge.n	.L_0811bc1c
	ldr	r2, [sp, #8]
	movs	r1, #16
	mov	r9, r2
	mov	r3, r9
	str	r0, [sp, #0]
	add	r1, sp
	lsls	r3, r3, #2
	mov	sl, r1
	mov	fp, r3
.L_0811bb94:
	lsls	r3, r7, #2
	mov	r1, sl
	ldr	r6, [r1, r3]
	cmp	r6, #0
	beq.n	.L_0811bc14
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_0811bbb2
	cmp	r2, #2
	beq.n	.L_0811bbee
	b.n	.L_0811bc14
.L_0811bbb2:
	ldr	r5, [r6, #80]
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	mov	r2, fp
	orrs	r3, r2
	strb	r3, [r5, #9]
	mov	r1, r8
	lsls	r3, r7, #1
	ldrh	r0, [r1, r3]
	bl	sub_08016ca4
	movs	r2, #44
	adds	r2, #255
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #4
	bne.n	.L_0811bbe4
	ldrb	r2, [r5, #26]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r5, #26]
	b.n	.L_0811bc14
.L_0811bbe4:
	ldrb	r2, [r5, #26]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r5, #26]
	b.n	.L_0811bc14
.L_0811bbee:
	movs	r2, #13
	mov	r3, r9
	negs	r2, r2
	ldr	r1, [r6, #80]
	mov	ip, r2
	lsls	r6, r3, #2
	movs	r4, #3
.L_0811bbfc:
	ldmia	r1!, {r0}
	cmp	r0, #0
	beq.n	.L_0811bc0e
	adds	r5, r0, #0
	ldrb	r2, [r5, #9]
	mov	r3, ip
	ands	r3, r2
	orrs	r3, r6
	strb	r3, [r5, #9]
.L_0811bc0e:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_0811bbfc
.L_0811bc14:
	ldr	r3, [sp, #0]
	adds	r7, #1
	cmp	r7, r3
	blt.n	.L_0811bb94
.L_0811bc1c:
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
