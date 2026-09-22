.syntax unified
	.thumb
	.set sub_08014cc0, 0x08014cc0
	.global Overlay_081963ec
Overlay_081963ec:
	push	{lr}
	movs	r3, #15
	sub	sp, #4
	ands	r3, r1
	lsrs	r1, r1, #4
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	bl	.L_08196404
	add	sp, #4
	pop	{pc}
.L_08196404:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #152
	mov	sl, r1
	ldr	r1, [sp, #184]
	movs	r6, #218
	lsls	r6, r6, #1
	mov	r9, r1
	adds	r1, r6, #0
	str	r2, [sp, #12]
	mov	r8, r3
	ldr	r7, [pc, #804]
	bl	sub_08014cc0
	adds	r6, r0, #0
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4, r5}
	stmia	r1!, {r2, r3, r4, r5}
	adds	r6, #16
	adds	r7, #16
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_08196448
	ldr	r0, [pc, #784]
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4}
	stmia	r1!, {r2, r3, r4}
	adds	r6, #12
.L_08196448:
	movs	r3, #12
	mov	r1, r8
	ands	r1, r3
	mov	ip, r1
	cmp	r1, #4
	bne.n	.L_0819645e
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4}
	stmia	r1!, {r2, r3, r4}
	adds	r6, #12
.L_0819645e:
	mov	r2, ip
	adds	r7, #12
	cmp	r2, #8
	bne.n	.L_08196470
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4, r5}
	stmia	r1!, {r2, r3, r4, r5}
	adds	r6, #16
.L_08196470:
	mov	r3, ip
	adds	r7, #16
	cmp	r3, #12
	bne.n	.L_08196482
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4}
	stmia	r1!, {r2, r3, r4}
	adds	r6, #12
