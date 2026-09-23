.syntax unified
	.thumb
	.set sub_080048b0, 0x080048b0
	.global Unnamed_080ed408
	.global Func_080ed408
	.thumb_func
Unnamed_080ed408:
Func_080ed408:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	r9, r1
	ldr	r1, [sp, #48]
	mov	sl, r1
	str	r2, [sp, #12]
	mov	r2, sl
	mov	r8, r3
	movs	r1, #3
	cmp	r2, #3
	bne.n	.L_080ed42c
	movs	r1, #6
.L_080ed42c:
	movs	r3, #12
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #4
	bne.n	.L_080ed438
	adds	r1, #3
.L_080ed438:
	cmp	r3, #8
	bne.n	.L_080ed43e
	adds	r1, #4
.L_080ed43e:
	cmp	r3, #12
	bne.n	.L_080ed444
	adds	r1, #3
.L_080ed444:
	cmp	r3, #0
	bne.n	.L_080ed44a
	adds	r1, #1
.L_080ed44a:
	movs	r3, #2
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed456
	adds	r1, #7
.L_080ed456:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #2
	cmp	r3, #0
	bne.n	.L_080ed464
	adds	r1, #1
.L_080ed464:
	cmp	r3, #0
	beq.n	.L_080ed46e
	adds	r1, #2
	adds	r1, #2
	adds	r1, #5
.L_080ed46e:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #9
	cmp	r3, #0
	bne.n	.L_080ed47c
	adds	r1, #1
.L_080ed47c:
	adds	r1, #9
	cmp	r3, #0
	bne.n	.L_080ed484
	adds	r1, #1
.L_080ed484:
	mov	r3, sl
	adds	r1, #3
	cmp	r3, #1
	beq.n	.L_080ed49c
	cmp	r3, #1
	bcc.n	.L_080ed498
	cmp	r3, #2
	beq.n	.L_080ed49c
	cmp	r3, #3
	beq.n	.L_080ed4a0
.L_080ed498:
	adds	r1, #2
	b.n	.L_080ed4a2
.L_080ed49c:
	adds	r1, #4
	b.n	.L_080ed4a2
.L_080ed4a0:
	adds	r1, #6
.L_080ed4a2:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #3
	cmp	r3, #0
	bne.n	.L_080ed4b0
	adds	r1, #2
.L_080ed4b0:
	mov	r3, sl
	adds	r1, #2
	cmp	r3, #1
	beq.n	.L_080ed4d0
	cmp	r3, #1
	bcc.n	.L_080ed4c4
	cmp	r3, #2
	beq.n	.L_080ed4d4
	cmp	r3, #3
	beq.n	.L_080ed4d8
.L_080ed4c4:
	movs	r3, #0
.L_080ed4c6:
	adds	r3, #1
	adds	r1, #2
	cmp	r3, #7
	ble.n	.L_080ed4c6
	b.n	.L_080ed4da
.L_080ed4d0:
	adds	r1, #25
	b.n	.L_080ed4da
.L_080ed4d4:
	adds	r1, #32
	b.n	.L_080ed4da
.L_080ed4d8:
	adds	r1, #28
.L_080ed4da:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #1
	cmp	r3, #0
	bne.n	.L_080ed4e8
	adds	r1, #2
.L_080ed4e8:
	mov	r3, sl
	adds	r1, #5
	cmp	r3, #1
	beq.n	.L_080ed500
	cmp	r3, #1
	bcc.n	.L_080ed4fc
	cmp	r3, #2
	beq.n	.L_080ed500
	cmp	r3, #3
	beq.n	.L_080ed504
.L_080ed4fc:
	adds	r1, #2
	b.n	.L_080ed506
.L_080ed500:
	adds	r1, #4
	b.n	.L_080ed506
.L_080ed504:
	adds	r1, #6
.L_080ed506:
	adds	r1, #8
	lsls	r1, r1, #2
	bl	sub_080048b0
	ldr	r6, [pc, #796]
	adds	r5, r0, #0
	ldr	r3, [pc, #796]
	ldr	r2, [pc, #796]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
	adds	r6, #12
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L_080ed534
	ldr	r0, [pc, #780]
	adds	r1, r5, #0
	ldr	r2, [pc, #772]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L_080ed534:
	movs	r4, #12
	mov	r2, r8
	ands	r4, r2
	cmp	r4, #4
	bne.n	.L_080ed54c
	ldr	r3, [pc, #752]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #748]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L_080ed54c:
	adds	r6, #12
	cmp	r4, #8
	bne.n	.L_080ed560
	ldr	r3, [pc, #732]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #736]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
.L_080ed560:
	adds	r6, #16
	cmp	r4, #12
	bne.n	.L_080ed574
	ldr	r3, [pc, #712]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #708]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L_080ed574:
	adds	r6, #12
	cmp	r4, #0
	bne.n	.L_080ed57e
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L_080ed57e:
	movs	r3, #2
	mov	r1, r8
	ands	r3, r1
	adds	r6, #4
	cmp	r3, #0
	beq.n	.L_080ed5bc
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed59e
	ldr	r3, [r6, #8]
	b.n	.L_080ed5a0
.L_080ed59e:
	ldr	r3, [r6, #12]
.L_080ed5a0:
	stmia	r5!, {r3}
	ldr	r3, [r6, #16]
	stmia	r5!, {r3}
	ldr	r3, [r6, #20]
	stmia	r5!, {r3}
	movs	r2, #1
	ldr	r3, [sp, #12]
	lsls	r2, r3
	ldr	r3, [r6, #24]
	adds	r3, r3, r2
	stmia	r5!, {r3}
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	stmia	r5!, {r3}
.L_080ed5bc:
	adds	r6, #32
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	str	r5, [sp, #8]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r2, #1
	mov	r1, r8
	ands	r1, r2
	cmp	r1, #0
	bne.n	.L_080ed5de
	mov	r3, r9
	lsls	r2, r3
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r5!, {r3}
.L_080ed5de:
	adds	r6, #4
	cmp	r1, #0
	beq.n	.L_080ed638
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed5fe
	ldr	r3, [r6, #8]
	stmia	r5!, {r3}
	ldr	r3, [r6, #12]
	b.n	.L_080ed604
.L_080ed5fe:
	ldr	r3, [r6, #16]
	stmia	r5!, {r3}
	ldr	r3, [r6, #20]
.L_080ed604:
	stmia	r5!, {r3}
	ldr	r3, [r6, #24]
	stmia	r5!, {r3}
	ldr	r3, [r6, #28]
	stmia	r5!, {r3}
	ldr	r3, [pc, #560]
	mov	r2, r9
	lsls	r1, r2, #1
	ldrh	r2, [r3, r1]
	ldr	r3, [r6, #32]
	adds	r3, r3, r2
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed62a
	ldr	r3, [r6, #36]
	b.n	.L_080ed62c
.L_080ed62a:
	ldr	r3, [r6, #40]
.L_080ed62c:
	stmia	r5!, {r3}
	ldr	r3, [pc, #528]
	ldrh	r2, [r3, r1]
	ldr	r3, [r6, #44]
	adds	r3, r3, r2
	stmia	r5!, {r3}
.L_080ed638:
	adds	r6, #48
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	fp, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldr	r3, [pc, #488]
	ldr	r2, [pc, #508]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
	adds	r6, #24
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r7, #1
	mov	r4, r8
	ands	r4, r7
	cmp	r4, #0
	bne.n	.L_080ed666
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L_080ed666:
	adds	r6, #4
	ldr	r3, [pc, #452]
	ldr	r2, [pc, #476]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #20
	adds	r6, #20
	str	r5, [sp, #4]
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [sp, #12]
	subs	r3, #3
	adds	r2, r7, #0
	lsls	r2, r3
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r5!, {r3}
	mov	r2, r9
	ldr	r3, [r6, #8]
	subs	r2, #3
	lsls	r2, r2, #7
	adds	r3, r3, r2
	stmia	r5!, {r3}
	ldr	r3, [r6, #12]
	stmia	r5!, {r3}
	adds	r6, #16
	cmp	r4, #0
	bne.n	.L_080ed6a8
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L_080ed6a8:
	adds	r6, #4
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed6c2
	ldr	r3, [r6, #0]
	b.n	.L_080ed6c4
.L_080ed6c2:
	ldr	r3, [r6, #4]
.L_080ed6c4:
	stmia	r5!, {r3}
	mov	r2, sl
	adds	r6, #8
	adds	r4, r5, #0
	cmp	r2, #1
	beq.n	.L_080ed6fc
	cmp	r2, #1
	bcc.n	.L_080ed6dc
	cmp	r2, #2
	beq.n	.L_080ed712
	cmp	r2, #3
	beq.n	.L_080ed732
.L_080ed6dc:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed6ec
	ldr	r3, [pc, #328]
	ldr	r0, [pc, #352]
	b.n	.L_080ed6f0
.L_080ed6ec:
	ldr	r3, [pc, #320]
	ldr	r0, [pc, #352]
.L_080ed6f0:
	adds	r1, r5, #0
	ldr	r2, [pc, #352]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	b.n	.L_080ed750
.L_080ed6fc:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed70c
	ldr	r3, [pc, #296]
	ldr	r0, [pc, #332]
	b.n	.L_080ed726
.L_080ed70c:
	ldr	r3, [pc, #288]
	ldr	r0, [pc, #332]
	b.n	.L_080ed726
.L_080ed712:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed722
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #320]
	b.n	.L_080ed726
.L_080ed722:
	ldr	r3, [pc, #268]
	ldr	r0, [pc, #316]
.L_080ed726:
	adds	r1, r5, #0
	ldr	r2, [pc, #272]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
	b.n	.L_080ed750
.L_080ed732:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed742
	ldr	r3, [pc, #240]
	ldr	r0, [pc, #296]
	b.n	.L_080ed746
.L_080ed742:
	ldr	r3, [pc, #236]
	ldr	r0, [pc, #292]
.L_080ed746:
	adds	r1, r5, #0
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
.L_080ed750:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	subs	r3, r4, r5
	subs	r3, #8
	ldr	r1, [pc, #276]
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r5!, {r2}
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ed77c
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
.L_080ed77c:
	adds	r6, #8
	mov	r2, ip
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	str	r5, [sp, #0]
	mov	r1, sl
	cmp	r1, #1
	beq.n	.L_080ed7de
	cmp	r1, #1
	bcc.n	.L_080ed7ae
	cmp	r1, #2
	beq.n	.L_080ed7fe
	cmp	r1, #3
	beq.n	.L_080ed88c
.L_080ed7ae:
	ldr	r3, [pc, #156]
	ldr	r1, [pc, #156]
	movs	r7, #4
	mov	r2, r8
	movs	r4, #0
	ands	r7, r2
	mov	r9, r3
	mov	lr, r1
.L_080ed7be:
	cmp	r7, #0
	beq.n	.L_080ed7c8
	ldr	r3, [pc, #108]
	mov	r0, r9
	b.n	.L_080ed7cc
.L_080ed7c8:
	ldr	r3, [pc, #100]
	mov	r0, lr
.L_080ed7cc:
	adds	r1, r5, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	adds	r4, #1
	cmp	r4, #7
	ble.n	.L_080ed7be
	b.n	.L_080ed8b6
.L_080ed7de:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed7ee
	ldr	r3, [pc, #68]
	ldr	r0, [pc, #136]
	b.n	.L_080ed7f2
.L_080ed7ee:
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #132]
.L_080ed7f2:
	adds	r1, r5, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #100
	b.n	.L_080ed8b6
.L_080ed7fe:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed80c
	ldr	r4, [pc, #116]
	b.n	.L_080ed80e
.L_080ed80c:
	ldr	r4, [pc, #116]
.L_080ed80e:
	ldr	r3, [pc, #32]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #64
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #64
	b.n	.L_080ed8b6
	movs	r0, r0
	.4byte 0x080edcc4
	.4byte 0x040000d4
	.4byte 0x84000003
	.4byte 0x080edcb8
	.4byte 0x84000004
	.4byte 0x080ef034
	.4byte 0x84000006
	.4byte 0x84000005
	.4byte 0x080edaf0
	.4byte 0x080edaf8
	.4byte 0x84000002
	.4byte 0x080edb10
	.4byte 0x080edb00
	.4byte 0x080edbf8
	.4byte 0x080edbe8
	.4byte 0x080edca0
	.4byte 0x080edc88
	.4byte 0x00ffffff
	.4byte 0x080edb84
	.4byte 0x080edb20
	.4byte 0x84000019
	.4byte 0x080edc48
	.4byte 0x080edc08
	.2byte 0x0010
	.2byte 0x8400
.L_080ed88c:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed89a
	ldr	r4, [pc, #408]
	b.n	.L_080ed89c
.L_080ed89a:
	ldr	r4, [pc, #408]
.L_080ed89c:
	ldr	r3, [pc, #408]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #408]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #56
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #396]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #56
.L_080ed8b6:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080ed8ce
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
.L_080ed8ce:
	adds	r6, #8
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldr	r2, [sp, #0]
	subs	r3, r2, r5
	ldr	r1, [pc, #356]
	subs	r3, #8
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r5!, {r2}
	mov	r2, ip
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed910
	ldr	r3, [r6, #0]
	b.n	.L_080ed912
.L_080ed910:
	ldr	r3, [r6, #4]
.L_080ed912:
	stmia	r5!, {r3}
	mov	r2, sl
	adds	r6, #8
	adds	r4, r5, #0
	cmp	r2, #1
	beq.n	.L_080ed94a
	cmp	r2, #1
	bcc.n	.L_080ed92a
	cmp	r2, #2
	beq.n	.L_080ed960
	cmp	r2, #3
	beq.n	.L_080ed980
.L_080ed92a:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed93a
	ldr	r3, [pc, #256]
	ldr	r0, [pc, #268]
	b.n	.L_080ed93e
.L_080ed93a:
	ldr	r3, [pc, #252]
	ldr	r0, [pc, #264]
.L_080ed93e:
	adds	r1, r5, #0
	ldr	r2, [pc, #264]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	b.n	.L_080ed99e
.L_080ed94a:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed95a
	ldr	r3, [pc, #224]
	ldr	r0, [pc, #248]
	b.n	.L_080ed974
.L_080ed95a:
	ldr	r3, [pc, #220]
	ldr	r0, [pc, #244]
	b.n	.L_080ed974
.L_080ed960:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ed970
	ldr	r3, [pc, #204]
	ldr	r0, [pc, #232]
	b.n	.L_080ed974
.L_080ed970:
	ldr	r3, [pc, #196]
	ldr	r0, [pc, #232]
.L_080ed974:
	adds	r1, r5, #0
	ldr	r2, [pc, #232]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
	b.n	.L_080ed99e
.L_080ed980:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ed990
	ldr	r3, [pc, #172]
	ldr	r0, [pc, #212]
	b.n	.L_080ed994
.L_080ed990:
	ldr	r3, [pc, #164]
	ldr	r0, [pc, #212]
.L_080ed994:
	adds	r1, r5, #0
	ldr	r2, [pc, #212]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
.L_080ed99e:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	subs	r3, r4, r5
	subs	r3, #8
	ldr	r4, [pc, #148]
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r4
	adds	r2, r2, r3
	stmia	r5!, {r2}
	mov	r1, ip
	subs	r3, r5, r1
	subs	r3, #8
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	str	r2, [r1, #0]
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #168]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
	adds	r6, #12
	ldr	r2, [sp, #4]
	subs	r3, r2, r5
	subs	r3, #8
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r4
	adds	r2, r2, r3
	stmia	r5!, {r2}
	ldr	r1, [sp, #8]
	subs	r3, r5, r1
	subs	r3, #8
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	str	r2, [r1, #0]
	mov	r2, fp
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	mov	r3, fp
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldmia	r6!, {r3}
	str	r3, [r5, #0]
	ldr	r3, [pc, #100]
	eors	r3, r6
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	movs	r0, #1
	subs	r0, r0, r2
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080eefdc
	.4byte 0x080eefa4
	.4byte 0x040000d4
	.4byte 0x8400000e
	.4byte 0x00ffffff
	.4byte 0x080edaf0
	.4byte 0x080edaf8
	.4byte 0x84000002
	.4byte 0x080edb10
	.4byte 0x080edb00
	.4byte 0x080edbf8
	.4byte 0x080edbe8
	.4byte 0x84000004
	.4byte 0x080edca0
	.4byte 0x080edc88
	.4byte 0x84000006
	.4byte 0x84000003
	.4byte 0x080ede48
