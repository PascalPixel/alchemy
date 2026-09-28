.syntax unified
	.thumb
	push	{lr}
	movs	r0, #8
	movs	r1, #10
	bl 0x0200922c
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r2, #57
	movs	r3, #16
	movs	r1, #63
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r2, #1
	str	r3, [sp, #0]
	str	r1, [sp, #8]
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r2, #63
	movs	r1, #52
	movs	r3, #5
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #22
	movs	r1, #15
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r2, #56
	movs	r1, #58
	movs	r3, #3
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #7
	movs	r1, #22
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #12
	movs	r3, #3
	movs	r2, #66
	movs	r1, #57
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #44
	movs	r1, #25
	movs	r2, #0
	movs	r3, #4
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #8
	movs	r2, #61
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #16
	movs	r1, #13
	movs	r2, #0
	movs	r3, #4
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r2, #57
	movs	r1, #52
	movs	r3, #2
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #21
	movs	r1, #9
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r2, #63
	movs	r1, #52
	movs	r3, #5
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #15
	movs	r0, #22
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x02009274
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #83
	bl 0x0200913c
	cmp	r0, #0
	bne.n	.L_02000184
	movs	r1, #196
	movs	r2, #172
	movs	r0, #64
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #24
	bne.n	.L_02000184
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #21
	bne.n	.L_02000184
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_02000184:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	movs	r3, #9
	movs	r2, #65
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #83
	movs	r2, #3
	movs	r3, #1
	movs	r0, #51
	bl 0x0200916c
	movs	r0, #3
	bl 0x0200921c
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	movs	r3, #18
	movs	r2, #66
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #83
	movs	r2, #3
	movs	r3, #1
	movs	r0, #51
	bl 0x0200916c
	movs	r0, #4
	bl 0x0200921c
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #5
	bl 0x0200921c
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r2, #56
	movs	r1, #58
	movs	r3, #3
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	adds	r5, r0, #0
	movs	r1, #22
	movs	r0, #7
	movs	r2, #0
	str	r3, [sp, #0]
	bl 0x02009274
	movs	r1, #136
	movs	r2, #188
	movs	r0, #8
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_02000256
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #23
	bne.n	.L_02000256
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_02000256:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #3
	movs	r2, #56
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #5
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #5
	movs	r2, #0
	bl 0x02009274
	movs	r1, #136
	movs	r2, #248
	movs	r0, #9
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_020002d8
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #15
	bne.n	.L_020002d8
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_020002d8:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #124]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #3
	movs	r2, #56
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #5
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #5
	movs	r2, #0
	bl 0x02009274
	movs	r1, #152
	movs	r2, #132
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #9
	bne.n	.L_0200035a
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #16
	bne.n	.L_0200035a
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	movs	r1, #16
	ldr	r0, [r6, #0]
	negs	r1, r1
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_0200035a:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #3
	movs	r2, #56
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #5
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #5
	movs	r2, #0
	bl 0x02009274
	movs	r1, #136
	movs	r2, #140
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_020003dc
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_020003dc
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_020003dc:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #3
	movs	r2, #56
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #5
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #5
	movs	r2, #0
	bl 0x02009274
	movs	r1, #208
	movs	r2, #140
	movs	r0, #12
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #6
	bne.n	.L_0200045c
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_0200045c
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_0200045c:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #3
	movs	r2, #56
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #5
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #5
	movs	r2, #0
	bl 0x02009274
	movs	r1, #176
	movs	r2, #132
	movs	r0, #13
	lsls	r1, r1, #15
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #5
	bne.n	.L_020004dc
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #16
	bne.n	.L_020004dc
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_020004dc:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #3
	movs	r2, #56
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #5
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #5
	movs	r2, #0
	bl 0x02009274
	movs	r1, #208
	movs	r2, #248
	movs	r0, #14
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #6
	bne.n	.L_0200055c
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #15
	bne.n	.L_0200055c
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	ldr	r0, [r6, #0]
	movs	r1, #16
	movs	r2, #0
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_0200055c:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #124]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	sub	sp, #12
	bl 0x020091ac
	movs	r3, #8
	movs	r2, #61
	movs	r1, #52
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r3, #4
	adds	r5, r0, #0
	movs	r1, #13
	movs	r0, #16
	movs	r2, #0
	bl 0x02009274
	movs	r1, #140
	movs	r2, #148
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x020091cc
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #17
	bne.n	.L_020005de
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #18
	bne.n	.L_020005de
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r1, #129
	ldr	r0, [r6, #0]
	lsls	r1, r1, #1
	bl 0x02009204
	movs	r2, #16
	ldr	r0, [r6, #0]
	movs	r1, #0
	negs	r2, r2
	bl 0x020091bc
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r0, [r6, #0]
	bl 0x020091c4
	bl 0x020091a4
.L_020005de:
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	sub	sp, #12
	movs	r3, #5
	movs	r2, #60
	movs	r1, #56
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #7
	movs	r1, #23
	movs	r2, #0
	movs	r3, #4
	bl 0x02009274
	add	sp, #12
	pop	{pc}
	push	{lr}
	sub	sp, #12
	movs	r3, #5
	movs	r2, #60
	movs	r1, #56
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	movs	r0, #7
	movs	r1, #23
	movs	r2, #0
	movs	r3, #4
	bl 0x02009274
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #181
	bl 0x0200913c
	cmp	r0, #0
	bne.n	.L_02000662
	movs	r2, #212
	movs	r0, #249
	movs	r1, #168
	lsls	r2, r2, #1
	bl 0x0200927c
	ldr	r2, [pc, #40]
	movs	r3, #149
	lsls	r3, r3, #2
	adds	r1, r2, r3
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #181
	strh	r3, [r1, #0]
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r3
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r0, #106
	movs	r1, #0
	bl 0x02009224
.L_02000662:
	add	sp, #12
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #11
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r0, #0
	movs	r1, #0
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200916c
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r0, #2
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #11
	str	r3, [sp, #4]
	movs	r5, #7
	movs	r0, #1
	movs	r1, #0
	movs	r2, #1
	movs	r3, #2
	str	r5, [sp, #0]
	bl 0x0200916c
	movs	r3, #14
	str	r3, [sp, #4]
	movs	r0, #2
	movs	r1, #5
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #0]
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #23
	movs	r3, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #6
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	movs	r0, #9
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #15
	movs	r3, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #10
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #16
	movs	r3, #9
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #11
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #17
	movs	r3, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #12
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #17
	movs	r3, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	movs	r0, #13
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #16
	movs	r3, #5
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #14
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #15
	movs	r3, #6
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x020091ac
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009144
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #85
	adds	r5, #35
	strb	r3, [r2, #0]
	strb	r3, [r5, #0]
	movs	r2, #18
	movs	r3, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #15
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
	add	sp, #8
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #230
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	mov	r8, r3
	ldr	r3, [r6, #8]
	sub	sp, #12
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	adds	r7, r1, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	adds	r3, r3, r7
	str	r3, [r5, #8]
	bl 0x02009174
	ldr	r3, [r6, #8]
	movs	r2, #128
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	lsls	r2, r2, #12
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r0, r6, #0
	adds	r3, r3, r2
	adds	r1, r5, #0
	str	r3, [r5, #8]
	bl 0x02009174
	cmp	r0, #0
	bgt.n	.L_0200093c
	ldr	r2, [pc, #80]
	ldr	r3, [r6, #8]
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	adds	r1, r5, #0
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x02009174
	cmp	r0, #0
	bgt.n	.L_0200093c
	ldr	r2, [pc, #56]
	ldr	r3, [r6, #8]
	adds	r0, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #40]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r1, r5, #0
	adds	r3, r3, r2
	str	r3, [r5, #8]
	bl 0x02009174
	cmp	r0, #0
	bgt.n	.L_0200093c
	mov	r2, r8
	ldr	r3, [r2, #16]
	adds	r3, r3, r7
	str	r3, [r2, #16]
	ldr	r3, [r6, #16]
	adds	r3, r3, r7
	str	r3, [r6, #16]
.L_0200093c:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0005b333
	.2byte 0x4ccd
	.2byte 0xfffa
	.2byte 0xb560
	ldr	r3, [pc, #96]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x020091ac
	ldr	r3, [pc, #84]
	movs	r2, #7
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_020009a2
	ldr	r2, [r6, #12]
	movs	r3, #192
	lsls	r3, r3, #11
	adds	r2, r2, r3
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	movs	r0, #14
	bl 0x0200915c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000998
	movs	r1, #0
	bl 0x0200917c
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200914c
	ldr	r1, [pc, #36]
	adds	r0, r5, #0
	bl 0x02009154
.L_02000998:
	movs	r0, #184
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x02009284
.L_020009a2:
	movs	r1, #217
	lsls	r1, r1, #8
	adds	r1, #153
	adds	r0, r6, #0
	bl 0x020088a0
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0300122c
	.4byte 0x0200928c
	.4byte 0x4a046983
	.4byte 0x6183189b
	.4byte 0x189b69c3
	.4byte 0x200061c3
	.4byte 0x00004770
	.2byte 0xf334
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r0, #9
	bl 0x020091ac
	mov	r9, r0
	movs	r0, #64
	bl 0x020091ac
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	mov	sl, r3
	bl 0x0200919c
	movs	r0, #0
	bl 0x0200925c
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x02009244
	movs	r1, #1
	ldr	r0, [pc, #876]
	bl 0x0200923c
	movs	r0, #60
	bl 0x0200924c
	movs	r0, #168
	movs	r1, #1
	movs	r2, #158
	lsls	r2, r2, #16
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #16
	bl 0x0200920c
	movs	r0, #26
	bl 0x02009284
	movs	r0, #60
	bl 0x02009194
	movs	r1, #2
	movs	r0, #8
	bl 0x020091dc
	movs	r0, #144
	lsls	r0, r0, #2
	bl 0x02009284
	movs	r0, #60
	bl 0x02009194
	ldr	r0, [pc, #816]
	bl 0x020091e4
	movs	r2, #226
	lsls	r2, r2, #1
	add	sl, r2
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200918c
	movs	r1, #168
	movs	r2, #152
	lsls	r1, r1, #16
	movs	r0, #64
	lsls	r2, r2, #16
	bl 0x020091cc
	adds	r2, r6, #0
	movs	r3, #0
	mov	r8, r3
	adds	r2, #85
	movs	r3, #6
	strb	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [r6, #20]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r6, #20]
	movs	r0, #220
	bl 0x02009284
	movs	r0, #206
	movs	r2, #0
	movs	r3, #0
	movs	r1, #0
	lsls	r0, r0, #1
	bl 0x0200915c
	ldr	r1, [pc, #728]
	adds	r5, r0, #0
	bl 0x02009154
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x02009184
	adds	r3, r5, #0
	adds	r3, #85
	mov	r2, r8
	strb	r2, [r3, #0]
	movs	r2, #128
	ldr	r3, [r6, #8]
	lsls	r2, r2, #12
	str	r3, [r5, #8]
	movs	r0, #15
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r6, #16]
	str	r3, [r5, #16]
	ldr	r3, [r6, #20]
	movs	r6, #128
	str	r3, [r5, #20]
	ldr	r3, [pc, #680]
	lsls	r6, r6, #9
	str	r3, [r5, #108]
	bl 0x02009194
	mov	r3, r8
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	str	r3, [r5, #16]
	mov	r2, sl
	ldrh	r0, [r2, #0]
	str	r3, [r5, #108]
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200918c
	ldr	r3, [pc, #644]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r3, r2
	movs	r1, #166
	movs	r2, #188
	ldr	r0, [r5, #0]
	bl 0x020091b4
	ldr	r0, [r5, #0]
	bl 0x020091c4
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x020091d4
	mov	r3, r9
	str	r6, [r3, #48]
	movs	r1, #184
	movs	r2, #188
	movs	r0, #9
	bl 0x020091b4
	movs	r0, #9
	bl 0x020091c4
	movs	r1, #128
	movs	r2, #0
	movs	r0, #9
	lsls	r1, r1, #8
	bl 0x020091f4
	movs	r0, #9
	movs	r1, #0
	bl 0x020091ec
	movs	r2, #0
	ldr	r0, [r5, #0]
	movs	r1, #0
	bl 0x020091f4
	ldr	r0, [r5, #0]
	movs	r1, #3
	bl 0x020091dc
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020091f4
	mov	r2, sl
	ldrh	r0, [r2, #0]
	movs	r1, #0
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	negs	r2, r2
	bl 0x0200918c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #9
	bl 0x020091fc
	movs	r1, #176
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #9
	bl 0x020091f4
	movs	r0, #30
	bl 0x02009194
	movs	r0, #9
	movs	r1, #0
	bl 0x020091ec
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200918c
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x020091fc
	movs	r0, #30
	bl 0x02009194
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200918c
	movs	r1, #10
	movs	r2, #0
	adds	r1, #255
	movs	r0, #9
	bl 0x020091fc
	movs	r0, #30
	bl 0x02009194
	movs	r0, #9
	movs	r1, #0
	bl 0x020091ec
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200918c
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl 0x020091f4
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #9
	bl 0x020091f4
	movs	r0, #10
	bl 0x02009194
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x020091fc
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x020091fc
	movs	r0, #60
	bl 0x02009194
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	asrs	r0, r0, #16
	movs	r1, #0
	negs	r2, r2
	bl 0x0200918c
	movs	r1, #192
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020091f4
	movs	r1, #176
	movs	r0, #9
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020091f4
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	ldr	r0, [r5, #0]
	bl 0x020091fc
	movs	r1, #6
	adds	r1, #255
	movs	r2, #0
	movs	r0, #9
	bl 0x020091fc
	movs	r0, #60
	bl 0x02009194
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	negs	r2, r2
	movs	r1, #0
	asrs	r0, r0, #16
	bl 0x0200918c
	movs	r0, #40
	bl 0x02009194
	movs	r1, #3
	ldr	r0, [r5, #0]
	bl 0x020091dc
	movs	r0, #40
	bl 0x02009194
	movs	r0, #9
	movs	r1, #3
	bl 0x020091dc
	movs	r0, #9
	movs	r1, #0
	bl 0x020091ec
	movs	r0, #8
	movs	r1, #2
	bl 0x020091dc
	mov	r3, sl
	ldrh	r0, [r3, #0]
	mov	r2, sl
	adds	r3, r0, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #16
	movs	r2, #3
	negs	r2, r2
	movs	r1, #0
	asrs	r0, r0, #16
	bl 0x0200918c
	movs	r0, #30
	bl 0x02009194
	movs	r0, #8
	movs	r1, #4
	bl 0x020091dc
	movs	r0, #9
	movs	r1, #2
	bl 0x020091dc
	ldr	r0, [r5, #0]
	bl 0x020091ac
	cmp	r0, #0
	beq.n	.L_02000d2e
	movs	r3, #10
	ldrsh	r1, [r0, r3]
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	movs	r0, #9
	bl 0x020091b4
.L_02000d2e:
	movs	r0, #9
	bl 0x020091c4
	movs	r2, #0
	movs	r1, #0
	movs	r0, #9
	bl 0x020091cc
	movs	r0, #9
	bl 0x020091c4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #206
	bl 0x02009144
	bl 0x02009264
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200923c
	movs	r0, #60
	bl 0x0200924c
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x02009214
	movs	r0, #60
	bl 0x02009194
	bl 0x020091a4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x00403108
	.4byte 0x00001fe1
	.4byte 0x020092a4
	.4byte 0x020089bd
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #64
	bl 0x020091ac
	cmp	r0, #0
	beq.n	.L_02000daa
	ldr	r3, [r0, #8]
	cmp	r3, #0
	beq.n	.L_02000daa
	movs	r0, #130
	lsls	r0, r0, #5
	bl 0x02009234
.L_02000daa:
	pop	{pc}
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x92e0
	.2byte 0x0200
	.global Func_02000db4
	.thumb_func
Func_02000db4:
	push	{lr}
	ldr	r3, [pc, #24]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #16]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_02000dcc
	ldr	r0, [pc, #12]
.L_02000dcc:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000007e
	.2byte 0x9310
	.2byte 0x0200
	.global Func_02000ddc
	.thumb_func
Func_02000ddc:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x9330
	.2byte 0x0200
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000dfc
	ldr	r0, [pc, #52]
	b.n	.L_02000e26
.L_02000dfc:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000e06
	ldr	r0, [pc, #52]
	b.n	.L_02000e26
.L_02000e06:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000e10
	ldr	r0, [pc, #48]
	b.n	.L_02000e26
.L_02000e10:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000e1a
	ldr	r0, [pc, #48]
	b.n	.L_02000e26
.L_02000e1a:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000e24
	ldr	r0, [pc, #44]
	b.n	.L_02000e26
.L_02000e24:
	ldr	r0, [pc, #44]
.L_02000e26:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x02009390
	.4byte 0x0000007f
	.4byte 0x020093a8
	.4byte 0x00000080
	.4byte 0x020093d8
	.4byte 0x00000081
	.4byte 0x02009498
	.4byte 0x00000082
	.4byte 0x02009510
	.2byte 0x9378
	.2byte 0x0200
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000e70
	ldr	r0, [pc, #52]
	b.n	.L_02000e9a
.L_02000e70:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000e7a
	ldr	r0, [pc, #52]
	b.n	.L_02000e9a
.L_02000e7a:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_02000e84
	ldr	r0, [pc, #48]
	b.n	.L_02000e9a
.L_02000e84:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000e8e
	ldr	r0, [pc, #48]
	b.n	.L_02000e9a
.L_02000e8e:
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000e98
	ldr	r0, [pc, #44]
	b.n	.L_02000e9a
.L_02000e98:
	ldr	r0, [pc, #44]
.L_02000e9a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x0200954c
	.4byte 0x0000007f
	.4byte 0x02009594
	.4byte 0x00000080
	.4byte 0x020095c4
	.4byte 0x00000081
	.4byte 0x020096cc
	.4byte 0x00000082
	.4byte 0x02009714
	.2byte 0x9540
	.2byte 0x0200
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	movs	r0, #0
	sub	sp, #8
	bl 0x02009254
	ldr	r5, [pc, #556]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #548]
	cmp	r2, r3
	bne.n	.L_02000f4e
	adds	r1, #2
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	cmp	r6, #1
	bne.n	.L_02000f4e
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #39
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_02000f2a
	ldr	r3, [pc, #520]
	movs	r0, #152
	movs	r1, #128
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	adds	r2, r5, r0
	adds	r1, #98
	strh	r3, [r2, #0]
	adds	r3, r5, r1
	strh	r6, [r3, #0]
	b.n	.L_02000f4e
.L_02000f2a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #40
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_02000f4e
	ldr	r2, [pc, #488]
	movs	r0, #152
	movs	r1, #128
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	adds	r3, r5, r0
	adds	r1, #98
	strh	r2, [r3, #0]
	adds	r2, r5, r1
	movs	r3, #3
	strh	r3, [r2, #0]
.L_02000f4e:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200913c
	cmp	r0, #0
	bne.n	.L_02000f84
	ldr	r1, [pc, #444]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #448]
	cmp	r2, r3
	bne.n	.L_02000f84
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	ble.n	.L_02000f84
	adds	r2, #50
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl 0x0200926c
.L_02000f84:
	ldr	r3, [pc, #400]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #408]
	cmp	r2, r3
	bne.n	.L_0200107e
	adds	r0, #32
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_02000fb4
	movs	r3, #8
	movs	r2, #23
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #6
	movs	r1, #23
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_02000fb4:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_02000fd6
	movs	r3, #8
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_02000fd6:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_02000ff8
	movs	r3, #9
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_02000ff8:
	movs	r0, #130
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_0200101a
	movs	r3, #8
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_0200101a:
	movs	r0, #129
	lsls	r0, r0, #2
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_0200103a
	movs	r3, #6
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_0200103a:
	movs	r0, #131
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_0200105c
	movs	r3, #5
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_0200105c:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #6
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_0200107e
	movs	r3, #6
	movs	r2, #15
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_0200107e:
	ldr	r3, [pc, #152]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #164]
	cmp	r2, r3
	bne.n	.L_020010b0
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_020010b0
	movs	r3, #17
	movs	r2, #18
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #15
	movs	r1, #18
	movs	r2, #1
	movs	r3, #1
	bl 0x02009164
.L_020010b0:
	ldr	r3, [pc, #100]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #116]
	cmp	r2, r3
	bne.n	.L_02001112
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #206
	bl 0x0200913c
	cmp	r0, #0
	beq.n	.L_02001112
	movs	r0, #240
	lsls	r0, r0, #4
	adds	r0, #147
	bl 0x0200913c
	cmp	r0, #0
	bne.n	.L_02001112
	movs	r0, #10
	adds	r0, #255
	bl 0x0200913c
	cmp	r0, #0
	bne.n	.L_02001112
	movs	r0, #64
	bl 0x020091ac
	movs	r1, #168
	movs	r2, #152
	adds	r5, r0, #0
	lsls	r1, r1, #16
	lsls	r2, r2, #16
	movs	r0, #64
	bl 0x020091cc
	adds	r2, r5, #0
	movs	r3, #6
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r1, #128
	ldr	r3, [r5, #20]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r5, #20]
.L_02001112:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x0000007e
	.4byte 0x0000007b
	.4byte 0x00000078
	.4byte 0x0000007f
	.4byte 0x00000080
	.4byte 0x00000082
	.2byte 0x0081
	.2byte 0x0000
	.global Func_02001138
	.thumb_func
Func_02001138:
	movs	r0, #0
	bx	lr
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
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
	.4byte 0x002e0184
	.4byte 0x018c0114
	.4byte 0x011c0036
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000007e
	.4byte 0x0010207c
	.4byte 0x0020107f
	.4byte 0x0030307f
	.4byte 0x0040407f
	.4byte 0x0050507f
	.4byte 0x0000007f
	.4byte 0x0010207e
	.4byte 0x00201080
	.4byte 0x00000080
	.4byte 0x0010207f
	.4byte 0x00201082
	.4byte 0x00000081
	.4byte 0x00102082
	.4byte 0x00000082
	.4byte 0x00102080
	.4byte 0x00201081
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x003a00f3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte 0x02008045
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte 0x0200818d
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte 0x020081b1
	.4byte 0x50008905
	.4byte 0xffff002d
	.4byte 0x020081d5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte 0x02008039
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x0200866d
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200869d
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte 0x02008061
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte 0x02008081
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte 0x020081e1
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte 0x02008261
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte 0x020082e1
	.4byte 0x50008905
	.4byte 0xffff002d
	.4byte 0x02008365
	.4byte 0x50008905
	.4byte 0xffff002e
	.4byte 0x020083e5
	.4byte 0x50008905
	.4byte 0xffff002f
	.4byte 0x02008465
	.4byte 0x50008905
	.4byte 0xffff0030
	.4byte 0x020084e5
	.4byte 0x50008905
	.4byte 0xffff0031
	.4byte 0x02008101
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x020086cd
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte 0x02008705
	.4byte 0x00001815
	.4byte 0x0202000a
	.4byte 0x02008741
	.4byte 0x00001815
	.4byte 0x0203000b
	.4byte 0x0200877d
	.4byte 0x00001815
	.4byte 0x0204000c
	.4byte 0x020087b9
	.4byte 0x00001815
	.4byte 0x0205000d
	.4byte 0x020087f1
	.4byte 0x00001815
	.4byte 0x0206000e
	.4byte 0x0200882d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x09ce0028
	.4byte 0x020089d5
	.4byte 0x00000000
	.4byte 0x0f930008
	.4byte 0x02008d91
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte 0x020085e9
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte 0x02008609
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte 0x020080a1
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte 0x020080c1
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte 0x020080e1
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte 0x02008565
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200894d
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x02008869
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
