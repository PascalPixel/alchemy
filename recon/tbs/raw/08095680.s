.syntax unified
	.thumb
	.set sub_08009080, 0x08009080
	.set sub_080090f0, 0x080090f0
	.set sub_080770c0, 0x080770c0
	.set sub_0808ba1c, 0x0808ba1c
	.set sub_0808d394, 0x0808d394
	.global Unnamed_08095680
	.global Func_08095680
	.thumb_func
Unnamed_08095680:
Func_08095680:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r5, [pc, #208]
	movs	r0, #141
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r7, r3
	ldr	r3, [pc, #192]
	mov	sl, r3
	mov	r0, sl
	ands	r0, r2
	mov	sl, r0
	ldr	r0, [pc, #184]
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_0809574c
	cmp	r7, #0
	bne.n	.L_0809574c
	ldr	r3, [pc, #176]
	movs	r7, #128
	mov	r1, sl
	lsls	r7, r7, #4
	ands	r7, r1
	ands	r1, r3
	ldr	r3, [pc, #168]
	mov	sl, r1
	add	r3, sl
	cmp	r3, #80
	bhi.n	.L_0809574c
	ldr	r2, [pc, #160]
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	ble.n	.L_0809574c
	subs	r2, #66
	movs	r1, #8
	adds	r2, r2, r5
	mov	r8, r1
	mov	r9, r2
.L_080956e2:
	mov	r0, r8
	bl	sub_0808d394
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08095742
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	ldr	r2, [pc, #120]
	subs	r3, #48
	add	r2, sl
	cmp	r3, r2
	bne.n	.L_08095742
	mov	r0, r8
	bl	sub_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08095742
	cmp	r7, #0
	bne.n	.L_08095722
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #3
	str	r7, [r6, #20]
	strb	r3, [r2, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl	sub_080090f0
	b.n	.L_0809573a
.L_08095722:
	mov	r1, r9
	ldr	r0, [r1, #0]
	bl	sub_0808ba1c
	ldr	r3, [r0, #16]
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r0, [pc, #64]
	adds	r3, r3, r0
	adds	r0, r6, #0
	bl	sub_080090f0
.L_0809573a:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08009080
.L_08095742:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #65
	ble.n	.L_080956e2
.L_0809574c:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000fff
	.4byte 0x00000109
	.4byte 0x000007ff
	.4byte 0xfffffed4
	.4byte 0x00000236
	.4byte 0xffe00000
