.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad000, 0x080ad000
	.set sub_080ad008, 0x080ad008
	.set sub_080ad170, 0x080ad170
	.set sub_080ad1b8, 0x080ad1b8
	.set sub_0811a038, 0x0811a038
	.set sub_0811a0b0, 0x0811a0b0
	.set sub_0811a31c, 0x0811a31c
	.global Overlay_08118d70
Overlay_08118d70:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #20
	mov	r8, sp
	mov	r0, r8
	bl	sub_0811a038
	adds	r7, r0, #0
	lsls	r0, r7, #1
	add	r0, r8
	bl	sub_0811a0b0
	adds	r7, r7, r0
	movs	r0, #0
	mov	sl, r0
	cmp	sl, r7
	bge.n	.L_08118e58
	movs	r1, #0
	movs	r5, #0
	movs	r6, #0
	mov	r9, r1
.L_08118da0:
	mov	r3, r8
	ldrh	r0, [r6, r3]
	bl	sub_08016ca4
	adds	r2, r0, #0
	movs	r0, #48
	adds	r0, #255
	movs	r1, #3
	adds	r3, r2, r0
.L_08118db2:
	mov	r0, r9
	subs	r1, #1
	strb	r0, [r3, #0]
	subs	r3, #1
	cmp	r1, #0
	bge.n	.L_08118db2
	movs	r1, #153
	lsls	r1, r1, #1
	movs	r0, #52
	adds	r3, r2, r1
	adds	r0, #255
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #3
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #3
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	adds	r1, #2
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	adds	r0, #2
	adds	r3, r2, r1
	strb	r5, [r3, #0]
	mov	r1, r8
	adds	r3, r2, r0
	strb	r5, [r3, #0]
	ldrh	r0, [r6, r1]
	bl	sub_080ad008
	movs	r3, #1
	add	sl, r3
	adds	r6, #2
	cmp	sl, r7
	blt.n	.L_08118da0
.L_08118e58:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	sub	sp, #32
	mov	r6, sp
	adds	r0, r6, #0
	bl	sub_0811a038
	adds	r5, r0, #0
	lsls	r0, r5, #1
	adds	r0, r6, r0
	bl	sub_0811a0b0
	adds	r5, r5, r0
	lsls	r1, r5, #1
	adds	r1, r6, r1
	movs	r0, #2
	bl	sub_0811a31c
	adds	r5, r5, r0
	cmp	r5, #0
	ble.n	.L_08118eac
	movs	r7, #0
.L_08118e8e:
	ldrh	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r1, #44
	adds	r2, r0, #0
	adds	r1, #255
	adds	r3, r2, r1
	adds	r1, #26
	strb	r7, [r3, #0]
	subs	r5, #1
	adds	r3, r2, r1
	adds	r6, #2
	strb	r7, [r3, #0]
	cmp	r5, #0
	bne.n	.L_08118e8e
.L_08118eac:
	add	sp, #32
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #20
	mov	r5, sp
	adds	r0, r5, #0
	bl	sub_0811a038
	movs	r2, #0
	mov	sl, r0
	mov	r8, r2
	cmp	r8, sl
	bge.n	.L_08118f5e
	mov	r9, r5
.L_08118ed0:
	mov	r3, r9
	ldrh	r7, [r3, #0]
	movs	r6, #0
.L_08118ed6:
	movs	r5, #0
.L_08118ed8:
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_080ad1b8
	cmp	r0, #0
	beq.n	.L_08118f46
	movs	r0, #0
	cmp	r7, #7
	bls.n	.L_08118eee
	movs	r0, #1
.L_08118eee:
	bl	sub_080ad000
	movs	r2, #148
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r1, r3, #0
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r0, #0
	adds	r1, #8
	cmp	r0, r3
	bge.n	.L_08118f30
	ldrb	r3, [r1, #0]
	cmp	r6, r3
	bne.n	.L_08118f12
	ldrb	r3, [r1, #1]
	cmp	r5, r3
	beq.n	.L_08118f30
.L_08118f12:
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	adds	r0, #1
	cmp	r0, r3
	bge.n	.L_08118f30
	lsls	r2, r0, #2
	ldrb	r3, [r1, r2]
	cmp	r6, r3
	bne.n	.L_08118f12
	adds	r3, r1, r2
	ldrb	r3, [r3, #1]
	cmp	r5, r3
	bne.n	.L_08118f12
.L_08118f30:
	movs	r2, #144
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	cmp	r0, r3
	bne.n	.L_08118f46
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_080ad170
.L_08118f46:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_08118ed8
	adds	r6, #1
	cmp	r6, #3
	ble.n	.L_08118ed6
	movs	r2, #1
	movs	r3, #2
	add	r8, r2
	add	r9, r3
	cmp	r8, sl
	blt.n	.L_08118ed0
.L_08118f5e:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
