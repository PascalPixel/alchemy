.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008b19, 0x02008039, 0x02008045, 0x0200804d, 0x02008a3d, 0x02008041, 0x02008fa1
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xcd14
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xcda4
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02000060
	ldr	r0, [pc, #4]
	b.n	.L_02000062
.L_02000060:
	ldr	r0, [pc, #4]
.L_02000062:
	pop	{pc}
	.4byte 0x0200cfe4
	.2byte 0xcdbc
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	cmp	r6, #0
	bne.n	.L_0200007a
	bl 0x0200c004
.L_0200007a:
	cmp	r5, #1
	bne.n	.L_0200008c
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200ca04
	bl 0x02008fa4
.L_0200008c:
	cmp	r5, #2
	bne.n	.L_020000a2
	movs	r0, #249
	movs	r1, #128
	movs	r2, #206
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	movs	r3, #2
	bl 0x0200bf48
.L_020000a2:
	cmp	r5, #3
	bne.n	.L_020000b8
	movs	r0, #249
	movs	r1, #128
	movs	r2, #206
	lsls	r0, r0, #17
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	movs	r3, #30
	bl 0x0200bf48
.L_020000b8:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_020000c6
	bl 0x0200c0c0
.L_020000c6:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #188]
	adds	r1, r5, #0
	bl 0x0200c868
	cmp	r5, #1
	bne.n	.L_02000188
	movs	r0, #19
	bl 0x0200caac
	adds	r6, r0, #0
	movs	r0, #21
	bl 0x0200caac
	movs	r2, #128
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r2, r2, #11
	lsls	r3, r3, #8
	str	r2, [r6, #48]
	str	r3, [r6, #52]
	str	r3, [r5, #52]
	str	r2, [r5, #48]
	movs	r1, #244
	movs	r3, #156
	ldr	r2, [r6, #12]
	adds	r0, r6, #0
	lsls	r1, r1, #17
	lsls	r3, r3, #17
	bl 0x0200ca2c
	movs	r1, #220
	movs	r3, #156
	adds	r0, r6, #0
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #17
	bl 0x0200ca34
	movs	r1, #244
	movs	r3, #156
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	lsls	r1, r1, #17
	lsls	r3, r3, #17
	bl 0x0200ca2c
	movs	r1, #220
	movs	r3, #188
	lsls	r1, r1, #17
	movs	r2, #0
	lsls	r3, r3, #17
	adds	r0, r5, #0
	bl 0x0200ca34
	movs	r0, #1
	bl 0x0200cc8c
	movs	r0, #2
	bl 0x0200cc8c
	movs	r7, #0
.L_02000148:
	movs	r1, #18
	movs	r2, #19
	movs	r0, #1
	bl 0x0200cc24
	movs	r1, #20
	movs	r2, #21
	movs	r0, #2
	bl 0x0200cc24
	movs	r0, #1
	bl 0x0200c994
	movs	r0, #1
	bl 0x0200cc8c
	movs	r0, #2
	bl 0x0200cc8c
	adds	r0, r6, #0
	bl 0x0200ca7c
	cmp	r0, #0
	beq.n	.L_02000182
	adds	r0, r5, #0
	bl 0x0200ca7c
	cmp	r0, #0
	bne.n	.L_02000188
.L_02000182:
	adds	r7, #1
	cmp	r7, #59
	ble.n	.L_02000148
.L_02000188:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xcd0c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #104]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	adds	r5, r1, #0
	bl 0x0200caac
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200caac
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	cmp	r5, #10
	bne.n	.L_020001fa
	ldr	r3, [r6, #8]
	asrs	r7, r3, #20
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	cmp	r6, #34
	bne.n	.L_020001de
	cmp	r5, #32
	bne.n	.L_020001de
	cmp	r7, #33
	bne.n	.L_020001de
	movs	r0, #132
	movs	r1, #128
	lsls	r0, r0, #18
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #255
	bl 0x0200cc7c
.L_020001de:
	cmp	r6, #33
	bne.n	.L_020001fa
	cmp	r5, #32
	bne.n	.L_020001fa
	cmp	r7, #32
	bne.n	.L_020001fa
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #18
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #255
	bl 0x0200cc7c
.L_020001fa:
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r1
	mov	r0, sl
	sub	sp, #16
	bl 0x0200caac
	adds	r7, r0, #0
	ldr	r3, [r7, #8]
	movs	r2, #0
	asrs	r6, r3, #20
	ldr	r3, [r7, #16]
	mov	r8, r2
	asrs	r5, r3, #20
	mov	r3, sl
	cmp	r3, #10
	bne.n	.L_02000246
	movs	r0, #132
	movs	r1, #128
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #18
	bl 0x0200cc7c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #18
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #0
	bl 0x0200cc7c
.L_02000246:
	cmp	r6, #31
	bne.n	.L_0200025a
	cmp	r5, #32
	bne.n	.L_0200025a
	movs	r0, #138
	lsls	r0, r0, #4
	bl 0x0200ca04
	movs	r2, #1
	mov	r8, r2
.L_0200025a:
	cmp	r6, #25
	bne.n	.L_02000270
	cmp	r5, #33
	bne.n	.L_02000270
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x0200ca04
	movs	r3, #1
	mov	r8, r3
.L_02000270:
	cmp	r6, #21
	bne.n	.L_02000286
	cmp	r5, #33
	bne.n	.L_02000286
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200ca04
	movs	r2, #1
	mov	r8, r2
.L_02000286:
	cmp	r6, #27
	bne.n	.L_0200029c
	cmp	r5, #33
	bne.n	.L_0200029c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200ca04
	movs	r3, #1
	mov	r8, r3
.L_0200029c:
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_02000386
	mov	r3, sl
	cmp	r3, #10
	beq.n	.L_020002b0
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200cc9c
.L_020002b0:
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #10
	adds	r2, r7, #0
	adds	r2, #85
	str	r3, [r7, #72]
	movs	r3, #3
	strb	r3, [r2, #0]
	mov	r0, sl
	bl 0x0200caac
	movs	r1, #1
	bl 0x0200ca6c
	movs	r0, #5
	bl 0x0200c994
	ldr	r3, [r7, #12]
	movs	r6, #0
	cmp	r3, #0
	ble.n	.L_020002ec
.L_020002da:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200c994
	cmp	r6, #59
	bgt.n	.L_020002ec
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bgt.n	.L_020002da
.L_020002ec:
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200cc9c
	movs	r0, #1
	bl 0x0200c994
	movs	r0, #240
	bl 0x0200cc9c
	adds	r2, r7, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200ca0c
	movs	r6, #0
.L_0200031e:
	movs	r0, #138
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #1
	bl 0x0200ca1c
	mov	r8, sp
	mov	r2, r8
	adds	r5, r0, #0
	lsls	r3, r6, #2
	str	r5, [r2, r3]
	movs	r3, #166
	lsls	r3, r3, #8
	adds	r3, #102
	str	r3, [r5, #24]
	movs	r1, #1
	bl 0x0200ca0c
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200ca6c
	movs	r0, #154
	bl 0x0200cc9c
	adds	r6, #1
	movs	r0, #5
	bl 0x0200c994
	cmp	r6, #2
	ble.n	.L_0200031e
	bl 0x02008fa4
	ldr	r3, [pc, #44]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200cacc
	movs	r0, #30
	bl 0x0200c994
	mov	r5, r8
	movs	r6, #2
.L_0200037a:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl 0x0200ca24
	cmp	r6, #0
	bge.n	.L_0200037a
.L_02000386:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x02008190
	movs	r0, #160
	movs	r1, #232
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #160
	movs	r1, #240
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #160
	movs	r1, #248
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #160
	movs	r1, #128
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #232
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #240
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #248
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #128
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #168
	movs	r1, #132
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	pop	{pc}
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl 0x0200caac
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	movs	r5, #0
	cmp	r2, #21
	bne.n	.L_0200044c
	cmp	r3, #33
	bne.n	.L_0200044c
	movs	r5, #1
.L_0200044c:
	movs	r0, #160
	movs	r1, #232
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #160
	movs	r1, #240
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #160
	movs	r1, #248
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #160
	movs	r1, #128
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #232
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #240
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #248
	lsls	r1, r1, #17
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #128
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #168
	movs	r1, #132
	lsls	r0, r0, #17
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #255
	bl 0x0200cc7c
	cmp	r5, #0
	beq.n	.L_020004f2
	movs	r0, #168
	movs	r1, #1
	movs	r2, #140
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200cb8c
.L_020004f2:
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl 0x02008200
	cmp	r5, #0
	beq.n	.L_0200050e
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200cb7c
.L_0200050e:
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #15
	sub	sp, #32
	bl 0x0200caac
	adds	r7, r0, #0
	movs	r0, #16
.L_02000528:
	bl 0x0200caac
	ldr	r3, [pc, #432]
	mov	sl, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200caac
	ldrh	r3, [r0, #6]
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r6, r3, r1
	movs	r3, #192
	lsls	r3, r3, #8
	mov	r2, sl
	ands	r6, r3
	cmp	r2, #0
	beq.n	.L_0200056c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_0200056c
	mov	r3, sl
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #16]
	movs	r2, #2
	movs	r3, #255
	bl 0x0200cc7c
.L_0200056c:
	add	r5, sp, #8
	adds	r0, r5, #0
	bl 0x0200c240
	cmp	r0, #0
	bne.n	.L_0200057a
	b.n	.L_020006b2
.L_0200057a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200c9fc
	add	r1, sp, #24
	mov	r8, r1
	cmp	r0, #0
	beq.n	.L_02000594
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r6, r2
	bne.n	.L_020005aa
.L_02000594:
	mov	r2, sp
	mov	r3, r8
	ldmia	r3!, {r0, r1}
	stmia	r2!, {r0, r1}
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #12]
	bl 0x0200c4c4
	b.n	.L_020006b2
.L_020005aa:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #166
	bl 0x0200ca04
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	movs	r0, #168
	movs	r1, #1
	movs	r2, #140
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #1
	bl 0x0200cb8c
	mov	r3, r8
	mov	r2, sp
	ldmia	r3!, {r0, r1}
	stmia	r2!, {r0, r1}
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #12]
	bl 0x0200c4c4
	ldr	r5, [pc, #252]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	adds	r0, r0, r5
	movs	r2, #0
	movs	r3, #0
	bl 0x0200cc7c
	ldr	r0, [r7, #8]
	movs	r6, #128
	lsls	r6, r6, #12
	ldr	r1, [r7, #16]
	movs	r2, #0
	movs	r3, #0
	adds	r0, r0, r6
	bl 0x0200cc7c
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	adds	r0, r0, r5
	movs	r2, #2
	movs	r3, #0
	bl 0x0200cc7c
	ldr	r0, [r7, #8]
	ldr	r2, [pc, #208]
	ldr	r1, [r7, #16]
	adds	r0, r0, r2
	movs	r3, #0
	movs	r2, #2
	bl 0x0200cc7c
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #16]
	movs	r2, #2
	movs	r3, #0
	adds	r0, r0, r6
	bl 0x0200cc7c
	ldr	r0, [r7, #8]
	movs	r3, #192
	lsls	r3, r3, #13
	movs	r2, #2
	adds	r0, r0, r3
	ldr	r1, [r7, #16]
	movs	r3, #0
	bl 0x0200cc7c
	movs	r0, #15
	movs	r1, #3
	bl 0x0200cb0c
	movs	r2, #16
	movs	r0, #15
	movs	r1, #0
	bl 0x0200caf4
	movs	r0, #20
	bl 0x0200ca8c
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #204
	adds	r2, r7, #0
	str	r3, [r7, #72]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r5, #0
	cmp	r3, #0
	ble.n	.L_02000686
.L_02000674:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200c994
	cmp	r5, #59
	bgt.n	.L_02000686
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bgt.n	.L_02000674
.L_02000686:
	bl 0x02008fa4
	movs	r0, #240
	bl 0x0200cc9c
	movs	r1, #8
	movs	r0, #15
	bl 0x0200cb0c
	movs	r0, #60
	bl 0x0200ca8c
	ldr	r3, [pc, #64]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200cb7c
	bl 0x0200ca9c
.L_020006b2:
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_020006d4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_020006d4
	mov	r2, sl
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #16]
	movs	r3, #0
	movs	r2, #2
	bl 0x0200cc7c
.L_020006d4:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xffe80000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb520
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
.L_020006f8:
	ldr	r0, [r3, #0]
	bl 0x0200cbb4
	cmp	r0, #16
	bne.n	.L_02000720
	adds	r0, r5, #0
	movs	r1, #16
	bl 0x02008394
	bl 0x0200cc44
	cmp	r0, #0
	beq.n	.L_02000716
	bl 0x0200cc4c
.L_02000716:
	adds	r0, r5, #0
	movs	r1, #16
	bl 0x0200842c
	b.n	.L_02000728
.L_02000720:
	cmp	r0, #15
	bne.n	.L_02000728
	bl 0x02008514
.L_02000728:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #168
	bl 0x0200ca04
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #232]
	bl 0x0200cb3c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #22
	bl 0x0200cb6c
	ldr	r3, [pc, #216]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb34
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #2
	movs	r0, #22
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #0
	movs	r0, #22
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	bne.n	.L_020007f0
	movs	r5, #192
	movs	r0, #20
	lsls	r5, r5, #18
	bl 0x0200ca8c
	ldr	r3, [r5, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	movs	r1, #0
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r0, #22
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	bne.n	.L_020007f0
	movs	r0, #20
	bl 0x0200ca8c
	ldr	r3, [r5, #108]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r2, [r3, #0]
	movs	r1, #0
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r0, #22
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	beq.n	.L_02000802
.L_020007f0:
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb4c
	b.n	.L_02000820
.L_02000802:
	movs	r0, #20
	bl 0x0200ca8c
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #22
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb4c
.L_02000820:
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	bl 0x0200ca9c
	pop	{r5, pc}
	.4byte 0x00001b58
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02000866
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #4
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_020008c8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #4
	bl 0x0200ca04
	bl 0x02009cb0
	b.n	.L_020008c8
.L_02000866:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_020008ac
	movs	r0, #138
	lsls	r0, r0, #4
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_020008c8
	movs	r0, #151
	bl 0x0200ca84
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_020008c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #173
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_020008c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #173
	bl 0x0200ca04
	bl 0x020097ec
	b.n	.L_020008c8
.L_020008ac:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #169
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_020008c8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #169
	bl 0x0200ca04
	bl 0x0200912c
.L_020008c8:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	movs	r0, #158
	bl 0x0200cc9c
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl 0x0200ca44
	ldr	r5, [pc, #76]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200caac
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	bl 0x0200cab4
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200cb0c
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x0200caec
	movs	r0, #10
	bl 0x0200ca8c
	adds	r0, r6, #0
	bl 0x0200cbac
	bl 0x0200cbd4
	bl 0x0200cbdc
	bl 0x0200ca9c
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #8]
	bl 0x020088cc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x0200d290
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200ca04
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	b.n	.L_02000a08
.L_0200096c:
	movs	r0, #151
	bl 0x0200ca84
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_02000a32
	movs	r0, #248
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200ca04
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	movs	r1, #244
	movs	r2, #220
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200cae4
	movs	r1, #16
	movs	r3, #0
	movs	r2, #0
	movs	r0, #5
	negs	r1, r1
	bl 0x0200cbec
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200cb5c
	movs	r0, #20
	bl 0x0200ca8c
	ldr	r0, [pc, #120]
	bl 0x0200cb3c
	movs	r2, #5
	movs	r0, #5
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r3, [pc, #100]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_020009f2
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200cad4
.L_020009f2:
	movs	r0, #5
	bl 0x0200cafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	bl 0x0200ca9c
	b.n	.L_02000a32
.L_02000a08:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02000a32
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02000a32
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #190
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_0200096c
.L_02000a32:
	pop	{pc}
	.4byte 0x00001cef
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x0200d298
	.4byte 0x21804b11
	.4byte 0x2301781a
	.4byte 0x00984053
	.4byte 0x4b0f18c0
	.4byte 0x18c00180
	.4byte 0x04c98803
	.4byte 0x800b3116
	.4byte 0x04db2380
	.4byte 0x895c33b0
	.4byte 0x021222c5
	.4byte 0x402232ff
	.4byte 0x22fe815a
	.4byte 0x01d2895c
	.4byte 0x402232ff
	.4byte 0x3002815a
	.4byte 0x4a04895a
	.4byte 0x3b0cc307
	.4byte 0x00004770
	.4byte 0x0200d4f0
	.4byte 0x0200d6e0
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	movs	r3, #192
.L_02000a9c:
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r7, r3, r2
	ldr	r3, [pc, #68]
	movs	r6, #0
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [pc, #56]
	lsls	r2, r2, #6
	adds	r5, r2, r3
.L_02000aba:
	ldr	r3, [pc, #56]
	ldrb	r0, [r3, #0]
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	subs	r0, r0, r6
	subs	r0, r0, r3
	adds	r0, #160
	lsls	r0, r0, #9
	bl 0x0200c9b4
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	asrs	r0, r0, #15
	adds	r3, r3, r0
	adds	r6, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r6, #160
	bne.n	.L_02000aba
	ldr	r3, [pc, #8]
	movs	r1, #1
	ldrb	r2, [r3, #0]
	eors	r2, r1
	strb	r2, [r3, #0]
	pop	{r5, r6, r7, pc}
	.4byte 0x0200d4f0
	.4byte 0x0200d6e0
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200c99c
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl 0x0200c99c
	pop	{pc}
	.4byte 0x02008a99
	.2byte 0x8a45
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	sub	sp, #8
	bl 0x02008af8
	ldr	r3, [pc, #912]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200caac
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
.L_02000b48:
	strb	r3, [r0, #0]
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02000b5a
	b.n	.L_02000cd6
.L_02000b5a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #5
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02000c5c
	movs	r0, #129
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02000c5c
	movs	r1, #184
	movs	r2, #154
	movs	r0, #30
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #136
	movs	r2, #150
	movs	r0, #31
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #168
	movs	r2, #146
	movs	r0, #28
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #128
	movs	r0, #28
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #30
	movs	r1, #0
	movs	r2, #13
	bl 0x0200cc5c
	movs	r1, #128
	movs	r0, #31
	lsls	r1, r1, #8
	movs	r2, #13
	bl 0x0200cc5c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #13
	bl 0x0200cc5c
	movs	r1, #224
	movs	r2, #216
	movs	r0, #25
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r1, #176
	movs	r2, #204
	movs	r0, #27
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r1, #152
	movs	r2, #188
	movs	r0, #29
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r1, #128
	movs	r2, #0
	movs	r0, #29
	lsls	r1, r1, #7
	bl 0x0200cb54
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200cc54
	movs	r0, #25
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200cc54
	movs	r0, #27
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x0200cc54
	movs	r0, #29
.L_02000c3a:
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r0, #25
	movs	r1, #13
	bl 0x0200cb0c
	movs	r0, #27
	movs	r1, #13
	bl 0x0200cb0c
	movs	r0, #29
	movs	r1, #13
	bl 0x0200cb0c
.L_02000c5c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #5
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02000cd6
	movs	r3, #6
	str	r3, [sp, #4]
	movs	r5, #11
	movs	r0, #40
	movs	r1, #0
	movs	r2, #24
	movs	r3, #31
	str	r5, [sp, #0]
	bl 0x0200ca5c
	movs	r3, #75
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #84
	movs	r1, #15
	movs	r2, #10
	movs	r3, #10
	bl 0x0200ca5c
	movs	r3, #24
	str	r3, [sp, #4]
	movs	r0, #10
	movs	r3, #1
	movs	r1, #24
	movs	r2, #1
	str	r5, [sp, #0]
	bl 0x0200ca54
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
.L_02000cd6:
	ldr	r2, [pc, #492]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r1, r2, r3
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #99
	beq.n	.L_02000ce8
	b.n	.L_02000e2e
.L_02000ce8:
	movs	r3, #245
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	strh	r3, [r1, #0]
	movs	r2, #138
	movs	r1, #136
	movs	r0, #4
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #240
	movs	r2, #138
	movs	r0, #26
	lsls	r1, r1, #15
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #184
	movs	r2, #154
	movs	r0, #30
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #136
	movs	r2, #150
	movs	r0, #31
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #160
	movs	r2, #146
	lsls	r2, r2, #18
	movs	r0, #28
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #30
	bl 0x0200cc54
	movs	r0, #30
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #31
	bl 0x0200cc54
	movs	r0, #31
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200cc54
	movs	r0, #28
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r0, #27
	movs	r1, #2
	bl 0x0200cb64
	movs	r0, #25
	movs	r1, #2
	bl 0x0200cb64
	movs	r0, #29
	movs	r1, #2
	bl 0x0200cb64
	movs	r1, #160
	movs	r2, #196
	movs	r0, #25
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r1, #176
	movs	r2, #204
	movs	r0, #27
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r1, #152
	movs	r2, #188
	lsls	r2, r2, #17
	movs	r0, #29
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200cc54
	movs	r0, #27
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x0200cc54
	movs	r0, #29
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r1, #208
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #0
	movs	r0, #29
	lsls	r1, r1, #7
	bl 0x0200cb54
	movs	r0, #27
	movs	r1, #13
	bl 0x0200cb0c
	movs	r0, #29
	movs	r1, #13
	bl 0x0200cb0c
	movs	r0, #30
	movs	r1, #13
	bl 0x0200cb0c
	movs	r0, #31
	movs	r1, #13
	bl 0x0200cb0c
	movs	r0, #28
	movs	r1, #13
	bl 0x0200cb0c
	bl 0x0200b274
.L_02000e2e:
	bl 0x02008fa4
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #166
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02000e46
	movs	r0, #15
	bl 0x0200c0ec
.L_02000e46:
	bl 0x0200cc14
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #163
	movs	r2, #8
	movs	r3, #9
	movs	r0, #0
	bl 0x0200cc2c
	movs	r0, #8
	bl 0x0200caac
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x0200caac
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	bl 0x0200cc1c
	movs	r2, #13
	movs	r1, #12
	movs	r0, #0
	bl 0x0200cc24
	movs	r0, #12
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r0, #13
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	ldr	r0, [pc, #40]
	bl 0x0200c7fc
	movs	r0, #14
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #162
	bl 0x0200c9fc
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_02000f28
	b.n	.L_02000ecc
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xcd0c
	.2byte 0x0200
.L_02000ecc:
	movs	r0, #18
	bl 0x0200caac
	adds	r2, r0, #0
	adds	r2, #89
	ldrb	r3, [r2, #0]
	movs	r6, #8
	orrs	r3, r6
	strb	r3, [r2, #0]
	movs	r0, #19
	bl 0x0200caac
	movs	r5, #128
	adds	r3, r0, #0
	adds	r3, #85
	lsls	r5, r5, #13
	strb	r7, [r3, #0]
	movs	r1, #18
	movs	r2, #19
	str	r5, [r0, #12]
	movs	r0, #1
	bl 0x0200cc24
	movs	r0, #1
	bl 0x0200cc94
	movs	r0, #20
	bl 0x0200caac
	adds	r2, r0, #0
	adds	r2, #89
	ldrb	r3, [r2, #0]
	movs	r0, #21
	orrs	r3, r6
	strb	r3, [r2, #0]
	bl 0x0200caac
	adds	r3, r0, #0
	adds	r3, #85
	strb	r7, [r3, #0]
	movs	r1, #20
	str	r5, [r0, #12]
	movs	r2, #21
	movs	r0, #2
	bl 0x0200cc24
.L_02000f28:
	movs	r0, #16
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r0, #17
	bl 0x0200caac
	movs	r1, #0
	bl 0x0200ca6c
	movs	r0, #10
	movs	r1, #2
	bl 0x0200cb64
	movs	r0, #23
	bl 0x0200caac
	movs	r3, #1
	adds	r0, #98
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02000f98
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02000f98
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
.L_02000f98:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #166
	sub	sp, #8
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001022
	movs	r0, #15
	bl 0x0200caac
	movs	r1, #168
	movs	r2, #134
	adds	r5, r0, #0
	lsls	r2, r2, #18
	movs	r0, #15
	lsls	r1, r1, #17
	bl 0x0200cb04
	movs	r0, #15
	movs	r1, #4
	bl 0x0200cb0c
	adds	r0, r5, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	adds	r2, r5, #0
	strb	r3, [r0, #0]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #316]
	movs	r1, #0
	str	r1, [r5, #40]
	movs	r0, #168
	movs	r1, #132
	str	r3, [r5, #12]
	str	r3, [r5, #20]
	lsls	r1, r1, #18
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #168
	movs	r1, #132
	lsls	r1, r1, #18
	movs	r2, #0
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200cc7c
	movs	r0, #176
	movs	r1, #132
	lsls	r0, r0, #17
	lsls	r1, r1, #18
	movs	r2, #0
	movs	r3, #0
	bl 0x0200cc7c
.L_02001022:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #164
	bl 0x0200c9fc
	cmp	r0, #0
.L_0200102e:
	beq.n	.L_0200103a
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
.L_0200103a:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #165
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001052
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
.L_02001052:
	movs	r0, #138
	lsls	r0, r0, #4
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_020010a8
	movs	r0, #10
	bl 0x0200caac
	movs	r1, #252
	movs	r2, #130
	adds	r5, r0, #0
	lsls	r2, r2, #18
	movs	r0, #10
	lsls	r1, r1, #17
	bl 0x0200cb04
	movs	r0, #10
	movs	r1, #2
	bl 0x0200cb0c
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r1, [r5, #16]
	movs	r3, #0
	ldr	r0, [r5, #8]
	bl 0x0200cc7c
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200cc84
.L_020010a8:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #161
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001100
	movs	r0, #11
	bl 0x0200caac
	movs	r1, #204
	movs	r2, #134
	adds	r5, r0, #0
	lsls	r2, r2, #18
	movs	r0, #11
	lsls	r1, r1, #17
	bl 0x0200cb04
	movs	r0, #11
	movs	r1, #2
	bl 0x0200cb0c
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r1, [r5, #16]
	movs	r3, #0
	ldr	r0, [r5, #8]
	bl 0x0200cc7c
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200cc84
.L_02001100:
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001122
	movs	r3, #30
	movs	r2, #24
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #32
	movs	r1, #24
	movs	r2, #2
	movs	r3, #2
	bl 0x0200ca4c
.L_02001122:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb520
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #1008]
	bl 0x0200cb3c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #196
	movs	r0, #4
	movs	r1, #232
	lsls	r2, r2, #1
	bl 0x0200cae4
	movs	r2, #16
	movs	r3, #128
	movs	r0, #26
	movs	r1, #0
	negs	r2, r2
	lsls	r3, r3, #6
	bl 0x0200cbec
	movs	r3, #128
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	lsls	r3, r3, #6
	bl 0x0200cbec
	movs	r3, #128
	lsls	r3, r3, #6
	movs	r0, #6
	movs	r1, #0
	movs	r2, #16
	bl 0x0200cbec
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl 0x0200cbfc
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #22
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #22
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #1
	movs	r0, #23
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #23
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r0, #24
	lsls	r1, r1, #6
	movs	r2, #0
.L_02001216:
	bl 0x0200cb54
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #11
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #1
	movs	r0, #24
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #24
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #24
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #25
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #14
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #1
	movs	r0, #22
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
.L_02001304:
	movs	r1, #8
	adds	r1, #255
	movs	r2, #40
	movs	r0, #22
	bl 0x0200cb6c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #244
	movs	r2, #208
	movs	r3, #1
	movs	r1, #0
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #23
	bl 0x0200cb6c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200cb74
	movs	r0, #40
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #196
	movs	r1, #128
	movs	r2, #208
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r1, r1, #13
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #24
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb6c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb6c
	movs	r1, #6
	movs	r2, #50
	adds	r1, #255
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #25
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #20
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #22
	movs	r1, #3
	bl 0x0200cb0c
	movs	r0, #23
	movs	r1, #3
	bl 0x0200cb0c
	movs	r1, #3
	movs	r0, #24
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #160
	movs	r0, #23
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	ldr	r3, [pc, #176]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200cb9c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #148]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_020014c6
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200cad4
.L_020014c6:
	movs	r0, #5
	bl 0x0200cafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #84]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #6
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001504
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200cad4
.L_02001504:
	movs	r0, #6
	bl 0x0200cafc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #26
	ldr	r1, [pc, #24]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #26
	movs	r1, #2
	bl 0x0200cb0c
	b.n	.L_02001538
	.4byte 0x00001b4c
	.4byte 0x02000240
	.2byte 0x3333
	.2byte 0x0001
.L_02001538:
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001550
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #26
	bl 0x0200cad4
.L_02001550:
	movs	r0, #26
	bl 0x0200cafc
	movs	r2, #0
	movs	r0, #26
	movs	r1, #0
	bl 0x0200cb04
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200cb7c
	bl 0x0200cb94
	bl 0x0200ca9c
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02001586
	b.n	.L_020017dc
.L_02001586:
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001596
	b.n	.L_020017dc
.L_02001596:
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #572]
	bl 0x0200cb3c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #172
	bl 0x0200ca04
	movs	r1, #132
	movs	r2, #224
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200cae4
	movs	r2, #16
	movs	r3, #128
	movs	r0, #26
	movs	r1, #0
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200cbec
	movs	r3, #128
	movs	r0, #5
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200cbec
	movs	r2, #16
	movs	r3, #192
	movs	r0, #6
	movs	r1, #16
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200cbec
	movs	r0, #7
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001604
	movs	r3, #192
	movs	r0, #7
	movs	r1, #8
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x0200cbec
.L_02001604:
	movs	r0, #6
	bl 0x0200cafc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #26
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
.L_02001628:
	movs	r0, #26
	movs	r1, #0
.L_0200162c:
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200cb6c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #4
	bl 0x0200cb14
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200cb6c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #7
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001716
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #264]
	bl 0x0200cab4
	movs	r0, #7
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r3, [pc, #256]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001706
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200cad4
.L_02001706:
	movs	r0, #7
	bl 0x0200cafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
.L_02001716:
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #5
	ldr	r1, [pc, #196]
	bl 0x0200cab4
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r3, [pc, #184]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_0200174c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200cad4
.L_0200174c:
	movs	r0, #5
	bl 0x0200cafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #128]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #6
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_0200178a
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200cad4
.L_0200178a:
	movs	r0, #6
	bl 0x0200cafc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #26
	ldr	r1, [pc, #64]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #26
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_020017c8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #26
	bl 0x0200cad4
.L_020017c8:
	movs	r0, #26
	bl 0x0200cafc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	bl 0x0200ca9c
.L_020017dc:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001cdc
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #912]
	bl 0x0200cb3c
	movs	r2, #196
	movs	r0, #4
	movs	r1, #232
	lsls	r2, r2, #1
	bl 0x0200cae4
	movs	r2, #16
	movs	r3, #128
	movs	r0, #26
	movs	r1, #0
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200cbec
	movs	r3, #128
	movs	r0, #5
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200cbec
	movs	r2, #16
	movs	r3, #192
	movs	r0, #6
	movs	r1, #16
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200cbec
	movs	r0, #7
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001850
	movs	r3, #192
	movs	r0, #7
	movs	r1, #8
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x0200cbec
.L_02001850:
	movs	r0, #6
	bl 0x0200cafc
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #26
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #0
	movs	r0, #26
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	bne.n	.L_020018ea
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #6
	bl 0x0200cb6c
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200cb14
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200cb4c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_02001938
.L_020018ea:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #40
	adds	r3, #2
	strh	r3, [r2, #0]
	bl 0x0200ca8c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200cb14
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
.L_02001938:
	movs	r0, #7
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_0200196c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200cb5c
	movs	r0, #10
	bl 0x0200ca8c
	b.n	.L_02001a2e
.L_0200196c:
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200cb6c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #26
	bl 0x0200cb6c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #26
	bl 0x0200cb6c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
.L_020019f2:
	movs	r0, #6
	bl 0x0200cb54
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
.L_02001a2e:
	ldr	r0, [pc, #352]
	bl 0x0200cb3c
	movs	r1, #3
	movs	r0, #26
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200cb0c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200cb0c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200cb0c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #7
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02001ac2
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #268]
	bl 0x0200cab4
	movs	r0, #7
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r3, [pc, #260]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001ab2
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200cad4
.L_02001ab2:
	movs	r0, #7
	bl 0x0200cafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
.L_02001ac2:
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #5
	ldr	r1, [pc, #200]
	bl 0x0200cab4
	movs	r0, #5
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r3, [pc, #188]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001af8
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200cad4
.L_02001af8:
	movs	r0, #5
	bl 0x0200cafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #132]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #6
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001b36
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200cad4
.L_02001b36:
	movs	r0, #6
	bl 0x0200cafc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #26
	ldr	r1, [pc, #68]
	adds	r2, #153
	bl 0x0200cab4
	movs	r0, #26
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r0, [r5, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02001b74
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #26
	bl 0x0200cad4
.L_02001b74:
	movs	r0, #26
	bl 0x0200cafc
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	bl 0x0200ca9c
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001ce1
	.4byte 0x00001cea
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
	adds	r6, r2, #0
	adds	r5, r1, #0
	lsls	r3, r3, #16
	movs	r0, #244
	asrs	r7, r3, #16
	lsls	r0, r0, #1
	adds	r3, r6, #0
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl 0x0200ca1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02001be8
	movs	r1, #1
	ldr	r5, [r6, #80]
	bl 0x0200ca0c
	ldr	r1, [pc, #40]
	adds	r0, r6, #0
	bl 0x0200ca14
	movs	r1, #1
	adds	r0, r6, #0
	bl 0x0200ca74
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r3, [sp, #16]
	ldr	r1, [pc, #12]
	adds	r2, #9
	strh	r3, [r2, #0]
	strb	r1, [r5, #26]
	strh	r7, [r5, #18]
.L_02001be8:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xd6d4
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	bl 0x0200caac
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #12
	ldr	r0, [r5, #8]
	mov	sl, r3
	ldr	r1, [r5, #12]
	movs	r3, #224
	lsls	r3, r3, #13
	mov	r8, r3
	movs	r3, #128
	ldr	r2, [r5, #16]
	add	r1, r8
	lsls	r3, r3, #5
	movs	r6, #15
	add	r0, sl
	str	r6, [sp, #0]
	mov	fp, r3
	bl 0x02009b9c
	movs	r0, #151
	bl 0x0200cc9c
	movs	r0, #15
	bl 0x0200ca8c
	ldr	r0, [r5, #8]
	ldr	r3, [pc, #108]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
	movs	r3, #240
	ldr	r2, [r5, #16]
	add	r1, r8
	lsls	r3, r3, #8
	str	r6, [sp, #0]
	mov	r9, r3
	bl 0x02009b9c
	movs	r0, #151
	bl 0x0200cc9c
	movs	r0, #15
	bl 0x0200ca8c
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [r5, #16]
	add	r1, r8
	mov	r3, fp
	add	r0, sl
	str	r6, [sp, #0]
	bl 0x02009b9c
	movs	r0, #151
	bl 0x0200cc9c
	movs	r0, #15
	bl 0x0200ca8c
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r3, [pc, #40]
	ldr	r2, [r5, #16]
	add	r1, r8
	adds	r0, r0, r3
	mov	r3, r9
	str	r6, [sp, #0]
	bl 0x02009b9c
	movs	r0, #151
	bl 0x0200cc9c
	movs	r0, #15
	bl 0x0200ca8c
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb500
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #900]
	bl 0x0200cb3c
	movs	r1, #179
	movs	r2, #179
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #28
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #179
	movs	r2, #179
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #29
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #23
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #24
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #164
	movs	r2, #154
	movs	r0, #28
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #224
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #148
	movs	r2, #154
	movs	r0, #29
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #224
	movs	r0, #29
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #164
	movs	r1, #1
	movs	r2, #158
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200cb74
	movs	r0, #50
	bl 0x0200ca8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #29
	bl 0x0200cb6c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #28
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #28
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #29
	bl 0x0200cb54
	movs	r0, #20
.L_02001dd4:
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #29
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #29
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #160
	movs	r0, #28
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbf4
	movs	r1, #160
	movs	r0, #29
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbfc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r1, #0
	movs	r2, #0
	movs	r0, #29
	bl 0x0200cb04
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #132
	movs	r2, #192
	movs	r0, #23
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #140
	movs	r2, #196
	movs	r0, #24
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #232
	movs	r2, #192
	movs	r0, #4
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200cb04
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #132
	movs	r1, #1
	movs	r2, #196
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cbfc
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #0
	movs	r0, #23
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	bne.n	.L_02001f18
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cb4c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001f48
.L_02001f18:
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #4
	movs	r0, #23
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #23
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
.L_02001f48:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #24
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #24
	bl 0x0200cb6c
	movs	r0, #24
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r2, #23
	movs	r1, #6
	movs	r0, #24
	bl 0x0200cb1c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #24
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #24
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #8
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cbfc
	movs	r0, #4
	movs	r1, #23
	bl 0x0200cc04
	movs	r1, #96
	movs	r0, #24
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbf4
	movs	r1, #96
	movs	r0, #23
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbfc
	movs	r0, #23
	movs	r1, #0
	movs	r2, #80
	bl 0x0200cbf4
	movs	r1, #32
	movs	r0, #24
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbfc
	movs	r1, #0
	movs	r2, #80
	movs	r0, #24
	bl 0x0200cbfc
	movs	r0, #4
	bl 0x0200cacc
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r2, #0
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cb04
	movs	r0, #4
	movs	r1, #1
	bl 0x0200cb7c
	bl 0x0200cb94
	bl 0x0200ca9c
	pop	{pc}
	.2byte 0x24c1
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_0200205c
	bl 0x0200aa00
.L_0200205c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #5
	bl 0x0200ca04
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #1016]
	bl 0x0200cb3c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r1, #224
	movs	r2, #158
	movs	r0, #25
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r0, #25
	movs	r1, #0
.L_02002094:
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #224
	movs	r2, #162
	movs	r0, #27
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	movs	r2, #154
	movs	r0, #28
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #152
	movs	r0, #29
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #0
	movs	r2, #0
	movs	r0, #29
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #144
	movs	r1, #1
	movs	r2, #166
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	bl 0x0200cb8c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #28
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cab4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #29
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cab4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #25
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cab4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #27
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cab4
	movs	r0, #25
	movs	r1, #32
	movs	r2, #0
	bl 0x0200cbf4
	movs	r0, #27
	movs	r1, #32
	movs	r2, #0
	bl 0x0200cbf4
	movs	r0, #28
	movs	r1, #24
	movs	r2, #24
	bl 0x0200cbf4
	movs	r1, #24
	movs	r2, #24
	movs	r0, #29
	bl 0x0200cbf4
	movs	r0, #28
	bl 0x0200cafc
	movs	r0, #29
	bl 0x0200cafc
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #0
	movs	r2, #0
	movs	r0, #29
	bl 0x0200cb54
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #0
	movs	r0, #28
	bl 0x0200cb6c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #40
	movs	r0, #28
	bl 0x0200cb6c
	movs	r1, #192
	movs	r2, #192
	movs	r0, #28
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200cab4
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #9
	movs	r0, #29
	lsls	r1, r1, #10
	bl 0x0200cab4
	ldr	r1, [pc, #676]
	movs	r0, #28
	bl 0x0200cabc
	ldr	r1, [pc, #672]
	movs	r0, #29
	bl 0x0200cabc
	movs	r0, #28
	bl 0x0200cac4
	movs	r0, #29
	bl 0x0200cac4
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #28
	movs	r1, #2
	bl 0x0200cb2c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #29
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #29
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #28
	ldr	r1, [pc, #596]
	adds	r2, #204
	bl 0x0200cab4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #29
	ldr	r1, [pc, #584]
	adds	r2, #204
	bl 0x0200cab4
	movs	r2, #204
	lsls	r2, r2, #8
	movs	r0, #25
	ldr	r1, [pc, #568]
	adds	r2, #204
	bl 0x0200cab4
	movs	r2, #204
	lsls	r2, r2, #8
	adds	r2, #204
	movs	r0, #27
	ldr	r1, [pc, #552]
	bl 0x0200cab4
	movs	r0, #25
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	movs	r0, #25
	lsls	r1, r1, #1
	bl 0x0200cb74
	movs	r0, #27
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200cb74
	movs	r0, #40
	bl 0x0200ca8c
	ldr	r1, [pc, #508]
	movs	r0, #28
	bl 0x0200cabc
	ldr	r1, [pc, #504]
	movs	r0, #29
	bl 0x0200cabc
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #1
	bl 0x0200cb7c
	movs	r0, #25
	movs	r1, #64
	movs	r2, #0
	bl 0x0200cbf4
	movs	r0, #27
	movs	r1, #64
	movs	r2, #0
	bl 0x0200cbfc
	movs	r2, #154
	movs	r0, #25
	movs	r1, #168
	lsls	r2, r2, #2
	bl 0x0200cadc
	movs	r2, #154
	lsls	r2, r2, #2
	movs	r1, #184
	movs	r0, #27
	bl 0x0200cae4
	movs	r0, #25
	bl 0x0200cac4
	movs	r0, #25
	movs	r1, #1
	bl 0x0200cb0c
	movs	r1, #224
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #28
	bl 0x0200cac4
	movs	r0, #29
	bl 0x0200cac4
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	movs	r0, #25
	lsls	r1, r1, #1
	bl 0x0200cb74
	movs	r0, #27
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200cb74
	movs	r0, #40
	bl 0x0200ca8c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r0, #192
	movs	r1, #1
	movs	r2, #240
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r0, #240
	movs	r1, #1
	movs	r2, #172
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r0, #244
	movs	r1, #1
	movs	r2, #228
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r0, #192
	movs	r1, #1
	movs	r2, #154
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #192
	movs	r0, #28
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200cb0c
	movs	r1, #3
	movs	r0, #29
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #27
	bl 0x0200cb6c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	b.n	.L_02002484
	.4byte 0x000024ce
	.4byte 0x0200d4f4
	.4byte 0x0200d56c
	.4byte 0x00019999
	.4byte 0x0200d5d0
	.2byte 0xd620
	.2byte 0x0200
.L_02002484:
	movs	r2, #0
	movs	r1, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200cb0c
	movs	r1, #3
	movs	r0, #29
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #208
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	movs	r0, #28
	lsls	r1, r1, #1
	bl 0x0200cb74
	movs	r0, #29
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
.L_02002512:
	lsls	r1, r1, #1
	movs	r0, #29
	bl 0x0200cb74
	movs	r0, #50
	bl 0x0200ca8c
	movs	r1, #176
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #29
	bl 0x0200cb54
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #28
	bl 0x0200cb6c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #29
	bl 0x0200cb6c
	movs	r1, #176
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #144
	movs	r1, #1
	movs	r2, #136
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #16
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #4
	bl 0x0200cb6c
	movs	r1, #192
	movs	r2, #192
	movs	r0, #25
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cab4
	movs	r1, #192
	movs	r2, #192
	movs	r0, #27
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200cab4
	movs	r1, #32
	movs	r2, #32
	movs	r0, #25
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cbf4
	movs	r1, #32
	movs	r2, #32
	movs	r0, #27
	negs	r1, r1
	negs	r2, r2
	bl 0x0200cbfc
	movs	r2, #24
	movs	r0, #25
	movs	r1, #0
	negs	r2, r2
	bl 0x0200cbf4
	movs	r2, #24
	movs	r1, #0
	negs	r2, r2
	movs	r0, #27
	bl 0x0200cbfc
	movs	r0, #25
	bl 0x0200cafc
	movs	r1, #208
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #130
	movs	r0, #4
	movs	r1, #136
	lsls	r2, r2, #2
	bl 0x0200cae4
	movs	r3, #160
	lsls	r3, r3, #7
	movs	r0, #26
	movs	r1, #16
	movs	r2, #0
	bl 0x0200cbec
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #26
	bl 0x0200cafc
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #26
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #2
	movs	r0, #27
	bl 0x0200cb2c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #160
	movs	r0, #26
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #192
	movs	r0, #27
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #25
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #2
	movs	r0, #28
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #28
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #29
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #29
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #20
	bl 0x0200cb4c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #64
	bl 0x0200cbf4
	movs	r1, #0
	movs	r2, #64
	movs	r0, #29
	bl 0x0200cbf4
	movs	r0, #60
	bl 0x0200ca8c
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb04
	movs	r1, #176
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #0
	movs	r0, #25
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	beq.n	.L_0200288a
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #26
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020028d0
.L_0200288a:
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #26
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #6
	strh	r3, [r2, #0]
	adds	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
.L_020028d0:
	movs	r2, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #4
	bl 0x0200cb14
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #4
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #25
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	movs	r0, #27
	bl 0x0200cab4
	movs	r0, #4
	bl 0x0200caac
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #26
	bl 0x0200caac
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r1, #0
	movs	r0, #4
	movs	r2, #120
	bl 0x0200cbf4
	movs	r0, #26
	movs	r1, #0
	movs	r2, #120
	bl 0x0200cbf4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #80
	bl 0x0200cbf4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #80
	bl 0x0200cbfc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	subs	r3, #172
	str	r3, [r2, #0]
	bl 0x0200cbd4
	bl 0x0200cbdc
	ldr	r0, [pc, #12]
	movs	r1, #81
	bl 0x0200cba4
	bl 0x0200ca9c
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0061
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	sub	sp, #8
	bl 0x0200c9fc
	cmp	r0, #0
	bne.n	.L_02002a22
	bl 0x0200b260
.L_02002a22:
	movs	r0, #129
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200ca04
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200ca04
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #1016]
	bl 0x0200cb3c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #138
	movs	r0, #4
	movs	r1, #136
	lsls	r2, r2, #2
	bl 0x0200cae4
	movs	r1, #16
	movs	r3, #160
	lsls	r3, r3, #8
	movs	r0, #26
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbec
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #26
	bl 0x0200cafc
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #26
	bl 0x0200cb6c
	movs	r0, #176
	movs	r1, #1
	movs	r2, #196
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #25
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #2
	movs	r0, #27
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #224
	movs	r2, #128
	movs	r0, #22
	lsls	r1, r1, #16
	lsls	r2, r2, #12
	movs	r5, #192
	bl 0x0200cb04
	lsls	r5, r5, #18
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	ldr	r0, [r5, #32]
	movs	r1, #0
	adds	r3, r0, #0
	adds	r3, #236
	movs	r2, #128
	str	r1, [r3, #0]
	lsls	r2, r2, #19
	adds	r3, #8
	str	r2, [r3, #0]
	subs	r3, #4
	str	r1, [r3, #0]
	movs	r0, #128
	adds	r3, #8
	movs	r1, #128
	str	r2, [r3, #0]
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x0200cb84
	movs	r0, #208
	movs	r1, #1
	movs	r2, #168
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #16
	bl 0x0200cb8c
	movs	r0, #50
	bl 0x0200ca8c
	movs	r0, #78
	bl 0x0200cc9c
	bl 0x0200cbd4
	movs	r0, #16
	bl 0x0200cbc4
	bl 0x0200cbdc
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200cb8c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #202
	movs	r1, #1
	movs	r2, #166
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #18
	movs	r3, #0
	bl 0x0200cb8c
	ldr	r3, [r5, #108]
	movs	r1, #230
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	movs	r5, #15
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #12]
	ldr	r1, [r0, #8]
	bl 0x0200ca2c
	movs	r0, #50
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #1
	bl 0x0200cb64
	movs	r0, #23
	movs	r1, #1
	bl 0x0200cb64
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #23
	ldr	r1, [pc, #664]
	adds	r2, #153
	bl 0x0200cab4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #22
	ldr	r1, [pc, #648]
	adds	r2, #153
	bl 0x0200cab4
	movs	r1, #198
	movs	r2, #214
	movs	r0, #22
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #206
	movs	r2, #214
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #186
	movs	r2, #254
	movs	r0, #26
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200cb04
	movs	r1, #186
	movs	r2, #254
	lsls	r2, r2, #18
	lsls	r1, r1, #18
	movs	r0, #25
	bl 0x0200cb04
	movs	r0, #35
	bl 0x0200cc9c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cbbc
	bl 0x0200cbcc
	bl 0x0200cbdc
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200cb84
	movs	r0, #202
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #22
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r2, #10
	movs	r0, #22
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #25
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #23
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #160
	movs	r0, #22
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb6c
	movs	r1, #128
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x0200cb6c
	movs	r0, #22
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x0200cb74
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #0
	movs	r2, #16
	movs	r0, #22
	bl 0x0200cbfc
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cb6c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #16
	bl 0x0200cbfc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cb6c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #50
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #45
	movs	r0, #22
	bl 0x0200cb6c
	movs	r1, #0
	movs	r2, #32
	movs	r0, #22
	bl 0x0200cbfc
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #22
	bl 0x0200cb6c
	movs	r2, #32
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200cbfc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #50
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #23
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cb6c
	movs	r0, #23
	movs	r1, #0
	b.n	.L_02002e44
	.4byte 0x0000253b
	.2byte 0x3333
	.2byte 0x0001
.L_02002e44:
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #22
	bl 0x0200cb6c
	movs	r1, #0
	movs	r2, #32
	movs	r0, #22
	bl 0x0200cbfc
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #22
	bl 0x0200cb6c
	movs	r2, #32
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200cbfc
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cb6c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #22
	bl 0x02009bf4
	movs	r0, #103
	bl 0x0200cc9c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #22
	bl 0x0200cb6c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #22
	bl 0x02009bf4
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #22
	bl 0x0200cb6c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cb6c
	movs	r2, #10
	movs	r0, #23
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #2
	movs	r0, #22
	bl 0x0200cb2c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #160
	movs	r0, #22
	lsls	r1, r1, #7
	bl 0x0200cb5c
	movs	r1, #7
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #6
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #5
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #23
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #23
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x0200cb6c
	movs	r1, #7
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #1
	bl 0x0200cb0c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #15
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #23
	bl 0x0200cb6c
	movs	r0, #23
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #22
	bl 0x0200cb6c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #8
	bl 0x0200cbfc
	movs	r0, #22
	movs	r1, #10
	movs	r2, #16
	bl 0x0200cbfc
	movs	r1, #160
	movs	r0, #23
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #22
	movs	r1, #6
	movs	r2, #16
	bl 0x0200cbfc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #22
	bl 0x0200cb54
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #22
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x0200cb6c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #20
	adds	r0, #22
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #8
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #22
	movs	r1, #7
	movs	r2, #0
	bl 0x0200cb1c
.L_020030e0:
	movs	r0, #22
	bl 0x0200caac
	ldr	r2, [pc, #384]
	ldrh	r3, [r0, #6]
	subs	r5, #1
	adds	r3, r3, r2
	strh	r3, [r0, #6]
	movs	r0, #1
	bl 0x0200c994
	cmp	r5, #0
	bge.n	.L_020030e0
	movs	r1, #5
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #7
	bl 0x0200ca8c
	movs	r0, #22
	movs	r1, #0
	movs	r2, #20
	bl 0x0200cb4c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #23
	bl 0x0200cb6c
	movs	r2, #36
	movs	r1, #0
	movs	r0, #23
	bl 0x0200cbfc
	movs	r0, #15
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #23
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r0, #23
	movs	r1, #6
	movs	r2, #23
	bl 0x0200cb1c
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r2, #15
	adds	r0, #23
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #9
	movs	r0, #22
	bl 0x0200cb0c
	movs	r0, #70
	bl 0x0200ca8c
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #22
	ldr	r7, [r3, #32]
	bl 0x0200caac
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #23
	bl 0x0200caac
	movs	r3, #64
	adds	r0, #85
	strb	r5, [r0, #0]
	mov	r8, r3
	str	r3, [sp, #4]
	movs	r5, #42
	movs	r0, #42
	movs	r1, #60
	movs	r2, #17
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200ca5c
	movs	r6, #68
	movs	r0, #42
	movs	r1, #60
	movs	r2, #17
	movs	r3, #4
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ca5c
	mov	r1, r8
	movs	r5, #106
	str	r1, [sp, #4]
	movs	r0, #106
	movs	r1, #60
	movs	r2, #17
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200ca5c
	movs	r0, #106
	movs	r1, #60
	movs	r2, #17
	movs	r3, #4
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ca5c
	movs	r5, #239
.L_020031f2:
	movs	r3, #138
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r2, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldr	r3, [r2, #0]
	movs	r0, #22
	adds	r3, r3, r1
	str	r3, [r2, #0]
	bl 0x0200caac
	ldr	r1, [pc, #84]
	ldr	r3, [r0, #16]
	subs	r5, #1
	adds	r3, r3, r1
	str	r3, [r0, #16]
	movs	r0, #23
	bl 0x0200caac
	ldr	r2, [pc, #68]
	ldr	r3, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #16]
	movs	r0, #1
	bl 0x0200c994
	cmp	r5, #0
	bge.n	.L_020031f2
	movs	r0, #78
	bl 0x0200cc9c
	movs	r0, #30
	bl 0x0200ca8c
	bl 0x0200ca9c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200cb8c
	ldr	r0, [pc, #20]
	movs	r1, #99
	bl 0x0200cba4
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0xfffff000
	.4byte 0xffff0000
	.2byte 0x0062
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	bl 0x0200ca94
	movs	r0, #0
	bl 0x0200cbe4
	ldr	r0, [pc, #1016]
	bl 0x0200cb3c
	movs	r0, #152
	movs	r1, #1
	movs	r2, #136
	lsls	r0, r0, #16
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x0200cb8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #230
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r5, #254
	ldr	r3, [r0, #16]
	ldr	r2, [r0, #12]
	ldr	r1, [r0, #8]
	bl 0x0200ca2c
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200cbbc
	bl 0x0200cbcc
	bl 0x0200cbdc
	movs	r0, #15
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #152
	movs	r1, #1
	movs	r2, #196
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #16
	bl 0x0200cb8c
	bl 0x0200cb94
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #25
	movs	r1, #6
	movs	r2, #15
	bl 0x0200cb1c
	movs	r1, #6
	movs	r2, #23
	movs	r0, #25
	bl 0x0200cb1c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #51
	adds	r2, #153
	movs	r0, #25
	bl 0x0200cab4
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #8
	movs	r1, #0
	movs	r0, #25
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #25
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	ldr	r1, [pc, #676]
	ldr	r2, [pc, #680]
	movs	r0, #25
	bl 0x0200cab4
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #16
	strb	r3, [r0, #0]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #25
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r7, #0
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #8
	movs	r0, #25
	bl 0x0200cbf4
	movs	r0, #133
	bl 0x0200cc9c
	ldr	r1, [pc, #584]
	ldr	r2, [pc, #588]
	movs	r0, #29
	bl 0x0200cab4
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #4
	movs	r1, #0
	strb	r3, [r0, #0]
	negs	r2, r2
	movs	r0, #29
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200caac
	movs	r3, #128
	lsls	r3, r3, #8
	mov	fp, r3
	mov	r2, fp
	strh	r2, [r0, #6]
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #4
	movs	r1, #0
	strb	r3, [r0, #0]
	negs	r2, r2
	movs	r0, #29
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200caac
	movs	r3, #176
	lsls	r3, r3, #8
	mov	r8, r3
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #4
	movs	r1, #0
	strb	r3, [r0, #0]
	negs	r2, r2
	movs	r0, #29
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200caac
	movs	r3, #208
	lsls	r3, r3, #8
	mov	r9, r3
	mov	r2, r9
	strh	r2, [r0, #6]
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #4
	movs	r1, #0
	negs	r2, r2
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200caac
	strh	r7, [r0, #6]
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #4
	movs	r1, #0
	strb	r3, [r0, #0]
	negs	r2, r2
	movs	r0, #29
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #29
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #29
	bl 0x0200caac
	movs	r3, #192
	lsls	r3, r3, #6
	mov	sl, r3
	mov	r2, sl
	strh	r2, [r0, #6]
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #0
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #168
	movs	r2, #184
	lsls	r2, r2, #17
	movs	r0, #29
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #0
	movs	r0, #29
	bl 0x0200cc54
	movs	r0, #29
	bl 0x0200caac
	movs	r1, #1
	bl 0x0200ca6c
	movs	r1, #160
	movs	r0, #29
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #29
	movs	r1, #6
	movs	r2, #40
	bl 0x0200cb1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #29
	bl 0x0200cb6c
	mov	r1, sl
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #51
	adds	r2, #153
	movs	r0, #25
	bl 0x0200cab4
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #8
	strb	r3, [r0, #0]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #25
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200ca8c
	ldr	r1, [pc, #72]
	ldr	r2, [pc, #76]
	movs	r0, #25
	bl 0x0200cab4
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #16
	movs	r0, #25
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #25
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #133
	bl 0x0200cc9c
	ldr	r1, [pc, #12]
	ldr	r2, [pc, #12]
	movs	r0, #29
	b.n	.L_02003694
	.2byte 0x0000
	.4byte 0x00002559
	.4byte 0x00026666
	.2byte 0x3333
	.2byte 0x0001
.L_02003694:
	bl 0x0200cab4
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #0
	movs	r2, #3
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200caac
	strh	r7, [r0, #6]
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #0
	movs	r2, #3
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200caac
	mov	r3, r9
	strh	r3, [r0, #6]
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r0, #0]
	movs	r2, #3
	movs	r0, #27
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200caac
	mov	r2, r8
	strh	r2, [r0, #6]
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #0
	movs	r2, #3
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #27
	bl 0x0200caac
	mov	r3, fp
	strh	r3, [r0, #6]
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #0
	ands	r5, r3
	movs	r2, #3
	strb	r5, [r0, #0]
	movs	r0, #27
	bl 0x0200cbfc
	movs	r0, #1
	bl 0x0200ca8c
	movs	r0, #27
	bl 0x0200caac
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r6, r3
	strb	r6, [r0, #0]
	movs	r0, #27
	bl 0x0200caac
	movs	r6, #160
	lsls	r6, r6, #7
	strh	r6, [r0, #6]
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #168
	movs	r2, #212
	lsls	r2, r2, #17
.L_020037c8:
	movs	r0, #27
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #0
	movs	r0, #27
	bl 0x0200cc54
	movs	r0, #27
	bl 0x0200caac
	movs	r1, #1
	bl 0x0200ca6c
	movs	r0, #27
	bl 0x0200caac
	mov	r2, r8
.L_020037ec:
	strh	r2, [r0, #6]
	movs	r1, #6
	movs	r0, #27
	movs	r2, #40
	bl 0x0200cb1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #27
	bl 0x0200cb6c
	mov	r1, r9
	movs	r2, #0
	movs	r0, #25
.L_0200380a:
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
.L_0200382a:
	bl 0x0200cb4c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #50
	movs	r0, #29
	bl 0x0200cb6c
	mov	r1, sl
	movs	r2, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200cb6c
	movs	r1, #5
	movs	r0, #25
	bl 0x0200cb0c
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #29
	mov	r1, sl
	movs	r2, #0
	bl 0x0200cb54
	movs	r2, #0
	mov	r1, r9
	movs	r0, #27
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #29
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	movs	r0, #29
	lsls	r1, r1, #1
	bl 0x0200cb74
	movs	r0, #27
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200cb74
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #29
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r0, #29
	movs	r1, #16
	movs	r2, #16
	bl 0x0200cbfc
	movs	r0, #29
	movs	r1, #0
	movs	r2, #40
	bl 0x0200cbfc
	movs	r2, #0
	mov	r1, r8
	movs	r0, #29
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #25
	bl 0x0200cb0c
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #29
	mov	r1, r8
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #27
	mov	r1, r8
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #27
	movs	r1, #3
	bl 0x0200cb0c
	movs	r1, #3
	movs	r0, #29
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #25
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #27
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #29
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #28
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #30
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	movs	r0, #31
	adds	r1, #204
	bl 0x0200cab4
	movs	r0, #25
	movs	r1, #1
	bl 0x0200cb7c
	movs	r1, #16
	movs	r0, #25
	negs	r1, r1
	movs	r2, #16
	bl 0x0200cbfc
	movs	r0, #25
	movs	r1, #0
	movs	r2, #16
	bl 0x0200cbfc
	movs	r2, #252
	movs	r0, #25
	movs	r1, #152
	lsls	r2, r2, #1
	bl 0x0200cadc
	movs	r1, #16
	movs	r0, #29
	negs	r1, r1
	movs	r2, #0
	bl 0x0200cbf4
	movs	r1, #16
	movs	r0, #27
	negs	r1, r1
	movs	r2, #16
	bl 0x0200cbfc
	movs	r1, #16
	movs	r0, #29
	negs	r1, r1
	movs	r2, #16
	bl 0x0200cbf4
	movs	r0, #27
	movs	r1, #0
	movs	r2, #16
	bl 0x0200cbfc
	movs	r2, #244
	movs	r0, #27
	movs	r1, #152
	lsls	r2, r2, #1
	bl 0x0200cadc
	movs	r2, #236
	lsls	r2, r2, #1
	movs	r0, #29
	movs	r1, #152
	bl 0x0200cae4
	movs	r0, #25
	movs	r1, #1
	bl 0x0200cb0c
	movs	r0, #27
	movs	r1, #1
	bl 0x0200cb0c
	movs	r0, #29
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r0, #27
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200cb54
	movs	r2, #0
	adds	r1, r6, #0
	movs	r0, #25
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #4
	bl 0x0200cb14
	movs	r0, #192
	lsls	r0, r0, #7
	movs	r2, #10
	adds	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x0200cb74
	movs	r0, #26
	movs	r1, #3
	bl 0x0200cb24
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #26
	bl 0x0200cb74
	movs	r0, #40
	bl 0x0200ca8c
	movs	r0, #27
	movs	r1, #0
	movs	r2, #64
	bl 0x0200cbf4
	movs	r0, #29
	movs	r1, #0
	movs	r2, #64
	bl 0x0200cbf4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #64
	bl 0x0200cbfc
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #25
	bl 0x0200cb6c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #26
	mov	r1, sl
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200ca8c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r2, #16
	movs	r1, #0
	movs	r0, #25
	bl 0x0200cbfc
	movs	r0, #10
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #25
	bl 0x0200cb2c
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #30
	movs	r1, #2
	bl 0x0200cb24
	movs	r0, #31
	movs	r1, #2
	bl 0x0200cb24
	movs	r1, #2
	movs	r0, #28
	bl 0x0200cb2c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #176
	movs	r2, #148
	lsls	r2, r2, #18
	movs	r0, #28
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #0
	movs	r0, #28
	bl 0x0200cc54
	movs	r0, #28
	bl 0x0200caac
	movs	r1, #1
	bl 0x0200ca6c
	movs	r0, #28
	movs	r1, #6
	movs	r2, #20
	bl 0x0200cb1c
	movs	r0, #28
	mov	r1, fp
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #128
	movs	r2, #152
	lsls	r2, r2, #18
	movs	r0, #31
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #0
	movs	r0, #31
	bl 0x0200cc54
	movs	r0, #31
	bl 0x0200caac
	movs	r1, #1
	bl 0x0200ca6c
	movs	r0, #31
	movs	r1, #6
	movs	r2, #20
	bl 0x0200cb1c
	movs	r0, #31
	mov	r1, r9
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #192
	movs	r2, #154
	lsls	r2, r2, #18
	movs	r0, #30
	lsls	r1, r1, #16
	bl 0x0200cb04
	movs	r1, #0
	movs	r0, #30
	bl 0x0200cc54
	movs	r0, #30
	bl 0x0200caac
	movs	r1, #1
	bl 0x0200ca6c
	movs	r0, #30
	movs	r1, #6
	movs	r2, #20
	bl 0x0200cb1c
	movs	r2, #0
	mov	r1, r8
	movs	r0, #30
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #25
	movs	r1, #4
	bl 0x0200cb14
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x0200cb4c
	movs	r0, #30
	movs	r1, #3
	bl 0x0200cb0c
	movs	r0, #31
	movs	r1, #3
	bl 0x0200cb0c
	movs	r1, #3
	movs	r0, #28
	bl 0x0200cb14
	movs	r0, #30
	bl 0x0200ca8c
	movs	r0, #4
	movs	r1, #25
	bl 0x0200cc04
	movs	r0, #26
	movs	r1, #25
	bl 0x0200cc04
	movs	r0, #30
	movs	r1, #25
	bl 0x0200cc04
	movs	r0, #31
	movs	r1, #25
	bl 0x0200cc04
	movs	r0, #28
	movs	r1, #25
	bl 0x0200cc04
	ldr	r5, [pc, #612]
	movs	r0, #25
	adds	r1, r5, #0
	bl 0x0200cabc
	adds	r1, r5, #0
	movs	r0, #27
	bl 0x0200cabc
	movs	r0, #10
	bl 0x0200ca8c
	adds	r1, r5, #0
	movs	r0, #29
	bl 0x0200cabc
	movs	r0, #70
	bl 0x0200ca8c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #28
	negs	r1, r1
	bl 0x0200cbfc
	adds	r1, r5, #0
	movs	r0, #28
	bl 0x0200cabc
	movs	r0, #33
	bl 0x0200ca8c
	movs	r2, #0
	movs	r0, #31
	movs	r1, #16
	bl 0x0200cbfc
	adds	r1, r5, #0
	movs	r0, #31
	bl 0x0200cabc
	movs	r0, #15
	bl 0x0200ca8c
	adds	r1, r5, #0
	movs	r0, #30
	bl 0x0200cabc
	movs	r0, #25
	bl 0x0200cac4
	movs	r2, #0
	adds	r1, r6, #0
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #30
	bl 0x0200cac4
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #1
	movs	r0, #4
	bl 0x0200cb7c
	bl 0x0200cb94
	movs	r0, #40
	bl 0x0200ca8c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #26
	bl 0x0200cb6c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200cb54
	mov	r1, fp
	movs	r2, #0
	movs	r0, #4
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #0
	movs	r0, #26
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	bne.n	.L_02003d3e
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02003d60
.L_02003d3e:
	movs	r0, #40
	bl 0x0200ca8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #26
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
.L_02003d60:
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200cab4
	movs	r2, #16
	movs	r0, #26
	movs	r1, #0
	negs	r2, r2
	bl 0x0200cbfc
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #2
	movs	r0, #26
	bl 0x0200cb2c
	movs	r0, #20
	bl 0x0200ca8c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #16
	bl 0x0200cbfc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200cb54
	movs	r1, #0
	movs	r2, #0
	movs	r0, #26
	bl 0x0200cb54
	movs	r0, #20
	bl 0x0200ca8c
	movs	r1, #0
	movs	r0, #26
	bl 0x0200cb44
	movs	r0, #4
	movs	r1, #0
	bl 0x0200caa4
	cmp	r0, #0
	beq.n	.L_02003e2a
	movs	r0, #40
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r1, #3
	movs	r0, #26
	bl 0x0200cb14
	movs	r0, #10
	bl 0x0200ca8c
	movs	r2, #10
	movs	r0, #26
	movs	r1, #0
	bl 0x0200cb4c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_02003e4c
.L_02003e2a:
	movs	r0, #20
	bl 0x0200ca8c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #26
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200cb4c
.L_02003e4c:
	movs	r1, #3
	movs	r0, #4
	bl 0x0200cb14
	movs	r0, #20
	bl 0x0200ca8c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #26
	ldr	r1, [pc, #88]
	bl 0x0200cab4
	movs	r0, #26
	movs	r1, #2
	bl 0x0200cb0c
	ldr	r3, [pc, #76]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200caac
	cmp	r0, #0
	beq.n	.L_02003e90
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #26
	bl 0x0200cad4
.L_02003e90:
	movs	r0, #26
	bl 0x0200cafc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #26
	bl 0x0200cb04
	movs	r0, #10
	bl 0x0200ca8c
	bl 0x0200ca9c
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200d670
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r3, #192
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_02003ee2:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_02003f30
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl 0x0200cc6c
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl 0x0200cc74
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_02003f30
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_02003f30:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_02003ee2
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r2, [sp, #0]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r2, #0
	ble.n	.L_02003ff4
	adds	r7, r2, #0
.L_02003f70:
	bl 0x0200c9ac
	movs	r1, #176
	lsls	r1, r1, #5
	add	r1, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	sl, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	movs	r3, #160
	add	r6, fp
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r2, [sp, #8]
	mov	r8, r1
	str	r2, [r5, #0]
	ldr	r3, [sp, #4]
	mov	r9, r0
	str	r3, [r5, #4]
	ldr	r1, [sp, #0]
	subs	r7, #1
	str	r1, [r5, #8]
	bl 0x0200c9ac
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl 0x0200c9bc
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl 0x0200c9ac
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #12
	movs	r2, #128
	adds	r6, r6, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r6, #0
	bl 0x0200c9bc
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
.L_02003fe8:
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_02003f70
.L_02003ff4:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
.L_02003ffe:
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #8
	movs	r0, #220
	sub	sp, #4
	bl 0x0200c9c4
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl 0x0200c9f4
	adds	r1, r6, #0
	bl 0x0200c9d4
	bl 0x0200c9ec
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl 0x0200c9e4
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #2
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #160
	movs	r3, #192
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	movs	r1, #63
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_0200405c:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl 0x0200cc64
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
.L_0200407c:
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_0200405c
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
.L_020040a0:
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200c99c
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0xbec5
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200c9a4
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200c9dc
	movs	r0, #220
	bl 0x0200c9cc
	pop	{r5, pc}
	.2byte 0xbec5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #8
	mov	r8, r3
	bl 0x0200caac
	ldr	r3, [r0, #80]
	ldr	r5, [pc, #232]
	ldr	r3, [r3, #40]
	movs	r1, #0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r4, r3, #16
	ldrh	r3, [r5, r1]
	lsrs	r2, r4, #16
	cmp	r2, r3
	beq.n	.L_02004132
.L_02004118:
	movs	r2, #128
	lsls	r3, r1, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	lsrs	r2, r3, #16
	asrs	r1, r3, #16
	cmp	r2, #5
	bhi.n	.L_02004132
	lsls	r3, r2, #1
	ldrh	r3, [r5, r3]
	lsrs	r2, r4, #16
	cmp	r2, r3
	bne.n	.L_02004118
.L_02004132:
	lsls	r3, r1, #16
	lsrs	r2, r3, #16
	cmp	r2, #6
	bne.n	.L_0200413e
	movs	r0, #0
	b.n	.L_020041e6
.L_0200413e:
	ldr	r6, [pc, #180]
	lsls	r2, r2, #2
	ldrsb	r4, [r6, r2]
	adds	r1, r4, #0
	cmp	r4, #0
	bge.n	.L_0200414c
	negs	r1, r4
.L_0200414c:
	adds	r3, r2, #2
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_02004156
	negs	r3, r3
.L_02004156:
	adds	r3, r1, r3
	asrs	r7, r3, #4
	adds	r3, r2, #1
	ldrsb	r1, [r6, r3]
	adds	r5, r1, #0
	cmp	r1, #0
	bge.n	.L_02004166
	negs	r5, r1
.L_02004166:
	adds	r3, r2, #3
	ldrsb	r2, [r6, r3]
	cmp	r2, #0
	bge.n	.L_02004170
	negs	r2, r2
.L_02004170:
	adds	r5, r5, r2
	mov	sl, r5
	ldr	r6, [r0, #8]
	mov	r3, sl
	ldr	r5, [r0, #16]
	asrs	r3, r3, #4
	mov	sl, r3
	lsls	r3, r4, #16
	adds	r6, r6, r3
	lsls	r3, r1, #16
.L_02004184:
	adds	r5, r5, r3
	movs	r3, #164
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	asrs	r6, r6, #20
	asrs	r1, r3, #20
	movs	r3, #166
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r3, [r3, #0]
	lsls	r2, r1, #16
	asrs	r3, r3, #20
	lsls	r3, r3, #16
	asrs	r5, r5, #20
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r2, r6, r2
	adds	r3, r5, r3
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	r3, sl
	bl 0x0200ca54
	movs	r3, #255
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r0, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x0200c1f8
	mov	r2, sl
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #2
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	bl 0x0200c1f8
	movs	r0, #1
.L_020041e6:
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0200cca4
	.2byte 0xccb0
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r3, #0
	ldr	r3, [sp, #12]
	lsls	r2, r2, #7
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r0, r0, #1
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r0, [r4, r3]
	adds	r1, r1, r2
	lsls	r1, r1, #2
	adds	r0, r0, r1
	movs	r1, #0
	ldr	r6, [sp, #16]
	cmp	r1, ip
	bcs.n	.L_0200423e
.L_02004224:
	lsls	r3, r1, #9
	movs	r2, #0
	adds	r3, r0, r3
	cmp	r2, r5
	bcs.n	.L_02004238
.L_0200422e:
	adds	r2, #1
	strb	r6, [r3, #2]
	adds	r3, #4
	cmp	r2, r5
	bcc.n	.L_0200422e
.L_02004238:
	adds	r1, #1
	cmp	r1, ip
	bcc.n	.L_02004224
.L_0200423e:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	adds	r5, r6, #0
	sub	sp, #40
	adds	r1, r6, #0
	adds	r5, #12
	add	r0, sp, #24
	adds	r1, #16
	adds	r2, r5, #0
	bl 0x0200c3a8
	adds	r4, r0, #0
	cmp	r4, #0
	bne.n	.L_0200426a
	b.n	.L_0200438a
.L_0200426a:
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #300]
	str	r5, [sp, #20]
	lsls	r1, r5, #2
	ldrsb	r2, [r0, r1]
	cmp	r2, #0
	bge.n	.L_0200427a
	negs	r2, r2
.L_0200427a:
	adds	r3, r1, #2
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_02004284
	negs	r3, r3
.L_02004284:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #16]
	adds	r3, r1, #1
	ldrsb	r2, [r0, r3]
	cmp	r2, #0
	bge.n	.L_02004294
	negs	r2, r2
.L_02004294:
	adds	r3, r1, #3
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	bge.n	.L_0200429e
	negs	r3, r3
.L_0200429e:
	adds	r3, r2, r3
	asrs	r3, r3, #4
	str	r3, [sp, #12]
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #248]
	ldr	r1, [pc, #248]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	mov	r9, r1
	mov	r2, r9
	ands	r2, r3
	lsls	r3, r3, #16
	mov	sl, r3
	movs	r3, #0
	str	r3, [r6, #20]
	mov	fp, r3
	adds	r3, r4, #0
	adds	r3, #34
	str	r3, [sp, #8]
	ldr	r1, [sp, #8]
	movs	r3, #2
	strb	r3, [r1, #0]
	mov	r9, r2
	ldr	r3, [r4, #8]
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r4, #16]
	add	r3, sl
	str	r3, [r6, #8]
	ldr	r3, [r4, #12]
	str	r3, [sp, #32]
.L_020042dc:
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #188]
	lsls	r3, r3, #2
	str	r3, [sp, #4]
	adds	r3, #1
	ldrsb	r2, [r2, r3]
	ldr	r3, [r6, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	movs	r1, #0
	mov	r8, r1
	str	r3, [sp, #36]
	cmp	r8, r2
	bge.n	.L_0200434a
.L_020042fa:
	ldr	r3, [pc, #160]
	ldr	r1, [sp, #4]
	add	r5, sp, #28
	ldrsb	r2, [r3, r1]
	ldr	r3, [r6, #0]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	movs	r7, #0
	cmp	r7, r2
	bge.n	.L_02004334
.L_02004312:
	adds	r0, r4, #0
	add	r1, sp, #28
	str	r4, [sp, #0]
	bl 0x0200ca64
	ldr	r4, [sp, #0]
	cmp	r0, #2
	beq.n	.L_0200435c
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r2, [sp, #16]
	adds	r7, #1
	cmp	r7, r2
	blt.n	.L_02004312
.L_02004334:
	add	r2, sp, #28
	ldr	r3, [r2, #8]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #8]
	ldr	r3, [sp, #12]
	movs	r2, #1
	add	r8, r2
	cmp	r8, r3
	blt.n	.L_020042fa
.L_0200434a:
	ldr	r3, [r6, #0]
	movs	r1, #1
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [r6, #8]
	add	fp, r1
	add	r3, sl
	str	r3, [r6, #8]
	b.n	.L_020042dc
.L_0200435c:
	ldr	r2, [sp, #8]
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r3, fp
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0200438c
	mov	r1, r9
	ldr	r3, [r4, #8]
	mov	r2, fp
	muls	r2, r1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r0, #1
	ldr	r3, [r4, #12]
	str	r3, [r6, #4]
	mov	r3, sl
	mov	r2, fp
	muls	r2, r3
	ldr	r3, [r4, #16]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	b.n	.L_0200438c
.L_0200438a:
	movs	r0, #0
.L_0200438c:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200ccb0
	.4byte 0x0200ccc8
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #236]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200caac
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	ldr	r1, [sp, #8]
	lsrs	r3, r3, #12
	str	r3, [r1, #0]
	movs	r2, #8
	adds	r5, #52
	mov	fp, r2
	mov	lr, r5
.L_020043e4:
	mov	r3, lr
	ldr	r6, [r3, #0]
	movs	r5, #0
.L_020043ea:
	ldr	r3, [r6, #80]
	ldr	r2, [pc, #200]
	ldr	r3, [r3, #40]
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	bne.n	.L_0200448e
	ldr	r0, [sp, #8]
	movs	r2, #10
	ldrsh	r1, [r7, r2]
	ldr	r3, [r0, #0]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldr	r4, [pc, #180]
	asrs	r2, r3, #16
	adds	r1, r1, r2
	asrs	r1, r1, #4
	mov	r9, r1
	movs	r1, #18
	ldrsh	r2, [r7, r1]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r2, r2, r3
	asrs	r2, r2, #4
	mov	r8, r2
	movs	r2, #10
	ldrsh	r0, [r6, r2]
	lsls	r2, r5, #2
	ldrsb	r3, [r4, r2]
	adds	r3, r0, r3
	asrs	r3, r3, #4
	mov	sl, r3
	movs	r3, #18
	ldrsh	r1, [r6, r3]
	adds	r3, r2, #1
	ldrsb	r3, [r4, r3]
	adds	r3, r1, r3
	asrs	r3, r3, #4
	mov	ip, r3
	adds	r3, r2, #2
	ldrsb	r3, [r4, r3]
	adds	r2, #3
	adds	r0, r0, r3
	ldrsb	r3, [r4, r2]
	asrs	r0, r0, #4
	adds	r1, r1, r3
	asrs	r1, r1, #4
	cmp	sl, r9
	bgt.n	.L_0200448e
	cmp	r9, r0
	bge.n	.L_0200448e
	cmp	ip, r8
	bgt.n	.L_0200448e
	cmp	r8, r1
	bge.n	.L_0200448e
	ldr	r0, [sp, #0]
	movs	r3, #1
	ands	r3, r5
	str	r5, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0200447c
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	sl, r3
	beq.n	.L_0200448e
	ldr	r2, [sp, #4]
	mov	r1, fp
	str	r1, [r2, #0]
	adds	r0, r6, #0
	b.n	.L_020044a4
.L_0200447c:
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	ip, r3
	beq.n	.L_0200448e
	ldr	r0, [sp, #4]
	mov	r3, fp
	str	r3, [r0, #0]
	adds	r0, r6, #0
	b.n	.L_020044a4
.L_0200448e:
	adds	r5, #1
	cmp	r5, #5
	bls.n	.L_020043ea
	movs	r2, #1
	add	fp, r2
	movs	r1, #4
	mov	r3, fp
	add	lr, r1
	cmp	r3, #63
	bls.n	.L_020043e4
	movs	r0, #0
.L_020044a4:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200cca4
	.4byte 0x0200ccc8
	.4byte 0x0200ccb0
	.2byte 0xb084
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r0, [sp, #88]
	str	r1, [sp, #92]
	str	r2, [sp, #96]
	str	r3, [sp, #100]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #133
	str	r3, [sp, #28]
	ldr	r3, [pc, #632]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	mov	sl, r0
	ldr	r0, [r0, #0]
	bl 0x0200caac
	mov	r8, r0
	ldr	r0, [sp, #104]
	bl 0x0200caac
	mov	r3, r8
	ldr	r3, [r3, #48]
	mov	r4, r8
	str	r3, [sp, #20]
	adds	r6, r0, #0
	ldr	r4, [r4, #52]
	mov	r0, sp
	adds	r0, #32
	str	r0, [sp, #12]
	str	r4, [sp, #16]
	ldr	r2, [sp, #100]
	ldr	r3, [r6, #8]
	movs	r1, #0
	str	r3, [r0, #0]
	mov	r9, r1
	ldr	r3, [r6, #16]
	mov	r1, sp
	adds	r1, #44
	str	r3, [r0, #8]
	ldr	r5, [pc, #576]
	str	r1, [sp, #8]
	lsls	r7, r2, #2
	ldrsb	r1, [r5, r7]
	ldr	r3, [r6, #8]
	lsls	r2, r1, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	asrs	r3, r3, #20
	str	r3, [r2, #0]
	mov	lr, r3
	adds	r3, r7, #1
	ldrsb	r4, [r5, r3]
	ldr	r3, [r6, #16]
	ldr	r0, [sp, #8]
	lsls	r2, r4, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r0, #8]
	adds	r0, r1, #0
	mov	ip, r3
	cmp	r0, #0
	bge.n	.L_02004554
	negs	r0, r0
.L_02004554:
	adds	r3, r7, #2
	ldrsb	r1, [r5, r3]
	cmp	r1, #0
	bge.n	.L_0200455e
	negs	r1, r1
.L_0200455e:
	adds	r3, r0, r1
	asrs	r3, r3, #4
	adds	r1, r4, #0
	str	r3, [sp, #24]
	cmp	r1, #0
	bge.n	.L_0200456c
	negs	r1, r1
.L_0200456c:
	adds	r3, r7, #3
	ldrsb	r2, [r5, r3]
	cmp	r2, #0
	bge.n	.L_02004576
	negs	r2, r2
.L_02004576:
	adds	r3, r1, r2
	asrs	r3, r3, #4
	str	r3, [sp, #0]
	mov	fp, r3
	movs	r3, #0
	str	r3, [sp, #4]
	mov	r1, lr
	mov	r2, ip
	ldr	r3, [sp, #24]
	movs	r0, #0
	bl 0x0200c1f8
	mov	r1, sl
	movs	r2, #200
	ldr	r0, [r1, #0]
	lsls	r2, r2, #5
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200cab4
	mov	r2, sl
	ldr	r0, [r2, #0]
	movs	r1, #8
	bl 0x0200cb0c
	movs	r0, #15
	bl 0x0200c994
	ldr	r4, [sp, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [r4, #0]
	ldr	r2, [sp, #96]
	subs	r1, r1, r3
	ldr	r3, [r4, #8]
	asrs	r1, r1, #17
	subs	r2, r2, r3
	mov	r3, sl
	asrs	r2, r2, #17
	ldr	r0, [r3, #0]
	bl 0x0200caf4
	mov	r4, sl
	ldr	r0, [r4, #0]
	bl 0x0200caac
	ldr	r3, [pc, #408]
	str	r3, [r0, #108]
	movs	r0, #4
	bl 0x0200c994
	movs	r1, #2
	adds	r0, r6, #0
	bl 0x0200ca0c
	movs	r0, #239
	bl 0x0200cc9c
	movs	r2, #200
	movs	r1, #128
	lsls	r2, r2, #5
	ldr	r0, [sp, #104]
	lsls	r1, r1, #8
	adds	r2, #153
	bl 0x0200cab4
	adds	r0, r6, #0
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #92]
	ldr	r3, [sp, #96]
	bl 0x0200ca34
	ldr	r3, [pc, #348]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r5, r3, r0
	ldr	r0, [r5, #0]
	bl 0x0200cafc
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200cb0c
	movs	r1, #152
	movs	r2, #200
	lsls	r1, r1, #7
	lsls	r2, r2, #5
	ldr	r0, [r5, #0]
	adds	r1, #204
	adds	r2, #153
	bl 0x0200cab4
	ldr	r2, [pc, #320]
	mov	r1, r9
	lsls	r3, r1, #2
	ldr	r2, [r2, r3]
	ldr	r0, [r5, #0]
	lsls	r2, r2, #16
	asrs	r1, r2, #31
	asrs	r2, r2, #17
	bl 0x0200caf4
	ldr	r3, [sp, #108]
	cmp	r3, #0
	beq.n	0x0200c64c
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6828
	bl 0x0200cafc
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200cb0c
	mov	r3, r8
	movs	r2, #0
	str	r2, [r3, #108]
	ldr	r4, [sp, #20]
	movs	r5, #255
	str	r4, [r3, #48]
	ldr	r0, [sp, #16]
	str	r0, [r3, #52]
	adds	r0, r6, #0
	bl 0x0200ca3c
	movs	r0, #149
	lsls	r0, r0, #1
	bl 0x0200cc9c
	movs	r0, #213
	bl 0x0200cc9c
	ldr	r2, [r6, #12]
	ldr	r1, [sp, #88]
	ldr	r3, [sp, #96]
	adds	r0, r6, #0
	bl 0x0200ca2c
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200ca0c
	ldr	r1, [pc, #212]
	ldr	r0, [sp, #88]
	ldrsb	r3, [r1, r7]
	adds	r2, r7, #1
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldrsb	r3, [r1, r2]
	mov	sl, r1
	ldr	r1, [sp, #96]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	ldr	r4, [sp, #28]
	asrs	r0, r0, #20
	asrs	r1, r1, #20
	str	r0, [sp, #88]
	str	r1, [sp, #96]
	mov	r9, r2
	movs	r2, #164
	lsls	r2, r2, #1
	adds	r3, r4, r2
	ldr	r3, [r3, #0]
	adds	r2, #4
	asrs	r3, r3, #20
	mov	r8, r3
	adds	r3, r4, r2
	ldr	r6, [r3, #0]
	mov	r4, r8
	asrs	r6, r6, #20
	adds	r3, r4, r0
	adds	r2, r6, r1
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	mov	r3, fp
	ldr	r2, [sp, #24]
	bl 0x0200ca54
	mov	r0, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r0, [sp, #0]
	ldr	r3, [sp, #24]
	movs	r0, #0
	str	r5, [sp, #4]
	bl 0x0200c1f8
	mov	r3, fp
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #96]
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r5, [sp, #4]
	bl 0x0200c1f8
	ldr	r0, [sp, #12]
	mov	r4, sl
	ldrsb	r3, [r4, r7]
	ldr	r1, [r0, #0]
	ldr	r2, [sp, #8]
	lsls	r3, r3, #16
	adds	r1, r1, r3
	asrs	r1, r1, #20
	str	r1, [r2, #0]
	mov	r3, r9
	ldrsb	r2, [r4, r3]
	ldr	r3, [r0, #8]
	ldr	r4, [sp, #8]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [r4, #8]
	add	r8, r1
	adds	r6, r6, r3
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #24]
	mov	r0, r8
	adds	r1, r6, #0
	mov	r3, fp
	bl 0x0200ca54
	ldr	r0, [sp, #8]
	mov	r3, fp
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #8]
	movs	r4, #0
	str	r3, [sp, #0]
	movs	r0, #2
	ldr	r3, [sp, #24]
	str	r4, [sp, #4]
	bl 0x0200c1f8
	bl 0x0200cc0c
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r3}
	add	sp, #16
	bx	r3
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200ccb0
	.4byte 0x0200c775
	.2byte 0xccc8
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #12
	lsrs	r1, r3, #12
	adds	r3, r1, #2
	ands	r3, r2
	lsls	r1, r3, #12
	ldr	r3, [r5, #8]
	sub	sp, #12
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	movs	r0, #128
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl 0x0200c9bc
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200cc3c
	cmp	r0, #0
	beq.n	.L_020047d0
	movs	r4, #0
.L_020047ac:
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
.L_020047b2:
	ldrsh	r1, [r3, r2]
	ldr	r2, [pc, #64]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	cmp	r1, r3
	beq.n	.L_020047f4
	adds	r4, #1
	cmp	r4, #5
	bls.n	.L_020047ac
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200ca2c
.L_020047d0:
	ldr	r3, [r5, #8]
	adds	r0, r5, #0
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	adds	r1, r6, #0
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl 0x0200ca64
	cmp	r0, #0
	ble.n	.L_020047f4
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r5, #0
	bl 0x0200ca2c
.L_020047f4:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0xcca4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_02004860
	adds	r7, r0, #0
.L_02004812:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200caac
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl 0x0200ca6c
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x0200c8ec
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02004812
.L_02004860:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_020048d4
.L_02004884:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_020048d0
	adds	r0, r7, #0
	bl 0x0200caac
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_020048ac
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_020048ac:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_020048de
	adds	r0, r5, #0
	bl 0x0200ca04
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x0200c8ec
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_020048de
.L_020048d0:
	adds	r5, #6
	movs	r1, #255
.L_020048d4:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_02004884
.L_020048de:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl 0x0200caac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl 0x0200cc34
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200c9fc
	cmp	r0, #0
	beq.n	.L_02004960
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_0200494c
	cmp	r6, #1
	bcc.n	.L_02004942
.L_0200493c:
	cmp	r6, #2
	beq.n	.L_02004956
	b.n	.L_0200498e
.L_02004942:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200ca0c
	b.n	.L_0200498e
.L_0200494c:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200ca0c
	b.n	.L_0200498e
.L_02004956:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200ca0c
	b.n	.L_0200498e
.L_02004960:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_0200497c
	cmp	r6, #1
	bcc.n	.L_02004972
	cmp	r6, #2
	beq.n	.L_02004986
	b.n	.L_0200498e
.L_02004972:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200ca0c
	b.n	.L_0200498e
.L_0200497c:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200ca0c
	b.n	.L_0200498e
.L_02004986:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200ca0c
.L_0200498e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000129, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x08020091, 0x080200a9, 0x080200c1, 0x080200c9, 0x080200e9, 0x08020149, 0x08020151, 0x08020171, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020211, 0x08020219, 0x08020291, 0x080202f9, 0x080ad2e9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8231, 0x080c8239, 0x080c8241, 0x080c8251, 0x080c8269, 0x080c8279, 0x080c82c1, 0x080c8379, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85e9, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8691, 0x080c86a9, 0x080c86d1, 0x080c86d9, 0x080c86e9, 0x080c8711, 0x080c8739, 0x080c8741, 0x080c8749, 0x080c87a9, 0x080c87b9, 0x080c87c1, 0x080c87d1, 0x080c87e1, 0x080c8841, 0x080c8849, 0x080c8869, 0x080c8871, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte 0x0820f8e0
	.4byte 0x2008e0f8
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0xffff000a
	.4byte 0x0001000e
	.4byte 0xffff08a2
	.4byte 0xffff0000
	.4byte 0x000001e8
	.4byte 0xc00001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000208
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0x000001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000001b8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000088
	.4byte 0xc0000238
	.4byte 0x00200000
	.4byte 0x02600000
	.4byte 0x000002c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000062
	.4byte 0x1010b05f
	.4byte 0xffffffff
	.4byte 0x10201063
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000007
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00015000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001c000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001e000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000007
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0010
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0025
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0026
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00018000
	.4byte 0xffff0018
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0070
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff00c3
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff00bb
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00020000
	.4byte 0xffff0073
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x0200d284
	.4byte 0x00110010
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008941
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008731
	.4byte 0x00008d15
	.4byte 0x08a80416
	.4byte 0x02008731
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001b60
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001b61
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001b62
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001b63
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001b64
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001b65
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001b66
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00002535
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x00002536
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002537
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002538
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00002539
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x0000253a
	.4byte 0x50009705
	.4byte 0x08a7000a
	.4byte 0x0200806d
	.4byte 0x00008515
	.4byte 0x08a30008
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x08a2000e
	.4byte 0x020080c9
	.4byte 0x10008c15
	.4byte 0x08a0000a
	.4byte 0x02008191
	.4byte 0x00008c15
	.4byte 0x08a0000a
	.4byte 0x02008201
	.4byte 0x10008c15
	.4byte 0x08a1000b
	.4byte 0x02008191
	.4byte 0x00008c15
	.4byte 0x08a1000b
	.4byte 0x02008201
	.4byte 0x10008c15
	.4byte 0x08a40010
	.4byte 0x02008395
	.4byte 0x00008c15
	.4byte 0x08a40010
	.4byte 0x0200842d
	.4byte 0x10008c15
	.4byte 0x08a50011
	.4byte 0x02008191
	.4byte 0x00008c15
	.4byte 0x08a50011
	.4byte 0x02008201
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008191
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008201
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x020086ed
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x020086ed
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x020086ed
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008515
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02008515
	.4byte 0x0000a602
	.4byte 0xffff000c
	.4byte 0x02008955
	.4byte 0x0000b602
	.4byte 0xffff000c
	.4byte 0x02008955
	.4byte 0x00002602
	.4byte 0xffff000c
	.4byte 0x02008955
	.4byte 0x00003602
	.4byte 0xffff000c
	.4byte 0x02008955
	.4byte 0x0000a602
	.4byte 0xffff000b
	.4byte 0x02008955
	.4byte 0x0000b602
	.4byte 0xffff000b
	.4byte 0x02008955
	.4byte 0x00002602
	.4byte 0xffff000b
	.4byte 0x02008955
	.4byte 0x00003602
	.4byte 0xffff000b
	.4byte 0x02008955
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008839
	.4byte 0x00000002
	.4byte 0x09050015
	.4byte 0x0200a049
	.4byte 0x00000002
	.4byte 0x090f0016
	.4byte 0x0200aa09
	.4byte 0x00000002
	.4byte 0x08ac0017
	.4byte 0x02009575
	.4byte 0x00000002
	.4byte 0x08bf0018
	.4byte 0x02008969
	.4byte 0x00000002
	.4byte 0x08be0019
	.4byte 0x02008959
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0xfffa0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xffb00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xffe00000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xffe80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000026
