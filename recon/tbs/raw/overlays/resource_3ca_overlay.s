.syntax unified
	.thumb
	.section .text.x02008194,"ax",%progbits
	.align 2
	.global Func_02000194
	.thumb_func
Func_02000194:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #444]
	ldr	r3, [r3, #0]
	mov	r9, r3
	ldr	r3, [pc, #440]
	ldr	r3, [r3, #0]
	sub	sp, #20
	cmp	r3, #0
	beq.n	.L_020001e0
	ldr	r5, [pc, #436]
	ldr	r0, [r5, #0]
	lsls	r0, r0, #9
	bl 0x0200933c
	ldr	r3, [pc, #428]
	movs	r1, #3
	mov	ip, pc
	bx	r3
	.4byte 0x30084b6a
	.4byte 0x466a681b
	.4byte 0x32120200
	.4byte 0x8013181b
	.4byte 0x4b678812
	.4byte 0x682b801a
	.2byte 0x3301
	.2byte 0x602b
.L_020001e0:
	ldr	r3, [pc, #404]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020002bc
	ldr	r2, [pc, #400]
	ldr	r0, [r2, #0]
	lsls	r0, r0, #9
	mov	fp, r2
	bl 0x0200933c
	ldr	r3, [pc, #372]
	movs	r1, #2
	mov	ip, pc
	bx	r3
	.4byte 0x22a04b60
	.4byte 0x0406681b
	.4byte 0x444a0052
	.4byte 0x6013199b
	.4byte 0x22b84b5d
	.4byte 0x0052681b
	.4byte 0x199b444a
	.4byte 0x4b5b6013
	.4byte 0x46984a5b
	.4byte 0x4692681b
	.4byte 0xd00c4553
	.4byte 0xf0012000
	.4byte 0x4642f8d3
	.4byte 0x1c056813
	.4byte 0x1c2a199b
	.4byte 0x616b60eb
	.4byte 0x23003255
	.4byte 0x4f537013
	.4byte 0x4553683b
	.4byte 0x2001d00e
	.4byte 0xf8c2f001
	.4byte 0x1c05683b
	.4byte 0x60eb199b
	.4byte 0x68134642
	.4byte 0x199b1c2a
	.4byte 0x3255616b
	.4byte 0x70132300
	.4byte 0x683b4f4a
	.4byte 0xd00e4553
	.4byte 0xf0012003
	.4byte 0x683bf8af
	.4byte 0x199b1c05
	.4byte 0x464260eb
	.4byte 0x1c2a6813
	.4byte 0x616b199b
	.4byte 0x23003255
	.4byte 0x4f427013
	.4byte 0x4553683b
	.4byte 0x2002d00e
	.4byte 0xf89cf001
	.4byte 0x1c05683b
	.4byte 0x60eb199b
	.4byte 0x68134642
	.4byte 0x199b1c2a
	.4byte 0x3255616b
	.4byte 0x70132300
	.4byte 0x6813465a
	.2byte 0x3301
	.2byte 0x6013
.L_020002bc:
	ldr	r3, [pc, #220]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020003ac
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	movs	r7, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_020003ac
	mov	r3, r9
	adds	r3, #228
	ldr	r6, [r3, #0]
	adds	r3, #4
	ldr	r2, [pc, #176]
	ldr	r5, [r3, #0]
	ands	r6, r2
	ands	r5, r2
	bl 0x02009334
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #4
	add	r2, sp, #4
	adds	r6, r6, r3
	movs	r3, #0
	str	r3, [r2, #4]
	str	r6, [r2, #0]
	str	r2, [sp, #0]
	bl 0x02009334
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #5
	adds	r5, r5, r3
	movs	r3, #240
	ldr	r2, [sp, #0]
	lsls	r3, r3, #13
	adds	r5, r5, r3
	str	r5, [r2, #8]
	ldr	r1, [r2, #0]
	adds	r3, r5, #0
	ldr	r2, [r2, #4]
	ldr	r0, [pc, #144]
	bl 0x0200937c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020003ac
	ldr	r3, [pc, #136]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #60
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	ldr	r1, [pc, #44]
	adds	r3, #102
	strh	r7, [r3, #0]
	subs	r3, #17
	strb	r1, [r3, #0]
	subs	r2, #65
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r1, [r5, #80]
	ldrb	r2, [r1, #9]
	subs	r3, #15
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #0
	bl 0x02009394
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200936c
	b.n	.L_020003ac
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0x020097e8
	.4byte 0x020097ec
	.4byte 0x03000118
	.4byte 0x020097f0
	.4byte 0x04000052
	.4byte 0x020097fc
	.4byte 0x02009800
	.4byte 0x02009804
	.4byte 0x02009808
	.4byte 0x0200980c
	.4byte 0xffff0000
	.4byte 0x02009810
	.4byte 0x02009814
	.4byte 0x02009818
	.4byte 0x020097f8
	.4byte 0x03001e40
	.4byte 0x000001f7
	.2byte 0x8101
	.2byte 0x0200
.L_020003ac:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.section .text.x02008b34,"ax",%progbits
	.align 2
	.global Func_02000b34
	.thumb_func
Func_02000b34:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #372]
	ldr	r3, [r3, #0]
	sub	sp, #8
	mov	sl, r3
	bl 0x020093bc
	bl 0x0200947c
	movs	r1, #156
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #232
	bl 0x020093f4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #0
	bl 0x02009434
	movs	r0, #40
	bl 0x020093b4
	movs	r0, #140
	bl 0x0200949c
	movs	r6, #160
	movs	r5, #0
	lsls	r6, r6, #19
.L_02000b76:
	lsls	r3, r5, #11
	lsls	r2, r5, #5
	orrs	r3, r2
	strh	r3, [r6, #0]
	movs	r0, #10
	adds	r5, #1
	bl 0x020093b4
	cmp	r5, #15
	ble.n	.L_02000b76
	movs	r2, #252
	movs	r3, #160
	lsls	r2, r2, #7
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r2, [pc, #288]
	movs	r7, #129
	ldr	r6, [pc, #288]
	mov	r8, r2
	lsls	r7, r7, #4
	movs	r5, #2
.L_02000ba0:
	movs	r0, #212
	bl 0x0200949c
	mov	r3, r8
	strh	r3, [r6, #0]
	movs	r0, #3
	bl 0x020093b4
	strh	r7, [r6, #0]
	movs	r0, #65
	subs	r5, #1
	bl 0x020093b4
	cmp	r5, #0
	bge.n	.L_02000ba0
	ldr	r3, [pc, #256]
	movs	r5, #1
	str	r5, [r3, #0]
	ldr	r3, [pc, #252]
	movs	r2, #0
	movs	r1, #200
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #248]
	mov	r8, r2
	bl 0x02009324
	ldr	r6, [pc, #244]
	movs	r0, #20
	str	r5, [r6, #0]
	bl 0x020093b4
	movs	r0, #163
	bl 0x0200949c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200939c
	movs	r0, #60
	bl 0x020093b4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	str	r5, [r6, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200939c
	movs	r0, #60
	bl 0x020093b4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200939c
	ldr	r3, [pc, #168]
	mov	r2, r8
	movs	r1, #200
	movs	r6, #160
	movs	r5, #184
	str	r2, [r3, #0]
	ldr	r0, [pc, #160]
	lsls	r1, r1, #4
	lsls	r6, r6, #1
	lsls	r5, r5, #1
	ldr	r7, [pc, #156]
	bl 0x02009324
	add	r6, sl
	movs	r2, #0
	add	r5, sl
.L_02000c44:
	ldr	r3, [r6, #0]
	adds	r3, r3, r7
	str	r3, [r6, #0]
	ldr	r3, [r5, #0]
	adds	r3, r3, r7
	adds	r2, r2, r7
	str	r3, [r5, #0]
	movs	r0, #1
	str	r2, [sp, #0]
	bl 0x0200931c
	ldr	r3, [pc, #128]
	ldr	r2, [sp, #0]
	cmp	r2, r3
	ble.n	.L_02000c44
	ldr	r0, [pc, #112]
	bl 0x0200932c
	ldr	r0, [pc, #116]
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #116]
	ldrh	r1, [r0, #0]
	movs	r6, #0
	ldr	r4, [pc, #56]
	str	r6, [r3, #0]
	adds	r3, r2, #0
	mov	r5, sp
	ands	r3, r1
	orrs	r3, r4
	adds	r5, #6
	strh	r3, [r5, #0]
	strh	r3, [r0, #0]
	subs	r0, #2
	ldrh	r1, [r0, #0]
	adds	r3, r2, #0
	ands	r3, r1
	orrs	r3, r4
	strh	r3, [r5, #0]
	ldr	r1, [pc, #84]
	strh	r3, [r0, #0]
	ldrh	r3, [r1, #0]
	ands	r2, r3
	ldr	r3, [pc, #20]
	orrs	r2, r3
	ldr	r3, [pc, #32]
	movs	r0, #144
	strh	r2, [r5, #0]
	str	r6, [r3, #0]
	strh	r2, [r1, #0]
	lsls	r0, r0, #1
	b.n	.L_02000cec
	.2byte 0x0000
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x03001e70
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x020097e8
	.4byte 0x020097ec
	.4byte 0x02008195
	.4byte 0x020097f8
	.4byte 0x020097f4
	.4byte 0x02008169
	.4byte 0x00003333
	.4byte 0x0059ffff
	.4byte 0x0400000e
	.4byte 0x0000fffc
	.2byte 0x000a
	.2byte 0x0400
.L_02000cec:
	bl 0x0200949c
	movs	r0, #1
	bl 0x0200931c
	movs	r0, #145
	bl 0x0200949c
	movs	r2, #191
	ldr	r3, [pc, #136]
	strh	r2, [r3, #0]
	movs	r5, #0
	ldr	r6, [pc, #132]
.L_02000d06:
	strh	r5, [r6, #0]
	movs	r0, #1
	adds	r5, #1
	bl 0x020093b4
	cmp	r5, #16
	ble.n	.L_02000d06
	movs	r0, #40
	bl 0x020093b4
	movs	r0, #1
	movs	r1, #1
	ldr	r2, [pc, #112]
	negs	r0, r0
	negs	r1, r1
	bl 0x0200939c
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #96]
	str	r3, [r2, #0]
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #88]
	str	r3, [r2, #0]
	ldr	r2, [pc, #88]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r6, [pc, #68]
	movs	r5, #16
.L_02000d4a:
	strh	r5, [r6, #0]
	movs	r0, #8
	subs	r5, #1
	bl 0x020093b4
	cmp	r5, #0
	bge.n	.L_02000d4a
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl 0x02009324
	movs	r0, #80
	bl 0x0200949c
	bl 0x02009494
	movs	r0, #20
	bl 0x020093b4
	bl 0x020093c4
	bl 0x02008430
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x04000050
	.4byte 0x04000054
	.4byte 0x0000e666
	.4byte 0x02009804
	.4byte 0x02009808
	.4byte 0x020097fc
	.2byte 0x80b1
	.2byte 0x0200
	.section .text.x02008f80,"ax",%progbits
	.align 2
	.global Engine_BuildScrollPage
	.thumb_func
Engine_BuildScrollPage:
	.global Func_02000f80
	.thumb_func
Func_02000f80:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #288]
	ldr	r0, [pc, #288]
	ldr	r6, [r3, #0]
	movs	r2, #14
	ldrsh	r1, [r0, r2]
	movs	r2, #240
	lsls	r2, r2, #4
.L_02000f9c:
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r2, #241
	lsls	r3, r3, #7
	lsls	r2, r2, #4
	adds	r5, r6, r3
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	subs	r2, #14
	lsls	r1, r1, #16
	sub	sp, #4
	mov	sl, r3
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	str	r1, [sp, #0]
	lsrs	r3, r1, #16
	ldr	r1, [pc, #240]
	adds	r2, r2, r3
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	adds	r4, r3, #0
	muls	r4, r2
	ldr	r2, [pc, #232]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	ldrh	r0, [r0, #12]
	mov	lr, r3
	ldr	r3, [pc, #228]
	mov	r9, r0
	movs	r0, #255
	movs	r7, #0
	mov	r8, r3
	mov	fp, r0
	mov	r1, fp
	asrs	r3, r4, #16
	ands	r3, r1
	ldr	r2, [pc, #212]
	lsls	r3, r3, #1
	ldrsh	r0, [r2, r3]
	mov	r1, lr
	mov	ip, pc
	bx	r8
	.4byte 0xda002800
	.4byte 0x020330ff
	.4byte 0x444b0c1b
	.4byte 0x802b3701
	.4byte 0x35044454
	.4byte 0xd1ea2fa0
	.4byte 0x011222f0
	.4byte 0x781b18b3
	.4byte 0x405a2201
	.4byte 0x1a9b0113
	.4byte 0x01db4829
	.4byte 0x1c9d18f3
	.4byte 0x681b1833
	.4byte 0x469a4927
	.4byte 0x881a1873
	.4byte 0x38089b00
	.4byte 0x18330c19
	.4byte 0x1852681b
	.4byte 0x43541c1c
	.4byte 0x18b34a22
	.4byte 0x469e681b
	.4byte 0x20ff4b1c
	.4byte 0x46982700
	.4byte 0x46834689
	.4byte 0x14234659
	.4byte 0x4a19400b
	.4byte 0x5ed0005b
	.4byte 0x00004671
	.4byte 0x474046fc
	.4byte 0xda002800
	.4byte 0x020330ff
	.4byte 0x444b0c1b
	.4byte 0x802b3701
	.4byte 0x35044454
	.4byte 0xd1e92fa0
	.4byte 0x18f24b11
	.4byte 0x20f08813
	.4byte 0x80133301
	.4byte 0x18310100
	.4byte 0x2201780b
	.4byte 0x700b4053
	.4byte 0xbce8b001
	.4byte 0x46a94698
	.4byte 0x46bb46b2
	.4byte 0xbc01bce0
	.4byte 0x00004700
	.4byte 0x03001ed8
	.4byte 0x03001ad0
	.4byte 0x00000f08
	.4byte 0x00000f18
	.4byte 0x03000118
	.4byte 0x020094c8
	.4byte 0x00000f14
	.4byte 0x00000f02
	.2byte 0x0f1c
	.2byte 0x0000
	.section .text.x020091c4,"ax",%progbits
	.align 2
	.global BabiFune_StepFade
	.thumb_func
BabiFune_StepFade:
	.global Func_020011c4
	.thumb_func
Func_020011c4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #208]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #204]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #204]
	ldrh	r3, [r3, #2]
	mov	lr, r2
	lsrs	r3, r3, #5
	mov	r1, lr
	mov	ip, r3
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	ldr	r0, [pc, #192]
	ldrh	r2, [r2, #0]
	cmp	r3, #0
	beq.n	.L_020011f0
	subs	r3, r2, #1
	mov	r2, lr
	strh	r3, [r2, #0]
.L_020011f0:
	movs	r5, #0
.L_020011f2:
	mov	r4, lr
	ldrh	r3, [r4, #0]
	lsls	r4, r3, #16
	asrs	r1, r4, #16
	negs	r3, r1
.L_020011fc:
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #0
	movs	r7, #255
	stmia	r0!, {r2}
	ands	r3, r7
	lsls	r2, r5, #21
	ldr	r6, [pc, #152]
	orrs	r3, r2
	orrs	r3, r6
	stmia	r0!, {r3}
	mov	r2, ip
	adds	r5, #1
	stmia	r0!, {r2}
	cmp	r5, #7
	bls.n	.L_020011f2
	lsrs	r3, r4, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r2, r3, #0
	adds	r2, #136
	ands	r2, r7
	movs	r5, #0
	movs	r7, #0
	adds	r4, r6, #0
	adds	r1, r0, #0
.L_02001232:
	lsls	r3, r5, #21
	orrs	r3, r2
	orrs	r3, r4
	str	r3, [r1, #4]
	adds	r5, #1
	mov	r3, ip
	str	r7, [r1, #0]
	str	r3, [r1, #8]
	adds	r0, #12
	adds	r1, #12
	cmp	r5, #7
	bls.n	.L_02001232
	ldr	r3, [pc, #84]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r2, #152
	movs	r3, #255
	ldr	r4, [pc, #72]
	movs	r5, #0
	ands	r2, r3
	movs	r6, #0
	adds	r1, r0, #0
.L_02001266:
	lsls	r3, r5, #21
	orrs	r3, r2
	orrs	r3, r4
	str	r3, [r1, #4]
	adds	r5, #1
	mov	r3, ip
	str	r6, [r1, #0]
	str	r3, [r1, #8]
	adds	r1, #12
	cmp	r5, #7
	bls.n	.L_02001266
	ldr	r6, [pc, #36]
	movs	r5, #0
.L_02001280:
	adds	r0, r6, #0
	movs	r1, #255
	adds	r5, #1
	bl 0x02009364
	adds	r6, #12
	cmp	r5, #23
	bls.n	.L_02001280
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x02009c1a
	.4byte 0x03001b10
	.4byte 0x02009c18
	.4byte 0x02009af8
	.2byte 0x4000
	.2byte 0x8000
	.section .text.x02008430,"ax",%progbits
	.balign 4
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x020093bc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl 0x0200944c
	movs r3, #18
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #3
	movs r3, #1
	movs r0, #18
	bl 0x0200938c
	ldr r0, [pc, #936]
	bl 0x0200941c
	movs r1, #10
	movs r3, #192
	movs r0, #1
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl 0x02009484
	movs r3, #192
	movs r0, #3
	movs r1, #0
	movs r2, #24
	lsls r3, r3, #8
	bl 0x02009484
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #16
	movs r1, #10
	movs r0, #2
	bl 0x02009484
	movs r0, #1
	bl 0x020093fc
	movs r0, #50
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #3
	bl 0x02009434
	movs r0, #40
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #2
	bl 0x02009434
	movs r0, #60
	bl 0x020093b4
	movs r0, #0
	movs r1, #3
	bl 0x02009404
	movs r0, #1
	movs r1, #3
	bl 0x02009404
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #60
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #30
	bl 0x020093b4
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x02009424
	movs r0, #0
	movs r1, #0
	bl 0x020093cc
	cmp r0, #0
	bne .L_02000430_0
	movs r0, #30
	bl 0x020093b4
	movs r1, #129
	movs r2, #50
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200943c
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
	ldr r3, [pc, #640]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000430_1
.L_02000430_0:
	movs r0, #30
	bl 0x020093b4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #50
	bl 0x0200943c
	ldr r3, [pc, #604]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
.L_02000430_1:
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r2, #50
	movs r0, #2
	ldr r1, [pc, #556]
	bl 0x0200943c
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #516]
	bl 0x0200943c
	movs r1, #2
	movs r0, #3
	bl 0x02009414
	movs r0, #30
	bl 0x020093b4
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	movs r2, #50
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200943c
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #2
	movs r0, #2
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r1, #4
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x02009424
	movs r0, #0
	movs r1, #0
	bl 0x020093cc
	cmp r0, #0
	bne .L_02000430_2
	movs r0, #20
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
	ldr r3, [pc, #308]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000430_3
.L_02000430_2:
	movs r0, #20
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	ldr r3, [pc, #252]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
.L_02000430_3:
	movs r0, #10
	bl 0x020093b4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #0
	bl 0x0200943c
	movs r0, #10
	bl 0x020093b4
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #20
	bl 0x020093b4
	movs r1, #3
	movs r0, #0
	bl 0x0200940c
	movs r0, #30
	bl 0x020093b4
	movs r0, #10
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x02009424
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #0
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r0, #0
	movs r1, #0
	bl 0x020093cc
	cmp r0, #0
	bne .L_02000430_4
	movs r0, #30
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r0, #3
	movs r1, #0
	bl 0x0200942c
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000430_5
	.4byte 0x000028fe
	.4byte 0x03001ebc
	.4byte 0x00000101
.L_02000430_4:
	movs r0, #30
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	ldr r3, [pc, #724]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #3
	movs r1, #0
	bl 0x0200942c
.L_02000430_5:
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x0200942c
	movs r0, #17
	bl 0x0200949c
	movs r0, #10
	bl 0x020093b4
	movs r0, #0
	movs r1, #3
	bl 0x02009404
	movs r0, #1
	movs r1, #3
	bl 0x02009404
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #40
	bl 0x020093b4
	movs r0, #0
	ldr r1, [pc, #532]
	ldr r2, [pc, #532]
	bl 0x020093dc
	movs r0, #1
	ldr r1, [pc, #520]
	ldr r2, [pc, #524]
	bl 0x020093dc
	movs r0, #2
	ldr r1, [pc, #512]
	ldr r2, [pc, #512]
	bl 0x020093dc
	ldr r2, [pc, #508]
	movs r0, #3
	ldr r1, [pc, #500]
	bl 0x020093dc
	ldr r1, [pc, #500]
	movs r0, #0
	bl 0x020093e4
	movs r0, #50
	bl 0x020093b4
	movs r0, #132
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200944c
	ldr r1, [pc, #472]
	movs r0, #1
	bl 0x020093e4
	movs r0, #50
	bl 0x020093b4
	ldr r1, [pc, #464]
	movs r0, #2
	bl 0x020093e4
	movs r0, #2
	bl 0x020093ec
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r2, #32
	movs r1, #0
	negs r2, r2
	movs r0, #3
	bl 0x0200948c
	movs r0, #30
	bl 0x020093b4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #3
	bl 0x02009434
	movs r0, #60
	bl 0x020093b4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	ldr r1, [pc, #364]
	movs r0, #3
	bl 0x020093e4
	movs r0, #3
	bl 0x020093ec
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r0, #216
	movs r1, #1
	movs r2, #168
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl 0x0200944c
	bl 0x02009454
	movs r0, #20
	bl 0x020093b4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #40
	bl 0x020093b4
	movs r0, #0
	movs r1, #3
	bl 0x02009404
	movs r0, #1
	movs r1, #3
	bl 0x02009404
	movs r0, #3
	movs r1, #3
	bl 0x02009404
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #30
	bl 0x020093b4
	movs r0, #67
	bl 0x0200949c
	movs r0, #240
	bl 0x020094a4
	bl 0x020092ac
	movs r0, #80
	bl 0x020093b4
	ldr r6, [pc, #192]
	movs r0, #8
	ldr r5, [r6]
	bl 0x020093d4
	movs r3, #131
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #48]
	movs r3, #142
	lsls r3, r3, #1
	adds r5, r5, r3
	ldr r3, [pc, #172]
	movs r0, #8
	str r3, [r5]
	movs r1, #60
	movs r2, #0
	bl 0x0200948c
	ldr r3, [pc, #160]
	movs r2, #0
	str r3, [r5]
	movs r1, #60
	movs r0, #8
	bl 0x0200948c
	movs r0, #80
	bl 0x020093b4
	movs r0, #100
	bl 0x020093b4
	ldr r0, [pc, #140]
	ldr r1, [pc, #140]
	bl 0x02009444
	movs r0, #202
	movs r1, #1
	movs r2, #168
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl 0x0200944c
	movs r0, #150
	lsls r0, r0, #1
	bl 0x020093b4
	ldr r1, [r6, #76]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	movs r2, #0
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #96
	str r3, [r2]
	bl 0x0200946c
	bl 0x02009474
	movs r0, #30
	bl 0x020093b4
	movs r0, #141
	lsls r0, r0, #1
	bl 0x020093ac
	ldr r0, [pc, #60]
	movs r1, #9
	bl 0x0200945c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x02009820
	.4byte 0x020098e0
	.4byte 0x0200998c
	.4byte 0x02009a4c
	.4byte 0x03001e70
	.4byte 0xffffd000
	.4byte 0xffffa000
	.4byte 0x00023333
	.4byte 0x0000028f
	.4byte 0x00000000
	.section .rodata.part1,"a",%progbits
	.global BabiFune_PaletteFrames
BabiFune_PaletteFrames:
	.4byte 0x69c05860
	.4byte 0x69c07f20
	.4byte 0x48005860
	.4byte 0x69c05860
	.4byte 0x69c07f20
	.4byte 0x00005860
	.global BabiFune_DriftScript
BabiFune_DriftScript:
	.4byte 0x0000001b
	.4byte 0x00640000
	.4byte 0x012d00c8
	.4byte 0x01f50191
	.4byte 0x02bc0259
	.4byte 0x0381031f
	.4byte 0x044403e3
	.4byte 0x050404a5
	.4byte 0x05c20563
	.4byte 0x067b061f
	.4byte 0x073106d7
	.4byte 0x07e2078a
	.4byte 0x088f0839
	.4byte 0x093608e3
	.4byte 0x09d70987
	.4byte 0x0a730a26
	.4byte 0x0b080abe
	.4byte 0x0b960b50
	.4byte 0x0c1d0bda
	.4byte 0x0c9d0c5e
	.4byte 0x0d140cd9
	.4byte 0x0d840d4d
	.4byte 0x0deb0db9
	.4byte 0x0e4a0e1c
	.4byte 0x0ea00e76
	.4byte 0x0eed0ec8
	.4byte 0x0f310f10
	.4byte 0x0f6b0f4f
	.4byte 0x0f9c0f85
	.4byte 0x0fc30fb1
	.4byte 0x0fe10fd3
	.4byte 0x0ff40fec
	.4byte 0x0ffe0ffb
	.4byte 0x0ffe1000
	.4byte 0x0ff40ffb
	.4byte 0x0fe10fec
	.4byte 0x0fc30fd3
	.4byte 0x0f9c0fb1
	.4byte 0x0f6b0f85
	.4byte 0x0f310f4f
	.4byte 0x0eed0f10
	.4byte 0x0ea00ec8
	.4byte 0x0e4a0e76
	.4byte 0x0deb0e1c
	.4byte 0x0d840db9
	.4byte 0x0d140d4d
	.4byte 0x0c9d0cd9
	.4byte 0x0c1d0c5e
	.4byte 0x0b960bda
	.4byte 0x0b080b50
	.4byte 0x0a730abe
	.4byte 0x09d70a26
	.4byte 0x09360987
	.4byte 0x088f08e3
	.4byte 0x07e20839
	.4byte 0x0731078a
	.4byte 0x067b06d7
	.4byte 0x05c2061f
	.4byte 0x05040563
	.4byte 0x044404a5
	.4byte 0x038103e3
	.4byte 0x02bc031f
	.4byte 0x01f50259
	.4byte 0x012d0191
	.4byte 0x006400c8
	.4byte 0xff9c0000
	.4byte 0xfed3ff38
	.4byte 0xfe0bfe6f
	.4byte 0xfd44fda7
	.4byte 0xfc7ffce1
	.4byte 0xfbbcfc1d
	.4byte 0xfafcfb5b
	.4byte 0xfa3efa9d
	.4byte 0xf985f9e1
	.4byte 0xf8cff929
	.4byte 0xf81ef876
	.4byte 0xf771f7c7
	.4byte 0xf6caf71d
	.4byte 0xf629f679
	.4byte 0xf58df5da
	.4byte 0xf4f8f542
	.4byte 0xf46af4b0
	.4byte 0xf3e3f426
	.4byte 0xf363f3a2
	.4byte 0xf2ecf327
	.4byte 0xf27cf2b3
	.4byte 0xf215f247
	.4byte 0xf1b6f1e4
	.4byte 0xf160f18a
	.4byte 0xf113f138
	.4byte 0xf0cff0f0
	.4byte 0xf095f0b1
	.4byte 0xf064f07b
	.4byte 0xf03df04f
	.4byte 0xf01ff02d
	.4byte 0xf00cf014
	.4byte 0xf002f005
	.4byte 0xf002f000
	.4byte 0xf00cf005
	.4byte 0xf01ff014
	.4byte 0xf03df02d
	.4byte 0xf064f04f
	.4byte 0xf095f07b
	.4byte 0xf0cff0b1
	.4byte 0xf113f0f0
	.4byte 0xf160f138
	.4byte 0xf1b6f18a
	.4byte 0xf215f1e4
	.4byte 0xf27cf247
	.4byte 0xf2ecf2b3
	.4byte 0xf363f327
	.4byte 0xf3e3f3a2
	.4byte 0xf46af426
	.4byte 0xf4f8f4b0
	.4byte 0xf58df542
	.4byte 0xf629f5da
	.4byte 0xf6caf679
	.4byte 0xf771f71d
	.4byte 0xf81ef7c7
	.4byte 0xf8cff876
	.4byte 0xf985f929
	.4byte 0xfa3ef9e1
	.4byte 0xfafcfa9d
	.4byte 0xfbbcfb5b
	.4byte 0xfc7ffc1d
	.4byte 0xfd44fce1
	.4byte 0xfe0bfda7
	.4byte 0xfed3fe6f
	.4byte 0xff9cff38
	.global BabiFune_SceneTableA
BabiFune_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x00000148
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_SceneTableB
BabiFune_SceneTableB:
	.4byte 0x000000bc
	.4byte 0x0011f0b4
	.4byte 0x000001ff
	.global BabiFune_SceneTableC
BabiFune_SceneTableC:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01f6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01f6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_SceneTableD
BabiFune_SceneTableD:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200804d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008071
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200807d
	.4byte 0x0000f204
	.4byte 0xffff000a
	.4byte 0x02008b35
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_Count
BabiFune_Count:
	.4byte 0x00000010
	.global BabiFune_CountTicks
BabiFune_CountTicks:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global BabiFune_StoredSlot0
BabiFune_StoredSlot0:
	.4byte 0xffff0000
	.global BabiFune_StoredRecord1
BabiFune_StoredRecord1:
	.4byte 0xffff0000
	.global BabiFune_StoredSlot3
BabiFune_StoredSlot3:
	.4byte 0xffff0000
	.global BabiFune_StoredRecord2
BabiFune_StoredRecord2:
	.4byte 0xffff0000
	.global BabiFune_PaletteStep
BabiFune_PaletteStep:
	.4byte 0x00000000
	.global BabiFune_ActionScriptA
BabiFune_ActionScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x020083c1
	.4byte 0x00000010
	.global BabiFune_ActionScriptB
BabiFune_ActionScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffd80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x020083dd
	.4byte 0x00000010
	.global BabiFune_ActionScriptC
BabiFune_ActionScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffec0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008415
	.4byte 0x00000010
	.global BabiFune_ActionScriptD
BabiFune_ActionScriptD:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x020083f9
	.4byte 0x00000010
	.section .bss,"aw",%nobits
	.space 288
	.global BabiFune_FadeStep
BabiFune_FadeStep:
	.space 2
	.global BabiFune_FadeSlot
BabiFune_FadeSlot:
	.space 2
