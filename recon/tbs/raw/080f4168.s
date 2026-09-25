.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f3c, 0x08002f3c
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080040e8, 0x080040e8
	.set sub_08004458, 0x08004458
	.set sub_080048b0, 0x080048b0
	.set sub_080048f4, 0x080048f4
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005268, 0x08005268
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_08015000, 0x08015000
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_08077230, 0x08077230
	.set sub_080772c8, 0x080772c8
	.set sub_080f4028, 0x080f4028
	.set sub_080f4100, 0x080f4100
	.set sub_080f9010, 0x080f9010
	.global LuckyDice_Run
	.global Func_080f4168
	.thumb_func
LuckyDice_Run:
Func_080f4168:
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
.L_080f44b8:
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
	b.n	.L_080f47bc
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
	b.n	.L_080f49e6
	movs	r0, r0
	.4byte 0x00007084
	.4byte 0x00007088
	.4byte 0xff600000
	.4byte 0x0000709c
	.4byte 0x000070a0
	.4byte 0xffd80000
	.4byte 0x000070a4
	.4byte 0x00000906
	.4byte 0x02000240
	.4byte 0x05000200
	.4byte 0xc0006000
	.4byte 0x40102020
	.4byte 0x40102030
	.4byte 0x40042020
	.4byte 0x40042030
	.4byte 0x40202000
	.4byte 0x0000012f
	.4byte 0x80002040
	.4byte 0x080f53fc
	.4byte 0x080f5400
	.4byte 0x80002000
	.4byte 0x000001ff
	.4byte 0x000003e7
	.4byte 0x0000270f
	.4byte 0x000004cc
	.4byte 0x00000907
	.4byte 0x03001b04
	.2byte 0x0910
	.2byte 0x0000
