.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020030, 0x08020030
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_080200f8, 0x080200f8
	.set sub_08038118, 0x08038118
	.set sub_08038120, 0x08038120
	.set sub_08038130, 0x08038130
	.set sub_080381c8, 0x080381c8
	.set sub_0811a188, 0x0811a188
	.set sub_0811a44c, 0x0811a44c
	.set sub_0811b9d4, 0x0811b9d4
	.set sub_0811bc64, 0x0811bc64
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811be68, 0x0811be68
	.set sub_0811bec8, 0x0811bec8
	.set sub_0811c120, 0x0811c120
	.set sub_0811c650, 0x0811c650
	.set sub_08126804, 0x08126804
	.set sub_08126ae4, 0x08126ae4
	.set sub_08138008, 0x08138008
	.set sub_08138010, 0x08138010
	.set sub_08138018, 0x08138018
	.set sub_08138020, 0x08138020
	.global Func_0811c66c
	.thumb_func
Func_0811c66c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	bl	sub_08014878
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L_0811c6b6
	mov	r5, sp
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_0811a188
	adds	r6, r0, #0
	movs	r7, #0
	cmp	r6, #0
	beq.n	.L_0811c6ac
	mov	r8, r5
	movs	r5, #0
.L_0811c696:
	mov	r2, r8
	ldrsh	r0, [r5, r2]
	bl	sub_0811bec8
	adds	r7, #1
	movs	r0, #8
	bl	sub_08013560
	adds	r5, #2
	cmp	r7, r6
	bne.n	.L_0811c696
.L_0811c6ac:
	movs	r0, #22
	bl	sub_08013560
	movs	r0, #1
	b.n	.L_0811c6be
