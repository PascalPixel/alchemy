.syntax unified
	.thumb
	.set sub_0800205c, 0x0800205c
	.set sub_0800206c, 0x0800206c
	.set sub_080c9f2c, 0x080c9f2c
	.set sub_080ca1a4, 0x080ca1a4
	.set sub_080ca5d8, 0x080ca5d8
	.set sub_080cad84, 0x080cad84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d7524, 0x080d7524
	.global Overlay_080d2b4c
Overlay_080d2b4c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r1, #0
	ldr	r3, [r3, #108]
	movs	r1, #128
	lsls	r1, r1, #4
	mov	r8, r1
	adds	r7, r0, #0
	mov	sl, r3
	mov	r3, r8
	ands	r3, r7
	movs	r1, #20
	mov	r8, r3
	adds	r0, r6, #0
	movs	r3, #255
	ands	r7, r3
	bl	sub_0800205c
	movs	r1, #20
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_0800206c
	cmp	r0, #19
	bne.n	.L_080d2b98
	movs	r1, #200
	ldr	r2, [pc, #112]
	lsls	r1, r1, #5
	adds	r1, #80
	adds	r3, r5, r1
	ldrsb	r0, [r2, r3]
	cmp	r0, #0
	beq.n	.L_080d2bf4
	subs	r0, #1
.L_080d2b98:
	lsls	r3, r5, #2
	adds	r3, r3, r5
	lsls	r3, r3, #2
	adds	r6, r3, r0
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080d2bac
	adds	r0, r7, #0
	bl	sub_080d7524
.L_080d2bac:
	movs	r1, #150
	lsls	r1, r1, #1
	ldr	r2, [pc, #76]
	adds	r3, r6, r1
	mov	r1, r8
	orrs	r3, r1
	movs	r1, #149
	lsls	r1, r1, #2
	adds	r2, r2, r1
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	bl	sub_080ca1a4
	movs	r3, #178
	lsls	r3, r3, #1
	add	r3, sl
	strh	r0, [r3, #0]
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d2bec
	bl	sub_080cdf5c
	bl	sub_080cad84
	adds	r0, #8
	bl	sub_080c9f2c
.L_080d2bec:
	movs	r0, #0
	movs	r1, #0
	bl	sub_080ca5d8
.L_080d2bf4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02001000
	.4byte 0x02000240
