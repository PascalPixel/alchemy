.syntax unified
	.thumb
	.set sub_08038288, 0x08038288
	.set sub_080facd8, 0x080facd8
	.global Overlay_080fad88
Overlay_080fad88:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	ldr	r2, [pc, #28]
	adds	r3, r5, #0
	adds	r3, #62
	mov	ip, r5
.L_080fad94:
	strh	r2, [r3, #0]
	subs	r3, #2
	cmp	r3, ip
	bge.n	.L_080fad94
	ldr	r3, [pc, #12]
	movs	r7, #0
	mov	ip, r3
	adds	r0, #216
	movs	r6, #0
	adds	r4, r5, #0
	movs	r1, #14
	b.n	.L_080fadb0
	.2byte 0x0000
	.2byte 0x0000
.L_080fadb0:
	mov	r3, ip
	strh	r3, [r6, r5]
	ldrh	r2, [r0, #0]
	adds	r0, #2
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fadc4
	strh	r2, [r4, #0]
	adds	r7, #1
	adds	r4, #2
.L_080fadc4:
	subs	r1, #1
	adds	r6, #2
	cmp	r1, #0
	bge.n	.L_080fadb0
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	sl, r0
	adds	r5, r3, #0
	mov	r8, r1
	adds	r5, #76
	mov	r6, sl
	movs	r7, #14
.L_080fadec:
	ldrh	r1, [r6, #0]
	adds	r6, #2
	cmp	r1, #0
	beq.n	.L_080fae14
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080fae08
	ldr	r3, [r5, #0]
	movs	r0, #2
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	sub_08038288
	b.n	.L_080fae14
.L_080fae08:
	ldr	r3, [r5, #0]
	movs	r0, #7
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	sub_08038288
.L_080fae14:
	subs	r7, #1
	adds	r5, #4
	cmp	r7, #0
	bge.n	.L_080fadec
	mov	r0, sl
	bl	sub_080facd8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
