.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_0801401c, 0x0801401c
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014d00, 0x08014d00
	.set sub_0801591c, 0x0801591c
	.set sub_08016ce4, 0x08016ce4
	.set sub_080202d0, 0x080202d0
	.set sub_0803c378, 0x0803c378
	.set sub_08042314, 0x08042314
	.global Overlay_08045018
Overlay_08045018:
	push	{r5, lr}
	ldr	r4, [pc, #36]
	ldr	r5, [pc, #36]
	ldr	r3, [r4, #0]
	movs	r1, #7
	lsrs	r3, r3, #2
	ands	r3, r1
	ldrb	r2, [r0, #8]
	ldrb	r3, [r5, r3]
	adds	r2, r2, r3
	ldr	r3, [r4, #0]
	strb	r2, [r0, #20]
	ldr	r0, [r0, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	ldrb	r2, [r0, #8]
	ldrb	r3, [r5, r3]
	adds	r2, r2, r3
	strb	r2, [r0, #20]
	pop	{r5, pc}
	.4byte 0x03001100
	.2byte 0xf6d6
	.2byte 0x0805
.L_08045048:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #128
	lsls	r3, r3, #3
	mov	sl, r3
	adds	r6, r0, #0
	adds	r7, r1, #0
	movs	r0, #56
	mov	r1, sl
	mov	r8, r2
	bl	sub_08014d00
	adds	r5, r0, #0
	ldr	r0, [pc, #64]
	bl	sub_08013300
	adds	r2, r0, #0
	cmp	r7, #95
	bgt.n	.L_0804509e
	lsls	r3, r6, #1
	ldrh	r0, [r3, r2]
	adds	r1, r5, #0
	adds	r0, r2, r0
	bl	sub_0801591c
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0804508e
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r0, r5, #0
	bl	sub_080202d0
.L_0804508e:
	adds	r0, r7, #0
	mov	r1, sl
	adds	r2, r5, #0
	bl	sub_080142d4
	movs	r0, #56
	bl	sub_0801314c
.L_0804509e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x01d7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	sub_080143ac
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #96
	beq.n	.L_080450f2
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	.L_08045048
	ldr	r3, [sp, #28]
	movs	r1, #128
	str	r3, [sp, #0]
	mov	r2, r8
	mov	r3, sl
	lsls	r1, r1, #24
	adds	r0, r5, #0
	bl	sub_08042314
	ldrb	r3, [r0, #21]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r0, #21]
	movs	r3, #251
	strb	r3, [r0, #15]
.L_080450f2:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	ldr	r3, [pc, #136]
	ldr	r1, [pc, #136]
	ldr	r3, [r3, #0]
	movs	r2, #7
	lsrs	r3, r3, #1
	ands	r3, r2
	lsls	r3, r3, #2
	ldr	r1, [r1, r3]
	sub	sp, #8
	adds	r5, r0, #0
	cmp	r1, #0
	bge.n	.L_08045118
	adds	r1, #255
.L_08045118:
	asrs	r1, r1, #8
	cmp	r5, #0
	beq.n	.L_08045198
	ldr	r3, [sp, #0]
	ldr	r4, [pc, #108]
	lsls	r1, r1, #16
	movs	r2, #255
	lsrs	r1, r1, #16
	ands	r3, r4
	lsls	r2, r2, #8
	adds	r2, #255
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #0]
	mov	r0, sp
	ldr	r3, [r0, #4]
	ands	r3, r4
	str	r3, [r0, #4]
	bl	sub_0801401c
	ldrb	r2, [r5, #23]
	movs	r3, #31
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r5, #23]
	ldrb	r3, [r5, #21]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #21]
	ldrh	r2, [r5, #6]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #240
	adds	r2, r2, r3
	ldr	r3, [pc, #24]
	ldrh	r1, [r5, #22]
	ands	r2, r3
	ldr	r3, [pc, #36]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #22]
	ldrb	r3, [r5, #8]
	adds	r3, #240
	strb	r3, [r5, #20]
	movs	r3, #252
	strb	r3, [r5, #15]
	b.n	.L_08045198
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001100
	.4byte 0x0805f6e0
	.4byte 0xffff0000
	.2byte 0xfe00
	.2byte 0xffff
.L_08045198:
	.2byte 0xb002
	pop	{r5, pc}
	push	{lr}
	movs	r0, #118
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080451ae
	ldr	r0, [pc, #8]
	b.n	.L_080451b0
.L_080451ae:
	ldr	r0, [pc, #8]
.L_080451b0:
	pop	{pc}
	movs	r0, r0
	.4byte 0x00003007
	.2byte 0x2fd2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #116]
	ldr	r3, [pc, #120]
	movs	r7, #0
	mov	r8, r2
	mov	sl, r3
.L_080451ce:
	lsls	r3, r7, #1
	ldr	r2, [pc, #112]
	adds	r3, r3, r7
	lsls	r3, r3, #7
	movs	r6, #0
	adds	r5, r3, r2
.L_080451da:
	adds	r0, r5, #0
	movs	r1, #64
	ldr	r2, [pc, #104]
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2401
	adds	r0, r5, #4
.L_080451e8:
	adds	r1, r6, #0
	cmp	r7, #1
	bne.n	.L_080451f2
	cmp	r4, #1
	ble.n	.L_0804521c
.L_080451f2:
	cmp	r7, #0
	bne.n	.L_08045204
	subs	r3, r4, #2
	cmp	r6, r3
	ble.n	.L_08045204
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L_08045204
	movs	r1, #0
.L_08045204:
	lsls	r1, r1, #3
	mov	r3, r8
	ldr	r3, [r3, r1]
	ldr	r2, [r0, #0]
	adds	r1, #4
	eors	r2, r3
	str	r2, [r0, #0]
	mov	r3, r8
	ldr	r2, [r0, #32]
	ldr	r1, [r3, r1]
	eors	r2, r1
	str	r2, [r0, #32]
.L_0804521c:
	adds	r4, #1
	adds	r0, #4
	cmp	r4, #7
	ble.n	.L_080451e8
	adds	r6, #1
	adds	r5, #64
	cmp	r6, #5
	ble.n	.L_080451da
	adds	r7, #1
	cmp	r7, #1
	ble.n	.L_080451ce
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805f700
	.4byte 0x03000260
	.4byte 0x06006280
	.2byte 0x4444
	.2byte 0x4444
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	lsls	r5, r3, #1
	movs	r3, #243
	lsls	r3, r3, #8
	mov	r8, r1
	adds	r3, #21
	movs	r1, #128
	mov	r9, r2
	adds	r3, r3, r5
	lsls	r1, r1, #3
	sub	sp, #4
	movs	r6, #0
	mov	fp, r3
	orrs	r1, r3
	mov	r2, r8
	mov	r3, r9
	str	r6, [sp, #0]
	mov	sl, r0
	bl	sub_0803c378
	movs	r3, #243
	lsls	r3, r3, #8
	adds	r3, #20
	adds	r5, r5, r3
	mov	r2, r8
	mov	r0, sl
	adds	r1, r5, #0
	mov	r3, r9
	adds	r2, #1
	str	r6, [sp, #0]
	bl	sub_0803c378
	movs	r3, #2
	add	r8, r3
	mov	r0, sl
	mov	r1, fp
	mov	r2, r8
	mov	r3, r9
	str	r6, [sp, #0]
	bl	sub_0803c378
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