.L_08196482:
	mov	r1, ip
	adds	r7, #12
	cmp	r1, #0
	bne.n	.L_0819648e
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
.L_0819648e:
	movs	r3, #2
	mov	r2, r8
	ands	r3, r2
	adds	r7, #4
	cmp	r3, #0
	beq.n	.L_081964da
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3}
	stmia	r1!, {r2, r3}
	adds	r6, #8
	adds	r7, #8
	movs	r3, #8
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_081964b4
	ldr	r3, [r7, #0]
	b.n	.L_081964b6
.L_081964b4:
	ldr	r3, [r7, #4]
.L_081964b6:
	stmia	r6!, {r3}
	adds	r7, #8
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3}
	stmia	r1!, {r2, r3}
	adds	r6, #8
	adds	r7, #8
	movs	r3, #1
	ldr	r2, [sp, #12]
	lsls	r3, r2
	ldr	r2, [r7, #0]
	adds	r2, r2, r3
	stmia	r6!, {r2}
	ldr	r2, [r7, #4]
	adds	r2, r2, r3
	stmia	r6!, {r2}
	subs	r7, #24
.L_081964da:
	adds	r7, #32
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	str	r6, [sp, #8]
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	movs	r2, #1
	mov	r1, r8
	ands	r1, r2
	cmp	r1, #0
	bne.n	.L_081964fc
	mov	r3, sl
	lsls	r2, r3
	ldr	r3, [r7, #0]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r6!, {r3}
.L_081964fc:
	adds	r7, #4
	cmp	r1, #0
	beq.n	.L_08196556
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
	ldr	r3, [r7, #4]
	stmia	r6!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0819651c
	ldr	r3, [r7, #8]
	stmia	r6!, {r3}
	ldr	r3, [r7, #12]
	b.n	.L_08196522
.L_0819651c:
	ldr	r3, [r7, #16]
	stmia	r6!, {r3}
	ldr	r3, [r7, #20]
.L_08196522:
	stmia	r6!, {r3}
	ldr	r3, [r7, #24]
	stmia	r6!, {r3}
	ldr	r3, [r7, #28]
	stmia	r6!, {r3}
	ldr	r3, [pc, #548]
	mov	r2, sl
	lsls	r1, r2, #1
	ldrh	r2, [r3, r1]
	ldr	r3, [r7, #32]
	adds	r3, r3, r2
	stmia	r6!, {r3}
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08196548
	ldr	r3, [r7, #36]
	b.n	.L_0819654a
.L_08196548:
	ldr	r3, [r7, #40]
.L_0819654a:
	stmia	r6!, {r3}
	ldr	r3, [pc, #516]
	ldrh	r2, [r3, r1]
	ldr	r3, [r7, #44]
	adds	r3, r3, r2
	stmia	r6!, {r3}
.L_08196556:
	adds	r7, #48
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	mov	fp, r6
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	movs	r5, #132
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	lsls	r5, r5, #24
	adds	r3, #212
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #24
	adds	r7, #24
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	movs	r3, #1
	mov	r4, r8
	ands	r4, r3
	mov	ip, r3
	cmp	r4, #0
	bne.n	.L_08196592
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
.L_08196592:
	adds	r7, #4
	movs	r2, #5
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r7, #0
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #20
	adds	r7, #20
	str	r6, [sp, #4]
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
	ldr	r3, [sp, #12]
	subs	r3, #3
	mov	r2, ip
	lsls	r2, r3
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r6!, {r3}
	mov	r2, sl
	ldr	r3, [r7, #8]
	subs	r2, #3
	lsls	r2, r2, #7
	adds	r3, r3, r2
	stmia	r6!, {r3}
	ldr	r3, [r7, #12]
	stmia	r6!, {r3}
	adds	r7, #16
	cmp	r4, #0
	bne.n	.L_081965da
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
.L_081965da:
	adds	r1, r6, #4
	adds	r7, #4
	mov	ip, r1
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3}
	stmia	r1!, {r2, r3}
	adds	r6, #8
	adds	r7, #8
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081965fa
	ldr	r3, [r7, #0]
	b.n	.L_081965fc
.L_081965fa:
	ldr	r3, [r7, #4]
.L_081965fc:
	stmia	r6!, {r3}
	mov	r3, r9
	adds	r7, #8
	mov	lr, r6
	mov	sl, r6
	cmp	r3, #1
	beq.n	.L_0819663e
	cmp	r3, #1
	bcc.n	.L_08196658
	cmp	r3, #2
	beq.n	.L_0819662c
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08196620
	ldr	r0, [pc, #312]
	b.n	.L_08196622
.L_08196620:
	ldr	r0, [pc, #312]
.L_08196622:
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3}
	stmia	r1!, {r2, r3}
	adds	r6, #8
	b.n	.L_08196686
.L_0819662c:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0819663a
	ldr	r0, [pc, #296]
	b.n	.L_0819664e
.L_0819663a:
	ldr	r0, [pc, #296]
	b.n	.L_0819664e
.L_0819663e:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0819664c
	ldr	r0, [pc, #284]
	b.n	.L_0819664e
.L_0819664c:
	ldr	r0, [pc, #284]
.L_0819664e:
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4, r5}
	stmia	r1!, {r2, r3, r4, r5}
	adds	r6, #16
	b.n	.L_08196686
.L_08196658:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08196670
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #256]
	b.n	.L_0819667c
.L_08196670:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #248]
.L_0819667c:
	adds	r1, r6, #0
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #24
.L_08196686:
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	mov	r1, lr
	subs	r3, r1, r6
	ldr	r1, [pc, #232]
	ldmia	r7!, {r2}
	subs	r3, #8
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r6!, {r2}
	mov	r2, sl
	subs	r2, r6, r2
	str	r2, [sp, #0]
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081966b8
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
	ldr	r3, [r7, #4]
	stmia	r6!, {r3}
.L_081966b8:
	adds	r7, #8
	mov	r2, ip
	subs	r3, r6, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	adds	r1, r6, #4
	mov	ip, r1
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3}
	stmia	r1!, {r2, r3}
	adds	r6, #8
	adds	r7, #8
	mov	lr, r6
	mov	r2, r9
	cmp	r2, #1
	beq.n	.L_0819678c
	cmp	r2, #1
	bcc.n	.L_081967c2
	cmp	r2, #2
	beq.n	.L_0819671a
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08196708
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #120]
	adds	r1, r6, #0
	adds	r2, #16
	b.n	.L_081967ba
.L_08196708:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #108]
	adds	r1, r6, #0
	adds	r2, #16
	b.n	.L_081967ba
.L_0819671a:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08196732
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #84]
	b.n	.L_0819673e
.L_08196732:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #72]
.L_0819673e:
	adds	r1, r6, #0
	adds	r2, #25
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #100
	b.n	.L_081967f6
	movs	r0, r0
	.4byte 0x081972a0
	.4byte 0x08197224
	.4byte 0x081973f0
	.4byte 0x08196fdc
	.4byte 0x08196fe4
	.4byte 0x0819707c
	.4byte 0x0819706c
	.4byte 0x08197164
	.4byte 0x08197154
	.4byte 0x0819720c
	.4byte 0x081971f4
	.4byte 0x00ffffff
	.4byte 0x0819702c
	.4byte 0x08196fec
	.4byte 0x081970f0
	.2byte 0x708c
	.2byte 0x0819
.L_0819678c:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0819679a
	ldr	r4, [pc, #348]
	b.n	.L_0819679c
.L_0819679a:
	ldr	r4, [pc, #348]
.L_0819679c:
	movs	r5, #132
	lsls	r5, r5, #24
	movs	r3, #128
	adds	r5, #16
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #64
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
.L_081967ba:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #64
	b.n	.L_081967f6
.L_081967c2:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081967d0
	ldr	r4, [pc, #300]
	b.n	.L_081967d2
.L_081967d0:
	ldr	r4, [pc, #300]
.L_081967d2:
	movs	r5, #132
	lsls	r5, r5, #24
	movs	r3, #128
	adds	r5, #14
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #56
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #56
.L_081967f6:
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	movs	r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0819680c
	ldr	r3, [r7, #0]
	stmia	r6!, {r3}
	ldr	r3, [r7, #4]
	stmia	r6!, {r3}
.L_0819680c:
	adds	r7, #8
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	mov	r2, lr
	subs	r3, r2, r6
	ldr	r1, [pc, #236]
	ldmia	r7!, {r2}
	subs	r3, #8
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r6!, {r2}
	mov	r2, ip
	subs	r3, r6, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	mov	ip, r6
	ldmia	r7!, {r3}
	stmia	r6!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0819684e
	ldr	r3, [r7, #0]
	b.n	.L_08196850
.L_0819684e:
	ldr	r3, [r7, #4]
.L_08196850:
	stmia	r6!, {r3}
	adds	r7, #8
	ldr	r2, [sp, #0]
	lsrs	r3, r2, #2
	lsls	r2, r3, #2
	adds	r5, r2, #0
	cmp	r2, #0
	bge.n	.L_08196862
	adds	r2, #3
.L_08196862:
	movs	r4, #132
	movs	r3, #128
	lsls	r4, r4, #24
	asrs	r2, r2, #2
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r0, sl
	adds	r1, r6, #0
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, r6, r5
	mov	r1, ip
	subs	r3, r6, r1
	ldr	r5, [pc, #132]
	ldr	r2, [r1, #0]
	subs	r3, #8
	lsrs	r3, r3, #2
	ands	r3, r5
	orrs	r2, r3
	str	r2, [r1, #0]
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3, r4}
	stmia	r1!, {r2, r3, r4}
	adds	r6, #12
	adds	r7, #12
	ldr	r2, [sp, #4]
	subs	r3, r2, r6
	subs	r3, #8
	ldmia	r7!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r5
	adds	r2, r2, r3
	stmia	r6!, {r2}
	ldr	r1, [sp, #8]
	subs	r3, r6, r1
	ldr	r2, [r1, #0]
	subs	r3, #8
	lsrs	r3, r3, #2
	ands	r3, r5
	orrs	r2, r3
	str	r2, [r1, #0]
	mov	r2, fp
	subs	r3, r6, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r5
	orrs	r2, r3
	mov	r3, fp
	str	r2, [r3, #0]
	adds	r0, r7, #0
	adds	r1, r6, #0
	ldmia	r0!, {r2, r3}
	stmia	r1!, {r2, r3}
	adds	r2, r7, #0
	adds	r2, #8
	ldr	r3, [pc, #48]
	movs	r0, #1
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	subs	r0, r0, r3
	add	sp, #152
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x081971b4
	.4byte 0x08197174
	.4byte 0x08197268
	.4byte 0x08197230
	.4byte 0x00ffffff
	.4byte 0x081973f0
