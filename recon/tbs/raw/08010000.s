.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_0800fec8, 0x0800fec8
	.set sub_0800ff54, 0x0800ff54
	.global Map_UpdateLayerScroll
	.global Func_08010000
	.thumb_func
Map_UpdateLayerScroll:
Func_08010000:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #512]
	ldr	r3, [r3, #0]
	movs	r4, #130
	ldr	r1, [r3, #0]
	mov	r8, r3
	lsls	r4, r4, #1
	sub	sp, #8
	add	r4, r8
	cmp	r1, #0
	bne.n	.L_08010024
	b.n	.L_080101fe
.L_08010024:
	ldmia	r1!, {r3}
	ldr	r2, [pc, #492]
	adds	r7, r3, r2
	ldmia	r1!, {r2}
	ldr	r3, [r1, #0]
	ldr	r1, [pc, #488]
	subs	r3, r3, r2
	adds	r6, r3, r1
	mov	r3, r8
	adds	r3, #236
	mov	r2, r8
	ldr	r2, [r2, #4]
	ldr	r3, [r3, #0]
	adds	r0, r3, r2
	mov	r3, r8
	adds	r3, #244
	ldr	r3, [r3, #0]
	mov	ip, r2
	subs	r3, r3, r2
	ldr	r2, [pc, #464]
	adds	r1, r3, r2
	mov	r3, r8
	mov	r2, r8
	adds	r3, #240
	ldr	r5, [r2, #8]
	ldr	r3, [r3, #0]
	adds	r3, r3, r5
	mov	lr, r3
	mov	r3, r8
	adds	r3, #248
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #444]
	subs	r3, r3, r5
	adds	r3, r3, r2
	cmp	r0, r1
	ble.n	.L_0801006e
	adds	r1, r0, #0
.L_0801006e:
	cmp	lr, r3
	ble.n	.L_08010074
	mov	r3, lr
.L_08010074:
	cmp	r7, r0
	bge.n	.L_0801007a
	adds	r7, r0, #0
.L_0801007a:
	cmp	r7, r1
	ble.n	.L_08010080
	adds	r7, r1, #0
.L_08010080:
	cmp	r6, lr
	bge.n	.L_08010086
	mov	r6, lr
.L_08010086:
	cmp	r6, r3
	ble.n	.L_0801008c
	adds	r6, r3, #0
.L_0801008c:
	mov	r3, ip
	cmp	r3, #0
	beq.n	.L_080100c4
	str	r4, [sp, #0]
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	mov	r2, r8
	ldr	r2, [r2, #4]
	adds	r1, r0, #0
	mov	lr, r2
	ldr	r3, [pc, #380]
	mov	r0, lr
	subs	r1, r5, r1
	mov	ip, pc
	bx	r3
	mov	r2, r8
	adds	r7, r7, r0
	ldr	r1, [r2, #12]
	mov	r0, lr
	mov	ip, pc
	bx	r3
	mov	r3, r8
	str	r0, [r3, #4]
	ldr	r5, [r3, #8]
	ldr	r4, [sp, #0]
.L_080100c4:
	cmp	r5, #0
	beq.n	.L_080100fa
	str	r4, [sp, #0]
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	mov	r2, r8
	ldr	r2, [r2, #8]
	adds	r1, r0, #0
	mov	lr, r2
	ldr	r3, [pc, #324]
	mov	r0, lr
	subs	r1, r5, r1
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r2, r8
	adds	r6, r6, r0
	ldr	r1, [r2, #12]
	mov	r0, lr
	mov	ip, pc
	bx	r3
	mov	r3, r8
	str	r0, [r3, #8]
	ldr	r4, [sp, #0]
.L_080100fa:
	mov	r1, r8
	adds	r1, #228
	movs	r2, #232
	str	r1, [sp, #4]
	add	r2, r8
	str	r7, [r1, #0]
	ldr	r1, [pc, #284]
	movs	r3, #0
	str	r6, [r2, #0]
	mov	fp, r2
	mov	sl, r3
	mov	r9, r1
.L_08010112:
	ldr	r2, [sp, #4]
	ldr	r1, [r4, #16]
	ldr	r0, [r2, #0]
	mov	ip, pc
	bx	r9
	mov	r3, fp
	adds	r7, r0, #0
	ldr	r1, [r4, #20]
	ldr	r0, [r3, #0]
	mov	ip, pc
	bx	r9
	ldr	r2, [r4, #24]
	adds	r6, r0, #0
	cmp	r2, #0
	beq.n	.L_08010142
	ldr	r3, [r4, #32]
	adds	r3, r3, r2
	adds	r7, r7, r3
	str	r3, [r4, #32]
	ldrh	r3, [r4, #40]
	ldr	r2, [pc, #236]
	lsls	r3, r3, #19
	orrs	r3, r2
	ands	r7, r3
.L_08010142:
	ldr	r2, [r4, #28]
	cmp	r2, #0
	beq.n	.L_0801015a
	ldr	r3, [r4, #36]
	adds	r3, r3, r2
	adds	r6, r6, r3
	str	r3, [r4, #36]
	ldrh	r3, [r4, #42]
	ldr	r2, [pc, #212]
	lsls	r3, r3, #19
	orrs	r3, r2
	ands	r6, r3
.L_0801015a:
	ldr	r3, [r4, #8]
	adds	r7, r7, r3
	ldr	r3, [r4, #12]
	adds	r1, r7, #0
	adds	r6, r6, r3
	cmp	r7, #0
	bge.n	.L_0801016c
	ldr	r2, [pc, #188]
	adds	r1, r7, r2
.L_0801016c:
	asrs	r1, r1, #19
	mov	r8, r1
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_0801017a
	ldr	r3, [pc, #176]
	adds	r2, r6, r3
.L_0801017a:
	ldr	r1, [r4, #0]
	asrs	r5, r2, #19
	adds	r3, r1, #0
	movs	r2, #128
	eors	r3, r7
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080101aa
	cmp	r1, r7
	bge.n	.L_0801019c
	mov	r1, r8
	adds	r1, #30
	mov	r0, sl
	adds	r2, r5, #0
	str	r4, [sp, #0]
	b.n	.L_080101a4
.L_0801019c:
	mov	r0, sl
	mov	r1, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
.L_080101a4:
	bl	sub_0800ff54
	ldr	r4, [sp, #0]
.L_080101aa:
	ldr	r1, [r4, #4]
	movs	r2, #128
	adds	r3, r1, #0
	eors	r3, r6
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080101d8
	cmp	r1, r6
	bge.n	.L_080101ca
	adds	r2, r5, #0
	adds	r2, #20
	mov	r0, sl
	mov	r1, r8
	str	r4, [sp, #0]
	b.n	.L_080101d2
.L_080101ca:
	mov	r0, sl
	mov	r1, r8
	adds	r2, r5, #0
	str	r4, [sp, #0]
.L_080101d2:
	bl	sub_0800fec8
	ldr	r4, [sp, #0]
.L_080101d8:
	mov	r1, sl
	movs	r3, #3
	subs	r3, r3, r1
	ldr	r1, [pc, #76]
	lsls	r3, r3, #2
	asrs	r2, r7, #16
	strh	r2, [r1, r3]
	asrs	r2, r6, #16
	adds	r3, r3, r1
	strh	r2, [r3, #2]
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	str	r7, [r4, #0]
	str	r6, [r4, #4]
	adds	r4, #48
	cmp	r3, #2
	bhi.n	.L_080101fe
	b.n	.L_08010112
.L_080101fe:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0xff880000
	.4byte 0xffa00000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x03000118
	.4byte 0x0007ffff
	.2byte 0x1ad0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #440]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	adds	r0, r1, #0
	movs	r1, #130
	sub	sp, #32
	lsls	r1, r1, #1
	str	r3, [sp, #20]
	adds	r3, r3, r1
	mov	lr, r3
	ldr	r3, [pc, #424]
	adds	r0, r0, r3
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #420]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r7, r7, r2
	cmp	r7, r3
	bge.n	.L_08010266
	adds	r7, r3, #0
.L_08010266:
	ldr	r3, [sp, #20]
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #404]
	adds	r3, r3, r5
	cmp	r7, r3
	ble.n	.L_08010276
	adds	r7, r3, #0
.L_08010276:
	ldr	r3, [sp, #20]
	adds	r3, #240
	ldr	r3, [r3, #0]
	cmp	r0, r3
	bge.n	.L_08010282
	adds	r0, r3, #0
.L_08010282:
	ldr	r3, [sp, #20]
	adds	r3, #248
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #380]
	adds	r3, r3, r1
	cmp	r0, r3
	ble.n	.L_08010292
	adds	r0, r3, #0
.L_08010292:
	ldr	r2, [sp, #20]
	adds	r2, #228
	str	r2, [sp, #16]
	str	r7, [r2, #0]
	ldr	r3, [sp, #20]
	movs	r5, #0
	adds	r3, #232
	str	r3, [sp, #12]
	str	r0, [r3, #0]
	str	r5, [sp, #28]
.L_080102a6:
	ldr	r1, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #1
	ldr	r5, [sp, #20]
	adds	r3, r1, r2
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_080102b8
	b.n	.L_080103da
.L_080102b8:
	ldr	r1, [sp, #16]
	mov	r2, lr
	ldr	r0, [r1, #0]
	movs	r4, #22
	ldr	r1, [r2, #16]
	ldr	r3, [pc, #328]
	mov	ip, pc
	bx	r3
	ldr	r5, [sp, #12]
	mov	r2, lr
	adds	r7, r0, #0
	ldr	r1, [r2, #20]
	ldr	r0, [r5, #0]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r3, lr
	ldr	r2, [r3, #24]
	cmp	r2, #0
	beq.n	.L_080102f4
	ldr	r3, [r3, #32]
	mov	r5, lr
	adds	r3, r3, r2
	adds	r7, r7, r3
	str	r3, [r5, #32]
	ldrh	r3, [r5, #40]
	ldr	r1, [pc, #288]
	lsls	r3, r3, #19
	orrs	r3, r1
	ands	r7, r3
.L_080102f4:
	mov	r3, lr
	ldr	r2, [r3, #28]
	cmp	r2, #0
	beq.n	.L_08010312
	ldr	r3, [r3, #36]
	mov	r5, lr
	adds	r3, r3, r2
	adds	r0, r0, r3
	str	r3, [r5, #36]
	ldrh	r3, [r5, #42]
	ldr	r1, [pc, #260]
	lsls	r3, r3, #19
	orrs	r3, r1
	ands	r0, r3
	movs	r4, #32
.L_08010312:
	mov	r2, lr
	ldr	r3, [r2, #8]
	adds	r7, r7, r3
	ldr	r3, [r2, #12]
	adds	r0, r0, r3
	movs	r3, #48
	add	lr, r3
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_0801032a
	ldr	r5, [pc, #232]
	adds	r1, r7, r5
.L_0801032a:
	asrs	r7, r1, #19
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L_08010336
	ldr	r3, [pc, #220]
	adds	r2, r0, r3
.L_08010336:
	ldr	r5, [sp, #28]
	lsls	r3, r5, #11
	ldr	r5, [pc, #216]
	asrs	r0, r2, #19
	adds	r5, r3, r5
	lsrs	r3, r2, #31
	adds	r3, r0, r3
	movs	r2, #127
	asrs	r3, r3, #1
	ands	r3, r2
	lsls	r6, r3, #7
	movs	r2, #0
	movs	r3, #30
	lsrs	r4, r4, #1
	ands	r3, r0
	mov	r8, r2
	mov	fp, r4
	str	r5, [sp, #24]
	lsls	r5, r3, #5
	cmp	r8, fp
	bcs.n	.L_080103da
	lsrs	r3, r1, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	str	r3, [sp, #8]
	movs	r1, #30
	movs	r3, #127
	mov	r9, r3
	mov	sl, r1
.L_08010370:
	ldr	r4, [sp, #8]
	mov	r2, r9
	adds	r0, r7, #0
	mov	r3, sl
	movs	r1, #0
	ands	r4, r2
	ands	r0, r3
	mov	ip, r1
.L_08010380:
	adds	r3, r6, r4
	ldr	r2, [pc, #148]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	ldr	r2, [pc, #144]
	lsls	r1, r1, #20
	lsrs	r1, r1, #17
	str	r1, [sp, #4]
	adds	r3, r1, r2
	ldr	r1, [sp, #24]
	adds	r2, r5, r0
	lsls	r2, r2, #1
	adds	r2, r2, r1
	str	r2, [sp, #0]
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	ldr	r2, [sp, #4]
	ldr	r1, [pc, #120]
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #0]
	str	r3, [r2, #64]
	movs	r2, #1
	mov	r3, r9
	adds	r4, #1
	add	ip, r2
	ands	r4, r3
	adds	r0, #2
	mov	r1, sl
	mov	r3, ip
	ands	r0, r1
	cmp	r3, #15
	bls.n	.L_08010380
	movs	r3, #254
	lsls	r3, r3, #6
	adds	r6, #128
	ands	r6, r3
	movs	r3, #240
	adds	r5, #64
	lsls	r3, r3, #2
	add	r8, r2
	ands	r5, r3
	cmp	r8, fp
	bcc.n	.L_08010370
.L_080103da:
	ldr	r5, [sp, #28]
	adds	r5, #1
	str	r5, [sp, #28]
	cmp	r5, #2
	bhi.n	.L_080103e6
	b.n	.L_080102a6
.L_080103e6:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0xffa00000
	.4byte 0xff880000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x03000118
	.4byte 0x0007ffff
	.4byte 0x06002800
	.4byte 0x02010000
	.4byte 0x02020000
	.2byte 0x0004
	.2byte 0x0202
.L_08010424:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	mov	fp, r2
	lsls	r1, r1, #7
	lsls	r3, r4, #7
	ldr	r2, [pc, #260]
	adds	r1, r1, r0
	add	r3, fp
	lsls	r1, r1, #2
	lsls	r3, r3, #2
	sub	sp, #36
	adds	r3, r3, r2
	adds	r1, r1, r2
	str	r3, [sp, #4]
	str	r1, [sp, #8]
	ldr	r3, [pc, #244]
	movs	r0, #130
	ldr	r3, [r3, #0]
	lsls	r0, r0, #1
	adds	r2, r3, r0
	add	r0, sp, #12
	mov	r9, r0
	movs	r6, #2
.L_0801045e:
	ldr	r3, [r2, #0]
	asrs	r3, r3, #20
	str	r3, [r0, #0]
	ldr	r3, [r2, #4]
	subs	r6, #1
	asrs	r3, r3, #20
	str	r3, [r0, #4]
	adds	r2, #48
	adds	r0, #8
	cmp	r6, #0
	bge.n	.L_0801045e
	ldr	r3, [sp, #72]
	adds	r7, r4, #0
	adds	r3, r7, r3
	cmp	r7, r3
	bcs.n	.L_0801052c
	ldr	r1, [sp, #68]
	str	r3, [sp, #0]
	movs	r3, #128
	subs	r3, r3, r1
	lsls	r3, r3, #2
	mov	r8, r3
.L_0801048a:
	ldr	r2, [sp, #68]
	mov	r1, fp
	adds	r3, r1, r2
	cmp	r1, r3
	bcs.n	.L_08010518
	mov	ip, r7
	mov	r4, ip
	mov	lr, r3
	movs	r3, #15
	ands	r4, r3
	mov	sl, r3
	mov	ip, r4
.L_080104a2:
	ldr	r2, [sp, #8]
	ldmia	r2!, {r5}
	adds	r0, r2, #0
	ldr	r4, [sp, #4]
	str	r0, [sp, #8]
	ldr	r3, [pc, #152]
	ldr	r2, [pc, #156]
	ands	r5, r3
	ldr	r3, [r4, #0]
	ands	r3, r2
	orrs	r3, r5
	stmia	r4!, {r3}
	adds	r2, r1, #0
	adds	r0, r4, #0
	mov	r3, sl
	mov	r4, ip
	ands	r2, r3
	lsls	r3, r4, #5
	adds	r3, r3, r2
	str	r0, [sp, #4]
	movs	r6, #0
	mov	r0, r9
	lsls	r4, r3, #2
.L_080104d0:
	ldr	r3, [r0, #0]
	cmp	r3, r1
	bgt.n	.L_08010504
	adds	r3, #16
	cmp	r3, r1
	ble.n	.L_08010504
	ldr	r3, [r0, #4]
	cmp	r3, r7
	bgt.n	.L_08010504
	adds	r3, #12
	cmp	r3, r7
	ble.n	.L_08010504
	lsls	r3, r5, #3
	ldr	r2, [pc, #100]
	ldr	r5, [pc, #100]
	adds	r0, r4, r2
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r0, #0]
	ldr	r0, [pc, #96]
	adds	r2, r3, r0
	ldr	r3, [pc, #96]
	adds	r0, r4, r3
	ldr	r3, [r2, #0]
	str	r3, [r0, #0]
	b.n	.L_08010512
.L_08010504:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r6, #1
	adds	r4, r4, r2
	adds	r0, #8
	cmp	r6, #2
	ble.n	.L_080104d0
.L_08010512:
	adds	r1, #1
	cmp	r1, lr
	bcc.n	.L_080104a2
.L_08010518:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	ldr	r5, [sp, #0]
	add	r3, r8
	add	r4, r8
	adds	r7, #1
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	cmp	r7, r5
	bcc.n	.L_0801048a
.L_0801052c:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x03001e70
	.4byte 0x00000fff
	.4byte 0xfffff000
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.2byte 0x2840
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r0, [r7, #0]
	ldr	r3, [pc, #96]
	mov	ip, r0
	sub	sp, #8
	mov	sl, r1
	mov	r8, r2
	cmp	ip, r3
	beq.n	.L_080105be
	mov	r9, r3
	adds	r6, r7, #2
.L_08010580:
	movs	r2, #0
	ldrsh	r1, [r6, r2]
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	movs	r4, #4
	ldrsh	r2, [r6, r4]
	movs	r4, #6
	ldrsh	r5, [r6, r4]
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	lsls	r1, r1, #16
	lsls	r5, r5, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	lsrs	r1, r1, #16
	mov	r2, sl
	mov	r3, r8
	lsrs	r5, r5, #16
	bl	.L_08010424
	adds	r7, #10
	adds	r0, r5, #0
	bl	sub_080030f8
	ldrh	r0, [r7, #0]
	mov	ip, r0
	adds	r6, #10
	cmp	ip, r9
	bne.n	.L_08010580
.L_080105be:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	ldr	r6, [sp, #72]
	lsls	r1, r1, #7
	adds	r1, r1, r0
	ldr	r0, [sp, #68]
	adds	r4, r3, #0
	lsls	r3, r6, #7
	adds	r3, r3, r0
	mov	fp, r2
	ldr	r2, [pc, #240]
	lsls	r1, r1, #2
	lsls	r3, r3, #2
	adds	r1, r1, r2
	adds	r3, r3, r2
	str	r1, [sp, #8]
	str	r3, [sp, #4]
	ldr	r3, [pc, #232]
	movs	r1, #130
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	add	r0, sp, #12
	adds	r2, r3, r1
	mov	r9, r0
	movs	r5, #2
.L_08010612:
	ldr	r3, [r2, #0]
	asrs	r3, r3, #20
	str	r3, [r0, #0]
	ldr	r3, [r2, #4]
	subs	r5, #1
	asrs	r3, r3, #20
	str	r3, [r0, #4]
	adds	r2, #48
	adds	r0, #8
	cmp	r5, #0
	bge.n	.L_08010612
	adds	r3, r6, r4
	cmp	r6, r3
	bge.n	.L_080106d4
	str	r3, [sp, #0]
	mov	r2, fp
	movs	r3, #128
	subs	r3, r3, r2
	lsls	r3, r3, #2
	mov	r8, r3
.L_0801063a:
	ldr	r1, [sp, #68]
	mov	r4, fp
	adds	r3, r1, r4
	cmp	r1, r3
	bge.n	.L_080106c0
	mov	ip, r6
	movs	r5, #15
	mov	r0, ip
	ands	r0, r5
	mov	lr, r3
	mov	sl, r5
	mov	ip, r0
.L_08010652:
	ldr	r4, [sp, #8]
	ldmia	r4!, {r3}
	ldr	r0, [sp, #4]
	ldr	r7, [pc, #148]
	adds	r2, r4, #0
	str	r2, [sp, #8]
	mov	r4, ip
	stmia	r0!, {r3}
	ands	r7, r3
	adds	r2, r1, #0
	mov	r3, sl
	ands	r2, r3
	lsls	r3, r4, #5
	adds	r5, r0, #0
	adds	r3, r3, r2
	str	r5, [sp, #4]
	mov	r0, r9
	movs	r5, #0
	lsls	r4, r3, #2
.L_08010678:
	ldr	r3, [r0, #0]
	cmp	r3, r1
	bgt.n	.L_080106ac
	adds	r3, #16
	cmp	r3, r1
	ble.n	.L_080106ac
	ldr	r3, [r0, #4]
	cmp	r3, r6
	bgt.n	.L_080106ac
	adds	r3, #12
	cmp	r3, r6
	ble.n	.L_080106ac
	ldr	r5, [pc, #96]
	adds	r0, r4, r5
	ldr	r5, [pc, #96]
	lsls	r3, r7, #3
	adds	r2, r3, r5
	ldr	r2, [r2, #0]
	str	r2, [r0, #0]
	ldr	r0, [pc, #92]
	adds	r2, r3, r0
	ldr	r3, [pc, #92]
	adds	r0, r4, r3
	ldr	r3, [r2, #0]
	str	r3, [r0, #0]
	b.n	.L_080106ba
.L_080106ac:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r5, #1
	adds	r4, r4, r2
	adds	r0, #8
	cmp	r5, #2
	ble.n	.L_08010678
.L_080106ba:
	adds	r1, #1
	cmp	r1, lr
	blt.n	.L_08010652
.L_080106c0:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	ldr	r5, [sp, #0]
	add	r3, r8
	add	r4, r8
	adds	r6, #1
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	cmp	r6, r5
	blt.n	.L_0801063a
.L_080106d4:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x03001e70
	.4byte 0x00000fff
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.4byte 0x06002840
