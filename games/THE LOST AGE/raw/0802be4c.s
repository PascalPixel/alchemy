.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08014878, 0x08014878
	.set sub_08014d78, 0x08014d78
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08015768, 0x08015768
	.set sub_0801587c, 0x0801587c
	.set sub_0801591c, 0x0801591c
	.set sub_0802b878, 0x0802b878
	.set sub_0802c446, 0x0802c446
	.set sub_0802c4a6, 0x0802c4a6
	.set sub_0802cc9c, 0x0802cc9c
	.set sub_0802d088, 0x0802d088
	.set sub_0802dd70, 0x0802dd70
	.set sub_080c8950, 0x080c8950
	.set sub_080c8970, 0x080c8970
	.global Overlay_0802be4c
Overlay_0802be4c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	sub	sp, #4
	adds	r6, r1, #0
	bl	sub_08014d78
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r7, r0, #0
	mov	r8, r3
	cmp	r5, #0
	bge.n	.L_0802be72
	ldr	r0, [pc, #448]
	adds	r5, r5, r0
.L_0802be72:
	asrs	r3, r5, #21
	movs	r2, #31
	adds	r1, r6, #0
	ands	r3, r2
	cmp	r1, #0
	bge.n	.L_0802be82
	ldr	r0, [pc, #432]
	adds	r1, r1, r0
.L_0802be82:
	asrs	r5, r1, #21
	ands	r5, r2
	lsls	r5, r5, #5
	ldr	r2, [pc, #424]
	adds	r5, r3, r5
	lsls	r5, r5, #2
	adds	r5, r5, r2
	ldr	r0, [r5, #0]
	movs	r1, #0
	lsls	r0, r0, #2
	lsrs	r0, r0, #26
	bl	sub_0802d088
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r3, r3, #2
	lsrs	r0, r3, #26
	bl	sub_08002054
	movs	r3, #3
	cmp	r0, #3
	beq.n	.L_0802bec2
	cmp	r0, #3
	bgt.n	.L_0802beba
	movs	r3, #2
	cmp	r0, #2
	beq.n	.L_0802bec2
	b.n	.L_0802bec0
.L_0802beba:
	movs	r3, #1
	cmp	r0, #4
	beq.n	.L_0802bec2
.L_0802bec0:
	movs	r3, #0
.L_0802bec2:
	lsls	r6, r3, #3
	adds	r6, r6, r3
	ldr	r3, [pc, #368]
	lsls	r6, r6, #2
	adds	r6, r6, r3
	movs	r3, #144
	lsls	r3, r3, #1
	movs	r5, #160
	add	r3, r8
	lsls	r5, r5, #19
	str	r6, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	ldr	r0, [r6, #0]
	mov	r8, r3
	bl	sub_08013300
	adds	r1, r7, #0
	bl	sub_0801591c
	mov	r2, r8
	strh	r2, [r7, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #312]
	adds	r0, r7, #0
	bl	sub_080c8970
	ldr	r0, [r6, #4]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	adds	r0, r5, #0
	bl	sub_0802cc9c
	ldr	r5, [pc, #288]
	ldr	r0, [r6, #8]
	bl	sub_08013300
	ldr	r1, [pc, #284]
	bl	sub_0801587c
	ldr	r0, [r6, #12]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #264]
	ldr	r2, [pc, #264]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #264]
	ldr	r0, [r6, #16]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #244]
	ldr	r2, [pc, #236]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #240]
	ldr	r0, [r6, #20]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #224]
	ldr	r2, [pc, #204]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #220]
	ldr	r0, [r6, #24]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #200]
	ldr	r2, [pc, #176]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [r6, #28]
	bl	sub_08013300
	ldr	r1, [pc, #192]
	bl	sub_0801587c
	ldr	r0, [r6, #32]
	bl	sub_08013300
	ldr	r1, [pc, #184]
	bl	sub_0801587c
	ldr	r3, [pc, #180]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r1, [pc, #172]
	ldr	r2, [pc, #176]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r4, #128
	ldr	r0, [pc, #172]
	ldr	r2, [pc, #172]
	lsls	r4, r4, #10
	movs	r1, #0
	adds	r4, #2
.L_0802bfd8:
	movs	r3, #15
.L_0802bfda:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_0802bfda
	adds	r1, #1
	cmp	r1, #19
	ble.n	.L_0802bfd8
	movs	r4, #128
	ldr	r2, [pc, #148]
	lsls	r4, r4, #10
	movs	r1, #0
	adds	r4, #2
.L_0802bff4:
	movs	r3, #15
.L_0802bff6:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_0802bff6
	adds	r1, #1
	cmp	r1, #23
	ble.n	.L_0802bff4
	movs	r4, #128
	ldr	r2, [pc, #120]
	lsls	r4, r4, #10
	movs	r1, #0
	adds	r4, #2
.L_0802c010:
	movs	r3, #15
.L_0802c012:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L_0802c012
	adds	r1, #1
	cmp	r1, #3
	ble.n	.L_0802c010
	adds	r0, r7, #0
	bl	sub_08013164
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x0802ed34
	.4byte 0x0202d000
	.4byte 0x02038000
	.4byte 0x0202c000
	.4byte 0x06008000
	.4byte 0x84000800
	.4byte 0x0203a000
	.4byte 0x0600a000
	.4byte 0x0203c000
	.4byte 0x0600c000
	.4byte 0x0203e000
	.4byte 0x0600e000
	.4byte 0x02028000
	.4byte 0x0202a000
	.4byte 0xf07ff07f
	.4byte 0x06002800
	.4byte 0x85000180
	.4byte 0x06003000
	.4byte 0x01810180
	.2byte 0x0100
	.2byte 0x0101
	push	{r5, r6, lr}
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r3, [r1, #28]
	movs	r2, #200
	lsls	r2, r2, #4
	adds	r6, r3, r2
	movs	r3, #128
	lsls	r3, r3, #19
	ldr	r1, [r1, #32]
	ldrh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	mov	ip, r1
	movs	r1, #128
	ands	r3, r2
	lsls	r1, r1, #19
	adds	r1, #176
	lsls	r3, r3, #16
	ldrh	r2, [r1, #10]
	asrs	r5, r3, #16
	movs	r3, #197
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #10]
	movs	r3, #254
	ldrh	r2, [r1, #10]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #10]
	movs	r4, #128
	lsls	r4, r4, #19
	adds	r4, #32
	ldrh	r3, [r1, #10]
	cmp	r6, #0
	beq.n	.L_0802c112
	ldr	r3, [pc, #148]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #10
	adds	r0, r6, r0
	ldmia	r0!, {r3}
	ldr	r2, [pc, #132]
	str	r3, [r4, #0]
	adds	r4, #4
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	stmia	r4!, {r3}
	ldmia	r0!, {r3}
	str	r3, [r4, #0]
	adds	r3, r1, #0
	subs	r1, #144
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0802c112:
	movs	r3, #130
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	movs	r2, #132
	lsls	r2, r2, #1
	add	r2, ip
	strh	r3, [r2, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r0, [r3, #0]
	movs	r3, #133
	lsls	r3, r3, #1
	add	r3, ip
	strh	r0, [r3, #0]
	movs	r3, #0
	ldrh	r1, [r2, #0]
	cmp	r1, #199
	bhi.n	.L_0802c152
	lsls	r2, r0, #16
	lsrs	r2, r2, #16
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	lsls	r3, r3, #1
	cmp	r1, r2
	bhi.n	.L_0802c152
	movs	r3, #0
	cmp	r1, #0
	bne.n	.L_0802c152
	movs	r3, #2
.L_0802c152:
	orrs	r5, r3
	lsls	r3, r5, #16
	movs	r2, #128
	lsrs	r3, r3, #16
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r2, #134
	lsls	r2, r2, #1
	add	r2, ip
	movs	r3, #0
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0300122c
	.2byte 0x0008
	.2byte 0xa660
.L_0802c174:
	push	{r5, lr}
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	movs	r2, #31
	asrs	r3, r3, #1
	ands	r3, r2
	ldr	r2, [pc, #100]
	lsls	r3, r3, #7
	adds	r4, r3, r2
	ldr	r5, [pc, #100]
	movs	r3, #62
	ands	r3, r0
	lsls	r3, r3, #6
	adds	r1, r3, r5
	movs	r0, #0
.L_0802c192:
	ldrh	r2, [r4, #0]
	ldr	r5, [pc, #88]
	lsls	r2, r2, #2
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r5, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	adds	r4, #4
	cmp	r0, #31
	bls.n	.L_0802c192
	movs	r2, #252
	movs	r3, #248
	lsls	r2, r2, #4
	lsls	r3, r3, #4
	adds	r1, r1, r2
	adds	r4, r4, r3
	movs	r0, #0
.L_0802c1c2:
	ldrh	r2, [r4, #0]
	ldr	r5, [pc, #40]
	lsls	r2, r2, #2
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r5, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	adds	r4, #4
	cmp	r0, #31
	bls.n	.L_0802c1c2
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x02020000
	.4byte 0x06004000
	.2byte 0x0000
	.2byte 0x0201
.L_0802c1f4:
	push	{r5, lr}
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	movs	r2, #31
	asrs	r3, r3, #1
	ands	r3, r2
	ldr	r2, [pc, #48]
	lsls	r3, r3, #2
	ldr	r5, [pc, #48]
	adds	r4, r3, r2
	movs	r3, #62
	ands	r3, r0
	adds	r1, r3, r5
	movs	r0, #0
.L_0802c210:
	ldrh	r2, [r4, #0]
	ldr	r5, [pc, #40]
	lsls	r2, r2, #2
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r5, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #128
	adds	r4, #128
	cmp	r0, #63
	bls.n	.L_0802c210
	pop	{r5, pc}
	.4byte 0x02020000
	.4byte 0x06004000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #48]
	sub	sp, #48
	str	r1, [sp, #32]
	adds	r1, #12
	ldr	r2, [r3, #28]
	mov	r9, r1
	str	r2, [sp, #28]
	movs	r1, #144
	ldr	r7, [r3, #32]
	movs	r3, #200
	lsls	r3, r3, #4
	adds	r3, r2, r3
	ldr	r5, [r7, #0]
	lsls	r1, r1, #4
	str	r3, [sp, #24]
	adds	r1, #92
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r2, #150
	str	r3, [sp, #20]
	lsls	r2, r2, #4
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	adds	r1, #22
	str	r3, [sp, #16]
	movs	r3, #0
	str	r3, [sp, #8]
	str	r3, [sp, #4]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0802c2a4
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne.n	.L_0802c2a8
	ldr	r3, [r7, #8]
	cmp	r3, #0
	bne.n	.L_0802c2a8
	b.n	sub_0802c4a6
.L_0802c2a4:
	bl	.L_0802c58c
.L_0802c2a8:
	cmp	r5, #0
	beq.n	.L_0802c38c
	ldr	r2, [r5, #0]
	ldr	r3, [r7, #4]
	ldr	r5, [r5, #8]
	mov	r8, r2
	mov	sl, r5
	cmp	r3, #0
	beq.n	.L_0802c2de
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	subs	r5, r5, r0
	ldr	r6, [pc, #492]
	adds	r1, r5, #0
	ldr	r0, [r7, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9002
	add	r8, r0
	ldr	r1, [r7, #12]
	ldr	r0, [r7, #4]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6078
.L_0802c2de:
	ldr	r3, [r7, #8]
	cmp	r3, #0
	beq.n	.L_0802c308
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	subs	r5, r5, r0
	ldr	r6, [pc, #448]
	adds	r1, r5, #0
	ldr	r0, [r7, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9001
	add	sl, r0
	ldr	r1, [r7, #12]
	ldr	r0, [r7, #8]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x60b8
.L_0802c308:
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_0802c312
	ldr	r0, [pc, #424]
	add	r0, r8
.L_0802c312:
	asrs	r4, r0, #20
	mov	r0, sl
	cmp	r0, #0
	bge.n	.L_0802c31e
	ldr	r0, [pc, #412]
	add	r0, sl
.L_0802c31e:
	adds	r6, r7, #0
	adds	r6, #228
	asrs	r0, r0, #20
	str	r0, [sp, #12]
	str	r6, [sp, #0]
	mov	r2, r8
	ldr	r1, [r6, #0]
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802c350
	cmp	r1, r8
	bge.n	.L_0802c348
	adds	r0, r4, #0
	adds	r0, #16
	bl	.L_0802c1f4
	b.n	.L_0802c350
.L_0802c348:
	adds	r0, r4, #0
	subs	r0, #16
	bl	.L_0802c1f4
.L_0802c350:
	adds	r5, r7, #0
	adds	r5, #232
	ldr	r1, [r5, #0]
	mov	r2, sl
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	ands	r3, r2
	mov	fp, r5
	cmp	r3, #0
	beq.n	.L_0802c37e
	cmp	r1, sl
	bge.n	.L_0802c376
	ldr	r0, [sp, #12]
	adds	r0, #12
	bl	.L_0802c174
	b.n	.L_0802c37e
.L_0802c376:
	ldr	r0, [sp, #12]
	subs	r0, #18
	bl	.L_0802c174
.L_0802c37e:
	ldr	r1, [sp, #0]
	mov	r3, r8
	str	r3, [r1, #0]
	mov	r2, sl
	mov	r3, fp
	str	r2, [r3, #0]
	b.n	.L_0802c394
.L_0802c38c:
	adds	r6, r7, #0
	adds	r5, r7, #0
	adds	r6, #228
	adds	r5, #232
.L_0802c394:
	ldr	r3, [r6, #0]
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r3, #0
	str	r3, [r1, #4]
	movs	r2, #144
	ldr	r3, [r5, #0]
	lsls	r2, r2, #4
	str	r3, [r1, #8]
	adds	r2, #113
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #0
	beq.n	.L_0802c3de
	ldr	r0, [sp, #8]
	ldr	r2, [pc, #256]
	cmp	r0, #0
	bge.n	.L_0802c3c6
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_0802c3c6:
	asrs	r3, r0, #16
	strh	r3, [r2, #4]
	ldr	r0, [sp, #4]
	cmp	r0, #0
	bge.n	.L_0802c3d8
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_0802c3d8:
	asrs	r3, r0, #16
	strh	r3, [r2, #6]
	b.n	sub_0802c4a6
.L_0802c3de:
	ldr	r2, [pc, #224]
	movs	r3, #120
	str	r3, [r2, #12]
	movs	r3, #96
	str	r3, [r2, #16]
	ldr	r2, [sp, #16]
	ldr	r0, [sp, #20]
	lsrs	r1, r2, #31
	adds	r1, r2, r1
	asrs	r1, r1, #1
	lsls	r2, r2, #1
	bl	sub_08015768
	bl	sub_08014de4
	mov	r0, r9
	bl	sub_08015128
	movs	r1, #143
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r0, [r3, #0]
	bl	sub_08015068
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r6, r7, r2
	ldrh	r0, [r6, #0]
	bl	sub_08015024
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #118
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	sub_0802c446
	add	r0, sp, #36
	mov	r2, r8
	str	r2, [r0, #0]
	str	r2, [r0, #4]
	ldr	r1, [sp, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r1, r2
	str	r3, [r0, #8]
	ldr	r1, [sp, #32]
	ldr	r3, [pc, #128]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7e8
	.2byte 0xfccd
	.2byte 0x4649
	ldr	r0, [sp, #32]
	bl	sub_080156e8
	ldr	r7, [pc, #116]
	ldrh	r0, [r6, #0]
	ldr	r3, [r7, #0]
	cmp	r3, r0
	beq.n	.L_0802c484
	bl	sub_08002090
	adds	r5, r0, #0
	ldrh	r0, [r6, #0]
	bl	sub_08002096
	ldr	r3, [pc, #96]
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4649
	ldr	r2, [sp, #28]
	bl	sub_0802dd70
	ldr	r3, [pc, #84]
	mov	r1, r8
	str	r1, [r3, #0]
	ldrh	r3, [r6, #0]
	str	r3, [r7, #0]
.L_0802c484:
	ldr	r3, [pc, #76]
	movs	r1, #192
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #24]
	lsls	r1, r1, #18
	lsls	r3, r3, #10
	adds	r3, r2, r3
	ldr	r4, [r1, #104]
	ldr	r0, [sp, #32]
	mov	r1, r9
	ldr	r2, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xb00c
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x000fffff
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x03000354
	.4byte 0x03001144
	.4byte 0x03000230
	.4byte 0x03001244
	.2byte 0x122c
	.2byte 0x0300
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
	sub	sp, #16
	movs	r1, #0
	str	r3, [sp, #12]
	str	r1, [sp, #8]
	str	r1, [sp, #4]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802c504
	ldmia	r3!, {r2}
	str	r2, [sp, #8]
	ldr	r3, [r3, #4]
	str	r3, [sp, #4]
.L_0802c504:
	ldr	r1, [sp, #8]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r3, r1, r2
	ldr	r1, [sp, #4]
	ldr	r2, [pc, #120]
	asrs	r3, r3, #25
	str	r3, [sp, #8]
	adds	r3, r1, r2
	asrs	r3, r3, #25
	str	r3, [sp, #4]
	movs	r3, #0
	mov	sl, r3
.L_0802c51e:
	mov	r2, sl
	ldr	r6, [sp, #4]
	movs	r1, #0
	lsls	r2, r2, #10
	mov	r9, r1
	mov	fp, r2
.L_0802c52a:
	adds	r3, r6, #0
	movs	r1, #31
	ands	r3, r1
	movs	r5, #0
	mov	r8, r6
	lsls	r7, r3, #5
.L_0802c536:
	ldr	r2, [sp, #8]
	mov	r0, sl
	adds	r1, r2, r5
	adds	r3, r1, #0
	movs	r2, #31
	ands	r3, r2
	adds	r4, r7, r3
	movs	r2, #164
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	adds	r5, #1
	ldrh	r4, [r2, r3]
	movs	r3, #1
	add	r4, fp
	str	r3, [sp, #0]
	mov	r2, r8
	adds	r3, r4, #0
	bl	sub_0802b878
	cmp	r5, #1
	bls.n	.L_0802c536
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	adds	r6, #1
	cmp	r1, #1
	bls.n	.L_0802c52a
	add	sl, r3
	mov	r2, sl
	cmp	r2, #1
	bls.n	.L_0802c51e
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xfec0
.L_0802c58c:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #24
	movs	r0, #0
	str	r3, [sp, #20]
	str	r0, [sp, #16]
	str	r0, [sp, #12]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802c5b8
	ldmia	r3!, {r1}
	str	r1, [sp, #16]
	ldr	r3, [r3, #4]
	str	r3, [sp, #12]
.L_0802c5b8:
	ldr	r2, [sp, #16]
	movs	r0, #255
	lsls	r0, r0, #24
	adds	r3, r2, r0
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #600]
	asrs	r3, r3, #25
	str	r3, [sp, #8]
	adds	r3, r1, r2
	asrs	r3, r3, #25
	str	r3, [sp, #4]
	movs	r3, #0
	mov	sl, r3
.L_0802c5d2:
	mov	r1, sl
	ldr	r5, [sp, #4]
	movs	r0, #0
	lsls	r1, r1, #10
	mov	r9, r0
	mov	fp, r1
.L_0802c5de:
	adds	r3, r5, #0
	movs	r2, #31
	ands	r3, r2
	movs	r6, #0
	mov	r8, r5
	lsls	r7, r3, #5
.L_0802c5ea:
	ldr	r3, [sp, #8]
	movs	r0, #31
	adds	r1, r3, r6
	adds	r3, r1, #0
	ands	r3, r0
	adds	r4, r7, r3
	ldr	r0, [sp, #20]
	movs	r2, #164
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r4, [r0, r3]
	movs	r3, #0
	add	r4, fp
	str	r3, [sp, #0]
	mov	r0, sl
	mov	r2, r8
	adds	r3, r4, #0
	bl	sub_0802b878
	cmp	r0, #0
	beq.n	.L_0802c618
	b.n	.L_0802c80c
.L_0802c618:
	adds	r6, #1
	cmp	r6, #1
	ble.n	.L_0802c5ea
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #1
	cmp	r2, #1
	ble.n	.L_0802c5de
	add	sl, r1
	mov	r3, sl
	cmp	r3, #1
	ble.n	.L_0802c5d2
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bge.n	.L_0802c63c
	ldr	r0, [pc, #484]
	adds	r3, r3, r0
.L_0802c63c:
	asrs	r2, r3, #21
	ldr	r3, [sp, #12]
	movs	r1, #31
	ands	r2, r1
	cmp	r3, #0
	bge.n	.L_0802c64c
	ldr	r0, [pc, #468]
	adds	r3, r3, r0
.L_0802c64c:
	asrs	r5, r3, #21
	ands	r5, r1
	lsls	r5, r5, #5
	ldr	r1, [pc, #464]
	adds	r5, r2, r5
	lsls	r5, r5, #2
	adds	r5, r5, r1
	ldr	r0, [r5, #0]
	movs	r1, #1
	lsls	r0, r0, #2
	lsrs	r0, r0, #26
	bl	sub_0802d088
	ldr	r3, [r5, #0]
	movs	r1, #10
	lsls	r3, r3, #2
	lsrs	r0, r3, #26
	bl	sub_08002054
	movs	r2, #3
	cmp	r0, #3
	beq.n	.L_0802c68c
	cmp	r0, #3
	bgt.n	.L_0802c684
	movs	r2, #2
	cmp	r0, #2
	beq.n	.L_0802c68c
	b.n	.L_0802c68a
.L_0802c684:
	movs	r2, #1
	cmp	r0, #4
	beq.n	.L_0802c68c
.L_0802c68a:
	movs	r2, #0
.L_0802c68c:
	lsls	r3, r2, #3
	adds	r3, r3, r2
	ldr	r2, [pc, #404]
	lsls	r3, r3, #2
	adds	r7, r3, r2
	ldr	r3, [sp, #20]
	movs	r0, #144
	lsls	r0, r0, #1
	adds	r2, r3, r0
	ldr	r3, [r2, #0]
	cmp	r7, r3
	beq.n	.L_0802c6b4
	str	r7, [r2, #0]
	ldr	r1, [sp, #20]
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #88
	adds	r2, r1, r3
	movs	r3, #12
	str	r3, [r2, #0]
.L_0802c6b4:
	bl	sub_080c8950
	movs	r2, #144
	ldr	r1, [sp, #20]
	lsls	r2, r2, #4
	adds	r2, #88
	adds	r5, r1, r2
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0802c6ca
	b.n	.L_0802c80c
.L_0802c6ca:
	cmp	r0, #0
	beq.n	.L_0802c6d0
	b.n	.L_0802c80c
.L_0802c6d0:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014d78
	ldr	r3, [r5, #0]
	mov	r8, r0
	subs	r3, #1
	cmp	r3, #11
	bls.n	.L_0802c6e4
	b.n	.L_0802c7f6
.L_0802c6e4:
	ldr	r2, [pc, #324]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0802c7ea
	.4byte 0x0802c7f6
	.4byte 0x0802c7dc
	.4byte 0x0802c7ca
	.4byte 0x0802c7bc
	.4byte 0x0802c7b0
	.4byte 0x0802c7a2
	.4byte 0x0802c796
	.4byte 0x0802c788
	.4byte 0x0802c77c
	.4byte 0x0802c76e
	.2byte 0xc71c
	.2byte 0x0802
	movs	r5, #160
	lsls	r5, r5, #19
	ldr	r0, [r7, #0]
	movs	r3, #0
	ldrsh	r6, [r5, r3]
	bl	sub_08013300
	mov	r1, r8
	bl	sub_0801591c
	movs	r3, #128
	movs	r2, #132
	mov	r0, r8
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	strh	r6, [r0, #0]
	adds	r3, #212
	adds	r1, r5, #0
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #232]
	mov	r0, r8
	bl	sub_080c8970
	ldr	r0, [r7, #4]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	adds	r0, r5, #0
	bl	sub_0802cc9c
	ldr	r0, [r7, #8]
	bl	sub_08013300
	ldr	r1, [pc, #204]
	bl	sub_0801587c
	b.n	.L_0802c7f6
	ldr	r0, [r7, #12]
	bl	sub_08013300
	ldr	r1, [pc, #192]
	bl	sub_0801587c
	b.n	.L_0802c7f6
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #180]
	ldr	r1, [pc, #180]
	b.n	.L_0802c7d4
	ldr	r0, [r7, #16]
	bl	sub_08013300
	ldr	r1, [pc, #176]
	bl	sub_0801587c
	b.n	.L_0802c7f6
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #160]
	ldr	r1, [pc, #164]
	b.n	.L_0802c7d4
	ldr	r0, [r7, #20]
	bl	sub_08013300
	ldr	r1, [pc, #156]
	bl	sub_0801587c
	b.n	.L_0802c7f6
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #144]
	ldr	r1, [pc, #144]
	b.n	.L_0802c7d4
	ldr	r0, [r7, #24]
	bl	sub_08013300
	ldr	r1, [pc, #140]
	bl	sub_0801587c
	b.n	.L_0802c7f6
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #128]
.L_0802c7d4:
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_0802c7f6
	ldr	r0, [r7, #28]
	bl	sub_08013300
	ldr	r1, [pc, #120]
	bl	sub_0801587c
	b.n	.L_0802c7f6
	ldr	r0, [r7, #32]
	bl	sub_08013300
	ldr	r1, [pc, #108]
	bl	sub_0801587c
.L_0802c7f6:
	ldr	r1, [sp, #20]
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #88
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	mov	r0, r8
	subs	r3, #1
	str	r3, [r2, #0]
	bl	sub_08013164
.L_0802c80c:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfec00000
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x0802ed34
	.4byte 0x0802c6ec
	.4byte 0x0202d000
	.4byte 0x0202c000
	.4byte 0x02038000
	.4byte 0x06008000
	.4byte 0x0203a000
	.4byte 0x0600a000
	.4byte 0x0203c000
	.4byte 0x0600c000
	.4byte 0x0203e000
	.4byte 0x0600e000
	.4byte 0x84000800
	.4byte 0x02028000
	.4byte 0x0202a000
