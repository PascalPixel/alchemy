.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.global Overlay_0802cfa4
Overlay_0802cfa4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #112]
	movs	r2, #3
	mov	sl, r3
	adds	r3, #176
	ldrh	r3, [r3, #0]
	movs	r1, #0
	mov	lr, r1
	ands	r2, r3
	sub	sp, #32
	cmp	lr, r2
	bcs.n	.L_0802d07c
	mov	r9, r2
.L_0802cfca:
	movs	r3, #44
	mov	r2, lr
	muls	r2, r3
	mov	r1, sl
	adds	r3, r2, #0
	adds	r5, r1, r3
	ldrh	r2, [r5, #6]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_0802d064
	movs	r2, #4
	ldrsh	r7, [r5, r2]
	movs	r3, #10
	ldrsh	r2, [r5, r3]
	ldr	r1, [r5, #0]
	subs	r3, r2, r7
	lsls	r3, r3, #24
	lsls	r6, r2, #16
	adds	r4, r5, #0
	lsrs	r0, r3, #24
	lsrs	r3, r6, #16
	mov	r8, r1
	adds	r4, #12
	cmp	r0, r3
	bcs.n	.L_0802d012
	mov	r1, sp
	mov	ip, r3
.L_0802d000:
	ldrh	r3, [r4, #0]
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
	adds	r3, r0, #1
	lsls	r3, r3, #24
	lsrs	r0, r3, #24
	adds	r4, #2
	cmp	r0, ip
	bcc.n	.L_0802d000
.L_0802d012:
	lsls	r1, r7, #16
	adds	r7, r1, #0
	lsrs	r2, r6, #16
	lsrs	r3, r7, #16
	movs	r0, #0
	subs	r2, r2, r3
	cmp	r0, r2
	bge.n	.L_0802d038
	mov	r1, sp
	mov	ip, r2
.L_0802d026:
	ldrh	r3, [r4, #0]
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
	adds	r3, r0, #1
	lsls	r3, r3, #24
	lsrs	r0, r3, #24
	adds	r4, #2
	cmp	r0, ip
	blt.n	.L_0802d026
.L_0802d038:
	movs	r2, #128
	movs	r3, #128
	lsrs	r4, r6, #16
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r0, sp
	mov	r1, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r1, r7, r2
	lsrs	r3, r1, #16
	cmp	r3, r4
	bcc.n	.L_0802d05c
	movs	r1, #0
.L_0802d05c:
	lsrs	r3, r1, #16
	strh	r3, [r5, #4]
	ldrh	r3, [r5, #8]
	b.n	.L_0802d06c
.L_0802d064:
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r2, r1
.L_0802d06c:
	strh	r3, [r5, #6]
	mov	r3, lr
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	lr, r3
	cmp	lr, r9
	bcc.n	.L_0802cfca
.L_0802d07c:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	adds	r6, r1, #0
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #115
	adds	r2, r5, r1
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, r0
	beq.n	.L_0802d0de
	strb	r0, [r2, #0]
	cmp	r0, #9
	bne.n	.L_0802d0b4
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #117
	adds	r2, r5, r3
	movs	r3, #12
	b.n	.L_0802d0dc
.L_0802d0b4:
	cmp	r0, #18
	bne.n	.L_0802d0d2
	movs	r0, #254
	lsls	r0, r0, #3
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_0802d0d2
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #117
	adds	r2, r5, r0
	movs	r3, #250
	b.n	.L_0802d0dc
.L_0802d0d2:
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #117
	adds	r2, r5, r1
	movs	r3, #0
.L_0802d0dc:
	strb	r3, [r2, #0]
.L_0802d0de:
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0802d0ec
	b.n	.L_0802d22e
.L_0802d0ec:
	cmp	r6, #0
	bne.n	.L_0802d106
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #117
	adds	r3, r5, r2
	movs	r0, #144
	ldrb	r2, [r3, #0]
	lsls	r0, r0, #4
	adds	r0, #116
	adds	r3, r5, r0
	strb	r2, [r3, #0]
	b.n	.L_0802d150
.L_0802d106:
	movs	r1, #144
	lsls	r1, r1, #4
	movs	r0, #144
	adds	r1, #117
	lsls	r0, r0, #4
	adds	r3, r5, r1
	adds	r0, #116
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	subs	r0, r2, r3
	cmp	r0, #0
	bne.n	.L_0802d128
	b.n	.L_0802d22e
.L_0802d128:
	ldr	r3, [pc, #260]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802d136
	b.n	.L_0802d22e
.L_0802d136:
	cmp	r0, #0
	bge.n	.L_0802d140
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0802d142
.L_0802d140:
	movs	r0, #1
.L_0802d142:
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #116
	adds	r2, r5, r1
	ldrb	r3, [r2, #0]
	adds	r3, r3, r0
	strb	r3, [r2, #0]
.L_0802d150:
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #116
	adds	r6, r5, r2
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	ble.n	.L_0802d1b8
	ldr	r0, [pc, #208]
	ldr	r1, [pc, #212]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_0802d194
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, r3, r0
	lsls	r2, r2, #6
	adds	r3, #4
	adds	r2, #142
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0802d194:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_0802d22c
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	lsls	r2, r2, #2
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	adds	r2, r2, r0
	lsls	r3, r3, #16
	adds	r2, #4
	b.n	.L_0802d21a
.L_0802d1b8:
	ldr	r0, [pc, #120]
	ldr	r1, [pc, #124]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_0802d1ec
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, r3, r0
	lsls	r2, r2, #6
	adds	r3, #4
	adds	r2, #206
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0802d1ec:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_0802d22c
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r3, #1
	strh	r3, [r0, #0]
	adds	r2, r2, r0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #116
	adds	r3, r5, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r2, #4
	negs	r3, r3
	lsls	r3, r3, #16
.L_0802d21a:
	lsrs	r3, r3, #16
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #84
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_0802d22c:
	strh	r4, [r1, #0]
.L_0802d22e:
	pop	{r5, r6, pc}
	.4byte 0x0300122c
	.4byte 0x020038e0
	.4byte 0x04000208
