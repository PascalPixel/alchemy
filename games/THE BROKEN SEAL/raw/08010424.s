.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.global Overlay_08010424
Overlay_08010424:
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