.L_0811c6b6:
	ldr	r0, [pc, #16]
	bl	sub_080381c8
	movs	r0, #0
.L_0811c6be:
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0c99
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r2, #0
	ldrsh	r5, [r0, r2]
	adds	r0, r5, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_0811c650
	cmp	r0, #0
	bge.n	.L_0811c6ea
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811c708
.L_0811c6ea:
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	movs	r0, #0
	cmp	r3, #0
	ble.n	.L_0811c708
	bl	sub_08038118
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #8]
	bl	sub_080381c8
	movs	r0, #0
.L_0811c708:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0c62
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r0, [sp, #12]
	ldr	r1, [sp, #12]
	movs	r0, #0
	movs	r2, #0
	ldrsh	r1, [r1, r2]
	mov	r9, r0
	adds	r0, r1, #0
	str	r1, [sp, #8]
	bl	sub_0811c650
	cmp	r0, #0
	blt.n	.L_0811c74a
	ldr	r2, [sp, #12]
	movs	r3, #10
	ldrsh	r2, [r2, r3]
	adds	r0, r2, #0
	str	r2, [sp, #4]
	bl	sub_0811c650
	cmp	r0, #0
	bge.n	.L_0811c750
.L_0811c74a:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811c976
.L_0811c750:
	ldr	r1, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	movs	r1, #160
	lsls	r1, r1, #7
	cmp	r3, #4
	bgt.n	.L_0811c76a
	movs	r1, #128
	lsls	r1, r1, #6
.L_0811c76a:
	movs	r3, #60
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	movs	r0, #10
	bl	sub_08013560
	bl	sub_08014878
	ldr	r0, [sp, #8]
	bl	sub_0811be3c
	ldr	r2, [sp, #4]
	ldr	r6, [r0, #0]
	cmp	r2, #7
	bhi.n	.L_0811c79c
	add	r3, sp, #104
	mov	sl, r3
	movs	r0, #2
	mov	r1, sl
	bl	sub_0811a188
	mov	fp, r0
	movs	r0, #128
	str	r0, [sp, #0]
	b.n	.L_0811c7ac
.L_0811c79c:
	add	r1, sp, #104
	movs	r0, #1
	mov	sl, r1
	bl	sub_0811a188
	movs	r2, #0
	str	r2, [sp, #0]
	mov	fp, r0
.L_0811c7ac:
	mov	r3, fp
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0811c7cc
.L_0811c7b4:
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #8]
	adds	r3, r5, r0
	cmp	r3, r1
	bne.n	.L_0811c7c6
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_08020090
.L_0811c7c6:
	adds	r5, #1
	cmp	r5, fp
	bne.n	.L_0811c7b4
.L_0811c7cc:
	movs	r0, #30
	bl	sub_08013560
	movs	r2, #128
	ldr	r3, [pc, #44]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	mov	r2, fp
	movs	r5, #0
	cmp	r2, #0
	beq.n	.L_0811c7f4
.L_0811c7e4:
	ldr	r3, [sp, #0]
	movs	r1, #1
	adds	r0, r5, r3
	adds	r5, #1
	bl	sub_08126ae4
	cmp	r5, fp
	bne.n	.L_0811c7e4
.L_0811c7f4:
	movs	r0, #128
	lsls	r0, r0, #19
	ldr	r7, [pc, #12]
	ldr	r6, [pc, #16]
	adds	r0, #82
	movs	r5, #0
	mov	r8, r0
	b.n	.L_0811c810
	.4byte 0x00003f40
	.4byte 0x00000010
	.2byte 0x1000
	.2byte 0x0000
.L_0811c810:
	subs	r3, r7, r5
	orrs	r3, r6
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #16
	bne.n	.L_0811c810
	movs	r0, #9
	bl	sub_08038130
	ldr	r2, [sp, #4]
	cmp	r2, #127
	ble.n	.L_0811c86a
	movs	r0, #2
	mov	r1, sl
	bl	sub_0811a188
	mov	r8, r0
	movs	r5, #0
	cmp	r9, r8
	beq.n	.L_0811c89e
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	adds	r7, r3, r1
.L_0811c848:
	adds	r6, r5, #0
	adds	r6, #128
	adds	r0, r6, #0
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	ble.n	.L_0811c862
	movs	r3, #1
	strh	r6, [r7, #0]
	add	r9, r3
	adds	r7, #2
.L_0811c862:
	adds	r5, #1
	cmp	r9, r8
	bne.n	.L_0811c848
	b.n	.L_0811c89e
.L_0811c86a:
	movs	r0, #1
	mov	r1, sl
	bl	sub_0811a188
	adds	r7, r0, #0
	movs	r5, #0
	cmp	r9, r7
	beq.n	.L_0811c89e
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	adds	r6, r3, r1
.L_0811c882:
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	ble.n	.L_0811c898
	movs	r3, #1
	strh	r5, [r6, #0]
	add	r9, r3
	adds	r6, #2
.L_0811c898:
	adds	r5, #1
	cmp	r9, r7
	bne.n	.L_0811c882
.L_0811c89e:
	ldr	r2, [pc, #48]
	mov	r0, r9
	lsls	r3, r0, #1
	mov	r1, sl
	strh	r2, [r1, r3]
	mov	r0, sl
	movs	r1, #0
	bl	sub_0811b9d4
	ldr	r1, [sp, #12]
	add	r0, sp, #16
	movs	r2, #8
	ldrsh	r3, [r1, r2]
	movs	r5, #0
	str	r3, [r0, #0]
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r2, [r0, #8]
	cmp	r3, #0
	beq.n	.L_0811c8e2
	mov	r1, sl
	add	r2, sp, #52
	movs	r4, #0
	b.n	.L_0811c8d4
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_0811c8d4:
	ldrh	r3, [r4, r1]
	adds	r5, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r5, r9
	bne.n	.L_0811c8d4
.L_0811c8e2:
	mov	r1, r9
	str	r1, [r0, #20]
	ldr	r2, [sp, #4]
	cmp	r2, #7
	bhi.n	.L_0811c8f0
	movs	r3, #1
	b.n	.L_0811c8f2
.L_0811c8f0:
	movs	r3, #0
.L_0811c8f2:
	str	r3, [r0, #4]
	bl	sub_08138010
	movs	r0, #10
	bl	sub_08013560
	bl	sub_0811a44c
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	mov	r3, fp
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0811c924
.L_0811c914:
	ldr	r1, [sp, #0]
	adds	r0, r5, r1
	movs	r1, #1
	adds	r5, #1
	bl	sub_08126ae4
	cmp	r5, fp
	bne.n	.L_0811c914
.L_0811c924:
	movs	r7, #128
	ldr	r6, [pc, #12]
	lsls	r7, r7, #19
	movs	r5, #0
	adds	r7, #82
	b.n	.L_0811c938
	.4byte 0x00003f40
	.2byte 0x1000
	.2byte 0x0000
.L_0811c938:
	adds	r3, r5, #0
	orrs	r3, r6
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #16
	bne.n	.L_0811c938
	mov	r2, fp
	movs	r5, #0
	cmp	r2, #0
	beq.n	.L_0811c962
.L_0811c952:
	ldr	r3, [sp, #0]
	movs	r1, #0
	adds	r0, r5, r3
	adds	r5, #1
	bl	sub_08126ae4
	cmp	r5, fp
	bne.n	.L_0811c952
.L_0811c962:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08126804
	movs	r0, #3
	bl	sub_08013560
	movs	r0, #0
.L_0811c976:
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r2, [r3, #0]
	movs	r1, #128
	ldr	r3, [r2, #0]
	lsls	r1, r1, #6
	sub	sp, #88
	adds	r6, r0, #0
	cmp	r3, r1
	bne.n	.L_0811c9a6
	str	r1, [r2, #0]
	movs	r0, #10
	bl	sub_08013560
	b.n	.L_0811c9ae
.L_0811c9a6:
	str	r1, [r2, #0]
	movs	r0, #30
	bl	sub_08013560
.L_0811c9ae:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	mov	r5, sp
	str	r0, [r5, #8]
	bl	sub_0811c650
	cmp	r0, #0
	blt.n	.L_0811c9ce
	ldrh	r0, [r6, #10]
	strh	r0, [r5, #36]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	bl	sub_0811c650
	cmp	r0, #0
	bge.n	.L_0811c9d4
.L_0811c9ce:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811ca4a
.L_0811c9d4:
	ldr	r0, [r5, #8]
	bl	sub_08016ca4
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_08016ca4
	bl	sub_08014878
	movs	r1, #1
	ldr	r0, [r5, #8]
	bl	sub_08038120
	ldr	r0, [pc, #96]
	bl	sub_080381c8
	movs	r2, #13
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	movs	r3, #0
	bl	sub_0811c120
	ldr	r0, [r5, #8]
	bl	sub_0811be3c
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	sub_08020098
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_0811be3c
	ldrh	r3, [r5, #36]
	movs	r2, #1
	str	r2, [r5, #20]
	cmp	r3, #7
	bhi.n	.L_0811ca26
	str	r2, [r5, #4]
	b.n	.L_0811ca2a
.L_0811ca26:
	movs	r3, #0
	str	r3, [r5, #4]
.L_0811ca2a:
	movs	r3, #0
	movs	r0, #4
	str	r3, [r5, #28]
	bl	sub_08013560
	adds	r0, r5, #0
	bl	sub_08138008
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	bl	sub_0811be68
	ldr	r0, [r5, #8]
	bl	sub_0811be68
	movs	r0, #0
.L_0811ca4a:
	add	sp, #88
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0c60
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	adds	r5, r0, #0
	ldr	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	sub	sp, #96
	ldr	r1, [pc, #372]
	cmp	r3, #4
	bgt.n	.L_0811ca76
	movs	r1, #128
	lsls	r1, r1, #6
.L_0811ca76:
	ldr	r3, [r2, #0]
	cmp	r3, r1
	bne.n	.L_0811ca88
	movs	r3, #40
	str	r3, [r2, #4]
	movs	r0, #40
	bl	sub_08013560
	b.n	.L_0811ca94
.L_0811ca88:
	movs	r3, #40
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	movs	r0, #40
	bl	sub_08013560
.L_0811ca94:
	movs	r2, #8
	ldrsh	r3, [r5, r2]
	add	r6, sp, #8
	str	r3, [r6, #0]
	movs	r1, #12
	ldrsh	r3, [r5, r1]
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	str	r3, [r6, #16]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	str	r0, [r6, #8]
	str	r3, [r6, #12]
	bl	sub_0811c650
	cmp	r0, #0
	bge.n	.L_0811cabc
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811cbd8
.L_0811cabc:
	ldr	r3, [r6, #12]
	cmp	r3, #127
	ble.n	.L_0811cac8
	add	r7, sp, #44
	movs	r0, #2
	b.n	.L_0811cacc
.L_0811cac8:
	add	r7, sp, #44
	movs	r0, #1
.L_0811cacc:
	adds	r1, r7, #0
	bl	sub_0811a188
	str	r0, [r6, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08038130
	ldr	r0, [r6, #8]
	bl	sub_0811be3c
	ldr	r0, [r0, #0]
	movs	r1, #3
	mov	sl, r0
	bl	sub_08020090
	mov	r0, sl
	movs	r1, #16
	bl	sub_08020098
	ldrh	r3, [r5, #10]
	cmp	r3, #7
	bhi.n	.L_0811cb18
	movs	r2, #1
	mov	r8, r2
	str	r2, [r6, #4]
	movs	r0, #1
	adds	r1, r7, #0
	bl	sub_0811a188
	mov	r3, r8
	b.n	.L_0811cb26
.L_0811cb18:
	movs	r3, #0
	str	r3, [r6, #4]
	movs	r0, #2
	adds	r1, r7, #0
	bl	sub_0811a188
	movs	r3, #1
.L_0811cb26:
	str	r3, [r6, #20]
	ldr	r3, [r6, #20]
	movs	r7, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_0811cb72
	movs	r5, #0
.L_0811cb34:
	lsls	r3, r7, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	movs	r0, #0
	ldr	r1, [r3, #80]
	ldrb	r3, [r1, #27]
	subs	r3, #1
	cmp	r3, #0
	beq.n	.L_0811cb66
	add	r2, sp, #96
	mov	ip, r3
	adds	r3, r2, r5
	adds	r2, r3, #0
	subs	r2, #34
	adds	r1, #40
.L_0811cb58:
	ldmia	r1!, {r3}
	adds	r0, #1
	ldrb	r3, [r3, #5]
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r0, ip
	bne.n	.L_0811cb58
.L_0811cb66:
	ldr	r3, [r6, #20]
	adds	r7, #1
	adds	r5, #4
	adds	r2, r6, #0
	cmp	r7, r3
	bne.n	.L_0811cb34
.L_0811cb72:
	movs	r7, #0
	adds	r0, r6, #0
	str	r7, [r6, #0]
	str	r7, [r6, #24]
	bl	sub_08138020
	movs	r3, #1
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	sub_08138020
	movs	r3, #2
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	sub_08138020
	movs	r3, #3
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	sub_08138020
	adds	r0, r6, #0
	str	r7, [r6, #0]
	bl	sub_08138018
	mov	r0, sl
	movs	r1, #1
	bl	sub_08020090
	add	r5, sp, #8
	ldr	r3, [r5, #20]
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L_0811cbd0
	movs	r6, #36
.L_0811cbb8:
	str	r2, [sp, #4]
	str	r2, [sp, #0]
	ldrsh	r0, [r2, r6]
	bl	sub_0811be68
	ldr	r1, [sp, #4]
	adds	r7, #1
	ldr	r3, [r1, #20]
	adds	r6, #2
	ldr	r2, [sp, #0]
	cmp	r7, r3
	bne.n	.L_0811cbb8
.L_0811cbd0:
	ldr	r0, [r5, #8]
	bl	sub_0811be68
	movs	r0, #0
.L_0811cbd8:
	add	sp, #96
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xe000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	cmp	r3, #0
	bne.n	.L_0811cc06
	mov	r2, sp
	movs	r3, #0
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	strb	r3, [r2, #2]
	strb	r3, [r2, #3]
.L_0811cc06:
	adds	r0, r5, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	ldrh	r3, [r6, #56]
	subs	r3, r3, r7
	strh	r3, [r6, #56]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0811cc1e
	movs	r3, #0
	strh	r3, [r6, #56]
.L_0811cc1e:
	adds	r0, r5, #0
	bl	sub_0811be3c
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	sub_08020090
	movs	r0, #0
	bl	sub_08038130
	bl	sub_08038118
	cmp	r5, #7
	bhi.n	.L_0811cc5e
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_0811cc46
	ldr	r0, [pc, #148]
	bl	sub_080381c8
.L_0811cc46:
	adds	r0, r7, #0
	movs	r1, #5
	bl	sub_08038120
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #132]
	bl	sub_080381c8
	b.n	.L_0811cc88
.L_0811cc5e:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0811cc6a
	ldr	r0, [pc, #120]
	bl	sub_080381c8
.L_0811cc6a:
	adds	r0, r7, #0
	movs	r1, #5
	bl	sub_08038120
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038120
	ldr	r0, [pc, #104]
	bl	sub_080381c8
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
.L_0811cc88:
	adds	r0, r5, #0
	bl	.L_0811ccf0
	cmp	r5, #7
	bhi.n	.L_0811ccaa
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bgt.n	.L_0811ccc0
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #68]
	bl	sub_080381c8
	b.n	.L_0811ccc0
.L_0811ccaa:
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bgt.n	.L_0811ccc0
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #48]
	bl	sub_080381c8
.L_0811ccc0:
	adds	r0, r5, #0
	bl	sub_0811be3c
	movs	r1, #1
	ldr	r0, [r0, #0]
	bl	sub_08020090
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000c6f
	.4byte 0x00000c73
	.4byte 0x00000c6e
	.4byte 0x00000c72
	.4byte 0x00000c71
	.2byte 0x0c84
	.2byte 0x0000
.L_0811ccf0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bgt.n	.L_0811cd2e
	adds	r0, r6, #0
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	movs	r1, #5
	ldr	r5, [r3, #80]
	adds	r0, r5, #0
	bl	sub_08020030
	ldr	r2, [r5, #40]
	movs	r3, #6
	strb	r3, [r2, #5]
	movs	r3, #255
	strb	r3, [r2, #22]
	movs	r0, #4
	bl	sub_08013560
	adds	r0, r5, #0
	bl	sub_080200f8
	adds	r0, r6, #0
	bl	sub_0811bc64
.L_0811cd2e:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r2, #10
	ldrsh	r5, [r0, r2]
	sub	sp, #28
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	adds	r0, r5, #0
	cmp	r3, #0
	bne.n	.L_0811cd76
	cmp	r5, #127
	ble.n	.L_0811cd52
	mov	r6, sp
	movs	r0, #2
	b.n	.L_0811cd56
.L_0811cd52:
	mov	r6, sp
	movs	r0, #1
.L_0811cd56:
	adds	r1, r6, #0
	bl	sub_0811a188
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_0811cd68
	movs	r0, #128
	lsls	r0, r0, #1
	b.n	.L_0811cd76
.L_0811cd68:
	bl	sub_08014878
	adds	r3, r5, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #1
	ldrsh	r0, [r6, r3]
.L_0811cd76:
	add	sp, #28
	pop	{r5, r6, pc}
	.align 2, 0