.L_080f47bc:
	ldr	r5, [sp, #96]
	cmp	r5, #3
	bne.n	.L_080f484c
	ldr	r6, [sp, #76]
	cmp	r6, #0
	bne.n	.L_080f47f8
	movs	r7, #0
	ldr	r0, [sp, #108]
	ldr	r1, [pc, #740]
	str	r7, [sp, #80]
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	movs	r0, #0
	ldr	r1, [sp, #72]
	movs	r3, #40
	str	r7, [sp, #0]
	bl	sub_080150b0
	ldr	r3, [pc, #724]
	ldr	r2, [sp, #108]
	movs	r4, #153
	lsls	r4, r4, #3
	ldr	r0, [r3, #16]
	adds	r3, r2, r4
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r7, [sp, #0]
	bl	sub_080150b0
.L_080f47f8:
	ldr	r5, [sp, #76]
	cmp	r5, #15
	bne.n	.L_080f4844
	ldr	r7, [sp, #108]
	ldr	r0, [pc, #688]
	movs	r6, #0
	str	r6, [sp, #96]
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015018
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #8
	movs	r3, #4
	movs	r0, #22
	bl	sub_08015010
	ldr	r5, [pc, #664]
	ldr	r2, [sp, #64]
	adds	r1, r0, #0
	str	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	ldr	r3, [sp, #64]
	adds	r0, r5, #1
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
	movs	r4, #1
	mov	r8, r4
.L_080f4844:
	ldr	r5, [sp, #76]
	adds	r5, #1
	str	r5, [sp, #76]
	b.n	.L_080f49e6
.L_080f484c:
	ldr	r6, [sp, #96]
	cmp	r6, #0
	beq.n	.L_080f4854
	b.n	.L_080f49e6
.L_080f4854:
	ldr	r7, [sp, #100]
	cmp	r7, #17
	bgt.n	.L_080f485c
	b.n	.L_080f49e6
.L_080f485c:
	ldr	r7, [pc, #608]
	ldr	r0, [pc, #612]
	ldr	r3, [r7, #0]
	ldr	r3, [r7, #0]
	ldr	r3, [r0, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f488a
	ldr	r1, [sp, #112]
	ldr	r3, [pc, #596]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r2, #0]
	ldr	r4, [sp, #112]
	ldr	r5, [pc, #584]
	adds	r2, r4, r5
	ldr	r3, [r2, #0]
	adds	r3, r3, r1
	str	r3, [r2, #0]
.L_080f488a:
	ldr	r3, [r0, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f48ac
	ldr	r6, [sp, #112]
	ldr	r0, [pc, #560]
	adds	r2, r6, r0
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #560]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	ldr	r3, [pc, #552]
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	adds	r3, r3, r1
	str	r3, [r2, #0]
.L_080f48ac:
	ldr	r4, [sp, #112]
	ldr	r5, [pc, #536]
	adds	r2, r4, r5
	ldr	r3, [r2, #0]
	ldr	r0, [pc, #540]
	cmp	r3, r0
	bgt.n	.L_080f48c0
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #0]
.L_080f48c0:
	movs	r1, #160
	lsls	r1, r1, #16
	cmp	r3, r1
	ble.n	.L_080f48ca
	str	r1, [r2, #0]
.L_080f48ca:
	ldr	r3, [pc, #512]
	ldr	r6, [sp, #112]
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	cmp	r3, r0
	bgt.n	.L_080f48dc
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, #0]
.L_080f48dc:
	cmp	r3, r1
	ble.n	.L_080f48e2
	str	r1, [r2, #0]
.L_080f48e2:
	ldr	r6, [pc, #468]
	bl	sub_080772c8
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [r6, #16]
	lsls	r3, r3, #1
	cmp	r2, r3
	bcs.n	.L_080f4902
	ldr	r4, [sp, #64]
	movs	r1, #1
	ldr	r0, [r4, #0]
	bl	sub_08015018
	bl	.L_080f535e
.L_080f4902:
	ldr	r5, [r7, #0]
	movs	r3, #2
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_080f4920
	movs	r0, #113
	bl	sub_080f9010
	ldr	r5, [sp, #64]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl	sub_08015018
	bl	.L_080f535e
.L_080f4920:
	ldr	r3, [r7, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f49e6
	movs	r0, #151
	lsls	r0, r0, #1
	bl	sub_080f9010
	ldr	r7, [sp, #64]
	movs	r1, #1
	ldr	r0, [r7, #0]
	bl	sub_08015018
	movs	r0, #1
	str	r0, [sp, #96]
	bl	sub_080772c8
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	ldr	r1, [sp, #92]
	ldr	r3, [r6, #16]
	cmp	r1, r3
	bls.n	.L_080f4956
	str	r3, [sp, #92]
.L_080f4956:
	ldr	r2, [sp, #92]
	negs	r0, r2
	bl	sub_08077230
	ldr	r0, [r6, #16]
	ldr	r4, [sp, #108]
	movs	r6, #153
	lsls	r6, r6, #3
	adds	r3, r4, r6
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_080150b0
	movs	r7, #0
	ldr	r0, [sp, #112]
	movs	r1, #225
	mov	fp, r7
	lsls	r1, r1, #7
	ldr	r7, [sp, #36]
	adds	r6, r0, r1
.L_080f4982:
	mov	r2, fp
	cmp	r2, #1
	ble.n	.L_080f4992
	ldr	r3, [pc, #332]
	str	r3, [r6, #8]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r6, #4]
.L_080f4992:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #64
	lsls	r3, r3, #12
	str	r3, [r6, #12]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #48
	lsls	r3, r3, #13
	str	r3, [r6, #16]
	bl	sub_08004458
	movs	r5, #63
	movs	r1, #6
	ands	r5, r0
	ldr	r0, [r6, #4]
	bl	sub_080022ec
	adds	r5, #140
	lsls	r5, r5, #12
	adds	r5, r5, r0
	str	r5, [r6, #20]
	bl	sub_08004458
	movs	r1, #144
	lsls	r1, r1, #7
	bl	sub_08002304
	movs	r3, #150
	lsls	r3, r3, #1
	str	r0, [r6, #24]
	stmia	r7!, {r3}
	movs	r3, #1
	add	fp, r3
	mov	r4, fp
	adds	r6, #28
	cmp	r4, #2
	bne.n	.L_080f4982
.L_080f49e6:
	mov	r5, r8
	cmp	r5, #1
	bne.n	.L_080f4a40
	ldr	r6, [sp, #112]
	movs	r7, #225
	ldr	r0, [pc, #212]
	lsls	r7, r7, #7
	movs	r3, #160
	adds	r2, r6, r7
	lsls	r3, r3, #14
	str	r3, [r2, #0]
	ldr	r1, [pc, #220]
	adds	r3, r6, r0
	movs	r0, #192
	lsls	r0, r0, #15
	str	r0, [r3, #0]
	adds	r3, r6, r1
	ldr	r1, [pc, #212]
	str	r1, [r3, #0]
	ldr	r3, [pc, #212]
	ldr	r4, [pc, #188]
	adds	r2, r6, r3
	ldr	r3, [pc, #212]
	ldr	r5, [pc, #212]
	str	r3, [r2, #0]
	adds	r3, r6, r4
	str	r0, [r3, #0]
	adds	r3, r6, r5
	str	r1, [r3, #0]
	bl	sub_08004458
	movs	r1, #6
	bl	sub_08002304
	str	r0, [sp, #136]
.L_080f4a2c:
	bl	sub_08004458
	movs	r1, #6
	bl	sub_08002304
	ldr	r6, [sp, #40]
	str	r0, [r6, #4]
	ldr	r3, [sp, #136]
	cmp	r3, r0
	beq.n	.L_080f4a2c
.L_080f4a40:
	movs	r7, #0
	mov	fp, r7
	mov	r0, sp
	adds	r0, #128
	mov	r1, fp
	str	r0, [sp, #44]
	movs	r2, #1
	str	r1, [r0, #0]
	ldr	r3, [pc, #136]
	movs	r5, #0
	str	r2, [sp, #132]
	movs	r4, #1
	ldr	r2, [sp, #44]
	mov	sl, r5
	mov	ip, r3
	mov	lr, r0
	lsls	r3, r4, #3
	mov	r0, sl
	ldr	r1, [r0, r2]
	subs	r3, r3, r4
	lsls	r3, r3, #2
	mov	r6, ip
	adds	r7, r3, r6
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r2, r3, #2
	ldr	r3, [sp, #112]
	add	r2, ip
	ldr	r2, [r3, r2]
	ldr	r3, [r3, r7]
	mov	r8, r3
	add	r6, sp, #132
	cmp	r2, r8
	ble.n	.L_080f4af4
.L_080f4a84:
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	str	r1, [r6, #0]
	mov	r1, sl
	subs	r6, #4
	subs	r5, #4
	cmp	r1, #0
	blt.n	.L_080f4af0
	mov	r2, lr
	adds	r0, r5, #0
	ldr	r1, [r0, r2]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r2, r3, #2
	ldr	r3, [sp, #112]
	add	r2, ip
	ldr	r2, [r3, r2]
	ldr	r3, [r3, r7]
	mov	r8, r3
	cmp	r2, r8
	bgt.n	.L_080f4a84
	b.n	.L_080f4af4
	movs	r0, r0
	.4byte 0x000004cc
	.4byte 0x02000240
	.4byte 0x00000910
	.4byte 0x03001b04
	.4byte 0x03001ae8
	.4byte 0x00007084
	.4byte 0x000070a0
	.4byte 0xfffc0000
	.4byte 0x001fffff
	.4byte 0xff4c0000
	.4byte 0x00007088
	.4byte 0xff600000
	.4byte 0x0000709c
	.4byte 0xffd80000
	.2byte 0x70a4
	.2byte 0x0000
.L_080f4af0:
	mov	r5, sl
	lsls	r0, r5, #2
.L_080f4af4:
	adds	r3, r0, #4
	mov	r6, lr
	str	r4, [r3, r6]
	ldr	r1, [sp, #68]
	movs	r7, #0
	lsls	r3, r1, #3
	movs	r0, #240
	movs	r2, #204
	adds	r2, r2, r3
	mov	fp, r7
	add	r0, sp
	adds	r3, #200
	ldr	r7, [sp, #44]
	add	r6, sp, #200
	mov	r9, r0
	mov	sl, r2
	mov	r8, r3
.L_080f4b16:
	ldr	r2, [r7, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r4, [sp, #112]
	lsls	r3, r3, #2
	movs	r5, #225
	adds	r3, r4, r3
	lsls	r5, r5, #7
	adds	r4, r3, r5
	adds	r0, r4, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08005268
	ldr	r2, [r6, #8]
	ldr	r0, [pc, #916]
	ldr	r4, [sp, #8]
	cmp	r2, r0
	bgt.n	.L_080f4b44
	movs	r3, #175
	lsls	r3, r3, #1
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L_080f4b44:
	ldr	r3, [pc, #904]
	cmp	r2, r3
	ble.n	.L_080f4b4e
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L_080f4b4e:
	ldr	r3, [r7, #0]
	ldr	r1, [pc, #896]
	lsls	r3, r3, #2
	adds	r2, r2, r1
	mov	r5, r9
	str	r2, [r5, r3]
	ldr	r5, [r7, #0]
	mov	r0, r9
	lsls	r3, r5, #2
	ldr	r1, [r0, r3]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	asrs	r1, r1, #1
	movs	r0, #128
	adds	r1, #128
	lsls	r0, r0, #4
	str	r4, [sp, #8]
	bl	sub_080022ec
	ldr	r3, [r6, #0]
	subs	r2, r3, r0
	ldr	r3, [r6, #4]
	subs	r0, r3, r0
	adds	r3, r0, #0
	movs	r1, #16
	adds	r3, #96
	negs	r1, r1
	ldr	r4, [sp, #8]
	cmp	r3, r1
	ble.n	.L_080f4bd8
	lsls	r1, r5, #25
	movs	r5, #158
	lsls	r5, r5, #2
	adds	r3, r2, r5
	ldr	r2, [pc, #836]
	ands	r3, r2
	lsls	r3, r3, #16
	subs	r2, #159
	orrs	r1, r3
	adds	r3, r0, r2
	movs	r2, #255
	ands	r3, r2
	orrs	r1, r3
	ldr	r3, [pc, #820]
	orrs	r1, r3
	ldr	r3, [sp, #108]
	mov	r5, r8
	str	r1, [r3, r5]
	ldr	r0, [r4, #24]
	ldr	r2, [pc, #812]
	cmp	r0, #0
	bge.n	.L_080f4bba
	ldr	r1, [pc, #812]
	adds	r0, r0, r1
.L_080f4bba:
	asrs	r3, r0, #10
	ldrb	r3, [r2, r3]
	movs	r2, #128
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	orrs	r3, r2
	ldr	r2, [sp, #108]
	mov	r4, sl
	str	r3, [r2, r4]
	ldr	r0, [sp, #68]
	adds	r0, #1
	movs	r5, #8
	str	r0, [sp, #68]
	add	sl, r5
	add	r8, r5
.L_080f4bd8:
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	adds	r7, #4
	cmp	r2, #2
	bne.n	.L_080f4b16
	ldr	r3, [sp, #96]
	cmp	r3, #0
	bne.n	.L_080f4ca6
	ldr	r2, [sp, #68]
	movs	r4, #0
	ldr	r7, [sp, #44]
	mov	r6, sp
	ldr	r0, [sp, #112]
	movs	r1, #225
	lsls	r3, r2, #3
	mov	fp, r4
	movs	r5, #200
	adds	r6, #240
	lsls	r1, r1, #7
	movs	r4, #204
	add	r5, sp
	str	r7, [sp, #24]
	adds	r0, r0, r1
	adds	r4, r4, r3
	adds	r7, r3, #0
	str	r6, [sp, #60]
	mov	r9, r5
	mov	sl, r0
	mov	r8, r4
	adds	r7, #200
.L_080f4c16:
	mov	r1, r9
	mov	r0, sl
	bl	sub_08005268
	ldr	r6, [sp, #24]
	ldmia	r6!, {r3}
	adds	r5, r6, #0
	ldr	r0, [sp, #60]
	str	r5, [sp, #24]
	lsls	r3, r3, #2
	ldr	r6, [r0, r3]
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	asrs	r6, r6, #1
	adds	r6, #128
	movs	r0, #128
	adds	r1, r6, #0
	lsls	r0, r0, #4
	bl	sub_080022ec
	mov	r1, r9
	ldr	r5, [r1, #0]
	subs	r5, r5, r0
	movs	r0, #192
	adds	r1, r6, #0
	lsls	r0, r0, #2
	bl	sub_080022ec
	mov	r3, r9
	ldr	r2, [r3, #4]
	movs	r4, #158
	ldr	r3, [pc, #640]
	lsls	r4, r4, #2
	movs	r6, #176
	adds	r2, r2, r0
	adds	r5, r5, r4
	lsls	r6, r6, #1
	ands	r5, r3
	adds	r2, r2, r6
	movs	r3, #255
	ands	r2, r3
	lsls	r5, r5, #16
	ldr	r3, [pc, #636]
	orrs	r5, r2
	ldr	r0, [sp, #108]
	orrs	r5, r3
	mov	r1, fp
	str	r5, [r0, r7]
	cmp	r1, #1
	bne.n	.L_080f4c84
	ldr	r3, [r0, r7]
	movs	r2, #128
	lsls	r2, r2, #21
	orrs	r3, r2
	str	r3, [r0, r7]
.L_080f4c84:
	ldr	r2, [sp, #108]
	ldr	r3, [pc, #612]
	mov	r4, r8
	str	r3, [r2, r4]
	ldr	r6, [sp, #68]
	movs	r1, #1
	add	fp, r1
	movs	r5, #8
	adds	r6, #1
	movs	r0, #28
	mov	r2, fp
	add	r8, r5
	adds	r7, #8
	str	r6, [sp, #68]
	add	sl, r0
	cmp	r2, #2
	bne.n	.L_080f4c16
.L_080f4ca6:
	ldr	r0, [sp, #112]
	movs	r6, #128
	movs	r1, #225
	ldr	r2, [sp, #68]
	movs	r3, #0
	lsls	r6, r6, #19
	lsls	r1, r1, #7
	mov	fp, r3
	mov	r9, r6
	lsls	r3, r2, #3
	adds	r6, r0, r1
	movs	r0, #200
	adds	r0, r0, r3
	adds	r3, #204
	add	r4, sp, #212
	add	r5, sp, #200
	add	r7, sp, #248
	mov	sl, r0
	mov	r8, r3
.L_080f4ccc:
	ldr	r3, [r6, #0]
	movs	r1, #0
	str	r1, [r4, #4]
	str	r3, [r4, #0]
	ldr	r3, [r6, #8]
	adds	r0, r4, #0
	str	r3, [r4, #8]
	adds	r1, r5, #0
	str	r4, [sp, #8]
	bl	sub_08005268
	ldr	r3, [r5, #8]
	ldr	r2, [pc, #484]
	ldr	r4, [sp, #8]
	cmp	r3, r2
	bgt.n	.L_080f4cf2
	movs	r3, #175
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L_080f4cf2:
	ldr	r2, [pc, #476]
	cmp	r3, r2
	ble.n	.L_080f4cfc
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080f4cfc:
	ldr	r0, [pc, #468]
	adds	r3, r3, r0
	str	r3, [r7, #0]
	movs	r1, #6
	ldrsh	r2, [r6, r1]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r1, [r5, #4]
	movs	r0, #16
	adds	r3, r1, #0
	adds	r3, #88
	negs	r0, r0
	ldr	r2, [r5, #0]
	cmp	r3, r0
	ble.n	.L_080f4d54
	movs	r0, #156
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldr	r2, [pc, #436]
	movs	r0, #172
	ands	r3, r2
	lsls	r0, r0, #1
	mov	r2, r9
	lsls	r3, r3, #16
	orrs	r3, r2
	adds	r2, r1, r0
	movs	r1, #255
	ands	r2, r1
	orrs	r3, r2
	ldr	r2, [pc, #420]
	ldr	r1, [sp, #108]
	orrs	r3, r2
	mov	r2, sl
	str	r3, [r1, r2]
	movs	r3, #137
	mov	r0, r8
	lsls	r3, r3, #4
	str	r3, [r1, r0]
	ldr	r2, [sp, #68]
	adds	r2, #1
	movs	r1, #8
	str	r2, [sp, #68]
	add	sl, r1
	add	r8, r1
.L_080f4d54:
	movs	r0, #1
	movs	r3, #128
	add	fp, r0
	lsls	r3, r3, #18
	mov	r1, fp
	add	r9, r3
	adds	r7, #4
	adds	r6, #28
	cmp	r1, #2
	bne.n	.L_080f4ccc
	ldr	r2, [sp, #96]
	cmp	r2, #1
	beq.n	.L_080f4d70
	b.n	.L_080f5260
.L_080f4d70:
	ldr	r4, [sp, #36]
	movs	r3, #0
	ldr	r5, [sp, #112]
	movs	r6, #225
	lsls	r6, r6, #7
	str	r3, [sp, #56]
	str	r4, [sp, #32]
	str	r3, [sp, #28]
	mov	fp, r3
	adds	r7, r5, r6
.L_080f4d84:
	ldr	r1, [r7, #0]
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #16]
	adds	r1, r1, r3
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #8]
	str	r1, [r7, #0]
	ldr	r2, [sp, #112]
	movs	r3, #225
	lsls	r3, r3, #7
	movs	r0, #0
	adds	r2, r2, r3
	mov	sl, r0
	mov	r9, r2
.L_080f4daa:
	cmp	fp, sl
	beq.n	.L_080f4e1c
	mov	r4, r9
	ldr	r3, [r4, #0]
	subs	r3, r1, r3
	asrs	r6, r3, #16
	ldr	r2, [r7, #4]
	ldr	r3, [r4, #4]
	mov	r5, r9
	subs	r2, r2, r3
	asrs	r4, r2, #16
	ldr	r3, [r5, #8]
	ldr	r2, [r7, #8]
	subs	r2, r2, r3
	asrs	r2, r2, #16
	mov	r8, r2
	adds	r3, r4, #0
	muls	r3, r4
	adds	r2, r6, #0
	muls	r2, r6
	mov	r0, r8
	adds	r2, r2, r3
	mov	r3, r8
	muls	r3, r0
	adds	r0, r2, r3
	ldr	r2, [pc, #272]
	cmp	r0, r2
	bgt.n	.L_080f4e1c
	str	r4, [sp, #8]
	ldr	r3, [pc, #268]
	bl	sub_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	lsls	r0, r6, #15
	bl	sub_080022ec
	ldr	r3, [r7, #12]
	ldr	r4, [sp, #8]
	adds	r3, r3, r0
	str	r3, [r7, #12]
	lsls	r0, r4, #15
	adds	r1, r5, #0
	bl	sub_080022ec
	ldr	r3, [r7, #16]
	adds	r3, r3, r0
	str	r3, [r7, #16]
	mov	r3, r8
	lsls	r0, r3, #15
	adds	r1, r5, #0
	bl	sub_080022ec
	ldr	r3, [r7, #20]
	adds	r3, r3, r0
	str	r3, [r7, #20]
	ldr	r1, [r7, #0]
.L_080f4e1c:
	movs	r5, #1
	add	sl, r5
	movs	r4, #28
	mov	r6, sl
	add	r9, r4
	cmp	r6, #2
	bne.n	.L_080f4daa
	ldr	r0, [pc, #204]
	cmp	r1, r0
	bge.n	.L_080f4e48
	ldr	r2, [r7, #12]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	str	r0, [r7, #0]
	cmp	r3, #0
	bge.n	.L_080f4e42
	adds	r3, #63
.L_080f4e42:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r1, r0, #0
.L_080f4e48:
	movs	r3, #200
	lsls	r3, r3, #16
	cmp	r1, r3
	ble.n	.L_080f4e66
	ldr	r2, [r7, #12]
	negs	r2, r2
	str	r3, [r7, #0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_080f4e62
	adds	r3, #63
.L_080f4e62:
	asrs	r3, r3, #6
	str	r3, [r7, #12]
.L_080f4e66:
	ldr	r2, [r7, #8]
	ldr	r1, [pc, #144]
	cmp	r2, r1
	bge.n	.L_080f4e86
	ldr	r2, [r7, #20]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	str	r1, [r7, #8]
	cmp	r3, #0
	bge.n	.L_080f4e80
	adds	r3, #63
.L_080f4e80:
	asrs	r3, r3, #6
	str	r3, [r7, #20]
	adds	r2, r1, #0
.L_080f4e86:
	ldr	r3, [r7, #4]
	movs	r0, #180
	lsls	r0, r0, #16
	adds	r3, r3, r0
	cmp	r2, r3
	ble.n	.L_080f4eb8
	ldr	r1, [r7, #20]
	str	r3, [r7, #8]
	cmp	r1, #0
	ble.n	.L_080f4eb8
	lsrs	r3, r1, #31
	ldr	r2, [r7, #16]
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r2, r2, r3
	str	r2, [r7, #16]
	negs	r2, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #4
	cmp	r2, #0
	bge.n	.L_080f4eb4
	adds	r2, #63
.L_080f4eb4:
	asrs	r3, r2, #6
	str	r3, [r7, #20]
.L_080f4eb8:
	ldr	r3, [r7, #16]
	ldr	r1, [pc, #68]
	adds	r2, r3, r1
	ldr	r3, [r7, #4]
	str	r2, [r7, #16]
	cmp	r3, #0
	ble.n	.L_080f4ec8
	b.n	.L_080f4fea
.L_080f4ec8:
	b.n	.L_080f4f04
	movs	r0, r0
	.4byte 0x0000015d
	.4byte 0x0000028a
	.4byte 0xfffffea2
	.4byte 0x000001ff
	.4byte 0x40002300
	.4byte 0x080f5408
	.4byte 0x000003ff
	.4byte 0x80002000
	.4byte 0x00000958
	.4byte 0x000009c3
	.4byte 0x030001d8
	.4byte 0xff380000
	.4byte 0xff6a0000
	.2byte 0x8000
	.2byte 0xffff
.L_080f4f04:
	.2byte 0x4252
	movs	r3, #0
	str	r3, [r7, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #4
	cmp	r2, #0
	bge.n	.L_080f4f16
	adds	r2, #63
.L_080f4f16:
	asrs	r3, r2, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080f4f74
	ldr	r3, [r7, #20]
	ldr	r2, [r7, #12]
	asrs	r3, r3, #8
	asrs	r2, r2, #8
	adds	r4, r3, #0
	muls	r4, r3
	adds	r0, r2, #0
	muls	r0, r2
	adds	r3, r4, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #524]
	bl	sub_080072f0
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080f4f4c
	adds	r3, #63
.L_080f4f4c:
	asrs	r6, r3, #6
	bl	sub_08004458
	ldr	r3, [pc, #504]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	b.n	.L_080f4f96
.L_080f4f74:
	ldr	r2, [r7, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080f4f82
	adds	r3, #63
.L_080f4f82:
	ldr	r2, [r7, #20]
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080f4f94
	adds	r3, #63
.L_080f4f94:
	asrs	r3, r3, #6
.L_080f4f96:
	str	r3, [r7, #20]
	ldr	r5, [sp, #32]
	ldr	r3, [r5, #0]
	ldr	r6, [sp, #28]
	cmp	r3, #19
	ble.n	.L_080f4fa6
	subs	r3, #20
	str	r3, [r5, #0]
.L_080f4fa6:
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #420]
	cmp	r3, r0
	bgt.n	.L_080f4fee
	movs	r1, #0
	str	r1, [r7, #12]
	str	r1, [r7, #16]
	str	r1, [r7, #20]
	ldr	r2, [sp, #32]
	str	r1, [r2, #0]
	ldr	r5, [r7, #24]
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_080f4fc6
	ldr	r3, [pc, #400]
	adds	r0, r5, r3
.L_080f4fc6:
	asrs	r0, r0, #10
	movs	r1, #3
	bl	sub_080022fc
	cmp	r0, #1
	bne.n	.L_080f4fdc
	movs	r4, #128
	lsls	r4, r4, #4
	adds	r3, r5, r4
	str	r3, [r7, #24]
	b.n	.L_080f4fee
.L_080f4fdc:
	cmp	r0, #2
	bne.n	.L_080f4fee
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r3, r5, r0
	str	r3, [r7, #24]
	b.n	.L_080f4fee
.L_080f4fea:
	mov	r1, fp
	lsls	r6, r1, #2
.L_080f4fee:
	ldr	r3, [r7, #12]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080f4ffa
	adds	r2, #63
.L_080f4ffa:
	asrs	r3, r2, #6
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080f500a
	adds	r2, #63
.L_080f500a:
	asrs	r3, r2, #6
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	lsls	r2, r3, #6
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080f501a
	adds	r2, #63
.L_080f501a:
	asrs	r3, r2, #6
	str	r3, [r7, #20]
	ldr	r4, [sp, #28]
	ldr	r5, [sp, #36]
	ldr	r2, [r7, #24]
	ldr	r3, [r4, r5]
	ldr	r0, [pc, #304]
	adds	r2, r2, r3
	str	r2, [r7, #24]
	cmp	r2, r0
	ble.n	.L_080f5036
	ldr	r1, [pc, #296]
	adds	r3, r2, r1
	str	r3, [r7, #24]
.L_080f5036:
	ldr	r2, [sp, #32]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L_080f5042
	subs	r3, #1
	str	r3, [r2, #0]
.L_080f5042:
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bne.n	.L_080f50a6
	ldr	r3, [r7, #16]
	cmp	r3, #0
	bne.n	.L_080f50a6
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bne.n	.L_080f50a6
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne.n	.L_080f50a6
	movs	r1, #192
	ldr	r0, [r7, #24]
	lsls	r1, r1, #4
	add	r5, sp, #224
	bl	sub_080022ec
	ldr	r1, [pc, #248]
	movs	r3, #0
	str	r0, [r5, r6]
	mov	sl, r3
	ldrsh	r3, [r1, r3]
	ldr	r2, [r7, #8]
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_080f5082
	adds	r3, r6, #0
	adds	r3, #8
	mov	r6, sl
	str	r6, [r5, r3]
	b.n	.L_080f50a0
.L_080f5082:
	movs	r0, #1
	add	sl, r0
	mov	r3, sl
	cmp	r3, #6
	beq.n	.L_080f50a0
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_080f5082
	ldr	r3, [sp, #28]
	add	r2, sp, #224
	adds	r3, #8
	mov	r5, sl
	str	r5, [r2, r3]
.L_080f50a0:
	ldr	r6, [sp, #56]
	adds	r6, #1
	str	r6, [sp, #56]
.L_080f50a6:
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #28]
	movs	r2, #1
	add	fp, r2
	adds	r0, #4
	adds	r1, #4
	mov	r3, fp
	str	r0, [sp, #32]
	str	r1, [sp, #28]
	adds	r7, #28
	cmp	r3, #2
	beq.n	.L_080f50c0
	b.n	.L_080f4d84
.L_080f50c0:
	ldr	r4, [sp, #56]
	cmp	r4, #2
	beq.n	.L_080f50c8
	b.n	.L_080f5260
.L_080f50c8:
	movs	r7, #1
	movs	r6, #0
	movs	r5, #2
	negs	r7, r7
	movs	r0, #60
	str	r5, [sp, #96]
	str	r6, [sp, #88]
	str	r7, [sp, #84]
	str	r6, [sp, #80]
	str	r0, [sp, #76]
	add	r1, sp, #224
	ldr	r2, [r1, #0]
	ldr	r3, [r1, #4]
	cmp	r2, r3
	bne.n	.L_080f5168
	ldr	r3, [r1, #8]
	cmp	r2, r3
	bne.n	.L_080f5168
	ldr	r3, [r1, #12]
	cmp	r2, r3
	bne.n	.L_080f5168
	ldr	r2, [sp, #52]
	movs	r3, #120
	str	r3, [r2, #4]
	str	r3, [sp, #144]
	str	r6, [sp, #84]
	ldr	r3, [sp, #136]
	ldr	r2, [r1, #0]
	cmp	r2, r3
	bne.n	.L_080f510c
	ldr	r4, [sp, #92]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	b.n	.L_080f5126
.L_080f510c:
	ldr	r5, [sp, #40]
	ldr	r3, [r5, #4]
	cmp	r2, r3
	bne.n	.L_080f5120
	ldr	r6, [sp, #92]
	lsls	r3, r6, #1
	adds	r3, r3, r6
	lsls	r3, r3, #3
	adds	r3, r3, r6
	b.n	.L_080f5126
.L_080f5120:
	ldr	r7, [sp, #92]
	lsls	r3, r7, #2
	adds	r3, r3, r7
.L_080f5126:
	str	r3, [sp, #80]
	ldr	r3, [pc, #56]
	ldr	r1, [sp, #108]
	movs	r2, #153
	lsls	r2, r2, #3
	ldr	r0, [r3, #16]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	str	r3, [sp, #0]
	bl	sub_080150b0
	movs	r0, #93
	bl	sub_080f9010
	b.n	.L_080f5260
	.4byte 0x030001d8
	.4byte 0x0000ffff
	.4byte 0x0002ffff
	.4byte 0x000003ff
	.4byte 0x000047ff
	.4byte 0xffffb800
	.4byte 0x080f541a
	.2byte 0x0240
	.2byte 0x0200
.L_080f5168:
	ldr	r5, [pc, #392]
	movs	r4, #0
	add	r7, sp, #768
	mov	r8, r4
	mov	fp, r4
	mov	ip, r5
	adds	r6, r7, #0
.L_080f5176:
	mov	r5, fp
	adds	r5, #1
	mov	sl, r5
	cmp	r5, #4
	beq.n	.L_080f51b0
	add	r1, sp, #768
	lsls	r3, r5, #2
	mov	r0, fp
	adds	r3, r3, r1
	ldr	r1, [pc, #364]
	add	r7, sp, #768
	lsls	r2, r0, #2
	mov	r0, ip
	adds	r4, r3, r0
	add	r7, ip
	adds	r0, r6, r1
.L_080f5196:
	ldr	r1, [r7, r2]
	ldmia	r4!, {r3}
	cmp	r1, r3
	bne.n	.L_080f51a6
	movs	r3, #1
	stmia	r0!, {r1}
	adds	r6, #4
	add	r8, r3
.L_080f51a6:
	movs	r1, #1
	add	sl, r1
	mov	r3, sl
	cmp	r3, #4
	bne.n	.L_080f5196
.L_080f51b0:
	mov	fp, r5
	cmp	r5, #3
	bne.n	.L_080f5176
	mov	r4, r8
	cmp	r4, #1
	bne.n	.L_080f51de
	ldr	r5, [sp, #92]
	ldr	r3, [pc, #316]
	str	r5, [sp, #80]
	ldr	r6, [sp, #108]
	movs	r7, #153
	lsls	r7, r7, #3
	ldr	r0, [r3, #16]
	movs	r1, #0
	adds	r3, r6, r7
	ldr	r2, [r3, #0]
	str	r1, [sp, #0]
	movs	r3, #0
	movs	r1, #6
	bl	sub_080150b0
	movs	r2, #3
	str	r2, [sp, #84]
.L_080f51de:
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L_080f521e
	ldr	r4, [sp, #92]
	lsls	r4, r4, #1
	str	r4, [sp, #80]
	ldr	r3, [pc, #272]
	ldr	r5, [sp, #108]
	movs	r6, #153
	lsls	r6, r6, #3
	ldr	r0, [r3, #16]
	adds	r3, r5, r6
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	movs	r7, #0
	str	r7, [sp, #0]
	bl	sub_080150b0
	bl	sub_08004458
	movs	r3, #1
	ands	r3, r0
	ldr	r0, [sp, #52]
	lsls	r3, r3, #2
	movs	r2, #60
	str	r2, [r3, r0]
	movs	r1, #2
	movs	r0, #91
	str	r1, [sp, #84]
	bl	sub_080f9010
.L_080f521e:
	mov	r2, r8
	cmp	r2, #3
	bne.n	.L_080f5260
	ldr	r4, [sp, #92]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	str	r3, [sp, #80]
	ldr	r5, [sp, #108]
	ldr	r3, [pc, #204]
	movs	r6, #153
	lsls	r6, r6, #3
	ldr	r0, [r3, #16]
	adds	r3, r5, r6
	ldr	r2, [r3, #0]
	movs	r1, #6
	movs	r3, #0
	movs	r7, #0
	str	r7, [sp, #0]
	bl	sub_080150b0
	bl	sub_08004458
	movs	r3, #1
	ands	r3, r0
	ldr	r0, [sp, #52]
	lsls	r3, r3, #2
	movs	r2, #60
	str	r2, [r3, r0]
	movs	r1, #1
	movs	r0, #92
	str	r1, [sp, #84]
	bl	sub_080f9010
.L_080f5260:
	ldr	r2, [sp, #68]
	cmp	r2, #128
	beq.n	.L_080f5286
	lsls	r3, r2, #3
	adds	r2, r3, #0
	ldr	r0, [pc, #148]
	movs	r1, #0
	adds	r2, #204
	adds	r3, #200
.L_080f5272:
	ldr	r4, [sp, #108]
	str	r0, [r4, r3]
	str	r1, [r4, r2]
	ldr	r5, [sp, #68]
	adds	r5, #1
	adds	r2, #8
	adds	r3, #8
	str	r5, [sp, #68]
	cmp	r5, #128
	bne.n	.L_080f5272
.L_080f5286:
	movs	r1, #228
	mov	r8, r1
	mov	r1, sp
	adds	r1, #240
	movs	r6, #0
	mov	r7, sp
	mov	r4, sp
	movs	r2, #220
	movs	r3, #212
	str	r1, [sp, #4]
	mov	fp, r6
	adds	r7, #166
	add	r5, sp, #164
	adds	r4, #162
	add	r0, sp, #160
	mov	lr, r2
	mov	ip, r3
	movs	r6, #204
.L_080f52aa:
	ldr	r1, [sp, #4]
	ldmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #4]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r3, #128
	ldr	r2, [pc, #52]
	ldr	r1, [pc, #48]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	strh	r3, [r7, #0]
	strh	r2, [r5, #0]
	strh	r1, [r4, #0]
	strh	r3, [r0, #0]
	ldrh	r2, [r7, #0]
	lsls	r2, r2, #16
	mov	sl, r2
	ldr	r2, [sp, #108]
	ldr	r3, [r2, r6]
	mov	r1, sl
	orrs	r3, r1
	str	r3, [r2, r6]
	ldrh	r2, [r5, #0]
	lsls	r2, r2, #16
	mov	sl, r2
	ldr	r2, [sp, #108]
	mov	r1, ip
	ldr	r3, [r2, r1]
	mov	r2, sl
	orrs	r3, r2
	ldr	r2, [sp, #108]
	str	r3, [r2, r1]
	b.n	.L_080f5304
	.4byte 0x00000000
	.4byte 0xfffffde0
	.4byte 0xfffffd78
	.4byte 0x02000240
	.2byte 0x2000
	.2byte 0x40f0
.L_080f5304:
	ldrh	r2, [r4, #0]
	ldr	r1, [sp, #108]
	lsls	r2, r2, #16
	mov	sl, r2
	mov	r2, lr
	ldr	r3, [r1, r2]
	mov	r1, sl
	orrs	r3, r1
	ldr	r1, [sp, #108]
	str	r3, [r1, r2]
	ldrh	r2, [r0, #0]
	lsls	r2, r2, #16
	mov	sl, r2
	mov	r2, r8
	ldr	r3, [r1, r2]
	mov	r1, sl
	orrs	r3, r1
	ldr	r1, [sp, #108]
	str	r3, [r1, r2]
	movs	r3, #1
	add	fp, r3
	movs	r2, #32
	mov	r1, fp
	add	r8, r2
	add	lr, r2
	add	ip, r2
	adds	r6, #32
	cmp	r1, #4
	bne.n	.L_080f52aa
	ldr	r0, [sp, #108]
	movs	r1, #224
	ldr	r3, [pc, #140]
	adds	r0, #200
	lsls	r1, r1, #19
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #100]
	adds	r2, #1
	str	r2, [sp, #100]
	bl	.L_080f44b8
.L_080f535e:
	movs	r3, #0
	movs	r6, #128
	mov	fp, r3
	lsls	r6, r6, #1
.L_080f5366:
	mov	r4, fp
	movs	r5, #128
	lsls	r3, r4, #12
	lsls	r5, r5, #9
	subs	r5, r5, r3
	ldr	r0, [sp, #116]
	adds	r2, r5, #0
	ldr	r1, [pc, #96]
	adds	r3, r6, #0
	bl	sub_080f4100
	adds	r2, r5, #0
	movs	r1, #160
	movs	r5, #1
	add	r0, sp, #256
	lsls	r1, r1, #19
	adds	r3, r6, #0
	add	fp, r5
	bl	sub_080f4100
	mov	r7, fp
	movs	r0, #1
	bl	sub_080030f8
	cmp	r7, #17
	bne.n	.L_080f5366
	movs	r0, #12
	bl	sub_08002dd8
	movs	r0, #45
	bl	sub_08002dd8
	movs	r0, #40
	bl	sub_08002dd8
	movs	r0, #39
	bl	sub_08002dd8
	movs	r0, #41
	bl	sub_08002dd8
	movs	r3, #192
	lsls	r3, r3, #2
	add	sp, r3
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x84000100
	.4byte 0x05000200
