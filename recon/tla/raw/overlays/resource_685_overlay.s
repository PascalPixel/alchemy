.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020084d5, 0x0200805d, 0x02008069, 0x02008071, 0x02008479, 0x02008065, 0x020085e5
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	ldr	r3, [r0, #24]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	str	r3, [r0, #24]
	cmp	r3, r2
	blt.n	.L_02000056
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	movs	r3, #0
	str	r3, [r0, #108]
.L_02000056:
	ldr	r3, [r0, #24]
	str	r3, [r0, #28]
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb660
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb690
	.2byte 0x0200
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #126
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_020000b4
	ldr	r3, [pc, #164]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #156]
	cmp	r2, r3
	bne.n	.L_02000096
	ldr	r0, [pc, #156]
	b.n	.L_02000124
.L_02000096:
	ldr	r3, [pc, #156]
	cmp	r2, r3
	bne.n	.L_020000a0
	ldr	r0, [pc, #152]
	b.n	.L_02000124
.L_020000a0:
	ldr	r3, [pc, #152]
	cmp	r2, r3
	bne.n	.L_020000aa
	ldr	r0, [pc, #152]
	b.n	.L_02000124
.L_020000aa:
	ldr	r3, [pc, #152]
	cmp	r2, r3
	bne.n	.L_02000122
	ldr	r0, [pc, #148]
	b.n	.L_02000124
.L_020000b4:
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200b404
	ldr	r3, [pc, #104]
	ldr	r1, [pc, #104]
	cmp	r0, #0
	beq.n	.L_020000f6
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, r1
	bne.n	.L_020000d8
	ldr	r0, [pc, #116]
	b.n	.L_02000124
.L_020000d8:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020000e2
	ldr	r0, [pc, #112]
	b.n	.L_02000124
.L_020000e2:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020000ec
	ldr	r0, [pc, #104]
	b.n	.L_02000124
.L_020000ec:
	ldr	r3, [pc, #84]
	cmp	r2, r3
	bne.n	.L_02000122
	ldr	r0, [pc, #100]
	b.n	.L_02000124
.L_020000f6:
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, r1
	beq.n	.L_02000122
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_0200010e
	ldr	r0, [pc, #80]
	b.n	.L_02000124
.L_0200010e:
	ldr	r3, [pc, #44]
	cmp	r2, r3
	bne.n	.L_02000118
	ldr	r0, [pc, #72]
	b.n	.L_02000124
.L_02000118:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000122
	ldr	r0, [pc, #68]
	b.n	.L_02000124
.L_02000122:
	ldr	r0, [pc, #68]
.L_02000124:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b9
	.4byte 0x0200baec
	.4byte 0x000000ba
	.4byte 0x0200bb94
	.4byte 0x000000bb
	.4byte 0x0200bc0c
	.4byte 0x000000bc
	.4byte 0x0200bc84
	.4byte 0x0200b8ac
	.4byte 0x0200b90c
	.4byte 0x0200b984
	.4byte 0x0200b9cc
	.4byte 0x0200b7a4
	.4byte 0x0200b81c
	.4byte 0x0200b864
	.2byte 0xb75c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	adds	r6, r2, #0
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	movs	r0, #158
	bl 0x0200b62c
	ldrh	r1, [r5, #4]
	ldrh	r2, [r5, #6]
	ldr	r0, [r5, #0]
	bl 0x0200b414
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200b484
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200b4dc
	ldr	r0, [r5, #0]
	cmp	r6, #0
	bne.n	.L_020001ca
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	bl 0x0200b4bc
	b.n	.L_020001d4
.L_020001ca:
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b4c4
.L_020001d4:
	movs	r0, #10
	bl 0x0200b454
	adds	r0, r7, #0
	bl 0x0200b574
	bl 0x0200b59c
	bl 0x0200b5a4
	bl 0x0200b464
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #8]
	bl 0x0200816c
	pop	{pc}
	.2byte 0x0000
	.2byte 0xbd10
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #232]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r2, #212
	ldr	r6, [pc, #224]
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	movs	r1, #168
	bl 0x0200b4b4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b534
	movs	r0, #158
	bl 0x0200b62c
	ldrh	r1, [r6, #4]
	ldrh	r2, [r6, #6]
	ldr	r0, [r6, #0]
	bl 0x0200b414
	movs	r0, #8
	bl 0x0200b47c
	movs	r3, #0
	adds	r0, #85
	movs	r1, #184
	movs	r2, #204
	strb	r3, [r0, #0]
	lsls	r1, r1, #16
	movs	r0, #8
	lsls	r2, r2, #17
	bl 0x0200b4d4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b484
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b5dc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	ldr	r5, [pc, #128]
	adds	r0, r5, #0
	bl 0x0200b50c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b514
	bl 0x0200b5f4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_020002a4
	movs	r0, #10
	bl 0x0200b454
	adds	r0, r5, #1
	bl 0x0200b50c
	b.n	.L_020002b0
.L_020002a4:
	movs	r0, #20
	bl 0x0200b454
	adds	r0, r5, #2
	bl 0x0200b50c
.L_020002b0:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b524
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b484
	movs	r2, #14
	movs	r0, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b5dc
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b4d4
	ldr	r6, [pc, #28]
	ldr	r0, [r6, #0]
	ldrh	r1, [r6, #4]
	ldrh	r2, [r6, #6]
	bl 0x0200b414
	movs	r0, #158
	bl 0x0200b62c
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0200bd10
	.4byte 0x0000241d
	.2byte 0xbd18
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200b50c
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200b514
	bl 0x0200b5f4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_02000334
	movs	r0, #10
	bl 0x0200b454
	adds	r0, r5, #1
	bl 0x0200b50c
	b.n	.L_02000340
.L_02000334:
	movs	r0, #20
	bl 0x0200b454
	adds	r0, r5, #2
	bl 0x0200b50c
.L_02000340:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200b524
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x2600
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [pc, #44]
	bl 0x0200b50c
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200b524
	adds	r0, r5, #0
	bl 0x0200b47c
	ldr	r3, [pc, #28]
.L_0200036a:
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200b47c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl 0x0200b624
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000025fa
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [pc, #36]
	bl 0x0200b50c
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200b524
	movs	r1, #208
	adds	r0, r5, #0
	lsls	r1, r1, #8
	bl 0x0200b534
	adds	r0, r5, #0
	bl 0x0200b47c
	movs	r1, #8
	bl 0x0200b5ec
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x25ff
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #77
	str	r3, [sp, #4]
	movs	r5, #41
	movs	r0, #41
	movs	r1, #97
	movs	r2, #1
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200b424
	movs	r3, #13
	str	r3, [sp, #4]
	movs	r0, #41
	movs	r1, #33
	movs	r2, #1
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200b41c
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	sub	sp, #8
	movs	r3, #77
	str	r3, [sp, #4]
	movs	r5, #41
	movs	r0, #50
	movs	r1, #97
	movs	r2, #1
	movs	r3, #4
	str	r5, [sp, #0]
	bl 0x0200b424
	movs	r3, #13
	str	r3, [sp, #4]
	movs	r1, #33
	movs	r2, #1
	movs	r3, #4
	movs	r0, #46
	str	r5, [sp, #0]
	bl 0x0200b41c
	ldr	r5, [pc, #68]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r6, r5, r0
	ldr	r0, [r6, #0]
	bl 0x0200b47c
	ldr	r3, [r0, #8]
	movs	r2, #240
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	lsls	r2, r2, #1
	asrs	r1, r3, #20
	adds	r3, r5, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000456
	cmp	r4, #41
	bne.n	.L_02000456
	adds	r3, r1, #0
	subs	r3, #13
	cmp	r3, #4
	bhi.n	.L_02000456
	movs	r1, #166
	movs	r2, #140
	ldr	r0, [r6, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b4d4
.L_02000456:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00bc
	.2byte 0x0000
	push	{lr}
	movs	r2, #144
	movs	r1, #163
	lsls	r2, r2, #4
	lsls	r1, r1, #1
	adds	r2, #120
	bl 0x0200b474
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #52]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #44]
	cmp	r2, r3
	beq.n	.L_020004aa
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000496
	ldr	r0, [pc, #40]
	b.n	.L_020004ac
.L_02000496:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_020004a0
	ldr	r0, [pc, #36]
	b.n	.L_020004ac
.L_020004a0:
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_020004aa
	ldr	r0, [pc, #36]
	b.n	.L_020004ac
.L_020004aa:
	ldr	r0, [pc, #36]
.L_020004ac:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b9
	.4byte 0x000000ba
	.4byte 0x0200be7c
	.4byte 0x000000bb
	.4byte 0x0200bffc
	.4byte 0x000000bc
	.4byte 0x0200c104
	.2byte 0xbd20
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r6, [pc, #240]
	adds	r2, #85
	str	r2, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r5, r6, r3
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #228]
	cmp	r2, r3
	beq.n	.L_0200050e
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	bl 0x0200b47c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_0200050e:
	movs	r0, #0
	bl 0x0200b5ac
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #192]
	cmp	r2, r3
	bne.n	.L_020005a6
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r5, r6, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #98
	bne.n	.L_02000530
	bl 0x02009774
.L_02000530:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #99
	bne.n	.L_02000548
	movs	r3, #245
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #23
	strh	r3, [r2, #0]
	strh	r3, [r5, #0]
	bl 0x02009868
.L_02000548:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_02000588
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #120
	bl 0x0200b404
	cmp	r0, #0
	bne.n	.L_02000588
	movs	r1, #163
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200b61c
	movs	r1, #166
	movs	r2, #232
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r0, #10
	bl 0x0200b47c
	movs	r3, #4
	adds	r0, #85
	strb	r3, [r0, #0]
.L_02000588:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_020005ce
	movs	r1, #166
	movs	r2, #200
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	b.n	.L_020005ce
.L_020005a6:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020005ce
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #126
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_020005ce
	movs	r1, #11
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #8
	bl 0x0200b47c
	movs	r1, #0
	bl 0x0200b42c
.L_020005ce:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b9
	.4byte 0x000000bc
	.2byte 0x00bb
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	ldr	r5, [pc, #140]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #162
	movs	r2, #200
	adds	r6, r0, #0
	lsls	r1, r1, #2
	ldr	r0, [r5, #0]
	bl 0x0200b4b4
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	ldr	r0, [r5, #0]
	movs	r1, #6
	bl 0x0200b4e4
	movs	r0, #7
	bl 0x0200b454
	adds	r1, r6, #0
	movs	r0, #10
	bl 0x0200b61c
	movs	r1, #162
	movs	r2, #196
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	movs	r0, #10
	bl 0x0200b4d4
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #16
	movs	r2, #8
	negs	r1, r1
	negs	r2, r2
	ldr	r0, [r5, #0]
	bl 0x0200b5dc
	movs	r0, #1
	bl 0x0200b454
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r1, #204
	adds	r2, #102
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200b484
	movs	r0, #8
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r7, #254
	adds	r3, r7, #0
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #6
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b5dc
	movs	r0, #1
	bl 0x0200b454
	movs	r0, #8
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #8
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	movs	r1, #0
	movs	r2, #0
	movs	r0, #10
	bl 0x0200b4d4
	movs	r0, #15
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #1
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	cmp	r5, #0
	bne.n	.L_02000730
	movs	r0, #8
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r7, #0
	ands	r3, r2
	movs	r2, #6
	strb	r3, [r0, #0]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #8
	bl 0x0200b5dc
	movs	r0, #1
	bl 0x0200b454
	movs	r0, #8
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200b454
.L_02000730:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	movs	r0, #0
	adds	r7, r2, #0
	mov	r9, r0
	str	r1, [sp, #8]
	cmp	r9, r7
	bgt.n	.L_020007f4
.L_02000752:
	movs	r2, #4
	str	r2, [sp, #4]
	movs	r1, #0
	mov	fp, r1
.L_0200075a:
	ldr	r0, [sp, #12]
	mov	r3, fp
	ldrh	r5, [r3, r0]
	movs	r3, #31
	mov	sl, r3
	ldr	r1, [pc, #56]
	mov	r2, sl
	ands	r2, r5
	lsls	r5, r5, #16
	lsrs	r0, r5, #21
	ands	r0, r1
	mov	r8, r0
	ldr	r0, [sp, #8]
	mov	sl, r2
	mov	r2, fp
	ldrh	r6, [r2, r0]
	lsrs	r5, r5, #26
	ands	r3, r6
	lsls	r6, r6, #16
	lsrs	r2, r6, #21
	lsrs	r6, r6, #26
	ands	r2, r1
	ands	r5, r1
	ands	r6, r1
	mov	r1, sl
	subs	r3, r3, r1
	mov	r0, r9
	muls	r0, r3
	adds	r1, r7, #0
	str	r2, [sp, #0]
	bl 0x0200b3a4
	ldr	r2, [sp, #0]
	mov	r3, r8
	b.n	.L_020007a4
	.2byte 0x001f
	.2byte 0x0000
.L_020007a4:
	subs	r2, r2, r3
	adds	r1, r7, #0
	add	sl, r0
	mov	r0, r9
	muls	r0, r2
	bl 0x0200b3a4
	subs	r6, r6, r5
	add	r8, r0
	adds	r1, r7, #0
	mov	r0, r9
	muls	r0, r6
	bl 0x0200b3a4
	adds	r5, r5, r0
	lsls	r5, r5, #10
	mov	r0, r8
	movs	r3, #160
	lsls	r0, r0, #5
	lsls	r3, r3, #19
	orrs	r5, r0
	mov	r1, sl
	adds	r3, #228
	orrs	r5, r1
	add	r3, fp
	strh	r5, [r3, #0]
	movs	r2, #2
	ldr	r3, [sp, #4]
	add	fp, r2
	subs	r3, #1
	str	r3, [sp, #4]
	cmp	r3, #0
	bge.n	.L_0200075a
	movs	r0, #1
	bl 0x0200b3ac
	movs	r0, #1
	add	r9, r0
	cmp	r9, r7
	ble.n	.L_02000752
.L_020007f4:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	ldr	r0, [pc, #352]
	bl 0x0200b50c
	adds	r0, r5, #0
	bl 0x020085e8
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b534
	movs	r1, #7
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #2
	movs	r2, #50
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #7
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #6
	movs	r2, #50
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #7
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #128
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b4fc
	movs	r0, #8
	bl 0x0200b454
	movs	r2, #5
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b51c
	movs	r0, #0
	bl 0x0200867c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b514
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_02000930
	movs	r0, #20
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000952
.L_02000930:
	movs	r0, #35
	bl 0x0200b454
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
.L_02000952:
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b534
	bl 0x0200b464
	pop	{r5, pc}
	.2byte 0x266e
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	ldr	r0, [pc, #168]
	bl 0x0200b50c
	adds	r0, r5, #0
	bl 0x020085e8
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	bl 0x0200b534
	movs	r1, #7
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #129
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #7
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #132
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #15
	bl 0x0200b454
	movs	r2, #5
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b51c
	movs	r0, #0
	bl 0x0200867c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b534
	bl 0x0200b464
	pop	{r5, pc}
	.2byte 0x2678
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #10
	sub	sp, #64
	bl 0x0200b47c
	ldr	r3, [pc, #264]
	adds	r7, r0, #0
	add	r0, sp, #28
	str	r3, [r0, #0]
	movs	r2, #160
	movs	r3, #0
	str	r3, [r0, #4]
	lsls	r2, r2, #19
	movs	r3, #238
	lsls	r3, r3, #16
	adds	r2, #228
	add	r1, sp, #52
	str	r3, [r0, #8]
	ldrh	r3, [r2, #0]
	mov	r9, r1
	mov	r4, r9
	strh	r3, [r4, #0]
	adds	r2, #2
	ldrh	r3, [r2, #0]
	mov	fp, r0
	mov	r0, r9
	strh	r3, [r0, #2]
	adds	r2, #2
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r1, #4]
	add	r6, sp, #40
	ldrh	r3, [r2, #0]
	strh	r3, [r4, #6]
	ldrh	r3, [r2, #2]
	movs	r2, #160
	strh	r3, [r0, #8]
	lsls	r2, r2, #19
	adds	r2, #196
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r6, #0]
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r6, #2]
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r6, #4]
	ldrh	r3, [r2, #0]
	strh	r3, [r6, #6]
	ldrh	r3, [r2, #2]
	strh	r3, [r6, #8]
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	ldr	r0, [pc, #164]
	bl 0x0200b50c
	adds	r0, r5, #0
	bl 0x020085e8
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #0
	movs	r2, #5
	movs	r0, #8
	bl 0x0200b51c
	movs	r0, #1
	bl 0x0200867c
	movs	r1, #166
	movs	r2, #200
	movs	r0, #8
	lsls	r1, r1, #2
	bl 0x0200b4b4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b514
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_02000b60
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #0
	movs	r2, #5
	movs	r0, #8
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000b82
	.2byte 0x0000
	.4byte 0x02960000
	.2byte 0x267d
	.2byte 0x0000
.L_02000b60:
	movs	r0, #40
	bl 0x0200b454
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
.L_02000b82:
	ldr	r3, [pc, #952]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r3, r3, r4
	movs	r1, #128
	ldr	r0, [r3, #0]
	movs	r2, #0
	lsls	r1, r1, #6
	bl 0x0200b52c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #166
	movs	r1, #1
	movs	r2, #232
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	bl 0x0200b55c
	bl 0x0200b564
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #8
	adds	r1, #255
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b544
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r0, #0
	mov	r8, r0
.L_02000be0:
	ldr	r3, [pc, #860]
	add	r5, sp, #16
	adds	r2, r5, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	movs	r1, #9
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #3
	bl 0x0200b454
	mov	r1, r8
	lsls	r3, r1, #2
	ldr	r1, [r5, r3]
	movs	r0, #10
	bl 0x0200b61c
	movs	r1, #167
	movs	r2, #200
	movs	r0, #10
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #72]
	movs	r2, #2
	movs	r0, #10
	movs	r1, #4
	bl 0x0200b4ec
	movs	r0, #10
	movs	r1, #1
	bl 0x0200b53c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #10
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b484
	movs	r1, #0
	movs	r2, #40
	movs	r0, #10
	bl 0x0200b4c4
	movs	r0, #155
	lsls	r0, r0, #1
	bl 0x0200b62c
	movs	r0, #34
	bl 0x0200b454
	ldr	r3, [r7, #8]
	add	r2, sp, #4
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r1, #0
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	movs	r0, #10
	str	r3, [r2, #8]
	movs	r2, #0
	bl 0x0200b4d4
	movs	r0, #20
	bl 0x0200b454
	movs	r0, #134
	bl 0x0200b62c
	mov	r0, r9
	adds	r1, r6, #0
	movs	r2, #12
	bl 0x02008734
	movs	r2, #48
	adds	r0, r6, #0
	mov	r1, r9
	bl 0x02008734
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #2
	ble.n	.L_02000be0
	ldr	r5, [pc, #672]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r1, #254
	mov	sl, r1
	mov	r3, sl
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #12
	movs	r2, #12
	ldr	r0, [r5, #0]
	bl 0x0200b5dc
	movs	r0, #1
	bl 0x0200b454
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	adds	r0, #90
	movs	r2, #1
	ldrb	r3, [r0, #0]
	mov	r8, r2
	mov	r4, r8
	orrs	r3, r4
	strb	r3, [r0, #0]
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #6
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl 0x0200b4dc
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #7
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #60
	bl 0x0200b454
	movs	r0, #78
	bl 0x0200b62c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #60
	movs	r0, #8
	bl 0x0200b544
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #5
	adds	r0, #8
	bl 0x0200b51c
	movs	r0, #20
	bl 0x0200b454
	movs	r0, #220
	bl 0x0200b62c
	movs	r2, #48
	mov	r0, r9
	adds	r1, r6, #0
	bl 0x02008734
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200b54c
	movs	r0, #50
	bl 0x0200b454
	movs	r1, #2
	movs	r2, #60
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #128
	lsls	r1, r1, #5
	movs	r0, #16
	adds	r1, #144
	bl 0x0200b60c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #10
	lsls	r0, r0, #10
	bl 0x0200b434
	mov	r0, fp
	bl 0x0200b164
	movs	r0, #8
	movs	r1, #1
	bl 0x0200b4dc
	movs	r1, #6
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b4ec
	movs	r0, #194
	bl 0x0200b62c
	movs	r0, #4
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r1, sl
	adds	r2, r1, #0
	ands	r2, r3
	strb	r2, [r0, #0]
	movs	r1, #12
	movs	r2, #12
	negs	r2, r2
	negs	r1, r1
	movs	r0, #4
	bl 0x0200b5dc
	movs	r0, #1
	bl 0x0200b454
	movs	r0, #4
	bl 0x0200b47c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r4, r8
	orrs	r4, r3
	strb	r4, [r0, #0]
	movs	r0, #10
	mov	r8, r4
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b54c
	movs	r0, #30
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
.L_02000e04:
	lsls	r2, r2, #9
	lsls	r0, r0, #11
	bl 0x0200b434
	mov	r0, fp
	bl 0x0200b20c
	movs	r0, #194
	bl 0x0200b62c
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b4ec
	movs	r1, #6
	movs	r2, #23
	movs	r0, #8
	bl 0x0200b4ec
	movs	r0, #194
	bl 0x0200b62c
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #163
	movs	r0, #10
	lsls	r1, r1, #1
	bl 0x0200b61c
	movs	r1, #166
	movs	r2, #232
	lsls	r2, r2, #16
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200b4d4
	adds	r5, r7, #0
	movs	r0, #10
	movs	r1, #1
	bl 0x0200b53c
	adds	r5, #85
	movs	r3, #2
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #13
	str	r3, [r7, #12]
	ldr	r3, [pc, #200]
	movs	r0, #0
	str	r3, [r7, #108]
	movs	r1, #5
	movs	r2, #0
	str	r0, [r7, #20]
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	movs	r0, #10
	bl 0x0200b4ec
	movs	r0, #8
	bl 0x0200b614
	bl 0x0200b2a4
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r0, r0
	negs	r1, r1
	adds	r2, #102
	bl 0x0200b434
	ldr	r3, [r7, #12]
	cmp	r3, #0
	ble.n	.L_02000ebe
.L_02000eb2:
	movs	r0, #1
	bl 0x0200b3ac
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bgt.n	.L_02000eb2
.L_02000ebe:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #0
	str	r3, [r7, #12]
	movs	r3, #4
	strb	r3, [r5, #0]
	bl 0x0200b5bc
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b4fc
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #10
	movs	r1, #2
	bl 0x0200b53c
	bl 0x0200b464
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200b634
	.2byte 0x8039
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #464]
	movs	r1, #133
	lsls	r1, r1, #2
.L_02000f56:
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200b47c
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	cmp	r3, #41
	ble.n	.L_02000f94
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
	strh	r3, [r2, #0]
.L_02000f78:
	b.n	.L_0200111c
.L_02000f7a:
	adds	r0, r5, #0
	bl 0x0200b44c
	b.n	.L_02000fbc
.L_02000f82:
	movs	r5, #186
	adds	r5, #255
	adds	r0, r5, #0
	bl 0x0200b444
	cmp	r0, r8
	bne.n	.L_02000f7a
	adds	r5, r0, #0
	b.n	.L_02000fbc
.L_02000f94:
	movs	r5, #184
	adds	r5, #255
	adds	r0, r5, #0
	bl 0x0200b444
	movs	r2, #1
	negs	r2, r2
	mov	sl, r0
	cmp	r0, r2
	bne.n	.L_02000f7a
	adds	r5, #1
	adds	r0, r5, #0
	bl 0x0200b444
	mov	r8, r0
	cmp	r0, sl
	beq.n	.L_02000f82
	adds	r0, r5, #0
	bl 0x0200b44c
.L_02000fbc:
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L_020010ae
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #117
	bl 0x0200b404
	cmp	r0, #0
	bne.n	.L_02000fe4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #117
	bl 0x0200b40c
	adds	r0, r5, #0
	bl 0x02008804
	b.n	.L_0200111c
.L_02000fe4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200b404
	cmp	r0, #0
	bne.n	.L_02001004
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200b40c
	adds	r0, r5, #0
	bl 0x02008978
	b.n	.L_0200111c
.L_02001004:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_02001014
	b.n	.L_0200111c
.L_02001014:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200b40c
	movs	r0, #164
	movs	r1, #208
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #224
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #240
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #128
	movs	r3, #4
	lsls	r1, r1, #17
	movs	r2, #0
	negs	r3, r3
	lsls	r0, r0, #18
	bl 0x0200b43c
	adds	r0, r5, #0
	bl 0x02008a34
	movs	r0, #164
	movs	r1, #208
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #224
.L_02001080:
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #240
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #128
	lsls	r0, r0, #18
	lsls	r1, r1, #17
	movs	r2, #0
.L_020010a6:
	movs	r3, #0
	bl 0x0200b43c
	b.n	.L_0200111c
.L_020010ae:
	ldr	r3, [pc, #116]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r0, r6, #0
	ldr	r1, [r3, #0]
	movs	r2, #5
	bl 0x0200b504
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #117
	bl 0x0200b404
	cmp	r0, #0
	bne.n	.L_020010d6
	ldr	r0, [pc, #88]
	bl 0x0200b50c
	b.n	.L_02001108
.L_020010d6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #118
	bl 0x0200b404
	cmp	r0, #0
	bne.n	.L_020010ec
	ldr	r0, [pc, #68]
	bl 0x0200b50c
	b.n	.L_02001108
.L_020010ec:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #119
	bl 0x0200b404
	cmp	r0, #0
	bne.n	.L_02001102
	ldr	r0, [pc, #52]
	bl 0x0200b50c
	b.n	.L_02001108
.L_02001102:
	ldr	r0, [pc, #48]
	bl 0x0200b50c
.L_02001108:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200b524
	movs	r1, #160
	adds	r0, r6, #0
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
.L_0200111c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0000266c
	.4byte 0x00002676
	.4byte 0x0000267b
	.2byte 0x2688
	.2byte 0x0000
	push	{lr}
	ldr	r3, [r0, #24]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #9
	str	r3, [r0, #24]
	cmp	r3, r2
	blt.n	.L_02001156
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #24]
	movs	r3, #0
	str	r3, [r0, #108]
.L_02001156:
	ldr	r3, [r0, #24]
	str	r3, [r0, #28]
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #13
	sub	sp, #36
	bl 0x0200b47c
.L_0200116c:
	adds	r7, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #126
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_0200117e
	b.n	.L_0200174a
.L_0200117e:
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	ldr	r3, [pc, #952]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #146
	ldr	r0, [r5, #0]
	lsls	r1, r1, #2
	movs	r2, #248
	bl 0x0200b4b4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200b404
	cmp	r0, #0
	beq.n	.L_02001216
	movs	r1, #0
	ldr	r0, [r5, #0]
	bl 0x0200b534
	ldr	r0, [pc, #916]
	bl 0x0200b50c
	movs	r1, #0
	movs	r2, #5
	movs	r0, #11
	bl 0x0200b51c
	movs	r0, #36
	bl 0x0200b62c
	movs	r0, #166
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200b55c
	bl 0x0200b564
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	b.n	.L_02001466
.L_02001216:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200b40c
	ldr	r0, [pc, #808]
	bl 0x0200b50c
	movs	r1, #0
	movs	r2, #5
	movs	r0, #11
	bl 0x0200b51c
	movs	r0, #36
	bl 0x0200b62c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #166
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #18
	bl 0x0200b55c
	bl 0x0200b564
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b4ec
	movs	r0, #11
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b4ec
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b544
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #2
	movs	r0, #11
	bl 0x0200b4fc
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #10
	movs	r2, #45
	adds	r1, #255
.L_020013b0:
	movs	r0, #11
	bl 0x0200b544
	movs	r1, #192
	lsls	r1, r1, #6
.L_020013ba:
	adds	r1, #10
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200b544
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #4
	movs	r0, #8
	bl 0x0200b4e4
	movs	r0, #5
	bl 0x0200b454
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #3
	movs	r0, #11
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #11
	bl 0x0200b4e4
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
.L_02001466:
	ldr	r0, [pc, #232]
	bl 0x0200b50c
.L_0200146c:
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #20
	bl 0x0200b454
	movs	r0, #8
	bl 0x0200b47c
	movs	r3, #0
	mov	sl, r3
	movs	r3, #160
	lsls	r3, r3, #7
	strh	r3, [r0, #6]
	movs	r1, #5
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #30
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #6
	bl 0x0200b4dc
	movs	r1, #181
	movs	r2, #252
	lsls	r2, r2, #16
	lsls	r1, r1, #18
	movs	r0, #12
	bl 0x0200b4d4
	movs	r0, #12
	bl 0x0200b47c
	ldr	r5, [pc, #156]
	str	r5, [r0, #24]
	movs	r0, #12
	bl 0x0200b47c
	str	r5, [r0, #28]
	movs	r0, #12
	bl 0x0200b47c
	movs	r1, #0
	bl 0x0200b42c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #8
	bl 0x0200b514
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_02001558
	bl 0x0200b5bc
	movs	r0, #15
	bl 0x0200b454
	movs	r2, #0
	movs	r0, #12
	movs	r1, #0
	bl 0x0200b4d4
	movs	r1, #1
	movs	r0, #8
	bl 0x0200b4dc
	movs	r0, #8
	bl 0x0200b47c
	movs	r3, #128
	lsls	r3, r3, #8
	strh	r3, [r0, #6]
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r1, #16
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #4
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r0, #5
	bl 0x0200b454
	b.n	.L_02001746
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002624
	.4byte 0x0000260f
	.4byte 0x0000261e
	.2byte 0x3333
	.2byte 0x0001
.L_02001558:
	movs	r0, #25
	bl 0x0200b454
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
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #5
	adds	r0, #8
	bl 0x0200b51c
	movs	r0, #78
	bl 0x0200b62c
	movs	r0, #164
	movs	r1, #208
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #224
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #240
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	movs	r0, #164
	movs	r1, #128
	movs	r3, #4
	lsls	r0, r0, #18
	lsls	r1, r1, #17
	movs	r2, #0
	negs	r3, r3
	bl 0x0200b43c
	ldr	r3, [pc, #364]
	movs	r5, #128
	movs	r2, #85
	lsls	r5, r5, #7
	adds	r2, r2, r7
	movs	r6, #2
	str	r5, [r7, #72]
	mov	r8, r2
	strb	r6, [r2, #0]
	movs	r1, #10
	str	r3, [r7, #20]
	movs	r2, #0
	movs	r0, #12
	bl 0x0200b4ec
	movs	r0, #146
	bl 0x0200b62c
	movs	r0, #12
	movs	r1, #1
	bl 0x0200b53c
	movs	r1, #192
	movs	r2, #192
	movs	r0, #12
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200b484
	movs	r1, #161
	lsls	r1, r1, #2
	movs	r2, #248
	movs	r0, #12
	bl 0x0200b4ac
	movs	r0, #21
	bl 0x0200b454
	movs	r1, #0
	movs	r2, #0
	movs	r0, #12
	bl 0x0200b4d4
	movs	r0, #50
	bl 0x0200b454
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200b434
	movs	r0, #70
	bl 0x0200b454
	movs	r0, #148
	bl 0x0200b62c
	movs	r0, #25
	bl 0x0200b454
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #228
	ldrh	r3, [r2, #0]
	add	r0, sp, #24
	strh	r3, [r0, #0]
	adds	r2, #2
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r0, #2]
	add	r1, sp, #12
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r0, #4]
	ldrh	r3, [r2, #0]
	strh	r3, [r0, #6]
	ldrh	r3, [r2, #2]
	ldr	r2, [pc, #208]
	strh	r3, [r0, #8]
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r1, #0]
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r1, #2]
	ldrh	r3, [r2, #0]
	adds	r2, #2
	strh	r3, [r1, #4]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #6]
	ldrh	r3, [r2, #2]
	movs	r2, #48
	strh	r3, [r1, #8]
	bl 0x02008734
	movs	r0, #20
	bl 0x0200b454
	movs	r0, #194
	bl 0x0200b62c
	ldr	r3, [pc, #164]
	mov	r0, sp
	str	r3, [r0, #0]
	mov	r3, sl
	str	r3, [r0, #4]
	movs	r3, #238
	lsls	r3, r3, #16
	str	r3, [r0, #8]
	bl 0x0200af58
	movs	r0, #147
	bl 0x0200b62c
	movs	r1, #166
	movs	r2, #248
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	mov	r2, r8
	mov	r3, sl
	strb	r6, [r2, #0]
	str	r3, [r7, #20]
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r7, #12]
	ldr	r3, [pc, #108]
	str	r5, [r7, #24]
	str	r3, [r7, #108]
	str	r5, [r7, #28]
	movs	r1, #11
	movs	r2, #0
	movs	r0, #13
	bl 0x0200b4ec
	movs	r0, #147
	bl 0x0200b62c
	movs	r0, #13
	ldr	r1, [pc, #84]
	ldr	r2, [pc, #88]
	bl 0x0200b484
	movs	r1, #146
	movs	r2, #248
	movs	r0, #13
	lsls	r1, r1, #2
	bl 0x0200b4b4
	ldr	r5, [pc, #72]
	movs	r1, #98
	adds	r0, r5, #0
	bl 0x0200b58c
	adds	r0, r5, #0
	movs	r1, #99
	bl 0x0200b584
	ldr	r3, [pc, #60]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	strb	r6, [r3, #0]
	movs	r0, #12
	movs	r1, #5
	bl 0x0200b57c
.L_02001746:
	bl 0x0200b464
.L_0200174a:
	add	sp, #36
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfc190000
	.4byte 0x050001a4
	.4byte 0x02960000
	.4byte 0x02009139
	.4byte 0x00033333
	.4byte 0x00019999
	.4byte 0x000000bc
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	ldr	r0, [pc, #220]
	bl 0x0200b50c
	ldr	r6, [pc, #220]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r6, r2
	movs	r1, #142
	movs	r2, #248
	ldr	r0, [r5, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r1, #146
	movs	r2, #224
	movs	r0, #8
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r1, #150
	movs	r2, #240
	movs	r0, #13
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r1, #146
	movs	r2, #132
	movs	r0, #11
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b4d4
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #176
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200b52c
	ldr	r0, [r5, #0]
	movs	r1, #41
	bl 0x0200b4dc
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x0200b554
	bl 0x0200b594
	bl 0x0200b5a4
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	movs	r3, #218
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #40
	str	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #93
	str	r3, [r2, #0]
	bl 0x0200b59c
	bl 0x0200b5a4
	movs	r2, #242
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	movs	r2, #243
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl 0x0200b56c
	pop	{r5, r6, pc}
	.4byte 0x00002622
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	bl 0x0200b45c
	movs	r0, #0
	bl 0x0200b5b4
	ldr	r0, [pc, #988]
	bl 0x0200b50c
	ldr	r5, [pc, #984]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #142
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl 0x0200b4d4
	movs	r1, #148
	movs	r2, #240
	lsls	r2, r2, #16
	movs	r0, #13
	lsls	r1, r1, #18
	bl 0x0200b4d4
	movs	r0, #13
	movs	r1, #3
	bl 0x0200b4dc
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #164
	movs	r1, #1
	movs	r2, #248
	movs	r3, #0
	negs	r1, r1
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	bl 0x0200b55c
	bl 0x0200b594
	bl 0x0200b5a4
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #2
	movs	r0, #13
	bl 0x0200b4fc
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #13
	movs	r1, #1
	bl 0x0200a9f8
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b484
	movs	r1, #12
	movs	r2, #18
	movs	r0, #8
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b5dc
	movs	r1, #10
	movs	r2, #14
	movs	r0, #8
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b5dc
	movs	r1, #18
	movs	r2, #12
	movs	r0, #8
	negs	r1, r1
	negs	r2, r2
	bl 0x0200b5dc
	movs	r1, #166
	movs	r2, #200
	movs	r0, #8
	lsls	r1, r1, #2
	bl 0x0200b4b4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #8
	bl 0x0200b5dc
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #16
	bl 0x0200b5dc
.L_02001978:
	movs	r1, #8
	movs	r0, #8
	negs	r1, r1
	movs	r2, #16
	bl 0x0200b5dc
	movs	r1, #152
	movs	r2, #248
	movs	r0, #8
	lsls	r1, r1, #2
	bl 0x0200b4b4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #11
	bl 0x0200b544
	movs	r2, #217
	lsls	r2, r2, #8
	movs	r0, #11
	ldr	r1, [pc, #660]
	adds	r2, #153
	bl 0x0200b484
	movs	r1, #16
	negs	r1, r1
	movs	r2, #0
	movs	r0, #11
	bl 0x0200b5dc
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b4ec
	movs	r0, #11
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b4ec
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b54c
	movs	r0, #38
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b54c
	movs	r0, #37
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #254
	lsls	r1, r1, #7
	adds	r1, #246
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #25
	bl 0x0200b454
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #2
	movs	r0, #11
	bl 0x0200b4fc
	movs	r0, #30
	bl 0x0200b454
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r0, #11
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b51c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b544
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b51c
	movs	r2, #5
	movs	r0, #15
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #254
	lsls	r1, r1, #7
	adds	r1, #246
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #16
	movs	r0, #7
	movs	r1, #0
	negs	r2, r2
	movs	r3, #0
	bl 0x0200b5cc
	movs	r1, #16
	movs	r0, #5
	negs	r1, r1
	movs	r2, #0
	movs	r3, #0
	bl 0x0200b5cc
	movs	r1, #16
	movs	r2, #16
	movs	r0, #6
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	bl 0x0200b5cc
	movs	r2, #16
	movs	r1, #16
	negs	r2, r2
	movs	r3, #0
	movs	r0, #15
	bl 0x0200b5cc
	movs	r0, #15
	bl 0x0200b4cc
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #160
	movs	r1, #1
	movs	r2, #248
	movs	r3, #1
	lsls	r2, r2, #16
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200b55c
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #8
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b54c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b54c
	movs	r0, #40
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #3
	movs	r0, #5
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #5
	adds	r0, #5
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #0
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b54c
	movs	r0, #35
	bl 0x0200b454
	movs	r2, #0
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #7
	movs	r1, #4
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #7
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #15
	bl 0x0200b514
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_02001caa
	b.n	.L_02001c64
	.4byte 0x00002627
	.4byte 0x02000240
	.2byte 0xb333
	.2byte 0x0001
.L_02001c64:
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #6
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #6
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #5
	adds	r0, #6
	movs	r1, #0
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
.L_02001ca2:
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001ce8
.L_02001caa:
	movs	r0, #35
	bl 0x0200b454
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #6
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #6
	movs	r1, #4
	bl 0x0200b4e4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #5
	strh	r3, [r2, #0]
	adds	r0, #6
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
.L_02001ce8:
	movs	r2, #0
	movs	r0, #15
	movs	r1, #0
	bl 0x0200b52c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b534
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #179
	movs	r2, #179
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200b484
	ldr	r2, [pc, #628]
	movs	r0, #11
	ldr	r1, [pc, #628]
	bl 0x0200b484
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	movs	r0, #11
	lsls	r1, r1, #1
	bl 0x0200b54c
	ldr	r5, [pc, #608]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200b48c
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #166
	movs	r2, #145
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #11
	bl 0x0200b4ac
	movs	r0, #8
	bl 0x0200b494
	ldr	r2, [pc, #576]
	movs	r0, #8
	mov	sl, r2
	mov	r1, sl
	bl 0x0200b48c
	movs	r0, #11
	bl 0x0200b4cc
	ldr	r6, [pc, #560]
	movs	r0, #11
	adds	r1, r6, #0
	bl 0x0200b48c
	movs	r0, #8
	bl 0x0200b494
	ldr	r3, [pc, #548]
	movs	r0, #8
	mov	r8, r3
	mov	r1, r8
	bl 0x0200b48c
	movs	r0, #11
	bl 0x0200b494
	adds	r1, r5, #0
	movs	r0, #11
	bl 0x0200b48c
	movs	r0, #8
	bl 0x0200b494
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b52c
	movs	r0, #11
	bl 0x0200b494
	movs	r1, #192
	movs	r0, #11
.L_02001dae:
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b52c
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	movs	r0, #11
	lsls	r1, r1, #1
	bl 0x0200b54c
	adds	r1, r6, #0
	movs	r0, #8
	bl 0x0200b48c
	movs	r0, #20
	bl 0x0200b454
	movs	r0, #11
	bl 0x0200b4cc
	mov	r1, sl
	movs	r0, #11
	bl 0x0200b48c
	movs	r0, #8
	bl 0x0200b494
	adds	r1, r5, #0
	movs	r0, #8
	bl 0x0200b48c
	movs	r0, #11
	bl 0x0200b494
	mov	r1, r8
	movs	r0, #11
	bl 0x0200b48c
	movs	r0, #8
	bl 0x0200b494
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b52c
	movs	r0, #11
	bl 0x0200b494
	movs	r1, #176
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200b52c
	movs	r0, #11
	movs	r1, #4
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #4
	movs	r1, #3
	bl 0x0200b53c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200b53c
	movs	r0, #6
	movs	r1, #3
	bl 0x0200b53c
	movs	r0, #7
	movs	r1, #3
	bl 0x0200b53c
	movs	r1, #160
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #160
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200b52c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #142
	movs	r1, #1
	movs	r2, #248
	movs	r3, #1
	lsls	r2, r2, #16
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200b55c
	bl 0x0200b564
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #2
	movs	r0, #14
	bl 0x0200b53c
	movs	r0, #14
	bl 0x0200b47c
	movs	r6, #192
	lsls	r6, r6, #6
	strh	r6, [r0, #6]
	movs	r0, #14
	bl 0x0200b47c
	ldr	r5, [pc, #60]
	adds	r0, #85
	movs	r1, #130
	movs	r2, #200
	strb	r5, [r0, #0]
	lsls	r1, r1, #18
	movs	r0, #14
	lsls	r2, r2, #16
	bl 0x0200b4d4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #14
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b484
	movs	r0, #14
	movs	r1, #0
	movs	r2, #16
	bl 0x0200b5dc
	movs	r0, #14
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	b.n	.L_02001fa4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0001b333
	.4byte 0x00036666
	.4byte 0x0200c1e8
	.4byte 0x0200c254
	.4byte 0x0200c32c
	.2byte 0xc2d4
	.2byte 0x0200
.L_02001fa4:
	bl 0x0200b51c
	movs	r0, #4
	movs	r1, #14
	bl 0x0200b5e4
	movs	r0, #7
	movs	r1, #14
	bl 0x0200b5e4
	movs	r0, #5
	movs	r1, #14
	bl 0x0200b5e4
	movs	r0, #6
	movs	r1, #14
	bl 0x0200b5e4
	movs	r0, #15
	movs	r1, #14
	bl 0x0200b5e4
	movs	r0, #8
	movs	r1, #14
	bl 0x0200b5e4
	movs	r2, #16
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b5dc
	movs	r0, #14
	movs	r1, #3
	bl 0x0200b53c
	movs	r0, #14
	movs	r1, #0
	movs	r2, #32
	bl 0x0200b5dc
	movs	r0, #14
	movs	r1, #8
	movs	r2, #8
	bl 0x0200b5dc
	movs	r0, #14
	movs	r1, #64
	movs	r2, #0
	bl 0x0200b5dc
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #150
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #17
	bl 0x0200b55c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #11
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b484
	movs	r1, #32
	movs	r0, #11
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r1, #16
	movs	r2, #16
	negs	r2, r2
	movs	r0, #11
	negs	r1, r1
	bl 0x0200b5dc
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200b534
	movs	r1, #3
	movs	r0, #14
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #11
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b4fc
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #128
	movs	r2, #128
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b484
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #8
	bl 0x0200b5dc
	movs	r1, #16
	movs	r0, #8
	negs	r1, r1
	movs	r2, #16
	bl 0x0200b5dc
	movs	r1, #8
	movs	r2, #8
	movs	r0, #8
	negs	r1, r1
	bl 0x0200b5dc
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b534
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #176
	movs	r2, #0
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x0200b52c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #15
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #14
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #4
	bl 0x0200b4e4
	movs	r1, #0
	movs	r0, #11
	bl 0x0200b514
	movs	r1, #224
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x0200b534
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_020021dc
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #2
	movs	r0, #8
	bl 0x0200b4fc
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #2
	strh	r3, [r2, #0]
	b.n	.L_02002238
.L_020021dc:
	movs	r0, #35
	bl 0x0200b454
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #8
	adds	r3, #2
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #11
	bl 0x0200b544
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #6
	movs	r2, #15
	bl 0x0200b4ec
	movs	r0, #11
	movs	r1, #6
	movs	r2, #23
	bl 0x0200b4ec
.L_02002238:
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r0, #4
	bl 0x0200b534
	movs	r0, #15
	bl 0x0200b454
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #14
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #2
	movs	r0, #14
	bl 0x0200b4fc
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #0
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #60
	movs	r0, #11
	bl 0x0200b544
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #14
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b54c
	movs	r0, #40
	bl 0x0200b454
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #11
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #3
	movs	r0, #14
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #8
	bl 0x0200b544
	movs	r2, #5
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #14
	bl 0x0200b514
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #7
	bl 0x0200b534
	movs	r0, #4
	movs	r1, #0
	bl 0x0200b46c
	cmp	r0, #0
	bne.n	.L_0200243a
	movs	r0, #15
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #15
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #15
	movs	r1, #0
	bl 0x0200b51c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200246e
.L_0200243a:
	movs	r0, #35
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #15
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
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
	adds	r0, #15
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
.L_0200246e:
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #15
	lsls	r1, r1, #6
	bl 0x0200b52c
	movs	r1, #0
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #10
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b4f4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #11
	bl 0x0200b54c
	movs	r0, #50
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #11
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #14
	bl 0x0200b544
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #11
	bl 0x0200b544
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #11
	movs	r1, #0
	movs	r2, #15
	bl 0x0200b51c
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #14
	bl 0x0200b544
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #2
	movs	r2, #35
	adds	r1, #255
	movs	r0, #11
	bl 0x0200b544
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #14
	movs	r1, #4
	bl 0x0200b4e4
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #176
	lsls	r1, r1, #8
	movs	r0, #11
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #11
	movs	r1, #0
	bl 0x0200b51c
	movs	r0, #14
	movs	r1, #3
	bl 0x0200b4e4
	movs	r0, #14
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #8
	movs	r0, #14
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #8
	movs	r0, #14
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #11
	bl 0x0200b544
	movs	r1, #56
	movs	r2, #0
	movs	r0, #14
	negs	r1, r1
	bl 0x0200b5dc
	movs	r1, #176
	movs	r0, #14
	lsls	r1, r1, #8
	bl 0x0200b534
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #14
	bl 0x0200b544
	movs	r1, #0
	movs	r0, #14
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r2, #5
	movs	r0, #14
	movs	r1, #0
	bl 0x0200b51c
	movs	r1, #2
	movs	r0, #11
	bl 0x0200b4fc
	movs	r0, #5
	bl 0x0200b454
	movs	r2, #20
	movs	r0, #11
	movs	r1, #6
	bl 0x0200b4ec
	movs	r0, #4
	movs	r1, #11
	bl 0x0200b5e4
	movs	r0, #7
	movs	r1, #11
	bl 0x0200b5e4
	movs	r0, #5
	movs	r1, #11
	bl 0x0200b5e4
	movs	r0, #6
	movs	r1, #11
	bl 0x0200b5e4
	movs	r0, #15
	movs	r1, #11
	bl 0x0200b5e4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #11
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200b484
	movs	r1, #80
	movs	r2, #0
	negs	r1, r1
	movs	r0, #11
	bl 0x0200b5dc
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #14
	movs	r1, #3
	bl 0x0200b53c
	movs	r0, #11
	movs	r1, #3
	bl 0x0200b53c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #11
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200b484
	movs	r2, #64
	movs	r0, #14
	movs	r1, #0
	negs	r2, r2
	bl 0x0200b5d4
	movs	r1, #16
	movs	r0, #11
	negs	r1, r1
	movs	r2, #0
	bl 0x0200b5dc
	movs	r2, #64
	negs	r2, r2
	movs	r1, #0
	movs	r0, #11
	bl 0x0200b5d4
	movs	r0, #15
	bl 0x0200b454
	movs	r0, #14
	movs	r1, #2
	bl 0x0200b53c
	movs	r1, #2
	movs	r0, #11
	bl 0x0200b53c
	movs	r0, #14
	bl 0x0200b4cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #14
	bl 0x0200b4d4
	movs	r0, #11
	bl 0x0200b4cc
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b4d4
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b52c
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #60
	bl 0x0200b454
	movs	r2, #16
	movs	r0, #8
	movs	r1, #0
	bl 0x0200b5dc
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x0200b534
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200b544
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x0200b544
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200b544
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200b544
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #15
	bl 0x0200b544
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b52c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200b534
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #3
	movs	r0, #8
	bl 0x0200b4e4
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200b51c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200b52c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200b534
	movs	r0, #40
	bl 0x0200b454
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200b52c
	movs	r1, #0
	movs	r0, #15
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r0, #8
	movs	r1, #4
	bl 0x0200b4e4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x0200b51c
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #153
	movs	r0, #11
	adds	r1, #51
	bl 0x0200b484
	ldr	r1, [pc, #408]
	movs	r0, #8
	bl 0x0200b48c
	movs	r0, #30
	bl 0x0200b454
	movs	r0, #8
	bl 0x0200b49c
	movs	r1, #162
	movs	r2, #188
	movs	r0, #8
	lsls	r1, r1, #2
	bl 0x0200b4b4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200b534
	movs	r0, #10
	bl 0x0200b454
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200b52c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b52c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #6
	bl 0x0200b52c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #15
	bl 0x0200b534
	movs	r0, #20
	bl 0x0200b454
	movs	r1, #2
	movs	r0, #4
	bl 0x0200b53c
	movs	r0, #4
	bl 0x0200b47c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r2, #153
	lsls	r2, r2, #8
	strb	r3, [r0, #0]
	adds	r2, #153
	movs	r0, #5
	ldr	r1, [pc, #272]
	bl 0x0200b484
	movs	r0, #5
	movs	r1, #2
	bl 0x0200b4dc
	ldr	r3, [pc, #260]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	cmp	r0, #0
	beq.n	.L_0200290c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200b4a4
.L_0200290c:
	movs	r0, #5
	bl 0x0200b4cc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b4d4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #204]
	adds	r2, #153
	bl 0x0200b484
	movs	r0, #6
	movs	r1, #2
	bl 0x0200b4dc
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	cmp	r0, #0
	beq.n	.L_0200294a
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200b4a4
.L_0200294a:
	movs	r0, #6
	bl 0x0200b4cc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b4d4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #140]
	adds	r2, #153
	bl 0x0200b484
	movs	r0, #7
	movs	r1, #2
	bl 0x0200b4dc
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	cmp	r0, #0
	beq.n	.L_02002988
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200b4a4
.L_02002988:
	movs	r0, #7
	bl 0x0200b4cc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b4d4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #15
	ldr	r1, [pc, #80]
	adds	r2, #153
	bl 0x0200b484
	movs	r0, #15
	movs	r1, #2
	bl 0x0200b4dc
	ldr	r0, [r5, #0]
	bl 0x0200b47c
	cmp	r0, #0
	beq.n	.L_020029c6
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #15
	bl 0x0200b4a4
.L_020029c6:
	movs	r0, #15
	bl 0x0200b4cc
	movs	r1, #0
	movs	r2, #0
	movs	r0, #15
	bl 0x0200b4d4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #126
	bl 0x0200b40c
	bl 0x0200b464
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.4byte 0x0200c1e8
	.4byte 0x00013333
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	mov	r9, r1
	movs	r1, #133
	str	r0, [sp, #4]
	lsls	r1, r1, #3
	movs	r0, #220
	bl 0x0200b3cc
	mov	sl, r0
	ldr	r0, [sp, #4]
	bl 0x0200b47c
	mov	r1, r9
	adds	r7, r0, #0
	cmp	r1, #0
	bne.n	.L_02002a3e
	movs	r5, #15
.L_02002a28:
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	movs	r0, #1
	subs	r5, #1
	bl 0x0200b3ac
	cmp	r5, #0
	bge.n	.L_02002a28
.L_02002a3e:
	movs	r0, #2
	bl 0x0200b3ac
	movs	r3, #0
	str	r3, [r7, #24]
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200b42c
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #444]
	ldr	r1, [pc, #444]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #440]
	bl 0x0200b3fc
	mov	r1, sl
	bl 0x0200b3dc
	bl 0x0200b3f4
	movs	r1, #128
	lsls	r1, r1, #3
	mov	r2, sl
	mov	fp, r0
	bl 0x0200b3ec
	movs	r6, #128
	lsls	r6, r6, #3
	add	r6, sl
	movs	r3, #128
	str	r0, [sp, #0]
	movs	r1, #8
	movs	r2, #16
	lsls	r3, r3, #23
	mov	r8, r0
	adds	r0, r6, #0
	bl 0x0200b5fc
	movs	r3, #240
	strh	r3, [r6, #30]
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ldrb	r1, [r6, #5]
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r6, #5]
	movs	r3, #240
	orrs	r2, r3
	strb	r2, [r6, #9]
	ldr	r3, [r7, #8]
	add	r5, sp, #8
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r0, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl 0x0200b5c4
	ldr	r3, [r5, #0]
	str	r3, [r6, #12]
	mov	r3, r9
	ldr	r2, [r5, #8]
	str	r2, [r6, #16]
	cmp	r3, #1
	bne.n	.L_02002aea
	movs	r3, #128
	movs	r1, #160
	lsls	r3, r3, #10
	lsls	r1, r1, #12
	str	r3, [r6, #20]
	str	r3, [r6, #24]
	adds	r3, r2, r1
	str	r3, [r6, #16]
.L_02002aea:
	movs	r0, #145
	bl 0x0200b62c
	movs	r5, #0
.L_02002af2:
	movs	r6, #128
	lsls	r6, r6, #3
	subs	r3, r5, #6
	add	r6, sl
	cmp	r3, #56
	bls.n	.L_02002b00
	b.n	.L_02002c50
.L_02002b00:
	ldr	r2, [pc, #288]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200abec
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200abf4
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200abfc
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac04
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac0c
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac28
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac50
	.4byte 0x0200ac34
	.4byte 0x46424b09
	.4byte 0xe0223204
	.4byte 0x46424b07
	.4byte 0xe01e3208
	.4byte 0x46424b05
	.4byte 0xe01a320c
	.4byte 0x46424b03
	.4byte 0xe0163210
	.4byte 0x46424b01
	.4byte 0xe0123214
	.4byte 0x000003ff
	.4byte 0x0200b640
	.4byte 0x050003e0
	.4byte 0x000001e8
	.4byte 0x0200ab08
	.4byte 0x46424b01
	.4byte 0xe0043218
	.4byte 0x000003ff
	.4byte 0x46424b04
	.4byte 0x401a321c
	.4byte 0x4b038931
	.4byte 0x4313400b
	.4byte 0xe0038133
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_02002c50:
	adds	r0, r6, #0
	bl 0x0200b604
	adds	r5, #1
	movs	r0, #1
	bl 0x0200b3ac
	cmp	r5, #71
	bgt.n	.L_02002c64
	b.n	.L_02002af2
.L_02002c64:
	ldr	r0, [sp, #4]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b4d4
	mov	r0, fp
	bl 0x0200b3e4
	movs	r0, #220
	bl 0x0200b3d4
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	ldr	r0, [pc, #156]
	sub	sp, #4
	bl 0x0200b3fc
	adds	r1, r7, #0
	bl 0x0200b3dc
	movs	r0, #0
	adds	r2, r7, #0
.L_02002ca4:
	ldrb	r1, [r2, #0]
	movs	r3, #240
	ands	r3, r1
	cmp	r3, #240
	bne.n	.L_02002cb6
	movs	r3, #15
	ands	r3, r1
	adds	r3, #96
	strb	r3, [r2, #0]
.L_02002cb6:
	movs	r3, #128
	adds	r0, #1
	lsls	r3, r3, #5
	adds	r2, #1
	cmp	r0, r3
	blt.n	.L_02002ca4
	bl 0x0200b3f4
	movs	r1, #128
	adds	r2, r7, #0
	lsls	r1, r1, #5
	adds	r5, r0, #0
	bl 0x0200b3ec
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #92
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #12
	adds	r5, r7, r3
	movs	r3, #192
	str	r0, [sp, #0]
	movs	r1, #31
	adds	r0, r5, #0
	movs	r2, #31
	lsls	r3, r3, #24
	bl 0x0200b5fc
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #5
	strb	r3, [r5, #9]
	adds	r2, #100
	movs	r3, #1
	strh	r3, [r5, #30]
	movs	r6, #0
	adds	r3, r7, r2
	adds	r2, #4
	str	r6, [r3, #0]
	adds	r3, r7, r2
	str	r6, [r3, #0]
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x01d9
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	ldr	r0, [pc, #156]
	sub	sp, #4
	bl 0x0200b3fc
	adds	r1, r7, #0
	bl 0x0200b3dc
	movs	r0, #0
	adds	r2, r7, #0
.L_02002d50:
	ldrb	r1, [r2, #0]
	movs	r3, #240
	ands	r3, r1
	cmp	r3, #240
	bne.n	.L_02002d62
	movs	r3, #15
	ands	r3, r1
	adds	r3, #96
	strb	r3, [r2, #0]
.L_02002d62:
	movs	r3, #128
	adds	r0, #1
	lsls	r3, r3, #5
	adds	r2, #1
	cmp	r0, r3
	blt.n	.L_02002d50
	bl 0x0200b3f4
	movs	r1, #128
	adds	r2, r7, #0
	lsls	r1, r1, #4
	adds	r5, r0, #0
	bl 0x0200b3ec
	movs	r2, #131
	lsls	r2, r2, #5
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r0, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #52
	adds	r5, r7, r3
	str	r0, [sp, #0]
	movs	r1, #15
	adds	r0, r5, #0
	movs	r2, #47
	ldr	r3, [pc, #64]
	bl 0x0200b5fc
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #16
	orrs	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #5
	strb	r3, [r5, #9]
	adds	r2, #108
	movs	r3, #1
	strh	r3, [r5, #30]
	movs	r6, #0
	adds	r3, r7, r2
	adds	r2, #4
	str	r6, [r3, #0]
	adds	r3, r7, r2
	str	r6, [r3, #0]
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x000001f7
	.2byte 0x8000
	.2byte 0xc000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #16
	mov	fp, r3
	movs	r7, #0
.L_02002e00:
	movs	r6, #128
	lsls	r6, r6, #5
	adds	r6, #116
	movs	r0, #31
	add	r6, fp
	mov	r8, r0
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	adds	r0, r0, r7
	lsls	r0, r0, #11
	bl 0x0200b3c4
	ldrh	r3, [r6, #0]
	lsls	r5, r0, #1
	lsls	r3, r3, #16
	adds	r5, r5, r0
	asrs	r0, r3, #16
	lsrs	r3, r3, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	adds	r0, r0, r7
	movs	r3, #144
	lsls	r3, r3, #7
	lsls	r0, r0, #11
	adds	r0, r0, r3
	bl 0x0200b3c4
	lsls	r3, r0, #1
	lsls	r5, r5, #1
	adds	r3, r3, r0
	asrs	r5, r5, #16
	asrs	r3, r3, #16
	adds	r5, #20
	adds	r3, #3
	lsls	r2, r3, #10
	lsls	r5, r5, #5
	adds	r1, r7, #1
	cmp	r7, #15
	bgt.n	.L_02002e5a
	mov	r0, r8
	orrs	r2, r5
	orrs	r2, r0
	movs	r3, #111
	ldr	r0, [pc, #252]
	b.n	.L_02002e64
.L_02002e5a:
	mov	r3, r8
	orrs	r2, r5
	orrs	r2, r3
	ldr	r0, [pc, #240]
	movs	r3, #255
.L_02002e64:
	subs	r3, r3, r7
	lsls	r3, r3, #1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
	adds	r7, r1, #0
	cmp	r7, #31
	ble.n	.L_02002e00
	movs	r2, #128
	lsls	r2, r2, #5
	add	r2, fp
	ldr	r3, [r2, #0]
	add	r5, sp, #4
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #4
	add	r3, fp
	mov	r8, r3
	ldr	r3, [r3, #0]
	movs	r6, #128
	str	r3, [r5, #4]
	lsls	r6, r6, #5
	adds	r6, #8
	add	r6, fp
	ldr	r3, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	mov	sl, r2
	bl 0x0200b5c4
	movs	r7, #128
	ldr	r3, [r5, #0]
	lsls	r7, r7, #5
	adds	r7, #12
	add	r7, fp
	str	r3, [r7, #12]
	movs	r0, #128
	ldr	r3, [r5, #8]
	lsls	r0, r0, #5
	str	r3, [r7, #16]
	adds	r0, #100
	add	r0, fp
	movs	r2, #128
	ldr	r3, [r0, #0]
	lsls	r2, r2, #5
	adds	r2, #104
	add	r2, fp
	str	r3, [r7, #20]
	str	r2, [sp, #0]
	mov	r9, r0
	ldr	r3, [r2, #0]
	adds	r0, r7, #0
	str	r3, [r7, #24]
	bl 0x0200b604
	mov	r0, sl
	ldr	r3, [r0, #0]
	mov	r2, r8
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r2, #0]
	movs	r7, #128
	str	r3, [r5, #4]
	lsls	r7, r7, #5
	ldr	r3, [r6, #0]
	adds	r7, #52
	str	r3, [r5, #8]
	bl 0x0200b5c4
	ldr	r3, [r5, #0]
	add	r7, fp
	str	r3, [r7, #12]
	movs	r6, #128
	ldr	r3, [r5, #8]
	movs	r5, #128
	str	r3, [r7, #16]
	lsls	r5, r5, #5
	adds	r5, #108
	add	r5, fp
	ldr	r3, [r5, #0]
	lsls	r6, r6, #5
	str	r3, [r7, #20]
	adds	r6, #112
	add	r6, fp
	ldr	r3, [r6, #0]
	adds	r0, r7, #0
	str	r3, [r7, #24]
	bl 0x0200b604
	mov	r0, r9
	ldr	r3, [r0, #0]
	movs	r1, #40
	lsls	r0, r3, #5
	subs	r0, r0, r3
	bl 0x0200b3a4
	ldr	r2, [sp, #0]
	movs	r1, #40
	str	r0, [r2, #0]
	ldr	r3, [r5, #0]
	lsls	r0, r3, #5
	subs	r0, r0, r3
	bl 0x0200b3a4
	movs	r2, #128
	str	r0, [r6, #0]
	lsls	r2, r2, #5
	adds	r2, #116
	add	r2, fp
	ldrh	r3, [r2, #0]
	add	sp, #16
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0200
	.2byte 0x0500
	push	{r5, r6, r7, lr}
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r5, r0, #0
	adds	r1, #120
	movs	r0, #220
	bl 0x0200b3cc
	ldr	r3, [r5, #0]
	movs	r1, #128
	adds	r6, r0, #0
	lsls	r1, r1, #5
	adds	r2, r6, r1
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r2, r6, r3
	ldr	r3, [r5, #4]
	adds	r1, #8
	str	r3, [r2, #0]
	adds	r2, r6, r1
	ldr	r3, [r5, #8]
	movs	r7, #0
	str	r3, [r2, #0]
	bl 0x0200ac88
	bl 0x0200ad34
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #116
	adds	r2, r6, r3
	movs	r1, #144
	movs	r3, #0
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #208]
	bl 0x0200b3b4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200b434
	movs	r5, #0
.L_02002fba:
	cmp	r5, #1
	beq.n	.L_02002ffe
	cmp	r5, #1
	bgt.n	.L_02002fc8
	cmp	r5, #0
	beq.n	.L_02002fd2
	b.n	.L_02003060
.L_02002fc8:
	cmp	r5, #2
	beq.n	.L_0200300a
	cmp	r5, #3
	beq.n	.L_02003036
	b.n	.L_02003060
.L_02002fd2:
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #100
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r3, r3, r1
	movs	r1, #166
	lsls	r1, r1, #9
	adds	r1, #203
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_02003060
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	movs	r7, #1
	str	r3, [r2, #0]
	movs	r5, #1
	negs	r7, r7
	b.n	.L_02003060
.L_02002ffe:
	cmp	r7, #10
	bne.n	.L_02003060
	movs	r7, #1
	movs	r5, #2
	negs	r7, r7
	b.n	.L_02003060
.L_0200300a:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #108
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r3, r3, r1
	movs	r1, #204
	lsls	r1, r1, #7
	adds	r1, #101
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_02003060
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	movs	r7, #1
	str	r3, [r2, #0]
	movs	r5, #3
	negs	r7, r7
	b.n	.L_02003060
.L_02003036:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #108
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r3, r3, r1
	movs	r1, #166
	lsls	r1, r1, #9
	adds	r1, #203
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_02003060
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	movs	r5, #186
	str	r3, [r2, #0]
	lsls	r5, r5, #2
	adds	r5, #255
.L_02003060:
	movs	r0, #1
	bl 0x0200b3ac
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r7, #1
	cmp	r5, r3
	bne.n	.L_02002fba
	pop	{r5, r6, r7, pc}
	.2byte 0xade5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	sub	sp, #12
	mov	r2, sp
	str	r3, [r2, #0]
	adds	r1, #4
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #188]
	movs	r5, #0
	adds	r3, r3, r1
	movs	r1, #128
	str	r3, [r2, #4]
	lsls	r1, r1, #5
	adds	r1, #8
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	movs	r7, #0
	str	r3, [r2, #8]
.L_020030ac:
	cmp	r5, #1
	beq.n	.L_020030de
	cmp	r5, #1
	bgt.n	.L_020030ba
	cmp	r5, #0
	beq.n	.L_020030c0
	b.n	.L_02003108
.L_020030ba:
	cmp	r5, #2
	beq.n	.L_020030fe
	b.n	.L_02003108
.L_020030c0:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #108
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #140]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_02003108
	movs	r7, #1
	str	r5, [r2, #0]
	negs	r7, r7
	movs	r5, #1
	b.n	.L_02003108
.L_020030de:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #100
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #112]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_02003108
	movs	r3, #0
	movs	r7, #1
	str	r3, [r2, #0]
	movs	r5, #2
	negs	r7, r7
	b.n	.L_02003108
.L_020030fe:
	cmp	r7, #1
	bne.n	.L_02003108
	movs	r5, #186
	lsls	r5, r5, #2
	adds	r5, #255
.L_02003108:
	movs	r0, #1
	bl 0x0200b3ac
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r7, #1
	cmp	r5, r2
	bne.n	.L_020030ac
	ldr	r0, [pc, #68]
	bl 0x0200b3bc
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200b434
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #92
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200b3e4
	movs	r1, #131
	lsls	r1, r1, #5
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200b3e4
	movs	r0, #220
	bl 0x0200b3d4
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xffe00000
	.4byte 0xfffff000
	.4byte 0xfffff800
	.2byte 0xade5
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r5, r0, #0
	adds	r1, #120
	movs	r0, #220
	bl 0x0200b3cc
	ldr	r3, [r5, #0]
	movs	r1, #128
	adds	r6, r0, #0
	lsls	r1, r1, #5
	adds	r2, r6, r1
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r2, r6, r3
	ldr	r3, [r5, #4]
	adds	r1, #8
	str	r3, [r2, #0]
	adds	r2, r6, r1
	ldr	r3, [r5, #8]
	movs	r5, #0
	str	r3, [r2, #0]
	bl 0x0200ac88
	movs	r2, #128
	lsls	r2, r2, #5
	movs	r1, #128
	adds	r2, #108
	lsls	r1, r1, #5
	adds	r3, r6, r2
	adds	r1, #112
	movs	r2, #0
	str	r2, [r3, #0]
	adds	r3, r6, r1
	adds	r1, #4
	str	r2, [r3, #0]
	adds	r3, r6, r1
	movs	r1, #144
	strh	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #76]
	bl 0x0200b3b4
	movs	r0, #139
	bl 0x0200b62c
.L_020031c6:
	cmp	r5, #0
	bne.n	.L_020031f4
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #100
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	movs	r1, #166
	lsls	r1, r1, #9
	adds	r1, #203
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_020031f4
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	movs	r5, #186
	str	r3, [r2, #0]
	lsls	r5, r5, #2
	adds	r5, #255
.L_020031f4:
	movs	r0, #1
	bl 0x0200b3ac
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r5, r2
	bne.n	.L_020031c6
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xade5
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #120
	movs	r0, #220
	bl 0x0200b3cc
	adds	r6, r0, #0
	bl 0x0200ad34
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #120]
	bl 0x0200b3b4
	movs	r0, #144
	bl 0x0200b62c
	movs	r5, #0
.L_02003232:
	cmp	r5, #0
	beq.n	.L_0200323c
	cmp	r5, #1
	beq.n	.L_02003262
	b.n	.L_0200328c
.L_0200323c:
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #108
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	subs	r1, #108
	adds	r3, r3, r1
	movs	r1, #204
	lsls	r1, r1, #7
	adds	r1, #101
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_0200328c
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r2, #0]
	movs	r5, #1
	b.n	.L_0200328c
.L_02003262:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #108
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r3, r3, r1
	movs	r1, #166
	lsls	r1, r1, #9
	adds	r1, #203
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_0200328c
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	movs	r5, #186
	str	r3, [r2, #0]
	lsls	r5, r5, #2
	adds	r5, #255
.L_0200328c:
	movs	r0, #1
	bl 0x0200b3ac
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r5, r3
	bne.n	.L_02003232
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xade5
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r5, #0
	movs	r7, #0
.L_020032b2:
	cmp	r5, #1
	beq.n	.L_020032ee
	cmp	r5, #1
	bgt.n	.L_020032c0
	cmp	r5, #0
	beq.n	.L_020032ca
	b.n	.L_02003358
.L_020032c0:
	cmp	r5, #2
	beq.n	.L_02003316
	cmp	r5, #3
	beq.n	.L_0200334e
	b.n	.L_02003358
.L_020032ca:
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #108
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	subs	r1, #108
	adds	r3, r3, r1
	ldr	r1, [pc, #188]
	str	r3, [r2, #0]
	cmp	r3, r1
	ble.n	.L_02003358
	movs	r3, #128
	lsls	r3, r3, #10
	movs	r7, #1
	str	r3, [r2, #0]
	movs	r5, #1
	negs	r7, r7
	b.n	.L_02003358
.L_020032ee:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #108
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #160]
	adds	r3, r3, r1
	movs	r1, #166
	lsls	r1, r1, #9
	adds	r1, #203
	str	r3, [r2, #0]
	cmp	r3, r1
	bgt.n	.L_02003316
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r2, #0]
	movs	r7, #1
	movs	r5, #2
	negs	r7, r7
.L_02003316:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #108
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #120]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_0200332e
	movs	r3, #0
	str	r3, [r2, #0]
.L_0200332e:
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #100
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #96]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_02003358
	movs	r3, #0
	movs	r7, #1
	str	r3, [r2, #0]
	adds	r5, #1
	negs	r7, r7
	b.n	.L_02003358
.L_0200334e:
	cmp	r7, #1
	bne.n	.L_02003358
	movs	r5, #186
	lsls	r5, r5, #2
	adds	r5, #255
.L_02003358:
	movs	r0, #1
	bl 0x0200b3ac
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r7, #1
	cmp	r5, r2
	bne.n	.L_020032b2
	ldr	r0, [pc, #52]
	bl 0x0200b3bc
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #92
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200b3e4
	movs	r1, #131
	lsls	r1, r1, #5
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200b3e4
	movs	r0, #220
	bl 0x0200b3d4
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0001ffff
	.4byte 0xfffff000
	.4byte 0x0200ade5
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x08000119, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x08020171, 0x080201e9, 0x080201f1, 0x08020219, 0x08020229, 0x08020361, 0x080ad039, 0x080ad041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8079, 0x080c8089, 0x080c8099, 0x080c80a1, 0x080c80a9, 0x080c80b1, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8269, 0x080c8279, 0x080c8281, 0x080c8291, 0x080c8299, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8481, 0x080c84e1, 0x080c8571, 0x080c85c1, 0x080c85e9, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8681, 0x080c8779, 0x080c87c1, 0x080c87c9, 0x080c87f9, 0x080c8801, 0x080c8861, 0x080c88f1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x000001b7
	.4byte 0x000001b8
	.4byte 0x000001b9
	.4byte 0x575a0260
	.4byte 0x46754ad7
	.4byte 0x31cf3a32
	.4byte 0x28ea294c
	.4byte 0x00750009
	.4byte 0x01bf011f
	.4byte 0x031f027f
	.4byte 0x7fff03ff
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x40000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b9
	.4byte 0x101080b6
	.4byte 0xffffffff
	.4byte 0x1020c0ba
	.4byte 0xffffffff
	.4byte 0x103040b9
	.4byte 0xffffffff
	.4byte 0x104030b9
	.4byte 0xffffffff
	.4byte 0x105060b9
	.4byte 0xffffffff
	.4byte 0x106050b9
	.4byte 0xffffffff
	.4byte 0x000000ba
	.4byte 0x1070a0b6
	.4byte 0xffffffff
	.4byte 0x108090ba
	.4byte 0xffffffff
	.4byte 0x109080ba
	.4byte 0xffffffff
	.4byte 0x10a0b0ba
	.4byte 0xffffffff
	.4byte 0x10b0a0ba
	.4byte 0xffffffff
	.4byte 0x10c020b9
	.4byte 0xffffffff
	.4byte 0x10d0e0bb
	.4byte 0xffffffff
	.4byte 0x000000bb
	.4byte 0x10e0d0ba
	.4byte 0xffffffff
	.4byte 0x10f100bb
	.4byte 0xffffffff
	.4byte 0x1100f0bb
	.4byte 0xffffffff
	.4byte 0x111120bb
	.4byte 0xffffffff
	.4byte 0x112110bb
	.4byte 0xffffffff
	.4byte 0x1130b0b6
	.4byte 0xffffffff
	.4byte 0x114150bc
	.4byte 0xffffffff
	.4byte 0x000000bc
	.4byte 0x115140bb
	.4byte 0xffffffff
	.4byte 0x116170bc
	.4byte 0xffffffff
	.4byte 0x117160bc
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00013000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0000d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00003000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00003000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00013000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00038000
	.4byte 0xffff0027
	.4byte 0x00000008
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00033000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0003d000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0003b000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00038000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0003d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00035000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00033000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00028000
	.4byte 0xffff0073
	.4byte 0x00000008
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0001d000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00028000
	.4byte 0xffff0152
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00028000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00028000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00018000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
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
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00033000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00003000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00005000
	.4byte 0xffff0027
	.4byte 0x00000002
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00003000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00018000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00030000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00025000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00035000
	.4byte 0xffff01e9
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
	.4byte 0x00400001
	.4byte 0x00020001
	.4byte 0x00020006
	.4byte 0x00010040
	.4byte 0x00060002
	.4byte 0x0001ffff
	.4byte 0x00010040
	.4byte 0x00060002
	.4byte 0x00400000
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x0200bce4
	.4byte 0x0058000b
	.4byte 0x0200bcfa
	.4byte 0x0058000b
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
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
	.4byte 0x00000000
	.4byte 0x197e0008
	.4byte 0x0000268b
	.4byte 0x00000000
	.4byte 0x197e0009
	.4byte 0x0000268d
	.4byte 0x00000000
	.4byte 0x197e000a
	.4byte 0x00002694
	.4byte 0x00000000
	.4byte 0x197e000b
	.4byte 0x00002695
	.4byte 0x00000000
	.4byte 0x197e000c
	.4byte 0x00002696
	.4byte 0x00000000
	.4byte 0x197e000d
	.4byte 0x00002697
	.4byte 0x00008d15
	.4byte 0x197e0008
	.4byte 0x0000269b
	.4byte 0x00008d15
	.4byte 0x197e0009
	.4byte 0x0000269d
	.4byte 0x00008d15
	.4byte 0x197e000a
	.4byte 0x000026a4
	.4byte 0x00008d15
	.4byte 0x197e000b
	.4byte 0x000026a5
	.4byte 0x00008d15
	.4byte 0x197e000c
	.4byte 0x000026a6
	.4byte 0x00008d15
	.4byte 0x197e000d
	.4byte 0x000026a7
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000025f9
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte 0x02008351
	.4byte 0x00000000
	.4byte 0x197f000a
	.4byte 0x02008305
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x00002605
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x00002606
	.4byte 0x00008d15
	.4byte 0x197f000a
	.4byte 0x0000260c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002422
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002424
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000242b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000242d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000031
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x197e0008
	.4byte 0x0000268a
	.4byte 0x00000000
	.4byte 0x197e0009
	.4byte 0x0000268e
	.4byte 0x00000000
	.4byte 0x197e000a
	.4byte 0x0000268f
	.4byte 0x00000000
	.4byte 0x197e000b
	.4byte 0x00002690
	.4byte 0x00008d15
	.4byte 0x197e0008
	.4byte 0x0000269a
	.4byte 0x00008d15
	.4byte 0x197e0009
	.4byte 0x0000269e
	.4byte 0x00008d15
	.4byte 0x197e000a
	.4byte 0x0000269f
	.4byte 0x00008d15
	.4byte 0x197e000b
	.4byte 0x000026a0
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000025fb
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte 0x000025fd
	.4byte 0x00000000
	.4byte 0x197f000a
	.4byte 0x00002603
	.4byte 0x00000000
	.4byte 0x197f000b
	.4byte 0x00002604
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x00002607
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x00002609
	.4byte 0x00008d15
	.4byte 0x197f000a
	.4byte 0x0000260d
	.4byte 0x00008d15
	.4byte 0x197f000b
	.4byte 0x0000260e
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002421
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002425
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002426
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002427
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000242a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000242e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000242f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002430
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0x197e000f
	.4byte 0x020081f5
	.4byte 0x00000031
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte 0x02008209
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x197e0008
	.4byte 0x00002691
	.4byte 0x0000c400
	.4byte 0x197e000b
	.4byte 0x00002692
	.4byte 0x00000000
	.4byte 0x197e000a
	.4byte 0x00002693
	.4byte 0x00008d15
	.4byte 0x197e0008
	.4byte 0x000026a1
	.4byte 0x00008d15
	.4byte 0x197e000b
	.4byte 0x000026a2
	.4byte 0x00008d15
	.4byte 0x197e000a
	.4byte 0x000026a3
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000025fc
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte 0x000025fe
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x00002608
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x0000260a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000241c
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002428
	.4byte 0x00000003
	.4byte 0x097e0028
	.4byte 0x02008209
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000002
	.4byte 0x197f001e
	.4byte 0x0200915d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008f49
	.4byte 0x00008d15
	.4byte 0x09750008
	.4byte 0x0000266d
	.4byte 0x00008d15
	.4byte 0x09760008
	.4byte 0x00002677
	.4byte 0x00008d15
	.4byte 0x09770008
	.4byte 0x0000267c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002698
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008465
	.4byte 0x00000000
	.4byte 0x197e0009
	.4byte 0x00002689
	.4byte 0x00008d15
	.4byte 0x197e0009
	.4byte 0x00002699
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte 0x0200838d
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x0000260b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002420
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002429
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020083bd
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020083ed
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02b60000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02c60000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02c60000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02ba0000
	.4byte 0x00000000
	.4byte 0x01160000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01220000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
