.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008b19, 0x02008039, 0x02008045, 0x0200804d, 0x02008a61, 0x02008041, 0x02008c05
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xabcc
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xabfc
	.2byte 0x0200
	push	{lr}
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000060
	ldr	r0, [pc, #4]
	b.n	.L_02000062
.L_02000060:
	ldr	r0, [pc, #4]
.L_02000062:
	pop	{pc}
	.4byte 0x0200aefc
	.4byte 0x0200ac5c
	.4byte 0x4a036983
	.4byte 0x6183189b
	.4byte 0x189b69c3
	.4byte 0x477061c3
	.2byte 0xfa00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	adds	r6, r2, #0
	bl 0x0200aa44
	movs	r0, #0
	bl 0x0200ab64
	movs	r0, #158
	bl 0x0200abc4
	ldr	r5, [pc, #96]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200aa64
	movs	r3, #2
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #128
	movs	r2, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200aa6c
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200aaac
	ldr	r0, [r5, #0]
	cmp	r6, #0
	bne.n	.L_020000d2
	movs	r2, #8
	movs	r1, #2
	negs	r2, r2
	bl 0x0200aa84
	b.n	.L_020000dc
.L_020000d2:
	movs	r2, #8
	movs	r1, #0
	negs	r2, r2
	bl 0x0200aa8c
.L_020000dc:
	movs	r0, #10
	bl 0x0200aa3c
	adds	r0, r7, #0
	bl 0x0200ab44
	bl 0x0200ab4c
	bl 0x0200ab54
	bl 0x0200aa4c
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	subs	r3, r5, #1
	ldr	r6, [pc, #60]
	cmp	r3, #1
	bhi.n	.L_0200011a
	ldr	r3, [pc, #56]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200aa64
	ldr	r3, [pc, #48]
	str	r3, [r0, #108]
.L_0200011a:
	cmp	r5, #7
	bne.n	.L_02000130
	movs	r0, #8
	bl 0x0200a9f4
	adds	r0, r6, #0
	movs	r1, #7
	movs	r2, #1
	bl 0x02008080
	b.n	.L_0200013c
.L_02000130:
	adds	r0, r6, #0
	adds	r0, #8
	adds	r1, r5, #0
	movs	r2, #0
	bl 0x02008080
.L_0200013c:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x0200b1fc
	.4byte 0x02000240
	.2byte 0x806d
	.2byte 0x0200
	push	{lr}
	movs	r1, #196
	movs	r2, #142
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	movs	r0, #64
	bl 0x0200ab5c
	movs	r0, #64
	bl 0x0200aa64
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #64
	bl 0x0200aa64
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r0, #12]
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #64
	movs	r1, #0
	movs	r2, #0
	bl 0x0200ab5c
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x0200a9cc
	bl 0x0200aa44
	movs	r0, #0
	bl 0x0200ab64
	ldr	r0, [pc, #648]
	bl 0x0200aadc
	movs	r1, #192
	movs	r0, #32
	lsls	r1, r1, #6
	movs	r2, #0
.L_020001ac:
	bl 0x0200aafc
	movs	r0, #32
	movs	r1, #6
	movs	r2, #20
	bl 0x0200aabc
.L_020001ba:
	movs	r0, #32
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r0, #176
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #17
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200ab34
	bl 0x0200ab3c
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #32
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #32
	bl 0x0200ab24
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #0
	movs	r0, #33
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	movs	r2, #35
	adds	r1, #255
	movs	r0, #33
	bl 0x0200ab1c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #33
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #33
	bl 0x0200ab1c
	movs	r0, #33
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r0, #32
	movs	r1, #6
	movs	r2, #0
	bl 0x0200aabc
	movs	r0, #33
	movs	r1, #6
	movs	r2, #0
	bl 0x0200aabc
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #32
	adds	r1, #204
	adds	r2, #102
	bl 0x0200aa6c
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	adds	r1, #204
	adds	r2, #102
	movs	r0, #33
	bl 0x0200aa6c
	movs	r0, #32
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #16
	strb	r3, [r0, #0]
	movs	r1, #0
	negs	r2, r2
	movs	r0, #32
	bl 0x0200ab7c
	movs	r0, #33
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #16
	ands	r5, r3
	movs	r1, #0
	negs	r2, r2
	strb	r5, [r0, #0]
	movs	r0, #33
	bl 0x0200ab84
	movs	r0, #1
	bl 0x0200aa3c
	movs	r0, #33
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200aa3c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #32
	bl 0x0200ab1c
	movs	r0, #32
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #176
	movs	r2, #0
	movs	r0, #32
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #33
	bl 0x0200ab04
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #33
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r2, #5
	movs	r0, #34
	movs	r1, #0
	bl 0x0200aaec
	movs	r0, #32
	movs	r1, #34
	bl 0x0200ab8c
	movs	r0, #33
	movs	r1, #34
	bl 0x0200ab8c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #34
	ldr	r1, [pc, #260]
	adds	r2, #153
	bl 0x0200aa6c
	movs	r2, #32
	movs	r0, #34
	movs	r1, #0
	bl 0x0200ab84
	movs	r1, #192
	movs	r0, #34
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r0, #208
	movs	r1, #1
	movs	r2, #148
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #16
	bl 0x0200ab34
	bl 0x0200ab3c
	movs	r0, #15
	bl 0x0200aa3c
	movs	r0, #34
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aabc
	movs	r0, #34
	movs	r1, #6
	movs	r2, #23
	bl 0x0200aabc
	movs	r0, #34
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r0, #34
	movs	r1, #6
	movs	r2, #20
	bl 0x0200aabc
	movs	r1, #128
	movs	r2, #128
	movs	r0, #34
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200aa6c
	movs	r2, #48
	movs	r0, #34
	movs	r1, #0
	negs	r2, r2
	bl 0x0200ab84
	movs	r1, #176
	movs	r0, #32
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #208
	movs	r0, #33
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #32
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200aa6c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #33
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200aa6c
	movs	r1, #16
	movs	r2, #16
	movs	r0, #32
	negs	r1, r1
	negs	r2, r2
	bl 0x0200ab84
	movs	r2, #16
	movs	r0, #33
	movs	r1, #16
	negs	r2, r2
	bl 0x0200ab7c
	movs	r2, #16
	movs	r0, #32
	movs	r1, #0
	negs	r2, r2
	bl 0x0200ab84
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r2, #16
	movs	r0, #33
	movs	r1, #0
	negs	r2, r2
	bl 0x0200ab84
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	bl 0x0200aa4c
	pop	{r5, pc}
	.4byte 0x000025f3
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl 0x0200aa64
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r6, r0, #0
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	ldr	r0, [pc, #32]
	bl 0x0200aadc
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200aaf4
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200ab04
	adds	r0, r6, #0
	movs	r1, #8
	bl 0x0200ab9c
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x23f4
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200aadc
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200aae4
	bl 0x0200abb4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_02000498
	movs	r0, #10
	bl 0x0200aa3c
	adds	r0, r5, #1
	bl 0x0200aadc
	b.n	.L_020004a4
.L_02000498:
	movs	r0, #20
	bl 0x0200aa3c
	adds	r0, r5, #2
	bl 0x0200aadc
.L_020004a4:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aaf4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x25c5
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000508
	ldr	r5, [pc, #132]
	adds	r0, r5, #0
	bl 0x0200aadc
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200aae4
	bl 0x0200abb4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_020004f2
	movs	r0, #10
	bl 0x0200aa3c
	adds	r0, r5, #1
	bl 0x0200aadc
	b.n	.L_020004fe
.L_020004f2:
	movs	r0, #20
	bl 0x0200aa3c
	adds	r0, r5, #2
	bl 0x0200aadc
.L_020004fe:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aaf4
	b.n	.L_02000548
.L_02000508:
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x0200aadc
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200aae4
	bl 0x0200abb4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_02000534
	movs	r0, #10
	bl 0x0200aa3c
	adds	r0, r5, #1
	bl 0x0200aadc
	b.n	.L_02000540
.L_02000534:
	movs	r0, #20
	bl 0x0200aa3c
	adds	r0, r5, #2
	bl 0x0200aadc
.L_02000540:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aaf4
.L_02000548:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x000025c8
	.2byte 0x23f0
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #114
	bl 0x0200a9c4
	cmp	r0, #0
	bne.n	.L_02000578
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #114
	bl 0x0200a9cc
	ldr	r0, [pc, #20]
	bl 0x0200aadc
	b.n	.L_0200057e
.L_02000578:
	ldr	r0, [pc, #16]
	bl 0x0200aadc
.L_0200057e:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200aaf4
	pop	{r5, pc}
	.4byte 0x000025cf
	.2byte 0x25d0
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r5, [pc, #68]
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200aadc
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200aae4
	bl 0x0200abb4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_020005c0
	movs	r0, #10
	bl 0x0200aa3c
	adds	r0, r5, #1
	bl 0x0200aadc
	b.n	.L_020005cc
.L_020005c0:
	movs	r0, #20
	bl 0x0200aa3c
	adds	r0, r5, #2
	bl 0x0200aadc
.L_020005cc:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aaf4
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x23f8
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200aa44
	movs	r0, #0
	bl 0x0200ab64
	movs	r1, #160
	movs	r2, #1
	adds	r0, r5, #0
	adds	r1, #255
	negs	r2, r2
	bl 0x0200aa5c
	cmp	r0, #0
	bne.n	.L_02000606
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200a9d4
.L_02000606:
	bl 0x0200aa4c
	pop	{r5, pc}
	push	{lr}
	movs	r1, #160
	movs	r0, #31
	adds	r1, #255
	bl 0x0200abbc
	movs	r3, #192
	movs	r1, #156
	lsls	r3, r3, #8
	movs	r0, #31
	lsls	r1, r1, #17
	ldr	r2, [pc, #16]
	bl 0x0200aaa4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200a9cc
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0251
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #34
	bl 0x0200a9c4
	cmp	r0, #0
	bne.n	.L_0200064a
	b.n	.L_02000a5a
.L_0200064a:
	movs	r0, #0
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000656
	b.n	.L_02000a5a
.L_02000656:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #117
	bl 0x0200a9cc
	bl 0x0200aa44
	movs	r0, #0
	bl 0x0200ab64
	ldr	r0, [pc, #156]
	bl 0x0200aadc
	movs	r3, #192
	movs	r1, #157
	lsls	r3, r3, #8
	movs	r0, #29
	lsls	r1, r1, #17
	ldr	r2, [pc, #144]
	bl 0x0200aaa4
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #157
	movs	r2, #145
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200aa7c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200aafc
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #1
	movs	r0, #29
	bl 0x0200ab2c
	bl 0x0200ab3c
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #157
	movs	r2, #156
	movs	r0, #29
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200aa7c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #29
	bl 0x0200ab1c
	movs	r1, #0
	movs	r0, #29
	bl 0x0200aae4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_02000710
	movs	r2, #5
	movs	r0, #29
	movs	r1, #0
	bl 0x0200aaec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200072c
	.4byte 0x00002f82
	.2byte 0x0000
	.2byte 0x02a7
.L_02000710:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #29
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
.L_0200072c:
	movs	r1, #10
	adds	r1, #255
	movs	r2, #30
	movs	r0, #29
	bl 0x0200ab1c
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #16
	movs	r2, #0
	movs	r0, #30
	bl 0x0200ab74
	movs	r0, #30
	bl 0x0200aa94
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #30
	bl 0x0200ab1c
	movs	r2, #5
	movs	r0, #30
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #2
	movs	r0, #29
	bl 0x0200aacc
	movs	r0, #5
	bl 0x0200aa3c
	movs	r2, #5
	movs	r0, #29
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #3
	movs	r0, #30
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #29
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #192
	movs	r0, #29
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #4
	movs	r2, #0
	movs	r0, #30
	bl 0x0200aad4
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #128
	movs	r0, #30
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #4
	bl 0x0200aafc
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #30
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #30
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #29
	bl 0x0200ab1c
	movs	r0, #29
	movs	r1, #2
	movs	r2, #0
	bl 0x0200aabc
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #30
	bl 0x0200ab1c
	movs	r2, #5
	movs	r0, #30
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #2
	movs	r0, #29
	bl 0x0200aacc
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x0200ab1c
	movs	r0, #30
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #29
	bl 0x0200ab1c
	movs	r2, #5
	movs	r0, #29
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #3
	movs	r0, #30
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #30
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r2, #5
	movs	r0, #29
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #29
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x0200ab1c
	movs	r2, #5
	movs	r0, #30
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #29
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #29
	bl 0x0200ab1c
	movs	r2, #5
	movs	r0, #29
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #3
	movs	r0, #30
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	movs	r0, #29
	bl 0x0200aacc
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	bl 0x0200860c
	movs	r1, #4
	movs	r2, #0
	movs	r0, #30
	bl 0x0200aad4
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #128
	movs	r0, #30
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #4
	bl 0x0200aafc
	movs	r0, #20
	bl 0x0200aa3c
	movs	r2, #5
	movs	r0, #30
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #3
	movs	r0, #29
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #5
	bl 0x0200aaec
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #30
	bl 0x0200ab1c
	movs	r2, #5
	movs	r0, #30
	movs	r1, #0
	bl 0x0200aaec
	movs	r1, #3
	movs	r0, #29
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r2, #5
	movs	r0, #29
	movs	r1, #0
	bl 0x0200aaec
	movs	r0, #4
	movs	r1, #3
	bl 0x0200aaac
	movs	r1, #3
	movs	r0, #30
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #29
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #4
	movs	r1, #1
	bl 0x0200ab2c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #29
	ldr	r1, [pc, #124]
	adds	r2, #153
	bl 0x0200aa6c
	movs	r1, #0
	movs	r2, #100
	movs	r0, #29
	bl 0x0200ab7c
	movs	r0, #10
	bl 0x0200aa3c
	movs	r2, #0
	movs	r1, #30
	movs	r0, #4
	bl 0x0200aad4
	movs	r0, #30
	bl 0x0200aa3c
	movs	r0, #30
	movs	r1, #3
	bl 0x0200aaac
	movs	r1, #3
	movs	r0, #4
	bl 0x0200aab4
	movs	r0, #30
	bl 0x0200aa3c
	movs	r0, #30
	movs	r1, #2
	bl 0x0200aaac
	movs	r0, #4
	bl 0x0200aa64
	cmp	r0, #0
	beq.n	.L_02000a3c
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #30
	bl 0x0200aa74
.L_02000a3c:
	movs	r0, #30
	bl 0x0200aa94
	movs	r0, #30
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r0, #29
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	bl 0x0200aa4c
.L_02000a5a:
	pop	{pc}
	.2byte 0x3333
	.2byte 0x0001
	push	{lr}
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000a74
	ldr	r0, [pc, #4]
	b.n	.L_02000a76
.L_02000a74:
	ldr	r0, [pc, #4]
.L_02000a76:
	pop	{pc}
	.4byte 0x0200b410
	.2byte 0xb20c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #144]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02000b10
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #124
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000a9c:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #96
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000a9c
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #98
	strh	r0, [r3, #0]
	adds	r3, #58
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000ac6:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #128
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000ac6
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #130
	strh	r0, [r3, #0]
	adds	r3, #58
	ldrh	r3, [r3, #0]
	movs	r1, #14
	lsls	r3, r3, #16
	asrs	r0, r3, #16
.L_02000af0:
	movs	r4, #160
	lsls	r4, r4, #19
	lsls	r3, r1, #1
	adds	r4, #160
	adds	r2, r3, r4
	subs	r4, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	cmp	r1, #1
	bne.n	.L_02000af0
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #162
	strh	r0, [r3, #0]
.L_02000b10:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #200
	subs	r2, #172
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #208]
	bl 0x0200a9bc
	movs	r0, #8
	bl 0x0200a9fc
	movs	r0, #10
	bl 0x0200aa64
	movs	r1, #0
	bl 0x0200aa14
	movs	r0, #11
	bl 0x0200aa64
	movs	r1, #0
	bl 0x0200aa14
	movs	r1, #3
	movs	r0, #13
	bl 0x0200ab14
	movs	r0, #23
	bl 0x0200aa64
	movs	r5, #128
	lsls	r5, r5, #8
	str	r5, [r0, #24]
	movs	r0, #23
	bl 0x0200aa64
	str	r5, [r0, #28]
	bl 0x0200aba4
	movs	r1, #8
	movs	r2, #9
	movs	r0, #0
	bl 0x0200abac
	movs	r0, #3
	bl 0x0200a9fc
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000be8
	movs	r0, #20
	movs	r1, #2
	bl 0x0200ab14
	movs	r0, #21
	movs	r1, #3
	bl 0x0200ab14
	movs	r0, #22
	movs	r1, #3
	bl 0x0200ab14
	movs	r1, #3
	movs	r0, #19
	bl 0x0200ab14
	movs	r0, #18
	bl 0x0200aa64
	movs	r1, #9
	bl 0x0200ab9c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #115
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000be8
	movs	r0, #32
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r0, #33
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r0, #34
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
.L_02000be8:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #116
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000bfa
	bl 0x0200860c
.L_02000bfa:
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x8a81
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	sub	sp, #8
	bl 0x0200a9c4
	cmp	r0, #0
	beq.n	.L_02000c3a
	movs	r5, #16
	movs	r6, #23
	movs	r0, #11
	movs	r1, #51
	movs	r2, #5
	movs	r3, #4
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200aa0c
	movs	r0, #11
	movs	r1, #51
	movs	r2, #5
	movs	r3, #6
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200aa04
.L_02000c3a:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
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
	bl 0x0200a9ec
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000c84
	movs	r1, #1
	ldr	r5, [r6, #80]
	bl 0x0200a9dc
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	bl 0x0200a9e4
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
.L_02000c84:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0xb620
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	adds	r5, r1, #0
	bl 0x0200aa64
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200aa64
	movs	r3, #224
	lsls	r3, r3, #13
	mov	sl, r3
	ldr	r1, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #5
	mov	fp, r3
	movs	r3, #15
	adds	r5, r0, #0
	ldr	r2, [r6, #16]
	add	r1, sl
	mov	r8, r3
	ldr	r0, [r6, #8]
	str	r3, [sp, #0]
	mov	r3, fp
	bl 0x02008c40
	movs	r0, #151
	bl 0x0200abc4
	movs	r0, #15
	bl 0x0200aa3c
	ldr	r1, [r5, #12]
	movs	r3, #240
	lsls	r3, r3, #8
	mov	r9, r3
	mov	r3, r8
	ldr	r2, [r5, #16]
	add	r1, sl
	ldr	r0, [r5, #8]
	str	r3, [sp, #0]
	mov	r3, r9
	bl 0x02008c40
	movs	r0, #151
	bl 0x0200abc4
	movs	r0, #15
	bl 0x0200aa3c
	ldr	r1, [r6, #12]
	mov	r3, r8
	ldr	r2, [r6, #16]
	add	r1, sl
	ldr	r0, [r6, #8]
	str	r3, [sp, #0]
	mov	r3, fp
	bl 0x02008c40
	movs	r0, #151
	bl 0x0200abc4
	movs	r0, #15
	bl 0x0200aa3c
	ldr	r1, [r5, #12]
	mov	r3, r8
	ldr	r2, [r5, #16]
	ldr	r0, [r5, #8]
	add	r1, sl
	str	r3, [sp, #0]
	mov	r3, r9
	bl 0x02008c40
	movs	r0, #151
	bl 0x0200abc4
	movs	r0, #15
	bl 0x0200aa3c
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [sp, #32]
	mov	r8, r1
	mov	sl, r2
	ldr	r7, [r3, #108]
	bl 0x0200ab94
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200ab94
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200ab6c
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r1, r7, r2
	adds	r3, r0, #0
	ldrh	r0, [r1, #0]
	lsls	r3, r3, #16
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r1, #0]
	asrs	r0, r0, #16
	mov	r1, r8
	mov	r2, sl
	bl 0x0200aa1c
	adds	r0, r6, #0
	ldr	r3, [sp, #28]
	movs	r1, #0
	mov	r2, r9
	bl 0x0200aa2c
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_02000db6
	b.n	.L_02000dae
.L_02000da8:
	ldr	r0, [sp, #52]
	bl 0x0200a9b4
.L_02000dae:
	bl 0x0200aa24
	cmp	r0, #0
	beq.n	.L_02000da8
.L_02000db6:
	adds	r0, r5, #0
	bl 0x0200ab6c
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r1, r7, r2
	adds	r3, r0, #0
	ldrh	r0, [r1, #0]
	lsls	r3, r3, #16
	adds	r2, r0, #1
	strh	r2, [r1, #0]
	lsls	r0, r0, #16
	ldr	r1, [sp, #36]
	ldr	r2, [sp, #40]
	asrs	r0, r0, #16
	bl 0x0200aa1c
	adds	r0, r5, #0
	movs	r1, #0
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #48]
	bl 0x0200aa2c
	b.n	.L_02000dec
.L_02000de6:
	movs	r0, #1
	bl 0x0200a9b4
.L_02000dec:
	bl 0x0200aa24
	cmp	r0, #0
	beq.n	.L_02000de6
	movs	r0, #1
	bl 0x0200a9b4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r5, r1, #0
	bl 0x0200ab94
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl 0x0200ab94
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200aa34
	adds	r0, r5, #0
	bl 0x0200aa34
	pop	{r5, r6, pc}
	push	{lr}
	movs	r0, #151
	lsls	r0, r0, #4
	bl 0x0200a9cc
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r0, #151
	lsls	r0, r0, #4
	sub	sp, #28
	bl 0x0200a9c4
	cmp	r0, #0
	bne.n	.L_02000e4e
	bl 0x0200a99e
.L_02000e4e:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #113
	bl 0x0200a9cc
	bl 0x0200aa44
	movs	r0, #0
	bl 0x0200ab64
	ldr	r0, [pc, #764]
	bl 0x0200aadc
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #25
	adds	r1, #204
	adds	r2, #102
	bl 0x0200aa6c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #26
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #27
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r1, #156
	movs	r2, #130
	movs	r0, #25
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200aa9c
	movs	r1, #228
	movs	r2, #154
	movs	r0, #26
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200aa9c
	movs	r1, #168
	movs	r2, #154
	lsls	r2, r2, #18
	lsls	r1, r1, #16
	movs	r0, #27
	bl 0x0200aa9c
	movs	r0, #78
	bl 0x0200abc4
	movs	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #20
	movs	r0, #4
	bl 0x0200ab1c
	movs	r1, #156
	movs	r2, #154
	movs	r0, #4
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	bl 0x0200aa7c
	movs	r0, #156
	movs	r1, #1
	movs	r2, #150
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	bl 0x0200ab34
	bl 0x0200ab3c
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #39
	bl 0x0200abc4
	movs	r1, #152
	movs	r2, #144
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #25
	bl 0x0200aa7c
	movs	r0, #5
	bl 0x0200aa3c
	movs	r1, #16
	movs	r3, #192
	movs	r0, #7
	negs	r1, r1
	movs	r2, #0
	lsls	r3, r3, #8
	bl 0x0200ab74
	movs	r3, #192
	movs	r0, #5
	movs	r1, #0
	movs	r2, #16
	lsls	r3, r3, #8
.L_02000f38:
	bl 0x0200ab74
	movs	r1, #16
	movs	r3, #192
	movs	r0, #6
	negs	r1, r1
	movs	r2, #16
	lsls	r3, r3, #8
	bl 0x0200ab74
	movs	r1, #32
	movs	r3, #208
	lsls	r3, r3, #8
	negs	r1, r1
	movs	r2, #0
	movs	r0, #28
	bl 0x0200ab74
	movs	r0, #28
	bl 0x0200aa94
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200ab04
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #0
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #208
	movs	r2, #0
	movs	r0, #28
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200ab1c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #25
	bl 0x0200aae4
	movs	r0, #4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_0200101e
	movs	r0, #15
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02001042
.L_0200101e:
	movs	r0, #35
	bl 0x0200aa3c
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
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
.L_02001042:
	movs	r2, #0
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #28
	bl 0x0200ab1c
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #208
	movs	r0, #28
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #4
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #25
	bl 0x0200aaf4
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #25
	bl 0x0200aae4
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab04
	movs	r0, #4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_02001164
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020011a2
	.2byte 0x0000
	.2byte 0x243b
	.2byte 0x0000
.L_02001164:
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #6
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
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
	bl 0x0200aaf4
.L_020011a2:
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #208
	movs	r0, #28
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #28
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #5
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #7
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #6
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab24
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r2, #0
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aafc
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab04
	movs	r1, #172
	movs	r2, #154
	lsls	r1, r1, #1
	lsls	r2, r2, #2
	movs	r0, #26
	bl 0x0200aa7c
	movs	r0, #5
	bl 0x0200aa3c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ab1c
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #26
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #4
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #5
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #5
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #7
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #7
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #6
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	movs	r0, #6
	lsls	r1, r1, #1
	bl 0x0200ab24
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab24
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #28
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r2, #154
	movs	r1, #232
	lsls	r2, r2, #2
	movs	r0, #27
	bl 0x0200aa7c
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #60
	movs	r0, #28
	bl 0x0200ab1c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r2, #0
	movs	r0, #27
	movs	r1, #16
	bl 0x0200ab84
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r2, #25
	movs	r0, #27
	movs	r1, #4
	bl 0x0200aabc
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #208
	movs	r0, #27
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #15
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #4
	adds	r1, #255
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ab1c
	movs	r1, #4
	movs	r2, #50
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #5
	bl 0x0200aa3c
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200ab1c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #42
	movs	r0, #28
	bl 0x0200ab1c
	movs	r2, #16
	negs	r2, r2
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab84
	movs	r1, #208
	movs	r0, #28
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #7
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #128
	movs	r0, #28
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r2, #8
	movs	r1, #8
	negs	r2, r2
	movs	r0, #28
	bl 0x0200ab84
	movs	r0, #5
	bl 0x0200aa3c
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab1c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #25
	bl 0x0200aa6c
	movs	r0, #25
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #7
	movs	r2, #7
	strb	r3, [r0, #0]
	negs	r1, r1
	movs	r0, #25
	bl 0x0200ab84
	movs	r0, #1
	bl 0x0200aa3c
	movs	r0, #25
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r1, #132
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #25
	bl 0x0200aaf4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #28
	bl 0x0200ab1c
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #25
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab24
	movs	r0, #40
	bl 0x0200aa3c
	movs	r0, #26
	movs	r1, #2
	bl 0x0200aacc
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #204
	movs	r0, #25
	adds	r1, #153
	bl 0x0200aa6c
	movs	r0, #25
	movs	r1, #0
	bl 0x0200ab04
	movs	r2, #0
	movs	r0, #25
	movs	r1, #8
	bl 0x0200ab7c
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #27
	bl 0x0200ab1c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #26
	bl 0x0200ab1c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200ab1c
	movs	r1, #0
	movs	r0, #27
	bl 0x0200ab04
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #30
	bl 0x0200aa3c
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #204
	movs	r0, #28
	adds	r1, #153
	bl 0x0200aa6c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #2
	bl 0x0200aa3c
	movs	r0, #28
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #8
	ands	r5, r3
	movs	r2, #8
	negs	r1, r1
	strb	r5, [r0, #0]
	movs	r0, #28
	bl 0x0200ab84
	movs	r0, #1
	bl 0x0200aa3c
	movs	r0, #28
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #160
	orrs	r6, r3
	strb	r6, [r0, #0]
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #28
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #28
	movs	r1, #0
.L_0200190e:
	bl 0x0200aaf4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #42
	movs	r0, #26
	bl 0x0200ab1c
	movs	r0, #26
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aabc
	movs	r2, #23
	movs	r0, #26
	movs	r1, #6
	bl 0x0200aabc
.L_02001932:
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #6
.L_02001950:
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r2, #0
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #27
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aabc
	movs	r2, #23
	movs	r0, #27
	movs	r1, #6
	bl 0x0200aabc
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #26
	adds	r1, #204
	adds	r2, #102
	bl 0x0200aa6c
	movs	r1, #12
	movs	r2, #0
	negs	r1, r1
	movs	r0, #26
	bl 0x0200ab84
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #6
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #7
	bl 0x0200ab1c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #45
	movs	r0, #28
	bl 0x0200ab1c
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #27
	adds	r1, #204
	adds	r2, #102
	bl 0x0200aa6c
	movs	r1, #12
	movs	r2, #0
	movs	r0, #27
	bl 0x0200ab84
	movs	r0, #5
	bl 0x0200aa3c
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #27
	bl 0x0200ab1c
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #20
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200ab1c
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #208
	movs	r0, #27
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #7
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #26
	bl 0x0200ab1c
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200aafc
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab04
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #4
	adds	r1, #255
	movs	r2, #42
	movs	r0, #27
	bl 0x0200ab1c
	movs	r0, #27
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aabc
	movs	r2, #23
	movs	r0, #27
	movs	r1, #6
	bl 0x0200aabc
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #160
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200aafc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #26
	movs	r1, #2
	bl 0x0200aacc
	movs	r1, #176
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #224
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #224
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #27
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #160
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #160
	movs	r0, #6
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #160
	movs	r2, #0
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200aafc
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #15
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #25
	bl 0x0200aa3c
	movs	r0, #26
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #25
	bl 0x0200aaf4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #27
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	movs	r0, #25
	lsls	r1, r1, #6
	bl 0x0200ab04
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #160
	movs	r0, #25
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #7
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #15
	bl 0x0200aa3c
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #26
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #27
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #27
	movs	r1, #0
	bl 0x0200ab04
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #4
	movs	r1, #27
	bl 0x0200ab8c
	movs	r0, #7
	movs	r1, #27
	bl 0x0200ab8c
	movs	r0, #5
	movs	r1, #27
	bl 0x0200ab8c
	movs	r0, #6
	movs	r1, #27
	bl 0x0200ab8c
	movs	r0, #28
	movs	r1, #27
	bl 0x0200ab8c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #27
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r0, #27
	movs	r1, #20
	movs	r2, #0
	bl 0x0200ab84
	movs	r0, #27
	movs	r1, #2
	movs	r2, #32
	bl 0x0200ab84
	movs	r0, #27
	movs	r1, #32
	movs	r2, #32
	bl 0x0200ab84
	movs	r2, #32
	movs	r0, #27
	movs	r1, #0
	bl 0x0200ab84
	movs	r1, #128
	movs	r0, #26
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #26
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #26
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aafc
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #4
	movs	r1, #26
	bl 0x0200ab8c
	movs	r0, #7
	movs	r1, #26
	bl 0x0200ab8c
	movs	r0, #5
	movs	r1, #26
	bl 0x0200ab8c
	movs	r0, #6
	movs	r1, #26
	bl 0x0200ab8c
	movs	r0, #28
	movs	r1, #26
	bl 0x0200ab8c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #26
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #32
	bl 0x0200ab84
	movs	r1, #20
	movs	r0, #26
	negs	r1, r1
	movs	r2, #24
	bl 0x0200ab84
	movs	r0, #26
	movs	r1, #0
	movs	r2, #48
	bl 0x0200ab84
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #7
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #192
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #208
	lsls	r1, r1, #8
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #26
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r2, #0
	movs	r0, #27
	movs	r1, #0
	bl 0x0200aa9c
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #25
	bl 0x0200ab8c
	movs	r0, #7
	movs	r1, #25
	bl 0x0200ab8c
	movs	r0, #5
	movs	r1, #25
	bl 0x0200ab8c
	movs	r0, #6
	movs	r1, #25
	bl 0x0200ab8c
	movs	r0, #28
	movs	r1, #25
	bl 0x0200ab8c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #25
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r2, #24
	movs	r0, #25
	movs	r1, #22
	bl 0x0200ab84
	movs	r0, #25
	movs	r1, #1
	bl 0x0200ab2c
	movs	r0, #25
	movs	r1, #0
	movs	r2, #32
	bl 0x0200ab84
	movs	r1, #16
	movs	r0, #25
	negs	r1, r1
	movs	r2, #16
	bl 0x0200ab84
	movs	r0, #25
	movs	r1, #0
	movs	r2, #8
	bl 0x0200ab84
	movs	r1, #128
	movs	r2, #40
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r1, #176
	movs	r0, #25
	lsls	r1, r1, #8
	bl 0x0200ab04
	movs	r0, #152
	movs	r1, #1
	movs	r2, #162
	movs	r3, #1
	lsls	r2, r2, #18
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200ab34
	bl 0x0200ab3c
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	movs	r0, #28
	bl 0x0200aacc
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #28
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200aa6c
	movs	r2, #32
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab84
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #28
	bl 0x0200ab1c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #28
	bl 0x0200ab1c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab24
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r2, #0
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #25
	bl 0x0200aa3c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	adds	r1, #102
	adds	r2, #51
	movs	r0, #28
	bl 0x0200aa6c
	movs	r0, #28
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #0
	strb	r3, [r0, #0]
	movs	r1, #0
	mov	r9, r2
	movs	r0, #28
	subs	r2, #24
	bl 0x0200ab84
	movs	r0, #1
	bl 0x0200aa3c
	movs	r0, #28
	bl 0x0200aa64
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #8
	movs	r0, #28
	bl 0x0200aaac
	movs	r0, #30
	bl 0x0200aa3c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r0, #6
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #25
	bl 0x0200aa3c
	movs	r0, #5
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #6
	movs	r2, #30
	adds	r1, #255
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #4
	bl 0x0200ab1c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #7
	bl 0x0200ab1c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #25
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #25
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #6
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #25
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #25
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r0, #25
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #25
	bl 0x0200aaf4
	movs	r0, #78
	bl 0x0200abc4
	movs	r0, #25
	movs	r1, #0
	movs	r2, #80
	bl 0x0200ab84
	movs	r1, #0
	movs	r2, #0
	movs	r0, #25
	bl 0x0200aa9c
	movs	r0, #30
	bl 0x0200aa3c
	movs	r0, #152
	movs	r1, #1
	movs	r2, #156
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #18
	lsls	r0, r0, #17
	bl 0x0200ab34
	bl 0x0200ab3c
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #3
	bl 0x0200abc4
	movs	r1, #8
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #4
	movs	r2, #30
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #10
	movs	r2, #30
	adds	r1, #255
	movs	r0, #7
	bl 0x0200ab1c
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #7
	movs	r1, #2
	bl 0x0200aacc
	movs	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x0200aafc
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #1
	movs	r0, #28
	bl 0x0200aaac
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #28
	adds	r1, #204
	adds	r2, #102
	bl 0x0200aa6c
	movs	r2, #16
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab84
	movs	r1, #0
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #28
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #28
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #7
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200ab24
	movs	r0, #45
	bl 0x0200aa3c
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #5
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #1
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #25
	bl 0x0200aa3c
	movs	r0, #5
	movs	r1, #6
	bl 0x02008c90
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #6
	bl 0x0200ab1c
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #5
	movs	r1, #4
	bl 0x0200aaac
	movs	r0, #6
	movs	r1, #4
	bl 0x0200aab4
	movs	r3, #14
	str	r3, [sp, #12]
	movs	r2, #9
	mov	r8, r3
	mov	r3, r9
	str	r2, [sp, #20]
	str	r3, [sp, #24]
	movs	r5, #8
	movs	r3, #18
	movs	r6, #6
	mov	sl, r2
	movs	r0, #5
	movs	r2, #5
	movs	r1, #5
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	str	r6, [sp, #8]
	str	r5, [sp, #16]
	bl 0x0200ab0c
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #4
	movs	r1, #3
	bl 0x0200aac4
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #4
	bl 0x0200ab24
	movs	r0, #40
	bl 0x0200aa3c
	movs	r1, #192
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #6
	bl 0x0200ab1c
	mov	r2, r8
	str	r2, [sp, #12]
	mov	r3, sl
	mov	r2, r9
	str	r3, [sp, #20]
	str	r2, [sp, #24]
	movs	r0, #5
	movs	r1, #8
	movs	r2, #5
	movs	r3, #18
	str	r5, [sp, #0]
	str	r6, [sp, #4]
	str	r5, [sp, #8]
	str	r5, [sp, #16]
	bl 0x02008d4c
	movs	r1, #192
	movs	r0, #4
	lsls	r1, r1, #7
	bl 0x0200ab04
	movs	r0, #4
	movs	r1, #0
	bl 0x0200aa54
	cmp	r0, #0
	bne.n	.L_020026a8
	movs	r1, #6
	movs	r0, #5
	bl 0x02008e04
	movs	r0, #15
	bl 0x0200aa3c
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab1c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_020026dc
.L_020026a8:
	movs	r1, #6
	movs	r0, #5
	bl 0x02008e04
	movs	r0, #35
	bl 0x0200aa3c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #28
	bl 0x0200ab1c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	movs	r0, #28
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl 0x0200aaf4
.L_020026dc:
	movs	r1, #128
	movs	r0, #4
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #128
	movs	r2, #0
	movs	r0, #5
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #6
	bl 0x0200ab04
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #0
	movs	r0, #5
	bl 0x0200ab1c
	movs	r1, #8
	movs	r2, #45
	adds	r1, #255
	movs	r0, #6
	bl 0x0200ab1c
	movs	r0, #28
	movs	r1, #4
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #5
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aabc
	movs	r2, #23
	movs	r0, #5
	movs	r1, #6
	bl 0x0200aabc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aab4
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #6
	movs	r1, #6
	movs	r2, #15
	bl 0x0200aabc
	movs	r2, #23
	movs	r0, #6
	movs	r1, #6
	bl 0x0200aabc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #6
	movs	r1, #0
	bl 0x0200aaf4
	movs	r0, #28
	movs	r1, #3
	bl 0x0200aab4
	movs	r1, #0
	movs	r0, #28
	bl 0x0200aaf4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #28
	bl 0x0200ab1c
	movs	r2, #8
	negs	r2, r2
	movs	r0, #28
	movs	r1, #0
	bl 0x0200ab84
	movs	r1, #0
	movs	r0, #28
	bl 0x0200ab04
	movs	r0, #5
	bl 0x0200aa3c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #132
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #7
	bl 0x0200ab1c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #129
	movs	r2, #30
	lsls	r1, r1, #1
	movs	r0, #28
	bl 0x0200ab1c
	movs	r0, #28
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #3
	movs	r0, #7
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	adds	r0, #7
	bl 0x0200aaf4
	movs	r0, #5
	bl 0x0200aa3c
	movs	r1, #2
	movs	r2, #45
	adds	r1, #255
	movs	r0, #28
	bl 0x0200ab1c
	movs	r1, #3
	movs	r0, #7
	bl 0x0200aab4
	movs	r0, #10
	bl 0x0200aa3c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #7
	movs	r1, #0
	bl 0x0200aaf4
	movs	r1, #2
	movs	r0, #28
	bl 0x0200aacc
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aafc
	movs	r1, #224
	movs	r2, #0
	movs	r0, #6
	lsls	r1, r1, #8
	bl 0x0200aafc
	movs	r1, #0
	movs	r0, #7
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aa3c
	movs	r1, #3
	movs	r0, #4
	bl 0x0200aab4
	movs	r0, #15
	bl 0x0200aa3c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200aaac
	movs	r0, #6
	movs	r1, #3
	bl 0x0200aaac
	movs	r0, #7
	movs	r1, #3
	bl 0x0200aaac
	movs	r1, #3
	movs	r0, #28
	bl 0x0200aab4
	movs	r0, #20
	bl 0x0200aa3c
	movs	r2, #153
	lsls	r2, r2, #8
	adds	r2, #153
	movs	r0, #5
	ldr	r1, [pc, #264]
	bl 0x0200aa6c
	movs	r0, #5
	movs	r1, #2
	bl 0x0200aaac
	ldr	r3, [pc, #252]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	bl 0x0200aa64
	cmp	r0, #0
	beq.n	.L_020028d0
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #5
	bl 0x0200aa74
.L_020028d0:
	movs	r0, #5
	bl 0x0200aa94
	movs	r0, #5
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #6
	ldr	r1, [pc, #196]
	adds	r2, #153
	bl 0x0200aa6c
	movs	r0, #6
	movs	r1, #2
	bl 0x0200aaac
	ldr	r0, [r5, #0]
	bl 0x0200aa64
	cmp	r0, #0
	beq.n	.L_0200290e
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #6
	bl 0x0200aa74
.L_0200290e:
	movs	r0, #6
	bl 0x0200aa94
	movs	r0, #6
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #7
	ldr	r1, [pc, #132]
	adds	r2, #153
	bl 0x0200aa6c
	movs	r0, #7
	movs	r1, #2
	bl 0x0200aaac
	ldr	r0, [r5, #0]
	bl 0x0200aa64
	cmp	r0, #0
	beq.n	.L_0200294c
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #7
	bl 0x0200aa74
.L_0200294c:
	movs	r0, #7
	bl 0x0200aa94
	movs	r0, #7
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #28
	ldr	r1, [pc, #72]
	adds	r2, #153
	bl 0x0200aa6c
	movs	r0, #28
	movs	r1, #2
	bl 0x0200aaac
	ldr	r0, [r5, #0]
	bl 0x0200aa64
	cmp	r0, #0
	beq.n	.L_0200298a
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #28
	bl 0x0200aa74
.L_0200298a:
	movs	r0, #28
	bl 0x0200aa94
	movs	r0, #28
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aa9c
	bl 0x0200aa4c
	add	sp, #28
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x02000240
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x080200a9, 0x080200c1, 0x08020199, 0x080201a1, 0x080201e9, 0x080201f1, 0x08020219, 0x08038039, 0x08038049, 0x080380f9, 0x08038101, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8079, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d9, 0x080c80e1, 0x080c80e9, 0x080c80f1, 0x080c80f9, 0x080c8101, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8141, 0x080c8149, 0x080c8161, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c81f1, 0x080c8201, 0x080c8211, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c8409, 0x080c84e1, 0x080c85c9, 0x080c85e9, 0x080c85f1, 0x080c85f9, 0x080c8601, 0x080c8621, 0x080c8681, 0x080c86d1, 0x080c86d9, 0x080c8779, 0x080c8861, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0xffff0000
	.4byte 0x00000118
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b6
	.4byte 0x101010b7
	.4byte 0xffffffff
	.4byte 0x102020b7
	.4byte 0xffffffff
	.4byte 0x103030b7
	.4byte 0xffffffff
	.4byte 0x104040b7
	.4byte 0xffffffff
	.4byte 0x105050b7
	.4byte 0xffffffff
	.4byte 0x106060b7
	.4byte 0xffffffff
	.4byte 0x107010b8
	.4byte 0xffffffff
	.4byte 0x108010b9
	.4byte 0xffffffff
	.4byte 0x10928002
	.4byte 0xffffffff
	.4byte 0x10a070ba
	.4byte 0xffffffff
	.4byte 0x10b130bb
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000008
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00010000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff00ff
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00013000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0021
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
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00005000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00008000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001b000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001d000
	.4byte 0xffff00ff
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00013000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0021
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
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0003b000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0003d000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0024ffff
	.4byte 0x00020002
	.4byte 0x00060002
	.4byte 0x00040024
	.4byte 0x00020002
	.4byte 0xffff0006
	.4byte 0x0200b1e6
	.4byte 0x0009001d
	.4byte 0x0200b1e4
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x020080fd
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x020080fd
	.4byte 0x0000c403
	.4byte 0xffff0018
	.4byte 0x004024c0
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200814d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008179
	.4byte 0x00000002
	.4byte 0x0a750064
	.4byte 0x02008639
	.4byte 0x00000002
	.4byte 0x09700013
	.4byte 0x02008e25
	.4byte 0x00000002
	.4byte 0x09710014
	.4byte 0x02008e31
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000023ec
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000023ed
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000023ee
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000023ef
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020084b5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000023f3
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008431
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000023f5
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000023f6
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000023f7
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008591
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000023fb
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000023fc
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000023fd
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000023fe
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000023ff
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002400
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002401
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002402
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002403
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002404
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002405
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002423
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000242c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x020085dd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x020080fd
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020080fd
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte 0x020080fd
	.4byte 0x0000c403
	.4byte 0xffff0018
	.4byte 0x004024c0
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200814d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008179
	.4byte 0x00000002
	.4byte 0x0a750064
	.4byte 0x02008639
	.4byte 0x00000002
	.4byte 0x09700013
	.4byte 0x02008e25
	.4byte 0x00000002
	.4byte 0x09710014
	.4byte 0x02008e31
	.4byte 0x00000002
	.4byte 0x0973001a
	.4byte 0x02008189
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025c2
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025c3
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025c4
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008469
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020084b5
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000025cb
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000025cc
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000025cd
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000025ce
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008555
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x000025d1
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025d2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025d3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025d4
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025d5
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000025d6
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000025d7
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000025d8
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000025d9
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000025da
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000025db
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000025dc
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000268c
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000269c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x020085dd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000026
