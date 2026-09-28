.syntax unified
	.thumb
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200ad20
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200adf8
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200ae08
	push	{r5, lr}
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200aa34
	bl 0x0200aa44
	movs	r0, #5
	bl 0x0200a8cc
	movs	r1, #2
	bl 0x0200aa5c
	movs	r0, #20
	bl 0x0200a8ac
	adds	r0, r5, #0
	bl 0x0200a3a4
	movs	r0, #5
	bl 0x0200a8cc
	movs	r1, #0
	bl 0x0200aa5c
	bl 0x0200aa54
	bl 0x0200aa4c
	movs	r0, #8
	bl 0x0200aa3c
	pop	{r5, pc}
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, lr}
	adds r5, r0, #0
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	adds r5, #100
	bl 0x0200a60c
	ldrh r2, [r5]
	movs r3, #224
	adds r2, #1
	strh r2, [r5]
	lsls r3, r3, #11
	lsls r2, r2, #16
	ands r3, r2
	cmp r3, #0
	bne .L_0200009c_0
	movs r0, #125
	bl 0x0200aa64
.L_0200009c_0:
	pop {r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	bl 0x0200a8cc
	adds	r6, r0, #0
	bl 0x0200a6f4
	ldr	r3, [pc, #64]
	adds	r7, r6, #0
	str	r3, [r6, #108]
	movs	r3, #0
	mov	r8, r3
	mov	r3, r8
	adds	r7, #35
	movs	r2, #96
	strb	r3, [r7, #0]
	adds	r0, r5, #0
	movs	r1, #96
	negs	r2, r2
	bl 0x0200a9e4
	movs	r1, #96
	adds	r0, r5, #0
	negs	r1, r1
	movs	r2, #96
	bl 0x0200a9e4
	mov	r3, r8
	str	r3, [r6, #108]
	movs	r3, #1
	strb	r3, [r7, #0]
	movs	r0, #30
	bl 0x0200a8ac
	bl 0x0200a7b0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x809d
	.2byte 0x0200
	.global Func_02000120
	.thumb_func
Func_02000120:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200af88
	.global Func_02000128
	.thumb_func
Func_02000128:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #456]
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200a8cc
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #5
	movs	r0, #10
	bl 0x0200a914
	movs	r0, #10
	movs	r1, #1
	bl 0x0200a95c
	movs	r0, #11
	movs	r1, #5
	bl 0x0200a914
	movs	r0, #11
	movs	r1, #1
	bl 0x0200a95c
	movs	r0, #14
	movs	r1, #2
	bl 0x0200a95c
	movs	r0, #19
	movs	r1, #2
	bl 0x0200a95c
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #146
	bl 0x0200a84c
	cmp	r0, #0
	beq.n	.L_020001a4
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
.L_020001a4:
	movs	r0, #137
	lsls	r0, r0, #4
	bl 0x0200a84c
	cmp	r0, #0
	beq.n	.L_02000214
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #21
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
.L_02000214:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #9
	bne.n	.L_02000242
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #145
	bl 0x0200a84c
	cmp	r0, #0
	bne.n	.L_02000242
	bl 0x0200a8b4
	movs	r0, #0
	bl 0x0200a9cc
	bl 0x0200830c
	bl 0x0200a8bc
.L_02000242:
	ldr	r5, [pc, #188]
	movs	r3, #241
	lsls	r3, r3, #1
	adds	r6, r5, r3
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #98
	bne.n	.L_02000268
	movs	r3, #245
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r0, #1
	strh	r3, [r6, #0]
	bl 0x0200a8a4
	bl 0x02009c30
.L_02000268:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #99
	bne.n	.L_0200027a
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #147
	bl 0x0200a854
.L_0200027a:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #90
	bne.n	.L_020002a6
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	movs	r2, #5
	str	r2, [r3, #0]
	movs	r0, #4
	bl 0x0200a89c
	movs	r0, #6
	bl 0x0200a89c
	movs	r0, #7
	bl 0x0200a89c
	ldr	r0, [pc, #100]
	movs	r1, #1
	bl 0x0200a99c
.L_020002a6:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #147
	bl 0x0200a84c
	cmp	r0, #0
	bne.n	.L_020002c6
	movs	r0, #22
	movs	r1, #5
	bl 0x0200a914
	movs	r0, #22
	movs	r1, #1
	bl 0x0200a95c
	b.n	.L_020002fc
.L_020002c6:
	movs	r1, #160
	movs	r2, #248
	lsls	r1, r1, #14
	lsls	r2, r2, #16
	movs	r0, #22
	bl 0x0200a90c
	movs	r0, #22
	bl 0x0200a8cc
	movs	r1, #128
	lsls	r1, r1, #7
	strh	r1, [r0, #6]
	movs	r2, #0
	movs	r0, #22
	bl 0x0200aa04
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a95c
	movs	r0, #22
	bl 0x0200a8cc
	movs	r1, #8
	bl 0x0200a9f4
.L_020002fc:
	movs	r0, #0
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.2byte 0x0004
	.2byte 0x0000
	.global Func_02000308
	.thumb_func
Func_02000308:
	movs r0, #0
	bx lr
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #145
	bl 0x0200a854
	movs	r0, #10
	bl 0x0200a8cc
	movs	r2, #0
	adds	r3, r0, #0
	mov	r8, r2
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #200
	lsls	r3, r3, #1
	adds	r3, #255
	movs	r6, #192
	movs	r5, #128
	str	r3, [r0, #72]
	lsls	r6, r6, #9
	lsls	r5, r5, #8
	movs	r1, #216
	movs	r2, #192
	movs	r3, #176
	str	r6, [r0, #48]
	str	r5, [r0, #52]
	lsls	r1, r1, #16
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	bl 0x0200a87c
	movs	r0, #11
	bl 0x0200a8cc
	adds	r3, r0, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #174
	str	r3, [r0, #72]
	movs	r1, #154
	movs	r2, #192
	movs	r3, #152
	str	r6, [r0, #48]
	str	r5, [r0, #52]
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	lsls	r1, r1, #18
	bl 0x0200a87c
	movs	r0, #204
	movs	r1, #200
	lsls	r0, r0, #8
	lsls	r1, r1, #5
	adds	r0, #204
	adds	r1, #153
	bl 0x0200a97c
	movs	r0, #188
	movs	r1, #1
	movs	r2, #248
	lsls	r2, r2, #16
	movs	r3, #0
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a9bc
	bl 0x0200a9c4
	movs	r0, #10
	bl 0x0200a8cc
	movs	r1, #4
	adds	r5, r0, #0
	movs	r0, #10
	bl 0x0200a914
	movs	r0, #165
	bl 0x0200aa64
	movs	r1, #176
	movs	r2, #128
	movs	r3, #176
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	adds	r0, r5, #0
	lsls	r1, r1, #17
	bl 0x0200a884
	ldr	r6, [pc, #108]
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl 0x0200a864
	movs	r0, #25
	bl 0x0200a8ac
	movs	r0, #188
	movs	r1, #1
	movs	r2, #208
	lsls	r2, r2, #15
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #17
	bl 0x0200a984
	movs	r0, #11
	bl 0x0200a8cc
	movs	r1, #4
	adds	r5, r0, #0
	movs	r0, #11
	bl 0x0200a914
	movs	r1, #208
	movs	r2, #128
	movs	r3, #152
	lsls	r2, r2, #12
	lsls	r3, r3, #16
	adds	r0, r5, #0
	lsls	r1, r1, #17
	bl 0x0200a884
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl 0x0200a864
	movs	r0, #60
	bl 0x0200a8ac
	movs	r0, #165
	bl 0x0200aa64
	movs	r0, #10
	bl 0x0200a904
	movs	r0, #11
	bl 0x0200a904
	bl 0x0200a98c
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0xb000
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #146
	bl 0x0200a854
	bl 0x0200a8b4
	movs	r0, #0
	bl 0x0200a9cc
	movs	r0, #165
	bl 0x0200aa64
	movs	r0, #10
	movs	r1, #3
	bl 0x0200a914
	movs	r1, #3
	movs	r0, #11
	bl 0x0200a914
	movs	r0, #21
	bl 0x0200a8ac
	movs	r0, #10
	bl 0x0200a8cc
	movs	r2, #0
	adds	r5, r0, #0
	mov	r9, r2
	adds	r3, r5, #0
	mov	r2, r9
	adds	r3, #85
	strb	r2, [r3, #0]
	movs	r6, #128
	movs	r3, #128
	lsls	r3, r3, #10
	lsls	r6, r6, #8
	str	r3, [r5, #48]
	str	r6, [r5, #52]
	movs	r0, #10
	movs	r1, #4
	mov	sl, r3
	bl 0x0200a914
	movs	r1, #154
	movs	r2, #192
	movs	r3, #176
	lsls	r3, r3, #16
	adds	r0, r5, #0
	lsls	r1, r1, #18
	lsls	r2, r2, #15
	bl 0x0200a884
	ldr	r2, [pc, #80]
	adds	r0, r5, #0
	mov	r8, r2
	mov	r1, r8
	bl 0x0200a864
	movs	r0, #11
	bl 0x0200a8cc
	adds	r5, r0, #0
	adds	r3, r5, #0
	mov	r2, r9
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	str	r3, [r5, #48]
	str	r6, [r5, #52]
	movs	r0, #11
	movs	r1, #4
	bl 0x0200a914
	movs	r1, #240
	movs	r2, #192
	movs	r3, #176
	adds	r0, r5, #0
	lsls	r2, r2, #15
	lsls	r3, r3, #16
	lsls	r1, r1, #15
	bl 0x0200a884
	adds	r0, r5, #0
	mov	r1, r8
	bl 0x0200a864
	bl 0x0200a8bc
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xb040
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #137
	lsls	r0, r0, #4
	bl 0x0200a854
	bl 0x0200a8b4
	movs	r0, #0
	bl 0x0200a9cc
	ldr	r0, [pc, #1016]
	bl 0x0200a93c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #12
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200a8d4
	movs	r1, #128
	movs	r2, #128
.L_02000548:
	movs	r0, #13
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200a8d4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a94c
	movs	r1, #188
	movs	r2, #140
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #5
	bl 0x0200a8f4
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #5
	bl 0x0200a964
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #5
	bl 0x0200a934
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #188
	movs	r2, #180
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #5
	bl 0x0200a8ec
	movs	r0, #30
	bl 0x0200a8ac
	movs	r0, #188
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #5
	bl 0x0200a904
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #12
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #13
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #13
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #17
	bl 0x0200a964
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #18
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a924
	movs	r0, #18
	movs	r1, #6
	movs	r2, #25
	bl 0x0200a924
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #15
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #19
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #19
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #20
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #19
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #16
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #16
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #16
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #16
	movs	r1, #64
	movs	r2, #0
	bl 0x0200a9e4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #16
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #20
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #21
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #20
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #21
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #21
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #64
	movs	r0, #21
	negs	r1, r1
	movs	r2, #64
	bl 0x0200a9e4
	movs	r1, #0
	movs	r2, #0
	movs	r0, #21
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #160
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #160
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #224
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #20
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r0, #188
	movs	r1, #1
	movs	r2, #180
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #16
	movs	r2, #16
	movs	r3, #128
	movs	r0, #8
	negs	r1, r1
	negs	r2, r2
	lsls	r3, r3, #7
	bl 0x0200a9d4
	movs	r2, #16
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #16
	negs	r2, r2
	movs	r0, #9
	bl 0x0200a9d4
	movs	r0, #8
	bl 0x0200a904
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200a8d4
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #8
	bl 0x0200a8d4
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #16
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a9e4
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #8
	bl 0x0200a934
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #40
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200a964
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a94c
	movs	r1, #0
	movs	r2, #16
	movs	r0, #9
	bl 0x0200a9e4
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #0
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #60
	b.n	.L_02000930
	.2byte 0x15b7
	.2byte 0x0000
.L_02000930:
	movs	r0, #5
	bl 0x0200a964
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	bl 0x0200a8d4
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #16
	bl 0x0200a9e4
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200a964
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a944
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a8c4
	cmp	r0, #0
	bne.n	.L_02000a7c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_02000ac2
.L_02000a7c:
	movs	r0, #30
	bl 0x0200a8ac
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #4
	bl 0x0200a91c
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
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
.L_02000ac2:
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a914
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #156
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #0
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a954
	movs	r0, #40
	bl 0x0200a8ac
	movs	r1, #224
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #224
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #18
	bl 0x0200a964
	movs	r0, #18
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #188
	movs	r1, #1
	movs	r2, #212
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #25
	bl 0x0200a8ac
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #188
	movs	r1, #1
	movs	r2, #180
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #5
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a914
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #188
	movs	r1, #1
	movs	r2, #204
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200a8d4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #16
	bl 0x0200a9e4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #240
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #240
	movs	r0, #18
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #240
	movs	r0, #19
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #20
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #12
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #12
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200a91c
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #13
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #13
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #144
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #25
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r0, #12
	movs	r1, #3
	bl 0x0200a91c
	movs	r1, #3
	movs	r0, #13
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #160
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #160
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #13
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #12
	ldr	r1, [pc, #1016]
	adds	r2, #153
	bl 0x0200a8d4
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #13
	ldr	r1, [pc, #1004]
	adds	r2, #153
	bl 0x0200a8d4
	movs	r1, #26
	movs	r0, #12
	negs	r1, r1
	movs	r2, #8
	bl 0x0200a9e4
	movs	r1, #176
	movs	r0, #12
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #16
	movs	r2, #12
	movs	r0, #13
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a9e4
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #13
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #9
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	movs	r1, #0
	adds	r0, #9
	bl 0x0200a94c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #8
	movs	r0, #9
	bl 0x0200a914
	movs	r0, #50
	bl 0x0200a8ac
	bl 0x0200aa44
	movs	r0, #9
	bl 0x0200a8cc
	movs	r1, #2
	bl 0x0200aa5c
	movs	r0, #178
	bl 0x0200aa64
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r1, #132
	movs	r0, #8
	bl 0x0200aa34
	movs	r0, #60
	bl 0x0200a8ac
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x0200aa64
	movs	r1, #0
	movs	r0, #12
	bl 0x0200a95c
	movs	r0, #12
	bl 0x0200a3a4
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #17
	bl 0x0200a964
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #18
	bl 0x0200a964
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #19
	bl 0x0200a964
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #20
	bl 0x0200a964
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #13
	bl 0x0200a964
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a964
	movs	r1, #128
	movs	r2, #50
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a964
	movs	r1, #1
	movs	r0, #13
	bl 0x0200a95c
	movs	r0, #13
	bl 0x0200a3a4
	movs	r0, #30
	bl 0x0200a8ac
	movs	r0, #9
	bl 0x0200a8cc
	movs	r1, #0
	bl 0x0200aa5c
	bl 0x0200aa54
	bl 0x0200aa4c
	movs	r0, #16
	bl 0x0200aa3c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #9
	movs	r0, #9
	bl 0x0200a914
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #14
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #14
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #14
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200a96c
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a96c
	movs	r0, #50
	bl 0x0200a8ac
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	movs	r0, #15
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r0, #14
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a924
	movs	r0, #14
	movs	r1, #6
	movs	r2, #25
	bl 0x0200a924
	movs	r0, #14
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #192
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200a964
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r1, #16
	movs	r0, #9
	negs	r1, r1
	movs	r2, #16
	bl 0x0200a9e4
	movs	r1, #192
	movs	r0, #9
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a954
	movs	r0, #180
	movs	r1, #1
	movs	r2, #204
	movs	r3, #1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #17
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	movs	r0, #17
	lsls	r1, r1, #1
	bl 0x0200a96c
	movs	r0, #18
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	movs	r0, #18
	lsls	r1, r1, #1
	bl 0x0200a96c
	movs	r0, #19
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	movs	r0, #19
	lsls	r1, r1, #1
	bl 0x0200a96c
	movs	r0, #20
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	movs	r0, #20
	lsls	r1, r1, #1
	bl 0x0200a96c
	movs	r0, #21
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #21
	bl 0x0200a96c
	movs	r0, #50
	bl 0x0200a8ac
	movs	r0, #17
	movs	r1, #4
	bl 0x0200a91c
	movs	r0, #17
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #18
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #18
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #19
	movs	r1, #4
	bl 0x0200a91c
	movs	r2, #10
	movs	r0, #19
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #20
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #20
	bl 0x0200a96c
	movs	r0, #55
	bl 0x0200a8ac
	movs	r1, #128
	movs	r0, #17
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #160
	movs	r0, #19
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #224
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #20
	bl 0x0200a954
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #20
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #20
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #18
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a924
	movs	r0, #19
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a924
	movs	r0, #20
	movs	r1, #6
	movs	r2, #25
	bl 0x0200a924
	bl 0x0200a6f4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #20
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #19
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #18
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r1, #230
	movs	r2, #230
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	adds	r2, #102
	adds	r1, #204
	movs	r0, #17
	bl 0x0200a8d4
	movs	r0, #20
	bl 0x0200a8cc
	ldr	r6, [pc, #40]
	adds	r7, r0, #0
	adds	r3, r7, #0
	movs	r5, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	movs	r0, #19
	str	r6, [r7, #108]
	bl 0x0200a8cc
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	movs	r0, #18
	str	r6, [r7, #108]
	bl 0x0200a8cc
	adds	r7, r0, #0
	b.n	.L_02001228
	.4byte 0x00013333
	.2byte 0x809d
	.2byte 0x0200
.L_02001228:
	adds	r3, r7, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	movs	r0, #20
	str	r6, [r7, #108]
	movs	r1, #10
	bl 0x0200a914
	movs	r0, #19
	movs	r1, #10
	bl 0x0200a914
	movs	r0, #18
	movs	r1, #10
	bl 0x0200a914
	movs	r1, #24
	movs	r0, #20
	negs	r1, r1
	movs	r2, #24
	bl 0x0200a8fc
	movs	r1, #24
	movs	r0, #19
	negs	r1, r1
	movs	r2, #24
	bl 0x0200a8fc
	movs	r1, #24
	negs	r1, r1
	movs	r2, #24
	movs	r0, #18
	bl 0x0200a8fc
	movs	r0, #18
	bl 0x0200a904
	movs	r2, #240
	movs	r0, #20
	movs	r1, #224
	lsls	r2, r2, #1
	bl 0x0200a8dc
	movs	r2, #240
	movs	r0, #19
	movs	r1, #224
	lsls	r2, r2, #1
	bl 0x0200a8dc
	movs	r2, #240
	movs	r1, #224
	lsls	r2, r2, #1
	movs	r0, #18
	bl 0x0200a8e4
	movs	r0, #20
	bl 0x0200a8cc
	adds	r7, r0, #0
	str	r5, [r7, #108]
	movs	r0, #19
	bl 0x0200a8cc
	adds	r7, r0, #0
	str	r5, [r7, #108]
	movs	r0, #18
	bl 0x0200a8cc
	adds	r7, r0, #0
	str	r5, [r7, #108]
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #17
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a924
	movs	r1, #6
	movs	r2, #25
	movs	r0, #17
	bl 0x0200a924
	movs	r0, #17
	bl 0x0200a8cc
	movs	r2, #100
	adds	r7, r0, #0
	adds	r2, r2, r7
	mov	sl, r2
	mov	r3, sl
	strh	r5, [r3, #0]
	movs	r0, #17
	str	r6, [r7, #108]
	movs	r1, #10
	bl 0x0200a914
	movs	r1, #20
	negs	r1, r1
	movs	r2, #20
	movs	r0, #17
	bl 0x0200a8fc
	movs	r0, #17
	bl 0x0200a904
	movs	r0, #17
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a924
	movs	r1, #20
	movs	r2, #20
	negs	r1, r1
	movs	r0, #17
	bl 0x0200a8fc
	movs	r0, #17
	bl 0x0200a904
	movs	r1, #1
	movs	r0, #17
	bl 0x0200a914
	movs	r0, #133
	bl 0x0200aa64
	ldr	r3, [r7, #8]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r2, [pc, #1016]
	ldr	r3, [r7, #16]
	str	r5, [r7, #108]
	adds	r3, r3, r2
	str	r3, [r7, #16]
	movs	r3, #192
	lsls	r3, r3, #8
	mov	r8, r3
	mov	r2, r8
	strh	r2, [r7, #6]
	movs	r2, #160
	ldr	r3, [r7, #80]
	lsls	r2, r2, #8
	mov	fp, r2
	mov	r2, fp
	strh	r2, [r3, #18]
	movs	r0, #55
	bl 0x0200a8ac
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #980]
	movs	r0, #17
	adds	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r9, r3
	ldr	r3, [r7, #80]
	mov	r2, r9
	strh	r5, [r3, #18]
	strh	r2, [r7, #6]
	movs	r1, #6
	movs	r2, #40
	bl 0x0200a924
	movs	r1, #11
	movs	r0, #17
	bl 0x0200a914
	movs	r0, #135
	bl 0x0200aa64
	movs	r0, #14
	bl 0x0200a8ac
	movs	r0, #135
	bl 0x0200aa64
	movs	r0, #14
	bl 0x0200a8ac
	movs	r0, #135
	bl 0x0200aa64
	movs	r0, #14
	bl 0x0200a8ac
	movs	r0, #135
	bl 0x0200aa64
	movs	r0, #14
	bl 0x0200a8ac
	movs	r0, #20
	bl 0x0200a8ac
	mov	r3, sl
	strh	r5, [r3, #0]
	movs	r0, #17
	str	r6, [r7, #108]
	movs	r1, #10
	bl 0x0200a914
	movs	r2, #236
	movs	r1, #224
	lsls	r2, r2, #1
	movs	r0, #17
	bl 0x0200a8e4
	movs	r0, #30
	bl 0x0200a8ac
	str	r5, [r7, #108]
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #17
	bl 0x0200a90c
	bl 0x0200a7b0
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #4
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #188
	movs	r1, #1
	movs	r2, #204
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x0200a984
	bl 0x0200a98c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r0, #5
	mov	r1, r9
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #15
	bl 0x0200a8ac
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	mov	r1, fp
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a954
	movs	r0, #40
	bl 0x0200a8ac
	movs	r1, #128
	movs	r0, #14
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #15
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #192
	mov	r2, r8
	movs	r0, #14
	lsls	r1, r1, #9
	bl 0x0200a8d4
	movs	r1, #32
	negs	r1, r1
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a9e4
	movs	r0, #9
	bl 0x0200a8cc
	movs	r6, #128
	lsls	r6, r6, #6
	strh	r6, [r0, #6]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #192
	mov	r2, r8
	movs	r0, #9
	lsls	r1, r1, #9
	bl 0x0200a8d4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #16
	bl 0x0200a9e4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #14
	movs	r1, #3
	bl 0x0200a95c
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #14
	bl 0x0200a8d4
	movs	r0, #14
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r1, #228
	movs	r2, #212
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200a8f4
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #14
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #1
	mov	r8, r2
	mov	r2, r8
	orrs	r3, r2
	movs	r1, #192
	strb	r3, [r0, #0]
	lsls	r1, r1, #7
	movs	r0, #14
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	lsls	r1, r1, #9
	movs	r0, #14
	bl 0x0200a8d4
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #179
	movs	r2, #178
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #51
	adds	r2, #153
	bl 0x0200a8d4
	movs	r0, #9
	movs	r1, #48
	movs	r2, #0
	bl 0x0200a9e4
	movs	r1, #128
	movs	r0, #9
	lsls	r1, r1, #5
	movs	r2, #0
	bl 0x0200a954
	adds	r1, r6, #0
	movs	r0, #5
	movs	r2, #0
	bl 0x0200a954
	adds	r1, r6, #0
	movs	r0, #8
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	movs	r0, #15
	bl 0x0200a8d4
	movs	r0, #15
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #228
	ands	r5, r3
	movs	r2, #220
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	strb	r5, [r0, #0]
	movs	r0, #15
	bl 0x0200a8f4
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #15
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #9
	bl 0x0200a964
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #14
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #14
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #16
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a9e4
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #9
	bl 0x0200a964
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #14
	movs	r1, #2
	bl 0x0200a92c
	movs	r1, #2
	movs	r0, #15
	bl 0x0200a934
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #14
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	bl 0x0200a8d4
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #15
	bl 0x0200a8d4
	movs	r0, #14
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r6, #254
	adds	r3, r6, #0
	ands	r3, r2
	movs	r1, #244
	movs	r2, #212
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200a8f4
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #14
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	movs	r1, #244
	movs	r2, #220
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #15
	bl 0x0200a8f4
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #15
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	movs	r1, #228
	movs	r2, #216
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #9
	lsls	r2, r2, #1
	b.n	.L_02001734
	.2byte 0x0000
	.4byte 0xfff80000
	.2byte 0x0000
	.2byte 0xfffc
.L_02001734:
	.2byte 0xf001
	.2byte 0xf8de
	.2byte 0x2200
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #128
	lsls	r0, r0, #5
	movs	r2, #10
	adds	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #14
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	movs	r0, #14
	lsls	r1, r1, #1
	bl 0x0200a96c
	movs	r0, #15
	movs	r1, #3
	bl 0x0200a92c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #15
	bl 0x0200a96c
	movs	r0, #50
	bl 0x0200a8ac
	movs	r0, #14
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	ands	r3, r2
	movs	r1, #252
	movs	r2, #212
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl 0x0200a8f4
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #14
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #0
	movs	r2, #0
	movs	r0, #14
	bl 0x0200a90c
	movs	r0, #15
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r1, #252
	ands	r6, r3
	movs	r2, #220
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	strb	r6, [r0, #0]
	movs	r0, #15
	bl 0x0200a8f4
	movs	r0, #1
	bl 0x0200a8ac
	movs	r0, #15
	bl 0x0200a8cc
	adds	r0, #90
	ldrb	r3, [r0, #0]
	mov	r2, r8
	orrs	r2, r3
	strb	r2, [r0, #0]
	movs	r1, #0
	movs	r0, #15
	mov	r8, r2
	movs	r2, #0
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r0, #9
	movs	r1, #64
	movs	r2, #0
	bl 0x0200a9e4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #9
	bl 0x0200a90c
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #1
	movs	r0, #5
	bl 0x0200a994
	bl 0x0200a98c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #8
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #0
	movs	r2, #10
	movs	r0, #8
	bl 0x0200a94c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a924
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #50
	movs	r0, #5
	bl 0x0200a964
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #5
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200a964
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #10
	adds	r1, #255
	movs	r2, #80
	movs	r0, #5
	bl 0x0200a964
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #131
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200a964
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a944
	movs	r0, #5
	movs	r1, #0
	bl 0x0200a8c4
	cmp	r0, #0
	bne.n	.L_0200194c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200196e
.L_0200194c:
	movs	r0, #40
	bl 0x0200a8ac
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
	movs	r2, #10
	bl 0x0200a94c
.L_0200196e:
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #160
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #76]
	adds	r2, #153
	bl 0x0200a8d4
	movs	r0, #8
	movs	r1, #2
	bl 0x0200a914
	movs	r0, #5
	bl 0x0200a8cc
	cmp	r0, #0
	beq.n	.L_02001a00
	movs	r2, #10
	ldrsh	r1, [r0, r2]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x0200a8dc
.L_02001a00:
	movs	r0, #8
	bl 0x0200a904
	movs	r2, #0
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a90c
	movs	r0, #5
	movs	r1, #1
	bl 0x0200a974
	bl 0x0200a8bc
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, lr}
	bl 0x0200a8b4
	movs	r0, #0
	bl 0x0200a9cc
	ldr	r0, [pc, #360]
	bl 0x0200a93c
	movs	r2, #133
	movs	r0, #5
	movs	r1, #72
	lsls	r2, r2, #1
	bl 0x0200a8f4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #1
	movs	r0, #22
	bl 0x0200a914
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #2
	adds	r1, #255
	movs	r2, #50
	movs	r0, #22
	bl 0x0200a964
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #22
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a924
	movs	r0, #22
	movs	r1, #6
	movs	r2, #35
	bl 0x0200a924
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #22
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #22
	movs	r1, #12
	bl 0x0200a914
	movs	r2, #0
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #22
	movs	r1, #1
	bl 0x0200a914
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #50
	movs	r0, #22
	bl 0x0200a964
	movs	r1, #0
	movs	r2, #0
	movs	r0, #22
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #22
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #22
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #128
	movs	r2, #128
	movs	r0, #22
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x0200a8d4
	movs	r1, #16
	movs	r2, #0
	movs	r0, #22
	bl 0x0200a9e4
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #22
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #128
	movs	r2, #128
	movs	r0, #22
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200a8d4
	movs	r0, #22
	movs	r1, #6
	movs	r2, #15
	bl 0x0200a924
	movs	r0, #22
	movs	r1, #6
	movs	r2, #25
	bl 0x0200a924
	movs	r0, #22
	movs	r1, #6
	movs	r2, #0
	bl 0x0200a924
	movs	r2, #0
	movs	r1, #26
	movs	r0, #22
	bl 0x0200a9dc
	movs	r0, #8
	bl 0x0200a8ac
	movs	r0, #22
	movs	r1, #1
	bl 0x0200a914
	ldr	r5, [pc, #48]
	movs	r1, #99
	adds	r0, r5, #0
	bl 0x0200a9ac
	adds	r0, r5, #0
	movs	r1, #98
	bl 0x0200a9b4
	ldr	r3, [pc, #32]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a9a4
	bl 0x0200a8bc
	pop	{r5, pc}
	.4byte 0x00001604
	.4byte 0x00000004
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200a8b4
	movs	r0, #0
	bl 0x0200a9cc
	ldr	r0, [pc, #108]
	bl 0x0200a93c
	movs	r3, #128
	lsls	r3, r3, #8
	movs	r1, #16
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a9d4
	movs	r0, #8
	bl 0x0200a904
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r0, #8
	movs	r1, #2
	bl 0x0200a914
	movs	r0, #5
	bl 0x0200a8cc
	cmp	r0, #0
	beq.n	.L_02001c04
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x0200a8dc
.L_02001c04:
	movs	r0, #8
	bl 0x0200a904
	movs	r1, #0
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #16
	movs	r0, #5
	negs	r1, r1
	movs	r2, #0
	bl 0x0200a9e4
	bl 0x0200a8bc
	pop	{pc}
	.2byte 0x1615
	.2byte 0x0000
	push	{r5, lr}
	bl 0x0200a8b4
	movs	r0, #0
	bl 0x0200a9cc
	movs	r1, #182
	movs	r2, #156
	movs	r0, #5
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a90c
	movs	r1, #181
	movs	r2, #142
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a90c
	movs	r1, #195
	movs	r2, #144
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #16
	bl 0x0200a90c
	movs	r1, #128
	movs	r0, #5
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #7
	bl 0x0200a954
	movs	r0, #8
	movs	r1, #2
	bl 0x0200a95c
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #5
	bl 0x0200a9fc
	movs	r0, #5
	bl 0x0200a8cc
	movs	r5, #192
	movs	r1, #0
	bl 0x0200a88c
	lsls	r5, r5, #18
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a994
	ldr	r3, [r5, #108]
	movs	r2, #218
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #60
	str	r2, [r3, #0]
	bl 0x0200a9bc
	bl 0x0200a9c4
	movs	r0, #20
	bl 0x0200a8ac
	ldr	r0, [pc, #760]
	bl 0x0200a93c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #40
	movs	r0, #8
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200a934
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #8
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #5
	bl 0x0200a934
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #188
	movs	r2, #164
	lsls	r2, r2, #16
	movs	r0, #5
	lsls	r1, r1, #17
	bl 0x0200a90c
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a9fc
	movs	r0, #5
	bl 0x0200a8cc
	movs	r1, #1
	bl 0x0200a88c
	movs	r2, #50
	movs	r0, #5
	movs	r1, #6
	bl 0x0200a924
	movs	r1, #4
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #14
	movs	r2, #2
	movs	r0, #5
	negs	r1, r1
	negs	r2, r2
	bl 0x0200a9e4
	movs	r1, #192
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #0
	movs	r2, #10
	movs	r0, #9
	bl 0x0200a94c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a954
	movs	r1, #240
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r0, #9
	movs	r1, #4
	bl 0x0200a91c
	movs	r0, #9
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #16
	bl 0x0200a9e4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #2
	movs	r0, #9
	bl 0x0200a934
	movs	r0, #10
	bl 0x0200a8ac
	movs	r2, #10
	movs	r0, #9
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #9
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #5
	movs	r1, #3
	bl 0x0200a914
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #5
	movs	r1, #9
	bl 0x0200a9ec
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #9
	adds	r1, #204
	adds	r2, #102
.L_02001e96:
	bl 0x0200a8d4
	movs	r1, #0
	movs	r2, #120
	movs	r0, #9
	bl 0x0200a9dc
	movs	r0, #40
	bl 0x0200a8ac
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200a8d4
	movs	r0, #8
	movs	r1, #24
	movs	r2, #0
	bl 0x0200a9e4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #20
	bl 0x0200a9e4
	movs	r1, #192
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #9
	bl 0x0200a904
	movs	r1, #0
	movs	r2, #0
	movs	r0, #9
	bl 0x0200a90c
	movs	r0, #10
	bl 0x0200a8ac
	movs	r1, #6
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200a964
	movs	r2, #10
	movs	r0, #8
	movs	r1, #0
	bl 0x0200a94c
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #30
	bl 0x0200a8ac
	movs	r1, #2
	movs	r0, #8
	bl 0x0200a934
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #4
	movs	r2, #0
	bl 0x0200a924
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #8
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r0, #8
	movs	r1, #0
	movs	r2, #10
	bl 0x0200a94c
	movs	r2, #0
	movs	r1, #0
	movs	r0, #5
	bl 0x0200a954
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #5
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r1, #3
	movs	r0, #8
	bl 0x0200a91c
	movs	r0, #20
	bl 0x0200a8ac
	movs	r2, #153
	lsls	r2, r2, #8
	movs	r0, #8
	ldr	r1, [pc, #76]
	adds	r2, #153
	bl 0x0200a8d4
	movs	r0, #8
	movs	r1, #2
	bl 0x0200a914
	movs	r0, #5
	bl 0x0200a8cc
	cmp	r0, #0
	beq.n	.L_02001fa4
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #8
	bl 0x0200a8dc
.L_02001fa4:
	movs	r0, #8
	bl 0x0200a904
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200a90c
	ldr	r3, [r5, #108]
	movs	r2, #218
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #8
	str	r2, [r3, #0]
	bl 0x0200a8bc
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000015f9
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r0, #132
	lsls	r0, r0, #4
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #68
	mov	sl, r3
	adds	r0, #20
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	adds	r3, r6, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #4
	cmp	r3, #7
	bls.n	.L_02002004
	b.n	.L_020021fc
.L_02002004:
	ldr	r2, [pc, #396]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0200a02c
	.4byte 0x0200a040
	.4byte 0x0200a076
	.4byte 0x0200a08c
	.4byte 0x0200a0e4
	.4byte 0x0200a11e
	.4byte 0x0200a152
	.4byte 0x0200a19c
	.4byte 0x011b2380
	.4byte 0x18f13356
	.4byte 0x5e0b2000
	.4byte 0xd0002b00
	.4byte 0xe062e0de
	.4byte 0x011b2380
	.4byte 0x18f2335c
	.4byte 0x20f88813
	.4byte 0x80133304
	.4byte 0x041b0340
	.4byte 0xdc004283
	.4byte 0x2380e0d0
	.4byte 0x3354011b
	.4byte 0x881318f2
	.4byte 0x33012080
	.4byte 0x01008013
	.4byte 0x305623ff
	.4byte 0x1832021b
	.4byte 0x2280e06a
	.4byte 0x32560112
	.4byte 0x200018b1
	.4byte 0x2b5a5e0b
	.4byte 0xe0b9d000
	.4byte 0xe03f3a02
	.4byte 0x011b2380
	.4byte 0x18f53356
	.4byte 0x5e292000
	.4byte 0xd10f2900
	.4byte 0x32554652
	.4byte 0x20802303
	.4byte 0x01007013
	.4byte 0x035b23c0
	.4byte 0x4652305e
	.4byte 0x18336293
	.4byte 0x20867019
	.4byte 0xfcd4f000
	.4byte 0x011b2380
	.4byte 0x18f2335c
	.4byte 0x209e8813
	.4byte 0x80133308
	.4byte 0x041b03c0
	.4byte 0xdc004283
	.4byte 0x2280e092
	.4byte 0x32540112
	.4byte 0x881a18b3
	.4byte 0xe0863201
	.4byte 0x011b2380
	.4byte 0x18f13356
	.4byte 0x5e0b2000
	.4byte 0xd1032b0a
	.4byte 0x46522300
	.4byte 0x60936113
	.4byte 0x5e0b2000
	.4byte 0xd0002b1e
	.4byte 0x2280e07a
	.4byte 0x32540112
	.4byte 0x881a18b3
	.4byte 0x801a3201
	.4byte 0x021b23ff
	.4byte 0x800b33ff
	.4byte 0x2380e06e
	.4byte 0x335c011b
	.4byte 0x881318f2
	.4byte 0x80133b08
	.4byte 0x2b00041b
	.4byte 0x2080dc64
	.4byte 0x30540100
	.4byte 0x88131832
	.4byte 0x80133301
	.4byte 0x011b2380
	.4byte 0x18f23356
	.4byte 0x021b23ff
	.4byte 0x801333ff
	.4byte 0x698be054
	.4byte 0xdd022b00
	.4byte 0x181b480f
	.4byte 0x2300e000
	.4byte 0x61cb618b
	.4byte 0x01122280
	.4byte 0x18b53256
	.4byte 0x5e2b2000
	.4byte 0xd1032b0a
	.4byte 0x0040209b
	.4byte 0xfc74f000
	.4byte 0x5eab2200
	.4byte 0xd13b2b5a
	.4byte 0x01002080
	.4byte 0x18333054
	.4byte 0x3201881a
	.4byte 0x0000e02f
	.4byte 0x0200a00c
	.4byte 0xfffffae2
	.4byte 0x01122280
	.4byte 0x18b53256
	.4byte 0x5e2b2000
	.4byte 0xd10a2b00
	.4byte 0xf0002090
	.4byte 0x20c0fc59
	.4byte 0x228021c0
	.4byte 0x02890280
	.4byte 0xf0000252
	.4byte 0x2200fb69
	.4byte 0x2b1e5eab
	.4byte 0x22e6d118
	.4byte 0x21012001
	.4byte 0x42400212
	.4byte 0x32664249
	.4byte 0xfb5cf000
	.4byte 0x5e2b2000
	.4byte 0xd10b2b1e
	.4byte 0x01122280
	.4byte 0x18b33254
	.4byte 0x009222ba
	.4byte 0x801a32ff
	.4byte 0x021b23ff
	.2byte 0x33ff
	.2byte 0x802b
.L_020021fc:
	mov	r0, sl
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #92
	strh	r3, [r0, #6]
	adds	r3, r6, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	bgt.n	.L_0200221a
	b.n	.L_0200238c
.L_0200221a:
	movs	r2, #0
	mov	r8, r2
	subs	r3, #8
	movs	r4, #0
	cmp	r8, r3
	bge.n	.L_020022c2
	movs	r3, #128
	movs	r2, #208
	lsls	r3, r3, #3
	lsls	r2, r2, #3
	adds	r0, r6, r3
	adds	r1, r6, r2
.L_02002232:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #72
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	adds	r5, r1, #0
	str	r3, [r5, #0]
	adds	r2, #4
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	lsls	r2, r4, #16
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r5, #4]
	movs	r2, #133
	lsls	r2, r2, #4
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	str	r3, [r5, #8]
	ldr	r3, [r5, #24]
	movs	r0, #128
	adds	r3, #1
	str	r3, [r5, #24]
	lsls	r0, r0, #4
	adds	r0, #88
	adds	r1, r6, r0
	ldrh	r1, [r1, #0]
	movs	r2, #3
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	ands	r1, r3
	ldrh	r3, [r7, #8]
	adds	r0, r7, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	adds	r1, r5, #0
	str	r4, [sp, #0]
	bl 0x0200aa14
	movs	r2, #1
	ldr	r4, [sp, #0]
	add	r8, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	mov	r3, r8
	adds	r0, #40
	adds	r1, #28
	adds	r4, #16
	cmp	r3, #15
	bgt.n	.L_0200238c
	b.n	.L_020022b0
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_020022b0:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #92
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #8
	cmp	r4, r3
	blt.n	.L_02002232
.L_020022c2:
	mov	r3, r8
	cmp	r3, #15
	bgt.n	.L_0200238c
	mov	r0, r8
	lsls	r3, r3, #3
	subs	r3, r3, r0
	movs	r0, #128
	lsls	r3, r3, #2
	movs	r2, #208
	lsls	r0, r0, #4
	adds	r3, r6, r3
	lsls	r2, r2, #3
	adds	r0, #72
	adds	r5, r3, r2
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	movs	r2, #128
	str	r3, [r5, #0]
	adds	r0, #20
	lsls	r2, r2, #4
	adds	r3, r6, r0
	adds	r2, #76
	adds	r1, r6, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [r1, #0]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r5, #4]
	movs	r2, #133
	lsls	r2, r2, #4
	adds	r3, r6, r2
	ldr	r0, [r5, #24]
	ldr	r3, [r3, #0]
	adds	r0, #1
	str	r3, [r5, #8]
	str	r0, [r5, #24]
	lsls	r0, r0, #12
	bl 0x0200a804
	ldr	r3, [pc, #108]
	adds	r1, r0, #0
	ldr	r0, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x686b
	mov	r2, r8
	adds	r3, r3, r0
	ldr	r0, [pc, #96]
	adds	r3, r3, r0
	str	r3, [r5, #4]
	lsls	r3, r2, #2
	add	r3, r8
	lsls	r3, r3, #3
	movs	r0, #128
	adds	r3, r6, r3
	lsls	r0, r0, #3
	adds	r7, r3, r0
	movs	r2, #128
	ldr	r3, [r5, #24]
	lsls	r2, r2, #4
	adds	r2, #90
	adds	r1, r6, r2
	asrs	r3, r3, #2
	movs	r2, #3
	ands	r3, r2
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #44]
	ldrh	r1, [r7, #8]
	ands	r2, r3
	ldr	r3, [pc, #52]
	adds	r0, r7, #0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #8]
	adds	r1, r5, #0
	bl 0x0200aa14
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #94
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0200238c
	ldr	r3, [r5, #4]
	mov	r2, sl
	str	r3, [r2, #12]
	b.n	.L_0200238c
	.4byte 0x000003ff
	.4byte 0x0300021c
	.4byte 0xfffe0000
	.2byte 0xfc00
	.2byte 0xffff
.L_0200238c:
	.2byte 0x2380
	lsls	r3, r3, #4
	adds	r3, #86
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	add	sp, #4
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #134
	adds	r5, r0, #0
	lsls	r1, r1, #4
	movs	r0, #220
	sub	sp, #4
	bl 0x0200a814
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl 0x0200a8cc
	adds	r6, r0, #0
	adds	r2, r6, #0
	movs	r1, #132
	movs	r3, #0
	adds	r2, #91
	lsls	r1, r1, #4
	strb	r3, [r2, #0]
	adds	r3, r7, r1
	str	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #72
	adds	r2, r7, r3
	ldr	r3, [r6, #8]
	adds	r1, #12
	str	r3, [r2, #0]
	adds	r2, r7, r1
	ldr	r3, [r6, #12]
	movs	r1, #128
	str	r3, [r2, #0]
	movs	r3, #133
	lsls	r3, r3, #4
	adds	r2, r7, r3
	ldr	r3, [r6, #16]
	lsls	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r2, #0]
	adds	r1, r7, #0
	ldr	r0, [pc, #284]
	bl 0x0200a824
	bl 0x0200a83c
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r2, r7, #0
	mov	fp, r0
	bl 0x0200a834
	movs	r2, #128
	lsls	r2, r2, #4
	mov	sl, r0
	adds	r2, #90
	adds	r3, r7, r2
	mov	r1, sl
.L_02002422:
	strh	r1, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #88
	mov	r2, sl
	adds	r3, r7, r1
	adds	r2, #32
	strh	r2, [r3, #0]
	movs	r2, #208
	lsls	r2, r2, #3
	movs	r3, #128
	adds	r2, r2, r7
	lsls	r3, r3, #3
	movs	r1, #15
	mov	r8, r2
	adds	r5, r7, r3
	mov	r9, r1
.L_02002444:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	adds	r0, r5, #0
	bl 0x0200aa0c
	adds	r0, r6, #0
	bl 0x0200aa2c
	subs	r0, #2
	strh	r0, [r5, #30]
	adds	r0, r6, #0
	bl 0x0200aa24
	ldrb	r2, [r5, #9]
	movs	r1, #13
	movs	r3, #3
	negs	r1, r1
	ands	r0, r3
	adds	r3, r1, #0
	ands	r2, r3
	ldrb	r3, [r5, #5]
	movs	r1, #32
	orrs	r3, r1
	lsls	r0, r0, #2
	strb	r3, [r5, #5]
	orrs	r2, r0
	movs	r3, #15
	ands	r2, r3
	subs	r3, #16
	strb	r2, [r5, #9]
	add	r9, r3
	mov	r2, r8
	str	r3, [r2, #24]
	mov	r1, r9
	movs	r3, #28
	adds	r5, #40
	add	r8, r3
	cmp	r1, #0
	bge.n	.L_02002444
	movs	r0, #145
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	lsls	r0, r0, #1
	bl 0x0200a86c
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #68
	adds	r3, r7, r2
	str	r0, [r3, #0]
	mov	r8, r0
	movs	r1, #1
	bl 0x0200a85c
	mov	r0, r8
	movs	r1, #0
	bl 0x0200a88c
	mov	r2, r8
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200a88c
	ldr	r2, [r6, #80]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r2, #18]
	ldr	r2, [pc, #72]
	ldr	r3, [r6, #8]
	ldr	r1, [pc, #60]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r1, [r3, #0]
	movs	r3, #128
	movs	r1, #128
	lsls	r3, r3, #4
	lsls	r1, r1, #4
	movs	r2, #128
	adds	r1, #86
	adds	r3, #84
	lsls	r2, r2, #4
	movs	r5, #0
	adds	r6, r7, r3
	adds	r2, #92
	adds	r3, r7, r1
	strh	r5, [r6, #0]
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #94
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r0, #221
	b.n	.L_02002528
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200aa6c
	.2byte 0x0000
	.2byte 0xfff4
.L_02002528:
	.2byte 0xf000
	.2byte 0xfa9c
	.2byte 0x2190
	lsls	r1, r1, #3
	ldr	r0, [pc, #84]
	bl 0x0200a7ec
	movs	r2, #186
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02002560
.L_02002544:
	movs	r0, #1
	bl 0x0200a7e4
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #84
	adds	r3, r7, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_02002544
.L_02002560:
	ldr	r0, [pc, #36]
	bl 0x0200a7f4
	mov	r0, r8
	bl 0x0200a874
	mov	r0, fp
	bl 0x0200a82c
	movs	r0, #220
	bl 0x0200a81c
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x9fd1
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #4
	movs	r3, #148
	adds	r5, r7, r2
	lsls	r3, r3, #5
	movs	r2, #95
	adds	r6, r7, r3
	mov	r8, r2
.L_020025aa:
	ldr	r3, [r5, #24]
	cmp	r3, #15
	bhi.n	.L_020025f4
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #130
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #1
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
	bl 0x0200aa14
	adds	r0, r5, #0
	movs	r1, #64
	movs	r2, #0
	bl 0x0200aa1c
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_020025f4
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_020025f4:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_020025aa
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
	sub	sp, #8
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	sl, r0
	mov	r8, r3
	adds	r7, r1, #0
	bl 0x0200a7fc
	movs	r1, #134
	lsls	r1, r1, #6
	add	r1, r8
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	mov	r9, r1
	lsls	r5, r2, #2
	adds	r5, r5, r2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r5, r5, #3
	movs	r1, #148
	mov	fp, r0
	lsls	r1, r1, #5
	lsls	r3, r3, #2
	add	r5, r8
	mov	r0, sl
	adds	r5, r5, r1
	add	r8, r3
	bl 0x0200aa24
	ldrb	r2, [r5, #9]
	movs	r3, #3
	ands	r0, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r5, #9]
	mov	r0, sl
	bl 0x0200aa2c
	movs	r6, #128
	lsls	r6, r6, #4
	add	r6, r8
	movs	r3, #0
	strh	r0, [r5, #30]
	str	r3, [r6, #24]
	str	r7, [r6, #0]
	ldr	r1, [sp, #4]
	mov	sl, r3
	str	r1, [r6, #4]
	ldr	r3, [sp, #0]
	str	r3, [r6, #8]
	bl 0x0200a7fc
	movs	r1, #128
	lsls	r1, r1, #10
	lsls	r0, r0, #1
	adds	r2, r6, #0
	adds	r0, r0, r1
	mov	r1, fp
	bl 0x0200a80c
	mov	r3, sl
	str	r3, [r6, #12]
	bl 0x0200a7fc
	movs	r1, #192
	lsls	r1, r1, #8
	lsrs	r0, r0, #1
	mov	r3, sl
	adds	r0, r0, r1
	str	r3, [r6, #20]
	str	r0, [r6, #16]
.L_020026b6:
	bl 0x0200a7fc
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #12
	add	r8, r1
	mov	r2, r8
	mov	r1, fp
	lsrs	r0, r0, #2
	bl 0x0200a80c
	mov	r3, r9
	ldrh	r0, [r3, #0]
	mov	r1, r9
	adds	r0, #1
	strh	r0, [r1, #0]
	lsls	r0, r0, #16
	movs	r1, #96
	asrs	r0, r0, #16
	bl 0x0200a7dc
	mov	r3, r9
	strh	r0, [r3, #0]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #136
	movs	r0, #220
	sub	sp, #4
	bl 0x0200a814
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl 0x0200a844
	adds	r1, r6, #0
	bl 0x0200a824
	bl 0x0200a83c
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl 0x0200a834
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #130
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #128
	movs	r3, #148
	lsls	r2, r2, #4
	lsls	r3, r3, #5
	movs	r1, #95
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_0200274c:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl 0x0200aa0c
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
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
	bge.n	.L_0200274c
	movs	r1, #134
	lsls	r1, r1, #6
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200a7ec
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001e1
	.2byte 0xa58d
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200a7f4
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #132
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
.L_020027cc:
	bl 0x0200a82c
	movs	r0, #220
	bl 0x0200a81c
	pop	{r5, pc}
	.4byte 0x0200a58d
	.section .text.x0200aa6c,"ax",%progbits
	.4byte 0xc13c0100
	.4byte 0xb9d2cf52
	.4byte 0x13465bb3
	.4byte 0x5afce9e8
	.4byte 0xafd1ba81
	.4byte 0x9a39eb47
	.4byte 0x7445b435
	.4byte 0x2ff63444
	.4byte 0x74759d7d
	.4byte 0x581ed0e8
	.4byte 0x499ece98
	.4byte 0x66cc9bb5
	.4byte 0x28a91658
	.4byte 0xd6468754
	.4byte 0xb8fc3ca3
	.4byte 0x0394251c
	.4byte 0xa3921e4e
	.4byte 0xed0a619d
	.4byte 0x290915e5
	.4byte 0xd0d9ce84
	.4byte 0xcb0f067c
	.4byte 0x3a788051
	.4byte 0x4d0804f7
	.4byte 0xe32e6224
	.4byte 0x26d300f1
	.4byte 0xc780ce20
	.4byte 0x883c5604
	.4byte 0x991f8fbc
	.4byte 0x64fd4ba6
	.4byte 0x5026593a
	.4byte 0x42e4728b
	.4byte 0x4d0a3424
	.4byte 0xbe74d99b
	.4byte 0xe6f939cd
	.4byte 0xb126f91e
	.4byte 0xf32807ec
	.4byte 0x7c84cbcd
	.4byte 0xdc069263
	.4byte 0x984841e2
	.4byte 0x998e2689
	.4byte 0xd98df5e0
	.4byte 0x7ce0c6f9
	.4byte 0x70784b43
	.4byte 0xf7f6bc36
	.4byte 0xc7b95e4d
	.4byte 0xe7719be3
	.4byte 0x17a9ad75
	.4byte 0x586f8f25
	.4byte 0x75fcd4d3
	.4byte 0x08068958
	.4byte 0x9c9a2b93
	.4byte 0xfbcfa216
	.4byte 0x7e458240
	.4byte 0x86f9f834
	.4byte 0xfc221ebe
	.4byte 0xef811f5e
	.4byte 0x813ce478
	.4byte 0x242f015d
	.4byte 0xc6de6c1d
	.4byte 0x6faf3beb
	.4byte 0x5013be3c
	.4byte 0xf8f0a8bc
	.4byte 0x78c181c6
	.4byte 0xcf386e27
	.4byte 0xde5c3c49
	.4byte 0x6677cfc8
	.4byte 0x2a0df303
	.4byte 0x8f8df3f1
	.4byte 0xdf1e77c1
	.4byte 0x3e78c038
	.4byte 0x3de7e014
	.4byte 0x638c3f8f
	.4byte 0x4481687e
	.4byte 0x401077ce
	.4byte 0xf831f014
	.4byte 0x36800cc6
	.4byte 0x7d8b83c0
	.4byte 0x80cfbf02
	.4byte 0xe1c3400a
	.4byte 0xb40d23c6
	.4byte 0xde213ce2
	.4byte 0x9e3c2f19
	.4byte 0xe39df5f3
	.4byte 0xa8c6f9ae
	.4byte 0x8291fe62
	.4byte 0x80556f9c
	.4byte 0x88e0f404
	.4byte 0x38c0bf22
	.4byte 0xe8dadf3f
	.4byte 0xe01df023
	.4byte 0x102cef93
	.4byte 0x12c8cc41
	.4byte 0xbe40f187
	.4byte 0x39f09e71
	.4byte 0xb7dfcef8
	.4byte 0x0b10cf5e
	.4byte 0x3e012227
	.4byte 0x0e4fbaf0
	.4byte 0x344d6c12
	.4byte 0xc30f8e72
	.4byte 0xf8225e77
	.4byte 0x3bebd6fb
	.4byte 0xbdc9e3ef
	.4byte 0x3cf1a54a
	.4byte 0x323dcfe0
	.4byte 0x859f5350
	.4byte 0x38339cab
	.4byte 0xf5e0510f
	.4byte 0x3410af12
	.4byte 0x1f8088f5
	.4byte 0x9fbaf811
	.4byte 0xf19223df
	.4byte 0x607f04a3
	.4byte 0xadf82f92
	.4byte 0x04abd09e
	.4byte 0xc14f8168
	.4byte 0xaf1efc75
	.4byte 0xf82f19f3
	.4byte 0x301f82fa
	.4byte 0xd7cc2978
	.4byte 0xe0bebc73
	.4byte 0xc17dfdfd
	.4byte 0x033e31e3
	.4byte 0x1f45f3f4
	.4byte 0x18ebc7bf
	.4byte 0x043e08c6
	.4byte 0x60fe70ff
	.4byte 0x904fe2be
	.4byte 0xbaf1735e
	.4byte 0xbb803205
	.4byte 0xa18451f8
	.4byte 0xc00d1f80
	.4byte 0xf0ba1e1e
	.4byte 0x9f9af819
	.4byte 0xc17d79e3
	.4byte 0x1f7008cf
	.4byte 0x2df20be7
	.4byte 0x31f1147f
	.4byte 0xc04f8120
	.4byte 0x32147c17
	.4byte 0x5e1eefe1
	.4byte 0x7c2640a1
	.4byte 0x181fd39d
	.4byte 0xefe15ebc
	.4byte 0x67053ea8
	.4byte 0x8e7809f0
	.4byte 0xd1fc1bc7
	.4byte 0xfc0bd7c0
	.4byte 0x4aebc0dd
	.4byte 0x39f48a5e
	.4byte 0xc10f8eb8
	.4byte 0xbe147c63
	.4byte 0x3e1defe0
	.4byte 0x49f24f08
	.4byte 0x00b382f9
	.4byte 0xf82f891f
	.4byte 0xe3240ab9
	.4byte 0x0c3ea7e7
	.4byte 0x2009f2ef
	.4byte 0xf907af8a
	.4byte 0xc688dc18
	.4byte 0x1e07e087
	.4byte 0xe754707f
	.4byte 0x3f13b043
	.4byte 0xb851f3b3
	.4byte 0x09008f99
	.4byte 0xebeb483c
	.4byte 0x4b053e49
	.4byte 0xf3f2c632
	.4byte 0x3e026724
	.4byte 0xa3f2ef0e
	.4byte 0xf09a5f2b
	.4byte 0x00003efe
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000178
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000178
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000180
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000070
	.4byte 0x40000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff005a
	.4byte 0x00000178
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x00000180
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000048
	.4byte 0x80000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00105006
	.4byte 0x00201005
	.4byte 0x000001ff
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff005c
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00020000
	.4byte 0xffff005c
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00028000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002a000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002a000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002e000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002e000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002e000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002e000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0002e000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00028000
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
	.4byte 0x00000002
	.4byte 0x08920009
	.4byte 0x02008441
	.4byte 0x00000002
	.4byte 0x0890000a
	.4byte 0x02008511
	.4byte 0x00000002
	.4byte 0x0893000b
	.4byte 0x02009a2d
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x02009bb1
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001608
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008055
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020080c9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
