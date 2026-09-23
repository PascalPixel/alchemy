.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08118028, 0x08118028
	.set sub_08118038, 0x08118038
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434d8, 0x081434d8
	.set sub_0814355c, 0x0814355c
	.global Func_081435e0
	.thumb_func
Func_081435e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #92]
	sub	sp, #4
	mov	sl, r2
	ldr	r2, [r3, #36]
	adds	r7, r0, #0
	mov	r9, r2
	ldr	r2, [r3, #96]
	adds	r3, #176
	str	r2, [sp, #0]
	ldr	r3, [r3, #0]
	mov	fp, r3
	bl	sub_0813ba50
	bl	sub_081434d8
	mov	r2, fp
	movs	r3, #1
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #24]
	movs	r2, #128
	ldr	r6, [pc, #24]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r5, [pc, #20]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_08143666
	b.n	.L_08143644
	.4byte 0x00000000
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_08143644:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #184
	adds	r3, r3, r6
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143666:
	strh	r1, [r5, #0]
	ldr	r2, [pc, #144]
	movs	r3, #0
	mov	r8, r3
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #206
	lsls	r3, r3, #3
	add	r3, r9
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118038
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #180
	mov	r2, r8
	add	r3, sl
	str	r2, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #184
	add	r3, sl
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #92]
	bl	sub_080145a8
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_081436d6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #230
	adds	r3, r3, r6
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_081436d6:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #12
	orrs	r7, r2
	strh	r7, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_08143726
	b.n	.L_08143704
	.4byte 0x00000784
	.4byte 0x03001120
	.2byte 0x3489
	.2byte 0x0814
.L_08143704:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #230
	adds	r3, r3, r6
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143726:
	strh	r1, [r5, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08118028
	mov	r3, r8
	mov	r2, fp
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_08013560
	bl	sub_0814355c
	ldr	r2, [pc, #20]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #72
	strh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_08143782
	b.n	.L_08143760
	.2byte 0x3537
	.2byte 0x0000
.L_08143760:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #238
	adds	r3, r3, r6
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143782:
	strh	r1, [r5, #0]
	movs	r5, #0
	ldr	r3, [pc, #168]
	movs	r2, #128
	mov	r8, r3
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	mov	ip, r5
	movs	r7, #0
	mov	r9, r2
	mov	lr, r3
	movs	r6, #0
.L_0814379c:
	mov	r2, r9
	movs	r4, #0
	adds	r0, r6, r2
	lsls	r1, r7, #1
.L_081437a4:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	fp, r3
	mov	r3, r8
	adds	r2, r5, r3
	adds	r4, #1
	mov	r3, fp
	strh	r3, [r2, #0]
	add	r0, lr
	adds	r1, #2
	adds	r5, #2
	cmp	r4, #8
	bne.n	.L_081437a4
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #5
	add	ip, r3
	adds	r6, r6, r2
	mov	r2, ip
	adds	r7, #8
	cmp	r2, #16
	bne.n	.L_0814379c
	movs	r1, #128
	ldr	r0, [sp, #0]
	ldr	r5, [pc, #88]
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	ldr	r0, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x23ee
	lsls	r3, r3, #7
	adds	r3, #168
	movs	r6, #0
	add	r3, sl
	str	r6, [r3, #0]
	movs	r3, #240
	ldr	r1, [pc, #64]
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, sl
	movs	r2, #238
	str	r6, [r3, #0]
	lsls	r2, r2, #7
	ldrh	r3, [r1, #4]
	adds	r2, #160
	add	r2, sl
	str	r3, [r2, #0]
	movs	r2, #238
	ldrh	r3, [r1, #6]
	lsls	r2, r2, #7
	adds	r2, #164
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x06003800
	.4byte 0x03000258
	.4byte 0x06004000
	.2byte 0x1120
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
	ldr	r2, [r3, #92]
	sub	sp, #4
	mov	r9, r2
	ldr	r2, [r3, #36]
	adds	r7, r0, #0
	mov	r8, r2
	ldr	r2, [r3, #96]
	adds	r3, #176
	str	r2, [sp, #0]
	ldr	r3, [r3, #0]
	mov	sl, r3
	bl	sub_081434d8
	mov	r2, sl
	movs	r3, #1
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #24]
	movs	r2, #128
	ldr	r6, [pc, #24]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r5, [pc, #20]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_081438c2
	b.n	.L_081438a0
	.4byte 0x00000000
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_081438a0:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #184
	adds	r3, r3, r6
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_081438c2:
	strh	r1, [r5, #0]
	ldr	r2, [pc, #148]
	movs	r3, #0
	mov	fp, r3
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #206
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #128
	bl	sub_08118038
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #180
	add	r2, r9
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #184
	add	r3, r9
	mov	r2, fp
	movs	r1, #144
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #92]
	bl	sub_080145a8
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_08143934
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #152
	adds	r3, r3, r6
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143934:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [pc, #24]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #12
	orrs	r7, r2
	strh	r7, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_08143986
	b.n	.L_08143964
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03001120
	.2byte 0x3489
	.2byte 0x0814
.L_08143964:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #152
	adds	r3, r3, r6
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08143986:
	strh	r1, [r5, #0]
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	sub_08118028
	mov	r3, fp
	mov	r2, sl
	str	r3, [r2, #12]
	movs	r0, #1
	bl	sub_08013560
	bl	sub_0814355c
	ldr	r2, [pc, #20]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #72
	strh	r2, [r3, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r6, #0]
	cmp	r2, #31
	bgt.n	.L_081439e2
	b.n	.L_081439c0
	.2byte 0x3537
	.2byte 0x0000
.L_081439c0:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r6, #0]
	movs	r2, #238
	adds	r3, r3, r6
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_081439e2:
	strh	r1, [r5, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #144]
	movs	r2, #128
	mov	r8, r3
	movs	r3, #128
	movs	r5, #0
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	mov	ip, r5
	movs	r7, #0
	mov	sl, r2
	mov	lr, r3
	movs	r6, #0
.L_08143a02:
	mov	r2, sl
	movs	r4, #0
	adds	r0, r6, r2
	lsls	r1, r7, #1
.L_08143a0a:
	adds	r3, r0, #0
	orrs	r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	fp, r3
	mov	r3, r8
	adds	r2, r5, r3
	adds	r4, #1
	mov	r3, fp
	strh	r3, [r2, #0]
	add	r0, lr
	adds	r1, #2
	adds	r5, #2
	cmp	r4, #8
	bne.n	.L_08143a0a
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #5
	add	ip, r3
	adds	r6, r6, r2
	mov	r2, ip
	adds	r7, #8
	cmp	r2, #16
	bne.n	.L_08143a02
	movs	r1, #128
	ldr	r0, [sp, #0]
	ldr	r5, [pc, #64]
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	ldr	r0, [pc, #56]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x23ee
	lsls	r3, r3, #7
	adds	r3, #168
	movs	r6, #0
	add	r3, r9
	str	r6, [r3, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, r9
	str	r6, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x06003800
	.4byte 0x03000258
	.4byte 0x06004000
