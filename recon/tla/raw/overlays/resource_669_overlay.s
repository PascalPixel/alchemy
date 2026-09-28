.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008251, 0x02008039, 0x02008045, 0x0200804d, 0x02008249, 0x02008041, 0x020083e5
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb3fc
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb42c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x0200b45c
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r0, r1, #0
	bl 0x0200b150
	ldr	r3, [r0, #8]
	asrs	r6, r3, #20
	ldr	r3, [r0, #16]
	asrs	r5, r3, #20
	cmp	r6, #24
	bne.n	.L_02000078
	cmp	r5, #40
	bne.n	.L_02000078
	movs	r0, #144
	lsls	r0, r0, #4
	bl 0x0200b0f0
.L_02000078:
	cmp	r6, #23
	bne.n	.L_02000088
	cmp	r5, #40
	bne.n	.L_02000088
	movs	r0, #144
	lsls	r0, r0, #4
	bl 0x0200b0f8
.L_02000088:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #1
	bl 0x0200b0f0
	ldr	r5, [pc, #136]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200b150
	movs	r6, #192
	lsls	r6, r6, #11
	str	r6, [r0, #40]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200b158
	movs	r2, #244
	ldr	r0, [r5, #0]
	movs	r1, #136
	lsls	r2, r2, #1
	bl 0x0200b180
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #236
	movs	r0, #15
	movs	r1, #120
	lsls	r2, r2, #1
	bl 0x0200b170
	movs	r2, #220
	adds	r2, #255
	movs	r1, #136
	movs	r0, #14
	bl 0x0200b178
	movs	r0, #15
	bl 0x0200b150
	str	r6, [r0, #40]
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200b2a8
	ldr	r0, [r5, #0]
	bl 0x0200b190
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200b1a0
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #15
	bl 0x0200b190
	movs	r0, #159
	bl 0x0200b2a8
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r2, #144
	movs	r1, #202
	lsls	r2, r2, #4
	adds	r1, #255
	adds	r2, #2
	bl 0x0200b148
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	movs	r0, #12
	bl 0x0200b150
	adds	r6, r0, #0
	bl 0x0200b130
	movs	r0, #0
	bl 0x0200b240
	ldr	r0, [pc, #236]
	bl 0x0200b1c8
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b1e0
	movs	r1, #144
	lsls	r1, r1, #5
	adds	r1, #16
	movs	r0, #8
	bl 0x0200b268
	bl 0x0200b278
	mov	r0, r8
	bl 0x0200b150
	movs	r1, #2
	bl 0x0200b290
	movs	r0, #201
	bl 0x0200b2a8
	adds	r7, r6, #0
	movs	r0, #30
	bl 0x0200b128
	adds	r7, #85
	movs	r3, #4
	strb	r3, [r7, #0]
	movs	r5, #39
.L_02000196:
	ldr	r3, [r6, #12]
	movs	r2, #204
	lsls	r2, r2, #6
	adds	r2, #51
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200b128
	cmp	r5, #0
	bge.n	.L_02000196
	movs	r0, #70
	bl 0x0200b128
	mov	r0, r8
	bl 0x0200b150
	movs	r1, #0
	bl 0x0200b290
	bl 0x0200b288
	bl 0x0200b280
	movs	r0, #8
	bl 0x0200b270
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r6, #72]
	movs	r0, #5
	bl 0x0200b128
	movs	r0, #132
	bl 0x0200b2a8
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200b0e8
	cmp	r0, #0
	bne.n	.L_0200023a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #174
	bl 0x0200b0f0
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #10
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #10
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #175
	lsls	r1, r1, #8
	movs	r0, #10
	adds	r1, #255
	movs	r2, #0
	bl 0x0200b1e8
.L_0200023a:
	bl 0x0200b138
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x1c10
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb654
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #380]
	adds	r2, #93
	str	r2, [r3, #0]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r6, r5, r3
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #99
	bne.n	.L_02000296
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #170
	bl 0x0200b0f0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200b0f0
	movs	r3, #245
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #3
	strh	r3, [r2, #0]
	strh	r3, [r6, #0]
	bl 0x02008c5c
.L_02000296:
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x0200b0e8
	cmp	r0, #0
	beq.n	.L_020002c6
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #9
	b.n	.L_02000302
.L_020002c6:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x0200b0e8
	cmp	r0, #0
	beq.n	.L_0200030c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x0200b0e8
	cmp	r0, #0
	bne.n	.L_02000320
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #11
.L_02000302:
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	b.n	.L_02000320
.L_0200030c:
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
.L_02000320:
	movs	r0, #144
	lsls	r0, r0, #4
	bl 0x0200b0e8
	cmp	r0, #0
	beq.n	.L_0200033a
	movs	r1, #196
	movs	r2, #162
	movs	r0, #13
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b198
.L_0200033a:
	movs	r0, #13
	bl 0x0200b150
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	movs	r1, #202
	strb	r3, [r0, #0]
	adds	r1, #255
	movs	r0, #15
	bl 0x0200b298
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #1
	bl 0x0200b0e8
	cmp	r0, #0
	bne.n	.L_020003a2
	movs	r0, #10
	adds	r0, #255
	bl 0x0200b0e8
	cmp	r0, #0
	bne.n	.L_020003d6
	movs	r0, #14
	bl 0x0200b150
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #128
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200b150
	adds	r1, r0, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #64
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #12]
	ldr	r3, [r0, #20]
	adds	r3, r3, r2
	str	r3, [r0, #20]
	b.n	.L_020003d6
.L_020003a2:
	movs	r1, #136
	movs	r0, #14
	lsls	r1, r1, #16
	ldr	r2, [pc, #52]
	bl 0x0200b198
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #2
	bl 0x0200b0e8
	cmp	r0, #0
	bne.n	.L_020003cc
	movs	r1, #240
	movs	r2, #236
	movs	r0, #15
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x0200b198
	b.n	.L_020003d6
.L_020003cc:
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
.L_020003d6:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0x01db
	movs	r0, #0
	bx	lr
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
	bl 0x0200b110
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_0200042c
	movs	r1, #1
	ldr	r5, [r6, #80]
	bl 0x0200b100
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	bl 0x0200b108
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
.L_0200042c:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xb720
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	bl 0x0200b150
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
	bl 0x020083e8
	movs	r0, #151
	bl 0x0200b2a8
	movs	r0, #15
	bl 0x0200b128
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
	bl 0x020083e8
	movs	r0, #151
	bl 0x0200b2a8
	movs	r0, #15
	bl 0x0200b128
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r2, [r5, #16]
	add	r1, r8
	mov	r3, fp
	add	r0, sl
	str	r6, [sp, #0]
	bl 0x020083e8
	movs	r0, #151
	bl 0x0200b2a8
	movs	r0, #15
	bl 0x0200b128
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #12]
	ldr	r3, [pc, #40]
	ldr	r2, [r5, #16]
	add	r1, r8
	adds	r0, r0, r3
	mov	r3, r9
	str	r6, [sp, #0]
	bl 0x020083e8
	movs	r0, #151
	bl 0x0200b2a8
	movs	r0, #15
	bl 0x0200b128
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
	.2byte 0xb520
	bl 0x0200b130
	movs	r0, #0
	bl 0x0200b240
	ldr	r0, [pc, #1016]
	bl 0x0200b1c8
	movs	r5, #192
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	lsls	r5, r5, #18
	bl 0x0200b210
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
	adds	r3, #8
	str	r2, [r3, #0]
	movs	r0, #8
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #228
	movs	r2, #204
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b188
	movs	r2, #16
	movs	r3, #128
	movs	r0, #25
	movs	r1, #0
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200b248
	movs	r3, #128
	movs	r0, #5
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #7
	bl 0x0200b248
	movs	r2, #16
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #16
	negs	r2, r2
	movs	r0, #6
	bl 0x0200b248
	movs	r0, #6
	bl 0x0200b190
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #188
	movs	r1, #1
	movs	r2, #208
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #9
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b1b0
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b1b0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #9
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #9
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #9
	bl 0x0200b1f8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #9
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #9
	bl 0x0200b1f8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #9
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #9
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #9
	bl 0x02008438
	movs	r0, #151
	bl 0x0200b2a8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #9
	bl 0x0200b1f8
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #9
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
.L_02000858:
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200b1f8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #4
	adds	r1, #255
.L_0200087c:
	movs	r2, #30
	movs	r0, #5
	bl 0x0200b1f8
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b1e8
.L_0200089a:
	movs	r2, #0
	movs	r1, #0
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	movs	r0, #25
	lsls	r1, r1, #1
	bl 0x0200b200
	movs	r0, #4
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200b200
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	b.n	.L_02000900
	.2byte 0x1b67
	.2byte 0x0000
.L_02000900:
	movs	r0, #9
	bl 0x0200b1f8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	movs	r2, #192
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	movs	r0, #9
	lsls	r1, r1, #9
	bl 0x0200b158
	ldr	r1, [pc, #792]
	movs	r0, #9
	bl 0x0200b160
	movs	r0, #5
	bl 0x0200b128
	ldr	r1, [pc, #780]
	movs	r0, #8
	bl 0x0200b160
	movs	r0, #8
	bl 0x0200b168
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #224
	movs	r1, #1
	movs	r2, #208
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b1b0
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b1b0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #5
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200b1e8
	movs	r0, #45
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #0
	movs	r1, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #5
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #25
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #25
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b1b0
	movs	r0, #25
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b1b0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #4
	adds	r1, #255
	movs	r2, #50
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #10
	adds	r1, #255
	movs	r2, #50
	movs	r0, #25
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	adds	r0, #25
	bl 0x0200b1d0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b140
	cmp	r0, #0
	bne.n	.L_02000b8e
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000bc2
.L_02000b8e:
	bl 0x0200b2a0
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	ldr	r2, [r5, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
.L_02000bc2:
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #9
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b1b0
	movs	r0, #9
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b1b0
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	negs	r2, r2
	movs	r1, #0
	movs	r0, #9
	bl 0x0200b250
	movs	r0, #10
.L_02000c22:
	bl 0x0200b128
	ldr	r0, [pc, #44]
	movs	r1, #99
	bl 0x0200b228
	ldr	r3, [pc, #40]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #10
	movs	r1, #1
	bl 0x0200b220
	bl 0x0200b138
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x0200b33c
	.4byte 0x0200b2b0
	.4byte 0x00000063
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	sub	sp, #28
	bl 0x0200b130
	movs	r0, #0
	bl 0x0200b240
	ldr	r0, [pc, #912]
	bl 0x0200b1c8
	movs	r1, #218
	movs	r2, #208
	lsls	r2, r2, #17
	movs	r0, #8
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b260
	movs	r0, #8
	bl 0x0200b150
	movs	r1, #0
	bl 0x0200b120
	movs	r1, #228
	movs	r2, #216
	lsls	r2, r2, #17
	movs	r0, #9
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200b260
	movs	r0, #9
	bl 0x0200b150
	movs	r1, #0
	bl 0x0200b120
	movs	r0, #9
	bl 0x0200b150
	movs	r5, #128
	lsls	r5, r5, #7
	movs	r1, #228
	movs	r2, #230
	lsls	r2, r2, #17
	strh	r5, [r0, #6]
	lsls	r1, r1, #17
	movs	r0, #22
	bl 0x0200b198
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #22
	bl 0x0200b260
	movs	r0, #22
	bl 0x0200b150
	movs	r1, #0
	bl 0x0200b120
	movs	r1, #210
	movs	r2, #212
	lsls	r2, r2, #17
	movs	r0, #23
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #23
	bl 0x0200b260
	movs	r0, #23
	bl 0x0200b150
	movs	r1, #0
	bl 0x0200b120
	movs	r1, #194
	movs	r2, #220
	lsls	r2, r2, #17
	movs	r0, #24
	lsls	r1, r1, #17
	bl 0x0200b198
	adds	r1, r5, #0
	movs	r0, #24
	bl 0x0200b260
	movs	r0, #24
	bl 0x0200b150
	movs	r1, #0
	bl 0x0200b120
	movs	r1, #228
	movs	r2, #188
	movs	r0, #25
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #236
	movs	r2, #196
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #236
	movs	r2, #188
	movs	r0, #6
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #228
	movs	r2, #138
	movs	r0, #10
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b198
	movs	r1, #236
	movs	r2, #140
	movs	r0, #21
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #220
	movs	r2, #140
	movs	r0, #20
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #228
	movs	r2, #148
	movs	r0, #19
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #236
	movs	r2, #148
	movs	r0, #17
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #220
	movs	r2, #148
	movs	r0, #18
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #228
	movs	r2, #152
	movs	r0, #16
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200b198
	movs	r1, #128
	movs	r2, #128
	movs	r0, #21
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #20
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #19
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #18
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #16
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #4
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #25
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #5
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #6
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #10
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #22
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #23
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r1, #128
	movs	r2, #128
	movs	r0, #24
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b158
	movs	r0, #232
	movs	r1, #1
	movs	r2, #204
	movs	r3, #0
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b230
	bl 0x0200b238
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #5
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
.L_02000ef0:
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #25
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #25
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #191
	lsls	r1, r1, #8
	movs	r0, #4
	adds	r1, #255
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	lsls	r1, r1, #8
	adds	r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #50
	bl 0x0200b128
	movs	r0, #4
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r2, #0
	adds	r1, r5, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b1a8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #5
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b1d0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b140
	cmp	r0, #0
	bne.n	.L_0200100c
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #0
	movs	r2, #10
	movs	r0, #6
	bl 0x0200b1d8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200103c
	.2byte 0x0000
	.2byte 0x1b8b
	.2byte 0x0000
.L_0200100c:
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #6
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #6
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
.L_0200103c:
	movs	r1, #2
	movs	r0, #25
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b1f8
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200b1f8
	movs	r0, #10
	movs	r1, #1
	bl 0x0200b208
	bl 0x0200b218
	movs	r1, #228
	movs	r2, #252
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b188
	movs	r0, #10
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #236
	movs	r2, #212
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #10
	bl 0x0200b188
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #204
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #25
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #10
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #10
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #10
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #6
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #10
	movs	r1, #4
	bl 0x0200b1a8
	movs	r0, #10
	movs	r1, #0
	movs	r2, #15
	bl 0x0200b1d8
	movs	r1, #191
	lsls	r1, r1, #8
	movs	r0, #4
	adds	r1, #255
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	lsls	r1, r1, #8
	adds	r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #50
	bl 0x0200b128
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #10
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #10
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #4
	bl 0x0200b1a8
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #25
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #8
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1f8
	movs	r1, #0
	mov	r9, r1
	str	r1, [r0, #108]
	ldr	r2, [r0, #12]
	ldr	r1, [r0, #8]
	movs	r6, #176
	movs	r5, #128
	lsls	r6, r6, #13
	lsls	r5, r5, #12
	ldr	r3, [r0, #16]
	adds	r1, r1, r6
	adds	r2, r2, r5
	bl 0x0200b118
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #5
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b1e8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200b1f8
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #208
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #5
	bl 0x0200b1f8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #10
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b1a8
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b1d8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #8
	bl 0x0200b250
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1f8
	mov	r2, r9
	str	r2, [r0, #108]
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	adds	r1, r1, r6
	adds	r2, r2, r5
	bl 0x0200b118
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #25
	bl 0x0200b1f8
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #152
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #16
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #18
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #16
	bl 0x0200b1f8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	bl 0x0200b1f8
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #19
	bl 0x0200b1f8
	movs	r0, #19
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #19
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #19
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #19
	bl 0x0200b1c0
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #19
	movs	r1, #4
	bl 0x0200b1a8
	movs	r1, #2
	movs	r0, #18
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #16
	bl 0x0200b1f8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #18
	bl 0x0200b1f8
	movs	r1, #2
	adds	r1, #255
	movs	r2, #60
	movs	r0, #19
	bl 0x0200b1f8
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #17
	bl 0x0200b1f8
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #18
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #19
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #19
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #16
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #18
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #18
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #19
	bl 0x0200b1f8
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #19
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #172
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #25
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #0
	movs	r2, #16
	movs	r0, #17
	bl 0x0200b258
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #16
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #18
	movs	r1, #0
	bl 0x0200b258
	movs	r1, #1
	movs	r0, #16
	bl 0x0200b1a0
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #11
	mov	r8, r1
	movs	r3, #17
	movs	r1, #6
	str	r3, [sp, #8]
	str	r1, [sp, #20]
	movs	r2, #22
	mov	sl, r1
	mov	r3, r8
	mov	r1, r9
	str	r3, [sp, #12]
	str	r2, [sp, #16]
	str	r1, [sp, #24]
	movs	r3, #13
	movs	r2, #1
	movs	r5, #4
	movs	r6, #16
	movs	r0, #18
	movs	r1, #13
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b1f0
	movs	r0, #16
	movs	r1, #2
	bl 0x0200b1b8
	movs	r1, #2
	movs	r0, #18
	bl 0x0200b1c0
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #17
	bl 0x0200b1f8
	movs	r1, #10
	adds	r1, #255
	movs	r2, #60
	movs	r0, #16
	bl 0x0200b1f8
	movs	r1, #132
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x0200b1f8
	movs	r0, #16
	movs	r1, #4
	bl 0x0200b1a8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #16
	bl 0x0200b1f8
	movs	r1, #8
	adds	r1, #255
	movs	r2, #70
	movs	r0, #18
	bl 0x0200b1f8
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x0200b1f8
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #18
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #18
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #18
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #19
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #19
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #60
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #25
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #25
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #5
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #16
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #17
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #18
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #19
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #25
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #25
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #16
	movs	r1, #2
	bl 0x0200b1b8
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r3, #9
	movs	r2, #14
	str	r3, [sp, #8]
	str	r2, [sp, #16]
	mov	r3, r8
	mov	r1, sl
	mov	r2, r9
	str	r3, [sp, #12]
	str	r1, [sp, #20]
	str	r2, [sp, #24]
	movs	r3, #25
	movs	r2, #1
	movs	r0, #17
	movs	r1, #12
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200b1f0
	movs	r0, #17
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	movs	r0, #17
	lsls	r1, r1, #1
	bl 0x0200b200
	movs	r0, #16
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #16
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #224
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	bl 0x0200b1f8
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #232
	movs	r1, #1
	movs	r2, #216
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b1f8
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1f8
	movs	r3, #0
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	str	r3, [r0, #108]
	movs	r5, #128
	movs	r3, #176
	lsls	r5, r5, #12
	lsls	r3, r3, #13
	adds	r1, r1, r3
	adds	r2, r2, r5
	ldr	r3, [r0, #16]
	bl 0x0200b118
	movs	r0, #50
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #172
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #16
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #16
	movs	r1, #4
	bl 0x0200b1a8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #18
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b1b0
	movs	r0, #18
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b1b0
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #232
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b1d8
	movs	r0, #8
	bl 0x0200b150
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r3, #160
	lsls	r3, r3, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	adds	r1, r1, r3
	adds	r2, r2, r5
	movs	r0, #8
	bl 0x0200b198
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b260
	movs	r0, #8
	bl 0x0200b150
	movs	r1, #1
	bl 0x0200b120
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r2, #40
	movs	r0, #8
	movs	r1, #6
	bl 0x0200b1b0
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b1a8
	movs	r1, #228
	movs	r2, #220
	lsls	r2, r2, #17
	movs	r0, #9
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #0
	movs	r0, #9
	bl 0x0200b260
	movs	r0, #9
	bl 0x0200b150
	movs	r1, #1
	bl 0x0200b120
	movs	r1, #228
	movs	r2, #228
	lsls	r2, r2, #17
	movs	r0, #22
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #0
	movs	r0, #22
	bl 0x0200b260
	movs	r0, #22
	bl 0x0200b150
	movs	r1, #1
	bl 0x0200b120
	movs	r1, #212
	movs	r2, #212
	lsls	r2, r2, #17
	movs	r0, #23
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #0
	movs	r0, #23
	bl 0x0200b260
	movs	r0, #23
	bl 0x0200b150
	movs	r1, #1
	bl 0x0200b120
	movs	r1, #204
	movs	r2, #220
	lsls	r2, r2, #17
	movs	r0, #24
	lsls	r1, r1, #17
	bl 0x0200b198
	movs	r1, #0
	movs	r0, #24
	bl 0x0200b260
	movs	r0, #24
	bl 0x0200b150
	movs	r1, #1
	bl 0x0200b120
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #22
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #22
	movs	r1, #6
	movs	r2, #0
	bl 0x0200b1b0
	movs	r0, #23
	movs	r1, #6
	movs	r2, #0
	bl 0x0200b1b0
	movs	r0, #24
	movs	r1, #6
	movs	r2, #0
	bl 0x0200b1b0
	movs	r2, #40
	movs	r0, #9
	movs	r1, #6
	bl 0x0200b1b0
	movs	r0, #22
	movs	r1, #4
	bl 0x0200b1a0
	movs	r0, #23
	movs	r1, #4
	bl 0x0200b1a0
	movs	r0, #24
	movs	r1, #4
	bl 0x0200b1a0
	movs	r1, #4
	movs	r0, #9
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #184
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b1a8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #18
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #232
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #6
	movs	r2, #40
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b1f8
	movs	r1, #2
	movs	r0, #10
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #10
	bl 0x0200b1c0
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #130
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #10
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #9
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #22
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #15
	bl 0x0200b128
	movs	r0, #23
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #24
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #16
	movs	r0, #8
	movs	r1, #16
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #176
	movs	r0, #10
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r2, #32
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r0, #23
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #24
	movs	r1, #16
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #16
	movs	r2, #32
	movs	r0, #5
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b250
	movs	r1, #16
	movs	r2, #32
	negs	r2, r2
	negs	r1, r1
	movs	r0, #6
	bl 0x0200b258
	movs	r0, #5
	bl 0x0200b190
	movs	r0, #5
	movs	r1, #1
	bl 0x0200b1a0
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #9
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r0, #23
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r0, #24
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r0, #232
	movs	r1, #1
	movs	r2, #192
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #23
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #4
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #16
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r0, #22
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #23
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r0, #24
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r2, #16
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r0, #23
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #24
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r2, #16
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #23
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r0, #24
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r0, #232
	movs	r1, #1
	movs	r2, #168
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #9
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #22
	bl 0x0200b1f8
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #22
	bl 0x0200b1f8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #23
	bl 0x0200b1f8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #24
	bl 0x0200b1f8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #9
	bl 0x0200b1f8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #208
	lsls	r2, r2, #17
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200b200
	movs	r0, #50
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #168
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #17
	bl 0x0200b150
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #228
	movs	r2, #156
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #17
	bl 0x0200b188
	movs	r0, #1
	bl 0x0200b128
	movs	r0, #17
	bl 0x0200b150
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #16
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #23
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #24
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #0
	movs	r1, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #16
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #18
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r2, #0
	movs	r1, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #32
	movs	r0, #9
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #23
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #24
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #148
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #19
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #19
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b1f8
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #18
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #18
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #18
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #19
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #19
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #19
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #19
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #18
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #19
	bl 0x0200b1f8
	movs	r0, #19
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #19
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #19
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #17
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #18
	movs	r1, #3
	bl 0x0200b1a8
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #19
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b158
	movs	r2, #32
	movs	r0, #19
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #16
	movs	r0, #19
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #0
	movs	r2, #0
	movs	r0, #19
	bl 0x0200b198
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #20
	movs	r1, #16
	movs	r2, #16
	bl 0x0200b258
	movs	r2, #0
	movs	r1, #0
	movs	r0, #20
	bl 0x0200b1e8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #20
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #20
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	ldr	r5, [pc, #1016]
	movs	r0, #21
	adds	r1, r5, #0
	bl 0x0200b160
	movs	r0, #10
	bl 0x0200b128
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200b160
	movs	r0, #10
	bl 0x0200b128
	adds	r1, r5, #0
	movs	r0, #9
	bl 0x0200b160
	movs	r0, #10
	bl 0x0200b128
	adds	r1, r5, #0
	movs	r0, #22
	bl 0x0200b160
	movs	r0, #10
	bl 0x0200b128
	adds	r1, r5, #0
	movs	r0, #23
	bl 0x0200b160
	movs	r0, #10
	bl 0x0200b128
	adds	r1, r5, #0
	movs	r0, #24
	bl 0x0200b160
	movs	r0, #75
	bl 0x0200b128
	movs	r0, #20
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b258
	movs	r2, #32
	movs	r0, #20
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #32
	movs	r0, #20
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #0
	movs	r2, #0
	movs	r0, #20
	bl 0x0200b198
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #18
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #16
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #18
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #16
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b258
	movs	r2, #32
	movs	r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #16
	movs	r0, #18
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r0, #18
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #192
	movs	r0, #18
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #16
	bl 0x0200b1f8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #232
	movs	r1, #1
	movs	r2, #160
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #25
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #18
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #4
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b250
	movs	r0, #25
	movs	r1, #16
	movs	r2, #0
	bl 0x0200b258
	movs	r2, #32
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #25
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #25
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #25
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #16
	bl 0x0200b1f8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #25
	bl 0x0200b1f8
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #16
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #16
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #0
	movs	r1, #0
	movs	r0, #18
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #18
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #32
	movs	r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #18
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #32
	movs	r0, #16
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b250
	movs	r1, #16
	movs	r0, #18
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x0200b198
	movs	r0, #16
	bl 0x0200b190
	movs	r1, #0
	movs	r2, #0
	movs	r0, #16
	bl 0x0200b198
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	adds	r1, #1
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #40
	bl 0x0200b128
	movs	r1, #32
	movs	r2, #8
	negs	r1, r1
	negs	r2, r2
	movs	r0, #17
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #17
	b.n	.L_02002a40
	.2byte 0xb3c8
	.2byte 0x0200
.L_02002a40:
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r2, #16
	movs	r1, #24
	negs	r2, r2
	movs	r0, #17
	bl 0x0200b258
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #16
	movs	r2, #16
	bl 0x0200b258
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #17
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #25
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #0
	movs	r0, #25
	bl 0x0200b1d0
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b140
	cmp	r0, #0
	bne.n	.L_02002b7a
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #25
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #4
	bl 0x0200b1a8
	movs	r1, #0
	movs	r2, #10
	movs	r0, #17
	bl 0x0200b1d8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002bc0
.L_02002b7a:
	movs	r0, #40
	bl 0x0200b128
	bl 0x0200b2a0
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #17
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
.L_02002bc0:
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	bl 0x0200b1f8
	movs	r1, #0
	movs	r2, #10
	movs	r0, #17
	bl 0x0200b1d8
	movs	r0, #17
	bl 0x02008438
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #17
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #17
	bl 0x0200b1f8
	movs	r2, #10
	movs	r0, #17
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #17
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r2, #40
	movs	r0, #17
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b258
	movs	r1, #24
	movs	r0, #17
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b258
	movs	r1, #0
	movs	r2, #0
	movs	r0, #17
	bl 0x0200b198
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #232
	movs	r1, #1
	movs	r2, #200
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200b210
	bl 0x0200b218
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #10
	bl 0x0200b1f8
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b1e8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #128
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200b1f8
	movs	r0, #10
	movs	r1, #3
	bl 0x0200b1b8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x0200b200
	movs	r0, #40
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #3
	movs	r0, #10
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b1d8
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #10
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #10
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #236
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200b188
	movs	r0, #10
	movs	r1, #1
	bl 0x0200b208
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #16
	movs	r2, #32
	movs	r0, #25
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b250
	movs	r1, #16
	movs	r2, #32
	negs	r2, r2
	movs	r0, #4
	negs	r1, r1
	bl 0x0200b258
	movs	r0, #25
	movs	r1, #1
	bl 0x0200b1a0
	movs	r0, #25
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #236
	movs	r2, #160
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b188
	movs	r2, #32
	movs	r0, #5
	movs	r1, #16
	negs	r2, r2
	bl 0x0200b250
	movs	r2, #32
	movs	r0, #6
	movs	r1, #16
	negs	r2, r2
	bl 0x0200b250
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #236
	movs	r2, #132
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b188
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b210
	movs	r1, #176
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #176
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #220
	movs	r2, #132
	movs	r0, #10
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	bl 0x0200b188
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b198
	movs	r0, #30
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #5
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200b1d8
	movs	r1, #2
	movs	r0, #6
	bl 0x0200b1c0
	movs	r0, #25
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r1, #0
	movs	r2, #10
	adds	r0, #25
	bl 0x0200b1d8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #25
	bl 0x0200b1f8
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #10
	bl 0x0200b128
	movs	r0, #6
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b1e8
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #25
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #6
	bl 0x0200b1e8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #2
	movs	r0, #6
	bl 0x0200b1c0
	movs	r0, #10
	bl 0x0200b128
	movs	r1, #0
	movs	r0, #6
	bl 0x0200b1d0
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b1e8
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b140
	cmp	r0, #0
	bne.n	.L_02002fbe
	movs	r0, #30
	bl 0x0200b128
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b1a8
	movs	r0, #20
	bl 0x0200b128
	movs	r1, #0
	movs	r2, #10
	movs	r0, #5
	bl 0x0200b1d8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02002fe8
.L_02002fbe:
	movs	r0, #40
	bl 0x0200b128
	movs	r0, #5
	movs	r1, #4
	bl 0x0200b1a8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #5
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b1d8
.L_02002fe8:
	movs	r0, #25
	movs	r1, #3
	bl 0x0200b1a0
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b1a0
	movs	r1, #3
	movs	r0, #4
	bl 0x0200b1a8
	movs	r0, #30
	bl 0x0200b128
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #5
	ldr	r1, [pc, #208]
	adds	r2, #153
	bl 0x0200b158
	movs	r0, #5
	movs	r1, #2
	bl 0x0200b1a0
	ldr	r3, [pc, #196]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	bl 0x0200b150
	cmp	r0, #0
	beq.n	.L_0200303c
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200b170
.L_0200303c:
	movs	r0, #5
	bl 0x0200b190
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #140]
	adds	r2, #153
	bl 0x0200b158
	movs	r0, #6
	movs	r1, #2
	bl 0x0200b1a0
	ldr	r0, [r5, #0]
	bl 0x0200b150
	cmp	r0, #0
	beq.n	.L_0200307a
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200b170
.L_0200307a:
	movs	r0, #6
	bl 0x0200b190
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b198
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #25
	ldr	r1, [pc, #76]
	adds	r2, #153
	bl 0x0200b158
	movs	r0, #25
	movs	r1, #2
	bl 0x0200b1a0
	ldr	r0, [r5, #0]
	bl 0x0200b150
	cmp	r0, #0
	beq.n	.L_020030b8
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #25
	bl 0x0200b170
.L_020030b8:
	movs	r0, #25
	bl 0x0200b190
	movs	r2, #0
	movs	r0, #25
	movs	r1, #0
	bl 0x0200b198
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200b208
	bl 0x0200b138
	add	sp, #28
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x00013333
	.4byte 0x02000240
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8079, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80c1, 0x080c80c9, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81f1, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8281, 0x080c8291, 0x080c83a9, 0x080c83b9, 0x080c84e1, 0x080c85e9, 0x080c85f1, 0x080c85f9, 0x080c87a9, 0x080c87f9, 0x080c8801, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x080c8861, 0x080c8931, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c0000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00340000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0xffff0000
	.4byte 0x000001c8
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000063
	.4byte 0x10102062
	.4byte 0xffffffff
	.4byte 0x10203063
	.4byte 0xffffffff
	.4byte 0x10302063
	.4byte 0xffffffff
	.4byte 0x10405063
	.4byte 0xffffffff
	.4byte 0x10504063
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x018c0000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0001a000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00028000
	.4byte 0xffff01d2
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00014000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x01db0000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff0018
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0x08aa0014
	.4byte 0x020084f5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200813d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c12
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c13
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c14
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008129
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte 0x02008055
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x02008059
	.4byte 0x00000008
	.4byte 0xffff001e
	.4byte 0x02008055
	.4byte 0x00000009
	.4byte 0xffff001e
	.4byte 0x02008059
	.4byte 0x00008715
	.4byte 0x0901000e
	.4byte 0x0200808d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000026
