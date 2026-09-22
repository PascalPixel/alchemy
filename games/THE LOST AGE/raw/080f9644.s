.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038068, 0x08038068
	.set sub_08038080, 0x08038080
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038288, 0x08038288
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad020, 0x080ad020
	.set sub_080ad048, 0x080ad048
	.set sub_080ad050, 0x080ad050
	.set sub_080ad1e8, 0x080ad1e8
	.set sub_080ad288, 0x080ad288
	.set sub_080c8510, 0x080c8510
	.set sub_080c85d0, 0x080c85d0
	.set sub_080f8170, 0x080f8170
	.set sub_080f8888, 0x080f8888
	.set sub_080f8ce8, 0x080f8ce8
	.set sub_080f9374, 0x080f9374
	.set sub_080fa2d0, 0x080fa2d0
	.set sub_080fa458, 0x080fa458
	.set sub_080fa50c, 0x080fa50c
	.set sub_080fa870, 0x080fa870
	.set sub_080faca0, 0x080faca0
	.set sub_080facb4, 0x080facb4
	.set sub_080fad1c, 0x080fad1c
	.set sub_080fad48, 0x080fad48
	.set sub_080fad88, 0x080fad88
	.set sub_080fadd0, 0x080fadd0
	.set sub_080fae2c, 0x080fae2c
	.set sub_080fae8c, 0x080fae8c
	.set sub_080fb104, 0x080fb104
	.set sub_080fb638, 0x080fb638
	.set sub_080fb6d4, 0x080fb6d4
	.set sub_080fb780, 0x080fb780
	.set sub_080fbd9c, 0x080fbd9c
	.set sub_080fbdbc, 0x080fbdbc
	.set sub_080fbddc, 0x080fbddc
	.set sub_080fbe48, 0x080fbe48
	.set sub_080fbe6c, 0x080fbe6c
	.set sub_080fc12c, 0x080fc12c
	.set sub_080fc1ac, 0x080fc1ac
	.set sub_080fc2e0, 0x080fc2e0
	.set sub_080fc6bc, 0x080fc6bc
	.set sub_081c0010, 0x081c0010
	.global Overlay_080f9644
