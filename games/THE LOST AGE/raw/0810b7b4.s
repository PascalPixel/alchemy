.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_08014128, 0x08014128
	.set sub_080145a8, 0x080145a8
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038010, 0x08038010
	.set sub_08038018, 0x08038018
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038060, 0x08038060
	.set sub_08038080, 0x08038080
	.set sub_080380b0, 0x080380b0
	.set sub_080380c8, 0x080380c8
	.set sub_08038140, 0x08038140
	.set sub_08038260, 0x08038260
	.set sub_080ad1f0, 0x080ad1f0
	.set sub_080ad268, 0x080ad268
	.set sub_080ad270, 0x080ad270
	.set sub_080f8038, 0x080f8038
	.set sub_080f8058, 0x080f8058
	.set sub_080f8060, 0x080f8060
	.set sub_080f8068, 0x080f8068
	.set sub_08108148, 0x08108148
	.set sub_0810824c, 0x0810824c
	.set sub_08108690, 0x08108690
	.set sub_08108aa8, 0x08108aa8
	.set sub_08108af0, 0x08108af0
	.set sub_08109270, 0x08109270
	.set sub_0810928c, 0x0810928c
	.set sub_08109a98, 0x08109a98
	.set sub_0810a004, 0x0810a004
	.set sub_081c0010, 0x081c0010
	.global Overlay_0810b7b4
