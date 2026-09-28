.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008689, 0x02008039, 0x02008045, 0x0200804d, 0x02008681, 0x02008041, 0x02008701
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8ebc
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8f64
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8f70
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	mov	r8, r1
	movs	r0, #206
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02008d00
	movs	r7, #0
	mov	sl, r0
	cmp	r7, r8
	bge.n	.L_02000114
.L_02000074:
	movs	r0, #1
	movs	r1, #1
	bl 0x02008d08
	movs	r0, #5
	movs	r1, #2
	bl 0x02008d08
	movs	r0, #241
	lsls	r0, r0, #9
	adds	r0, #64
	movs	r1, #5
	bl 0x02008d08
	bl 0x02008d10
	movs	r2, #0
	movs	r3, #34
	adds	r0, r6, #0
	movs	r1, #5
	bl 0x02008cb0
	ldr	r3, [pc, #132]
	movs	r2, #3
	ldr	r3, [r3, #0]
	movs	r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020000d6
.L_020000ae:
	movs	r0, #1
	adds	r5, #1
	bl 0x02008c68
	cmp	r5, #59
	bgt.n	.L_020000d6
	ldr	r3, [pc, #108]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020000ae
	b.n	.L_020000d6
.L_020000c8:
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r0, #1
	str	r3, [r1, #4]
	str	r3, [r1, #12]
	bl 0x02008c68
.L_020000d6:
	bl 0x02008cc0
	ldr	r1, [pc, #76]
	cmp	r0, #0
	beq.n	.L_020000c8
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	movs	r5, #0
	b.n	.L_020000fe
.L_020000ea:
	movs	r0, #1
	adds	r5, #1
	bl 0x02008c68
	cmp	r5, #9
	bgt.n	.L_0200010c
	ldr	r1, [pc, #48]
	movs	r2, #2
	ldr	r3, [r1, #0]
	ands	r3, r2
.L_020000fe:
	cmp	r3, #0
	bne.n	.L_02000114
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020000ea
.L_0200010c:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, r8
	blt.n	.L_02000074
.L_02000114:
	bl 0x02008d10
	mov	r0, sl
	movs	r1, #2
	bl 0x02008ca8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	subs	r1, r1, r0
	bl 0x02008054
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0000124c
	.2byte 0x1277
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	subs	r1, r0, r1
	bl 0x02008054
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001277
	.2byte 0x124c
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #12]
	ldr	r1, [pc, #12]
	ldr	r0, [pc, #16]
	subs	r1, r1, r3
	bl 0x02008054
	pop	{pc}
	.4byte 0x0000124c
	.4byte 0x00001277
	.2byte 0x12a2
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #12]
	ldr	r1, [pc, #12]
	subs	r1, r1, r0
	bl 0x02008054
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000012d2
	.2byte 0x12fd
	.2byte 0x0000
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #1
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #2
.L_020001a2:
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #3
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #24
	bl 0x02008e40
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #0
	bl 0x02008e50
	pop	{pc}
	push	{lr}
	bl 0x02008e48
	pop	{pc}
	push	{r5, r6, lr}
.L_020001d6:
	adds	r5, r0, #0
	movs	r0, #16
	adds	r0, #255
	bl 0x02008c78
	cmp	r0, #0
	bne.n	.L_0200021a
	ldr	r6, [pc, #176]
	adds	r0, r6, #0
	bl 0x02008e08
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02008e10
	bl 0x02008e30
	movs	r1, #0
	bl 0x02008de8
	cmp	r0, #0
	bne.n	.L_0200020c
	movs	r0, #10
	bl 0x02008dd8
	adds	r0, r6, #1
	b.n	.L_02000234
.L_0200020c:
	movs	r0, #20
	bl 0x02008dd8
	adds	r0, r6, #2
	bl 0x02008e08
	b.n	.L_02000238
.L_0200021a:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008c78
	cmp	r0, #0
	beq.n	.L_0200022a
	ldr	r0, [pc, #116]
	b.n	.L_02000234
.L_0200022a:
	bl 0x02008e70
	cmp	r0, #0
	beq.n	.L_02000242
	ldr	r0, [pc, #108]
.L_02000234:
	bl 0x02008e08
.L_02000238:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e18
	b.n	.L_02000294
.L_02000242:
	ldr	r6, [pc, #96]
	adds	r0, r6, #0
	bl 0x02008e08
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02008e10
	bl 0x02008e30
	movs	r1, #0
	bl 0x02008de8
	cmp	r0, #0
	bne.n	.L_02000286
	adds	r0, r6, #1
	bl 0x02008e08
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e18
	adds	r0, r5, #0
	bl 0x02008e58
	bl 0x02008e70
	cmp	r0, #0
	beq.n	.L_02000294
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008c80
	b.n	.L_02000294
.L_02000286:
	adds	r0, r6, #2
	bl 0x02008e08
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e18
.L_02000294:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001307
	.4byte 0x0000130d
	.4byte 0x0000130e
	.2byte 0x130a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #16
	adds	r0, #255
	bl 0x02008c78
	cmp	r0, #0
	bne.n	.L_020002c0
	ldr	r0, [pc, #192]
	b.n	.L_02000330
.L_020002c0:
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02008c78
	cmp	r0, #0
	beq.n	.L_020002d0
	ldr	r0, [pc, #180]
	b.n	.L_02000330
.L_020002d0:
	bl 0x02008e70
	cmp	r0, #0
	beq.n	.L_0200036c
	bl 0x02008e70
	adds	r6, r0, #0
	bl 0x02008d48
	ldrh	r0, [r0, #0]
	movs	r1, #2
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x02008d08
	movs	r1, #5
	mov	r0, r8
	bl 0x02008d08
	ldr	r7, [pc, #144]
	adds	r0, r7, #0
	bl 0x02008e08
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x02008e10
	bl 0x02008e30
	movs	r1, #0
	bl 0x02008de8
	cmp	r0, #0
	beq.n	.L_02000318
	adds	r0, r7, #1
	b.n	.L_02000330
.L_02000318:
	adds	r0, r6, #0
	bl 0x02008dc8
	cmp	r0, #0
	bge.n	.L_02000326
	adds	r0, r7, #2
	b.n	.L_02000330
.L_02000326:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #16]
	cmp	r3, r8
	bcs.n	.L_0200033e
	adds	r0, r7, #3
.L_02000330:
	bl 0x02008e08
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e18
	b.n	.L_0200037a
.L_0200033e:
	adds	r0, r7, #4
	bl 0x02008e08
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e18
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x02008e28
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02008de0
	movs	r0, #0
	bl 0x02008e68
	mov	r3, r8
	negs	r0, r3
	bl 0x02008da0
	b.n	.L_0200037a
.L_0200036c:
	ldr	r0, [pc, #32]
	bl 0x02008e08
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02008e18
.L_0200037a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0000130f
	.4byte 0x00001310
	.4byte 0x00001311
	.4byte 0x02000240
	.2byte 0x1316
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	mov	r1, sp
	add	r0, sp, #4
	bl 0x02008e60
	add	sp, #8
	pop	{pc}
	push	{r5, lr}
	movs	r5, #0
.L_020003a8:
	adds	r0, r5, #0
	adds	r5, #1
	bl 0x02008d20
	cmp	r5, #7
	ble.n	.L_020003a8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r0, [pc, #344]
	movs	r1, #1
	bl 0x02008cb8
	ldr	r2, [pc, #340]
	ldr	r3, [pc, #340]
	movs	r5, #9
	str	r3, [r2, #16]
.L_020003ca:
	movs	r1, #228
	movs	r0, #4
	bl 0x02008d50
	subs	r5, #1
	movs	r0, #4
	movs	r1, #229
	bl 0x02008d50
	cmp	r5, #0
	bge.n	.L_020003ca
	movs	r1, #184
	movs	r0, #4
	bl 0x02008d50
	movs	r1, #204
	movs	r0, #4
	bl 0x02008d50
	movs	r1, #224
	movs	r0, #4
	bl 0x02008d50
	movs	r1, #11
	movs	r0, #4
	bl 0x02008d50
	movs	r1, #12
	adds	r1, #255
	movs	r0, #4
	bl 0x02008d50
	movs	r1, #223
	movs	r0, #4
	bl 0x02008d50
	movs	r1, #226
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #227
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #230
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #232
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #231
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #237
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #10
	adds	r1, #255
	movs	r0, #5
	bl 0x02008d50
	movs	r1, #242
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #252
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #174
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #174
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #174
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #178
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #180
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #162
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #162
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #209
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #164
	adds	r1, #255
	movs	r0, #6
	bl 0x02008d50
	movs	r1, #189
	movs	r0, #7
	bl 0x02008d50
	movs	r1, #200
	movs	r0, #7
	bl 0x02008d50
	movs	r1, #201
	movs	r0, #7
	bl 0x02008d50
	movs	r1, #202
	movs	r0, #7
	bl 0x02008d50
	movs	r1, #203
	movs	r0, #7
	bl 0x02008d50
	movs	r1, #204
	movs	r0, #7
	bl 0x02008d50
	movs	r1, #207
	movs	r0, #7
	bl 0x02008d50
	movs	r0, #0
	bl 0x02008d80
	movs	r0, #1
	bl 0x02008d80
	movs	r0, #2
	bl 0x02008d80
	movs	r0, #3
	bl 0x02008d80
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0000114d
	.4byte 0x02000240
	.2byte 0xde31
	.2byte 0x000b
	push	{r5, r6, r7, lr}
	ldr	r0, [pc, #212]
	movs	r1, #1
	bl 0x02008cb8
	movs	r1, #100
	negs	r1, r1
	movs	r0, #4
	bl 0x02008d68
	movs	r1, #100
	negs	r1, r1
	movs	r0, #5
	bl 0x02008d68
	movs	r1, #33
	negs	r1, r1
	movs	r0, #6
	bl 0x02008d68
	movs	r1, #50
	negs	r1, r1
	movs	r0, #4
	bl 0x02008d70
	movs	r1, #40
	negs	r1, r1
	movs	r0, #5
	bl 0x02008d70
	movs	r1, #35
	negs	r1, r1
	movs	r0, #6
	bl 0x02008d70
	movs	r0, #4
	bl 0x02008c70
	movs	r2, #160
	adds	r7, r0, #0
	lsls	r2, r2, #1
	movs	r6, #50
	adds	r3, r7, r2
	movs	r5, #1
	adds	r6, #255
	strb	r5, [r7, r6]
	movs	r0, #5
	strb	r5, [r3, #0]
	bl 0x02008c70
	movs	r2, #152
	adds	r7, r0, #0
	lsls	r2, r2, #1
	adds	r3, r7, r2
	strb	r5, [r3, #0]
	movs	r3, #2
	strb	r3, [r7, r6]
	movs	r0, #4
	bl 0x02008c70
	movs	r5, #0
	adds	r7, r0, #0
	movs	r6, #216
	b.n	.L_020005a4
.L_020005a0:
	adds	r6, #2
	adds	r5, #1
.L_020005a4:
	cmp	r5, #14
	bgt.n	.L_020005c2
	ldrh	r0, [r6, r7]
	bl 0x02008d48
	ldrh	r3, [r6, r7]
	cmp	r3, #0
	beq.n	.L_020005c2
	ldrb	r3, [r0, #12]
	cmp	r3, #2
	bne.n	.L_020005a0
	movs	r0, #4
	adds	r1, r5, #0
	bl 0x02008d60
.L_020005c2:
	movs	r0, #5
	bl 0x02008c70
	movs	r5, #0
	adds	r7, r0, #0
	movs	r6, #216
	b.n	.L_020005d4
.L_020005d0:
	adds	r6, #2
	adds	r5, #1
.L_020005d4:
	cmp	r5, #14
	bgt.n	.L_020005f2
	ldrh	r0, [r6, r7]
	bl 0x02008d48
	ldrh	r3, [r6, r7]
	cmp	r3, #0
	beq.n	.L_020005f2
	ldrb	r3, [r0, #12]
	cmp	r3, #2
	bne.n	.L_020005d0
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x02008d60
.L_020005f2:
	bl 0x02008dc0
	pop	{r5, r6, r7, pc}
	.2byte 0x114c
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	lsls	r0, r6, #2
	adds	r0, r0, r6
	adds	r7, r2, #0
	lsls	r0, r0, #2
	adds	r0, r0, r7
	adds	r0, #48
	mov	r8, r3
	bl 0x02008c80
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl 0x02008d88
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_02000632
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl 0x02008d90
.L_02000632:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl 0x02008cb8
	movs	r6, #0
.L_02000644:
	movs	r5, #0
.L_02000646:
	adds	r1, r5, #0
	adds	r0, r6, #0
	adds	r5, #1
	bl 0x02008db8
	cmp	r5, #17
	ble.n	.L_02000646
	adds	r6, #1
	cmp	r6, #3
	ble.n	.L_02000644
	bl 0x02008dc0
	pop	{r5, r6, pc}
	.4byte 0x0000114e
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r1, #0
	movs	r0, #11
	bl 0x02008de0
	movs	r0, #22
	bl 0x02008d38
	movs	r0, #22
	bl 0x02008dd0
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x91f8
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #100]
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r3, #139
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #16
	movs	r1, #5
	bl 0x02008e00
	movs	r0, #17
	movs	r1, #5
	bl 0x02008e00
	movs	r0, #18
	movs	r1, #5
	bl 0x02008e00
	movs	r0, #19
	movs	r1, #5
	bl 0x02008e00
	movs	r0, #30
	movs	r1, #6
	bl 0x02008e00
	movs	r0, #31
	movs	r1, #6
	bl 0x02008e00
	movs	r0, #32
	movs	r1, #6
	bl 0x02008e00
	movs	r0, #33
	movs	r1, #6
	bl 0x02008e00
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #99
	bne.n	.L_020006f8
	bl 0x02008db0
.L_020006f8:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	mov	r8, r0
	mov	r0, sp
	bl 0x02008d78
	movs	r6, #0
	adds	r7, r0, #0
	cmp	r6, r7
	bge.n	.L_0200073e
.L_0200071c:
	lsls	r3, r6, #1
	mov	r2, sp
	ldrh	r5, [r2, r3]
	adds	r6, #1
	adds	r0, r5, #0
	bl 0x02008c70
	ldrb	r1, [r0, #15]
	adds	r0, r5, #0
	add	r1, r8
	bl 0x02008d98
	adds	r0, r5, #0
	bl 0x02008d40
	cmp	r6, r7
	blt.n	.L_0200071c
.L_0200073e:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #232]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #4
	bl 0x02008c70
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #30
	movs	r3, #9
	mov	r8, r0
	movs	r0, #0
	bl 0x02008ca0
	ldr	r5, [pc, #200]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02008cd0
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #2
	bl 0x02008cd0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #32
	movs	r7, #1
	bl 0x02008cd0
.L_0200079c:
	cmp	r7, #0
	beq.n	.L_020007d2
	adds	r0, r6, #0
	bl 0x02008d18
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #48
	bl 0x02008ce0
	ldr	r0, [pc, #140]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #48
	bl 0x02008ce8
	mov	r3, r8
	ldrb	r0, [r3, #15]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r2, r6, #0
	movs	r3, #72
	movs	r7, #0
	bl 0x02008cf8
.L_020007d2:
	movs	r0, #1
	bl 0x02008c68
	ldr	r1, [pc, #104]
	movs	r2, #8
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020007ee
	ldr	r3, [r1, #4]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020007fc
.L_020007ee:
	movs	r0, #5
	bl 0x02008704
	movs	r0, #93
	bl 0x02008e78
	movs	r7, #1
.L_020007fc:
	ldr	r5, [pc, #68]
	movs	r2, #1
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000816
	movs	r0, #1
	bl 0x02008704
	movs	r0, #91
	bl 0x02008e78
	movs	r7, #1
.L_02000816:
	ldr	r3, [r5, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200079c
	movs	r0, #113
	bl 0x02008e78
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x02008ca8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00001151
	.4byte 0x02008e80
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	sub	sp, #4
	movs	r5, #2
	mov	r8, r2
	movs	r1, #0
	movs	r2, #30
	movs	r3, #7
	movs	r0, #0
	str	r5, [sp, #0]
	bl 0x02008ca0
	movs	r1, #8
	adds	r7, r0, #0
	movs	r2, #13
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl 0x02008ca0
	movs	r3, #128
	movs	r2, #128
	movs	r6, #1
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	r9, r0
	mov	sl, r6
	adds	r3, #212
	ldr	r0, [pc, #508]
	ldr	r1, [pc, #512]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #24
	ldr	r0, [pc, #504]
	adds	r1, #28
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #112
	bl 0x02008e78
	movs	r0, #1
	bl 0x02008c68
.L_020008ac:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_02000924
	movs	r2, #0
	adds	r0, r7, #0
	mov	sl, r2
	bl 0x02008cc8
	mov	r0, r9
	bl 0x02008cc8
	ldr	r0, [pc, #464]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x02008ce8
	mov	r3, sl
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #80
	bl 0x02008cf0
	bl 0x02008da8
	cmp	r0, #0
	beq.n	.L_02000918
	ldr	r0, [pc, #432]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	bl 0x02008ce8
	ldr	r0, [pc, #424]
	adds	r1, r7, #0
	adds	r0, r6, r0
	movs	r2, #120
	movs	r3, #0
	bl 0x02008cd8
	ldr	r0, [pc, #412]
	adds	r1, r7, #0
	adds	r0, r6, r0
	movs	r2, #0
	movs	r3, #16
	bl 0x02008cd0
	mov	r0, r9
	adds	r1, r6, #0
	bl 0x02008e38
	b.n	.L_02000924
.L_02000918:
	ldr	r0, [pc, #392]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	bl 0x02008ce8
.L_02000924:
	movs	r0, #1
	bl 0x02008c68
	ldr	r3, [pc, #380]
	movs	r2, #1
	ldr	r3, [r3, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200094a
	adds	r0, r6, #0
	bl 0x02008d58
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_02000956
	movs	r0, #175
	bl 0x02008e78
.L_0200094a:
	ldr	r5, [pc, #348]
	movs	r2, #2
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200095e
.L_02000956:
	movs	r0, #113
	bl 0x02008e78
	b.n	.L_02000a6c
.L_0200095e:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000976
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #1
	mov	sl, r3
	bl 0x02008e78
.L_02000976:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000990
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #1
	mov	sl, r3
	bl 0x02008e78
.L_02000990:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020009a8
	movs	r2, #1
	movs	r0, #111
	mov	r8, r2
	adds	r6, #10
	mov	sl, r2
	bl 0x02008e78
.L_020009a8:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020009c2
	movs	r3, #255
	movs	r2, #1
	movs	r0, #111
	mov	r8, r3
	subs	r6, #10
	mov	sl, r2
	bl 0x02008e78
.L_020009c2:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020009dc
	movs	r3, #1
	movs	r0, #111
	mov	r8, r3
	adds	r6, #30
	mov	sl, r3
	bl 0x02008e78
.L_020009dc:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020009f8
	movs	r2, #255
	movs	r3, #1
	movs	r0, #111
	mov	r8, r2
	subs	r6, #30
	mov	sl, r3
	bl 0x02008e78
.L_020009f8:
	mov	r2, r8
	lsls	r5, r2, #24
	movs	r2, #1
	asrs	r3, r5, #24
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000a32
	movs	r3, #250
	lsls	r3, r3, #1
	movs	r1, #250
	adds	r0, r6, r3
	b.n	.L_02000a18
.L_02000a10:
	movs	r2, #244
	adds	r2, #255
	movs	r1, #250
	adds	r0, r6, r2
.L_02000a18:
	lsls	r1, r1, #1
	bl 0x02008c60
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r6
	bl 0x02008d48
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_02000a10
.L_02000a32:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r5, r3
	bne.n	.L_02000a66
	movs	r2, #250
	lsls	r2, r2, #1
	movs	r1, #250
	adds	r0, r6, r2
	b.n	.L_02000a4c
.L_02000a44:
	movs	r3, #246
	adds	r3, #255
	movs	r1, #250
	adds	r0, r6, r3
.L_02000a4c:
	lsls	r1, r1, #1
	bl 0x02008c60
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r6
	bl 0x02008d48
	ldrh	r3, [r0, #6]
	cmp	r3, #0
	beq.n	.L_02000a44
.L_02000a66:
	movs	r2, #0
	mov	r8, r2
	b.n	.L_020008ac
.L_02000a6c:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02008ca8
	mov	r0, r9
	movs	r1, #2
	bl 0x02008ca8
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x050001e8
	.4byte 0x02008e84
	.4byte 0x02008e90
	.4byte 0x0000025f
	.4byte 0x00000092
	.4byte 0x02008ea8
	.2byte 0x1150
	.2byte 0x0300
	push	{lr}
	bl 0x02008d28
	pop	{pc}
	push	{lr}
	bl 0x02008d30
	pop	{pc}
	push	{lr}
	sub	sp, #8
	adds	r4, r3, #0
	cmp	r0, #1
	bne.n	.L_02000ada
	ldr	r3, [sp, #16]
	adds	r0, r1, #0
	str	r3, [sp, #0]
	ldr	r3, [sp, #20]
	adds	r1, r2, #0
	str	r3, [sp, #4]
	adds	r2, r4, #0
	ldr	r3, [sp, #12]
	bl 0x02008c90
.L_02000ada:
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	mov	sl, r0
	adds	r0, r6, #0
	adds	r7, r2, #0
	mov	r8, r3
	bl 0x02008df0
	mov	r2, sl
	adds	r5, r0, #0
	cmp	r2, #1
	bne.n	.L_02000b10
	mov	r3, r8
	lsls	r2, r3, #16
	lsls	r1, r7, #16
	adds	r0, r6, #0
	bl 0x02008df8
	movs	r3, #0
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02000b10:
	mov	r2, sl
	cmp	r2, #2
	bne.n	.L_02000b46
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_02000b3e
.L_02000b20:
	movs	r0, #1
	bl 0x02008c68
	ldr	r3, [r5, #24]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #30
	adds	r3, r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	cmp	r3, r2
	ble.n	.L_02000b20
.L_02000b3e:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02000b46:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	mov	sl, r0
	adds	r0, r7, #0
	mov	r8, r3
	adds	r5, r2, #0
	bl 0x02008df0
	mov	r3, sl
	adds	r6, r0, #0
	cmp	r3, #2
	bne.n	.L_02000b96
	mov	r3, r8
	lsls	r2, r3, #16
	adds	r0, r7, #0
	lsls	r1, r5, #16
	bl 0x02008df8
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r6, #0
	str	r3, [r6, #12]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #10
	str	r3, [r6, #72]
	movs	r0, #50
	bl 0x02008c68
.L_02000b96:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	cmp	r6, #1
	bne.n	.L_02000bd6
	movs	r0, #177
	lsls	r3, r2, #16
	lsls	r1, r7, #16
	lsls	r0, r0, #1
	movs	r2, #0
	bl 0x02008c88
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000bd6
	movs	r1, #0
	bl 0x02008c98
	adds	r2, r5, #0
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
.L_02000bd6:
	cmp	r6, #2
	bne.n	.L_02000c52
	mov	r2, r8
	lsls	r3, r2, #16
	lsls	r1, r7, #16
	movs	r0, #252
	movs	r2, #0
	bl 0x02008c88
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000c52
	movs	r1, #0
	bl 0x02008c98
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r5, #24]
	str	r3, [r5, #28]
.L_02000c04:
	movs	r0, #1
	bl 0x02008c68
	ldr	r2, [r5, #24]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #30
	adds	r2, r2, r3
	ldrh	r3, [r5, #6]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	strh	r3, [r5, #6]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	str	r2, [r5, #24]
	str	r2, [r5, #28]
	cmp	r2, r3
	ble.n	.L_02000c04
	adds	r3, #1
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	ldr	r3, [pc, #40]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x02008df0
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
	strh	r3, [r5, #6]
	mov	r0, sl
	ldr	r1, [sp, #24]
	bl 0x02008e20
.L_02000c52:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.irp EntryTarget, 0x03000508, 0x080000c1, 0x080003c1, 0x080003c9, 0x080003d1, 0x080200c1, 0x080201e1, 0x08020219, 0x08038011, 0x08038019, 0x08038039, 0x08038041, 0x08038049, 0x08038061, 0x08038079, 0x08038081, 0x08038091, 0x08038099, 0x080380a9, 0x080380b1, 0x080380f9, 0x08038121, 0x08038141, 0x08038261, 0x080382f9, 0x08038319, 0x08038321, 0x08038341, 0x080ad009, 0x080ad011, 0x080ad021, 0x080ad029, 0x080ad059, 0x080ad0c1, 0x080ad0c9, 0x080ad101, 0x080ad111, 0x080ad151, 0x080ad159, 0x080ad199, 0x080ad1d9, 0x080ad1f9, 0x080ad241, 0x080ad249, 0x080ad2b9, 0x080ad2f1, 0x080ad2f9, 0x080c8011, 0x080c8061, 0x080c8071, 0x080c8089, 0x080c80f9, 0x080c8119, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c8281, 0x080c83e1, 0x080c8779, 0x080f8039, 0x08108009, 0x08108011, 0x08108019, 0x08108021, 0x08108071, 0x08108099, 0x081080a1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x0000764c
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0xc0000138
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0001
	.4byte 0x00000268
	.4byte 0x40000148
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x00000288
	.4byte 0xc0000098
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x40000058
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc0000198
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0x400000e8
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000142
	.4byte 0x01402142
	.4byte 0x000001ff
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff0046
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0141
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0128
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff00f3
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0019
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008191
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200819d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020081a9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020081b5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020081c1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020081cd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020081d5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020082a9
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200812d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008145
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200815d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008179
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02008395
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020083a5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008669
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x02008aad
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008ab5
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008749
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008849
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x02008521
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x020083b9
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x02008639
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x02008665
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008665
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x02008665
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte 0x02008665
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
