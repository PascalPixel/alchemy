.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008f29, 0x02008039, 0x02008045, 0x0200804d, 0x020080a5, 0x02008041, 0x02009065
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x91b0
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x91e0
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9220
	.2byte 0x0200
	push	{lr}
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1dfd
	.2byte 0x0000
	push	{lr}
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x1e25
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009090
	cmp	r0, #0
	beq.n	.L_020000b8
	ldr	r0, [pc, #4]
	b.n	.L_020000ba
.L_020000b8:
	ldr	r0, [pc, #4]
.L_020000ba:
	pop	{pc}
	.4byte 0x020094c0
	.2byte 0x9370
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090f8
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
	bne.n	.L_02000100
	movs	r0, #14
	adds	r1, r5, #0
	bl 0x02009190
	b.n	.L_0200011c
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000100:
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_0200011c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e00
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090f8
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
	bne.n	.L_02000160
	movs	r0, #14
	adds	r1, r5, #0
	bl 0x02009190
	b.n	.L_0200017c
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000160:
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_0200017c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e28
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090f8
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
	bne.n	.L_020001c0
	movs	r0, #15
	adds	r1, r5, #0
	bl 0x02009190
	b.n	.L_020001dc
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020001c0:
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_020001dc:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e02
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090f8
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
	bne.n	.L_02000220
	movs	r0, #15
	adds	r1, r5, #0
	bl 0x02009190
	b.n	.L_0200023c
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000220:
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_0200023c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e2a
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090f8
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
	bne.n	.L_02000280
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x02009198
	b.n	.L_0200029c
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000280:
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_0200029c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e07
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x020090f8
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
	bne.n	.L_020002e0
	movs	r0, #5
	adds	r1, r5, #0
	bl 0x02009198
	b.n	.L_020002fc
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002e0:
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r0, [pc, #20]
	bl 0x02009138
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_020002fc:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e2f
	.2byte 0x0000
	push	{r5, lr}
	bl 0x020090e0
	movs	r0, #0
	bl 0x02009180
	ldr	r5, [pc, #112]
	adds	r0, r5, #0
	bl 0x02009138
	movs	r1, #0
	movs	r0, #10
	bl 0x02009140
	bl 0x02009188
	movs	r1, #0
	bl 0x020090f0
	cmp	r0, #0
	bne.n	.L_02000370
	adds	r0, r5, #2
	bl 0x02009138
	movs	r1, #0
	movs	r0, #10
	bl 0x02009140
	bl 0x02009188
	movs	r1, #0
	bl 0x020090f0
	cmp	r0, #0
	bne.n	.L_02000370
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #16]
	cmp	r3, #19
	bls.n	.L_0200035c
	bl 0x02008390
	bl 0x020090e8
	b.n	.L_02000382
.L_0200035c:
	adds	r0, r5, #4
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
	b.n	.L_02000382
.L_02000370:
	ldr	r0, [pc, #24]
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x020090e8
.L_02000382:
	pop	{r5, pc}
	.4byte 0x00001daa
	.4byte 0x02000240
	.2byte 0x1dab
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r0, [pc, #916]
	sub	sp, #8
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	movs	r0, #30
	bl 0x020090d8
.L_020003a8:
	ldr	r3, [pc, #896]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r7, r3, r1
	ldr	r0, [r7, #0]
	movs	r1, #3
	bl 0x02009118
	add	r0, sp, #4
	mov	r1, sp
	bl 0x020091a0
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_020003ea
	ldr	r5, [pc, #868]
	adds	r0, r5, #0
	bl 0x02009138
	movs	r1, #0
	movs	r0, #10
	bl 0x02009140
	bl 0x02009188
	movs	r1, #0
	bl 0x020090f0
	cmp	r0, #1
	bne.n	.L_020003a8
	adds	r0, r5, #1
	b.n	.L_02000496
.L_020003ea:
	ldr	r0, [sp, #4]
	bl 0x02009088
	ldr	r3, [sp, #0]
	adds	r5, r0, #0
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r5, r3]
	bl 0x020090b8
	movs	r0, #18
	bl 0x020090f8
	ldr	r3, [sp, #0]
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r1, [r5, r3]
	bl 0x020090a0
	movs	r1, #168
	movs	r2, #208
	lsls	r2, r2, #15
	lsls	r1, r1, #18
	movs	r0, #18
	bl 0x02009108
	movs	r0, #60
	bl 0x020090d8
	ldr	r3, [sp, #0]
	movs	r1, #2
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r5, r3]
	bl 0x020090b0
	movs	r1, #1
	ldr	r0, [pc, #764]
	bl 0x020090a8
	ldr	r6, [pc, #764]
	adds	r0, r6, #0
	bl 0x02009138
	movs	r1, #0
	movs	r0, #10
	bl 0x02009140
	bl 0x02009188
	movs	r1, #0
	bl 0x020090f0
	cmp	r0, #1
	bne.n	.L_020004a4
	movs	r2, #0
	movs	r0, #18
	movs	r1, #0
	bl 0x02009108
	ldr	r3, [sp, #0]
	movs	r1, #2
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r5, r3]
	bl 0x020090b0
	movs	r1, #1
	adds	r0, r6, #1
	bl 0x020090a8
	adds	r0, r6, #2
	bl 0x02009138
	movs	r1, #0
	movs	r0, #10
	bl 0x02009140
	bl 0x02009188
	movs	r1, #0
	bl 0x020090f0
	cmp	r0, #1
	bne.n	.L_020003a8
	adds	r0, r6, #3
.L_02000496:
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	b.n	.L_0200075a
.L_020004a4:
	adds	r0, r6, #4
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x02009170
	movs	r0, #30
	bl 0x020090d8
	movs	r1, #1
	ldr	r0, [pc, #628]
	bl 0x02009168
	movs	r0, #30
	bl 0x02009178
	movs	r0, #120
	bl 0x020090d8
	movs	r0, #11
	bl 0x020091a8
	movs	r1, #5
	movs	r0, #10
	bl 0x02009110
	movs	r0, #10
	bl 0x02009120
	movs	r0, #10
	bl 0x020090f8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #166
	strb	r3, [r0, #0]
	movs	r2, #96
	movs	r0, #10
	lsls	r1, r1, #2
	bl 0x02009100
	movs	r1, #5
	movs	r0, #10
	bl 0x02009110
	movs	r0, #30
	bl 0x020090d8
	movs	r0, #10
	bl 0x020090f8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #170
	strb	r3, [r0, #0]
	movs	r2, #96
	movs	r0, #10
	lsls	r1, r1, #2
	bl 0x02009100
	movs	r0, #10
	movs	r1, #5
	bl 0x02009110
	movs	r1, #4
	movs	r2, #90
	movs	r0, #10
	bl 0x02009128
	movs	r0, #10
	bl 0x020090f8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #166
	strb	r3, [r0, #0]
	movs	r2, #96
	movs	r0, #10
	lsls	r1, r1, #2
	bl 0x02009100
	movs	r1, #5
	movs	r0, #10
	bl 0x02009110
	movs	r0, #30
	bl 0x020090d8
	movs	r0, #10
	bl 0x020090f8
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #170
	strb	r3, [r0, #0]
	movs	r2, #96
	movs	r0, #10
	lsls	r1, r1, #2
	bl 0x02009100
	movs	r0, #10
	movs	r1, #5
	bl 0x02009110
	movs	r1, #4
	movs	r2, #90
	movs	r0, #10
	bl 0x02009128
	movs	r0, #10
	bl 0x020090f8
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #168
	ands	r5, r3
	strb	r5, [r0, #0]
	movs	r2, #96
	movs	r0, #10
	lsls	r1, r1, #2
	bl 0x02009100
	movs	r0, #10
	movs	r1, #5
	bl 0x02009110
	movs	r0, #10
	movs	r1, #4
	movs	r2, #30
	bl 0x02009128
	movs	r2, #30
	movs	r0, #10
	movs	r1, #4
	bl 0x02009128
	movs	r0, #10
	movs	r1, #5
	bl 0x02009110
	movs	r1, #0
	movs	r0, #10
	bl 0x02009110
	movs	r0, #11
	bl 0x020091a8
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x02009168
	movs	r0, #30
	bl 0x02009178
	movs	r1, #168
	movs	r2, #208
	lsls	r2, r2, #15
	lsls	r1, r1, #18
	movs	r0, #20
	bl 0x02009108
	movs	r0, #144
	bl 0x020091a8
	movs	r1, #2
	movs	r0, #20
	bl 0x02009110
	movs	r0, #20
	bl 0x02009120
	movs	r1, #0
	movs	r2, #0
	movs	r0, #20
	bl 0x02009108
	movs	r0, #4
	bl 0x020090d8
	movs	r1, #168
	movs	r2, #208
	lsls	r2, r2, #15
	movs	r0, #19
	lsls	r1, r1, #18
	bl 0x02009108
	movs	r1, #4
	movs	r0, #19
	bl 0x02009110
	movs	r0, #19
	bl 0x02009120
	movs	r0, #15
	bl 0x020090d8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #19
	bl 0x02009108
	movs	r0, #30
	bl 0x020090d8
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	ldr	r0, [r7, #0]
	bl 0x02009160
	movs	r0, #60
	bl 0x020090d8
	movs	r0, #10
	bl 0x020091a8
	movs	r1, #128
	movs	r2, #60
	lsls	r1, r1, #1
	movs	r0, #10
	bl 0x02009160
	movs	r1, #0
	movs	r0, #10
	bl 0x02009148
	ldr	r0, [sp, #4]
	bl 0x02009088
	ldr	r3, [sp, #0]
	adds	r5, r0, #0
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r0, [r5, r3]
	bl 0x020090b8
	ldr	r3, [sp, #0]
	movs	r2, #128
	lsls	r3, r3, #1
	adds	r3, #216
	ldrh	r3, [r5, r3]
	ldr	r1, [pc, #152]
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	adds	r3, r2, r1
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	bhi.n	.L_020006c0
	bl 0x02008920
	b.n	.L_02000748
.L_020006c0:
	movs	r3, #163
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_020006fc
	adds	r0, r6, #0
	adds	r0, #29
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	movs	r0, #148
	lsls	r0, r0, #4
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_020006ea
.L_020006e4:
	adds	r0, r6, #0
	adds	r0, #30
	b.n	.L_020006ee
.L_020006ea:
	adds	r0, r6, #0
	adds	r0, #31
.L_020006ee:
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	b.n	.L_02000748
.L_020006fc:
	ldrb	r0, [r0, #2]
	adds	r2, r0, #0
	cmp	r2, #6
	bne.n	.L_0200070a
	bl 0x020087b4
	b.n	.L_02000748
.L_0200070a:
	cmp	r2, #1
	bne.n	.L_02000714
	bl 0x02008760
	b.n	.L_02000748
.L_02000714:
	adds	r3, r0, #0
	adds	r3, #254
	movs	r1, #192
	lsls	r3, r3, #24
	lsls	r1, r1, #18
	cmp	r3, r1
	bhi.n	.L_02000744
	bl 0x02008838
	b.n	.L_02000748
	.4byte 0x00001daf
	.4byte 0x02000240
	.4byte 0x00001db2
	.4byte 0x00001da9
	.4byte 0x00001db0
	.4byte 0x0040250d
	.2byte 0xfe49
	.2byte 0xffff
.L_02000744:
	bl 0x02008878
.L_02000748:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x02009108
	movs	r0, #20
	negs	r0, r0
	bl 0x020090c8
.L_0200075a:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x0200897c
	cmp	r0, #1
	beq.n	.L_0200078e
	cmp	r0, #1
	bgt.n	.L_02000784
	cmp	r0, #0
	beq.n	.L_0200078a
	b.n	.L_020007ac
.L_02000784:
	cmp	r0, #2
	beq.n	.L_0200079e
	b.n	.L_020007ac
.L_0200078a:
	adds	r0, r5, #1
	b.n	.L_02000790
.L_0200078e:
	adds	r0, r5, #2
.L_02000790:
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	b.n	.L_020007ac
.L_0200079e:
	adds	r0, r5, #3
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
.L_020007ac:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1db6
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #100]
	adds	r0, r5, #0
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x020089a4
	subs	r0, #3
	cmp	r0, #4
	bhi.n	.L_0200080c
	ldr	r2, [pc, #76]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x020087ec
	.4byte 0x020087f0
	.4byte 0x020087f4
	.4byte 0x020087f8
	.4byte 0x020087fc
	.4byte 0xe006480d
	.4byte 0xe004480d
	.4byte 0xe002480d
	.4byte 0xe000480d
	.4byte 0xf000480d
	.4byte 0x200afc9b
	.4byte 0xf0002100
	.2byte 0xfc9f
	.2byte 0xe006
.L_0200080c:
	adds	r0, r5, #6
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	pop	{r5, pc}
	.4byte 0x00001dba
	.4byte 0x020087d8
	.4byte 0x00001dbb
	.4byte 0x00001dbc
	.4byte 0x00001dbd
	.4byte 0x00001dbe
	.2byte 0x1dbf
	.2byte 0x0000
	push	{r5, lr}
	bl 0x02008c04
	adds	r5, r0, #0
	ldr	r0, [pc, #48]
	cmp	r5, r0
	bne.n	.L_02000856
	adds	r0, r5, #0
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	b.n	.L_02000872
.L_02000856:
	subs	r0, #42
	bl 0x02009138
	movs	r1, #0
	movs	r0, #10
	bl 0x02009148
	adds	r0, r5, #0
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
.L_02000872:
	pop	{r5, pc}
	.2byte 0x1dfa
	.2byte 0x0000
	push	{lr}
	ldr	r0, [pc, #132]
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x020089f8
	cmp	r0, #13
	bhi.n	.L_020008fe
	ldr	r2, [pc, #112]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x020088d0
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088fe
	.4byte 0x020088d4
	.4byte 0x020088d8
	.4byte 0x020088dc
	.4byte 0x020088e0
	.4byte 0x020088f0
	.4byte 0xe006480d
	.4byte 0xe004480d
	.4byte 0xe002480d
	.4byte 0xe000480d
	.4byte 0xf000480d
	.4byte 0x200afc29
	.4byte 0xf0002100
	.4byte 0xe006fc2d
	.4byte 0xf000480a
	.4byte 0x200afc21
	.4byte 0xf0002100
	.2byte 0xfc25
.L_020008fe:
	pop	{pc}
	.4byte 0x00001dc1
	.4byte 0x02008898
	.4byte 0x00001dc2
	.4byte 0x00001dc3
	.4byte 0x00001dc4
	.4byte 0x00001dc5
	.4byte 0x00001dc6
	.2byte 0x1dc7
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #84]
	adds	r0, r5, #0
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	bl 0x02008a7c
	cmp	r0, #15
	beq.n	.L_02000952
	cmp	r0, #15
	bgt.n	.L_02000944
	cmp	r0, #14
	beq.n	.L_0200094e
	b.n	.L_02000974
.L_02000944:
	cmp	r0, #16
	beq.n	.L_02000956
	cmp	r0, #17
	beq.n	.L_02000966
	b.n	.L_02000974
.L_0200094e:
	adds	r0, r5, #1
	b.n	.L_02000958
.L_02000952:
	adds	r0, r5, #2
	b.n	.L_02000958
.L_02000956:
	adds	r0, r5, #3
.L_02000958:
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
	b.n	.L_02000974
.L_02000966:
	adds	r0, r5, #4
	bl 0x02009138
	movs	r0, #10
	movs	r1, #0
	bl 0x02009148
.L_02000974:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1dc8
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_02000990
	movs	r0, #0
	b.n	.L_020009a2
.L_02000990:
	movs	r0, #148
	lsls	r0, r0, #4
	bl 0x02009090
	adds	r3, r0, #0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	adds	r0, #1
.L_020009a2:
	pop	{pc}
	push	{r5, lr}
	movs	r0, #144
	bl 0x020090d0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	bne.n	.L_020009b8
	movs	r0, #3
	b.n	.L_020009f4
.L_020009b8:
	movs	r0, #139
	bl 0x020090d0
	cmp	r0, r5
	bne.n	.L_020009c6
	movs	r0, #4
	b.n	.L_020009f4
.L_020009c6:
	movs	r0, #138
	bl 0x020090d0
	cmp	r0, r5
	bne.n	.L_020009d4
	movs	r0, #5
	b.n	.L_020009f4
.L_020009d4:
	movs	r0, #213
	bl 0x020090c0
	cmp	r0, r5
	bne.n	.L_020009e2
	movs	r0, #6
	b.n	.L_020009f4
.L_020009e2:
	movs	r0, #214
	bl 0x020090c0
	adds	r3, r0, #0
	mvns	r3, r3
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	adds	r0, #7
.L_020009f4:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_02000a0c
	movs	r0, #0
	b.n	.L_02000a7a
.L_02000a0c:
	movs	r0, #163
	lsls	r0, r0, #1
	bl 0x020090c0
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_02000a68
	movs	r0, #148
	lsls	r0, r0, #4
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_02000a78
	movs	r0, #184
	adds	r0, #255
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000a4e
	movs	r0, #220
	lsls	r0, r0, #1
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000a4e
	movs	r0, #186
	adds	r0, #255
	bl 0x020090c0
	cmp	r0, r5
	bne.n	.L_02000a52
.L_02000a4e:
	movs	r0, #9
	b.n	.L_02000a7a
.L_02000a52:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #5
	bl 0x02009090
	adds	r3, r0, #0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	adds	r0, #10
	b.n	.L_02000a7a
.L_02000a68:
	movs	r0, #148
	lsls	r0, r0, #4
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_02000a78
	movs	r0, #12
	b.n	.L_02000a7a
.L_02000a78:
	movs	r0, #13
.L_02000a7a:
	pop	{r5, pc}
	push	{r5, r6, lr}
	movs	r0, #184
	adds	r0, #255
	bl 0x020090c0
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_02000aae
	movs	r0, #186
	adds	r0, #255
	bl 0x020090c0
	adds	r6, r0, #0
	cmp	r6, r5
	bne.n	.L_02000aaa
	movs	r0, #220
	lsls	r0, r0, #1
	bl 0x020090c0
	cmp	r0, r6
	beq.n	.L_02000aae
.L_02000aaa:
	movs	r0, #14
	b.n	.L_02000b5c
.L_02000aae:
	movs	r0, #184
	adds	r0, #255
	bl 0x020090c0
	movs	r6, #1
	negs	r6, r6
	cmp	r0, r6
	beq.n	.L_02000ad8
	movs	r0, #220
	lsls	r0, r0, #1
	bl 0x020090c0
	adds	r5, r0, #0
	cmp	r5, r6
	bne.n	.L_02000ad8
	movs	r0, #186
	adds	r0, #255
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000b02
.L_02000ad8:
	movs	r0, #184
	adds	r0, #255
	bl 0x020090c0
	movs	r6, #1
	negs	r6, r6
	cmp	r0, r6
	beq.n	.L_02000b06
	movs	r0, #220
	lsls	r0, r0, #1
	bl 0x020090c0
	adds	r5, r0, #0
	cmp	r5, r6
	bne.n	.L_02000b06
	movs	r0, #186
	adds	r0, #255
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000b06
.L_02000b02:
	movs	r0, #15
	b.n	.L_02000b5c
.L_02000b06:
	movs	r0, #184
	adds	r0, #255
	bl 0x020090c0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L_02000b32
	movs	r0, #220
	lsls	r0, r0, #1
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000b32
	movs	r0, #186
	adds	r0, #255
	bl 0x020090c0
	cmp	r0, r5
	bne.n	.L_02000b32
	movs	r0, #16
	b.n	.L_02000b5c
.L_02000b32:
	movs	r0, #184
	adds	r0, #255
	bl 0x020090c0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L_02000b5c
	movs	r0, #220
	lsls	r0, r0, #1
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000b5c
	movs	r0, #186
	adds	r0, #255
	bl 0x020090c0
	cmp	r0, r5
	beq.n	.L_02000b5c
	movs	r0, #17
.L_02000b5c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #17
.L_02000b66:
	lsls	r3, r6, #2
	adds	r3, r3, r6
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r0, r3, #0
	adds	r0, #48
	cmp	r6, #0
	bne.n	.L_02000b7a
	cmp	r5, #7
	beq.n	.L_02000b92
.L_02000b7a:
	cmp	r6, #1
	bne.n	.L_02000b86
	cmp	r5, #9
	beq.n	.L_02000b92
	cmp	r5, #10
	beq.n	.L_02000b92
.L_02000b86:
	bl 0x02009090
	cmp	r0, #0
	beq.n	.L_02000b92
	adds	r0, r5, #1
	b.n	.L_02000b9c
.L_02000b92:
	subs	r5, #1
	cmp	r5, #6
	bgt.n	.L_02000b66
	movs	r0, #1
	negs	r0, r0
.L_02000b9c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #0
	mov	sl, r0
	movs	r5, #7
	sub	sp, #4
	adds	r7, r1, #0
	mov	r8, r3
	cmp	r5, sl
	bge.n	.L_02000bf6
	str	r2, [sp, #0]
.L_02000bba:
	cmp	r7, #0
	bne.n	.L_02000bc2
	cmp	r5, #7
	beq.n	.L_02000bf0
.L_02000bc2:
	cmp	r7, #1
	bne.n	.L_02000bce
	cmp	r5, #9
	beq.n	.L_02000bf0
	cmp	r5, #10
	beq.n	.L_02000bf0
.L_02000bce:
	lsls	r3, r7, #2
	adds	r3, r3, r7
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r6, r3, #0
	adds	r6, #48
	adds	r0, r6, #0
	bl 0x02009090
	cmp	r0, #0
	bne.n	.L_02000bf0
	ldr	r3, [sp, #0]
	stmia	r3!, {r6}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	movs	r2, #1
	add	r8, r2
.L_02000bf0:
	adds	r5, #1
	cmp	r5, sl
	blt.n	.L_02000bba
.L_02000bf6:
	mov	r0, r8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #160
	movs	r0, #252
	lsls	r1, r1, #1
	sub	sp, #16
	bl 0x02009078
	movs	r3, #156
	lsls	r3, r3, #6
	movs	r6, #0
	adds	r3, #15
	adds	r7, r0, #0
	mov	r8, r3
	mov	sl, r6
	movs	r5, #0
.L_02000c28:
	adds	r0, r5, #0
	bl 0x02008b60
	lsls	r3, r5, #2
	mov	r2, sp
	str	r0, [r2, r3]
	cmp	r8, r0
	ble.n	.L_02000c3c
	mov	r8, r0
	mov	sl, r5
.L_02000c3c:
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02000c50
	lsls	r2, r6, #2
	adds	r2, r7, r2
	adds	r1, r5, #0
	bl 0x02008ba0
	adds	r6, r6, r0
.L_02000c50:
	adds	r5, #1
	cmp	r5, #3
	bls.n	.L_02000c28
	cmp	r6, #0
	bne.n	.L_02000c8c
	movs	r5, #0
.L_02000c5c:
	lsls	r2, r6, #2
	adds	r1, r5, #0
	adds	r2, r7, r2
	movs	r0, #18
	bl 0x02008ba0
	adds	r5, #1
	adds	r6, r6, r0
	cmp	r5, #3
	bls.n	.L_02000c5c
	cmp	r6, #0
	bne.n	.L_02000c7e
	movs	r0, #252
	bl 0x02009080
	ldr	r0, [pc, #512]
	b.n	.L_02000e72
.L_02000c7e:
	movs	r0, #18
	mov	r1, sl
	adds	r2, r7, #0
	bl 0x02008ba0
	movs	r0, #0
	b.n	.L_02000c96
.L_02000c8c:
	bl 0x02009070
	adds	r1, r6, #0
	bl 0x02009068
.L_02000c96:
	lsls	r3, r0, #2
	ldr	r3, [r3, r7]
	adds	r0, r3, #0
	subs	r0, #56
	cmp	r0, #69
	bls.n	.L_02000ca4
	b.n	.L_02000e68
.L_02000ca4:
	ldr	r2, [pc, #472]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x02008dc4
	.4byte 0x02008dc8
	.4byte 0x02008dcc
	.4byte 0x02008dd0
	.4byte 0x02008dd4
	.4byte 0x02008dd8
	.4byte 0x02008ddc
	.4byte 0x02008de0
	.4byte 0x02008de4
	.4byte 0x02008de8
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008dec
	.4byte 0x02008df0
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008df4
	.4byte 0x02008df8
	.4byte 0x02008dfc
	.4byte 0x02008e00
	.4byte 0x02008e04
	.4byte 0x02008e08
	.4byte 0x02008e0c
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e10
	.4byte 0x02008e14
	.4byte 0x02008e18
	.4byte 0x02008e1c
	.4byte 0x02008e20
	.4byte 0x02008e24
	.4byte 0x02008e28
	.4byte 0x02008e2c
	.4byte 0x02008e30
	.4byte 0x02008e34
	.4byte 0x02008e38
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e68
	.4byte 0x02008e3c
	.4byte 0x02008e40
	.4byte 0x02008e44
	.4byte 0x02008e48
	.4byte 0x02008e4c
	.4byte 0x02008e50
	.4byte 0x02008e54
	.4byte 0x02008e58
	.4byte 0x02008e5c
	.4byte 0x02008e60
	.4byte 0x02008e64
	.4byte 0xe0504d2f
	.4byte 0xe04e4d2f
	.4byte 0xe04c4d2f
	.4byte 0xe04a4d2f
	.4byte 0xe0484d2f
	.4byte 0xe0464d2f
	.4byte 0xe0444d2f
	.4byte 0xe0424d2f
	.4byte 0xe0404d2f
	.4byte 0xe03e4d2f
	.4byte 0xe03c4d2f
	.4byte 0xe03a4d2f
	.4byte 0xe0384d2f
	.4byte 0xe0364d2f
	.4byte 0xe0344d2f
	.4byte 0xe0324d2f
	.4byte 0xe0304d2f
	.4byte 0xe02e4d2f
	.4byte 0xe02c4d2f
	.4byte 0xe02a4d2f
	.4byte 0xe0284d2f
	.4byte 0xe0264d2f
	.4byte 0xe0244d2f
	.4byte 0xe0224d2f
	.4byte 0xe0204d2f
	.4byte 0xe01e4d2f
	.4byte 0xe01c4d2f
	.4byte 0xe01a4d2f
	.4byte 0xe0184d2f
	.4byte 0xe0164d2f
	.4byte 0xe0144d2f
	.4byte 0xe0124d2f
	.4byte 0xe0104d2f
	.4byte 0xe00e4d2f
	.4byte 0xe00c4d2f
	.4byte 0xe00a4d2f
	.4byte 0xe0084d2f
	.4byte 0xe0064d2f
	.4byte 0xe0044d2f
	.4byte 0xe0024d2f
	.2byte 0x4d2f
	.2byte 0xe000
.L_02000e68:
	ldr	r5, [pc, #16]
	movs	r0, #252
	bl 0x02009080
.L_02000e70:
	adds	r0, r5, #0
.L_02000e72:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x00001dfa
	.4byte 0x02008cac
	.4byte 0x00001dd1
	.4byte 0x00001dd2
	.4byte 0x00001dd3
	.4byte 0x00001dd4
	.4byte 0x00001dd5
	.4byte 0x00001dd6
	.4byte 0x00001dd7
	.4byte 0x00001dd8
	.4byte 0x00001dd9
	.4byte 0x00001dda
	.4byte 0x00001ddb
	.4byte 0x00001ddc
	.4byte 0x00001ddd
	.4byte 0x00001dde
	.4byte 0x00001ddf
	.4byte 0x00001de0
	.4byte 0x00001de1
	.4byte 0x00001de2
	.4byte 0x00001de3
	.4byte 0x00001de4
	.4byte 0x00001de5
	.4byte 0x00001de6
	.4byte 0x00001de7
	.4byte 0x00001de8
	.4byte 0x00001de9
	.4byte 0x00001dea
	.4byte 0x00001deb
	.4byte 0x00001dec
	.4byte 0x00001ded
	.4byte 0x00001dee
	.4byte 0x00001def
	.4byte 0x00001df0
	.4byte 0x00001df1
	.4byte 0x00001df2
	.4byte 0x00001df3
	.4byte 0x00001df4
	.4byte 0x00001df5
	.4byte 0x00001df6
	.4byte 0x00001df7
	.4byte 0x00001df8
	.2byte 0x1df9
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	ldr	r3, [pc, #284]
	adds	r2, #11
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x020090f8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r6, #32
	orrs	r3, r6
	movs	r2, #0
	strb	r3, [r0, #0]
	movs	r0, #17
	mov	r8, r2
	bl 0x02009158
	movs	r0, #18
	bl 0x02009158
	movs	r1, #2
	movs	r0, #10
	bl 0x02009150
	movs	r0, #12
	bl 0x020090f8
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #4
	orrs	r3, r5
	movs	r1, #2
	strb	r3, [r0, #0]
	movs	r0, #16
	bl 0x02009150
	movs	r0, #16
	bl 0x020090f8
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #18
	bl 0x020090f8
	adds	r3, r0, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	ldr	r3, [pc, #188]
	movs	r5, #192
	str	r3, [r0, #12]
	str	r3, [r0, #20]
	movs	r0, #18
	bl 0x020090f8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #18
	bl 0x02009150
.L_02000fbe:
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x02009108
	movs	r0, #19
	bl 0x020090f8
	adds	r3, r0, #0
	mov	r2, r8
	lsls	r5, r5, #12
	adds	r3, #85
	strb	r2, [r3, #0]
	str	r5, [r0, #12]
	str	r5, [r0, #20]
.L_02000fdc:
	movs	r0, #19
	bl 0x020090f8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x02009150
	movs	r1, #0
	movs	r2, #0
	movs	r0, #19
	bl 0x02009108
	movs	r0, #20
	bl 0x020090f8
	adds	r3, r0, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	str	r5, [r0, #12]
	str	r5, [r0, #20]
	movs	r0, #20
	bl 0x020090f8
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r1, #1
	orrs	r6, r3
	strb	r6, [r0, #0]
	movs	r0, #20
	bl 0x02009150
	movs	r2, #0
	movs	r1, #0
	movs	r0, #20
	bl 0x02009108
	movs	r0, #19
	bl 0x020090f8
	movs	r1, #0
	bl 0x02009098
	movs	r0, #20
	bl 0x020090f8
	movs	r1, #0
	bl 0x02009098
	movs	r0, #20
	bl 0x020090f8
	movs	r1, #4
	bl 0x02009130
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0x2000
	bx	lr
	.irp EntryTarget, 0x03000514, 0x080000f9, 0x08000149, 0x08000151, 0x080003c1, 0x080003c9, 0x08020219, 0x08020301, 0x08038041, 0x08038121, 0x080ad011, 0x080ad039, 0x080ad1d9, 0x080ad2e9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c80d1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8131, 0x080c8139, 0x080c8171, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c8201, 0x080c8209, 0x080c8211, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c84e1, 0x080c8779, 0x08108009, 0x08108019, 0x08108071, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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
	.4byte 0x0000006a
	.4byte 0x10101069
	.4byte 0xffffffff
	.4byte 0x10202069
	.4byte 0xffffffff
	.4byte 0x10303069
	.4byte 0xffffffff
	.4byte 0x10404069
	.4byte 0xffffffff
	.4byte 0x10505069
	.4byte 0xffffffff
	.4byte 0x1060706a
	.4byte 0xffffffff
	.4byte 0x1070606a
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff0019
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x005e0000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x028b0000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00012000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02b50000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00016000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0106
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff011d
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
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
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001da4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001da5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001da8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001dfb
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001dfc
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020080c5
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008185
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e06
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008245
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008305
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001da6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001da7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001dfd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001dfe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001dff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e01
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e03
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e08
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e09
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x02008055
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001e1f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001e20
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001da8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001e23
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001e24
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008125
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020081e5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e2e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082a5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008305
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001e21
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001e22
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001e25
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001e26
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001e27
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e29
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e2b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e30
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e31
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0200807d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
