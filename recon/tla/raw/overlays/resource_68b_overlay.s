.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008bdd, 0x020081a1, 0x020081ad, 0x020081b5, 0x02008a65, 0x020081a9, 0x02008e79
	overlay_veneer \EntryTarget
	.endr
	push	{r5, lr}
	ldmia	r0!, {r5}
	ldmia	r1!, {r3}
	ldmia	r0!, {r4}
	subs	r5, r5, r3
	ldmia	r1!, {r3}
	asrs	r5, r5, #16
	ldr	r2, [r1, #0]
	subs	r4, r4, r3
	ldr	r3, [r0, #0]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r0, r5, #0
	muls	r0, r5
	adds	r2, r4, #0
	muls	r2, r4
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd20
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	adds	r6, r1, #0
	adds	r7, r6, #0
	mov	r5, r8
	adds	r7, #8
	adds	r5, #8
	mov	sl, r2
	adds	r0, r7, #0
	movs	r2, #0
	adds	r1, r5, #0
	mov	r9, r2
	bl 0x02008038
	cmp	r0, sl
	bge.n	.L_020000d8
	mov	r2, r8
	ldr	r3, [r2, #16]
	ldr	r0, [r6, #16]
	ldr	r1, [r7, #0]
	subs	r0, r0, r3
	ldr	r3, [r5, #0]
	movs	r5, #128
	subs	r1, r1, r3
	bl 0x02009b5c
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldr	r3, [pc, #48]
	lsls	r5, r5, #5
	adds	r1, r0, r5
	mov	r5, r8
	ldrh	r2, [r5, #6]
	adds	r4, r0, r3
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r4, r3
	ands	r1, r3
	ands	r0, r3
	ands	r3, r2
	cmp	r0, r3
.L_020000ca:
	beq.n	.L_020000d4
	cmp	r1, r3
	beq.n	.L_020000d4
	cmp	r4, r3
	bne.n	.L_020000d8
.L_020000d4:
	movs	r2, #1
	mov	r9, r2
.L_020000d8:
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #168]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r7, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r7, #0]
	bl 0x02009be4
	bl 0x02009d54
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl 0x02009ba4
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r5, r6, #0
	ands	r3, r2
	adds	r5, #91
	cmp	r3, #141
	bne.n	.L_0200013e
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r2, #19
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200018c
.L_0200013e:
	adds	r3, r6, #0
	adds	r3, #100
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #1
	beq.n	.L_0200018c
	ldr	r0, [r7, #0]
	bl 0x02009be4
	adds	r1, r0, #0
	adds	r0, r6, #0
	adds	r0, #8
	adds	r1, #8
	bl 0x02008038
	cmp	r0, #11
	ble.n	.L_02000182
	ldr	r1, [r6, #104]
	adds	r0, r6, #0
	movs	r2, #18
	bl 0x02008070
	cmp	r0, #0
	bne.n	.L_0200018c
	ldr	r0, [r7, #0]
	bl 0x02009be4
	movs	r2, #26
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl 0x02008070
	cmp	r0, #0
	bne.n	.L_0200018c
.L_02000182:
	movs	r3, #0
	adds	r0, r6, #0
	strb	r3, [r5, #0]
	movs	r1, #2
	b.n	.L_02000194
.L_0200018c:
	movs	r3, #1
	adds	r0, r6, #0
	strb	r3, [r5, #0]
.L_02000192:
	movs	r1, #1
.L_02000194:
	bl 0x02009b7c
	movs	r0, #0
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9e84
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9eb4
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9f14
	.2byte 0x0200
	push	{lr}
	movs	r1, #244
	movs	r2, #157
	lsls	r2, r2, #19
	lsls	r1, r1, #17
	movs	r0, #12
	bl 0x02009c2c
	movs	r0, #12
	bl 0x02009be4
	movs	r1, #157
	bl 0x02009b9c
.L_020001d8:
	movs	r0, #12
	movs	r1, #3
	bl 0x02009c34
	movs	r1, #128
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
.L_020001ec:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	adds	r6, r2, #0
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	movs	r0, #158
	bl 0x02009d7c
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl 0x02009b84
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x02009be4
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x02009bec
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x02009c34
	ldr	r0, [r5, #0]
	cmp	r6, #0
	bne.n	.L_0200024e
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	bl 0x02009c14
	b.n	.L_02000258
.L_0200024e:
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x02009c1c
.L_02000258:
	movs	r0, #10
	bl 0x02009bc4
	adds	r0, r7, #0
	bl 0x02009cd4
	bl 0x02009cdc
	bl 0x02009ce4
	bl 0x02009bd4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r6, [pc, #104]
	movs	r7, #0
	cmp	r5, #3
	bne.n	.L_020002a4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #106
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_020002a4
	ldr	r0, [pc, #88]
	bl 0x02009c6c
	movs	r0, #1
	negs	r0, r0
	movs	r1, #0
	bl 0x02009c84
	b.n	.L_020002e6
.L_020002a4:
	cmp	r5, #1
	bne.n	.L_020002ac
	ldr	r6, [pc, #60]
	movs	r7, #0
.L_020002ac:
	cmp	r5, #2
	bne.n	.L_020002b4
	ldr	r6, [pc, #60]
	movs	r7, #0
.L_020002b4:
	cmp	r5, #3
	bne.n	.L_020002bc
	ldr	r6, [pc, #56]
	movs	r7, #1
.L_020002bc:
	cmp	r5, #12
	bne.n	.L_020002c4
	ldr	r6, [pc, #52]
	movs	r7, #0
.L_020002c4:
	cmp	r5, #11
	bne.n	.L_020002cc
	ldr	r6, [pc, #48]
	movs	r7, #1
.L_020002cc:
	cmp	r5, #10
	bne.n	.L_020002d4
	ldr	r6, [pc, #44]
	movs	r7, #1
.L_020002d4:
	cmp	r5, #16
	bne.n	.L_020002dc
	ldr	r6, [pc, #40]
	movs	r7, #0
.L_020002dc:
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl 0x020081f0
.L_020002e6:
	pop	{r5, r6, r7, pc}
	.4byte 0x0200a1bc
	.4byte 0x00002880
	.4byte 0x0200a1c4
	.4byte 0x0200a1cc
	.4byte 0x0200a1d4
	.4byte 0x0200a1dc
	.4byte 0x0200a1e4
	.2byte 0xa1ec
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #18
	ldr	r5, [r3, #108]
	bl 0x02009be4
	adds	r6, r0, #0
	movs	r3, #6
	ldrsh	r2, [r6, r3]
	adds	r7, r6, #0
	movs	r3, #1
	adds	r7, #100
	strh	r3, [r7, #0]
	mov	r8, r2
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0200035c
	ldr	r0, [pc, #72]
	bl 0x02009c6c
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #18
	movs	r2, #2
	bl 0x02009c64
	b.n	.L_02000362
.L_0200035c:
	ldr	r0, [pc, #52]
	bl 0x02009c6c
.L_02000362:
	movs	r0, #18
	movs	r1, #0
	bl 0x02009c34
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	bl 0x02009bd4
	movs	r3, #0
	strh	r3, [r7, #0]
	mov	r3, r8
	strh	r3, [r6, #6]
	movs	r0, #1
	bl 0x02009b4c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00002735
	.4byte 0x02000240
	.2byte 0x2742
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #19
	ldr	r5, [r3, #108]
	bl 0x02009be4
	adds	r6, r0, #0
	movs	r3, #6
	ldrsh	r2, [r6, r3]
	adds	r7, r6, #0
	movs	r3, #1
	adds	r7, #100
	strh	r3, [r7, #0]
	mov	r8, r2
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_020003ec
	ldr	r0, [pc, #72]
	bl 0x02009c6c
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #19
	movs	r2, #2
	bl 0x02009c64
	b.n	.L_020003f2
.L_020003ec:
	ldr	r0, [pc, #52]
	bl 0x02009c6c
.L_020003f2:
	movs	r0, #19
	movs	r1, #0
	bl 0x02009c34
	movs	r0, #19
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	bl 0x02009bd4
	movs	r3, #0
	strh	r3, [r7, #0]
	mov	r3, r8
	strh	r3, [r6, #6]
	movs	r0, #1
	bl 0x02009b4c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00002736
	.4byte 0x02000240
	.2byte 0x2743
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #39
	movs	r2, #71
	movs	r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #37
	movs	r1, #72
	movs	r2, #2
	movs	r3, #2
	bl 0x02009d44
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #49
	movs	r2, #71
	movs	r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #59
	movs	r1, #64
	movs	r2, #4
	movs	r3, #3
	bl 0x02009d44
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #53
	movs	r2, #69
	movs	r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #52
	movs	r1, #69
	movs	r2, #1
	movs	r3, #1
	bl 0x02009d44
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #52
	movs	r2, #64
	movs	r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #53
	movs	r1, #64
	movs	r2, #1
	movs	r3, #1
	bl 0x02009d44
	add	sp, #12
	pop	{pc}
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x02009be4
	movs	r3, #49
	movs	r2, #71
	movs	r1, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #64
	movs	r0, #59
	movs	r2, #4
	movs	r3, #3
	bl 0x02009d44
.L_020004d4:
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #187
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_0200052e
	movs	r1, #202
	movs	r2, #147
	movs	r0, #69
	lsls	r1, r1, #18
	lsls	r2, r2, #19
	bl 0x02009c2c
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #50
	bne.n	.L_0200052e
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #73
	bne.n	.L_0200052e
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009cac
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x02009c1c
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x02009c24
	bl 0x02009bd4
.L_0200052e:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x02009d74
	ldr	r0, [pc, #24]
	movs	r1, #2
	bl 0x02009ccc
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #194
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	pop	{pc}
	.2byte 0x0000
	.4byte 0x000000ca
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	movs	r5, #214
	movs	r7, #254
	lsls	r5, r5, #18
	lsls	r7, r7, #18
	adds	r0, #99
	bl 0x02009b74
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x02009d5c
	movs	r6, #64
	adds	r1, r7, #0
	adds	r3, r0, #0
	movs	r7, #129
	orrs	r3, r6
	lsls	r7, r7, #19
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x02009d64
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x02009d5c
	adds	r3, r0, #0
	orrs	r3, r6
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	bl 0x02009d64
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r2, #192
	movs	r1, #64
	lsls	r2, r2, #2
	bl 0x02009cf4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #65
	adds	r2, #1
	bl 0x02009cf4
	pop	{pc}
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #66
	adds	r2, #2
	bl 0x02009cf4
	pop	{pc}
	push	{lr}
	movs	r2, #129
	lsls	r2, r2, #2
	movs	r1, #67
	adds	r2, #255
	bl 0x02009cf4
	pop	{pc}
	push	{lr}
	movs	r2, #193
	movs	r1, #68
	lsls	r2, r2, #2
	bl 0x02009cf4
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r2, #192
	lsls	r2, r2, #2
	movs	r1, #70
	adds	r2, #6
	bl 0x02009cf4
	pop	{pc}
	push	{lr}
	movs	r2, #130
	lsls	r2, r2, #2
	movs	r1, #71
	adds	r2, #255
	bl 0x02009cf4
	pop	{pc}
	push	{lr}
	movs	r2, #194
	movs	r1, #72
	lsls	r2, r2, #2
	bl 0x02009cf4
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #680]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x02009be4
	ldr	r3, [r0, #12]
	movs	r2, #192
	lsls	r2, r2, #15
	cmp	r3, r2
	bgt.n	.L_02000654
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000666
.L_02000654:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020008d4
.L_02000666:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000682
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000694
.L_02000682:
	ldr	r0, [pc, #600]
	bl 0x02009c6c
	movs	r0, #1
	negs	r0, r0
	movs	r1, #0
	bl 0x02009c84
	b.n	.L_020008d4
.L_02000694:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #104
	bl 0x02009b74
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	ldr	r0, [pc, #564]
	bl 0x02009c6c
	movs	r1, #218
	movs	r2, #220
	lsls	r2, r2, #18
	lsls	r1, r1, #18
	movs	r0, #12
	bl 0x02009c2c
	movs	r0, #12
	bl 0x02009be4
	movs	r1, #43
	bl 0x02009b9c
	movs	r0, #12
	movs	r1, #3
	bl 0x02009c34
	movs	r1, #160
	movs	r0, #12
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c8c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #218
	movs	r1, #1
	movs	r2, #194
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x02009ca4
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x02009c7c
	movs	r0, #12
	movs	r1, #4
	bl 0x02009c3c
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #218
	movs	r1, #1
	movs	r2, #214
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #218
	movs	r1, #1
	movs	r2, #232
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	negs	r1, r1
	movs	r3, #1
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #133
	bl 0x02009bbc
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	cmp	r5, r2
	beq.n	.L_02000778
	b.n	.L_020008cc
.L_02000778:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #98
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000788
	b.n	.L_020008cc
.L_02000788:
	ldr	r0, [pc, #344]
	bl 0x02009c6c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #214
	movs	r2, #238
	lsls	r2, r2, #2
	movs	r0, #4
	lsls	r1, r1, #2
	bl 0x02009c0c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #4
	bl 0x02009ca4
	movs	r0, #218
	movs	r2, #198
	movs	r3, #1
	adds	r1, r5, #0
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x02009ca4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #218
	movs	r2, #214
	movs	r3, #1
	lsls	r0, r0, #18
	adds	r1, r5, #0
	lsls	r2, r2, #18
	bl 0x02009cbc
	movs	r0, #12
	movs	r1, #4
	movs	r2, #0
	bl 0x02009c44
	movs	r1, #218
	movs	r2, #228
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	movs	r0, #12
	bl 0x02009c0c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #12
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bec
	movs	r1, #32
	movs	r2, #0
	movs	r0, #12
	negs	r1, r1
	bl 0x02009d14
	movs	r1, #192
	movs	r0, #12
	lsls	r1, r1, #6
	bl 0x02009c94
	movs	r0, #212
	movs	r2, #226
	movs	r3, #1
	adds	r1, r5, #0
	lsls	r2, r2, #18
	lsls	r0, r0, #18
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #15
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #5
	bl 0x02009c7c
	movs	r1, #210
	movs	r2, #230
	movs	r0, #28
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r1, #210
	movs	r2, #234
	movs	r0, #27
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r1, #28
	movs	r2, #27
	movs	r0, #1
	bl 0x02009d2c
	movs	r1, #28
	movs	r2, #27
	movs	r0, #1
	bl 0x02009d34
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #5
	bl 0x02009c7c
	movs	r2, #0
	movs	r0, #12
	movs	r1, #32
	bl 0x02009d14
	movs	r0, #4
	movs	r1, #1
	bl 0x02009cb4
	movs	r2, #32
	movs	r0, #12
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d14
	movs	r0, #135
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009b74
.L_020008cc:
	bl 0x020081bc
	bl 0x02009bd4
.L_020008d4:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002796
	.4byte 0x00002797
	.2byte 0x289d
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x02009be4
	ldr	r3, [r0, #12]
	movs	r2, #180
	lsls	r2, r2, #15
	cmp	r3, r2
	bge.n	.L_02000918
	ldr	r0, [pc, #44]
	movs	r1, #1
	bl 0x02009bac
	movs	r0, #126
	bl 0x02009d7c
	movs	r0, #1
	bl 0x02009bb4
	b.n	.L_02000928
.L_02000918:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
.L_02000928:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x28a2
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #8
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000976
	ldr	r3, [pc, #268]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r0, #30
	bl 0x02009c5c
	movs	r0, #30
	bl 0x02009bfc
	ldr	r0, [pc, #248]
	bl 0x02009c6c
	movs	r0, #30
	movs	r1, #0
	bl 0x02009c84
	movs	r1, #208
	movs	r0, #30
	lsls	r1, r1, #8
	bl 0x02009c94
	b.n	.L_02000a52
.L_02000976:
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	ldr	r0, [pc, #216]
	bl 0x02009c6c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x02009ca4
	ldr	r3, [pc, #192]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r1, [r5, #0]
	movs	r2, #0
	movs	r0, #30
	bl 0x02009c5c
	movs	r0, #30
	bl 0x02009bfc
	movs	r0, #5
	bl 0x02009bc4
	movs	r1, #0
	movs	r0, #30
	bl 0x02009c74
	movs	r0, #4
	movs	r1, #0
	bl 0x02009bdc
	cmp	r0, #0
	beq.n	.L_020009d6
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c7c
	b.n	.L_02000a44
.L_020009d6:
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #30
	movs	r1, #3
	bl 0x02009c3c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #30
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x02009c7c
	movs	r1, #232
	movs	r2, #130
	lsls	r2, r2, #3
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x02009c0c
	movs	r1, #192
	lsls	r1, r1, #8
	ldr	r0, [r5, #0]
	bl 0x02009c94
	movs	r0, #5
	bl 0x02009bc4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #30
	ldr	r1, [pc, #56]
	adds	r2, #153
	bl 0x02009bec
	movs	r1, #220
	movs	r2, #131
	movs	r0, #30
	lsls	r1, r1, #1
	lsls	r2, r2, #3
	bl 0x02009c0c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #8
	bl 0x02009b74
.L_02000a44:
	movs	r1, #208
	movs	r0, #30
	lsls	r1, r1, #8
	bl 0x02009c94
	bl 0x02009bd4
.L_02000a52:
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x000028ac
	.4byte 0x000028a9
	.2byte 0x3333
	.2byte 0x0001
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xa1f4
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #92]
	movs	r2, #3
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000aca
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #252
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000a88:
	movs	r4, #160
.L_02000a8a:
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #224
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000a88
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #226
	strh	r0, [r3, #0]
	adds	r3, #58
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000ab2:
	ldr	r4, [pc, #28]
	lsls	r3, r1, #1
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000ab2
	ldr	r3, [pc, #12]
	strh	r0, [r3, #0]
.L_02000aca:
	pop	{pc}
	.4byte 0x0300122c
	.4byte 0x05000100
	.4byte 0x05000102
	.4byte 0x781b4b10
	.4byte 0x18c00098
	.4byte 0x01804b0f
	.4byte 0x220018c0
	.4byte 0x21805e83
	.4byte 0x311804c9
	.4byte 0x2380800b
	.4byte 0x33b004db
	.4byte 0x22c5895c
	.4byte 0x32ff0212
	.4byte 0x815a4022
	.4byte 0x895c22fe
	.4byte 0x32ff01d2
	.4byte 0x815a4022
	.4byte 0x895a3002
	.4byte 0xc3074a03
	.4byte 0x47703b0c
	.4byte 0x0200a590
	.4byte 0x0200a5a0
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r1, r1, r3
	ldr	r3, [pc, #112]
	sub	sp, #8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [pc, #104]
	lsls	r2, r2, #6
	adds	r2, r2, r3
	add	r7, sp, #4
	movs	r3, #0
	str	r3, [r7, #0]
	mov	sl, r1
	mov	r8, r2
.L_02000b5a:
	ldr	r3, [pc, #92]
	ldr	r0, [r7, #0]
	ldrb	r3, [r3, #0]
	mov	r1, sl
	adds	r0, r0, r3
	movs	r2, #6
	ldrsh	r3, [r1, r2]
	adds	r0, r0, r3
	lsls	r0, r0, #9
	bl 0x02009b64
	str	r0, [sp, #0]
	mov	r3, sl
	movs	r2, #2
	ldrsh	r5, [r3, r2]
	ldr	r6, [r7, #0]
	asrs	r0, r0, #15
	adds	r5, r5, r0
	movs	r1, #3
	adds	r0, r6, #0
	bl 0x02009b44
	adds	r5, r5, r0
	mov	r1, r8
	subs	r5, #1
	movs	r2, #2
	adds	r6, #1
	strh	r5, [r1, #0]
	add	r8, r2
	str	r6, [r7, #0]
	cmp	r6, #160
	bne.n	.L_02000b5a
	ldr	r3, [pc, #20]
	movs	r1, #1
	ldrb	r2, [r3, #0]
	add	sp, #8
	eors	r2, r1
	strb	r2, [r3, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200a590
	.4byte 0x0200a5a0
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x02009b54
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #8]
	bl 0x02009b54
	pop	{pc}
	.4byte 0x02008b29
	.2byte 0x8ad9
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #200
	adds	r2, #85
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #624]
	sub	sp, #8
	bl 0x02009b54
	movs	r0, #0
	bl 0x02009cfc
	movs	r0, #11
	bl 0x02009be4
	movs	r1, #0
	bl 0x02009b94
	movs	r0, #9
	movs	r1, #3
	bl 0x02009c9c
	movs	r1, #3
	movs	r0, #29
	bl 0x02009c9c
	movs	r0, #21
	bl 0x02009be4
	ldr	r3, [pc, #580]
	movs	r1, #3
	str	r3, [r0, #108]
	movs	r0, #22
	bl 0x02009c34
	movs	r1, #160
	movs	r2, #238
	lsls	r2, r2, #18
	movs	r0, #18
	lsls	r1, r1, #14
	bl 0x02009c2c
	ldr	r1, [pc, #556]
	movs	r0, #18
	bl 0x02009bf4
	movs	r0, #18
	bl 0x02009be4
	adds	r6, r0, #0
	movs	r0, #19
	bl 0x02009be4
	ldr	r5, [pc, #540]
	movs	r1, #176
	movs	r2, #226
	lsls	r2, r2, #18
	str	r0, [r6, #104]
	str	r5, [r6, #108]
	movs	r0, #19
	lsls	r1, r1, #15
	bl 0x02009c2c
	ldr	r1, [pc, #524]
	movs	r0, #19
	bl 0x02009bf4
	movs	r0, #19
	bl 0x02009be4
	adds	r6, r0, #0
	movs	r0, #18
	bl 0x02009be4
	str	r0, [r6, #104]
	movs	r0, #144
	lsls	r0, r0, #4
	str	r5, [r6, #108]
	adds	r0, #8
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000c9c
	movs	r1, #220
	movs	r2, #131
	movs	r0, #30
	lsls	r1, r1, #17
	lsls	r2, r2, #19
	bl 0x02009c2c
.L_02000c9c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000cd4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000cd4
	movs	r1, #228
	movs	r2, #130
	movs	r0, #24
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r1, #236
	movs	r2, #130
	movs	r0, #25
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c2c
.L_02000cd4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000cee
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c2c
	b.n	.L_02000cf8
.L_02000cee:
	movs	r0, #26
	bl 0x02009be4
	ldr	r3, [pc, #368]
	str	r3, [r0, #108]
.L_02000cf8:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000d2a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #105
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000d2a
	movs	r1, #224
	movs	r2, #254
	movs	r0, #7
	lsls	r1, r1, #14
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r0, #7
	movs	r1, #9
	bl 0x02009c34
.L_02000d2a:
	bl 0x02009d24
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #98
	movs	r2, #8
	movs	r3, #9
	movs	r0, #0
	bl 0x02009d3c
	movs	r0, #8
	bl 0x02009be4
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #32
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #9
	bl 0x02009be4
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #135
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000da6
	movs	r0, #28
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #27
	movs	r1, #2
	bl 0x02009c9c
	movs	r1, #210
	movs	r2, #230
	movs	r0, #28
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r1, #210
	movs	r2, #234
	movs	r0, #27
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r1, #135
	lsls	r1, r1, #4
	movs	r0, #1
	adds	r1, #255
	movs	r2, #28
	movs	r3, #27
	bl 0x02009d3c
.L_02000da6:
	movs	r0, #28
	bl 0x02009be4
	movs	r1, #0
	bl 0x02009b94
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #99
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000dcc
	movs	r0, #10
	movs	r1, #1
	bl 0x02009d4c
	bl 0x02008560
.L_02000dcc:
	movs	r0, #129
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000de2
	movs	r0, #67
	movs	r1, #0
	bl 0x02009cec
.L_02000de2:
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000df6
	movs	r0, #68
	movs	r1, #0
	bl 0x02009cec
.L_02000df6:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000e0c
	movs	r0, #70
	movs	r1, #0
	bl 0x02009cec
.L_02000e0c:
	movs	r0, #130
	lsls	r0, r0, #2
	adds	r0, #255
	bl 0x02009b6c
	cmp	r0, #0
	bne.n	.L_02000e22
	movs	r0, #71
	movs	r1, #0
	bl 0x02009cec
.L_02000e22:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #102
	bl 0x02009b6c
	cmp	r0, #0
	beq.n	.L_02000e44
	movs	r3, #54
	movs	r2, #45
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #3
	movs	r1, #1
	movs	r2, #1
	movs	r3, #1
	bl 0x02009b8c
.L_02000e44:
	movs	r0, #31
	bl 0x02009be4
	adds	r6, r0, #0
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	movs	r0, #0
	str	r3, [r6, #20]
	str	r3, [r6, #12]
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02008a6d
	.4byte 0x02009b3d
	.4byte 0x02009d84
	.4byte 0x020080e9
	.2byte 0x9e04
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	ldr	r0, [pc, #516]
	bl 0x02009c6c
	movs	r0, #12
	bl 0x02009be4
	movs	r1, #56
	bl 0x02009b9c
	cmp	r5, #21
	bne.n	.L_02000ed4
	movs	r1, #188
	movs	r2, #242
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c0c
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c8c
	movs	r6, #128
	movs	r2, #16
	movs	r0, #12
	movs	r1, #16
	negs	r2, r2
	movs	r3, #0
	lsls	r6, r6, #8
	movs	r7, #160
	bl 0x02009d0c
	movs	r5, #0
	adds	r6, #10
	lsls	r7, r7, #7
	b.n	.L_02000f0c
.L_02000ed4:
	movs	r1, #138
	movs	r2, #242
	movs	r0, #4
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	bl 0x02009c0c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #16
	movs	r2, #16
	movs	r3, #128
	movs	r0, #12
	negs	r1, r1
	negs	r2, r2
	lsls	r3, r3, #8
	bl 0x02009d0c
	movs	r5, #128
	movs	r6, #10
	movs	r7, #192
	lsls	r5, r5, #8
	negs	r6, r6
	lsls	r7, r7, #6
.L_02000f0c:
	movs	r0, #12
	bl 0x02009c24
	movs	r0, #4
	movs	r1, #12
	bl 0x02009d1c
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #12
	lsls	r1, r1, #9
	bl 0x02009bec
	movs	r0, #12
	movs	r1, #1
	bl 0x02009cb4
	movs	r1, #232
	movs	r2, #238
	lsls	r2, r2, #2
	movs	r0, #12
	lsls	r1, r1, #1
	bl 0x02009c0c
	adds	r1, r5, #0
	movs	r0, #12
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x02009ca4
	movs	r2, #5
	movs	r0, #12
	movs	r1, #0
	bl 0x02009c7c
	adds	r1, r6, #0
	movs	r0, #12
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x02009ca4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #5
	bl 0x02009c7c
	movs	r1, #232
	movs	r2, #196
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #12
	bl 0x02009c0c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	negs	r1, r1
	negs	r2, r2
	bl 0x02009cbc
	movs	r0, #12
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c44
	movs	r2, #23
	movs	r1, #6
	movs	r0, #12
	bl 0x02009c44
	movs	r0, #10
	bl 0x02009bc4
	adds	r1, r7, #0
	movs	r0, #12
	bl 0x02009c94
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #5
	movs	r1, #0
	movs	r0, #12
	bl 0x02009c7c
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #4
	movs	r1, #1
	bl 0x02009c9c
	movs	r1, #232
	movs	r2, #214
	movs	r0, #4
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x02009c2c
	movs	r1, #232
	movs	r2, #199
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x02009c0c
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #12
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #12
	ldr	r1, [pc, #108]
	bl 0x02009bec
	movs	r0, #12
	movs	r1, #2
	bl 0x02009c34
	ldr	r3, [pc, #100]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009be4
	cmp	r0, #0
	beq.n	.L_02001052
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x02009c04
.L_02001052:
	movs	r0, #12
	bl 0x02009c24
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #4
	bl 0x02009be4
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #232
	movs	r2, #194
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #4
	lsls	r2, r2, #2
	bl 0x02009c0c
	bl 0x020081bc
	bl 0x02009bd4
	pop	{r5, r6, r7, pc}
	.4byte 0x00002896
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	ldr	r0, [pc, #168]
	bl 0x02009c6c
	movs	r0, #12
	bl 0x02009be4
	movs	r1, #56
	bl 0x02009b9c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x02009c94
	movs	r0, #12
	movs	r1, #1
	bl 0x02009c9c
	movs	r3, #208
	lsls	r3, r3, #8
	movs	r1, #0
	movs	r2, #16
	movs	r0, #12
	bl 0x02009d0c
	movs	r0, #12
	bl 0x02009c24
	movs	r0, #5
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #5
	bl 0x02009c7c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #12
	ldr	r1, [pc, #92]
	bl 0x02009bec
	movs	r0, #12
	movs	r1, #2
	bl 0x02009c34
	ldr	r3, [pc, #84]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02009be4
	cmp	r0, #0
	beq.n	.L_02001126
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x02009c04
.L_02001126:
	movs	r0, #12
	bl 0x02009c24
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c2c
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #16
	movs	r0, #4
	movs	r1, #0
	negs	r2, r2
	bl 0x02009d14
	bl 0x020081bc
	bl 0x02009bd4
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00002899
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #100
	bl 0x02009b74
	bl 0x02009bcc
	movs	r0, #0
	bl 0x02009d04
	ldr	r0, [pc, #1020]
	bl 0x02009c6c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #24
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009bec
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	lsls	r1, r1, #9
	movs	r0, #25
	bl 0x02009bec
	movs	r0, #12
	bl 0x02009be4
	movs	r1, #56
	bl 0x02009b9c
	movs	r1, #236
	movs	r2, #142
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c0c
	movs	r1, #16
	movs	r3, #208
	lsls	r3, r3, #8
	movs	r0, #12
	negs	r1, r1
	movs	r2, #0
	bl 0x02009d0c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009c8c
	movs	r0, #12
	bl 0x02009c24
	movs	r0, #232
	movs	r1, #1
	movs	r2, #136
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c44
	movs	r0, #12
	movs	r1, #6
	movs	r2, #23
	bl 0x02009c44
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #24
	bl 0x02009ca4
	movs	r0, #24
	movs	r1, #16
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #24
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #12
	movs	r1, #0
.L_02001244:
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009c8c
	movs	r0, #45
	bl 0x02009bc4
	movs	r1, #208
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009c8c
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #16
	movs	r0, #25
	negs	r1, r1
	movs	r2, #0
	bl 0x02009d14
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #232
	movs	r2, #138
	movs	r0, #25
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c0c
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #4
	movs	r1, #3
	bl 0x02009c4c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x02009cac
	movs	r0, #40
	bl 0x02009bc4
	movs	r0, #25
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #25
	movs	r1, #16
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #4
	movs	r1, #3
	bl 0x02009c4c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x02009cac
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #8
.L_02001306:
	movs	r0, #25
	negs	r1, r1
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #6
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #25
	bl 0x02009c3c
.L_0200132a:
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #236
	movs	r2, #130
	movs	r0, #25
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c0c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #25
	movs	r1, #0
	bl 0x02009c7c
	movs	r0, #4
	movs	r1, #3
	bl 0x02009c34
	movs	r1, #3
	movs	r0, #12
	bl 0x02009c3c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #24
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #24
	bl 0x02009ca4
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #8
	movs	r2, #16
	movs	r3, #192
	lsls	r3, r3, #8
	negs	r2, r2
	negs	r1, r1
	movs	r0, #7
	bl 0x02009d0c
	movs	r0, #7
	bl 0x02009c24
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #7
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009c7c
	movs	r0, #24
	movs	r1, #3
	bl 0x02009c4c
	movs	r1, #129
	movs	r0, #24
	lsls	r1, r1, #1
	bl 0x02009cac
	movs	r0, #25
	movs	r1, #3
	bl 0x02009c4c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x02009cac
	movs	r0, #45
	bl 0x02009bc4
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #7
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #24
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #35
	bl 0x02009bc4
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #25
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x02009ca4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #24
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #24
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #7
	bl 0x02009ca4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #12
	bl 0x02009ca4
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #2
	movs	r0, #7
	bl 0x02009c54
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #128
	movs	r2, #60
	lsls	r1, r1, #1
	movs	r0, #12
	bl 0x02009ca4
	movs	r0, #4
	movs	r1, #12
	bl 0x02009d1c
	movs	r0, #7
	movs	r1, #12
	bl 0x02009d1c
	movs	r0, #24
	movs	r1, #12
	bl 0x02009d1c
	movs	r0, #25
	movs	r1, #12
	bl 0x02009d1c
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #12
	lsls	r1, r1, #9
	bl 0x02009bec
	movs	r0, #12
	movs	r1, #1
	bl 0x02009cb4
	movs	r1, #212
	movs	r2, #150
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #12
	bl 0x02009c0c
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #200
	movs	r1, #1
	movs	r2, #162
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #192
	lsls	r1, r1, #6
	b.n	.L_02001578
	.2byte 0x0000
	.2byte 0x2708
	.2byte 0x0000
.L_02001578:
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #1
	bl 0x02009cb4
	movs	r1, #216
	movs	r2, #134
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #12
	bl 0x02009c0c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #40
	bl 0x02009bc4
	movs	r0, #220
	movs	r1, #1
	movs	r2, #144
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #60
	bl 0x02009bc4
	movs	r0, #220
	movs	r1, #1
	movs	r2, #132
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #12
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #1
	bl 0x02009cb4
	movs	r1, #228
	movs	r2, #142
	movs	r0, #12
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x02009c0c
	movs	r0, #232
	movs	r1, #1
	movs	r2, #136
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #18
	bl 0x02009cbc
	bl 0x02009cc4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #7
	adds	r1, #51
	adds	r2, #153
	bl 0x02009bec
	movs	r0, #7
	movs	r1, #8
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009c8c
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #0
	movs	r0, #7
	bl 0x02009c74
	movs	r0, #4
	movs	r1, #0
	bl 0x02009bdc
	cmp	r0, #0
	bne.n	.L_0200175e
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #4
	bl 0x02009c3c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009c7c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200178e
.L_0200175e:
	movs	r0, #40
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #7
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #7
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
.L_0200178e:
	movs	r0, #4
	movs	r1, #7
	bl 0x02009d1c
	movs	r0, #12
	movs	r1, #7
	bl 0x02009d1c
	movs	r0, #24
	movs	r1, #7
	bl 0x02009d1c
	movs	r0, #25
	movs	r1, #7
	bl 0x02009d1c
	movs	r0, #7
	movs	r1, #2
	bl 0x02009c9c
	movs	r0, #7
	movs	r1, #32
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #192
	movs	r0, #24
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #4
	bl 0x02009c3c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #2
	movs	r0, #7
	bl 0x02009c54
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #7
	bl 0x02009ca4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #50
	bl 0x02009bc4
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #12
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #7
	bl 0x02009ca4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #12
	bl 0x02009ca4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r0, #12
	movs	r1, #6
	movs	r2, #15
	bl 0x02009c44
	movs	r2, #23
	movs	r1, #6
	movs	r0, #12
	bl 0x02009c44
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #2
	movs	r0, #7
	bl 0x02009c54
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #2
	movs	r0, #24
	bl 0x02009c54
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #24
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #2
	movs	r0, #25
	bl 0x02009c54
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #50
	bl 0x02009bc4
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #30
	bl 0x02009bc4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #12
	bl 0x02009ca4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #4
	bl 0x02009ca4
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x02009c7c
	movs	r0, #7
	movs	r1, #4
	bl 0x02009c3c
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #3
	movs	r0, #24
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #24
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #2
	movs	r0, #25
	bl 0x02009c54
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #45
	movs	r0, #7
	bl 0x02009ca4
	movs	r1, #16
	movs	r0, #7
	negs	r1, r1
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #7
	bl 0x02009c8c
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #7
	movs	r1, #0
	bl 0x02009c7c
	movs	r1, #3
	movs	r0, #24
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r2, #10
	movs	r0, #24
	movs	r1, #0
	bl 0x02009c7c
	movs	r0, #25
	movs	r1, #4
	bl 0x02009c3c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #16
	movs	r0, #7
	negs	r1, r1
	movs	r2, #0
	bl 0x02009d14
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #7
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r0, #7
	movs	r1, #0
	movs	r2, #10
	bl 0x02009c7c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #12
	bl 0x02009c8c
	movs	r0, #20
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #12
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #3
	movs	r0, #4
	bl 0x02009c3c
	movs	r0, #10
	bl 0x02009bc4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x02009c8c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #7
	ldr	r1, [pc, #132]
	bl 0x02009bec
	movs	r0, #7
	movs	r1, #2
	bl 0x02009c34
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x02009be4
	cmp	r0, #0
	beq.n	.L_02001adc
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x02009c04
.L_02001adc:
	movs	r0, #7
	bl 0x02009c24
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c2c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #64]
	adds	r2, #153
	bl 0x02009bec
	movs	r0, #12
	movs	r1, #2
	bl 0x02009c34
	ldr	r0, [r5, #0]
	bl 0x02009be4
	cmp	r0, #0
	beq.n	.L_02001b1a
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #12
	bl 0x02009c04
.L_02001b1a:
	movs	r0, #12
	bl 0x02009c24
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x02009c2c
	bl 0x020081bc
	bl 0x02009bd4
	pop	{r5, pc}
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x02009d6c
	pop	{pc}
	.irp EntryTarget, 0x03000508, 0x080000c1, 0x080000d1, 0x08000101, 0x08000119, 0x080003c9, 0x080003d1, 0x08020091, 0x08020171, 0x080201f1, 0x08020219, 0x08020239, 0x08020291, 0x08038041, 0x080ad209, 0x080ad2e9, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80c1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8161, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8269, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c8421, 0x080c8429, 0x080c8481, 0x080c84e1, 0x080c85e9, 0x080c85f9, 0x080c8601, 0x080c86a9, 0x080c86b9, 0x080c86c1, 0x080c86e9, 0x080c8761, 0x080c8781, 0x080c87e9, 0x080c8839, 0x080c8849, 0x080c8879, 0x080c88e1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffff0000
	.4byte 0x000001cc
	.4byte 0x40000226
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ca
	.4byte 0x101010cb
	.4byte 0xffffffff
	.4byte 0x102020cb
	.4byte 0xffffffff
	.4byte 0x103030cb
	.4byte 0xffffffff
	.4byte 0x104030c9
	.4byte 0xffffffff
	.4byte 0x105010cd
	.4byte 0xffffffff
	.4byte 0x10c0c0cb
	.4byte 0xffffffff
	.4byte 0x10f0f0cb
	.4byte 0xffffffff
	.4byte 0x110100cb
	.4byte 0xffffffff
	.4byte 0x10b010cc
	.4byte 0xffffffff
	.4byte 0x10a0a0cc
	.4byte 0xffffffff
	.4byte 0x111110cc
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03b80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x01024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00024000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x04e80000
	.4byte 0x00018000
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x04100000
	.4byte 0x00014000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00015000
	.4byte 0xffff00a4
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00018000
	.4byte 0xffff00a6
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00015000
	.4byte 0xffff00a7
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x0002c000
	.4byte 0xffff00a8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00024000
	.4byte 0xffff00d8
	.4byte 0x00000002
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x04500000
	.4byte 0x00004000
	.4byte 0xffff00bd
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00004000
	.4byte 0xffff0050
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x04380000
	.4byte 0x00020000
	.4byte 0xffff00cb
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x03d80000
	.4byte 0x00004000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00013000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00015000
	.4byte 0xffff00a6
	.4byte 0x00000002
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03f00000
	.4byte 0x00018000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x0002d000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03d80000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00010002
	.4byte 0x00010001
	.4byte 0x00020006
	.4byte 0x00010002
	.4byte 0x00060001
	.4byte 0x0000ffff
	.4byte 0x00020001
	.4byte 0x00060001
	.4byte 0x00020000
	.4byte 0x00010002
	.4byte 0xffff0006
	.4byte 0x00010003
	.4byte 0x00010001
	.4byte 0xffff0000
	.4byte 0x0200a184
	.4byte 0x002f0016
	.4byte 0x0200a184
	.4byte 0x002f0023
	.4byte 0x0200a19a
	.4byte 0x00280009
	.4byte 0x0200a1b0
	.4byte 0x002d0036
	.4byte 0x0200a19a
	.4byte 0x0014001c
	.4byte 0x0200a19a
	.4byte 0x000d001c
	.4byte 0x0200a184
	.4byte 0x00320036
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008279
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008279
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008279
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000c401
	.4byte 0x19080005
	.4byte 0x00000005
	.4byte 0x0000c602
	.4byte 0x1966000c
	.4byte 0x02008279
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x0000c602
	.4byte 0x19690010
	.4byte 0x02008279
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x02008279
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02008279
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x09640014
	.4byte 0x02009161
	.4byte 0x00000002
	.4byte 0x09640015
	.4byte 0x02008e7d
	.4byte 0x00000002
	.4byte 0x09640016
	.4byte 0x02008e7d
	.4byte 0x00000002
	.4byte 0x09640017
	.4byte 0x0200909d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000272f
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002730
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002731
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002732
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002733
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002734
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008309
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008399
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002737
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002738
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002739
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000273a
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x0000273b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000273c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000273d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000273e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000273f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002740
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002741
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x02008309
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x02008399
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002744
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002745
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002746
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002747
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte 0x00002748
	.4byte 0x00000000
	.4byte 0x196a0018
	.4byte 0x0000286a
	.4byte 0x00000000
	.4byte 0x196a0019
	.4byte 0x0000286b
	.4byte 0x00008d15
	.4byte 0x196a0018
	.4byte 0x0000286c
	.4byte 0x00008d15
	.4byte 0x196a0019
	.4byte 0x0000286d
	.4byte 0x00000000
	.4byte 0x19690018
	.4byte 0x000027f2
	.4byte 0x00000000
	.4byte 0x19690019
	.4byte 0x000027f3
	.4byte 0x00008d15
	.4byte 0x19690018
	.4byte 0x000027f4
	.4byte 0x00008d15
	.4byte 0x19690019
	.4byte 0x000027f5
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000272b
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x0000272c
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000272d
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000272e
	.4byte 0x00000000
	.4byte 0x196a001a
	.4byte 0x00002873
	.4byte 0x00008d15
	.4byte 0x196a001a
	.4byte 0x00002879
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x000027fb
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00002801
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte 0x00002782
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x000028a0
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x000028a1
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x02008935
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x000028ad
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x020088e9
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte 0x020088e9
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte 0x0200862d
	.4byte 0x00008515
	.4byte 0x09620008
	.4byte 0x00000000
	.4byte 0x00000c15
	.4byte 0x0963000a
	.4byte 0x02008561
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte 0x02008429
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte 0x02008449
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte 0x02008469
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte 0x02008489
	.4byte 0x50008905
	.4byte 0xffff0026
	.4byte 0x020084a9
	.4byte 0x50008805
	.4byte 0x03030068
	.4byte 0x020085dd
	.4byte 0x50008805
	.4byte 0x03040069
	.4byte 0x020085ed
	.4byte 0x50008805
	.4byte 0x0306006b
	.4byte 0x020085fd
	.4byte 0x50008805
	.4byte 0x0307006c
	.4byte 0x0200860d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
