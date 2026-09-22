.syntax unified
	.thumb
	.set sub_08002074, 0x08002074
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad3a8, 0x080ad3a8
	.set sub_080affac, 0x080affac
	.global Overlay_080b005c
Overlay_080b005c:
	push	{lr}
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	bl	sub_080ad3a8
	adds	r0, #42
	ldrb	r1, [r0, #0]
	cmp	r1, #47
	bls.n	.L_080b0074
	movs	r1, #0
.L_080b0074:
	ldr	r3, [pc, #8]
	lsls	r2, r1, #1
	adds	r2, r2, r1
	lsls	r2, r2, #3
	ldr	r0, [r3, r2]
	pop	{pc}
	.2byte 0x6684
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r1
	sub	sp, #16
	bl	sub_08016ca4
	movs	r1, #42
	adds	r2, r0, #0
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b00d4
	adds	r1, #33
	adds	r3, r2, r1
	ldrh	r0, [r3, #0]
	bl	sub_080ad3a8
	adds	r0, #42
	ldrb	r0, [r0, #0]
	cmp	r0, #47
	bls.n	.L_080b00b6
	movs	r0, #0
.L_080b00b6:
	ldr	r2, [pc, #128]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r3, r3, r2
	adds	r2, r3, #0
	movs	r6, #0
	mov	r1, sl
	adds	r2, #8
.L_080b00c8:
	ldmia	r2!, {r3}
	adds	r6, #1
	stmia	r1!, {r3}
	cmp	r6, #3
	ble.n	.L_080b00c8
	b.n	.L_080b012e
.L_080b00d4:
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r5, sp
	adds	r1, r2, #0
	ldrh	r0, [r3, #0]
	adds	r2, r5, #0
	adds	r1, #248
	bl	sub_080affac
	ldr	r7, [pc, #80]
	mov	r8, r5
	movs	r6, #3
	movs	r5, #0
.L_080b00f0:
	mov	r3, r8
	ldr	r0, [r5, r3]
	ldr	r1, [pc, #72]
	bl	sub_08002074
	mov	r1, r8
	lsls	r3, r0, #2
	ldr	r2, [r5, r1]
	adds	r3, r3, r0
	lsls	r3, r3, #1
	subs	r4, r2, r3
	cmp	r0, #15
	ble.n	.L_080b010c
	movs	r0, #15
.L_080b010c:
	cmp	r0, #0
	bge.n	.L_080b0112
	movs	r0, #0
.L_080b0112:
	mov	r3, sl
	lsls	r2, r0, #2
	adds	r1, r5, r3
	ldrh	r3, [r7, r2]
	adds	r2, r2, r7
	adds	r3, r3, r4
	strh	r3, [r1, #0]
	subs	r6, #1
	ldrh	r3, [r2, #2]
	adds	r5, #4
	adds	r3, r3, r4
	strh	r3, [r1, #2]
	cmp	r6, #0
	bge.n	.L_080b00f0
.L_080b012e:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x080c6684
	.4byte 0x080c6644
	.2byte 0x999a
	.2byte 0x1999
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r2
	movs	r2, #1
	adds	r7, r0, #0
	negs	r2, r2
	sub	sp, #16
	mov	fp, r2
	movs	r0, #0
	cmp	r7, #7
	ble.n	.L_080b0166
	b.n	.L_080b0270
.L_080b0166:
	mov	r5, sp
	adds	r0, r7, #0
	adds	r2, r5, #0
	bl	sub_080affac
	mov	ip, fp
	mov	r0, fp
	movs	r4, #0
	adds	r2, r5, #0
.L_080b0178:
	ldmia	r2!, {r3}
	cmp	ip, r3
	bge.n	.L_080b0182
	mov	ip, r3
	adds	r0, r4, #0
.L_080b0182:
	adds	r4, #1
	cmp	r4, #3
	ble.n	.L_080b0178
	movs	r1, #1
	negs	r1, r1
	mov	ip, r1
	movs	r4, #0
	adds	r2, r5, #0
.L_080b0192:
	cmp	r4, r0
	beq.n	.L_080b01a0
	ldr	r3, [r2, #0]
	cmp	ip, r3
	bge.n	.L_080b01a0
	mov	ip, r3
	adds	r1, r4, #0
.L_080b01a0:
	adds	r4, #1
	adds	r2, #4
	cmp	r4, #3
	ble.n	.L_080b0192
	lsls	r3, r1, #2
	ldr	r3, [r5, r3]
	cmp	r3, #9
	bgt.n	.L_080b01b2
	adds	r1, r0, #0
.L_080b01b2:
	ldr	r2, [pc, #204]
	lsls	r3, r0, #2
	adds	r3, r3, r1
	lsls	r3, r3, #2
	ldr	r6, [r2, r3]
	cmp	r6, #2
	bne.n	.L_080b01c8
	cmp	r7, #5
	bne.n	.L_080b01c8
	movs	r6, #13
	b.n	.L_080b01d2
.L_080b01c8:
	cmp	r6, #6
	bne.n	.L_080b01d2
	cmp	r7, #7
	bne.n	.L_080b01d2
	movs	r6, #14
.L_080b01d2:
	movs	r3, #222
	lsls	r3, r3, #1
	cmp	r8, r3
	bne.n	.L_080b01de
	movs	r6, #16
	b.n	.L_080b01f8
.L_080b01de:
	movs	r1, #188
	adds	r1, #255
	cmp	r8, r1
	bne.n	.L_080b01ee
	movs	r6, #15
	b.n	.L_080b01f8
.L_080b01ea:
	mov	fp, r4
	b.n	.L_080b0262
.L_080b01ee:
	movs	r2, #190
	adds	r2, #255
	cmp	r8, r2
	bne.n	.L_080b01f8
	movs	r6, #17
.L_080b01f8:
	ldr	r3, [pc, #136]
	movs	r1, #158
	mov	sl, r3
	lsls	r1, r1, #7
	mov	r8, sl
	adds	r1, #192
	movs	r7, #158
	mov	ip, r5
	add	r1, r8
	lsls	r7, r7, #7
	movs	r4, #243
	mov	r9, ip
	mov	lr, r1
	adds	r7, #188
.L_080b0214:
	mov	r2, sl
	ldr	r3, [r7, r2]
	cmp	r3, r6
	bne.n	.L_080b0254
	mov	r1, lr
	ldrb	r3, [r1, #0]
	mov	r1, ip
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	movs	r5, #0
	cmp	r3, r2
	blt.n	.L_080b0250
	mov	r2, r8
	adds	r3, r7, r2
	mov	r0, r9
	adds	r1, r3, #4
.L_080b0238:
	adds	r5, #1
	cmp	r5, #3
	bgt.n	.L_080b0250
	adds	r1, #1
	ldrb	r3, [r1, #0]
	adds	r0, #4
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r0, #0]
	lsls	r2, r2, #1
	cmp	r3, r2
	bge.n	.L_080b0238
.L_080b0250:
	cmp	r5, #4
	beq.n	.L_080b01ea
.L_080b0254:
	movs	r3, #84
	negs	r3, r3
	subs	r4, #1
	add	lr, r3
	subs	r7, #84
	cmp	r4, #0
	bge.n	.L_080b0214
.L_080b0262:
	movs	r1, #1
	negs	r1, r1
	cmp	fp, r1
	bne.n	.L_080b026e
	movs	r2, #0
	mov	fp, r2
.L_080b026e:
	mov	r0, fp
.L_080b0270:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080c6604
	.4byte 0x080c15f4
