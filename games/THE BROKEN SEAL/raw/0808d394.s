.syntax unified
	.thumb
	.global Overlay_0808d394
Overlay_0808d394:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #140]
	movs	r2, #1
	movs	r1, #0
	negs	r2, r2
	ldr	r7, [r3, #0]
	adds	r4, r0, #0
	movs	r6, #8
	mov	ip, r1
	mov	lr, r2
.L_0808d3a8:
	ldmia	r7!, {r0}
	cmp	r0, #0
	beq.n	.L_0808d404
	cmp	r4, #7
	bgt.n	.L_0808d3d6
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldrh	r2, [r0, #0]
	cmp	r3, lr
	beq.n	.L_0808d404
	movs	r1, #1
	negs	r1, r1
.L_0808d3c0:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, r4
	beq.n	.L_0808d410
	adds	r0, #24
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	ldrh	r2, [r0, #0]
	cmp	r3, r1
	bne.n	.L_0808d3c0
	b.n	.L_0808d404
.L_0808d3d6:
	ldrh	r2, [r0, #0]
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	adds	r1, r2, #0
	cmp	r3, lr
	beq.n	.L_0808d404
	movs	r5, #1
	negs	r5, r5
.L_0808d3e6:
	lsls	r3, r1, #16
	movs	r1, #224
	lsls	r1, r1, #11
	cmp	r3, r1
	ble.n	.L_0808d3f6
	cmp	r6, r4
	beq.n	.L_0808d410
	adds	r6, #1
.L_0808d3f6:
	adds	r0, #24
	ldrh	r2, [r0, #0]
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	adds	r1, r2, #0
	cmp	r3, r5
	bne.n	.L_0808d3e6
.L_0808d404:
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	cmp	r3, #3
	ble.n	.L_0808d3a8
	ldrh	r2, [r0, #0]
.L_0808d410:
	lsls	r3, r2, #16
	movs	r5, #1
	asrs	r3, r3, #16
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_0808d41e
	movs	r0, #0
.L_0808d41e:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
