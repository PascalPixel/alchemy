.syntax unified
	.thumb
	.global Func_08196404
	.thumb_func
Func_08196404:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #152
	mov r10, r1
	ldr r1, [sp, #184]
	movs r6, #218
	lsls r6, r6, #1
	mov r9, r1
	adds r1, r6, #0
	str r2, [sp, #12]
	mov r8, r3
	ldr r7, .L_0819674c
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4, r5}
	stmia r1!, {r2, r3, r4, r5}
	adds r6, #16
	adds r7, #16
	mov r2, r9
	cmp r2, #0
	bne .L_08196448
	ldr r0, .L_08196750
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4}
	stmia r1!, {r2, r3, r4}
	adds r6, #12
.L_08196448:
	movs r3, #12
	mov r1, r8
	ands r1, r3
	mov r12, r1
	cmp r1, #4
	bne .L_0819645e
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4}
	stmia r1!, {r2, r3, r4}
	adds r6, #12
.L_0819645e:
	mov r2, r12
	adds r7, #12
	cmp r2, #8
	bne .L_08196470
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4, r5}
	stmia r1!, {r2, r3, r4, r5}
	adds r6, #16
.L_08196470:
	mov r3, r12
	adds r7, #16
	cmp r3, #12
	bne .L_08196482
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4}
	stmia r1!, {r2, r3, r4}
	adds r6, #12
.L_08196482:
	mov r1, r12
	adds r7, #12
	cmp r1, #0
	bne .L_0819648e
	ldr r3, [r7]
	stmia r6!, {r3}
.L_0819648e:
	movs r3, #2
	mov r2, r8
	ands r3, r2
	adds r7, #4
	cmp r3, #0
	beq .L_081964da
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3}
	stmia r1!, {r2, r3}
	adds r6, #8
	adds r7, #8
	movs r3, #8
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_081964b4
	ldr r3, [r7]
	b .L_081964b6