Overlay_080f9644:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r1, [sp, #32]
	movs	r1, #0
	str	r0, [sp, #36]
	str	r2, [sp, #28]
	str	r1, [sp, #24]
	str	r1, [sp, #20]
	str	r1, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r8, r1
	mov	r9, r3
	bl	.L_080fa280
.L_080f9672:
	mov	r2, r8
	cmp	r2, #12
	bls.n	.L_080f967c
	bl	.L_080fa27c
.L_080f967c:
	lsls	r3, r2, #2
	ldr	r2, [pc, #896]
	ldr	r3, [r3, r2]
	mov	pc, r3
	str	r6, [sp, #736]
	lsrs	r7, r1, #32
	str	r7, [sp, #96]
	lsrs	r7, r1, #32
	ldr	r0, [sp, #800]
	lsrs	r7, r1, #32
	add	r0, pc, #704
	lsrs	r7, r1, #32
	ldr	r3, [sp, #600]
	lsrs	r7, r1, #32
	ldr	r2, [sp, #96]
	lsrs	r7, r1, #32
	ldr	r1, [sp, #656]
	lsrs	r7, r1, #32
	ldr	r5, [sp, #856]
	lsrs	r7, r1, #32
	add	r2, pc, #512
	lsrs	r7, r1, #32
	str	r7, [sp, #520]
	lsrs	r7, r1, #32
	add	r2, pc, #160
	lsrs	r7, r1, #32
	add	r1, pc, #552
	lsrs	r7, r1, #32
	add	r2, pc, #432
	lsrs	r7, r1, #32
	movs	r2, #180
	lsls	r2, r2, #1
	add	r2, r9
	movs	r3, #0
	strh	r3, [r2, #0]
	bl	sub_080fbe48
	bl	sub_080fbdbc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r1, [pc, #812]
	movs	r0, #0
	bl	sub_080facb4
	mov	r3, r9
	ldr	r0, [r3, #48]
	bl	sub_08038260
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_080f9374
	movs	r0, #0
	bl	sub_080fa50c
	movs	r3, #1
	adds	r7, r0, #0
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L_080f9708
	movs	r2, #0
	str	r3, [sp, #16]
	movs	r3, #1
	str	r2, [sp, #20]
	str	r3, [sp, #24]
.L_080f9708:
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08038260
	bl	sub_080fa458
	bl	.L_080fa276
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_080fad1c
	movs	r3, #0
	mov	r8, r3
	cmp	r0, #0
	bne.n	.L_080f9732
	bl	.L_080fa280
.L_080f9732:
	bl	sub_080fbe48
	bl	sub_080fbdbc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r1, r9
	ldr	r2, [r1, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r1, [pc, #696]
	movs	r0, #0
	bl	sub_080facb4
	movs	r0, #0
	bl	sub_080fc6bc
	movs	r3, #1
	movs	r2, #0
	negs	r3, r3
	str	r0, [sp, #20]
	mov	r8, r2
	cmp	r0, r3
	bne.n	.L_080f976e
	bl	.L_080fa280
.L_080f976e:
	movs	r2, #177
	lsls	r2, r2, #1
	adds	r2, #255
	add	r2, r9
	movs	r3, #255
	movs	r1, #9
	strb	r3, [r2, #0]
	mov	r8, r1
	bl	.L_080fa280
	bl	sub_080fb104
	movs	r5, #1
	adds	r7, r0, #0
	negs	r5, r5
	cmp	r7, r5
	bne.n	.L_080f97a0
	movs	r3, #128
	movs	r2, #1
	lsls	r3, r3, #2
	mov	r8, r2
	adds	r3, #30
	add	r3, r9
	mov	r1, r8
	strh	r1, [r3, #0]
.L_080f97a0:
	cmp	r7, #0
	bne.n	.L_080f9898
	movs	r2, #182
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r3, [r2, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	mov	sl, r2
	bl	sub_080c8510
	cmp	r0, #0
	beq.n	.L_080f97ec
	movs	r3, #1
	str	r3, [sp, #24]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #36]
	str	r3, [r1, #0]
	ldr	r2, [sp, #32]
	mov	r3, sl
	str	r7, [r2, #0]
	ldr	r1, [sp, #28]
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r2
	movs	r2, #1
	str	r3, [r1, #0]
	str	r2, [sp, #16]
	bl	.L_080fa280
.L_080f97ec:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldrh	r1, [r2, #0]
	mov	fp, r3
	bl	sub_080fb638
	adds	r6, r0, #0
	cmp	r6, #1
	bne.n	.L_080f980a
	movs	r3, #2
	mov	r8, r3
.L_080f980a:
	cmp	r6, #2
	bne.n	.L_080f9864
	bl	sub_080fa2d0
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08038268
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #94
	add	r3, r9
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #484]
	adds	r2, r5, #0
	adds	r0, r0, r3
	movs	r1, #0
	bl	sub_080f8ce8
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r5, #226
	movs	r3, #13
	mov	r1, fp
	lsls	r5, r5, #1
	strb	r3, [r2, #5]
	ldrb	r0, [r1, #0]
	add	r5, r9
	bl	sub_08016ca4
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080fad88
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080fadd0
	movs	r2, #0
	mov	r8, r2
.L_080f9864:
	adds	r3, r6, #1
	cmp	r3, #1
	bhi.n	.L_080f9898
	movs	r3, #1
	str	r3, [sp, #24]
	mov	r1, fp
	ldrb	r3, [r1, #0]
	ldr	r2, [sp, #36]
	str	r3, [r2, #0]
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #32]
	str	r3, [r1, #0]
	mov	r3, sl
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r1, [sp, #28]
	adds	r3, #255
	ands	r3, r2
	movs	r2, #1
	str	r3, [r1, #0]
	str	r2, [sp, #16]
.L_080f9898:
	cmp	r7, #1
	bne.n	.L_080f98a0
	movs	r3, #3
	mov	r8, r3
.L_080f98a0:
	cmp	r7, #3
	bne.n	.L_080f98a8
	movs	r1, #6
	mov	r8, r1
.L_080f98a8:
	cmp	r7, #5
	bne.n	.L_080f98b0
	movs	r2, #5
	mov	r8, r2
.L_080f98b0:
	cmp	r7, #4
	bne.n	.L_080f98b8
	movs	r3, #11
	mov	r8, r3
.L_080f98b8:
	cmp	r7, #2
	beq.n	.L_080f98c0
	bl	.L_080fa280
.L_080f98c0:
	movs	r1, #10
	mov	r8, r1
	bl	.L_080fa280
	mov	r5, r9
	adds	r5, #240
	bl	sub_080fa458
	bl	sub_080fbddc
	bl	sub_080fbd9c
	ldr	r0, [r5, #0]
	bl	sub_08038260
	bl	sub_080fc12c
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #296]
	movs	r2, #16
	movs	r3, #16
	bl	sub_08038080
	movs	r0, #0
	bl	sub_080fa870
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L_080f99f6
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	movs	r7, #0
	bl	sub_080faca0
	cmp	r0, #0
	beq.n	.L_080f9918
	movs	r7, #8
.L_080f9918:
	bl	sub_080fa2d0
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r2, r9
	add	r3, r9
	adds	r6, r0, #0
	ldrb	r1, [r3, #0]
	ldr	r0, [r2, #40]
	adds	r3, r7, #0
	movs	r2, #0
	bl	sub_080f8170
	cmp	r6, r5
	beq.n	.L_080f9968
	mov	r3, r9
	ldr	r0, [r3, #48]
	bl	sub_08038268
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #94
	add	r3, r9
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r3, [pc, #188]
	movs	r1, #0
	adds	r0, r0, r3
	adds	r2, r5, #0
	bl	sub_080f8ce8
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	sub_080fb6d4
	movs	r1, #1
	mov	r8, r1
.L_080f9968:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	movs	r5, #226
	add	r3, r9
	lsls	r5, r5, #1
	ldrb	r0, [r3, #0]
	add	r5, r9
	bl	sub_08016ca4
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080fad88
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080fadd0
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #30
	add	r2, r9
	movs	r3, #1
	strh	r3, [r2, #0]
	bl	.L_080fa280
	mov	r5, r9
	adds	r5, #240
	bl	sub_080fbddc
	bl	sub_080fbd9c
	ldr	r0, [r5, #0]
	bl	sub_08038260
	bl	sub_080fc12c
	ldr	r1, [r5, #0]
	movs	r3, #16
	ldr	r0, [pc, #84]
	movs	r2, #16
	bl	sub_08038080
	movs	r0, #1
	bl	sub_080fa870
	movs	r1, #1
	movs	r3, #4
	negs	r1, r1
	mov	r8, r3
	cmp	r0, r1
	beq.n	.L_080f99dc
	bl	.L_080fa280
.L_080f99dc:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r3, [r3, #0]
	movs	r2, #180
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080fae8c
.L_080f99f6:
	movs	r2, #9
	mov	r8, r2
	bl	.L_080fa280
	movs	r0, r0
	.4byte 0x080f9684
	.4byte 0x00001007
	.4byte 0x00001008
	.4byte 0x00001120
	.4byte 0x0000100a
	.2byte 0x100b
	.2byte 0x0000
	movs	r5, #182
	lsls	r5, r5, #1
	add	r5, r9
	bl	sub_080fa458
	ldrh	r3, [r5, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_080f9a56
	ldrh	r3, [r5, #0]
	lsrs	r3, r3, #11
	adds	r5, r3, #1
	cmp	r5, #1
	ble.n	.L_080f9a56
	bl	sub_080fc12c
	movs	r0, #0
	adds	r1, r5, #0
	movs	r2, #1
	bl	sub_080fbe6c
	adds	r6, r0, #0
.L_080f9a56:
	movs	r1, #1
	movs	r3, #9
	negs	r1, r1
	mov	r8, r3
	cmp	r6, r1
	bne.n	.L_080f9a66
	bl	.L_080fa280
.L_080f9a66:
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r2, #255
	add	r2, r9
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r3, #182
	lsls	r3, r3, #1
	movs	r2, #128
	add	r3, r9
	lsls	r2, r2, #1
	ldrh	r3, [r3, #0]
	adds	r2, #255
	mov	r8, r2
	movs	r5, #134
	mov	r1, r8
	lsls	r5, r5, #2
	add	r5, r9
	ands	r1, r3
	lsls	r3, r6, #11
	orrs	r1, r3
	ldr	r3, [r5, #0]
	movs	r0, #2
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	sub_08038288
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r3, #120
	ldr	r2, [r5, #0]
	strh	r3, [r2, #6]
	movs	r3, #28
	ldr	r2, [r5, #0]
	strh	r3, [r2, #8]
	ldr	r0, [r5, #0]
	bl	sub_080f8888
	mov	r3, r9
	ldr	r0, [r3, #56]
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #72
	movs	r3, #120
	bl	sub_08038068
	mov	r3, r9
	adds	r3, #240
	ldr	r0, [r3, #0]
	bl	sub_08038260
	ldr	r0, [sp, #20]
	bl	sub_080fc1ac
	cmp	r0, #0
	bne.n	.L_080f9b72
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r7, [r3, #0]
	adds	r0, r7, #0
	bl	sub_08016ca4
	adds	r0, r6, #1
	cmp	r0, #0
	ble.n	.L_080f9b1a
	adds	r5, r0, #0
	mov	r6, r8
.L_080f9af4:
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_080ad050
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r0, r6, #0
	ands	r0, r3
	movs	r1, #1
	subs	r5, #1
	bl	sub_080ad1e8
	cmp	r5, #0
	bne.n	.L_080f9af4
.L_080f9b1a:
	adds	r0, r7, #0
	bl	sub_080ad288
	adds	r0, r7, #0
	bl	sub_080ad008
	bl	sub_080fbdbc
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r1, #0
	bl	sub_080fae2c
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #13
	strb	r2, [r3, #5]
	mov	r1, r9
	ldr	r3, [r1, #20]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	sub_08013560
	mov	r2, r9
	ldr	r0, [r2, #48]
	bl	sub_08038268
	movs	r2, #13
	ldr	r0, [pc, #760]
	movs	r1, #15
	bl	sub_080f8ce8
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #30
	movs	r3, #1
	add	r2, r9
	strh	r3, [r2, #0]
	b.n	.L_080f9b74
.L_080f9b72:
	movs	r3, #9
.L_080f9b74:
	mov	r8, r3
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_080ad008
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	sub_080c85d0
	b.n	.L_080fa280
	movs	r5, #182
	lsls	r5, r5, #1
	add	r5, r9
	movs	r7, #128
	ldrh	r3, [r5, #0]
	lsls	r7, r7, #1
	adds	r7, #255
	adds	r0, r7, #0
	movs	r1, #0
	ands	r0, r3
	mov	sl, r1
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9c86
	movs	r6, #140
	ldrh	r3, [r5, #0]
	lsls	r6, r6, #1
	adds	r6, #255
	add	r6, r9
	adds	r1, r7, #0
	ldrb	r0, [r6, #0]
	ands	r1, r3
	bl	sub_080fad48
	adds	r5, r0, #0
	cmp	r5, #30
	bne.n	.L_080f9bd8
	movs	r2, #1
	mov	sl, r2
.L_080f9bd8:
	ldrb	r0, [r6, #0]
	bl	sub_080fad1c
	cmp	r0, #15
	bne.n	.L_080f9be6
	cmp	r5, #0
	beq.n	.L_080f9cae
.L_080f9be6:
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r1, sl
	lsrs	r3, r3, #11
	adds	r3, #1
	cmp	r1, #0
	bne.n	.L_080f9cdc
	lsls	r2, r3, #24
	asrs	r1, r2, #24
	adds	r3, r5, r1
	cmp	r3, #30
	ble.n	.L_080f9c06
	movs	r3, #30
	subs	r1, r3, r5
.L_080f9c06:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r2, r3
	ble.n	.L_080f9c1a
	movs	r0, #0
	movs	r2, #0
	bl	sub_080fbe6c
	adds	r6, r0, #0
	b.n	.L_080f9c1c
.L_080f9c1a:
	movs	r6, #0
.L_080f9c1c:
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080f9c26
	b.n	.L_080f9df8
.L_080f9c26:
	movs	r7, #0
	adds	r6, #1
	cmp	r7, r6
	bge.n	.L_080f9cdc
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #255
	mov	fp, r3
.L_080f9c36:
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r1, fp
	ands	r1, r3
	bl	sub_080ad020
	movs	r1, #1
	adds	r5, r0, #0
	negs	r1, r1
	cmp	r5, r1
	beq.n	.L_080f9c7a
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_080ad050
	movs	r3, #181
	lsls	r3, r3, #1
	add	r3, r9
	strh	r5, [r3, #0]
	b.n	.L_080f9c7e
.L_080f9c7a:
	movs	r2, #1
	mov	sl, r2
.L_080f9c7e:
	adds	r7, #1
	cmp	r7, r6
	blt.n	.L_080f9c36
	b.n	.L_080f9cdc
.L_080f9c86:
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #255
	ands	r1, r3
	bl	sub_080ad020
	movs	r5, #1
	adds	r6, r0, #0
	negs	r5, r5
	cmp	r6, r5
	bne.n	.L_080f9cb4
.L_080f9cae:
	movs	r3, #7
	mov	r8, r3
	b.n	.L_080fa280
.L_080f9cb4:
	movs	r3, #181
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_080ad050
	adds	r6, r0, #0
	cmp	r6, r5
	bne.n	.L_080f9cdc
	movs	r1, #1
	mov	sl, r1
.L_080f9cdc:
	movs	r5, #128
	lsls	r5, r5, #2
	movs	r7, #140
	adds	r5, #22
	lsls	r7, r7, #1
	add	r5, r9
	adds	r7, #255
	add	r7, r9
	ldrb	r0, [r5, #0]
	bl	sub_080ad288
	ldrb	r0, [r7, #0]
	bl	sub_080ad288
	ldrb	r0, [r5, #0]
	bl	sub_080ad008
	ldrb	r0, [r7, #0]
	bl	sub_080ad008
	mov	r2, sl
	movs	r6, #1
	cmp	r2, #0
	bne.n	.L_080f9d38
	ldrb	r3, [r7, #0]
	movs	r2, #182
	strb	r3, [r5, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #0]
	mov	r3, r9
	adds	r3, #240
	ldr	r0, [r3, #0]
	bl	sub_08038260
	bl	sub_080fc12c
	movs	r0, #0
	bl	sub_080fc2e0
	adds	r6, r0, #0
.L_080f9d38:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080f9d46
	b.n	.L_080fa280
.L_080f9d46:
	movs	r1, #1
	ldrb	r0, [r7, #0]
	bl	sub_080fae2c
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r0, #1
	bl	sub_08013560
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080f9d6e
	mov	r2, r9
	ldr	r0, [r2, #48]
	bl	sub_08038268
	ldr	r0, [pc, #240]
	b.n	.L_080f9d7c
.L_080f9d6e:
	mov	r3, r9
	ldr	r0, [r3, #48]
	bl	sub_08038268
	cmp	r6, #1
	bne.n	.L_080f9d86
	ldr	r0, [pc, #228]
.L_080f9d7c:
	movs	r1, #15
	movs	r2, #14
	bl	sub_080f8ce8
	b.n	.L_080fa0a6
.L_080f9d86:
	ldrb	r3, [r7, #0]
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r9
	adds	r0, r3, #0
	ldrh	r1, [r2, #0]
	movs	r2, #0
	bl	sub_080fae8c
	ldr	r5, [pc, #200]
	movs	r2, #14
	adds	r0, r5, #0
	movs	r1, #15
	bl	sub_080f8ce8
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080f9dbc
	b.n	.L_080fa0a6
.L_080f9dbc:
	movs	r0, #103
	bl	sub_081c0010
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08038268
	adds	r0, r5, #7
	movs	r1, #14
	movs	r2, #14
	bl	sub_080f8ce8
	b.n	.L_080fa0a6
	movs	r3, #0
	mov	sl, r3
	bl	sub_080fbe48
	bl	sub_080fbdbc
	ldr	r1, [pc, #132]
	movs	r0, #0
	bl	sub_080facb4
	movs	r0, #1
	bl	sub_080fc6bc
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080f9e04
.L_080f9df8:
	movs	r2, #6
	mov	r8, r2
	b.n	.L_080fa280
.L_080f9dfe:
	movs	r3, #1
	mov	sl, r3
	b.n	.L_080f9f04
.L_080f9e04:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r3, #140
	str	r0, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #8]
	adds	r0, r5, #0
	bl	sub_08014d78
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_08014d78
	adds	r2, r5, #0
	ldr	r1, [sp, #12]
	mov	r8, r0
	ldr	r6, [pc, #44]
	mov	r0, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c2a
	mov	r0, r8
	ldr	r1, [sp, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x35ca
	movs	r7, #0
	add	r5, r9
	b.n	.L_080f9e76
	movs	r0, r0
	.4byte 0x000010ae
	.4byte 0x000010b6
	.4byte 0x000010b0
	.4byte 0x000010ad
	.4byte 0x0000100c
	.2byte 0x0730
	.2byte 0x0300
.L_080f9e70:
	adds	r3, r7, #1
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
.L_080f9e76:
	cmp	r7, #29
	bhi.n	.L_080f9e9a
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r0, [r5, #0]
	ldrh	r1, [r3, #0]
	bl	sub_080ad050
	adds	r6, r0, #0
	cmp	r6, #2
	beq.n	.L_080f9e9a
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080f9e70
	movs	r2, #1
	mov	sl, r2
.L_080f9e9a:
	adds	r3, r7, #1
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
	movs	r5, #0
.L_080f9ea2:
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9ed4
	ldr	r0, [pc, #28]
	ands	r0, r2
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f9ed4
	movs	r3, #1
	mov	sl, r3
	b.n	.L_080f9ed4
	movs	r0, r0
	.4byte 0x00000200
	.2byte 0x01ff
	.2byte 0x0000
.L_080f9ed4:
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #181
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_080ad050
	adds	r6, r0, #0
	cmp	r6, #2
	beq.n	.L_080f9f04
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080f9efa
	b.n	.L_080f9dfe
.L_080f9efa:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #29
	bls.n	.L_080f9ea2
.L_080f9f04:
	adds	r3, r5, #1
	ldr	r2, [pc, #8]
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	b.n	.L_080f9f24
	movs	r0, r0
	.2byte 0x05ff
	.2byte 0x0000
.L_080f9f14:
	movs	r3, #181
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
.L_080f9f24:
	cmp	r7, #0
	beq.n	.L_080f9f54
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r1, r2, #0
	ands	r1, r3
	str	r2, [sp, #4]
	bl	sub_080ad020
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	ldr	r2, [sp, #4]
	cmp	r6, r3
	bne.n	.L_080f9f14
	movs	r1, #1
	mov	sl, r1
.L_080f9f54:
	ldr	r7, [pc, #0]
	b.n	.L_080f9f6c
	.2byte 0x05ff
	.2byte 0x0000
.L_080f9f5c:
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	adds	r3, r5, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
.L_080f9f6c:
	cmp	r5, #0
	beq.n	.L_080f9f98
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r1, r7, #0
	ands	r1, r3
	bl	sub_080ad020
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_080f9f5c
	movs	r3, #1
	mov	sl, r3
.L_080f9f98:
	movs	r0, #1
	bl	sub_08013560
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080f9fcc
	movs	r2, #166
	ldr	r0, [sp, #12]
	ldr	r5, [pc, #780]
	mov	r1, fp
	lsls	r2, r2, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x22a6
	mov	r1, r8
	ldr	r0, [sp, #8]
	lsls	r2, r2, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x464a
	ldr	r0, [r2, #48]
	bl	sub_08038268
	ldr	r0, [pc, #756]
	movs	r1, #15
	b.n	.L_080fa088
.L_080f9fcc:
	movs	r5, #128
	lsls	r5, r5, #2
	movs	r7, #140
	adds	r5, #22
	lsls	r7, r7, #1
	add	r5, r9
	adds	r7, #255
	add	r7, r9
	ldrb	r0, [r5, #0]
	bl	sub_080ad288
	ldrb	r0, [r7, #0]
	bl	sub_080ad288
	ldrb	r0, [r5, #0]
	bl	sub_080ad008
	ldrb	r0, [r7, #0]
	bl	sub_080ad008
	bl	sub_080fbddc
	mov	r3, r9
	adds	r3, #240
	ldr	r0, [r3, #0]
	bl	sub_08038260
	ldrb	r3, [r7, #0]
	strb	r3, [r5, #0]
	movs	r5, #182
	lsls	r5, r5, #1
	add	r5, r9
	ldrh	r2, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r5, #0]
	bl	sub_080fc12c
	movs	r0, #0
	bl	sub_080fc2e0
	adds	r6, r0, #0
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fa09a
	mov	r3, r9
	ldr	r0, [r3, #48]
	bl	sub_08038268
	bl	sub_080fbd9c
	ldrb	r0, [r7, #0]
	movs	r1, #1
	bl	sub_080fae2c
	cmp	r6, #0
	bne.n	.L_080fa090
	ldrb	r3, [r7, #0]
	movs	r2, #181
	lsls	r2, r2, #1
	add	r2, r9
	adds	r0, r3, #0
	ldrh	r1, [r2, #0]
	movs	r2, #0
	bl	sub_080fae8c
	ldr	r6, [pc, #612]
	movs	r2, #14
	adds	r0, r6, #0
	movs	r1, #15
	bl	sub_080f8ce8
	ldrh	r0, [r5, #0]
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fa09a
	movs	r0, #103
	bl	sub_081c0010
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08038268
	adds	r0, r6, #7
	movs	r1, #14
.L_080fa088:
	movs	r2, #14
	bl	sub_080f8ce8
	b.n	.L_080fa09a
.L_080fa090:
	ldr	r0, [pc, #560]
	movs	r1, #15
	movs	r2, #14
	bl	sub_080f8ce8
.L_080fa09a:
	mov	r0, r8
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
.L_080fa0a6:
	bl	sub_080c85d0
	movs	r2, #0
	mov	r8, r2
	b.n	.L_080fa280
	movs	r7, #128
	lsls	r7, r7, #2
	movs	r3, #180
	adds	r7, #22
	lsls	r3, r3, #1
	add	r3, r9
	add	r7, r9
	ldrh	r1, [r3, #0]
	ldrb	r0, [r7, #0]
	mov	sl, r3
	bl	sub_080ad048
	movs	r5, #1
	movs	r1, #1
	adds	r6, r0, #0
	negs	r5, r5
	mov	r8, r1
	cmp	r6, r5
	bne.n	.L_080fa0d8
	b.n	.L_080fa280
.L_080fa0d8:
	movs	r2, #2
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_080fa0f8
	mov	r3, r9
	ldr	r0, [r3, #48]
	bl	sub_08038268
	movs	r1, #0
	ldr	r0, [pc, #476]
	adds	r2, r5, #0
	bl	sub_080f8ce8
	movs	r1, #1
	mov	r8, r1
	b.n	.L_080fa280
.L_080fa0f8:
	ldrb	r0, [r7, #0]
	bl	sub_080ad288
	ldrb	r0, [r7, #0]
	bl	sub_080ad008
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r5, #226
	movs	r3, #13
	lsls	r5, r5, #1
	strb	r3, [r2, #5]
	add	r5, r9
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080fad88
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080fadd0
	movs	r0, #1
	bl	sub_08013560
	ldrb	r3, [r7, #0]
	mov	r2, sl
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080fae8c
	mov	r3, r9
	ldr	r0, [r3, #48]
	bl	sub_08038268
	ldr	r5, [pc, #368]
	movs	r2, #8
	adds	r0, r5, #0
	movs	r1, #15
	bl	sub_080f8ce8
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fa170
	b.n	.L_080fa276
.L_080fa170:
	movs	r0, #103
	bl	sub_081c0010
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08038268
	adds	r0, r5, #7
	movs	r1, #14
	movs	r2, #8
	bl	sub_080f8ce8
	b.n	.L_080fa276
	movs	r6, #128
	lsls	r6, r6, #2
	adds	r6, #22
	add	r6, r9
	ldrb	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	mov	r8, r3
	lsls	r2, r2, #1
	adds	r2, #216
	ldrh	r1, [r0, r2]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r0, r2]
	ldrb	r0, [r6, #0]
	bl	sub_080ad288
	ldrb	r0, [r6, #0]
	bl	sub_080ad008
	mov	r1, r9
	ldr	r2, [r1, #20]
	movs	r3, #0
	movs	r5, #226
	mov	sl, r3
	lsls	r5, r5, #1
	movs	r3, #13
	strb	r3, [r2, #5]
	add	r5, r9
	ldrb	r0, [r6, #0]
	bl	sub_08016ca4
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080fad88
	movs	r3, #133
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	movs	r5, #152
	bl	sub_080fadd0
	lsls	r5, r5, #2
	movs	r0, #1
	bl	sub_08013560
	add	r5, r9
	movs	r3, #1
	strb	r3, [r5, #0]
	mov	r2, r8
	ldrb	r3, [r6, #0]
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080fae8c
	mov	r3, sl
	strb	r3, [r5, #0]
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08038268
	movs	r2, #8
	ldr	r0, [pc, #176]
	movs	r1, #14
	bl	sub_080f8ce8
	bl	sub_080c85d0
	b.n	.L_080fa276
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	sub_080fb780
	mov	r1, r9
	ldr	r0, [r1, #40]
	bl	sub_08038260
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, r9
	ldrb	r3, [r3, #0]
	movs	r2, #180
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080fae8c
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r1, #9
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r8, r1
	b.n	.L_080fa280
	movs	r2, #0
	movs	r0, #0
	movs	r1, #30
	bl	sub_080fbe6c
.L_080fa276:
	movs	r2, #1
	mov	r8, r2
	b.n	.L_080fa280
.L_080fa27c:
	movs	r3, #1
	str	r3, [sp, #24]
.L_080fa280:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	bne.n	.L_080fa296
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fa296
	bl	.L_080f9672
.L_080fa296:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fa2a8
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #16]
.L_080fa2a8:
	ldr	r0, [sp, #16]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
