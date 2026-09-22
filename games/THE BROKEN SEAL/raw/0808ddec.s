.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080044d0, 0x080044d0
	.set sub_080072f0, 0x080072f0
	.set sub_0808ba1c, 0x0808ba1c
	.global Overlay_0808ddec
Overlay_0808ddec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #1
	sub	sp, #4
	negs	r1, r1
	movs	r2, #32
	mov	r9, r0
	str	r1, [sp, #0]
	mov	fp, r2
	bl	sub_0808ba1c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0808def2
	movs	r3, #0
	mov	sl, r3
.L_0808de16:
	cmp	sl, r9
	beq.n	.L_0808dee8
	mov	r0, sl
	bl	sub_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0808dee8
	movs	r1, #89
	adds	r1, r1, r6
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_0808dee8
	ldr	r4, [r6, #12]
	ldr	r1, [r7, #12]
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_0808de48
	ldr	r2, [pc, #196]
	cmp	r3, r2
	ble.n	.L_0808de50
	b.n	.L_0808dee8
.L_0808de48:
	ldr	r2, [pc, #188]
	subs	r3, r1, r4
	cmp	r3, r2
	bgt.n	.L_0808dee8
.L_0808de50:
	ldr	r2, [r6, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_0808de5e
	ldr	r3, [pc, #176]
	adds	r0, r0, r3
.L_0808de5e:
	subs	r2, r4, r1
	asrs	r0, r0, #16
	cmp	r2, #0
	bge.n	.L_0808de6a
	ldr	r1, [pc, #164]
	adds	r2, r2, r1
.L_0808de6a:
	asrs	r1, r2, #16
	ldr	r3, [r7, #16]
	ldr	r2, [r6, #16]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_0808de7a
	ldr	r3, [pc, #148]
	adds	r2, r2, r3
.L_0808de7a:
	asrs	r3, r2, #16
	adds	r2, r0, #0
	muls	r2, r0
	adds	r0, r2, #0
	adds	r2, r1, #0
	muls	r2, r1
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #124]
	bl	sub_080072f0
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_0808deb2
	lsls	r0, r5, #2
	adds	r0, r0, r5
	lsls	r0, r0, #1
	movs	r1, #13
	bl	sub_080022ec
	adds	r5, r0, #0
.L_0808deb2:
	cmp	r5, fp
	bge.n	.L_0808dee8
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #16]
	ldr	r1, [r6, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	sub_080044d0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r5, #11
	ble.n	.L_0808dee2
	ldrh	r3, [r7, #6]
	subs	r3, r0, r3
	lsls	r3, r3, #16
	ldr	r1, [pc, #60]
	asrs	r0, r3, #16
	cmp	r0, r1
	blt.n	.L_0808dee8
	ldr	r2, [pc, #56]
	cmp	r0, r2
	bgt.n	.L_0808dee8
.L_0808dee2:
	mov	r3, sl
	str	r3, [sp, #0]
	mov	fp, r5
.L_0808dee8:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #66
	ble.n	.L_0808de16
.L_0808def2:
	ldr	r0, [sp, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x002fffff
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0xffffd001
	.4byte 0x00002fff