.L_081964b4:
	ldr r3, [r7, #4]
.L_081964b6:
	stmia r6!, {r3}
	adds r7, #8
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3}
	stmia r1!, {r2, r3}
	adds r6, #8
	adds r7, #8
	movs r3, #1
	ldr r2, [sp, #12]
	lsls r3, r2
	ldr r2, [r7]
	adds r2, r2, r3
	stmia r6!, {r2}
	ldr r2, [r7, #4]
	adds r2, r2, r3
	stmia r6!, {r2}
	subs r7, #24
.L_081964da:
	adds r7, #32
	ldmia r7!, {r3}
	stmia r6!, {r3}
	str r6, [sp, #8]
	ldmia r7!, {r3}
	stmia r6!, {r3}
	movs r2, #1
	mov r1, r8
	ands r1, r2
	cmp r1, #0
	bne .L_081964fc
	mov r3, r10
	lsls r2, r3
	ldr r3, [r7]
	adds r3, r3, r2
	subs r3, #1
	stmia r6!, {r3}
.L_081964fc:
	adds r7, #4
	cmp r1, #0
	beq .L_08196556
	ldr r3, [r7]
	stmia r6!, {r3}
	ldr r3, [r7, #4]
	stmia r6!, {r3}
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0819651c
	ldr r3, [r7, #8]
	stmia r6!, {r3}
	ldr r3, [r7, #12]
	b .L_08196522
.L_0819651c:
	ldr r3, [r7, #16]
	stmia r6!, {r3}
	ldr r3, [r7, #20]
.L_08196522:
	stmia r6!, {r3}
	ldr r3, [r7, #24]
	stmia r6!, {r3}
	ldr r3, [r7, #28]
	stmia r6!, {r3}
	ldr r3, .L_08196754
	mov r2, r10
	lsls r1, r2, #1
	ldrh r2, [r3, r1]
	ldr r3, [r7, #32]
	adds r3, r3, r2
	stmia r6!, {r3}
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_08196548
	ldr r3, [r7, #36]
	b .L_0819654a
.L_08196548:
	ldr r3, [r7, #40]
.L_0819654a:
	stmia r6!, {r3}
	ldr r3, .L_08196754
	ldrh r2, [r3, r1]
	ldr r3, [r7, #44]
	adds r3, r3, r2
	stmia r6!, {r3}
.L_08196556:
	adds r7, #48
	ldmia r7!, {r3}
	stmia r6!, {r3}
	mov r11, r6
	ldmia r7!, {r3}
	stmia r6!, {r3}
	movs r5, #132
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	lsls r5, r5, #24
	adds r3, #212
	adds r0, r7, #0
	adds r1, r6, #0
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #24
	adds r7, #24
	ldmia r7!, {r3}
	stmia r6!, {r3}
	movs r3, #1
	mov r4, r8
	ands r4, r3
	mov r12, r3
	cmp r4, #0
	bne .L_08196592
	ldr r3, [r7]
	stmia r6!, {r3}
.L_08196592:
	adds r7, #4
	movs r2, #5
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r7, #0
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #20
	adds r7, #20
	str r6, [sp, #4]
	ldr r3, [r7]
	stmia r6!, {r3}
	ldr r3, [sp, #12]
	subs r3, #3
	mov r2, r12
	lsls r2, r3
	ldr r3, [r7, #4]
	adds r3, r3, r2
	subs r3, #1
	stmia r6!, {r3}
	mov r2, r10
	ldr r3, [r7, #8]
	subs r2, #3
	lsls r2, r2, #7
	adds r3, r3, r2
	stmia r6!, {r3}
	ldr r3, [r7, #12]
	stmia r6!, {r3}
	adds r7, #16
	cmp r4, #0
	bne .L_081965da
	ldr r3, [r7]
	stmia r6!, {r3}
.L_081965da:
	adds r1, r6, #4
	adds r7, #4
	mov r12, r1
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3}
	stmia r1!, {r2, r3}
	adds r6, #8
	adds r7, #8
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_081965fa
	ldr r3, [r7]
	b .L_081965fc
.L_081965fa:
	ldr r3, [r7, #4]
.L_081965fc:
	stmia r6!, {r3}
	mov r3, r9
	adds r7, #8
	mov lr, r6
	mov r10, r6
	cmp r3, #1
	beq .L_0819663e
	cmp r3, #1
	bcc .L_08196658
	cmp r3, #2
	beq .L_0819662c
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_08196620
	ldr r0, .L_08196758
	b .L_08196622
.L_08196620:
	ldr r0, .L_0819675c
.L_08196622:
	adds r1, r6, #0
	ldmia r0!, {r2, r3}
	stmia r1!, {r2, r3}
	adds r6, #8
	b .L_08196686
.L_0819662c:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0819663a
	ldr r0, .L_08196760
	b .L_0819664e
.L_0819663a:
	ldr r0, .L_08196764
	b .L_0819664e
.L_0819663e:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0819664c
	ldr r0, .L_08196768
	b .L_0819664e
.L_0819664c:
	ldr r0, .L_0819676c
.L_0819664e:
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4, r5}
	stmia r1!, {r2, r3, r4, r5}
	adds r6, #16
	b .L_08196686
.L_08196658:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_08196670
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08196770
	b .L_0819667c
.L_08196670:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08196774
.L_0819667c:
	adds r1, r6, #0
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #24
.L_08196686:
	ldmia r7!, {r3}
	stmia r6!, {r3}
	mov r1, lr
	subs r3, r1, r6
	ldr r1, .L_08196778
	ldmia r7!, {r2}
	subs r3, #8
	lsrs r3, r3, #2
	ands r3, r1
	adds r2, r2, r3
	stmia r6!, {r2}
	mov r2, r10
	subs r2, r6, r2
	str r2, [sp, #0]
	ldmia r7!, {r3}
	stmia r6!, {r3}
	movs r3, #1
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	bne .L_081966b8
	ldr r3, [r7]
	stmia r6!, {r3}
	ldr r3, [r7, #4]
	stmia r6!, {r3}
.L_081966b8:
	adds r7, #8
	mov r2, r12
	subs r3, r6, r2
	subs r3, #8
	ldr r2, [r2]
	lsrs r3, r3, #2
	ands r3, r1
	orrs r2, r3
	mov r3, r12
	str r2, [r3]
	adds r1, r6, #4
	mov r12, r1
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3}
	stmia r1!, {r2, r3}
	adds r6, #8
	adds r7, #8
	mov lr, r6
	mov r2, r9
	cmp r2, #1
	beq .L_0819678c
	cmp r2, #1
	bcc .L_081967c2
	cmp r2, #2
	beq .L_0819671a
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_08196708
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0819677c
	adds r1, r6, #0
	adds r2, #16
	b .L_081967ba
.L_08196708:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08196780
	adds r1, r6, #0
	adds r2, #16
	b .L_081967ba
.L_0819671a:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_08196732
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08196784
	b .L_0819673e
.L_08196732:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08196788
.L_0819673e:
	adds r1, r6, #0
	adds r2, #25
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #100
	b .L_081967f6
	.2byte 0x0000
.L_0819674c:
	.4byte BattleFx_PatchTemplateCode
.L_08196750:
	.4byte Data_08197224
.L_08196754:
	.4byte Data_081973f0
.L_08196758:
	.4byte Data_08196fdc
.L_0819675c:
	.4byte Data_08196fe4
.L_08196760:
	.4byte Data_0819707c
.L_08196764:
	.4byte Data_0819706c
.L_08196768:
	.4byte Data_08197164
.L_0819676c:
	.4byte Data_08197154
.L_08196770:
	.4byte Data_0819720c
.L_08196774:
	.4byte Data_081971f4
.L_08196778:
	.4byte 0x00ffffff
.L_0819677c:
	.4byte Data_0819702c
.L_08196780:
	.4byte Data_08196fec
.L_08196784:
	.4byte Data_081970f0
.L_08196788:
	.4byte Data_0819708c
.L_0819678c:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0819679a
	ldr r4, .L_081968f4
	b .L_0819679c
.L_0819679a:
	ldr r4, .L_081968f8
.L_0819679c:
	movs r5, #132
	lsls r5, r5, #24
	movs r3, #128
	adds r5, #16
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #64
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
.L_081967ba:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #64
	b .L_081967f6
.L_081967c2:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_081967d0
	ldr r4, .L_081968fc
	b .L_081967d2
.L_081967d0:
	ldr r4, .L_08196900
.L_081967d2:
	movs r5, #132
	lsls r5, r5, #24
	movs r3, #128
	adds r5, #14
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #56
	adds r0, r4, #0
	adds r1, r6, #0
	adds r2, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #56
.L_081967f6:
	ldmia r7!, {r3}
	stmia r6!, {r3}
	movs r3, #1
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	bne .L_0819680c
	ldr r3, [r7]
	stmia r6!, {r3}
	ldr r3, [r7, #4]
	stmia r6!, {r3}
.L_0819680c:
	adds r7, #8
	ldmia r7!, {r3}
	stmia r6!, {r3}
	mov r2, lr
	subs r3, r2, r6
	ldr r1, .L_08196904
	ldmia r7!, {r2}
	subs r3, #8
	lsrs r3, r3, #2
	ands r3, r1
	adds r2, r2, r3
	stmia r6!, {r2}
	mov r2, r12
	subs r3, r6, r2
	subs r3, #8
	ldr r2, [r2]
	lsrs r3, r3, #2
	ands r3, r1
	orrs r2, r3
	mov r3, r12
	str r2, [r3]
	ldmia r7!, {r3}
	stmia r6!, {r3}
	mov r12, r6
	ldmia r7!, {r3}
	stmia r6!, {r3}
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0819684e
	ldr r3, [r7]
	b .L_08196850
.L_0819684e:
	ldr r3, [r7, #4]
.L_08196850:
	stmia r6!, {r3}
	adds r7, #8
	ldr r2, [sp, #0]
	lsrs r3, r2, #2
	lsls r2, r3, #2
	adds r5, r2, #0
	cmp r2, #0
	bge .L_08196862
	adds r2, #3
.L_08196862:
	movs r4, #132
	movs r3, #128
	lsls r4, r4, #24
	asrs r2, r2, #2
	lsls r3, r3, #19
	adds r3, #212
	mov r0, r10
	adds r1, r6, #0
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, r6, r5
	mov r1, r12
	subs r3, r6, r1
	ldr r5, .L_08196904
	ldr r2, [r1]
	subs r3, #8
	lsrs r3, r3, #2
	ands r3, r5
	orrs r2, r3
	str r2, [r1]
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3, r4}
	stmia r1!, {r2, r3, r4}
	adds r6, #12
	adds r7, #12
	ldr r2, [sp, #4]
	subs r3, r2, r6
	subs r3, #8
	ldmia r7!, {r2}
	lsrs r3, r3, #2
	ands r3, r5
	adds r2, r2, r3
	stmia r6!, {r2}
	ldr r1, [sp, #8]
	subs r3, r6, r1
	ldr r2, [r1]
	subs r3, #8
	lsrs r3, r3, #2
	ands r3, r5
	orrs r2, r3
	str r2, [r1]
	mov r2, r11
	subs r3, r6, r2
	subs r3, #8
	ldr r2, [r2]
	lsrs r3, r3, #2
	ands r3, r5
	orrs r2, r3
	mov r3, r11
	str r2, [r3]
	adds r0, r7, #0
	adds r1, r6, #0
	ldmia r0!, {r2, r3}
	stmia r1!, {r2, r3}
	adds r2, r7, #0
	adds r2, #8
	ldr r3, .L_08196908
	movs r0, #1
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	subs r0, r0, r3
	add sp, #152
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081968f4:
	.4byte Data_081971b4
.L_081968f8:
	.4byte Data_08197174
.L_081968fc:
	.4byte SentouKouka_IroGyaku
.L_08196900:
	.4byte SentouKouka_IroJun
.L_08196904:
	.4byte 0x00ffffff
.L_08196908:
	.4byte Data_081973f0
