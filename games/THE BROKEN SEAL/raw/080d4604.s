.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080d4604
Overlay_080d4604:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #80
	ldr	r2, [pc, #64]
	str	r1, [sp, #56]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	ldr	r5, [pc, #56]
	ldr	r2, [r2, #8]
	mov	r9, r1
	add	r5, r9
	str	r2, [sp, #40]
	str	r0, [r5, #0]
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_080d4642
	movs	r0, #1
	bl	sub_080cd594
	movs	r3, #60
	movs	r0, #48
	str	r3, [sp, #36]
	str	r0, [sp, #32]
	b.n	.L_080d4682
.L_080d4642:
	ldr	r1, [sp, #56]
	cmp	r1, #1
	bne.n	.L_080d4660
	movs	r0, #0
	bl	sub_080cd594
	movs	r2, #60
	movs	r3, #64
	str	r2, [sp, #36]
	b.n	.L_080d4680
	movs	r0, r0
	.4byte 0x03001eec
	.2byte 0x7828
	.2byte 0x0000
.L_080d4660:
	movs	r0, #0
	bl	sub_080cd594
	ldr	r3, [r5, #0]
	add	r5, sp, #68
	ldr	r0, [r3, #8]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #36]
	ldr	r3, [r5, #4]
	adds	r3, #48
.L_080d4680:
	str	r3, [sp, #32]
.L_080d4682:
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r5, [pc, #44]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #60]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r0, sp
	adds	r0, #60
	str	r0, [sp, #24]
	mov	r1, r9
	str	r3, [r0, #4]
	b.n	.L_080d46cc
	.4byte 0x00001010
	.4byte 0x04000052
	.2byte 0x1e50
	.2byte 0x0300
.L_080d46cc:
	movs	r2, #1
	ldr	r0, [pc, #768]
	movs	r3, #1
	bl	sub_080e0524
	ldr	r0, [pc, #764]
	ldr	r1, [sp, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r1, [sp, #56]
	cmp	r1, #1
	bne.n	.L_080d46fe
	ldr	r0, [pc, #748]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #744]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
	b.n	.L_080d4718
.L_080d46fe:
	ldr	r2, [sp, #56]
	cmp	r2, #2
	bne.n	.L_080d4718
	ldr	r0, [pc, #728]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #716]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080d4718:
	movs	r3, #0
	str	r3, [sp, #48]
	ldr	r3, [pc, #708]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r1, [pc, #700]
	lsls	r3, r3, #1
	adds	r3, #2
	ldrh	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080d4736
	b.n	.L_080d4894
.L_080d4736:
	mov	r0, r9
	str	r0, [sp, #12]
.L_080d473a:
	ldr	r2, [sp, #12]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	sl, r1
	adds	r7, r2, r3
.L_080d4746:
	mov	r0, sl
	lsls	r6, r0, #1
	bl	sub_08004458
	ldr	r3, [pc, #668]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, sl
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	movs	r2, #1
	adds	r3, #25
	add	sl, r2
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080d4746
	movs	r0, #0
	mov	sl, r0
	ldr	r0, [pc, #600]
	mov	r2, r9
	ldr	r3, [r2, r0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	ldr	r1, [pc, #592]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r1, r3]
	adds	r4, r1, #0
	cmp	r3, #0
	beq.n	.L_080d486a
	ldr	r3, [sp, #32]
	lsls	r3, r3, #16
	mov	fp, r3
.L_080d47a8:
	mov	r1, r9
	adds	r5, r1, r0
	ldr	r3, [r5, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r4, r3]
	ldr	r0, [sp, #48]
	adds	r2, r3, #0
	muls	r2, r0
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r1, [pc, #552]
	lsls	r3, r3, #2
	adds	r6, r3, r1
	str	r4, [sp, #8]
	bl	sub_08004458
	ldr	r3, [pc, #544]
	ands	r3, r0
	adds	r3, #32
	mov	r8, r3
	bl	sub_08004458
	ldr	r3, [pc, #524]
	ldr	r5, [r5, #0]
	adds	r7, r0, #0
	ands	r7, r3
	ldr	r3, [r5, #4]
	ldr	r4, [sp, #8]
	cmp	r3, #1
	bne.n	.L_080d4804
	ldr	r3, [r5, #24]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [sp, #48]
	adds	r2, r2, r3
	lsls	r2, r2, #1
	adds	r2, #4
	ldr	r0, [sp, #36]
	ldrh	r3, [r4, r2]
	subs	r3, r0, r3
	adds	r3, #28
	b.n	.L_080d481a
.L_080d4804:
	ldr	r3, [r5, #24]
	ldr	r1, [sp, #48]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	adds	r2, r2, r1
	lsls	r2, r2, #1
	adds	r2, #4
	ldrh	r3, [r4, r2]
	ldr	r2, [sp, #36]
	adds	r3, r2, r3
	subs	r3, #28
.L_080d481a:
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	mov	r3, fp
	str	r3, [r6, #4]
	adds	r0, r7, #0
	bl	sub_08002322
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	adds	r0, r7, #0
	bl	sub_0800231c
	mov	r3, r8
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r6, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	movs	r0, #1
	add	sl, r0
	ldr	r0, [pc, #404]
	adds	r3, #32
	mov	r2, r9
	str	r3, [r6, #24]
	ldr	r3, [r2, r0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	ldr	r1, [pc, #392]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r1, r3]
	adds	r4, r1, #0
	cmp	sl, r3
	bne.n	.L_080d47a8
.L_080d486a:
	ldr	r3, [sp, #12]
	movs	r0, #224
	lsls	r0, r0, #1
	ldr	r2, [sp, #48]
	adds	r3, r3, r0
	str	r3, [sp, #12]
	adds	r2, #1
	ldr	r3, [pc, #360]
	str	r2, [sp, #48]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #2
	ldrh	r3, [r1, r3]
	ldr	r0, [sp, #48]
	cmp	r0, r3
	beq.n	.L_080d4894
	b.n	.L_080d473a
.L_080d4894:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #344]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #336]
	bl	sub_080041d8
	movs	r1, #0
	str	r1, [sp, #44]
	ldr	r0, [pc, #300]
	mov	r2, r9
	ldr	r3, [r2, r0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r1, [pc, #292]
	lsls	r3, r3, #1
	adds	r3, #2
	ldrh	r3, [r1, r3]
	ldr	r2, [pc, #308]
	cmp	r3, r2
	bne.n	.L_080d48d0
	b.n	.L_080d4c9c
.L_080d48d0:
	ldr	r3, [sp, #56]
	ldr	r1, [pc, #272]
	subs	r3, #1
	add	r1, r9
	str	r3, [sp, #20]
	str	r1, [sp, #28]
.L_080d48dc:
	ldr	r3, [pc, #292]
	ldr	r1, [r3, #0]
	mov	r3, r9
	ldr	r2, [r3, r0]
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	.L_080d4908
	ldr	r0, [sp, #44]
	cmp	r0, #51
	bgt.n	.L_080d4908
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_080d4900
	ldrh	r3, [r1, #54]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r3, r2
	b.n	.L_080d4906
.L_080d4900:
	ldrh	r3, [r1, #54]
	ldr	r0, [pc, #260]
	adds	r3, r3, r0
.L_080d4906:
	strh	r3, [r1, #54]
.L_080d4908:
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #3
	bne.n	.L_080d4926
	ldr	r2, [sp, #44]
	cmp	r2, #4
	bne.n	.L_080d4926
	movs	r1, #128
	ldr	r3, [pc, #240]
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	ldr	r2, [pc, #236]
	bl	sub_080072f0
.L_080d4926:
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bhi.n	.L_080d493a
	ldr	r0, [sp, #44]
	cmp	r0, #2
	bne.n	.L_080d4952
	movs	r0, #145
	bl	sub_080b50e8
	b.n	.L_080d4952
.L_080d493a:
	ldr	r1, [sp, #44]
	cmp	r1, #2
	bne.n	.L_080d4946
	movs	r0, #145
	bl	sub_080f9010
.L_080d4946:
	ldr	r2, [sp, #44]
	cmp	r2, #24
	bne.n	.L_080d4952
	movs	r0, #134
	bl	sub_080b50e8
.L_080d4952:
	movs	r3, #0
	str	r3, [sp, #48]
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldr	r1, [pc, #132]
	adds	r3, #2
	ldrh	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080d496e
	b.n	.L_080d4c5c
.L_080d496e:
	mov	r2, r9
	str	r2, [sp, #16]
.L_080d4972:
	ldr	r3, [sp, #48]
	ldr	r0, [sp, #44]
	lsls	r3, r3, #3
	mov	fp, r3
	cmp	r0, fp
	bne.n	.L_080d4986
	ldr	r2, [pc, #148]
	movs	r3, #12
	add	r2, r9
	str	r3, [r2, #0]
.L_080d4986:
	ldr	r1, [sp, #44]
	cmp	r1, fp
	bge.n	.L_080d498e
	b.n	.L_080d4af8
.L_080d498e:
	mov	r3, fp
	adds	r3, #2
	cmp	r1, r3
	bge.n	.L_080d4a46
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080d4a18
	ldr	r2, [r2, #24]
	ldr	r0, [sp, #48]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r3, r3, r0
	ldr	r1, [pc, #60]
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r2, [r1, r3]
	ldr	r3, [sp, #36]
	subs	r2, r3, r2
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #64
	str	r3, [sp, #4]
	ldr	r3, [sp, #32]
	adds	r2, #12
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	mov	r1, r9
	subs	r3, #32
	bl	sub_080072f4
	b.n	.L_080d4a46
	.4byte 0x0000007d
	.4byte 0x00000073
	.4byte 0x00000087
	.4byte 0x03001388
	.4byte 0x000000c4
	.4byte 0x00007828
	.4byte 0x080ee262
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x1ffffff9
	.4byte 0x03001e80
	.4byte 0xffffff00
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.2byte 0x77a8
	.2byte 0x0000
.L_080d4a18:
	ldr	r2, [r2, #24]
	ldr	r0, [sp, #48]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r3, r3, r0
	ldr	r1, [pc, #672]
	lsls	r3, r3, #1
	adds	r3, #4
	ldrh	r2, [r1, r3]
	ldr	r3, [sp, #36]
	adds	r2, r3, r2
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #64
	str	r3, [sp, #4]
	ldr	r3, [sp, #32]
	subs	r2, #44
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	mov	r1, r9
	subs	r3, #32
	bl	sub_080072f4
.L_080d4a46:
	ldr	r0, [sp, #44]
	cmp	r0, fp
	blt.n	.L_080d4af8
	ldr	r2, [pc, #628]
	ldr	r3, [sp, #16]
	movs	r0, #225
	movs	r1, #0
	lsls	r0, r0, #7
	mov	sl, r1
	mov	r8, r2
	adds	r5, r3, r0
.L_080d4a5c:
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	ldr	r2, [sp, #32]
	adds	r7, r3, r2
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080d4a90
	ldr	r2, [r2, #24]
	movs	r0, #2
	ldrsh	r1, [r5, r0]
	ldr	r3, [sp, #36]
	ldr	r0, [sp, #48]
	adds	r1, r1, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, #4
	mov	r2, r8
	ldrh	r3, [r2, r3]
	subs	r1, r1, r3
	adds	r6, r1, #0
	adds	r6, #28
	b.n	.L_080d4ab0
.L_080d4a90:
	ldr	r2, [r2, #24]
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #48]
	ldr	r0, [sp, #36]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r1, r1, r0
	adds	r3, #4
	mov	r0, r8
	ldrh	r3, [r0, r3]
	adds	r1, r1, r3
	adds	r6, r1, #0
	subs	r6, #28
.L_080d4ab0:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_080d4ade
	movs	r1, #3
	bl	sub_080022ec
	ldr	r2, [pc, #520]
	ldrb	r1, [r2, r0]
	movs	r0, #32
	str	r0, [sp, #0]
	lsls	r1, r1, #11
	movs	r0, #64
	adds	r2, r6, #0
	adds	r3, r7, #0
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #16
	subs	r3, #32
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r0, [r5, #24]
.L_080d4ade:
	cmp	r0, #0
	ble.n	.L_080d4ae6
	subs	r3, r0, #1
	b.n	.L_080d4aea
.L_080d4ae6:
	movs	r3, #1
	negs	r3, r3
.L_080d4aea:
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #12
	bne.n	.L_080d4a5c
.L_080d4af8:
	mov	r3, fp
	ldr	r1, [sp, #44]
	adds	r3, #5
	cmp	r1, r3
	ble.n	.L_080d4be4
	ldr	r2, [sp, #56]
	ldr	r7, [pc, #452]
	cmp	r2, #2
	beq.n	.L_080d4b0e
	movs	r7, #128
	lsls	r7, r7, #5
.L_080d4b0e:
	ldr	r4, [pc, #448]
	movs	r3, #0
	mov	r2, r9
	mov	sl, r3
	ldr	r3, [r2, r4]
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #424]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r0, r1, #0
	lsls	r3, r3, #1
	ldrh	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080d4be4
.L_080d4b2a:
	mov	r2, r9
	ldr	r3, [r2, r4]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r0, r3]
	ldr	r0, [sp, #48]
	adds	r2, r3, #0
	muls	r2, r0
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #396]
	lsls	r3, r3, #2
	adds	r6, r3, r2
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080d4bca
	adds	r0, r6, #0
	adds	r2, r7, #0
	movs	r1, #60
	bl	sub_080e3908
	ldr	r3, [r6, #24]
	movs	r0, #216
	ldr	r2, [r6, #4]
	subs	r3, #1
	lsls	r0, r0, #15
	str	r3, [r6, #24]
	cmp	r2, r0
	ble.n	.L_080d4b78
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080d4bc8
.L_080d4b78:
	ldr	r0, [r6, #0]
	ldr	r1, [pc, #348]
	cmp	r0, r1
	bhi.n	.L_080d4bc8
	cmp	r2, #0
	blt.n	.L_080d4bc8
	asrs	r2, r2, #16
	asrs	r6, r0, #16
	movs	r1, #5
	adds	r0, r3, #0
	mov	r8, r2
	bl	sub_080022ec
	adds	r0, #1
	mov	r2, sl
	movs	r4, #1
	lsls	r5, r0, #1
	ands	r4, r2
	ldr	r2, [pc, #316]
	subs	r3, r5, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #40]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	mov	r2, r8
	subs	r2, r2, r0
	str	r0, [sp, #0]
	subs	r6, r6, r3
	str	r5, [sp, #4]
	ldr	r3, [sp, #24]
	mov	r8, r2
	lsls	r4, r4, #2
	ldr	r4, [r4, r3]
	ldr	r0, [sp, #52]
	adds	r2, r6, #0
	mov	r3, r8
	bl	sub_080072f4
.L_080d4bc8:
	ldr	r1, [pc, #248]
.L_080d4bca:
	ldr	r4, [pc, #260]
	mov	r2, r9
	ldr	r3, [r2, r4]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	movs	r0, #1
	adds	r3, r3, r2
	add	sl, r0
	lsls	r3, r3, #1
	adds	r0, r1, #0
	ldrh	r3, [r0, r3]
	cmp	sl, r3
	bne.n	.L_080d4b2a
.L_080d4be4:
	ldr	r2, [pc, #232]
	movs	r3, #0
	mov	r0, r9
	mov	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d4c32
	mov	r7, fp
	adds	r7, #6
	movs	r6, #36
.L_080d4bfa:
	ldr	r1, [sp, #44]
	cmp	r1, r7
	bne.n	.L_080d4c20
	mov	r3, r9
	adds	r5, r3, r2
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, sl
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
.L_080d4c20:
	ldr	r2, [pc, #172]
	movs	r3, #1
	mov	r0, r9
	add	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_080d4bfa
.L_080d4c32:
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r1, r1, r2
	adds	r3, #1
	str	r1, [sp, #16]
	str	r3, [sp, #48]
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldr	r1, [pc, #116]
	adds	r3, #2
	ldrh	r3, [r1, r3]
	ldr	r2, [sp, #48]
	cmp	r2, r3
	beq.n	.L_080d4c5c
	b.n	.L_080d4972
.L_080d4c5c:
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [sp, #44]
	adds	r3, #1
	str	r3, [sp, #44]
	ldr	r0, [pc, #80]
	mov	r1, r9
	ldr	r3, [r1, r0]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldr	r2, [pc, #56]
	adds	r3, #2
	ldrh	r3, [r2, r3]
	ldr	r1, [sp, #44]
	lsls	r3, r3, #3
	adds	r3, #56
	cmp	r1, r3
	beq.n	.L_080d4c9c
	b.n	.L_080d48dc
.L_080d4c9c:
	ldr	r0, [pc, #68]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ee262
	.4byte 0x080ee294
	.4byte 0xfffff000
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
