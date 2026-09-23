.syntax unified
	.thumb
	.global Func_08014644
	.thumb_func
Func_08014644:
	push	{r5, lr}
	ldr	r4, [pc, #68]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #64]
	ldrh	r2, [r3, #0]
	strh	r3, [r3, #0]
	movs	r1, #0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_08014668
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	str	r1, [r4, #0]
	strh	r3, [r4, #4]
	movs	r5, #0
	b.n	.L_08014684
.L_08014668:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #23
	bgt.n	.L_08014684
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_08014668
	movs	r3, #0
	str	r3, [r4, #0]
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	strh	r3, [r4, #4]
	adds	r5, r1, #0
.L_08014684:
	ldr	r3, [pc, #8]
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, pc}
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #52]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #48]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r1, #0
	movs	r2, #1
.L_080146a8:
	cmp	r0, #0
	beq.n	.L_080146b2
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_080146ba
.L_080146b2:
	ldrb	r3, [r4, #5]
	orrs	r3, r2
	strb	r3, [r4, #5]
	adds	r5, r1, #0
.L_080146ba:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #23
	ble.n	.L_080146a8
	ldr	r3, [pc, #12]
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #60]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #56]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r4, #0
	movs	r7, #1
.L_080146e8:
	ldrb	r3, [r1, #3]
	cmp	r3, #2
	bne.n	.L_08014702
	ldrb	r2, [r1, #6]
	adds	r3, r7, #0
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #0
	bne.n	.L_08014702
	ldrb	r3, [r1, #5]
	orrs	r3, r0
	strb	r3, [r1, #5]
	adds	r5, r4, #0
.L_08014702:
	adds	r4, #1
	adds	r1, #8
	cmp	r4, #23
	ble.n	.L_080146e8
	ldr	r3, [pc, #12]
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	adds	r6, r1, #0
	ldr	r1, [pc, #48]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #48]
	ldrh	r2, [r3, #0]
	strh	r3, [r3, #0]
	movs	r4, #0
	ldr	r3, [r1, #0]
	cmp	r3, r0
	bne.n	.L_0801473a
	strb	r6, [r1, #6]
	movs	r5, #0
	b.n	.L_0801474c
.L_0801473a:
	adds	r4, #1
	adds	r1, #8
	cmp	r4, #23
	bgt.n	.L_0801474c
	ldr	r3, [r1, #0]
	cmp	r3, r0
	bne.n	.L_0801473a
	strb	r6, [r1, #6]
	adds	r5, r4, #0
.L_0801474c:
	ldr	r3, [pc, #8]
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	ldr	r4, [pc, #52]
	movs	r5, #1
	negs	r5, r5
	ldr	r3, [pc, #48]
	ldrh	r2, [r3, #0]
	adds	r7, r2, #0
	strh	r3, [r3, #0]
	movs	r1, #0
	movs	r6, #254
.L_08014770:
	cmp	r0, #0
	beq.n	.L_0801477a
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L_08014784
.L_0801477a:
	ldrb	r2, [r4, #5]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r4, #5]
	adds	r5, r1, #0
.L_08014784:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #23
	ble.n	.L_08014770
	ldr	r3, [pc, #8]
	strh	r7, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6, r7, pc}
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	ldr	r4, [pc, #48]
	movs	r0, #1
	negs	r0, r0
	ldr	r3, [pc, #44]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r1, #0
	movs	r5, #254
.L_080147b0:
	ldrb	r3, [r4, #3]
	cmp	r3, #2
	bne.n	.L_080147c0
	ldrb	r2, [r4, #5]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r4, #5]
	adds	r0, r1, #0
.L_080147c0:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #23
	ble.n	.L_080147b0
	ldr	r3, [pc, #8]
	strh	r6, [r3, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02003610
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #40]
	adds	r7, r0, #0
	ldrb	r3, [r3, #0]
	ldr	r6, [pc, #36]
	asrs	r7, r7, #8
	cmp	r3, #1
	bne.n	.L_08014802
	movs	r5, #25
	subs	r6, #8
.L_080147ec:
	subs	r5, #1
	cmp	r5, #0
	beq.n	.L_08014802
	adds	r6, #8
	ldrb	r3, [r6, #5]
	cmp	r3, r7
	bne.n	.L_080147ec
	ldr	r0, [r6, #0]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0xe7f4
.L_08014802:
	pop	{r5, r6, r7, pc}
	.4byte 0x03001228
	.2byte 0x3610
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r4, #0
	ldrb	r3, [r3, #0]
	ldr	r1, [pc, #36]
	asrs	r0, r0, #8
	cmp	r3, #1
	bne.n	.L_08014836
	movs	r2, #25
	subs	r1, #8
.L_08014820:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_0801482a
	adds	r0, r4, #0
	b.n	.L_08014836
.L_0801482a:
	adds	r1, #8
	ldrb	r3, [r1, #5]
	cmp	r3, r0
	bne.n	.L_08014820
	adds	r4, #1
	b.n	.L_08014820
.L_08014836:
	pop	{pc}
	.4byte 0x03001228
	.4byte 0x02003610
