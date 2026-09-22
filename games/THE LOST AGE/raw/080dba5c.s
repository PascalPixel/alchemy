.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d78, 0x08014d78
	.set sub_080201d0, 0x080201d0
	.set sub_080dbcd8, 0x080dbcd8
	.set sub_080dbd5c, 0x080dbd5c
	.set sub_080dbda8, 0x080dbda8
	.set sub_080dc390, 0x080dc390
	.set sub_080eaf98, 0x080eaf98
	.global Overlay_080dba5c
Overlay_080dba5c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r7, [r3, #0]
	sub	sp, #16
	ldr	r2, [r7, #16]
	ldrh	r3, [r7, #0]
	mov	r9, r2
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L_080dbb2c
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #242
	movs	r5, #128
	adds	r2, r7, r3
	lsls	r5, r5, #2
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_08014d78
	adds	r6, r0, #0
	ldr	r3, [pc, #160]
	movs	r2, #0
	adds	r1, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf738
	.2byte 0xfc84
	.2byte 0x1c29
	adds	r2, r6, #0
	mov	r8, r0
	bl	sub_080142d4
	movs	r2, #254
	lsls	r2, r2, #3
	adds	r3, r7, r2
	movs	r2, #0
	mov	sl, r2
	mov	r2, r8
	adds	r5, r0, #0
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08013164
	movs	r3, #249
	lsls	r3, r3, #3
	adds	r6, r7, r3
	movs	r3, #128
	lsls	r3, r3, #24
	adds	r0, r6, #0
	movs	r1, #16
	movs	r2, #31
	str	r5, [sp, #0]
	bl	sub_080eaf98
	ldrb	r3, [r6, #9]
	mov	r2, sl
	strh	r2, [r6, #30]
	movs	r2, #13
	ldrb	r1, [r6, #5]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r2, r3
	movs	r3, #128
	lsls	r3, r3, #10
	strb	r2, [r6, #9]
	str	r3, [r6, #24]
	ldr	r3, [r7, #4]
	add	r5, sp, #4
	str	r3, [r5, #0]
	mov	r2, r9
	ldr	r3, [r2, #12]
	movs	r2, #224
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #4]
	mov	r2, r9
	ldr	r3, [r2, #16]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	movs	r1, #144
	str	r3, [r6, #12]
	ldr	r0, [pc, #24]
	ldr	r3, [r5, #8]
	lsls	r1, r1, #3
	str	r3, [r6, #16]
	bl	sub_080145a8
.L_080dbb2c:
	add	sp, #16
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x03000260
	.2byte 0xba45
	.2byte 0x080d
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r1, #242
	adds	r2, r5, r1
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_080dbb72
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r0, [pc, #20]
	bl	sub_08014644
	movs	r2, #254
	lsls	r2, r2, #3
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08014274
.L_080dbb72:
	pop	{r5, pc}
	.2byte 0xba45
	.2byte 0x080d
.L_080dbb78:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r4, r2, #0
	ldr	r2, [r3, #108]
	movs	r6, #197
	lsls	r6, r6, #1
	ldr	r5, [r3, #32]
	adds	r3, r2, r6
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080dbbbe
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_080dbb9e
	ldr	r2, [pc, #88]
	adds	r3, r0, r2
.L_080dbb9e:
	asrs	r2, r3, #21
	movs	r0, #31
	ands	r2, r0
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_080dbbae
	ldr	r4, [pc, #72]
	adds	r3, r1, r4
.L_080dbbae:
	asrs	r3, r3, #21
	ands	r3, r0
	lsls	r3, r3, #5
	ldr	r6, [pc, #64]
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r2, r3, r6
	b.n	.L_080dbbf0
.L_080dbbbe:
	cmp	r4, #2
	bgt.n	.L_080dbbd2
	lsls	r3, r4, #3
	subs	r3, r3, r4
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [r5, r3]
	b.n	.L_080dbbd4
.L_080dbbd2:
	ldr	r2, [pc, #40]
.L_080dbbd4:
	cmp	r0, #0
	bge.n	.L_080dbbdc
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_080dbbdc:
	asrs	r0, r0, #20
	cmp	r1, #0
	bge.n	.L_080dbbe6
	ldr	r4, [pc, #28]
	adds	r1, r1, r4
.L_080dbbe6:
	asrs	r3, r1, #20
	lsls	r3, r3, #7
	adds	r3, r0, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
.L_080dbbf0:
	adds	r0, r2, #0
	pop	{r5, r6, pc}
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x02010000
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r5, r0, #0
	mov	r8, r3
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r6, r1, #0
	adds	r7, r2, #0
	cmp	r3, #3
	bne.n	.L_080dbc2e
	bl	sub_080dbcd8
	b.n	.L_080dbca2
.L_080dbc2e:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080dbda8
	cmp	r0, #0
	bne.n	.L_080dbc58
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080dbd5c
	cmp	r0, #0
	bne.n	.L_080dbc58
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080201d0
	cmp	r0, #0
	beq.n	.L_080dbc5c
.L_080dbc58:
	movs	r0, #0
	b.n	.L_080dbca2
.L_080dbc5c:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080dbcd8
	cmp	r0, #0
	beq.n	.L_080dbc6e
	movs	r0, #1
	b.n	.L_080dbca2
.L_080dbc6e:
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #54
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080dbc92
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	.L_080dbb78
	ldrb	r3, [r0, #3]
	movs	r0, #1
	ands	r0, r3
	b.n	.L_080dbca2
.L_080dbc92:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	.L_080dbb78
	ldrb	r3, [r0, #3]
	movs	r0, #1
	bics	r0, r3
.L_080dbca2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r2, #2
	bl	.L_080dbb78
	ldrb	r3, [r0, #2]
	movs	r2, #0
	cmp	r3, #231
	bne.n	.L_080dbcba
	movs	r2, #1
.L_080dbcba:
	adds	r0, r2, #0
	pop	{pc}
