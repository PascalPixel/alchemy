.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_0801b010, 0x0801b010
	.set sub_0801b9a8, 0x0801b9a8
	.set sub_0801b9ec, 0x0801b9ec
	.set sub_0801ba68, 0x0801ba68
	.set sub_0801bd98, 0x0801bd98
	.global Func_0801b664
	.thumb_func
Func_0801b664:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	ldr	r0, [pc, #396]
	adds	r5, r7, r0
	ldrh	r1, [r5, #0]
	adds	r0, r7, #0
	bl	sub_0801b9a8
	ldr	r1, [pc, #388]
	movs	r3, #0
	adds	r2, r7, r1
	mov	sl, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldrh	r3, [r5, #0]
	movs	r0, #229
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r0, r0, #2
	adds	r0, r0, r7
	ldrh	r2, [r0, #0]
	mov	r8, r0
	cmp	r2, #5
	bhi.n	.L_0801b6a2
	b.n	.L_0801b7b2
.L_0801b6a2:
	movs	r1, #231
	lsls	r1, r1, #2
	adds	r6, r7, r1
	ldrh	r3, [r6, #0]
	ldrh	r2, [r5, #0]
	ldrh	r1, [r0, #0]
	adds	r3, r3, r2
	cmp	r3, r1
	bne.n	.L_0801b776
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r5, [r3, #0]
	mov	r3, sl
	strh	r3, [r7, #10]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_0801b6e4
	ldr	r0, [pc, #316]
	movs	r3, #230
	adds	r1, r7, r0
	lsls	r3, r3, #2
	ldr	r0, [pc, #312]
	adds	r2, r7, r3
.L_0801b6d2:
	ldrh	r3, [r1, #0]
	strh	r3, [r5, #24]
	ldrh	r3, [r2, #0]
	strh	r0, [r5, #20]
	strh	r3, [r5, #26]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L_0801b6d2
.L_0801b6e4:
	ldr	r0, [pc, #284]
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	movs	r1, #230
	strh	r2, [r5, #24]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	strh	r3, [r5, #26]
	ldr	r3, [pc, #272]
	lsls	r2, r2, #16
	strh	r3, [r5, #20]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	asrs	r2, r2, #16
	cmp	r2, r3
	beq.n	.L_0801b718
.L_0801b706:
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #24
	ldrsh	r2, [r5, r1]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	cmp	r2, r3
	bne.n	.L_0801b706
.L_0801b718:
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0801b73e
	movs	r2, #213
	lsls	r2, r2, #2
	adds	r6, r7, r2
.L_0801b72a:
	ldrh	r0, [r6, #0]
	ldrh	r1, [r6, #32]
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_0801bd98
	ldr	r5, [r5, #4]
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_0801b72a
.L_0801b73e:
	ldr	r0, [pc, #188]
	movs	r1, #231
	adds	r3, r7, r0
	movs	r2, #0
	lsls	r1, r1, #2
	strh	r2, [r3, #0]
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r5, [r3, #0]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	ldr	r5, [r5, #4]
	adds	r3, #16
	cmp	r5, #0
	beq.n	.L_0801b770
	movs	r2, #12
.L_0801b764:
	strh	r3, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	adds	r3, #16
	cmp	r5, #0
	bne.n	.L_0801b764
.L_0801b770:
	movs	r3, #1
	strh	r3, [r7, #62]
	b.n	.L_0801b7be
.L_0801b776:
	cmp	r2, #4
	bne.n	.L_0801b7be
	adds	r3, #1
	cmp	r3, r1
	bcs.n	.L_0801b7be
	ldr	r1, [pc, #136]
	adds	r3, r2, r1
	strh	r3, [r5, #0]
	movs	r3, #8
	strh	r3, [r7, #60]
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_0801ba68
	ldrh	r2, [r5, #0]
	ldrh	r3, [r6, #0]
	mov	r0, r8
	adds	r3, r3, r2
	ldrh	r2, [r0, #0]
	adds	r3, #2
	cmp	r3, r2
	bne.n	.L_0801b7ac
	mov	r1, sl
	strh	r1, [r7, #62]
.L_0801b7ac:
	movs	r3, #1
	strh	r3, [r7, #10]
	b.n	.L_0801b7be
.L_0801b7b2:
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_0801b7be
	mov	r2, sl
	strh	r2, [r5, #0]
.L_0801b7be:
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #56]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r3, r7, r0
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_0801b9ec
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	sub_0801b010
	movs	r0, #1
	bl	sub_080030f8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x000003a2
	.4byte 0x00000396
	.4byte 0x0000fff4
	.2byte 0xffff
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #384]
	adds	r7, r0, #0
	adds	r6, r7, r1
	ldrh	r1, [r6, #0]
	bl	sub_0801b9a8
	ldr	r3, [pc, #376]
	adds	r2, r7, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #229
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	adds	r3, r1, #0
	cmp	r3, #5
	bhi.n	.L_0801b83c
	b.n	.L_0801b948
.L_0801b83c:
	movs	r2, #231
	lsls	r2, r2, #2
	adds	r5, r7, r2
	ldrh	r1, [r5, #0]
	ldrh	r2, [r6, #0]
	adds	r3, r1, #0
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L_0801b888
	adds	r6, r2, #0
	cmp	r6, #1
	bne.n	.L_0801b87a
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_0801b87a
	movs	r3, #8
	strh	r3, [r7, #8]
	ldr	r1, [pc, #316]
	ldrh	r3, [r5, #0]
	adds	r3, r3, r1
	strh	r3, [r5, #0]
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_0801ba68
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0801b876
	strh	r3, [r7, #10]
.L_0801b876:
	strh	r6, [r7, #62]
	b.n	.L_0801b95c
.L_0801b87a:
	ldr	r3, [pc, #280]
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #280]
	adds	r3, r3, r1
	strh	r3, [r2, #0]
	b.n	.L_0801b95c
.L_0801b888:
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	movs	r0, #0
	ldr	r5, [r3, #0]
	strh	r0, [r7, #62]
	ldr	r3, [r5, #4]
	movs	r1, #64
	cmp	r3, #0
	beq.n	.L_0801b8b0
	movs	r2, #12
.L_0801b89e:
	ldrh	r3, [r5, #16]
	adds	r3, r3, r1
	strh	r3, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	subs	r1, #16
	cmp	r3, #0
	bne.n	.L_0801b89e
.L_0801b8b0:
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r5, [r3, #0]
	b.n	.L_0801b8c0
.L_0801b8ba:
	movs	r0, #1
	bl	sub_080030f8
.L_0801b8c0:
	movs	r3, #16
	ldrsh	r2, [r5, r3]
	movs	r1, #24
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	bne.n	.L_0801b8ba
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	movs	r1, #0
	cmp	r3, #5
	beq.n	.L_0801b8e6
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	subs	r3, #5
.L_0801b8e0:
	adds	r1, #1
	cmp	r1, r3
	bne.n	.L_0801b8e0
.L_0801b8e6:
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r2, #84
	ldr	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r1, [r3, #0]
	ldr	r3, [pc, #156]
	adds	r2, r7, r3
	movs	r3, #4
	strh	r3, [r2, #0]
	cmp	r5, #0
	beq.n	.L_0801b91e
	lsls	r3, r1, #1
	movs	r1, #213
	adds	r3, r3, r7
	lsls	r1, r1, #2
	adds	r6, r3, r1
.L_0801b90a:
	ldrh	r0, [r6, #0]
	ldrh	r1, [r6, #32]
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_0801bd98
	ldr	r5, [r5, #4]
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_0801b90a
.L_0801b91e:
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r1, [pc, #120]
	ldr	r5, [r3, #0]
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L_0801b942
	ldr	r2, [pc, #112]
.L_0801b934:
	strh	r1, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	adds	r1, #16
	cmp	r3, #0
	bne.n	.L_0801b934
.L_0801b942:
	movs	r3, #1
	strh	r3, [r7, #10]
	b.n	.L_0801b95c
.L_0801b948:
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0801b956
	ldr	r1, [pc, #72]
	adds	r3, r2, r1
	b.n	.L_0801b95a
.L_0801b956:
	ldr	r2, [pc, #68]
	adds	r3, r1, r2
.L_0801b95a:
	strh	r3, [r6, #0]
.L_0801b95c:
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #52]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_0801b9ec
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	sub_0801b010
	movs	r0, #1
	bl	sub_080030f8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x000003a2
	.4byte 0x0000ffff
	.4byte 0x00000396
	.4byte 0x0000fff4
