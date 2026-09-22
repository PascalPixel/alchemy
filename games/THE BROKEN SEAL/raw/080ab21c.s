.syntax unified
	.thumb
	.global Overlay_080ab21c
Overlay_080ab21c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r3, #0
	ldr	r3, [pc, #176]
	adds	r4, r0, #0
	adds	r0, r1, #0
	ldr	r1, [sp, #36]
	ldr	r3, [r3, #0]
	mov	sl, r1
	mov	r9, r3
	mov	r3, sl
	lsls	r3, r3, #12
	adds	r7, r2, #0
	mov	sl, r3
	cmp	r4, #0
	bge.n	.L_080ab24c
	adds	r7, r7, r4
	movs	r4, #0
.L_080ab24c:
	adds	r3, r4, r7
	cmp	r3, #29
	ble.n	.L_080ab256
	movs	r3, #30
	subs	r7, r3, r4
.L_080ab256:
	cmp	r0, #0
	bge.n	.L_080ab25e
	adds	r5, r5, r0
	movs	r0, #0
.L_080ab25e:
	adds	r3, r0, r5
	cmp	r3, #29
	ble.n	.L_080ab268
	movs	r3, #20
	subs	r5, r3, r0
.L_080ab268:
	cmp	r7, #0
	ble.n	.L_080ab2ce
	cmp	r5, #0
	ble.n	.L_080ab2ce
	ldr	r6, [pc, #112]
	lsls	r2, r0, #6
	lsls	r3, r4, #1
	adds	r2, r2, r3
	add	r6, r9
	movs	r1, #2
	str	r2, [sp, #0]
	mov	r8, r6
	mov	fp, r1
.L_080ab282:
	ldr	r4, [sp, #0]
	adds	r1, r7, #0
	add	r4, r9
	cmp	r1, #0
	beq.n	.L_080ab2b2
	ldr	r6, [pc, #88]
	movs	r3, #15
	mov	lr, r3
	mov	ip, r6
.L_080ab294:
	ldrh	r2, [r4, #0]
	mov	r6, lr
	lsrs	r3, r2, #12
	ands	r3, r6
	cmp	r3, #15
	bne.n	.L_080ab2aa
	mov	r3, ip
	ands	r2, r3
	mov	r6, sl
	orrs	r2, r6
	strh	r2, [r4, #0]
.L_080ab2aa:
	subs	r1, #1
	adds	r4, #2
	cmp	r1, #0
	bne.n	.L_080ab294
.L_080ab2b2:
	lsrs	r3, r0, #2
	mov	r1, r8
	mov	r2, fp
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
	subs	r5, #1
	ldr	r3, [sp, #0]
	adds	r3, #64
	str	r3, [sp, #0]
	adds	r0, #1
	cmp	r5, #0
	bne.n	.L_080ab282
.L_080ab2ce:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0xffff0fff
