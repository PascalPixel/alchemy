.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008539, 0x02008039, 0x02008045, 0x0200804d, 0x02008531, 0x02008041, 0x02008605
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb098
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb0c8
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb0e0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	movs	r0, #9
	bl 0x0200aa98
	mov	sl, r0
	movs	r0, #10
	bl 0x0200aa98
	mov	r9, r0
	movs	r0, #23
	bl 0x0200aa98
	adds	r6, r0, #0
	ldr	r2, [r6, #12]
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r5, [r6, #80]
	bl 0x0200aa10
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_0200009a
	cmp	r2, #2
	beq.n	.L_0200009a
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200ab68
	movs	r7, #0
	b.n	.L_02000118
.L_0200009a:
	movs	r0, #139
	lsls	r0, r0, #2
	bl 0x0200ab68
	movs	r7, #0
.L_020000a4:
	mov	r2, sl
	ldr	r3, [r2, #12]
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #153
	adds	r3, r3, r2
	mov	r2, sl
	str	r3, [r2, #12]
	movs	r2, #224
	ldr	r3, [r6, #12]
	lsls	r2, r2, #3
	adds	r2, #174
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [pc, #236]
	ldr	r3, [r6, #8]
	movs	r0, #1
	adds	r3, r3, r2
	str	r3, [r6, #8]
	adds	r7, #1
	ldrh	r3, [r5, #18]
	adds	r3, #32
	strh	r3, [r5, #18]
	bl 0x0200a950
	cmp	r7, #63
	ble.n	.L_020000a4
	b.n	.L_0200011c
.L_020000dc:
	mov	r2, r9
	ldr	r3, [r2, #12]
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #153
	adds	r3, r3, r2
	mov	r2, r9
	str	r3, [r2, #12]
	movs	r2, #224
	ldr	r3, [r6, #12]
	lsls	r2, r2, #3
	adds	r2, #174
	adds	r3, r3, r2
	str	r3, [r6, #12]
	movs	r2, #160
	ldr	r3, [r6, #8]
	lsls	r2, r2, #4
	adds	r2, #61
	adds	r3, r3, r2
	str	r3, [r6, #8]
	movs	r2, #255
	ldrh	r3, [r5, #18]
	lsls	r2, r2, #8
	adds	r2, #224
	adds	r3, r3, r2
	strh	r3, [r5, #18]
	movs	r0, #1
	bl 0x0200a950
	adds	r7, #1
.L_02000118:
	cmp	r7, #63
	ble.n	.L_020000dc
.L_0200011c:
	mov	r3, r8
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_020001a4
	movs	r0, #30
	bl 0x0200aa80
	mov	r0, sl
	bl 0x0200905c
	mov	r0, r9
	bl 0x0200905c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #10
	bl 0x0200ab68
	movs	r7, #255
.L_02000142:
	movs	r0, #9
	bl 0x0200aa98
	movs	r5, #200
	ldr	r3, [r0, #12]
	lsls	r5, r5, #5
	adds	r5, #153
	adds	r3, r3, r5
	str	r3, [r0, #12]
	movs	r0, #10
	bl 0x0200aa98
	ldr	r3, [r0, #12]
	subs	r7, #1
	adds	r3, r3, r5
	str	r3, [r0, #12]
	movs	r0, #1
	ldr	r3, [r6, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	bl 0x0200a950
	cmp	r7, #0
	bge.n	.L_02000142
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200ab68
	movs	r0, #40
	bl 0x0200aa80
	movs	r0, #80
	bl 0x0200ab68
	movs	r0, #10
	bl 0x0200aa80
	bl 0x0200ab60
	mov	r3, sl
	movs	r2, #4
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, r9
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
.L_020001a4:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xf5c3
	.2byte 0xffff
	push	{r5, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a9d8
	bl 0x0200aa88
	movs	r0, #0
	bl 0x0200ab18
	ldr	r5, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200aac0
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aad0
	movs	r0, #11
	movs	r1, #9
	movs	r2, #0
	bl 0x020097a8
	movs	r0, #40
	bl 0x0200aa80
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a9d0
	cmp	r0, #0
	bne.n	.L_02000210
	movs	r0, #0
	bl 0x02008054
	movs	r0, #40
	bl 0x0200aa80
	b.n	.L_02000216
.L_02000210:
	movs	r0, #2
	bl 0x02008054
.L_02000216:
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200aae0
	movs	r0, #40
	bl 0x0200aa80
	movs	r0, #9
	bl 0x0200aa98
	bl 0x02009070
	movs	r0, #10
	bl 0x0200aa98
	bl 0x02009070
	movs	r0, #11
	bl 0x0200aa98
	bl 0x02009070
	movs	r0, #12
	bl 0x0200aa98
	bl 0x02009070
	bl 0x0200aaf0
	bl 0x0200aa90
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #160
.L_02000268:
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a9d8
	bl 0x0200aa88
	movs	r0, #0
	bl 0x0200ab18
	ldr	r5, [pc, #148]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200aac0
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200aad0
	movs	r0, #12
	movs	r1, #10
	movs	r2, #1
	bl 0x020097a8
	movs	r0, #40
	bl 0x0200aa80
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a9d0
	cmp	r0, #0
	bne.n	.L_020002c2
	movs	r0, #1
	bl 0x02008054
	movs	r0, #40
	bl 0x0200aa80
	b.n	.L_020002c8
.L_020002c2:
	movs	r0, #3
	bl 0x02008054
.L_020002c8:
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200aae0
	movs	r0, #40
	bl 0x0200aa80
	movs	r0, #9
	bl 0x0200aa98
	bl 0x02009070
	movs	r0, #10
	bl 0x0200aa98
	bl 0x02009070
	movs	r0, #11
	bl 0x0200aa98
	bl 0x02009070
	movs	r0, #12
	bl 0x0200aa98
	bl 0x02009070
	bl 0x0200aaf0
	bl 0x0200aa90
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	adds	r0, r1, #0
	bl 0x0200aa98
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #16]
	movs	r2, #0
	adds	r0, r3, #0
	movs	r3, #4
	bl 0x0200aa78
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	mov	r0, r8
	sub	sp, #8
	bl 0x0200aa98
	mov	r2, r8
	adds	r7, r0, #0
	cmp	r2, #13
	bne.n	.L_0200041e
	ldr	r0, [r7, #8]
	asrs	r3, r0, #20
	cmp	r3, #6
	bne.n	.L_02000414
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_02000414
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	adds	r6, r7, #0
	bl 0x0200a9d8
	adds	r6, #85
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #2
	bl 0x0200a950
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
	movs	r5, #0
	b.n	.L_02000384
.L_02000374:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a950
	cmp	r5, #29
	bgt.n	.L_0200038e
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
.L_02000384:
	cmp	r2, r3
	bgt.n	.L_02000374
	ldr	r3, [r7, #40]
	cmp	r3, #0
	bne.n	.L_02000374
.L_0200038e:
	movs	r3, #0
	strb	r3, [r6, #0]
	ldr	r3, [pc, #408]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200aaa8
	movs	r1, #0
	movs	r2, #0
	mov	r0, r8
	bl 0x0200aab8
	movs	r0, #134
	bl 0x0200ab68
	movs	r3, #70
	str	r3, [sp, #0]
	movs	r6, #10
	movs	r0, #66
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	movs	r3, #75
	str	r3, [sp, #4]
	movs	r5, #6
	movs	r0, #5
	movs	r1, #75
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200aa50
	movs	r3, #12
	str	r3, [sp, #4]
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	movs	r0, #5
	str	r5, [sp, #0]
	bl 0x0200aa48
	movs	r0, #30
	bl 0x0200aa80
	movs	r0, #167
	bl 0x0200ab68
	movs	r3, #71
	str	r3, [sp, #0]
	movs	r0, #70
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	movs	r0, #20
	bl 0x0200aa80
	bl 0x020081b4
	b.n	.L_0200041e
.L_02000414:
	ldr	r1, [r7, #16]
	movs	r2, #0
	movs	r3, #6
	bl 0x0200aa78
.L_0200041e:
	mov	r3, r8
	cmp	r3, #14
	bne.n	.L_020004fe
	ldr	r0, [r7, #8]
	asrs	r3, r0, #20
	cmp	r3, #57
	bne.n	.L_020004f4
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_020004f4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	adds	r6, r7, #0
	bl 0x0200a9d8
	adds	r6, #85
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #2
	bl 0x0200a950
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
	movs	r5, #0
	b.n	.L_02000464
.L_02000454:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a950
	cmp	r5, #29
	bgt.n	.L_0200046e
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #20]
.L_02000464:
	cmp	r2, r3
	bgt.n	.L_02000454
	ldr	r3, [r7, #40]
	cmp	r3, #0
	bne.n	.L_02000454
.L_0200046e:
	movs	r3, #0
	strb	r3, [r6, #0]
	ldr	r3, [pc, #184]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200aaa8
	movs	r1, #0
	movs	r2, #0
	mov	r0, r8
	bl 0x0200aab8
	movs	r0, #134
	bl 0x0200ab68
	movs	r3, #121
	str	r3, [sp, #0]
	movs	r6, #10
	movs	r0, #68
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	movs	r3, #75
	str	r3, [sp, #4]
	movs	r5, #57
	movs	r0, #58
	movs	r1, #75
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200aa50
	movs	r3, #12
	str	r3, [sp, #4]
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	movs	r0, #58
	str	r5, [sp, #0]
	bl 0x0200aa48
	movs	r0, #30
	bl 0x0200aa80
	movs	r0, #167
	bl 0x0200ab68
	movs	r3, #120
	str	r3, [sp, #0]
	movs	r0, #72
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	movs	r0, #20
	bl 0x0200aa80
	bl 0x02008264
	b.n	.L_020004fe
.L_020004f4:
	ldr	r1, [r7, #16]
	movs	r2, #0
	movs	r3, #6
	bl 0x0200aa78
.L_020004fe:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000524
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000524
	movs	r0, #146
	lsls	r0, r0, #4
	adds	r0, #255
	bl 0x0200a9d8
.L_02000524:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb290
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #184]
	adds	r2, #85
	str	r2, [r3, #0]
	subs	r2, #31
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000574
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02000574:
	movs	r0, #0
	bl 0x02008858
	movs	r0, #15
	movs	r1, #1
	bl 0x0200ab28
	movs	r1, #1
	movs	r0, #16
	bl 0x0200ab28
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #17
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #18
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #19
	bl 0x0200aa98
.L_020005c4:
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #21
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #22
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	bl 0x02008e80
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #0
	sub	sp, #8
	bl 0x02008764
	bl 0x02009cb4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000678
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aab8
	movs	r3, #70
	str	r3, [sp, #0]
	movs	r6, #10
	movs	r0, #66
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	movs	r3, #75
	str	r3, [sp, #4]
	movs	r5, #6
	movs	r0, #5
	movs	r1, #75
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200aa50
	movs	r3, #12
	str	r3, [sp, #4]
	movs	r0, #5
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200aa48
	movs	r3, #71
	str	r3, [sp, #0]
	movs	r0, #70
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	b.n	.L_02000690
.L_02000678:
	movs	r0, #13
	movs	r1, #3
	bl 0x02009dbc
	movs	r0, #13
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02000690:
	movs	r0, #13
	bl 0x0200aa98
	adds	r2, r0, #0
	adds	r1, r2, #0
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	movs	r3, #6
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #16]
	movs	r2, #0
	bl 0x0200aa78
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000712
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aab8
	movs	r3, #121
	str	r3, [sp, #0]
	movs	r6, #10
	movs	r0, #68
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	movs	r3, #75
	str	r3, [sp, #4]
	movs	r5, #57
	movs	r0, #58
	movs	r1, #75
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200aa50
	movs	r3, #12
	str	r3, [sp, #4]
	movs	r0, #58
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x0200aa48
	movs	r3, #120
	str	r3, [sp, #0]
	movs	r0, #72
	movs	r1, #39
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #4]
	bl 0x0200aa50
	b.n	.L_0200072a
.L_02000712:
	movs	r0, #14
	movs	r1, #5
	bl 0x02009dbc
	movs	r0, #14
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_0200072a:
	movs	r0, #14
	bl 0x0200aa98
	adds	r2, r0, #0
	adds	r1, r2, #0
	adds	r1, #85
	movs	r3, #0
	strb	r3, [r1, #0]
	movs	r3, #6
	ldr	r1, [r2, #16]
	ldr	r0, [r2, #8]
	movs	r2, #0
	bl 0x0200aa78
	movs	r0, #11
	bl 0x0200aa98
	movs	r5, #1
	adds	r0, #98
	strb	r5, [r0, #0]
	movs	r0, #12
.L_02000754:
	bl 0x0200aa98
	adds	r0, #98
	strb	r5, [r0, #0]
	add	sp, #8
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl 0x0200a9d8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_0200078c
	movs	r0, #98
	adds	r0, #255
	bl 0x0200a9d8
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200a9d8
.L_0200078c:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.4byte 0x00004770
	.2byte 0xb318
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_020007ae
	adds	r0, #3
.L_020007ae:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200a948
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_02000802
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r1, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_020007e2
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_0200083e
.L_020007e2:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200083e
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200083e
.L_02000802:
	movs	r5, #0
	movs	r6, #4
.L_02000806:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200a948
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_02000806
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	ldr	r1, [pc, #36]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200083e:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b314
	.4byte 0x0200b318
	.4byte 0x0200b340
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x0200887c
	ldr	r1, [pc, #80]
	movs	r2, #32
	ldr	r0, [pc, #80]
	ldr	r5, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4814
	ldr	r1, [pc, #80]
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	bl 0x0200a9d0
	cmp	r0, #0
	bne.n	.L_0200088c
	cmp	r6, #1
	bne.n	.L_0200089e
.L_0200088c:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200a958
	b.n	.L_020008b2
.L_0200089e:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_020008b2:
	pop	{r5, r6, pc}
	.4byte 0x0200b318
	.4byte 0x05000180
	.4byte 0x0200b340
	.4byte 0x03000730
	.4byte 0x0200b360
	.4byte 0x050001a0
	.4byte 0x0200b314
	.4byte 0x0200879d
	.4byte 0x0200b364
	.2byte 0x0184
	.2byte 0x0500
	push	{r5, lr}
	bl 0x0200aa98
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #85
	movs	r3, #4
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r3, [r5, #20]
	str	r2, [r5, #68]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r1, #50
	ldrb	r2, [r1, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #34
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r3, #0]
	bl 0x0200aa38
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200aa78
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200ab58
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #244]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200aa88
	movs	r0, #0
	bl 0x0200ab18
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	movs	r5, #0
	strh	r5, [r3, #0]
	movs	r3, #85
	adds	r3, r3, r6
	mov	r9, r3
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aa58
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020009d0
.L_0200098a:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_020009a6
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_020009a6:
	ldr	r3, [pc, #156]
	adds	r1, r6, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r6, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200a950
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0200098a
.L_020009d0:
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #68
	movs	r2, #1
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	strh	r2, [r3, #0]
	ldr	r3, [r7, #8]
	ldrh	r1, [r7, #6]
	subs	r2, #3
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #56]
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #16]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	adds	r3, r6, #0
	adds	r3, #35
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200aa58
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_02000a48
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_02000a48:
	bl 0x0200aae8
	bl 0x0200aaf8
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	ldr	r5, [pc, #52]
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200aa38
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200aa28
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_02000aae
	b.n	.L_02000a94
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02000a94:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a950
	cmp	r5, #59
	bgt.n	.L_02000aae
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_02000a94
.L_02000aae:
	movs	r0, #127
	bl 0x0200ab68
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_02000ace
.L_02000abc:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200a950
	cmp	r5, #59
	bgt.n	.L_02000ace
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02000abc
.L_02000ace:
	adds	r0, r7, #0
	bl 0x0200aa30
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200aa98
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200aae0
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #70
	add	r2, sl
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #170
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, sl
	strh	r6, [r3, #0]
	bl 0x0200aa90
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [r1, #0]
	ldr	r4, [r0, #0]
	ldr	r2, [r1, #8]
	subs	r4, r4, r3
	ldr	r3, [r0, #8]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x0000
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #80]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200aa98
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_02000ba6
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000ba6
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02000ba6
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02000bb4
.L_02000ba6:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a9f8
	b.n	.L_02000cf2
	.2byte 0x0240
	.2byte 0x0200
.L_02000bb4:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200a9f8
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_02000bd4
	movs	r0, #231
	bl 0x0200ab68
.L_02000bd4:
	ldr	r3, [r7, #80]
	ldr	r0, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	movs	r2, #2
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	bl 0x0200ab50
	cmp	r0, #255
	beq.n	.L_02000cd6
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200ab20
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_02000cd6
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_02000cd6
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02000c9c
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_02000c34
	subs	r5, r3, r2
.L_02000c34:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x02008b1c
	cmp	r0, #12
	bgt.n	.L_02000c54
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_02000c54
	movs	r2, #1
	mov	r8, r2
.L_02000c54:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02000c9c
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200a9d0
	cmp	r0, #0
	bne.n	.L_02000c9c
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	strb	r3, [r1, #0]
	add	r2, sl
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #128
	ldr	r0, [pc, #56]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_02000c9c:
	ldrh	r0, [r6, #6]
	bl 0x0200a978
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200a970
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_02000cd0
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_02000cd0:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_02000cf2
.L_02000cd6:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl 0x0200aa00
	movs	r0, #228
	bl 0x0200ab68
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_02000cf2:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b31c
	.2byte 0xb33c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200ab68
	ldrh	r0, [r5, #6]
	bl 0x0200a978
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r6, [pc, #152]
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	add	r2, sp, #56
	adds	r3, r3, r0
	str	r3, [r2, #0]
	mov	r8, r2
	ldrh	r0, [r5, #6]
	bl 0x0200a970
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x692b
	mov	r2, r8
	adds	r3, r3, r0
	str	r3, [r2, #8]
	movs	r0, #140
	ldr	r1, [r2, #0]
	lsls	r0, r0, #1
	ldr	r2, [r5, #12]
	bl 0x0200aa08
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200a9f0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200aa58
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
.L_02000d70:
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #6]
	add	r4, sp, #16
	strh	r3, [r7, #6]
	adds	r3, r7, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #1
	str	r3, [r4, #0]
	movs	r3, #7
	str	r3, [r4, #4]
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #8]
	ldr	r3, [pc, #24]
	ldr	r1, [r5, #12]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	b.n	.L_02000dc8
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x02008b49
	.2byte 0x0000
	.2byte 0xfffa
.L_02000dc8:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
.L_02000dd0:
	bl 0x02009e78
	adds	r0, r7, #0
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	sub	sp, #56
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_02000e6e
	add	r6, sp, #16
	movs	r3, #3
	str	r3, [r6, #0]
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #14
	str	r3, [r6, #4]
	bl 0x0200a968
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200a968
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	lsrs	r3, r3, #16
	movs	r2, #32
	subs	r2, r2, r3
	mov	r3, sl
	ldr	r5, [r3, #12]
	lsls	r2, r2, #16
	adds	r5, r5, r2
	bl 0x0200a968
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200a940
	mov	r3, sl
	ldr	r2, [r3, #16]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r0, [sp, #0]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x02009e78
.L_02000e6e:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x0200aa98
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200aa98
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200aa98
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200aab8
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200aab8
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200aab8
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200aab8
	movs	r0, #24
	bl 0x0200aa98
	movs	r3, #85
	movs	r2, #0
	adds	r3, r3, r5
	str	r2, [r0, #24]
	strb	r2, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #20]
	movs	r0, #160
	str	r3, [r5, #12]
	movs	r3, #85
	adds	r3, r3, r6
	strb	r2, [r3, #0]
	mov	r9, r3
	ldr	r3, [r6, #20]
	lsls	r0, r0, #4
	str	r3, [r6, #12]
	movs	r3, #85
	adds	r3, r3, r7
	strb	r2, [r3, #0]
	mov	sl, r3
	ldr	r3, [r7, #20]
	adds	r0, #10
	str	r3, [r7, #12]
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000f5c
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #208]
	movs	r0, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [pc, #204]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #200]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200aa98
	movs	r1, #4
	bl 0x0200aa70
	movs	r0, #11
	bl 0x0200aa98
	movs	r1, #4
	bl 0x0200aa70
.L_02000f5c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000fa6
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #132]
	movs	r0, #10
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	ldr	r2, [pc, #120]
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200aa98
	movs	r1, #4
	bl 0x0200aa70
	movs	r0, #12
	bl 0x0200aa98
	movs	r1, #4
	bl 0x0200aa70
.L_02000fa6:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000fe6
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02000fe6
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #56]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r2, sl
	strb	r3, [r2, #0]
	mov	r2, fp
	strb	r3, [r2, #0]
.L_02000fe6:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00066640
	.4byte 0x0001eb80
	.4byte 0xfffd70c0
	.4byte 0x00028f40
	.4byte 0xfffff800
	.4byte 0x00199900
	.2byte 0x8480
	.2byte 0x001b
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r5, #99
	ldrb	r3, [r5, #0]
	ldr	r2, [pc, #60]
	movs	r1, #3
	ands	r1, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	cmp	r1, #1
	beq.n	.L_0200103a
	cmp	r1, #1
	bgt.n	.L_02001030
	cmp	r1, #0
	beq.n	.L_02001042
	b.n	.L_02001050
.L_02001030:
	cmp	r1, #2
	beq.n	.L_02001042
	cmp	r1, #3
	beq.n	.L_0200104a
	b.n	.L_02001050
.L_0200103a:
	movs	r1, #13
	bl 0x0200aa70
	b.n	.L_02001050
.L_02001042:
	movs	r1, #4
	bl 0x0200aa70
	b.n	.L_02001050
.L_0200104a:
	movs	r1, #4
	bl 0x0200aa70
.L_02001050:
	ldrb	r3, [r5, #0]
	adds	r3, #1
	strb	r3, [r5, #0]
	pop	{r5, pc}
	.4byte 0x03001174
	.4byte 0x32631c02
	.4byte 0x70132300
	.4byte 0x66c34b01
	.4byte 0x00004770
	.2byte 0x9011
	.2byte 0x0200
	push	{lr}
	movs	r3, #0
	movs	r1, #4
	str	r3, [r0, #108]
	bl 0x0200aa70
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #220
	ldr	r7, [r3, #0]
	ldr	r3, [r2, #108]
	movs	r0, #230
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	movs	r1, #176
	lsls	r1, r1, #4
	adds	r1, #2
	mov	r9, r3
	adds	r3, r7, r1
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	sub	sp, #44
	str	r2, [sp, #4]
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #5
	bls.n	.L_020010c4
	b.n	.L_020015e8
.L_020010c4:
	ldr	r2, [pc, #840]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x020090e4
	.4byte 0x02009264
	.4byte 0x020093a6
	.4byte 0x02009424
	.4byte 0x02009566
	.4byte 0x020095bc
	.4byte 0x011b23b0
	.4byte 0x18fa3308
	.4byte 0x231e24b0
	.4byte 0x01247013
	.4byte 0x193b340b
	.4byte 0x061b781b
	.4byte 0x25a0161b
	.4byte 0x2b00032d
	.4byte 0x4dc3d000
	.4byte 0x010020a0
	.4byte 0x183a30c4
	.4byte 0xae056813
	.4byte 0x21a01b5b
	.4byte 0x01096033
	.4byte 0x19c931c8
	.4byte 0x24c0680b
	.4byte 0x191b0364
	.4byte 0x30086073
	.4byte 0x1839468b
	.4byte 0x465c680b
	.4byte 0x230860b3
	.4byte 0x446b6812
	.4byte 0x4698601a
	.4byte 0x46406823
	.4byte 0x24b06043
	.4byte 0x21a0680b
	.4byte 0x9b016083
	.4byte 0x01096835
	.4byte 0x31e80124
	.4byte 0x1b523408
	.4byte 0x19c919e4
	.4byte 0x1c184689
	.4byte 0x21004350
	.4byte 0x46a25661
	.4byte 0xfbe8f001
	.4byte 0x4648182d
	.4byte 0x46426005
	.4byte 0x68756853
	.4byte 0x9c0121a0
	.4byte 0x31ec0109
	.4byte 0x46521b5b
	.4byte 0x468919c9
	.4byte 0x43581c20
	.4byte 0x56512100
	.4byte 0xfbd4f001
	.4byte 0x182d464b
	.4byte 0x4640601d
	.4byte 0x68b56883
	.4byte 0x24af9901
	.4byte 0x46521b5b
	.4byte 0x19e40124
	.4byte 0x43581c08
	.4byte 0x56512100
	.4byte 0xf00146a1
	.4byte 0x464bfbc1
	.4byte 0x601d182d
	.4byte 0x6823465c
	.4byte 0x46424893
	.4byte 0x6073181b
	.4byte 0x68136835
	.4byte 0x9c0121a0
	.4byte 0x31f40109
	.4byte 0x46521b5b
	.4byte 0x468919c9
	.4byte 0x43581c20
	.4byte 0x56512100
	.4byte 0xfba8f001
	.4byte 0x182d464b
	.4byte 0x4640601d
	.4byte 0x68756843
	.4byte 0x24a09901
	.4byte 0x1b5b0124
	.4byte 0x34f84652
	.4byte 0x1c0819e4
	.4byte 0x21004358
	.4byte 0x46a15651
	.4byte 0xfb94f001
	.4byte 0x182d464b
	.4byte 0x4640601d
	.4byte 0x68b56883
	.4byte 0x24a09901
	.4byte 0x1b5b0124
	.4byte 0x34fc4652
	.4byte 0x1c0819e4
	.4byte 0x21004358
	.4byte 0x46a15651
	.4byte 0xfb80f001
	.4byte 0x182d464b
	.4byte 0x601d24b0
	.4byte 0x34020124
	.4byte 0x46541939
	.4byte 0x5e0a2000
	.4byte 0x56e32300
	.4byte 0xd000429a
	.4byte 0x20b0e1c4
	.4byte 0xe1770100
	.4byte 0x010921b0
	.4byte 0x187a3108
	.4byte 0x7013233c
	.4byte 0x011222b0
	.4byte 0x18bb320b
	.4byte 0x061b781b
	.4byte 0x25f0161b
	.4byte 0x2b00036d
	.4byte 0x4d65d000
	.4byte 0x012424a0
	.4byte 0xab0534c4
	.4byte 0x4698193a
	.4byte 0x46406813
	.4byte 0x600321a0
	.4byte 0x31c80109
	.4byte 0x68031878
	.4byte 0x60634644
	.4byte 0x011b23a0
	.4byte 0x18f933cc
	.4byte 0xae02680b
	.4byte 0x24a060a3
	.4byte 0x01246812
	.4byte 0x60321b52
	.4byte 0x680334e8
	.4byte 0x6073193c
	.4byte 0x680b4640
	.4byte 0x940060b3
	.4byte 0x68059901
	.4byte 0x1c081b52
	.4byte 0x22b04350
	.4byte 0x32080112
	.4byte 0x210019d2
	.4byte 0x46925651
	.4byte 0xfb2af001
	.4byte 0x182d9b00
	.4byte 0x4640601d
	.4byte 0x68736845
	.4byte 0x24a09901
	.4byte 0x1b5b0124
	.4byte 0x34ec4652
	.4byte 0x1c0819e4
	.4byte 0x21004358
	.4byte 0x46a35651
	.4byte 0xfb16f001
	.4byte 0x182d465b
	.4byte 0x4640601d
	.4byte 0x68b36885
	.4byte 0x24af9901
	.4byte 0x46521b5b
	.4byte 0x19e40124
	.4byte 0x43581c08
	.4byte 0x56512100
	.4byte 0xf00146a1
	.4byte 0x464bfb03
	.4byte 0x601d182d
	.4byte 0x24a09800
	.4byte 0x01246803
	.4byte 0x193a34f4
	.4byte 0x465c6013
	.4byte 0x682321a0
	.4byte 0x31f80109
	.4byte 0x6013187a
	.4byte 0x20a04649
	.4byte 0x0100680b
	.4byte 0x183a30fc
	.4byte 0x23b06013
	.4byte 0x3302011b
	.4byte 0x240018fe
	.4byte 0x22a85f30
	.4byte 0x213c0112
	.4byte 0x18bd0400
	.4byte 0xfadef001
	.4byte 0x61a86168
	.4byte 0x20004651
	.4byte 0x23005e32
	.4byte 0x429a56cb
	.4byte 0xe127d000
	.4byte 0x011222b0
	.4byte 0x881a18bb
	.4byte 0x801a3201
	.4byte 0x24b0e105
	.4byte 0x34020124
	.4byte 0x2000193b
	.4byte 0x2b005e1b
	.4byte 0x21b0d105
	.4byte 0x310a0109
	.4byte 0x2301187a
	.4byte 0x22b07013
	.4byte 0x320b0112
	.4byte 0x781b18bb
	.4byte 0x161b061b
	.4byte 0x022d2580
	.4byte 0xd0002b00
	.4byte 0x23a04d11
	.4byte 0x33e8011b
	.4byte 0x681318fa
	.4byte 0x195b24a0
	.4byte 0x01246013
	.4byte 0x193a34f4
	.4byte 0x20b06813
	.4byte 0x6013195b
	.4byte 0x30020100
	.4byte 0x22001839
	.4byte 0x2b0e5e8b
	.4byte 0xe0efd000
	.4byte 0x193b340c
	.4byte 0x0000e0a3
	.4byte 0x020090cc
	.4byte 0xfff60000
	.4byte 0xffe80000
	.4byte 0xffe20000
	.4byte 0xffff8000
	.4byte 0x28009801
	.4byte 0x2083d102
	.4byte 0xfb9cf001
	.4byte 0x011222a0
	.4byte 0x24ad32c4
	.4byte 0x012418bb
	.4byte 0x681d21a0
	.4byte 0x193b0109
	.4byte 0x31dc681b
	.4byte 0x468a19c9
	.4byte 0x10ad9901
	.4byte 0x1b5b109b
	.4byte 0x43581c08
	.4byte 0xf0012160
	.4byte 0x182dfa71
	.4byte 0x465200ad
	.4byte 0x24a023ae
	.4byte 0x601520a0
	.4byte 0x0124011b
	.4byte 0x34c80100
	.4byte 0x30d419db
	.4byte 0x4698193a
	.4byte 0x6815183b
	.4byte 0x9901681b
	.4byte 0x1c081b5b
	.4byte 0x21604358
	.4byte 0xfa58f001
	.4byte 0x182d4642
	.4byte 0x24a023a0
	.4byte 0x601520a0
	.4byte 0x0124011b
	.4byte 0x34cc0100
	.4byte 0x30d833e4
	.4byte 0x18fe193a
	.4byte 0x6815183b
	.4byte 0x9901681b
	.4byte 0x1c081b5b
	.4byte 0x21604358
	.4byte 0xfa40f001
	.4byte 0x6035182d
	.4byte 0x46434652
	.4byte 0x46486811
	.4byte 0x1c2b681a
	.4byte 0xfa9ef001
	.4byte 0x012424b0
	.4byte 0x193b340b
	.4byte 0x061b781b
	.4byte 0x2580161b
	.4byte 0x2b00036d
	.4byte 0x4d7dd000
	.4byte 0x010921a0
	.4byte 0x187b31dc
	.4byte 0x20a0681b
	.4byte 0x30e80100
	.4byte 0x1b5b183c
	.4byte 0x31046023
	.4byte 0x22a0187b
	.4byte 0x0112681b
	.4byte 0x18b832ec
	.4byte 0x32046003
	.4byte 0x3a0c18b9
	.4byte 0x681b18bb
	.4byte 0x23a0600b
	.4byte 0x33f4011b
	.4byte 0x682318fa
	.4byte 0x601324a0
	.4byte 0x68030124
	.4byte 0x193a34f8
	.4byte 0x20a06013
	.4byte 0x0100680b
	.4byte 0x183a30fc
	.4byte 0x22b06013
	.4byte 0x32020112
	.4byte 0x240018b9
	.4byte 0x2b605f0b
	.4byte 0x3004d14a
	.4byte 0x881a183b
	.4byte 0x801a3201
	.4byte 0x021b23ff
	.4byte 0x800b33ff
	.4byte 0x21b0e040
	.4byte 0x31020109
	.4byte 0x2200187e
	.4byte 0x2b005eb3
	.4byte 0x20dcd108
	.4byte 0xfaf6f001
	.4byte 0x012424b1
	.4byte 0x6818193b
	.4byte 0xfd6af7ff
	.4byte 0x010020a8
	.4byte 0x696b183d
	.4byte 0x22cc4954
	.4byte 0x185b0192
	.4byte 0x616b3232
	.4byte 0x429361ab
	.4byte 0x2300dc22
	.4byte 0x23b0616b
	.4byte 0x18fa011b
	.4byte 0x33018813
	.4byte 0x23ff8013
	.4byte 0x33ff021b
	.4byte 0xe0158033
	.4byte 0x012424b0
	.4byte 0x19393402
	.4byte 0x5e0a2000
	.4byte 0xd1022a00
	.4byte 0x193b3407
	.4byte 0x2000701a
	.4byte 0x2b105e0b
	.4byte 0x21b0d106
	.4byte 0x010923ba
	.4byte 0x187a009b
	.2byte 0x33ff
	.2byte 0x8013
.L_020015e8:
	movs	r4, #176
	lsls	r4, r4, #4
	adds	r4, #11
	adds	r3, r7, r4
	movs	r2, #168
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsls	r2, r2, #4
	adds	r5, r7, r2
	ldr	r2, [r5, #20]
	add	r6, sp, #32
	cmp	r3, #0
	bne.n	.L_0200160e
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_02001618
	negs	r3, r2
	b.n	.L_02001618
.L_0200160e:
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_02001616
	negs	r3, r3
.L_02001616:
	negs	r3, r3
.L_02001618:
	str	r3, [r5, #20]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #220
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	movs	r1, #174
	str	r3, [r6, #0]
	lsls	r1, r1, #4
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r2, #160
	str	r3, [r6, #4]
	lsls	r2, r2, #4
	adds	r2, #228
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl 0x0200ab20
	ldr	r3, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r5, #12]
	ldr	r3, [r6, #8]
	str	r3, [r5, #16]
	bl 0x0200ab38
	movs	r4, #224
	movs	r0, #128
	movs	r3, #0
	lsls	r4, r4, #3
	lsls	r0, r0, #2
	mov	r8, r3
	adds	r5, r7, r4
	adds	r6, r7, r0
.L_02001660:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_02001718
	movs	r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001690
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r2, #232
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r5, #0]
	lsls	r4, r4, #4
	adds	r4, #236
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r0, #175
	str	r3, [r5, #4]
	lsls	r0, r0, #4
	adds	r3, r7, r0
	b.n	.L_020016b0
.L_02001690:
	movs	r1, #160
	lsls	r1, r1, #4
	adds	r1, #244
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r2, #160
	str	r3, [r5, #0]
	lsls	r2, r2, #4
	adds	r2, #248
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r5, #4]
	lsls	r4, r4, #4
	adds	r4, #252
	adds	r3, r7, r4
.L_020016b0:
	ldr	r3, [r3, #0]
	str	r3, [r5, #8]
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #10
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_020016e8
	bl 0x0200a968
	bl 0x0200a970
	ldr	r3, [r5, #4]
	lsls	r0, r0, #3
	adds	r3, r3, r0
	str	r3, [r5, #4]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #204
	str	r3, [r5, #16]
	b.n	.L_02001718
	.4byte 0xfff00000
	.2byte 0xf800
	.2byte 0xffff
.L_020016e8:
	.2byte 0xf001
	.2byte 0xf93e
	.2byte 0xf001
	.2byte 0xf940
	.2byte 0x686b
	lsls	r0, r0, #1
	adds	r3, r3, r0
	str	r3, [r5, #4]
	bl 0x0200a968
	bl 0x0200a978
	ldr	r3, [r5, #0]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r5, #0]
	bl 0x0200a968
	ldr	r1, [pc, #68]
	adds	r0, r0, r1
	lsls	r0, r0, #1
	str	r0, [r5, #16]
.L_02001718:
	ldr	r1, [r5, #24]
	cmp	r1, #15
	bhi.n	.L_0200175c
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #6
	adds	r2, r7, r3
	movs	r3, #3
	ands	r1, r3
	lsls	r3, r1, #1
	ldrh	r1, [r2, #0]
	ldr	r2, [pc, #32]
	adds	r1, r1, r3
	ldr	r3, [pc, #32]
	adds	r0, r6, #0
	ands	r1, r3
	ldrh	r3, [r6, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl 0x0200ab40
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	subs	r3, r3, r2
	str	r3, [r5, #4]
	b.n	.L_0200175c
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0x8000
	.2byte 0xffff
.L_0200175c:
	.2byte 0x69ab
.L_0200175e:
	movs	r4, #176
	adds	r2, r3, #1
	str	r2, [r5, #24]
	lsls	r4, r4, #4
	adds	r4, #9
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0200177c
	cmp	r2, #16
	bne.n	.L_0200177c
	movs	r3, #0
	str	r3, [r5, #24]
.L_0200177c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r1, #7
	bgt.n	.L_0200178c
	b.n	.L_02001660
.L_0200178c:
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	add	sp, #44
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	movs	r1, #176
	lsls	r1, r1, #4
	sub	sp, #12
	adds	r5, r0, #0
	adds	r1, #20
	movs	r0, #220
	str	r2, [sp, #8]
	bl 0x0200a980
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r7, r0, #0
	movs	r0, #230
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	adds	r0, r5, #0
	mov	fp, r3
	bl 0x0200aa98
	movs	r1, #176
	lsls	r1, r1, #4
	adds	r1, #12
	adds	r3, r7, r1
	str	r0, [r3, #0]
	adds	r0, r6, #0
	bl 0x0200aa98
	movs	r2, #177
	lsls	r2, r2, #4
	adds	r3, r7, r2
	str	r0, [r3, #0]
	ldr	r0, [pc, #156]
	bl 0x0200a9c8
	adds	r1, r7, #0
	bl 0x0200a9a0
	bl 0x0200a9b8
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r7, #0
	str	r0, [sp, #4]
	bl 0x0200a9b0
	mov	r9, r0
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #6
	adds	r3, r7, r0
	mov	r1, r9
	movs	r2, #224
	movs	r0, #128
	strh	r1, [r3, #0]
	lsls	r2, r2, #3
	movs	r3, #0
	lsls	r0, r0, #2
	movs	r1, #31
	adds	r6, r7, r2
	mov	sl, r3
	adds	r5, r7, r0
	mov	r8, r1
.L_02001838:
	mov	r2, r9
	adds	r0, r5, #0
	str	r2, [sp, #0]
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl 0x0200ab30
	ldrb	r3, [r5, #5]
	ldrb	r2, [r5, #9]
	movs	r0, #32
	orrs	r3, r0
	strb	r3, [r5, #5]
	movs	r1, #13
	movs	r3, #15
	ands	r3, r2
	negs	r1, r1
	ldr	r2, [pc, #56]
	ands	r3, r1
	adds	r1, #12
	add	r8, r1
	strb	r3, [r5, #9]
	strh	r2, [r5, #30]
	mov	r3, sl
	subs	r0, #34
	mov	r2, r8
	str	r3, [r6, #24]
	add	sl, r0
	adds	r5, #40
	adds	r6, #28
	cmp	r2, #0
	bge.n	.L_02001838
	ldr	r0, [pc, #32]
	bl 0x0200a9c8
	adds	r1, r7, #0
	bl 0x0200a9a0
	bl 0x0200a9b8
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	sl, r0
	b.n	.L_020018a0
	.4byte 0x000000f0
	.4byte 0x000001eb
	.2byte 0x01f6
	.2byte 0x0000
.L_020018a0:
	bl 0x0200a9b0
	movs	r1, #176
	lsls	r1, r1, #4
	movs	r2, #168
	adds	r1, #4
	lsls	r2, r2, #4
	adds	r5, r7, r2
	adds	r3, r7, r1
	strh	r0, [r3, #0]
	movs	r1, #30
	str	r0, [sp, #0]
	movs	r2, #7
	adds	r0, r5, #0
	ldr	r3, [pc, #52]
	bl 0x0200ab30
	ldr	r3, [pc, #44]
	movs	r2, #13
	strh	r3, [r5, #30]
	ldrb	r3, [r5, #9]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldrb	r3, [r5, #5]
	movs	r0, #32
	movs	r1, #15
	movs	r6, #0
	ands	r2, r1
	orrs	r3, r0
	strb	r3, [r5, #5]
	strb	r2, [r5, #9]
	str	r6, [r5, #20]
	str	r6, [r5, #24]
	ldr	r2, [sp, #8]
	cmp	r2, #0
	bne.n	.L_0200192e
	b.n	.L_020018f8
	.2byte 0x0000
	.4byte 0x000000f0
	.2byte 0x4000
	.2byte 0x8000
.L_020018f8:
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #196
	movs	r0, #160
	adds	r2, r7, r3
	lsls	r0, r0, #4
	movs	r3, #152
	lsls	r3, r3, #16
	adds	r0, #200
	movs	r1, #160
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	adds	r3, r7, r0
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r1, #204
	str	r0, [r3, #0]
	adds	r3, r7, r1
	movs	r1, #164
	lsls	r1, r1, #16
	str	r1, [r3, #0]
	movs	r3, #173
	lsls	r3, r3, #4
	adds	r2, r7, r3
	movs	r3, #235
	lsls	r3, r3, #17
	b.n	.L_0200195c
.L_0200192e:
	movs	r1, #160
	lsls	r1, r1, #4
	adds	r1, #196
	movs	r3, #218
	adds	r2, r7, r1
	lsls	r3, r3, #18
	str	r3, [r2, #0]
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r2, #200
	movs	r0, #128
	adds	r3, r7, r2
	lsls	r0, r0, #14
	adds	r1, #8
	str	r0, [r3, #0]
	adds	r3, r7, r1
	movs	r1, #164
	lsls	r1, r1, #16
	str	r1, [r3, #0]
	movs	r3, #173
	lsls	r3, r3, #4
	adds	r2, r7, r3
	ldr	r3, [pc, #208]
.L_0200195c:
	str	r3, [r2, #0]
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r2, #212
	adds	r3, r7, r2
	str	r0, [r3, #0]
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #216
	adds	r3, r7, r0
	str	r1, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #4
	adds	r2, #196
	adds	r3, r7, r2
	movs	r1, #160
	ldr	r3, [r3, #0]
	lsls	r1, r1, #4
	adds	r1, #220
	adds	r1, r1, r7
	movs	r0, #160
	str	r3, [r1, #0]
	lsls	r0, r0, #4
	movs	r3, #174
	lsls	r3, r3, #4
	adds	r0, #200
	adds	r6, r7, r3
	adds	r3, r7, r0
.L_02001994:
	ldr	r3, [r3, #0]
	adds	r2, #8
	str	r3, [r6, #0]
	mov	r8, r1
	adds	r3, r7, r2
	movs	r1, #160
	ldr	r3, [r3, #0]
	lsls	r1, r1, #4
	adds	r1, #228
	adds	r5, r7, r1
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	mov	r0, fp
	str	r3, [r0, #48]
	str	r3, [r0, #52]
	bl 0x0200aa20
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r2, [r6, #0]
	mov	r0, fp
	bl 0x0200aa28
	movs	r0, #142
	bl 0x0200ab68
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r0, #12
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl 0x0200905c
	movs	r0, #40
	bl 0x0200aa80
	movs	r1, #176
	add	r0, sp, #8
	ldrb	r0, [r0, #0]
	lsls	r1, r1, #4
	adds	r1, #11
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	movs	r0, #176
	lsls	r0, r0, #4
	subs	r1, #11
	adds	r0, #2
	movs	r2, #0
	adds	r5, r7, r1
	adds	r3, r7, r0
	strh	r2, [r5, #0]
	strh	r2, [r3, #0]
	movs	r3, #176
	lsls	r3, r3, #4
	ldr	r1, [pc, #32]
	adds	r3, #9
	adds	r2, r7, r3
	adds	r0, #8
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r3, r7, r0
	strb	r1, [r3, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl 0x0200a958
	movs	r2, #186
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	b.n	.L_02001a46
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02260000
	.2byte 0x9081
	.2byte 0x0200
.L_02001a34:
	movs	r0, #1
	bl 0x0200a950
	movs	r0, #176
	lsls	r0, r0, #4
	adds	r3, r7, r0
	movs	r2, #186
	movs	r1, #0
	ldrsh	r3, [r3, r1]
.L_02001a46:
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02001a34
	ldr	r0, [pc, #36]
	bl 0x0200a960
	mov	r0, sl
	bl 0x0200a9a8
	ldr	r0, [sp, #4]
	bl 0x0200a9a8
	movs	r0, #220
	bl 0x0200a988
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x9081
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #380]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [pc, #372]
	mov	sl, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r8, r2
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #8
	lsrs	r3, r3, #5
	str	r3, [sp, #4]
	ldr	r6, [pc, #356]
	ldr	r3, [r1, #0]
	movs	r1, #0
	ldr	r3, [r3, #4]
	mov	r9, r1
	str	r3, [sp, #0]
	ldr	r3, [pc, #348]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r9, r3
	blt.n	.L_02001aca
	b.n	.L_02001bfe
.L_02001aca:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_02001ad8
	b.n	.L_02001bee
.L_02001ad8:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_02001ae0
	b.n	.L_02001bee
.L_02001ae0:
	mov	r1, sl
	subs	r0, r3, r1
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r3, r3, r2
	ldr	r2, [r5, #16]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	mov	r1, r8
	subs	r2, r2, r1
	ldr	r1, [sp, #0]
	subs	r2, r2, r1
	subs	r4, r2, r3
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r3, #58
	mov	fp, r3
	ldr	r3, [pc, #284]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, #0
	mov	ip, r2
	asrs	r1, r0, #16
	mov	r0, ip
	adds	r3, #100
	asrs	r2, r4, #16
	cmp	r0, #0
	bne.n	.L_02001b56
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #7
	movs	r1, #167
	adds	r4, r2, #0
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #16
	cmp	r3, r1
	bhi.n	.L_02001bee
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_02001bee
	cmp	r4, #239
	bgt.n	.L_02001bee
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	mov	r3, ip
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #212]
	b.n	.L_02001b92
.L_02001b56:
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #0
	movs	r1, #175
	adds	r4, r2, #0
	adds	r3, #23
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #64
	cmp	r3, r1
	bhi.n	.L_02001bee
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_02001bee
	cmp	r4, #175
	bgt.n	.L_02001bee
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #152]
.L_02001b92:
	movs	r2, #128
	orrs	r4, r3
	stmia	r1!, {r4}
	ldr	r0, [sp, #4]
	lsls	r3, r7, #3
	adds	r3, r0, r3
	lsls	r2, r2, #4
	orrs	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	movs	r0, #1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_02001bd0
	adds	r0, r5, #0
	bl 0x0200ab48
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r6, #9]
	b.n	.L_02001be4
.L_02001bd0:
	movs	r3, #3
	ands	r3, r2
	movs	r0, #13
	ldrb	r2, [r6, #9]
	negs	r0, r0
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_02001be4:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200a9c0
	adds	r6, #12
.L_02001bee:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_02001bfe
	b.n	.L_02001aca
.L_02001bfe:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff0000
	.4byte 0x0200b380
	.4byte 0x020036e0
	.4byte 0x0200b3c4
	.4byte 0x0200b382
	.4byte 0x0200b384
	.4byte 0x0200b484
	.4byte 0x40002000
	.4byte 0xc000a000
	.2byte 0xb486
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200a990
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200a9a0
	ldr	r5, [pc, #76]
	bl 0x0200a9b8
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a9b0
	adds	r0, r6, #0
	bl 0x0200a998
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200a958
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b384
	.4byte 0x0200ab70
	.4byte 0x0200b380
	.4byte 0x02009a79
	.4byte 0x0200b382
	.4byte 0x0200b484
	.2byte 0xb486
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200a990
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200a9a0
	ldr	r5, [pc, #76]
	bl 0x0200a9b8
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a9b0
	adds	r0, r6, #0
	bl 0x0200a998
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200a958
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b384
	.4byte 0x0200acd3
	.4byte 0x0200b380
	.4byte 0x02009a79
	.4byte 0x0200b382
	.4byte 0x0200b484
	.2byte 0xb486
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200a990
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200a9a0
	ldr	r5, [pc, #80]
	bl 0x0200a9b8
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200a9b0
	adds	r0, r6, #0
	bl 0x0200a998
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200a958
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_02001db8
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b384
	.4byte 0x0200af02
	.4byte 0x0200b380
	.4byte 0x02009a79
	.4byte 0x0200b382
	.4byte 0x0200b484
	.2byte 0xb486
	.2byte 0x0200
.L_02001db8:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200aa98
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_02001de2
	adds	r3, r4, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #16]
	ldr	r0, [pc, #20]
	ldrh	r2, [r1, #0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	adds	r2, #1
	lsls	r3, r3, #2
	str	r4, [r0, r3]
	strh	r2, [r1, #0]
.L_02001de2:
	pop	{r5, pc}
	.4byte 0x0200b382
	.4byte 0x0200b384
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xb486
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02001e3c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001e3c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02001e3c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200aa98
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02001ec0
	cmp	r7, #0
	beq.n	.L_02001ec0
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02001ec8
.L_02001ec0:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02001ec8:
	mov	r3, sl
	bl 0x0200aa08
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02001ed6
	b.n	.L_02002022
.L_02001ed6:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200a9f0
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200aa00
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aa58
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02009df8
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02002022
	cmp	r7, #0
	beq.n	.L_02002022
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001f58
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200aac8
.L_02001f58:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001f78
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02009df8
.L_02001f78:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02001f8c
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02001f8c:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001fd2
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_02001fba
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200a940
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_02001fcc
.L_02001fba:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200a940
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_02001fcc:
	bl 0x0200a940
	str	r0, [r6, #52]
.L_02001fd2:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02001fee
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200a9f0
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200aa00
.L_02001fee:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002000
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02002000:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002012
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02002012:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002022
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02002022:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200b330
	.4byte 0x02009e41
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_0200214c
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl 0x0200aa98
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_0200208c
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02002094
.L_0200208c:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02002094:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_0200214c
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_0200214c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl 0x0200aa38
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_0200214c
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_0200214c
	cmp	r2, #239
	bgt.n	.L_0200214c
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl 0x0200a9c0
.L_0200214c:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b488
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_020022f0
.L_02002220:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_020022e4
.L_02002234:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_020022d4
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_020022d4
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_020022d4
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl 0x0200a9d0
	cmp	r0, #0
	bne.n	.L_02002288
	cmp	r5, sl
	bne.n	.L_020022c6
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200a9d8
	b.n	.L_020022c6
.L_02002288:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_020022c6
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200aa40
.L_020022c6:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_020022d4:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02002234
.L_020022e4:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_02002220
.L_020022f0:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200a9d0
	cmp	r0, #0
	beq.n	.L_02002348
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02002348
.L_02002322:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02002338
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02002338
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02002338:
	movs	r3, #8
	mov	r0, r8
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_02002322
.L_02002348:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200a990
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02002356:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02002356
	bl 0x0200a9b8
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200a9b0
	adds	r0, r5, #0
	bl 0x0200a998
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200a958
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b488
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xa041
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200aaa0
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200aab0
	movs	r0, #1
	bl 0x0200a950
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl 0x0200aa10
	movs	r0, #4
	bl 0x0200aa80
	bl 0x0200aaf8
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
.L_0200243a:
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200aa98
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200a3b0
	movs	r0, #161
	bl 0x0200ab68
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200aa40
	movs	r0, #12
	bl 0x0200aa80
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl 0x0200aa98
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
.L_020024d4:
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200a3b0
	movs	r0, #229
	bl 0x0200ab68
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200aa40
	movs	r0, #12
	bl 0x0200aa80
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	movs	r1, #0
	bl 0x0200aa58
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_0200255c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_0200255c:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200aac0
	movs	r0, #16
	bl 0x0200aa80
.L_02002570:
	cmp	r7, #5
	bne.n	.L_0200257a
	movs	r0, #204
	bl 0x0200ab68
.L_0200257a:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200a950
	cmp	r7, #39
	ble.n	.L_02002570
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200ab08
	bl 0x0200ab10
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200a940
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002618
	adds	r3, #15
.L_02002618:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200aa98
	adds	r7, r0, #0
	bl 0x0200aa88
	movs	r0, #0
	bl 0x0200ab18
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200aae8
	bl 0x0200aa18
	movs	r0, #1
	bl 0x0200a950
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200ab00
	bl 0x0200ab10
	movs	r0, #204
	bl 0x0200ab68
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200aa80
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020026da:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200a978
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200a970
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200a968
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200a968
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x02009e78
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020026da
	movs	r0, #188
	bl 0x0200ab68
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200aad8
	ldr	r0, [r5, #0]
	movs	r1, #49
	bl 0x0200aac0
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200aa60
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200aa60
	bl 0x0200aa68
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200aad8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200aa80
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200aac0
	bl 0x0200aa90
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a5e9
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200aa98
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_02002874
.L_0200280c:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02002868
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02002868
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200a9d0
	cmp	r0, #0
	bne.n	.L_0200283c
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200a438
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200a9d8
	strh	r7, [r6, #12]
	b.n	.L_02002874
.L_0200283c:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02002874
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200a4a8
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200a9e8
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200a9e8
	movs	r0, #1
	b.n	.L_02002876
.L_02002868:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_0200280c
.L_02002874:
	movs	r0, #0
.L_02002876:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xb488
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl 0x0200aa98
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200a9e0
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200a9e0
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020028d6
	cmp	r0, #0
	beq.n	.L_0200292a
.L_020028d6:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200a9e8
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200a9e8
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl 0x0200aa18
	bl 0x0200a640
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_0200292a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200b488
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000119, 0x08000121, 0x08000141, 0x08000151, 0x08000169, 0x08000179, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x080001e9, 0x08000291, 0x080003c9, 0x080003d1, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020139, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020219, 0x08020229, 0x08020231, 0x08020279, 0x08020361, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80b1, 0x080c80c9, 0x080c80f9, 0x080c8119, 0x080c8171, 0x080c81d1, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c1, 0x080c8781, 0x080c87c1, 0x080c87c9, 0x080c87d1, 0x080c87e9, 0x080c8831, 0x080c8849, 0x08108079, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x06345d01
	.4byte 0x08003b01
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte 0x08071768
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte 0x020010df
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte 0x02007800
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte 0x0800200e
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte 0x08076918
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.4byte 0x00000002
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte 0x08e7ee5a
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte 0x087d19d7
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.4byte 0x01000000
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0xffff0000
	.4byte 0x00600230
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000108
	.4byte 0x00101103
	.4byte 0x00202103
	.4byte 0x00303103
	.4byte 0x00404106
	.4byte 0x000001ff
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0xffff0153
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00028000
	.4byte 0xffff0153
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00020000
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00028000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff01a9
	.4byte 0x0000000b
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00020000
	.4byte 0xffff01a9
	.4byte 0x0000000b
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
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
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte 0x02008315
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x0200832d
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x02008315
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x0200832d
	.4byte 0x00000008
	.4byte 0xffffffff
	.4byte 0x02008315
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x0200832d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200afe4
	.4byte 0x0200b020
	.4byte 0x0200b05c
