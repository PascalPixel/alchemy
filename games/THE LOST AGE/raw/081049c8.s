.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08108088, 0x08108088
	.global Overlay_081049c8
Overlay_081049c8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #0
	mov	sl, r3
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	mov	r8, r2
	mov	fp, r0
	cmp	r8, r3
	bge.n	.L_08104a4c
	movs	r7, #129
	movs	r3, #1
	lsls	r7, r7, #2
	mov	r9, r3
	add	r7, sl
.L_081049fe:
	ldrh	r0, [r7, #0]
	bl	sub_08016ca4
	movs	r6, #0
	movs	r4, #0
	adds	r0, #248
.L_08104a0a:
	ldr	r5, [r0, #16]
	movs	r1, #0
.L_08104a0e:
	mov	r2, r9
	lsls	r2, r1
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08104a22
	ldr	r3, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08104a24
.L_08104a22:
	adds	r4, #1
.L_08104a24:
	adds	r1, #1
	cmp	r1, #19
	ble.n	.L_08104a0e
	adds	r6, #1
	adds	r0, #4
	cmp	r6, #3
	ble.n	.L_08104a0a
	mov	r3, r8
	mov	r2, fp
	strb	r4, [r2, r3]
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	movs	r2, #1
	add	r8, r2
	adds	r7, #2
	cmp	r8, r3
	blt.n	.L_081049fe
.L_08104a4c:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	bl	sub_080143ac
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #238
	movs	r6, #1
	adds	r3, r5, r2
	negs	r6, r6
	strh	r0, [r3, #0]
	cmp	r0, r6
	beq.n	.L_08104a80
	ldr	r2, [pc, #32]
	movs	r1, #128
	bl	sub_08108088
.L_08104a80:
	bl	sub_080143ac
	movs	r2, #158
	lsls	r2, r2, #3
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	cmp	r0, r6
	beq.n	.L_08104a98
	ldr	r2, [pc, #12]
	movs	r1, #128
	bl	sub_08108088
.L_08104a98:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x00000200
	.2byte 0x0201
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #238
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	sub_08014274
	movs	r3, #158
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrh	r0, [r5, #0]
	bl	sub_08014274
	pop	{r5, pc}
