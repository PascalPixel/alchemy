.syntax unified
	.thumb
	.set sub_0808a8d0, 0x0808a8d0
	.global Overlay_0808b158
Overlay_0808b158:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	adds	r6, r1, #0
	ldr	r5, [pc, #108]
	mov	r8, r3
	adds	r7, r0, #0
	bl	sub_0808a8d0
	ldrh	r4, [r5, #0]
	adds	r1, r4, #0
	lsls	r3, r1, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	mov	ip, r0
	cmp	r3, r2
	beq.n	.L_0808b1c4
	ldr	r0, [pc, #84]
	mov	lr, r2
.L_0808b182:
	ldrb	r2, [r5, #3]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0808b196
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r7
	bne.n	.L_0808b1b6
	b.n	.L_0808b19e
.L_0808b196:
	lsls	r3, r4, #16
	asrs	r3, r3, #16
	cmp	r3, ip
	bne.n	.L_0808b1b6
.L_0808b19e:
	ldrh	r2, [r5, #2]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, r0
	beq.n	.L_0808b1b0
	lsls	r3, r2, #17
	asrs	r3, r3, #17
	cmp	r3, r6
	bne.n	.L_0808b1b6
.L_0808b1b0:
	ldr	r5, [r5, #4]
	mov	r8, r5
	b.n	.L_0808b1c4
.L_0808b1b6:
	adds	r5, #8
	ldrh	r1, [r5, #0]
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	adds	r4, r1, #0
	cmp	r3, lr
	bne.n	.L_0808b182
.L_0808b1c4:
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0809ddd8
	.4byte 0x00007fff
