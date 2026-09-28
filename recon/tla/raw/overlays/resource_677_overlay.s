.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200886d, 0x02008039, 0x02008045, 0x0200804d, 0x02008205, 0x02008041, 0x0200895d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c18
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c48
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #24]
	b.n	.L_02000070
.L_02000064:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200006e
	ldr	r0, [pc, #24]
	b.n	.L_02000070
.L_0200006e:
	ldr	r0, [pc, #24]
.L_02000070:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008d
	.4byte 0x02008ca4
	.4byte 0x0000008e
	.4byte 0x02008dac
	.2byte 0x8c8c
	.2byte 0x0200
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_020000ae
	ldr	r0, [pc, #28]
	bl 0x02008a18
	b.n	.L_020000b4
.L_020000ae:
	ldr	r0, [pc, #24]
	bl 0x02008a18
.L_020000b4:
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002233
	.2byte 0x2228
	.2byte 0x0000
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_020000ee
	ldr	r0, [pc, #28]
	bl 0x02008a18
	b.n	.L_020000f4
.L_020000ee:
	ldr	r0, [pc, #24]
	bl 0x02008a18
.L_020000f4:
	movs	r0, #15
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002234
	.2byte 0x2229
	.2byte 0x0000
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_0200012e
	ldr	r0, [pc, #28]
	bl 0x02008a18
	b.n	.L_02000134
.L_0200012e:
	ldr	r0, [pc, #24]
	bl 0x02008a18
.L_02000134:
	movs	r0, #16
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002235
	.2byte 0x222a
	.2byte 0x0000
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_0200016e
	ldr	r0, [pc, #28]
	bl 0x02008a18
	b.n	.L_02000174
.L_0200016e:
	ldr	r0, [pc, #24]
	bl 0x02008a18
.L_02000174:
	movs	r0, #16
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002232
	.2byte 0x2227
	.2byte 0x0000
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	movs	r0, #10
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2218
	.2byte 0x0000
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	movs	r0, #11
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x221a
	.2byte 0x0000
	push	{lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	movs	r0, #12
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x221c
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_0200021c
	ldr	r0, [pc, #24]
	b.n	.L_02000228
.L_0200021c:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000226
	ldr	r0, [pc, #24]
	b.n	.L_02000228
.L_02000226:
	ldr	r0, [pc, #24]
.L_02000228:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008d
	.4byte 0x02008ea8
	.4byte 0x0000008e
	.4byte 0x02008fe0
	.2byte 0x8e84
	.2byte 0x0200
	push	{r5, lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008a18
	movs	r1, #0
	movs	r0, #9
	bl 0x02008a20
	bl 0x02008a58
	movs	r1, #0
	bl 0x020089c8
	cmp	r0, #0
	bne.n	.L_0200027c
	movs	r0, #10
	bl 0x020089a8
	adds	r0, r5, #1
	bl 0x02008a18
	b.n	.L_02000288
.L_0200027c:
	movs	r0, #20
	bl 0x020089a8
	adds	r0, r5, #2
	bl 0x02008a18
.L_02000288:
	movs	r0, #9
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x220a
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020089d0
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020002d8
	movs	r0, #20
	adds	r1, r5, #0
	bl 0x02008a68
	b.n	.L_020002f4
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002d8:
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
.L_020002f4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2217
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020089d0
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000338
	movs	r0, #21
	adds	r1, r5, #0
	bl 0x02008a68
	b.n	.L_02000354
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000338:
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
.L_02000354:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2219
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020089d0
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000398
	movs	r0, #22
	adds	r1, r5, #0
	bl 0x02008a68
	b.n	.L_020003b4
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000398:
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
.L_020003b4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x221b
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020089d0
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #32]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000400
	movs	r0, #7
	adds	r1, r5, #0
	bl 0x02008a78
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008970
	b.n	.L_0200041c
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000400:
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
.L_0200041c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x221d
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020089d0
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200045c
	adds	r0, r5, #0
	bl 0x02008a70
	b.n	.L_02000478
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200045c:
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	ldr	r0, [pc, #20]
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
.L_02000478:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2221
	.2byte 0x0000
	push	{r5, lr}
	bl 0x020089b0
	movs	r0, #0
	bl 0x02008a50
	movs	r1, #2
	movs	r0, #14
	bl 0x02008a08
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008a18
	movs	r1, #0
	movs	r0, #14
	bl 0x02008a20
	bl 0x02008a58
	movs	r1, #0
	bl 0x020089c8
	cmp	r0, #0
	bne.n	.L_020004c0
	movs	r0, #10
	bl 0x020089a8
	adds	r0, r5, #1
	bl 0x02008a18
	b.n	.L_020004cc
.L_020004c0:
	movs	r0, #20
	bl 0x020089a8
	adds	r0, r5, #2
	bl 0x02008a18
.L_020004cc:
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a28
	bl 0x020089b8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2223
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #174
	adds	r5, #255
.L_020004e6:
	adds	r0, r5, #0
	bl 0x02008990
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_02000510
	movs	r3, #182
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_020004e6
	movs	r5, #162
	adds	r5, #255
.L_02000502:
	adds	r0, r5, #0
	bl 0x02008990
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02000514
.L_02000510:
	movs	r0, #1
	b.n	.L_02000520
.L_02000514:
	movs	r3, #172
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_02000502
	movs	r0, #0
.L_02000520:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl 0x020084e0
	cmp	r0, #0
	beq.n	.L_02000538
	movs	r0, #16
	adds	r0, #255
	bl 0x02008968
	b.n	.L_02000556
.L_02000538:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	bne.n	.L_02000556
	bl 0x02008a90
	cmp	r0, #0
	bne.n	.L_02000556
	movs	r0, #16
	adds	r0, #255
	bl 0x02008970
.L_02000556:
	movs	r0, #16
	adds	r0, #255
	bl 0x02008960
	cmp	r0, #0
	bne.n	.L_02000598
	ldr	r5, [pc, #516]
	adds	r0, r5, #0
	bl 0x02008a18
	movs	r1, #0
	movs	r0, #14
	bl 0x02008a20
	bl 0x02008a58
	movs	r1, #0
	bl 0x020089c8
	cmp	r0, #0
	bne.n	.L_0200058a
	movs	r0, #10
	bl 0x020089a8
	adds	r0, r5, #1
	b.n	.L_020005b4
.L_0200058a:
	movs	r0, #20
	bl 0x020089a8
	adds	r0, r5, #2
	bl 0x02008a18
	b.n	.L_020005b8
.L_02000598:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_020005aa
	ldr	r0, [pc, #452]
	b.n	.L_020005b4
.L_020005aa:
	bl 0x02008a90
	cmp	r0, #0
	beq.n	.L_020005c2
	ldr	r0, [pc, #444]
.L_020005b4:
	bl 0x02008a18
.L_020005b8:
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a28
	b.n	.L_02000766
.L_020005c2:
	ldr	r5, [pc, #432]
	adds	r0, r5, #0
	bl 0x02008a18
	movs	r1, #0
	movs	r0, #14
	bl 0x02008a20
	bl 0x02008a58
	movs	r1, #0
	bl 0x020089c8
	cmp	r0, #0
	beq.n	.L_020005e2
	b.n	.L_02000758
.L_020005e2:
	adds	r0, r5, #1
	bl 0x02008a18
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a28
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x02008a60
	movs	r0, #14
	bl 0x020089d0
	movs	r1, #1
	bl 0x02008978
	ldr	r5, [pc, #368]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #244
	movs	r2, #204
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl 0x020089e0
	ldr	r0, [r5, #0]
	bl 0x020089f0
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x02008a30
	movs	r1, #252
	movs	r2, #204
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x020089e0
	movs	r0, #14
	bl 0x020089f0
	ldr	r1, [r5, #0]
	movs	r0, #14
	movs	r2, #0
	bl 0x02008a10
	movs	r0, #14
	bl 0x02008a80
	bl 0x02008a90
	cmp	r0, #0
	beq.n	.L_02000722
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #14
	adds	r1, #102
	adds	r2, #51
	bl 0x020089d8
	movs	r1, #142
	movs	r2, #204
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x020089e8
	movs	r0, #14
	bl 0x020089f0
	movs	r1, #142
	movs	r2, #180
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x020089e8
	movs	r0, #14
	bl 0x020089f0
	movs	r1, #158
	movs	r2, #173
	movs	r0, #16
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x020089e8
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #7
	bl 0x02008a30
	movs	r0, #16
	movs	r1, #0
	bl 0x02008a00
	movs	r1, #150
	movs	r2, #180
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #14
	bl 0x020089e8
	movs	r0, #14
	bl 0x020089f0
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a00
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #14
	bl 0x02008a30
	movs	r0, #14
	bl 0x020089d0
	movs	r3, #129
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x020089d0
	movs	r3, #1
	adds	r0, #35
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #14
	bl 0x02008a38
	movs	r0, #14
	bl 0x020089d0
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r1, #0
	movs	r0, #17
	movs	r2, #0
	bl 0x020089f8
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008968
	b.n	.L_02000766
.L_02000722:
	movs	r1, #236
	movs	r2, #199
	adds	r1, #255
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x020089e0
	movs	r0, #14
	bl 0x020089f0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #14
	bl 0x02008a30
	movs	r0, #14
	bl 0x020089d0
	movs	r1, #0
	bl 0x02008978
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a60
	b.n	.L_02000766
.L_02000758:
	adds	r0, r5, #2
	bl 0x02008a18
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a28
.L_02000766:
	pop	{r5, pc}
	.4byte 0x00001307
	.4byte 0x0000130d
	.4byte 0x0000130e
	.4byte 0x0000130a
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #16
	adds	r0, #255
	bl 0x02008960
	cmp	r0, #0
	bne.n	.L_02000794
	ldr	r0, [pc, #196]
	b.n	.L_02000806
.L_02000794:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_020007a6
	ldr	r0, [pc, #184]
	b.n	.L_02000806
.L_020007a6:
	bl 0x02008a90
	cmp	r0, #0
	beq.n	.L_02000842
	bl 0x02008a90
	adds	r6, r0, #0
	bl 0x02008988
	ldrh	r0, [r0, #0]
	movs	r1, #2
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x02008980
	movs	r1, #5
	mov	r0, r8
	bl 0x02008980
	ldr	r7, [pc, #144]
	adds	r0, r7, #0
	bl 0x02008a18
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02008a20
	bl 0x02008a58
	movs	r1, #0
	bl 0x020089c8
	cmp	r0, #0
	beq.n	.L_020007ee
	adds	r0, r7, #1
	b.n	.L_02000806
.L_020007ee:
	adds	r0, r6, #0
	bl 0x020089a0
	cmp	r0, #0
	bge.n	.L_020007fc
	adds	r0, r7, #2
	b.n	.L_02000806
.L_020007fc:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #16]
	cmp	r3, r8
	bcs.n	.L_02000814
	adds	r0, r7, #3
.L_02000806:
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	b.n	.L_02000850
.L_02000814:
	adds	r0, r7, #4
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x02008a40
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x020089c0
	movs	r0, #0
	bl 0x02008a88
	mov	r3, r8
	negs	r0, r3
	bl 0x02008998
	b.n	.L_02000850
.L_02000842:
	ldr	r0, [pc, #36]
	bl 0x02008a18
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008a28
.L_02000850:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0000130f
	.4byte 0x00001310
	.4byte 0x00001311
	.4byte 0x02000240
	.2byte 0x1316
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	ldr	r3, [pc, #204]
	subs	r2, #41
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #196]
	cmp	r2, r3
	bne.n	.L_020008f0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl 0x02008960
	cmp	r0, #0
	beq.n	.L_020008d4
	movs	r1, #150
	movs	r2, #180
	movs	r0, #14
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x020089f8
	movs	r1, #158
	movs	r2, #173
	movs	r0, #16
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x020089f8
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008a30
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x020089f8
	b.n	.L_020008dc
.L_020008d4:
	movs	r0, #14
	movs	r1, #0
	bl 0x02008a60
.L_020008dc:
	movs	r0, #17
	bl 0x020089d0
	movs	r1, #0
	bl 0x02008978
	movs	r0, #11
	movs	r1, #2
	bl 0x02008a38
.L_020008f0:
	ldr	r3, [pc, #92]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_0200094c
	movs	r0, #0
	bl 0x02008a48
	movs	r1, #2
	movs	r0, #12
	bl 0x02008a38
	movs	r0, #12
	bl 0x020089d0
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #4
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #10
	bl 0x02008a38
	movs	r0, #10
	bl 0x020089d0
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r1, #2
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x02008a38
	movs	r0, #11
	bl 0x020089d0
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
.L_0200094c:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0000008d
	.2byte 0x008e
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020219, 0x08038121, 0x080ad011, 0x080ad039, 0x080ad1d9, 0x080ad2f1, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8061, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d1, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8141, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c83e1, 0x080c8481, 0x080c84e1, 0x080c8779, 0x080c87b1, 0x08108009, 0x08108011, 0x08108019, 0x08108021, 0x08108099, 0x081080a1
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b50000
	.4byte 0x00000000
	.4byte 0x007f0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01c50000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000008d
	.4byte 0x1010108b
	.4byte 0xffffffff
	.4byte 0x1020208b
	.4byte 0xffffffff
	.4byte 0x1030708b
	.4byte 0xffffffff
	.4byte 0x1040408b
	.4byte 0xffffffff
	.4byte 0x0000008e
	.4byte 0x1010308b
	.4byte 0xffffffff
	.4byte 0x1020608b
	.4byte 0xffffffff
	.4byte 0x1030508b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x02008a98
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01c60000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01260000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x01eb0000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00024000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00018000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x015a0000
	.4byte 0x0001c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00010000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002209
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008245
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000220f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002210
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020083bd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000221e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200877d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200814d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008525
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000220d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000220e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002211
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002212
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000221f
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002220
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0200808d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x020080cd
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0200810d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0200808d
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403056
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002213
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002214
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020082fd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020082fd
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200835d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002215
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002216
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002218
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000221a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000221c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0200818d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x020081b5
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x020081dd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
