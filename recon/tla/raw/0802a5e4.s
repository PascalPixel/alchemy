.syntax unified
	.thumb
.L_0802a5e4:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r0, r5, #0
	bl	Func_08014dac
	ldr	r3, [pc, #68]
	ldr	r1, [pc, #72]
	adds	r2, r5, #0
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4d10
	adds	r0, r5, #0
	bl	Func_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #48]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #28]
	mov	r2, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	Func_08013164
	mov	r0, r8
	bl	Func_08013164
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x03000730
	.4byte 0x02010000
	.4byte 0x000000a0
	.4byte 0x08021704
	.2byte 0x8000
	.2byte 0x0201
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r3, [pc, #84]
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r5, r5, #2
	lsls	r0, r0, #2
	mov	sl, r1
	adds	r5, r5, r3
	bl	Func_08014d78
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r6, r0, #0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldrh	r0, [r5, #2]
	ldr	r3, [pc, #56]
	mov	r8, r1
	adds	r0, r0, r3
	bl	Resource_GetTableEntry
	adds	r1, r6, #0
	bl	Func_0801587c
	mov	r3, r8
	strh	r3, [r6, #0]
	movs	r2, #132
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r6, #0
	mov	r1, sl
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r6, #0
	bl	Func_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0802f380
	.2byte 0x026c
	.2byte 0x0000
	.global Func_0802a6b8
	.thumb_func
Func_0802a6b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #193
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	adds	r5, r0, #0
	strh	r3, [r1, #0]
	movs	r0, #0
	sub	sp, #8
	bl	Func_08013eb4
	ldr	r2, [pc, #532]
	lsls	r3, r5, #1
	adds	r3, r3, r5
	movs	r5, #216
	lsls	r5, r5, #1
	lsls	r3, r3, #2
	adds	r3, r3, r2
	adds	r1, r5, #0
	movs	r0, #32
	str	r3, [sp, #0]
	bl	Runtime_AllocateBlock
	adds	r1, r5, #0
	ldr	r3, [pc, #512]
	mov	r8, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a00
	ldr	r3, [pc, #504]
	ldrh	r0, [r2, #0]
	adds	r0, r0, r3
	bl	Resource_GetTableEntry
	adds	r7, r0, #0
	ldr	r3, [r7, #36]
	ldr	r1, [pc, #496]
	adds	r0, r7, r3
	bl	Func_0801587c
	bl	Tilemap_DecodeStagedBuffer
	movs	r3, #1
	add	r0, sp, #4
	negs	r3, r3
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #472]
	ldr	r2, [pc, #476]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r7, #40]
	adds	r0, r7, r3
	bl	Func_0801587c
	ldr	r3, [r7, #44]
	ldr	r1, [pc, #464]
	adds	r0, r7, r3
	bl	Func_0801587c
	bl	.L_0802a5e4
	ldr	r3, [r7, #48]
	ldr	r1, [pc, #452]
	adds	r0, r7, r3
	bl	Func_0801587c
	ldr	r0, [r7, #52]
	cmp	r0, #0
	beq.n	.L_0802a76a
	ldr	r5, [pc, #444]
	adds	r0, r7, r0
	adds	r1, r5, #0
	bl	Func_0801587c
	adds	r0, r5, #0
	bl	Func_0802cc9c
.L_0802a76a:
	ldr	r0, [r7, #56]
	cmp	r0, #0
	beq.n	.L_0802a780
	ldr	r5, [pc, #424]
	adds	r0, r7, r0
	adds	r1, r5, #0
	bl	Func_0801587c
	adds	r0, r5, #0
	bl	Func_0802ce4c
.L_0802a780:
	ldr	r3, [r7, #60]
	ldr	r1, [pc, #412]
	adds	r0, r7, r3
	bl	Func_0801587c
	ldrb	r3, [r7, #0]
	mov	r2, r8
	adds	r2, #236
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	adds	r2, #4
	ldrb	r3, [r7, #1]
	mov	r1, r8
	lsls	r3, r3, #19
	str	r3, [r2, #0]
	adds	r1, #244
	ldrb	r3, [r7, #2]
	mov	r0, r8
	lsls	r3, r3, #19
	str	r3, [r1, #0]
	adds	r0, #248
	ldrb	r3, [r7, #3]
	lsls	r3, r3, #19
	str	r3, [r0, #0]
	movs	r3, #228
	add	r3, r8
	mov	fp, r3
	mov	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #232
	add	r2, r8
	str	r3, [r2, #0]
	mov	r9, r2
	movs	r3, #130
	ldrb	r2, [r7, #4]
	lsls	r3, r3, #1
	add	r3, r8
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #5]
	movs	r2, #6
	adds	r2, #255
	add	r2, r8
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #6]
	movs	r2, #131
	lsls	r2, r2, #1
	add	r2, r8
	strb	r3, [r2, #0]
	movs	r2, #2
	adds	r2, #255
	movs	r3, #2
	add	r2, r8
	strb	r3, [r2, #0]
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_0802a7f8
	movs	r3, #128
	lsls	r3, r3, #20
	str	r3, [r1, #0]
.L_0802a7f8:
	ldr	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_0802a804
	movs	r3, #128
	lsls	r3, r3, #20
	str	r3, [r0, #0]
.L_0802a804:
	movs	r5, #132
	lsls	r5, r5, #1
	adds	r6, r7, #0
	movs	r3, #2
	add	r5, r8
	adds	r6, #12
	mov	sl, r3
.L_0802a812:
	ldrb	r4, [r6, #0]
	ldrb	r0, [r6, #1]
	lsls	r3, r4, #19
	str	r3, [r5, #8]
	lsls	r3, r0, #19
	str	r3, [r5, #12]
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	ldrb	r2, [r6, #6]
	lsls	r3, r3, #12
	str	r3, [r5, #20]
	movs	r3, #4
	ldrsb	r3, [r6, r3]
	lsrs	r0, r0, #1
	lsls	r3, r3, #12
	str	r3, [r5, #24]
	movs	r3, #5
	ldrsb	r3, [r6, r3]
	lsrs	r4, r4, #1
	lsls	r3, r3, #12
	str	r3, [r5, #28]
	movs	r3, #127
	ands	r3, r2
	ldrb	r2, [r6, #7]
	strh	r3, [r5, #40]
	movs	r3, #127
	ands	r3, r2
	movs	r2, #0
	str	r2, [r5, #32]
	str	r2, [r5, #36]
	lsls	r0, r0, #7
	ldr	r2, [pc, #188]
	strh	r0, [r5, #46]
	movs	r1, #2
	ldrsb	r1, [r6, r1]
	adds	r0, r0, r4
	lsls	r3, r3, #7
	strh	r3, [r5, #42]
	lsls	r3, r0, #2
	adds	r3, r3, r2
	lsls	r1, r1, #12
	str	r3, [r5, #48]
	ldr	r3, [pc, #172]
	str	r1, [r5, #16]
	strh	r4, [r5, #44]
	adds	r0, r0, r3
	mov	r2, fp
	str	r0, [r5, #52]
	ldr	r3, [pc, #176]
	ldr	r0, [r2, #0]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68ab
	mov	r2, r9
	adds	r0, r0, r3
	str	r0, [r5, #0]
	ldr	r1, [r5, #20]
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #156]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x68eb
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	adds	r0, r0, r3
	mov	r3, sl
	str	r0, [r5, #4]
	adds	r6, #8
	adds	r5, #56
	cmp	r3, #0
	bge.n	.L_0802a812
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r2, r8
	strh	r3, [r2, #20]
	movs	r1, #130
	lsls	r1, r1, #1
	add	r1, r8
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0802a8bc
	movs	r3, #192
	lsls	r3, r3, #5
	strh	r3, [r2, #20]
.L_0802a8bc:
	movs	r0, #6
	adds	r0, #255
	add	r0, r8
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0802a8d4
	mov	r2, r8
	ldrh	r3, [r2, #20]
	ldr	r2, [pc, #32]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r2, #20]
.L_0802a8d4:
	movs	r3, #131
	lsls	r3, r3, #1
	add	r3, r8
	mov	ip, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802a928
	mov	r2, r8
	ldrh	r3, [r2, #20]
	ldr	r2, [pc, #12]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r2, #20]
	b.n	.L_0802a928
	.4byte 0x00000400
	.4byte 0x00000200
	.4byte 0x0802f380
	.4byte 0x03000258
	.4byte 0x0000026c
	.4byte 0x02010001
	.4byte 0x0202c000
	.4byte 0x85000800
	.4byte 0x02010000
	.4byte 0x02024000
	.4byte 0x0202d000
	.4byte 0x0202de00
	.4byte 0x0202e000
	.2byte 0x021c
	.2byte 0x0300
.L_0802a928:
	ldrb	r3, [r7, #7]
	ldrb	r2, [r1, #0]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #160
	lsls	r3, r3, #3
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #14
	strh	r2, [r3, #0]
	ldrb	r2, [r0, #0]
	ldrb	r3, [r7, #8]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #192
	lsls	r3, r3, #3
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #12
	strh	r2, [r3, #0]
	mov	r3, ip
	ldrb	r2, [r3, #0]
	ldrb	r3, [r7, #9]
	lsls	r3, r3, #2
	orrs	r2, r3
	movs	r3, #224
	lsls	r3, r3, #3
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #10
	strh	r2, [r3, #0]
	movs	r5, #184
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	GameFlag_TestFar
	cmp	r0, #0
	beq.n	.L_0802a982
	adds	r0, r5, #0
	bl	GameFlag_ClearBitFar
	b.n	.L_0802aa24
.L_0802a982:
	movs	r2, #128
	lsls	r2, r2, #7
	mov	sl, r2
	mov	r0, sl
	bl	Func_08014d78
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0802aa24
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	mov	r8, r3
	ldr	r3, [sp, #0]
	ldr	r6, [pc, #180]
	ldrh	r0, [r3, #2]
	adds	r0, r0, r6
	bl	Resource_GetTableEntry
	adds	r1, r7, #0
	bl	Func_0801587c
	movs	r2, #224
	adds	r1, r7, #0
	strh	r5, [r7, #0]
	lsls	r2, r2, #1
	ldr	r5, [pc, #160]
	mov	r0, r8
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9a00
	ldrh	r0, [r2, #4]
	adds	r0, r0, r6
	bl	Resource_GetTableEntry
	adds	r1, r7, #0
	bl	0x080158cc
	mov	r2, sl
	adds	r1, r7, #0
	ldr	r0, [pc, #136]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b00
	ldrh	r0, [r3, #6]
	adds	r0, r0, r6
	bl	Resource_GetTableEntry
	adds	r1, r7, #0
	bl	0x080158cc
	adds	r1, r7, #0
	mov	r2, sl
	ldr	r0, [pc, #116]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9a00
	ldrh	r0, [r2, #8]
	adds	r0, r0, r6
	bl	Resource_GetTableEntry
	adds	r1, r7, #0
	bl	0x080158cc
	adds	r1, r7, #0
	mov	r2, sl
	ldr	r0, [pc, #92]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b00
	ldrh	r0, [r3, #10]
	adds	r0, r0, r6
	bl	Resource_GetTableEntry
	ldr	r1, [pc, #80]
	bl	0x080158cc
	adds	r0, r7, #0
	bl	Func_08013164
.L_0802aa24:
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #0
	adds	r3, #76
	strh	r2, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #1
	subs	r3, #80
	strh	r2, [r3, #0]
	ldr	r0, [pc, #52]
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #133
	bl	Func_080145a8
	movs	r0, #2
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000026c
	.4byte 0x03000730
	.4byte 0x06004000
	.4byte 0x06008000
	.4byte 0x0600c000
	.4byte 0x02028000
	.2byte 0xad85
	.2byte 0x0802
	.global Func_0802aa74
	.thumb_func
Func_0802aa74:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #0
	ldr	r1, [r3, #0]
	sub	sp, #4
	mov	lr, r0
	mov	r8, r0
	mov	ip, r0
	str	r1, [sp, #0]
	cmp	r1, #0
	bne.n	.L_0802aa96
	b.n	.L_0802ab98
.L_0802aa96:
	ldmia	r1!, {r2}
	adds	r3, #252
	mov	lr, r2
	adds	r2, r1, #0
	str	r2, [sp, #0]
	ldmia	r1!, {r5}
	mov	r8, r5
	adds	r5, r1, #0
	str	r5, [sp, #0]
	ldmia	r1!, {r7}
	mov	ip, r7
	adds	r7, r1, #0
	str	r7, [sp, #0]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802ab98
	adds	r2, r3, #0
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	movs	r1, #1
	negs	r1, r1
	ldrh	r4, [r2, #0]
	cmp	r3, r1
	beq.n	.L_0802ab88
	mov	sl, r1
.L_0802aac8:
	ldr	r5, [pc, #232]
	lsls	r3, r4, #16
	adds	r1, r3, r5
	movs	r7, #2
	ldrsh	r3, [r2, r7]
	ldr	r0, [pc, #228]
	lsls	r3, r3, #16
	add	r3, r8
	adds	r7, r3, r0
	movs	r5, #4
	ldrsh	r3, [r2, r5]
	movs	r0, #240
	lsls	r3, r3, #16
	lsls	r0, r0, #15
	adds	r4, r3, r0
	movs	r5, #6
	ldrsh	r3, [r2, r5]
	movs	r5, #192
	lsls	r3, r3, #16
	add	r3, r8
	lsls	r5, r5, #15
	adds	r0, r3, r5
	cmp	lr, r1
	blt.n	.L_0802ab7c
	cmp	ip, r7
	blt.n	.L_0802ab7c
	cmp	lr, r4
	bgt.n	.L_0802ab7c
	cmp	ip, r0
	bgt.n	.L_0802ab7c
	adds	r5, r1, #0
	mov	r1, lr
	subs	r6, r5, r1
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_0802ab12
	subs	r2, r1, r5
.L_0802ab12:
	mov	r1, lr
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_0802ab20
	cmp	r2, r3
	bgt.n	.L_0802ab28
	b.n	.L_0802ab2e
.L_0802ab20:
	mov	r1, lr
	subs	r3, r1, r4
	cmp	r2, r3
	ble.n	.L_0802ab2e
.L_0802ab28:
	adds	r5, r4, #0
	mov	r2, lr
	subs	r6, r5, r2
.L_0802ab2e:
	adds	r1, r7, #0
	mov	r3, ip
	subs	r4, r1, r3
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_0802ab3c
	subs	r2, r3, r1
.L_0802ab3c:
	mov	r7, ip
	subs	r3, r0, r7
	cmp	r3, #0
	blt.n	.L_0802ab4a
	cmp	r2, r3
	bgt.n	.L_0802ab52
	b.n	.L_0802ab58
.L_0802ab4a:
	mov	r7, ip
	subs	r3, r7, r0
	cmp	r2, r3
	ble.n	.L_0802ab58
.L_0802ab52:
	adds	r1, r0, #0
	mov	r0, ip
	subs	r4, r1, r0
.L_0802ab58:
	adds	r2, r6, #0
	cmp	r2, #0
	bge.n	.L_0802ab62
	mov	r3, lr
	subs	r2, r3, r5
.L_0802ab62:
	cmp	r4, #0
	blt.n	.L_0802ab6c
	cmp	r2, r4
	ble.n	.L_0802ab74
	b.n	.L_0802ab78
.L_0802ab6c:
	mov	r7, ip
	subs	r3, r7, r1
	cmp	r2, r3
	bgt.n	.L_0802ab78
.L_0802ab74:
	mov	lr, r5
	b.n	.L_0802ab88
.L_0802ab78:
	mov	ip, r1
	b.n	.L_0802ab88
.L_0802ab7c:
	adds	r2, #8
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r4, [r2, #0]
	cmp	r3, sl
	bne.n	.L_0802aac8
.L_0802ab88:
	ldr	r3, [sp, #0]
	mov	r1, lr
	subs	r3, #12
	str	r1, [r3, #0]
	ldr	r3, [sp, #0]
	mov	r2, ip
	subs	r3, #4
	str	r2, [r3, #0]
.L_0802ab98:
	mov	r3, ip
	mov	r5, r8
	subs	r1, r3, r5
	mov	r0, lr
	bl	Func_0802af9c
	bl	.L_0802ad84
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xff880000
	.2byte 0x0000
	.2byte 0xffc0
.L_0802abbc:
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #184]
	adds	r6, r1, #0
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	adds	r1, r1, r3
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r1, r1, r3
	ldr	r3, [pc, #160]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	mov	sl, r3
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	movs	r0, #127
	asrs	r3, r3, #1
	ands	r3, r0
	mov	lr, r0
	movs	r0, #30
	ands	r2, r0
	lsls	r2, r2, #5
	ldrh	r4, [r1, #42]
	mov	r8, r2
	movs	r2, #254
	lsls	r2, r2, #6
	ldrh	r5, [r1, #46]
	lsls	r7, r3, #7
	cmp	r4, r2
	beq.n	.L_0802ac16
	ldr	r2, [pc, #116]
	lsls	r3, r5, #2
	subs	r7, r7, r5
	adds	r2, r2, r3
	ands	r7, r4
	mov	ip, r2
.L_0802ac16:
	lsrs	r3, r6, #31
	ldrh	r5, [r1, #40]
	adds	r3, r6, r3
	asrs	r4, r3, #1
	mov	r3, lr
	ands	r4, r3
	ldrh	r1, [r1, #44]
	ands	r0, r6
	ands	r4, r5
	cmp	r5, #127
	beq.n	.L_0802ac34
	subs	r4, r4, r1
	lsls	r3, r1, #2
	ands	r4, r5
	add	ip, r3
.L_0802ac34:
	movs	r2, #30
	movs	r6, #0
	mov	lr, r2
.L_0802ac3a:
	adds	r3, r7, r4
	lsls	r3, r3, #2
	mov	r2, ip
	ldr	r1, [r3, r2]
	ldr	r2, [pc, #68]
	lsls	r1, r1, #21
	lsrs	r1, r1, #18
	adds	r3, r1, r2
	mov	r2, r8
	adds	r2, r2, r0
	lsls	r2, r2, #1
	ldr	r3, [r3, #0]
	mov	r9, r2
	add	r9, sl
	mov	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #48]
	adds	r4, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	mov	r2, r9
	str	r3, [r2, #64]
	adds	r0, #2
	mov	r3, lr
	adds	r6, #1
	ands	r4, r5
	ands	r0, r3
	cmp	r6, #15
	bls.n	.L_0802ac3a
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x06002800
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
.L_0802ac90:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #212]
	adds	r7, r1, #0
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #196]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	movs	r6, #132
	lsls	r6, r6, #1
	mov	r9, r3
	lsrs	r3, r2, #31
	adds	r1, r1, r6
	adds	r3, r2, r3
	movs	r0, #127
	ldrh	r6, [r1, #42]
	asrs	r3, r3, #1
	ands	r3, r0
	mov	sl, r0
	lsls	r4, r3, #7
	movs	r0, #254
	movs	r3, #30
	mov	lr, r6
	ands	r2, r3
	lsls	r0, r0, #6
	ldrh	r6, [r1, #46]
	mov	ip, r3
	lsls	r5, r2, #5
	cmp	lr, r0
	beq.n	.L_0802acf2
	subs	r4, r4, r6
	lsls	r3, r6, #2
	ldr	r6, [pc, #136]
	mov	r2, lr
	adds	r6, r6, r3
	ands	r4, r2
	mov	r8, r6
.L_0802acf2:
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	ldrh	r2, [r1, #40]
	asrs	r0, r3, #1
	mov	fp, r7
	mov	r3, sl
	ands	r0, r3
	mov	r6, fp
	mov	r3, ip
	ands	r6, r3
	ldrh	r1, [r1, #44]
	mov	fp, r6
	ands	r0, r2
	cmp	r2, #127
	beq.n	.L_0802ad18
	subs	r0, r0, r1
	lsls	r3, r1, #2
	ands	r0, r2
	add	r8, r3
.L_0802ad18:
	movs	r6, #1
	mov	ip, r6
	mov	r2, ip
	movs	r3, #240
	ands	r2, r7
	lsls	r3, r3, #2
	mov	ip, r2
	movs	r7, #0
	mov	sl, r3
.L_0802ad2a:
	adds	r3, r4, r0
	lsls	r3, r3, #2
	mov	r6, r8
	ldr	r1, [r3, r6]
	ldr	r3, [pc, #72]
	lsls	r1, r1, #21
	lsrs	r1, r1, #19
	add	r1, ip
	lsls	r1, r1, #1
	mov	r6, fp
	adds	r2, r1, r3
	adds	r3, r5, r6
	ldrh	r2, [r2, #0]
	add	r3, ip
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r6, [pc, #52]
	strh	r2, [r3, #0]
	adds	r2, r1, r6
	ldrh	r2, [r2, #0]
	adds	r3, #64
	strh	r2, [r3, #0]
	adds	r4, #128
	mov	r2, lr
	adds	r5, #64
	mov	r3, sl
	adds	r7, #1
	ands	r4, r2
	ands	r5, r3
	cmp	r7, #10
	bls.n	.L_0802ad2a
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x06002800
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
.L_0802ad84:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r6, #132
	ldr	r1, [r3, #0]
	mov	r8, r3
	lsls	r6, r6, #1
	sub	sp, #4
	add	r6, r8
	cmp	r1, #0
	bne.n	.L_0802adaa
	b.n	.L_0802af70
.L_0802adaa:
	ldmia	r1!, {r3}
	ldr	r2, [pc, #464]
	ldr	r5, [pc, #468]
	adds	r2, r2, r3
	mov	sl, r2
	ldmia	r1!, {r2}
	ldr	r3, [r1, #0]
	mov	r1, r8
	subs	r3, r3, r2
	adds	r7, r3, r5
	mov	r3, r8
	adds	r3, #236
	ldr	r1, [r1, #4]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #448]
	adds	r0, r3, r1
	mov	r3, r8
	adds	r3, #244
	ldr	r3, [r3, #0]
	mov	ip, r1
	subs	r3, r3, r1
	adds	r1, r3, r2
	mov	r3, r8
	mov	r5, r8
	adds	r3, #240
	ldr	r4, [r5, #8]
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #424]
	adds	r2, r3, r4
	mov	r3, r8
	adds	r3, #248
	ldr	r3, [r3, #0]
	subs	r3, r3, r4
	adds	r3, r3, r5
	cmp	r0, r1
	ble.n	.L_0802adf4
	adds	r1, r0, #0
.L_0802adf4:
	cmp	r2, r3
	ble.n	.L_0802adfa
	adds	r3, r2, #0
.L_0802adfa:
	cmp	sl, r0
	bge.n	.L_0802ae00
	mov	sl, r0
.L_0802ae00:
	cmp	sl, r1
	ble.n	.L_0802ae06
	mov	sl, r1
.L_0802ae06:
	cmp	r7, r2
	bge.n	.L_0802ae0c
	adds	r7, r2, #0
.L_0802ae0c:
	cmp	r7, r3
	ble.n	.L_0802ae12
	adds	r7, r3, #0
.L_0802ae12:
	mov	r1, ip
	cmp	r1, #0
	beq.n	.L_0802ae42
	bl	Func_08014878
	adds	r5, r0, #0
	bl	Func_08014878
	ldr	r3, [pc, #364]
	subs	r5, r5, r0
	mov	r2, r8
	adds	r1, r5, #0
	ldr	r0, [r2, #4]
	mov	r9, r3
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4645
	add	sl, r0
	ldr	r1, [r5, #12]
	ldr	r0, [r5, #4]
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x68ac
	str	r0, [r5, #4]
.L_0802ae42:
	cmp	r4, #0
	beq.n	.L_0802ae70
	bl	Func_08014878
	adds	r5, r0, #0
	bl	Func_08014878
	ldr	r2, [pc, #316]
	subs	r5, r5, r0
	mov	r1, r8
	ldr	r0, [r1, #8]
	mov	r9, r2
	adds	r1, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4643
	adds	r7, r7, r0
	ldr	r1, [r3, #12]
	ldr	r0, [r3, #8]
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x4645
	str	r0, [r5, #8]
.L_0802ae70:
	movs	r1, #232
	add	r1, r8
	ldr	r2, [r1, #0]
	movs	r5, #128
	subs	r3, r2, r7
	lsls	r5, r5, #13
	mov	r9, r1
	cmp	r3, r5
	ble.n	.L_0802ae88
	ldr	r1, [pc, #272]
	adds	r7, r2, r1
	subs	r3, r2, r7
.L_0802ae88:
	ldr	r5, [pc, #264]
	cmp	r3, r5
	bge.n	.L_0802ae94
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r7, r2, r1
.L_0802ae94:
	movs	r2, #228
	add	r2, r8
	mov	r3, sl
	mov	r5, r9
	str	r3, [r2, #0]
	str	r7, [r5, #0]
	movs	r1, #0
	mov	fp, r2
	mov	r8, r1
.L_0802aea6:
	mov	r2, fp
	ldr	r0, [r2, #0]
	ldr	r1, [r6, #16]
	ldr	r3, [pc, #224]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464d
	ldr	r2, [pc, #216]
	mov	sl, r0
	ldr	r1, [r6, #20]
	ldr	r0, [r5, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x69b2
	adds	r7, r0, #0
	cmp	r2, #0
	beq.n	.L_0802aed0
	ldr	r3, [r6, #32]
	adds	r3, r3, r2
	str	r3, [r6, #32]
	add	sl, r3
.L_0802aed0:
	ldr	r2, [r6, #28]
	cmp	r2, #0
	beq.n	.L_0802aede
	ldr	r3, [r6, #36]
	adds	r3, r3, r2
	str	r3, [r6, #36]
	adds	r7, r7, r3
.L_0802aede:
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	add	sl, r3
	ldr	r3, [r6, #12]
	mov	r2, sl
	adds	r7, r7, r3
	mov	r3, sl
	lsrs	r4, r3, #19
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r2
	lsrs	r5, r7, #19
	cmp	r3, #0
	beq.n	.L_0802af1c
	cmp	r1, sl
	bge.n	.L_0802af0e
	adds	r1, r4, #0
	adds	r1, #30
	mov	r0, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
	b.n	.L_0802af16
.L_0802af0e:
	adds	r1, r4, #0
	mov	r0, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
.L_0802af16:
	bl	.L_0802ac90
	ldr	r4, [sp, #0]
.L_0802af1c:
	ldr	r1, [r6, #4]
	movs	r2, #128
	adds	r3, r1, #0
	eors	r3, r7
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802af48
	cmp	r1, r7
	bge.n	.L_0802af3e
	adds	r2, r5, #0
	adds	r2, #20
	mov	r0, r8
	adds	r1, r4, #0
	bl	.L_0802abbc
	b.n	.L_0802af48
.L_0802af3e:
	mov	r0, r8
	adds	r1, r4, #0
	adds	r2, r5, #0
	bl	.L_0802abbc
.L_0802af48:
	mov	r3, sl
	mov	r5, r8
	str	r3, [r6, #0]
	movs	r3, #3
	subs	r3, r3, r5
	ldr	r5, [pc, #68]
	mov	r1, sl
	asrs	r2, r1, #16
	lsls	r3, r3, #2
	movs	r1, #1
	strh	r2, [r5, r3]
	add	r8, r1
	asrs	r2, r7, #16
	adds	r3, r3, r5
	strh	r2, [r3, #2]
	mov	r2, r8
	str	r7, [r6, #4]
	adds	r6, #56
	cmp	r2, #2
	bls.n	.L_0802aea6
.L_0802af70:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xff880000
	.4byte 0xffa00000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x0300021c
	.4byte 0xfff00000
	.2byte 0x1120
	.2byte 0x0300
	.global Func_0802af9c
	.thumb_func
Func_0802af9c:
.L_0802af9c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	mov	r8, r0
	adds	r0, r1, #0
	movs	r1, #132
	sub	sp, #40
	lsls	r1, r1, #1
	str	r3, [sp, #24]
	adds	r7, r3, r1
	ldr	r3, [pc, #440]
	ldr	r2, [pc, #440]
	adds	r0, r0, r3
	ldr	r3, [sp, #24]
	add	r8, r2
	adds	r3, #236
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L_0802afd2
	mov	r8, r3
.L_0802afd2:
	ldr	r3, [sp, #24]
	ldr	r4, [pc, #424]
	adds	r3, #244
	ldr	r3, [r3, #0]
	adds	r3, r3, r4
	cmp	r8, r3
	ble.n	.L_0802afe2
	mov	r8, r3
.L_0802afe2:
	ldr	r3, [sp, #24]
	adds	r3, #240
	ldr	r3, [r3, #0]
	cmp	r0, r3
	bge.n	.L_0802afee
	adds	r0, r3, #0
.L_0802afee:
	ldr	r3, [sp, #24]
	ldr	r1, [pc, #400]
	adds	r3, #248
	ldr	r3, [r3, #0]
	adds	r3, r3, r1
	cmp	r0, r3
	ble.n	.L_0802affe
	adds	r0, r3, #0
.L_0802affe:
	ldr	r2, [sp, #24]
	mov	r3, r8
	adds	r2, #228
	str	r2, [sp, #16]
	str	r3, [r2, #0]
	ldr	r4, [sp, #24]
	movs	r1, #0
	adds	r4, #232
	str	r4, [sp, #12]
	str	r0, [r4, #0]
	str	r1, [sp, #36]
.L_0802b014:
	ldr	r2, [sp, #36]
	movs	r4, #130
	ldr	r1, [sp, #24]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_0802b026
	b.n	.L_0802b15e
.L_0802b026:
	ldr	r2, [sp, #16]
	ldr	r1, [r7, #16]
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #344]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c03
	ldr	r2, [pc, #336]
	mov	r8, r0
	ldr	r1, [r7, #20]
	ldr	r0, [r4, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x69ba
	cmp	r2, #0
	beq.n	.L_0802b04e
	ldr	r3, [r7, #32]
	adds	r3, r3, r2
	str	r3, [r7, #32]
	add	r8, r3
.L_0802b04e:
	ldr	r2, [r7, #28]
	cmp	r2, #0
	beq.n	.L_0802b05c
	ldr	r3, [r7, #36]
	adds	r3, r3, r2
	str	r3, [r7, #36]
	adds	r0, r0, r3
.L_0802b05c:
	ldr	r3, [r7, #8]
	add	r8, r3
	ldr	r3, [r7, #12]
	mov	r1, r8
	adds	r0, r0, r3
	cmp	r1, #0
	bge.n	.L_0802b06e
	ldr	r1, [pc, #288]
	add	r1, r8
.L_0802b06e:
	asrs	r3, r1, #19
	mov	r8, r3
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L_0802b07c
	ldr	r4, [pc, #272]
	adds	r2, r0, r4
.L_0802b07c:
	ldr	r4, [sp, #36]
	asrs	r0, r2, #19
	lsls	r3, r4, #11
	ldr	r4, [pc, #268]
	adds	r4, r3, r4
	str	r4, [sp, #32]
	ldrh	r3, [r7, #42]
	ldrh	r4, [r7, #40]
	mov	sl, r3
	ldrh	r3, [r7, #44]
	mov	lr, r4
	ldr	r4, [pc, #256]
	ldrh	r5, [r7, #46]
	str	r3, [sp, #20]
	mov	r3, lr
	str	r4, [sp, #28]
	cmp	r3, #127
	beq.n	.L_0802b0aa
	ldr	r4, [sp, #20]
	lsls	r3, r4, #2
	ldr	r4, [pc, #236]
	adds	r4, r3, r4
	str	r4, [sp, #28]
.L_0802b0aa:
	movs	r3, #254
	lsls	r3, r3, #6
	mov	ip, r3
	cmp	sl, ip
	beq.n	.L_0802b0bc
	ldr	r4, [sp, #28]
	lsls	r3, r5, #2
	adds	r4, r4, r3
	str	r4, [sp, #28]
.L_0802b0bc:
	lsrs	r3, r2, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	movs	r2, #127
	ands	r3, r2
	lsls	r4, r3, #7
	movs	r3, #30
	ands	r3, r0
	lsls	r6, r3, #5
	cmp	sl, ip
	beq.n	.L_0802b0d8
	subs	r4, r4, r5
	mov	r2, sl
	ands	r4, r2
.L_0802b0d8:
	movs	r3, #0
	mov	r9, r3
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	str	r3, [sp, #8]
.L_0802b0e4:
	ldr	r1, [sp, #8]
	movs	r0, #127
	movs	r5, #30
	mov	r2, r8
	mov	r3, lr
	ands	r0, r1
	ands	r5, r2
	cmp	r3, #127
	beq.n	.L_0802b0fc
	ldr	r1, [sp, #20]
	subs	r0, r0, r1
	ands	r0, r3
.L_0802b0fc:
	movs	r2, #0
	movs	r3, #30
	mov	ip, r2
	mov	fp, r3
.L_0802b104:
	ldr	r2, [sp, #28]
	adds	r3, r4, r0
	lsls	r3, r3, #2
	ldr	r1, [r3, r2]
	ldr	r2, [pc, #136]
	lsls	r1, r1, #21
	lsrs	r1, r1, #18
	str	r1, [sp, #4]
	adds	r3, r1, r2
	ldr	r1, [sp, #32]
	adds	r2, r6, r5
	lsls	r2, r2, #1
	adds	r2, r2, r1
	str	r2, [sp, #0]
	ldr	r1, [pc, #120]
	ldr	r3, [r3, #0]
	adds	r0, #1
	str	r3, [r2, #0]
	ldr	r2, [sp, #4]
	adds	r5, #2
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #0]
	mov	r1, fp
	str	r3, [r2, #64]
	movs	r2, #1
	mov	r3, lr
	add	ip, r2
	ands	r0, r3
	mov	r3, ip
	ands	r5, r1
	cmp	r3, #15
	bls.n	.L_0802b104
	movs	r3, #240
	add	r9, r2
	adds	r4, #128
	mov	r1, sl
	adds	r6, #64
	lsls	r3, r3, #2
	mov	r2, r9
	ands	r4, r1
	ands	r6, r3
	cmp	r2, #10
	bls.n	.L_0802b0e4
	adds	r7, #56
.L_0802b15e:
	ldr	r3, [sp, #36]
	adds	r3, #1
	str	r3, [sp, #36]
	cmp	r3, #2
	bhi.n	.L_0802b16a
	b.n	.L_0802b014
.L_0802b16a:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffa00000
	.4byte 0xff880000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x0300021c
	.4byte 0x0007ffff
	.4byte 0x06002800
	.4byte 0x02010000
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
	.global Func_0802b1a0
	.thumb_func
Func_0802b1a0:
.L_0802b1a0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	mov	fp, r2
	lsls	r1, r1, #7
	ldr	r2, [pc, #260]
	lsls	r3, r4, #7
	adds	r1, r1, r0
	add	r3, fp
	lsls	r1, r1, #2
	lsls	r3, r3, #2
	sub	sp, #36
	adds	r3, r3, r2
	adds	r1, r1, r2
	str	r3, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r2, r3, r0
	add	r0, sp, #12
	mov	r9, r0
	movs	r6, #2
.L_0802b1dc:
	ldr	r3, [r2, #0]
	subs	r6, #1
	asrs	r3, r3, #20
	str	r3, [r0, #0]
	ldr	r3, [r2, #4]
	adds	r2, #56
	asrs	r3, r3, #20
	str	r3, [r0, #4]
	adds	r0, #8
	cmp	r6, #0
	bge.n	.L_0802b1dc
	ldr	r3, [sp, #72]
	adds	r7, r4, #0
	adds	r3, r7, r3
	cmp	r7, r3
	bcs.n	.L_0802b2ae
	ldr	r1, [sp, #68]
	str	r3, [sp, #0]
	movs	r3, #128
	subs	r3, r3, r1
	lsls	r3, r3, #2
	mov	r8, r3
.L_0802b208:
	ldr	r2, [sp, #68]
	mov	r1, fp
	adds	r3, r1, r2
	cmp	r1, r3
	bcs.n	.L_0802b29a
	mov	ip, r7
	mov	r4, ip
	mov	lr, r3
	movs	r3, #15
	ands	r4, r3
	mov	sl, r3
	mov	ip, r4
.L_0802b220:
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #4]
	ldmia	r2!, {r5}
	movs	r3, #240
	adds	r0, r2, #0
	str	r0, [sp, #8]
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r5, r3
	ldr	r2, [pc, #140]
	ldr	r3, [r4, #0]
	movs	r6, #0
	ands	r3, r2
	orrs	r3, r5
	stmia	r4!, {r3}
	adds	r2, r1, #0
	adds	r0, r4, #0
	mov	r3, sl
	mov	r4, ip
	ands	r2, r3
	lsls	r3, r4, #5
	adds	r3, r3, r2
	str	r0, [sp, #4]
	lsls	r4, r3, #2
	mov	r0, r9
.L_0802b252:
	ldr	r3, [r0, #0]
	cmp	r3, r1
	bgt.n	.L_0802b286
	adds	r3, #16
	cmp	r3, r1
	ble.n	.L_0802b286
	ldr	r3, [r0, #4]
	cmp	r3, r7
	bgt.n	.L_0802b286
	adds	r3, #12
	cmp	r3, r7
	ble.n	.L_0802b286
	lsls	r3, r5, #3
	ldr	r2, [pc, #84]
	ldr	r5, [pc, #88]
	adds	r0, r4, r2
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r0, #0]
	ldr	r0, [pc, #80]
	adds	r2, r3, r0
	ldr	r3, [pc, #80]
	adds	r0, r4, r3
	ldr	r3, [r2, #0]
	str	r3, [r0, #0]
	b.n	.L_0802b294
.L_0802b286:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r6, #1
	adds	r4, r4, r2
	adds	r0, #8
	cmp	r6, #2
	ble.n	.L_0802b252
.L_0802b294:
	adds	r1, #1
	cmp	r1, lr
	bcc.n	.L_0802b220
.L_0802b29a:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	ldr	r5, [sp, #0]
	add	r3, r8
	add	r4, r8
	adds	r7, #1
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	cmp	r7, r5
	bcc.n	.L_0802b208
.L_0802b2ae:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0xfffff000
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.2byte 0x2840
	.2byte 0x0600
	.global Func_0802b2d4
	.thumb_func
Func_0802b2d4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r0, [r7, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	ip, r0
	adds	r3, #255
	sub	sp, #8
	mov	sl, r1
	mov	r8, r2
	cmp	ip, r3
	beq.n	.L_0802b336
	mov	r9, r3
	adds	r6, r7, #2
.L_0802b2f8:
	movs	r2, #0
	ldrsh	r1, [r6, r2]
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	movs	r4, #4
	ldrsh	r2, [r6, r4]
	movs	r4, #6
	ldrsh	r5, [r6, r4]
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	lsls	r1, r1, #16
	lsls	r5, r5, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	lsrs	r1, r1, #16
	mov	r2, sl
	mov	r3, r8
	lsrs	r5, r5, #16
	bl	Func_0802b1a0
	adds	r7, #10
	adds	r0, r5, #0
	bl	WaitFrames
	ldrh	r0, [r7, #0]
	adds	r6, #10
	mov	ip, r0
	cmp	ip, r9
	bne.n	.L_0802b2f8
.L_0802b336:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
