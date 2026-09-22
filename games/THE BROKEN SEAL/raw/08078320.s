.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.global Overlay_08078320
Overlay_08078320:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #54
	ldrsh	r3, [r5, r2]
	adds	r2, r3, #0
	cmp	r1, r3
	bgt.n	.L_08078336
	movs	r2, #0
	cmp	r1, #0
	blt.n	.L_08078336
	adds	r2, r1, #0
.L_08078336:
	strh	r2, [r5, #58]
	movs	r3, #56
	ldrsh	r0, [r5, r3]
	lsls	r0, r0, #14
	movs	r2, #52
	ldrsh	r1, [r5, r2]
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_08078356
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_08078356
	adds	r3, r0, #0
.L_08078356:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0807836a
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0807836a
	movs	r3, #1
	strh	r3, [r5, #20]
.L_0807836a:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_08078388
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_08078388
	adds	r3, r0, #0
.L_08078388:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0807839c
	movs	r2, #58
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_0807839c
	movs	r3, #1
	strh	r3, [r5, #22]
.L_0807839c:
	pop	{r5}
	pop	{r0}
	bx	r0
