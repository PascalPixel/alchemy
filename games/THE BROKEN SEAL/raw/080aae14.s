.syntax unified
	.thumb
	.global Overlay_080aae14
Overlay_080aae14:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	mov	r8, r1
	movs	r1, #0
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	str	r1, [sp, #0]
	mov	r2, r8
	ldrh	r3, [r2, #0]
	mov	ip, r0
	mov	sl, r1
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_080aaeac
	ldr	r3, [pc, #88]
	ldr	r5, [sp, #8]
	mov	lr, r3
	mov	r0, r8
	subs	r5, #2
.L_080aae46:
	ldrh	r2, [r0, #0]
	mov	r3, lr
	ands	r3, r2
	strh	r3, [r5, #2]
	movs	r1, #1
	add	sl, r1
	mov	r1, ip
	ldrh	r2, [r1, #0]
	ldrh	r3, [r0, #0]
	eors	r3, r2
	mov	r2, lr
	ands	r3, r2
	adds	r5, #2
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080aae7e
	ldr	r7, [pc, #40]
	adds	r6, r0, #0
.L_080aae6a:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_080aae7e
	adds	r1, #4
	ldrh	r3, [r6, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_080aae6a
.L_080aae7e:
	cmp	r4, #32
	bne.n	.L_080aae9c
	movs	r3, #1
	add	fp, r3
	ldr	r2, [pc, #12]
	ldrh	r3, [r5, #0]
	orrs	r3, r2
	strh	r3, [r5, #0]
	b.n	.L_080aae9c
	.4byte 0x00003fff
	.4byte 0x00008000
	.2byte 0x3fff
	.2byte 0x0000
.L_080aae9c:
	mov	r3, r8
	adds	r0, #4
	adds	r3, #124
	cmp	r0, r3
	bgt.n	.L_080aaeac
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_080aae46
.L_080aaeac:
	mov	r2, ip
	ldrh	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_080aaf38
	mov	r1, sl
	ldr	r2, [sp, #8]
	lsls	r3, r1, #1
	mov	lr, ip
	adds	r0, r3, r2
	movs	r7, #0
.L_080aaec4:
	mov	r1, ip
	ldrh	r3, [r7, r1]
	mov	r1, r8
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ldr	r2, [pc, #56]
	ands	r3, r2
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_080aaef0
	ldr	r6, [pc, #40]
	mov	r5, lr
.L_080aaedc:
	adds	r4, #1
	cmp	r4, #31
	bgt.n	.L_080aaef0
	adds	r1, #4
	ldrh	r3, [r5, #0]
	ldrh	r2, [r1, #0]
	eors	r3, r2
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080aaedc
.L_080aaef0:
	cmp	r4, #32
	bne.n	.L_080aaf1a
	ldr	r3, [sp, #0]
	adds	r3, #1
	str	r3, [sp, #0]
	mov	r1, ip
	ldrh	r3, [r7, r1]
	ldr	r2, [pc, #8]
	b.n	.L_080aaf0c
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x3fff
	.2byte 0x0000
.L_080aaf0c:
	ands	r2, r3
	ldr	r3, [pc, #36]
	orrs	r2, r3
	strh	r2, [r0, #0]
	movs	r2, #1
	adds	r0, #2
	add	sl, r2
.L_080aaf1a:
	movs	r1, #1
	add	r9, r1
	movs	r3, #4
	mov	r2, r9
	adds	r7, #4
	add	lr, r3
	cmp	r2, #31
	bgt.n	.L_080aaf38
	mov	r1, ip
	ldrh	r3, [r7, r1]
	cmp	r3, #0
	bne.n	.L_080aaec4
	b.n	.L_080aaf38
	.2byte 0x4000
	.2byte 0x0000
.L_080aaf38:
	ldr	r3, [sp, #4]
	mov	r2, fp
	str	r2, [r3, #0]
	ldr	r1, [sp, #0]
	ldr	r3, [sp, #44]
	mov	r0, sl
	str	r1, [r3, #0]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
