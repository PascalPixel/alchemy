.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200827d, 0x02008039, 0x02008045, 0x0200804d, 0x020081e9, 0x02008041, 0x020087a5
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x96ec
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x971c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	beq.n	.L_0200006a
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_0200006a
	ldr	r0, [pc, #20]
	b.n	.L_0200006c
.L_0200006a:
	ldr	r0, [pc, #20]
.L_0200006c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x00000065
	.4byte 0x02009868
	.2byte 0x9778
	.2byte 0x0200
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	bl 0x0200929c
	pop	{pc}
	.4byte 0x020096e4
	.4byte 0x00004770
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x020094b8
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x020094c0
	bl 0x02009590
	movs	r1, #0
	bl 0x02009440
	cmp	r0, #0
	bne.n	.L_020000d0
	movs	r0, #10
	bl 0x02009428
	adds	r0, r5, #1
	bl 0x020094b8
	b.n	.L_020000dc
.L_020000d0:
	movs	r0, #20
	bl 0x02009428
	adds	r0, r5, #2
	bl 0x020094b8
.L_020000dc:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x020094d0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x1c92
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #8]
	bl 0x02009580
	pop	{pc}
	.2byte 0x0000
	.2byte 0x9608
	.2byte 0x0200
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x020093e0
	movs	r0, #200
	movs	r1, #152
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #4
	bl 0x02009420
	movs	r0, #208
	movs	r1, #152
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #4
	bl 0x02009420
	movs	r0, #216
	movs	r1, #152
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #4
	bl 0x02009420
	movs	r0, #192
	movs	r1, #144
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #255
	lsls	r0, r0, #17
	bl 0x020095d0
	movs	r0, #144
	movs	r1, #192
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #4
	bl 0x02009420
	movs	r0, #168
	movs	r1, #192
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #4
	bl 0x02009420
	pop	{pc}
	push	{lr}
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x020093e8
	movs	r0, #200
	movs	r1, #152
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02009420
	movs	r0, #208
	movs	r1, #152
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02009420
	movs	r0, #216
	movs	r1, #152
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02009420
	movs	r0, #192
	movs	r1, #144
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x020095d0
	movs	r0, #144
	movs	r1, #192
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02009420
	movs	r0, #168
	movs	r1, #192
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x02009420
	pop	{pc}
	.4byte 0x049b23c0
	.4byte 0x22d06edb
	.4byte 0x324a0112
	.4byte 0x2202189b
	.2byte 0x801a
	.2byte 0x4770
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	beq.n	.L_02000206
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_02000206
	ldr	r0, [pc, #20]
	b.n	.L_02000208
.L_02000206:
	ldr	r0, [pc, #20]
.L_02000208:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x00000065
	.4byte 0x02009a90
	.2byte 0x9a18
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x02009448
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #12
	cmp	r3, r2
	bge.n	.L_02000246
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #0
	b.n	.L_02000250
.L_02000246:
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #1
.L_02000250:
	strb	r3, [r2, #0]
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	bl 0x02009448
	ldr	r4, [pc, #20]
	ldr	r2, [r0, #12]
	movs	r3, #224
	lsls	r3, r3, #13
	ldr	r1, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #16]
	adds	r0, r4, #0
	bl 0x020095f0
	movs	r0, #0
	pop	{pc}
	.2byte 0x9be0
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #108]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #100]
	cmp	r2, r3
	bne.n	.L_020002a6
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl 0x020082f8
	b.n	.L_020002e6
.L_020002a6:
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_020002e6
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #8
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_020002d2
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	subs	r0, #54
	lsls	r2, r2, #1
	adds	r3, r3, r0
	adds	r2, #255
	b.n	.L_020002e0
.L_020002d2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
.L_020002e0:
	str	r2, [r3, #0]
	bl 0x0200851c
.L_020002e6:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000064
	.2byte 0x0065
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #32]
	ldr	r3, [pc, #524]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x02009448
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	ldrb	r2, [r5, #23]
	strb	r3, [r0, #0]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	movs	r1, #144
	strb	r3, [r5, #23]
	ldr	r0, [pc, #488]
	lsls	r1, r1, #3
	bl 0x020093c8
	movs	r0, #129
	lsls	r0, r0, #2
	movs	r6, #0
	bl 0x020093d8
	cmp	r0, #0
	beq.n	.L_02000344
	bl 0x020080fc
.L_02000344:
	movs	r0, #8
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	movs	r5, #192
	lsls	r5, r5, #12
	adds	r2, #85
	strb	r6, [r2, #0]
	movs	r0, #9
	str	r5, [r3, #12]
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	adds	r2, #85
	strb	r6, [r2, #0]
	movs	r0, #10
	str	r5, [r3, #12]
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	adds	r2, #85
	strb	r6, [r2, #0]
	movs	r0, #8
	str	r5, [r3, #12]
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #9
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #10
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #12
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	movs	r5, #128
	adds	r2, #85
	lsls	r5, r5, #13
	strb	r6, [r2, #0]
	movs	r0, #11
	str	r5, [r3, #12]
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	adds	r2, #85
	movs	r1, #128
	strb	r6, [r2, #0]
	movs	r0, #13
	str	r5, [r3, #12]
	lsls	r1, r1, #5
	bl 0x02009598
	movs	r0, #11
	movs	r1, #2
	bl 0x02009490
	movs	r0, #12
	movs	r1, #3
	bl 0x02009490
	movs	r0, #13
	movs	r1, #2
	bl 0x02009490
	movs	r0, #11
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #12
	movs	r1, #2
	bl 0x020094e0
	movs	r1, #3
	movs	r0, #13
	bl 0x020094e0
	movs	r0, #11
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #2
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #13
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #12
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
.L_0200043e:
	movs	r0, #13
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x020093d8
	cmp	r0, #0
	beq.n	.L_02000474
	movs	r1, #200
	movs	r2, #136
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x02009488
	movs	r1, #196
	movs	r2, #180
	movs	r0, #15
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x02009488
.L_02000474:
	movs	r0, #14
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	adds	r2, #85
	strb	r6, [r2, #0]
	movs	r2, #0
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	str	r6, [r3, #20]
	str	r6, [r3, #12]
	movs	r3, #4
	bl 0x02009420
	movs	r0, #15
	bl 0x02009448
	adds	r3, r0, #0
	adds	r2, r3, #0
	adds	r2, #85
	strb	r6, [r2, #0]
	movs	r2, #0
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	str	r6, [r3, #20]
	str	r6, [r3, #12]
	movs	r3, #4
	bl 0x02009420
	ldr	r0, [pc, #100]
	bl 0x02009578
	movs	r0, #16
	bl 0x02009448
	movs	r1, #9
	bl 0x02009560
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x020093d8
	cmp	r0, #0
	bne.n	.L_0200050c
	movs	r3, #13
	movs	r2, #58
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #58
	movs	r2, #12
	movs	r3, #7
	bl 0x02009408
	movs	r3, #25
	movs	r2, #66
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #58
	movs	r2, #2
	movs	r3, #9
	bl 0x02009408
	movs	r3, #15
	movs	r2, #74
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #58
	movs	r2, #10
	movs	r3, #5
	bl 0x02009408
.L_0200050c:
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x02008221
	.2byte 0x9608
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #624]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r6, #32
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #8
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #0
	bl 0x02009520
	ldr	r5, [pc, #584]
	movs	r0, #0
	bl 0x020095e0
	movs	r3, #128
	movs	r2, #15
	lsls	r3, r3, #23
	adds	r0, r5, #0
	movs	r1, #8
	bl 0x020095e8
	movs	r1, #144
	ldr	r0, [pc, #564]
	lsls	r1, r1, #3
	bl 0x020093c8
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #8
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000590
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #76
	adds	r3, r3, r2
	movs	r2, #3
	strh	r2, [r3, #0]
.L_02000590:
	bl 0x02009568
	movs	r1, #128
	movs	r2, #10
	lsls	r1, r1, #2
.L_0200059a:
	movs	r3, #11
	movs	r0, #0
	bl 0x02009570
	movs	r0, #10
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #2
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	ldr	r0, [pc, #472]
	bl 0x02009230
	movs	r0, #9
	bl 0x02009448
	movs	r1, #0
	bl 0x02009410
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #5
	bl 0x02009598
	movs	r0, #12
	movs	r1, #0
	bl 0x02009490
	movs	r0, #13
	movs	r1, #1
	bl 0x02009490
	movs	r0, #14
	movs	r1, #2
	bl 0x02009490
	movs	r0, #12
	movs	r1, #1
	bl 0x020094e0
	movs	r1, #1
	movs	r0, #13
	bl 0x020094e0
	movs	r0, #12
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #13
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x02009448
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #13
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #14
	bl 0x02009448
	movs	r1, #6
	bl 0x02009418
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x020093d8
	cmp	r0, #0
	beq.n	.L_0200066c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	b.n	.L_020006ee
.L_0200066c:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	bl 0x020093d8
	cmp	r0, #0
	beq.n	.L_020006ee
	movs	r2, #0
	movs	r0, #23
	movs	r1, #0
	bl 0x02009488
	movs	r0, #18
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #16
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #17
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #19
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #20
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x020093d8
	cmp	r0, #0
	bne.n	.L_020006d8
	movs	r1, #178
	movs	r2, #216
	movs	r0, #17
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x02009488
	movs	r1, #186
	movs	r2, #216
	movs	r0, #19
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x02009488
	b.n	.L_02000748
.L_020006d8:
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	b.n	.L_02000748
.L_020006ee:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r0, #23
	bl 0x02009448
	ldr	r3, [pc, #88]
	str	r3, [r0, #108]
.L_02000748:
	ldr	r3, [pc, #68]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #9
	bne.n	.L_0200078e
	movs	r0, #10
	adds	r0, #255
	bl 0x020093d8
	cmp	r0, #0
	bne.n	.L_0200078e
	bl 0x02009510
	bl 0x02009518
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x02009450
	movs	r0, #8
	movs	r1, #0
	movs	r2, #16
	bl 0x02009478
	movs	r0, #8
	bl 0x02009480
.L_0200078e:
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009be0
	.4byte 0x02008259
	.4byte 0x020096e4
	.2byte 0x9221
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #100]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #92]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_02000804
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x020093d8
	cmp	r0, #0
	beq.n	.L_02000804
	movs	r3, #21
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #23
	movs	r1, #46
	movs	r2, #4
	movs	r3, #5
	bl 0x02009408
	movs	r3, #84
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #86
	movs	r1, #44
	movs	r2, #5
	movs	r3, #6
	bl 0x02009408
	movs	r3, #20
	movs	r2, #13
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #22
	movs	r1, #45
	movs	r2, #5
	movs	r3, #6
.L_02000800:
	bl 0x02009400
.L_02000804:
	movs	r0, #0
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000065
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #171
	sub	sp, #12
	bl 0x020093d8
	cmp	r0, #0
	bne.n	.L_02000832
	bl 0x02009206
.L_02000832:
	movs	r0, #245
	lsls	r0, r0, #3
	adds	r0, #255
	bl 0x020093d8
	cmp	r0, #1
	bne.n	.L_02000844
	bl 0x02009206
.L_02000844:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #189
	bl 0x020093e0
	bl 0x02009430
	movs	r0, #0
	bl 0x02009528
	movs	r1, #128
	movs	r2, #128
	movs	r0, #22
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009450
	movs	r1, #128
	movs	r2, #128
	movs	r0, #21
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009450
	movs	r1, #128
	movs	r2, #128
	movs	r0, #17
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009450
	movs	r1, #128
	movs	r2, #128
	movs	r0, #19
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009450
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #20
	bl 0x02009450
	ldr	r0, [pc, #1020]
	bl 0x020094b8
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #200
	movs	r2, #144
	movs	r0, #4
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl 0x02009470
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r0, #186
	movs	r1, #1
	movs	r2, #132
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x02009500
	bl 0x02009508
	movs	r0, #20
	bl 0x02009428
	movs	r1, #3
	movs	r0, #18
	bl 0x02009498
	movs	r0, #20
	bl 0x02009428
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #3
	movs	r0, #22
	bl 0x02009498
	movs	r0, #30
	bl 0x02009428
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #18
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r0, #21
	movs	r1, #6
	movs	r2, #15
	bl 0x020094a0
	movs	r0, #21
	movs	r1, #6
	movs	r2, #23
	bl 0x020094a0
	movs	r0, #21
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x020094e8
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #22
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #2
	movs	r0, #16
	bl 0x020094b0
	movs	r0, #10
	bl 0x02009428
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #22
	bl 0x020094e8
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r1, #8
	adds	r1, #255
	movs	r2, #50
	movs	r0, #22
	bl 0x020094e8
	movs	r0, #186
	movs	r1, #1
	movs	r2, #156
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x02009500
	bl 0x02009508
	movs	r0, #20
	bl 0x02009428
	movs	r0, #21
	movs	r1, #3
	bl 0x02009490
	movs	r0, #24
	movs	r1, #3
	bl 0x020094a8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #24
	bl 0x020094f0
	movs	r0, #40
	bl 0x02009428
	movs	r0, #24
	movs	r1, #1
	bl 0x020094f8
	bl 0x02009508
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	movs	r0, #24
	lsls	r1, r1, #9
	bl 0x02009450
	ldr	r1, [pc, #664]
	movs	r0, #24
	bl 0x02009458
	movs	r0, #20
	bl 0x02009428
	movs	r1, #3
	movs	r0, #21
	bl 0x02009498
	movs	r0, #20
	bl 0x02009428
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #21
	bl 0x020094d8
	movs	r0, #24
	bl 0x02009460
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x020094d8
	movs	r0, #186
	movs	r1, #1
	movs	r2, #136
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x02009500
	bl 0x02009508
	movs	r0, #10
	bl 0x02009428
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #24
	bl 0x020094e8
	movs	r2, #10
	movs	r0, #24
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #4
	movs	r0, #24
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #24
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #24
	bl 0x020094e8
	movs	r2, #10
	movs	r1, #0
	movs	r0, #24
	bl 0x020094c8
	movs	r0, #21
	bl 0x02009448
	adds	r5, r0, #0
	movs	r0, #15
	bl 0x02009448
	movs	r1, #144
	lsls	r1, r1, #5
	adds	r6, r0, #0
	adds	r1, #16
	movs	r0, #8
	bl 0x020095a0
	bl 0x020095b0
	movs	r0, #21
	bl 0x02009448
	movs	r1, #2
	bl 0x020095c8
	movs	r0, #201
	bl 0x02009600
	movs	r0, #40
	bl 0x02009428
	adds	r0, r5, #0
	bl 0x02009530
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	mov	r8, r0
	ldr	r3, [r6, #12]
	str	r6, [r0, #104]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r5, #4]
	movs	r1, #192
	ldr	r3, [r6, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	lsls	r1, r1, #8
	bl 0x020093d0
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #0]
	mov	r0, r8
	bl 0x020093f8
	mov	r0, r8
	bl 0x02009538
	mov	r0, r8
	movs	r1, #4
	bl 0x020093f0
	ldr	r3, [pc, #376]
	movs	r1, #192
	str	r3, [r6, #108]
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #128
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x020094e8
	movs	r0, #10
	bl 0x02009428
	movs	r1, #3
	movs	r0, #17
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r0, #19
	movs	r1, #3
	bl 0x02009498
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #20
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r0, #202
	movs	r1, #1
	movs	r2, #144
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x02009500
	bl 0x02009508
	movs	r0, #4
	movs	r1, #3
	bl 0x020094a8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x020094f0
	movs	r0, #50
	bl 0x02009428
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #60
	movs	r0, #20
	bl 0x020094e8
	movs	r0, #186
	movs	r1, #1
	movs	r2, #136
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x02009500
	bl 0x02009508
	movs	r0, #20
	bl 0x02009428
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #20
	bl 0x02009550
	movs	r0, #20
	bl 0x02009428
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #20
	bl 0x020094e8
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #18
	bl 0x020094d8
	movs	r0, #40
	bl 0x02009428
	movs	r0, #18
	movs	r1, #3
	bl 0x020094a8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #18
	bl 0x020094f0
	movs	r0, #40
	bl 0x02009428
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #18
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r0, #18
	movs	r1, #0
	movs	r2, #20
	bl 0x020094c8
	movs	r1, #128
	movs	r2, #30
	movs	r5, #0
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x020094e8
	movs	r1, #0
	str	r5, [r6, #108]
	adds	r0, r6, #0
	bl 0x02009418
	mov	r0, r8
	bl 0x02009540
	movs	r0, #21
	bl 0x02009448
	movs	r1, #0
	bl 0x020095c8
	bl 0x020095c0
	bl 0x020095b8
	movs	r0, #8
	bl 0x020095a8
	movs	r0, #30
	bl 0x02009428
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	b.n	.L_02000ca8
	.2byte 0x0000
	.4byte 0x00001c77
	.4byte 0x02009610
	.2byte 0x9229
	.2byte 0x0200
.L_02000ca8:
	bl 0x020094d8
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #176
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #21
	movs	r1, #3
	bl 0x020094a8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x020094f0
	movs	r0, #40
	bl 0x02009428
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #21
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r1, #4
	movs	r0, #18
	bl 0x02009498
	movs	r0, #20
	bl 0x02009428
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #20
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #18
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #22
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #24
	bl 0x020094d8
	movs	r0, #40
	bl 0x02009428
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x020094e8
	movs	r1, #4
	movs	r0, #21
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #21
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r1, #144
	lsls	r1, r1, #5
	adds	r1, #16
	movs	r0, #8
	bl 0x020095a0
	bl 0x020095b0
	movs	r0, #21
	bl 0x02009448
	movs	r1, #2
	bl 0x020095c8
	movs	r0, #201
	bl 0x02009600
	movs	r0, #40
	bl 0x02009428
	movs	r2, #10
	movs	r1, #0
	movs	r0, #18
	bl 0x020094c8
	movs	r0, #21
	bl 0x02009448
	movs	r1, #0
	bl 0x020095c8
	bl 0x020095c0
	bl 0x020095b8
	movs	r0, #8
	bl 0x020095a8
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #128
	movs	r0, #20
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #176
	movs	r0, #24
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #40
	bl 0x02009428
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #21
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #21
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #10
	adds	r1, #255
	movs	r2, #50
	movs	r0, #21
	bl 0x020094e8
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x020094e8
	movs	r1, #2
	movs	r0, #21
	bl 0x020094b0
	movs	r0, #20
	bl 0x02009428
	movs	r0, #21
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #192
	movs	r0, #16
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #20
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #18
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #160
	movs	r0, #22
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #24
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r1, #2
	movs	r0, #21
	adds	r1, #255
	bl 0x020094f0
	movs	r2, #20
	movs	r0, #21
	movs	r1, #0
	bl 0x020094c8
	ldr	r3, [pc, #764]
	movs	r1, #144
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #760]
	bl 0x020093c8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #24
	bl 0x020094e8
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r1, #3
	movs	r0, #18
	bl 0x02009498
	movs	r0, #30
	bl 0x02009428
	movs	r1, #3
	movs	r0, #22
	bl 0x02009498
	movs	r0, #20
	bl 0x02009428
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #22
	movs	r1, #0
	movs	r2, #8
	bl 0x02009550
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #22
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #2
	movs	r0, #21
	bl 0x020094b0
	movs	r0, #10
	bl 0x02009428
	movs	r1, #128
	movs	r0, #21
	lsls	r1, r1, #1
	bl 0x020094f0
	movs	r2, #10
	movs	r0, #21
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #3
	movs	r0, #22
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r2, #0
	movs	r1, #0
	movs	r0, #21
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r1, #3
	movs	r0, #21
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r2, #10
	movs	r0, #21
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #3
	movs	r0, #22
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #22
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #3
	movs	r0, #21
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r0, #21
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #240
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #22
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #3
	movs	r0, #24
	bl 0x02009498
	movs	r0, #10
	bl 0x02009428
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x020094c8
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #22
	bl 0x020094d8
	movs	r0, #20
	bl 0x02009428
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #10
	adds	r0, #22
	movs	r1, #0
	bl 0x020094c8
	movs	r1, #3
	movs	r0, #18
	bl 0x02009498
	movs	r0, #30
	bl 0x02009428
	movs	r1, #3
	movs	r0, #22
	bl 0x02009498
	movs	r0, #30
	bl 0x02009428
	movs	r1, #128
	movs	r0, #22
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094d8
	movs	r2, #0
	movs	r1, #0
	movs	r0, #21
	bl 0x020094d8
	movs	r0, #30
	bl 0x02009428
	movs	r1, #3
	movs	r0, #22
	bl 0x02009490
	movs	r0, #30
	bl 0x02009428
	movs	r1, #3
	movs	r0, #21
	bl 0x02009498
	movs	r0, #20
	bl 0x02009428
	movs	r0, #24
	movs	r1, #21
	bl 0x02009558
	movs	r2, #16
	movs	r0, #22
	movs	r1, #0
	negs	r2, r2
	bl 0x02009548
	movs	r2, #16
	movs	r0, #21
	movs	r1, #0
	negs	r2, r2
	bl 0x02009550
	movs	r0, #22
	movs	r1, #52
	movs	r2, #0
	bl 0x02009548
	movs	r2, #0
	movs	r1, #52
	movs	r0, #21
	bl 0x02009550
	movs	r0, #15
	bl 0x02009428
	movs	r0, #22
	movs	r1, #3
	bl 0x020094a8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #22
	bl 0x020094f0
	movs	r0, #40
	bl 0x02009428
	movs	r0, #4
	movs	r1, #21
	bl 0x02009558
	movs	r0, #22
	movs	r1, #2
	bl 0x020094e0
	movs	r0, #21
	movs	r1, #2
	bl 0x020094e0
	ldr	r5, [pc, #200]
	movs	r0, #22
	adds	r1, r5, #0
	bl 0x02009458
	movs	r0, #10
	bl 0x02009428
	adds	r1, r5, #0
	movs	r0, #21
	bl 0x02009458
	movs	r0, #10
	bl 0x02009428
	movs	r0, #20
	movs	r1, #16
	movs	r2, #0
	bl 0x02009550
	movs	r1, #192
	movs	r0, #20
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r0, #17
	movs	r1, #0
	movs	r2, #16
	bl 0x02009548
	movs	r1, #0
	movs	r2, #16
	movs	r0, #19
	bl 0x02009550
	movs	r0, #5
	bl 0x02009428
	movs	r1, #192
	movs	r0, #17
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #192
	movs	r0, #19
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094d8
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #18
	bl 0x020094d8
	movs	r0, #21
	bl 0x02009460
	movs	r0, #24
	bl 0x02009468
	movs	r0, #4
	bl 0x02009468
	movs	r0, #22
	movs	r1, #0
	movs	r2, #0
	bl 0x02009488
	movs	r2, #0
	movs	r1, #0
	movs	r0, #21
	bl 0x02009488
	movs	r0, #24
	bl 0x02009448
	movs	r1, #9
	bl 0x02009560
	ldr	r3, [pc, #40]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x020094f8
	bl 0x02009438
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02009c08
	.4byte 0x02008815
	.4byte 0x02009674
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x020095d8
	pop	{pc}
	push	{lr}
	bl 0x020095f8
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_02001294
	adds	r7, r0, #0
.L_02001246:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x02009448
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
	bl 0x02009410
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
	bl 0x02009320
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02001246
.L_02001294:
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
	b.n	.L_02001308
.L_020012b8:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_02001304
	adds	r0, r7, #0
	bl 0x02009448
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_020012e0
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_020012e0:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_02001312
	adds	r0, r5, #0
	bl 0x020093e0
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x02009320
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_02001312
.L_02001304:
	adds	r5, #6
	movs	r1, #255
.L_02001308:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_020012b8
.L_02001312:
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
.L_0200132c:
	bl 0x02009448
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
	bl 0x02009588
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x020093d8
	cmp	r0, #0
	beq.n	.L_02001394
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_02001380
	cmp	r6, #1
	bcc.n	.L_02001376
	cmp	r6, #2
	beq.n	.L_0200138a
	b.n	.L_020013c2
.L_02001376:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x020093f0
	b.n	.L_020013c2
.L_02001380:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x020093f0
	b.n	.L_020013c2
.L_0200138a:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x020093f0
	b.n	.L_020013c2
.L_02001394:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_020013b0
	cmp	r6, #1
	bcc.n	.L_020013a6
	cmp	r6, #2
	beq.n	.L_020013ba
	b.n	.L_020013c2
.L_020013a6:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x020093f0
	b.n	.L_020013c2
.L_020013b0:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x020093f0
	b.n	.L_020013c2
.L_020013ba:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x020093f0
.L_020013c2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.irp EntryTarget, 0x080000d1, 0x08000129, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x08020149, 0x080201e9, 0x080201f1, 0x08020219, 0x08020279, 0x08020361, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80d9, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c83a9, 0x080c83b9, 0x080c8481, 0x080c84e1, 0x080c8529, 0x080c8531, 0x080c8539, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8681, 0x080c86a9, 0x080c86e9, 0x080c8701, 0x080c8709, 0x080c8711, 0x080c8779, 0x080c87a9, 0x080c87f9, 0x080c8801, 0x080c8809, 0x080c8811, 0x080c8819, 0x080c8821, 0x080c8841, 0x080c8879, 0x080c88a1, 0x080c88a9, 0x080c88b1, 0x080c88c1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x02030010
	.4byte 0x0000ffff
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc40000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xfffc0000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000004
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00360000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00010009
	.4byte 0xffff0201
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
	.4byte 0x00000064
	.4byte 0x1010c05f
	.4byte 0xffffffff
	.4byte 0x10203064
	.4byte 0xffffffff
	.4byte 0x10302064
	.4byte 0xffffffff
	.4byte 0x10406065
	.4byte 0xffffffff
	.4byte 0x10507065
	.4byte 0xffffffff
	.4byte 0x00000065
	.4byte 0x10604064
	.4byte 0xffffffff
	.4byte 0x10705064
	.4byte 0xffffffff
	.4byte 0x1080805f
	.4byte 0xffffffff
	.4byte 0x1090a065
	.4byte 0xffffffff
	.4byte 0x10a09065
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00020000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00028000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00020000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00028000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff02ab
	.4byte 0x00000007
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00005000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00005000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0002c000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002a000
	.4byte 0xffff00e7
	.4byte 0x00000002
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02008169
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020080fd
	.4byte 0x00001815
	.4byte 0x02030010
	.4byte 0x020080ed
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte 0x020081d5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x08bd0014
	.4byte 0x02008819
	.4byte 0x00000000
	.4byte 0x08a70017
	.4byte 0x00001b42
	.4byte 0x00008d15
	.4byte 0x08a70017
	.4byte 0x00001b43
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000025ba
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000025bb
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001c91
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020080a1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c95
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001c96
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001c97
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001c98
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001c99
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001c9a
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001c9b
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001c9c
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001c9d
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001c9e
	.4byte 0x00008515
	.4byte 0x0200000a
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008085
	.4byte 0x00001815
	.4byte 0x02030010
	.4byte 0x020080ed
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008099
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x0200809d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
