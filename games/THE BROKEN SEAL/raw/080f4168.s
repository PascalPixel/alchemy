.syntax unified
	.thumb
	.set sub_08002304, 0x08002304
	.set sub_08002f3c, 0x08002f3c
	.set sub_08002f40, 0x08002f40
	.set sub_080040e8, 0x080040e8
	.set sub_08004458, 0x08004458
	.set sub_080048b0, 0x080048b0
	.set sub_080048f4, 0x080048f4
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_08015000, 0x08015000
	.set sub_08015010, 0x08015010
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_08077230, 0x08077230
	.set sub_080f4028, 0x080f4028
	.set sub_080f4100, 0x080f4100
	.set sub_080f47bc, 0x080f47bc
	.set sub_080f49e6, 0x080f49e6
	.set sub_080f9010, 0x080f9010
	.global Overlay_080f4168
Overlay_080f4168:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #336]
	ldr	r1, [pc, #336]
	movs	r0, #41
	add	sp, r5
	bl	sub_080048b0
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r0, #40
	bl	sub_080048b0
	ldr	r1, [pc, #320]
	str	r0, [sp, #116]
	movs	r0, #39
	bl	sub_080048f4
	movs	r1, #195
	str	r0, [sp, #112]
	lsls	r1, r1, #3
	movs	r0, #45
	bl	sub_080048f4
	movs	r1, #76
	str	r0, [sp, #108]
	movs	r0, #12
	bl	sub_080048f4
	str	r0, [sp, #104]
	ldr	r0, [pc, #292]
	bl	sub_08002f3c
	bl	sub_080f4028
	bl	sub_080040e8
	ldr	r2, [pc, #280]
	movs	r3, #0
	movs	r0, #0
	strb	r3, [r2, #0]
	ldr	r6, [pc, #276]
	mov	fp, r0
	movs	r5, #0
	movs	r4, #0
.L_080f41cc:
	movs	r1, #0
	adds	r0, r4, #0
.L_080f41d0:
	mov	r3, fp
	adds	r2, r3, r6
	movs	r7, #2
	adds	r3, r1, r0
	adds	r1, #1
	strh	r3, [r2, #0]
	add	fp, r7
	cmp	r1, #32
	bne.n	.L_080f41d0
	adds	r5, #1
	adds	r4, #30
	cmp	r5, #20
	bne.n	.L_080f41cc
	ldr	r0, [pc, #244]
	bl	sub_08002f40
	movs	r1, #160
	adds	r4, r0, #0
	ldr	r3, [pc, #236]
	lsls	r1, r1, #19
	ldr	r2, [pc, #236]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #160
	lsls	r0, r0, #19
	add	r1, sp, #256
	ldr	r2, [pc, #228]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r4, r4, r0
	adds	r0, r4, #0
	ldr	r1, [pc, #220]
	bl	sub_08005340
	ldr	r3, [pc, #200]
	ldr	r0, [pc, #212]
	ldr	r1, [pc, #212]
	ldr	r2, [pc, #216]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #212]
	bl	sub_08002f40
	ldr	r3, [pc, #184]
	adds	r4, r0, #0
	ldr	r1, [sp, #116]
	ldr	r2, [pc, #184]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	ldr	r1, [pc, #196]
	ldr	r2, [pc, #176]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r4, r4, r1
	adds	r0, r4, #0
	ldr	r1, [pc, #164]
	bl	sub_08005340
	ldr	r3, [pc, #148]
	ldr	r0, [pc, #156]
	ldr	r1, [pc, #176]
	ldr	r2, [pc, #176]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r3, r5, #0
	ldr	r1, [pc, #156]
	ldr	r0, [sp, #116]
	movs	r2, #0
	bl	sub_080f4100
	movs	r1, #160
	adds	r3, r5, #0
	lsls	r1, r1, #19
	movs	r2, #0
	add	r0, sp, #256
	bl	sub_080f4100
	ldr	r2, [pc, #144]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #62
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #74
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #112]
	movs	r2, #0
	strh	r2, [r3, #4]
	strh	r2, [r3, #6]
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #28]
	strh	r2, [r3, #0]
	b.n	.L_080f4318
	.4byte 0x00000686
	.4byte 0x00003737
	.4byte 0x00002723
	.4byte 0x00003340
	.4byte 0x00003f44
	.4byte 0x00000810
	.4byte 0x0000ff60
	.4byte 0xfffffd00
	.4byte 0x0000060e
	.4byte 0x0000782c
	.4byte 0x0000000c
	.4byte 0x03001d18
	.4byte 0x06003000
	.4byte 0x00000042
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x84000080
	.4byte 0x02010000
	.4byte 0x06004000
	.4byte 0x84002580
	.4byte 0x00000043
	.4byte 0x05000200
	.4byte 0x06010000
	.4byte 0x84001f00
	.4byte 0x0400000a
	.4byte 0x03001ad0
	.2byte 0x0014
	.2byte 0x0400
.L_080f4318:
	adds	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #60]
	adds	r3, #38
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	mov	r4, sp
	movs	r3, #1
	movs	r2, #0
	adds	r4, #152
	negs	r3, r3
	ldr	r0, [sp, #112]
	movs	r1, #225
	lsls	r1, r1, #7
	str	r2, [sp, #96]
	str	r2, [sp, #88]
	str	r3, [sp, #84]
	str	r2, [sp, #80]
	str	r2, [sp, #76]
	str	r4, [sp, #36]
	str	r4, [sp, #20]
	mov	fp, r2
	movs	r6, #127
	movs	r7, #31
	adds	r5, r0, r1
	b.n	.L_080f4368
	.4byte 0x000000f0
	.2byte 0x00a0
	.2byte 0x0000
.L_080f4368:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #8]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #32
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #14
	movs	r2, #0
	str	r0, [r5, #20]
	str	r2, [r5, #24]
	ldr	r0, [sp, #20]
	movs	r3, #150
	lsls	r3, r3, #1
	stmia	r0!, {r3}
	movs	r1, #1
	add	fp, r1
	adds	r4, r0, #0
	mov	r3, fp
	str	r4, [sp, #20]
	adds	r5, #28
	cmp	r3, #2
	bne.n	.L_080f4368
	mov	r4, sp
	add	r3, sp, #224
	adds	r4, #144
	str	r2, [r3, #12]
	str	r2, [r3, #8]
	str	r2, [r3, #4]
	str	r2, [r3, #0]
	str	r4, [sp, #52]
	str	r2, [r4, #4]
	str	r2, [sp, #144]
	bl	sub_08004458
	movs	r1, #6
	bl	sub_08002304
	mov	r5, sp
	adds	r5, #136
	str	r0, [sp, #136]
	str	r5, [sp, #40]
.L_080f43f0:
	bl	sub_08004458
	movs	r1, #6
	bl	sub_08002304
	ldr	r6, [sp, #40]
	str	r0, [r6, #4]
	ldr	r3, [sp, #136]
	cmp	r3, r0
	beq.n	.L_080f43f0
	ldr	r7, [sp, #112]
	movs	r0, #225
	lsls	r0, r0, #7
	movs	r3, #160
	adds	r2, r7, r0
	lsls	r3, r3, #14
	ldr	r1, [pc, #824]
	str	r3, [r2, #0]
	movs	r0, #192
	ldr	r2, [pc, #824]
	adds	r3, r7, r1
	lsls	r0, r0, #15
	ldr	r1, [pc, #820]
	str	r0, [r3, #0]
	adds	r3, r7, r2
	str	r1, [r3, #0]
	ldr	r3, [pc, #816]
	ldr	r4, [pc, #820]
	adds	r2, r7, r3
	ldr	r3, [pc, #820]
	ldr	r5, [pc, #820]
	str	r3, [r2, #0]
	adds	r3, r7, r4
	str	r0, [r3, #0]
	adds	r3, r7, r5
	str	r1, [r3, #0]
	bl	sub_08015000
	movs	r6, #6
	movs	r1, #0
	movs	r2, #12
	movs	r3, #3
	movs	r0, #18
	str	r6, [sp, #0]
	bl	sub_08015010
	ldr	r7, [sp, #108]
	adds	r1, r0, #0
	movs	r0, #153
	ldr	r5, [pc, #788]
	lsls	r0, r0, #3
	mov	r8, r6
	adds	r6, r7, r0
	str	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #0
	bl	sub_08015080
	ldr	r3, [pc, #772]
	ldr	r2, [r6, #0]
	ldr	r0, [r3, #16]
	movs	r1, #6
	movs	r3, #0
	str	r3, [sp, #0]
	bl	sub_080150b0
	mov	r1, r8
	str	r1, [sp, #0]
	movs	r2, #8
	movs	r1, #16
	movs	r3, #4
	movs	r0, #22
	bl	sub_08015010
	movs	r2, #154
	lsls	r2, r2, #3
	adds	r1, r0, #0
	adds	r6, r7, r2
	adds	r0, r5, #0
	str	r1, [r6, #0]
	adds	r0, #10
	movs	r2, #0
	movs	r3, #0
	adds	r5, #11
	bl	sub_08015080
	adds	r0, r5, #0
	movs	r3, #8
	ldr	r1, [r6, #0]
	movs	r2, #0
	bl	sub_08015080
	ldr	r4, [sp, #104]
	movs	r3, #0
	adds	r4, #12
	adds	r5, r6, #0
	str	r3, [sp, #100]
	str	r4, [sp, #48]
	str	r5, [sp, #64]
	ldr	r7, [sp, #100]
	movs	r6, #0
	mov	r8, r6
	cmp	r7, #16
	bgt.n	.L_080f44e2
	movs	r6, #128
	lsls	r5, r7, #12
	lsls	r6, r6, #1
	ldr	r1, [pc, #676]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r0, [sp, #116]
	bl	sub_080f4100
	movs	r1, #160
	add	r0, sp, #256
	lsls	r1, r1, #19
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f4100
.L_080f44e2:
	bl	sub_080049ac
	ldr	r0, [sp, #104]
	ldr	r1, [sp, #48]
	bl	sub_080051d8
	ldr	r3, [pc, #644]
	ldr	r0, [sp, #108]
	movs	r2, #200
	str	r3, [r0, r2]
	movs	r3, #214
	movs	r2, #204
	lsls	r3, r3, #2
	str	r3, [r0, r2]
	ldr	r3, [pc, #632]
	movs	r2, #208
	str	r3, [r0, r2]
	movs	r3, #230
	movs	r2, #212
	lsls	r3, r3, #2
	str	r3, [r0, r2]
	ldr	r3, [pc, #620]
	movs	r2, #216
	str	r3, [r0, r2]
	movs	r3, #232
	movs	r2, #220
	lsls	r3, r3, #2
	str	r3, [r0, r2]
	ldr	r3, [pc, #612]
	movs	r2, #224
	movs	r1, #234
	str	r3, [r0, r2]
	lsls	r1, r1, #2
	movs	r3, #228
	str	r1, [r0, r3]
	ldr	r3, [pc, #600]
	movs	r2, #232
	str	r3, [r0, r2]
	movs	r3, #236
	str	r1, [r0, r3]
	movs	r5, #236
	movs	r1, #0
	ldr	r6, [pc, #592]
	ldr	r2, [sp, #40]
	mov	fp, r1
	lsls	r5, r5, #2
	movs	r4, #32
	movs	r0, #244
	movs	r1, #240
.L_080f4544:
	ldr	r7, [sp, #108]
	adds	r3, r4, #0
	orrs	r3, r6
	str	r3, [r7, r1]
	ldmia	r2!, {r3}
	lsls	r3, r3, #3
	adds	r3, r3, r5
	str	r3, [r7, r0]
	movs	r3, #1
	add	fp, r3
	mov	r7, fp
	adds	r0, #8
	adds	r1, #8
	adds	r4, #16
	cmp	r7, #2
	bne.n	.L_080f4544
	ldr	r3, [sp, #96]
	movs	r0, #7
	subs	r3, #2
	str	r0, [sp, #68]
	cmp	r3, #1
	bhi.n	.L_080f463c
	movs	r1, #0
	movs	r3, #56
	mov	fp, r1
	adds	r2, r3, #0
	movs	r1, #224
	adds	r7, r3, #0
	lsls	r1, r1, #14
	movs	r6, #0
	adds	r2, #204
	adds	r7, #200
.L_080f4584:
	ldr	r4, [sp, #88]
	mov	r3, fp
	lsls	r5, r3, #2
	cmp	r4, r6
	bne.n	.L_080f459c
	ldr	r0, [pc, #508]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	bl	sub_080f9010
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]
.L_080f459c:
	ldr	r0, [sp, #88]
	cmp	r0, r6
	blt.n	.L_080f45be
	ldr	r3, [pc, #492]
	ldr	r4, [sp, #108]
	orrs	r3, r1
	str	r3, [r4, r7]
	add	r3, sp, #224
	ldr	r3, [r3, r5]
	lsls	r3, r3, #5
	adds	r3, #152
	str	r3, [r4, r2]
	ldr	r5, [sp, #68]
	adds	r5, #1
	str	r5, [sp, #68]
	adds	r2, #8
	adds	r7, #8
.L_080f45be:
	movs	r3, #1
	movs	r0, #128
	add	fp, r3
	lsls	r0, r0, #14
	mov	r4, fp
	adds	r1, r1, r0
	adds	r6, #5
	cmp	r4, #4
	bne.n	.L_080f4584
	ldr	r5, [sp, #88]
	cmp	r5, #19
	ble.n	.L_080f463c
	ldr	r6, [sp, #84]
	cmp	r6, #0
	blt.n	.L_080f463c
	ldr	r3, [pc, #436]
	ldrb	r2, [r3, r6]
	movs	r7, #0
	mov	fp, r7
	cmp	r2, #0
	beq.n	.L_080f463c
	ldr	r3, [pc, #428]
	ldrb	r3, [r3, r6]
	ldr	r1, [sp, #68]
	lsls	r3, r3, #3
	movs	r0, #96
	adds	r4, r3, #0
	lsls	r3, r1, #3
	adds	r1, r3, #0
	mov	ip, r0
	adds	r0, r3, #0
	ldr	r3, [pc, #412]
	adds	r5, r2, #0
	mov	lr, r3
	movs	r3, #158
	lsls	r2, r5, #4
	lsls	r3, r3, #2
	ldr	r7, [pc, #404]
	adds	r4, #152
	adds	r0, #204
	adds	r1, #200
	subs	r2, r3, r2
.L_080f4612:
	adds	r3, r2, #0
	ands	r3, r7
	mov	r6, ip
	lsls	r3, r3, #16
	orrs	r3, r6
	mov	r6, lr
	orrs	r3, r6
	ldr	r6, [sp, #108]
	str	r3, [r6, r1]
	str	r4, [r6, r0]
	ldr	r3, [sp, #68]
	movs	r6, #1
	adds	r3, #1
	add	fp, r6
	adds	r0, #8
	adds	r1, #8
	str	r3, [sp, #68]
	adds	r4, #32
	adds	r2, #32
	cmp	fp, r5
	bne.n	.L_080f4612
.L_080f463c:
	ldr	r7, [sp, #96]
	cmp	r7, #2
	beq.n	.L_080f4644
	b.n	sub_080f47bc
.L_080f4644:
	ldr	r0, [sp, #88]
	cmp	r0, #20
	bne.n	.L_080f46d8
	ldr	r1, [sp, #80]
	cmp	r1, #0
	ble.n	.L_080f46d8
	adds	r0, r1, #0
	bl	sub_08077230
	ldr	r3, [sp, #80]
	movs	r2, #1
	str	r2, [sp, #72]
	cmp	r3, #9
	ble.n	.L_080f4686
	ldr	r5, [sp, #80]
	movs	r4, #2
	str	r4, [sp, #72]
	cmp	r5, #99
	ble.n	.L_080f4686
	ldr	r7, [sp, #80]
	ldr	r0, [pc, #308]
	movs	r6, #3
	str	r6, [sp, #72]
	cmp	r7, r0
	ble.n	.L_080f4686
	ldr	r2, [sp, #80]
	ldr	r3, [pc, #300]
	movs	r1, #4
	str	r1, [sp, #72]
	cmp	r2, r3
	ble.n	.L_080f4686
	movs	r4, #5
	str	r4, [sp, #72]
.L_080f4686:
	ldr	r5, [sp, #72]
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	movs	r0, #9
	subs	r0, r0, r3
	adds	r2, r5, #0
	movs	r3, #6
	str	r3, [sp, #0]
	adds	r2, #12
	movs	r1, #16
	movs	r3, #3
	bl	sub_08015010
	ldr	r7, [pc, #264]
	ldr	r6, [sp, #108]
	adds	r5, r6, r7
	ldr	r6, [pc, #260]
	adds	r1, r0, #0
	str	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	ldr	r0, [sp, #72]
	lsls	r2, r0, #3
	ldr	r1, [r5, #0]
	adds	r2, #48
	subs	r0, r6, #1
	movs	r3, #0
	bl	sub_08015080
	movs	r1, #0
	ldr	r2, [r5, #0]
	ldr	r0, [sp, #80]
	str	r1, [sp, #0]
	movs	r3, #48
	ldr	r1, [sp, #72]
	bl	sub_080150b0
.L_080f46d8:
	ldr	r2, [sp, #76]
	cmp	r2, #0
	ble.n	.L_080f46e2
	subs	r2, #1
	str	r2, [sp, #76]
.L_080f46e2:
	ldr	r3, [sp, #88]
	cmp	r3, #19
	ble.n	.L_080f4742
	ldr	r3, [pc, #200]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f4742
	movs	r0, #112
	bl	sub_080f9010
	ldr	r4, [sp, #80]
	cmp	r4, #0
	ble.n	.L_080f470a
	movs	r5, #3
	movs	r6, #0
	str	r5, [sp, #96]
	str	r6, [sp, #76]
	b.n	.L_080f4742
.L_080f470a:
	movs	r0, #0
	movs	r3, #6
	str	r0, [sp, #96]
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	bl	sub_08015010
	ldr	r5, [pc, #152]
	ldr	r2, [sp, #64]
	adds	r1, r0, #0
	str	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	ldr	r3, [sp, #64]
	movs	r7, #1
	ldr	r1, [r3, #0]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #8
	mov	r8, r7
	bl	sub_08015080
.L_080f4742:
	ldr	r4, [sp, #88]
	adds	r4, #1
	str	r4, [sp, #88]
	b.n	sub_080f49e6