Overlay_0810b7b4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	movs	r0, #0
	movs	r1, #1
	str	r0, [sp, #12]
	adds	r5, r2, #0
	mov	r8, r1
	mov	r9, r0
	bl	sub_08108148
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #11
	adds	r3, r7, r2
	strb	r5, [r3, #0]
	movs	r1, #12
	movs	r5, #2
	movs	r2, #14
	movs	r3, #8
	movs	r0, #16
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r1, #14
	str	r0, [r7, #36]
	movs	r2, #13
	movs	r3, #3
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_08038010
	mov	sl, r0
	movs	r0, #192
	lsls	r0, r0, #4
	adds	r0, #232
	adds	r3, r7, r0
	mov	r1, sl
	str	r1, [r3, #0]
	movs	r2, #1
	movs	r1, #0
	movs	r3, #1
	movs	r0, #30
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #236
	adds	r3, r7, r2
	mov	fp, r0
	movs	r1, #128
	movs	r6, #0
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #23
	mov	r2, fp
	movs	r3, #0
	str	r6, [sp, #0]
	bl	sub_080380c8
	movs	r3, #4
	adds	r5, r0, #0
	strb	r3, [r5, #5]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #220
	strb	r6, [r5, #4]
	movs	r1, #32
	adds	r6, r7, r3
	negs	r1, r1
	adds	r0, r6, #0
	movs	r2, #112
	bl	sub_08108aa8
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #5
	adds	r3, r7, r0
	mov	r1, r8
	str	r5, [r6, #0]
	movs	r2, #0
	strb	r1, [r3, #0]
	mov	r0, sl
	movs	r1, #2
	bl	sub_080f8060
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	adds	r5, r7, r2
.L_0810b87e:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0810b8e8
	ldr	r4, [sp, #12]
	movs	r0, #153
	lsls	r0, r0, #3
	lsls	r2, r4, #1
	adds	r3, r7, #2
	adds	r2, r2, r0
	ldrsh	r1, [r3, r2]
	adds	r3, r4, #0
	mov	r9, r1
	cmp	r4, #0
	bge.n	.L_0810b89c
	adds	r3, r4, #3
.L_0810b89c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #3
	subs	r1, #12
	mov	r0, sl
	movs	r2, #0
	bl	sub_08108af0
	movs	r3, #3
	mov	r1, r8
	strb	r3, [r5, #0]
	cmp	r1, #2
	bne.n	.L_0810b8d2
	ldr	r0, [sp, #12]
	cmp	r0, #0
	bge.n	.L_0810b8c4
	adds	r0, #3
.L_0810b8c4:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_080f8058
	movs	r0, #1
	bl	sub_08013560
.L_0810b8d2:
	ldr	r1, [sp, #12]
	movs	r2, #0
	mov	r0, sl
	bl	sub_0810928c
	ldr	r0, [r7, #36]
	mov	r1, r9
	bl	sub_0810a004
	movs	r2, #0
	mov	r8, r2
.L_0810b8e8:
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #204]
	movs	r3, #1
	ldr	r2, [r1, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0810b94c
	mov	r0, r9
	bl	sub_080ad1f0
	cmp	r0, #0
	bne.n	.L_0810b90c
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_0810b87e
.L_0810b90c:
	mov	r0, sl
	bl	sub_08038060
	movs	r0, #112
	bl	sub_081c0010
	mov	r0, r9
	bl	.L_0810b9c0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0810b93e
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #220
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r1, #2
	movs	r3, #12
	strb	r3, [r5, #0]
	mov	r8, r1
	b.n	.L_0810b87e
.L_0810b93e:
	ldr	r3, [sp, #8]
	mov	r2, r9
	str	r2, [r3, #0]
	ldr	r1, [sp, #4]
	movs	r6, #0
	str	r0, [r1, #0]
	b.n	.L_0810b984
.L_0810b94c:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810b96c
	movs	r0, #113
	bl	sub_081c0010
	ldr	r2, [sp, #8]
	movs	r3, #1
	negs	r3, r3
	str	r3, [r2, #0]
	ldr	r0, [sp, #4]
	adds	r6, r3, #0
	str	r3, [r0, #0]
	b.n	.L_0810b984
.L_0810b96c:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #4
	adds	r3, r7, r1
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	add	r0, sp, #12
	movs	r2, #4
	bl	sub_08108690
	mov	r8, r0
	b.n	.L_0810b87e
.L_0810b984:
	bl	sub_080f8068
	mov	r0, fp
	movs	r1, #2
	bl	sub_08038018
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038018
	movs	r1, #2
	ldr	r0, [r7, #36]
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
	bl	sub_0810824c
	adds	r0, r6, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
.L_0810b9c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #28]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r5, #2
	mov	r9, r3
	bl	sub_08016ca4
	movs	r1, #1
	str	r0, [sp, #16]
	movs	r2, #16
	movs	r3, #4
	mov	fp, r1
	mov	sl, r1
	movs	r0, #14
	movs	r1, #8
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r2, #30
	str	r0, [sp, #20]
	movs	r3, #3
	movs	r0, #0
	movs	r1, #5
	str	r5, [sp, #0]
	bl	sub_08038010
	movs	r3, #128
	str	r0, [sp, #24]
	lsls	r3, r3, #3
	adds	r3, #220
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #18
	strb	r3, [r2, #5]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	add	r2, r9
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r7, #0
.L_0810ba26:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_0810baa0
	movs	r3, #0
	ldr	r0, [sp, #28]
	mov	sl, r3
	bl	sub_080ad1f0
	mov	fp, r0
	mov	r3, fp
	subs	r3, #1
	cmp	r7, r3
	ble.n	.L_0810ba42
	adds	r7, r3, #0
.L_0810ba42:
	ldr	r1, [sp, #16]
	lsls	r3, r7, #1
	adds	r3, #216
	ldrh	r3, [r1, r3]
	ldr	r6, [pc, #60]
	mov	r2, r9
	ands	r6, r3
	ldr	r3, [r2, #36]
	movs	r1, #5
	adds	r0, r7, #0
	str	r3, [sp, #8]
	bl	sub_08002064
	movs	r1, #5
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08002054
	ldr	r3, [sp, #8]
	adds	r2, r0, #0
	lsls	r5, r5, #4
	lsls	r2, r2, #4
	adds	r0, r3, #0
	adds	r2, #8
	adds	r1, r5, #0
	bl	sub_08108af0
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #5
	movs	r3, #3
	add	r2, r9
	strb	r3, [r2, #0]
	ldr	r0, [sp, #20]
	b.n	.L_0810ba8c
	.2byte 0x01ff
	.2byte 0x0000
.L_0810ba8c:
	ldr	r1, [sp, #28]
	adds	r2, r7, #0
	bl	.L_0810bca0
	ldr	r3, [pc, #504]
	ldr	r0, [sp, #24]
	adds	r6, r6, r3
	adds	r1, r6, #0
	bl	sub_08109270
.L_0810baa0:
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [pc, #492]
	movs	r3, #1
	ldr	r2, [r4, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0810bb30
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #11
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0810bad4
	ldr	r0, [sp, #28]
	adds	r1, r7, #0
	bl	sub_080ad268
	b.n	.L_0810badc
.L_0810bad4:
	ldr	r0, [sp, #28]
	adds	r1, r7, #0
	bl	sub_080ad270
.L_0810badc:
	cmp	r0, #0
	bne.n	.L_0810baea
	movs	r0, #112
	bl	sub_081c0010
	adds	r5, r7, #0
	b.n	.L_0810bc6a
.L_0810baea:
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0810bb00
	ldr	r0, [pc, #420]
	movs	r1, #8
	movs	r2, #1
	movs	r3, #0
	bl	sub_08038038
	b.n	.L_0810bb14
.L_0810bb00:
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0810bb14
	ldr	r0, [pc, #400]
	movs	r1, #8
	movs	r2, #1
	movs	r3, #0
	bl	sub_08038038
.L_0810bb14:
	movs	r0, #113
	bl	sub_081c0010
	b.n	.L_0810bb22
.L_0810bb1c:
	movs	r0, #1
	bl	sub_08013560
.L_0810bb22:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0810bb1c
	bl	sub_08038140
	b.n	.L_0810ba26
.L_0810bb30:
	ldr	r6, [r4, #4]
	movs	r2, #2
	ands	r6, r2
	cmp	r6, #0
	beq.n	.L_0810bb46
	movs	r0, #113
	movs	r5, #1
	bl	sub_081c0010
	negs	r5, r5
	b.n	.L_0810bc6a
.L_0810bb46:
	ldr	r3, [r4, #0]
	movs	r1, #4
	ands	r3, r1
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_0810bbce
	ldr	r1, [sp, #16]
	lsls	r3, r7, #1
	adds	r3, #216
	movs	r0, #126
	ldrh	r5, [r1, r3]
	str	r2, [sp, #12]
	str	r4, [sp, #4]
	bl	sub_081c0010
	ldr	r2, [sp, #12]
	movs	r3, #10
	str	r2, [sp, #0]
	movs	r1, #9
	movs	r2, #16
	movs	r0, #0
	bl	sub_08038010
	adds	r1, r5, #0
	mov	sl, r0
	bl	sub_080f8038
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #9
	add	r3, r9
	strb	r6, [r3, #0]
	ldr	r4, [sp, #4]
	mov	r2, r8
	ldr	r3, [r4, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810bba4
	adds	r6, r4, #0
	movs	r5, #4
.L_0810bb96:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_0810bb96
.L_0810bba4:
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #9
	add	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, sl
	movs	r1, #2
	bl	sub_08038018
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #232
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	sub_08038060
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0810bc64
.L_0810bbce:
	ldr	r3, [r4, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810bbf2
	movs	r0, #111
	str	r4, [sp, #4]
	subs	r7, #1
	bl	sub_081c0010
	mov	r1, fp
	adds	r0, r7, r1
	bl	sub_08002064
	ldr	r4, [sp, #4]
	movs	r2, #1
	adds	r7, r0, #0
	mov	sl, r2
.L_0810bbf2:
	ldr	r3, [r4, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810bc18
	movs	r0, #111
	str	r4, [sp, #4]
	adds	r7, #1
	bl	sub_081c0010
	mov	r3, fp
	adds	r0, r7, r3
	mov	r1, fp
	bl	sub_08002064
	ldr	r4, [sp, #4]
	movs	r1, #1
	adds	r7, r0, #0
	mov	sl, r1
.L_0810bc18:
	ldr	r3, [r4, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810bc3e
	subs	r7, #5
	cmp	r7, #0
	bge.n	.L_0810bc2a
	adds	r7, #15
.L_0810bc2a:
	cmp	r7, fp
	blt.n	.L_0810bc34
.L_0810bc2e:
	subs	r7, #5
	cmp	r7, fp
	bge.n	.L_0810bc2e
.L_0810bc34:
	movs	r0, #111
	bl	sub_081c0010
	movs	r2, #1
	mov	sl, r2
.L_0810bc3e:
	ldr	r3, [pc, #84]
	movs	r2, #128
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0810bc4c
	b.n	.L_0810ba26
.L_0810bc4c:
	adds	r7, #5
	cmp	r7, fp
	blt.n	.L_0810bc54
	subs	r7, #15
.L_0810bc54:
	cmp	r7, #0
	bge.n	.L_0810bc5e
.L_0810bc58:
	adds	r7, #5
	cmp	r7, #0
	blt.n	.L_0810bc58
.L_0810bc5e:
	movs	r0, #111
	bl	sub_081c0010
.L_0810bc64:
	movs	r3, #1
	mov	sl, r3
	b.n	.L_0810ba26
.L_0810bc6a:
	ldr	r0, [sp, #24]
	movs	r1, #2
	bl	sub_08038018
	movs	r1, #2
	ldr	r0, [sp, #20]
	bl	sub_08038018
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r5, #0
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000092
	.4byte 0x03001150
	.4byte 0x00001244
	.2byte 0x1243
	.2byte 0x0000
.L_0810bca0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r6, r1, #0
	adds	r7, r0, #0
	adds	r0, r6, #0
	mov	r8, r2
	sub	sp, #8
	mov	fp, r3
	bl	sub_08016ca4
	mov	r1, r8
	lsls	r1, r1, #1
	mov	r9, r1
	mov	r2, r9
	adds	r2, #216
	ldrh	r3, [r0, r2]
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r3
	ldrh	r3, [r0, r2]
	mov	sl, r0
	lsrs	r3, r3, #11
	adds	r3, #1
	str	r3, [sp, #4]
	cmp	r7, #0
	beq.n	.L_0810bd92
	adds	r0, r7, #0
	bl	sub_08038260
	ldr	r0, [pc, #176]
	movs	r3, #0
	adds	r0, r5, r0
	adds	r1, r7, #0
	movs	r2, #0
	bl	sub_08038080
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #11
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0810bd18
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_080ad268
	b.n	.L_0810bd20
.L_0810bd18:
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_080ad270
.L_0810bd20:
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0810bd2c
	ldr	r0, [pc, #120]
	b.n	.L_0810bd36
.L_0810bd2c:
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0810bd42
	ldr	r0, [pc, #112]
.L_0810bd36:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
	b.n	.L_0810bd92
.L_0810bd42:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #11
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0810bd92
	mov	r3, r9
	adds	r3, #216
	mov	r2, sl
	ldrh	r0, [r2, r3]
	bl	sub_08109a98
	ldr	r5, [pc, #72]
	ldr	r3, [sp, #4]
	adds	r1, r7, #0
	adds	r6, r3, #0
	muls	r6, r0
	movs	r2, #8
	adds	r0, r5, #0
	movs	r3, #8
	bl	sub_08038080
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #5
	adds	r2, r7, #0
	movs	r3, #40
	subs	r5, #5
	bl	sub_080380b0
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #80
	movs	r3, #8
	bl	sub_08038080
.L_0810bd92:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0000025f
	.4byte 0x00001241
	.4byte 0x00001242
	.2byte 0x123a
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r2, [r3, #0]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #230
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0810bdf2
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #20
	subs	r1, #30
	adds	r6, r2, r3
	adds	r5, r2, r1
	movs	r7, #14
.L_0810bdda:
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r3, #96
	beq.n	.L_0810bdea
	adds	r0, r6, #0
	movs	r1, #240
	bl	sub_08014128
.L_0810bdea:
	subs	r7, #1
	adds	r6, #12
	cmp	r7, #0
	bge.n	.L_0810bdda
.L_0810bdf2:
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r4, #192
	lsls	r4, r4, #4
	ldr	r0, [pc, #44]
	adds	r4, #228
	movs	r1, #14
	adds	r2, r3, r4
.L_0810be0a:
	subs	r1, #1
	strh	r0, [r2, #0]
	subs	r2, #2
	cmp	r1, #0
	bge.n	.L_0810be0a
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #230
	adds	r3, r3, r1
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #3
	strb	r2, [r3, #0]
	adds	r1, #138
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	b.n	.L_0810be38
	movs	r0, r0
	.4byte 0x00000060
	.2byte 0xbdb1
	.2byte 0x0810
.L_0810be38:
	pop	{pc}
