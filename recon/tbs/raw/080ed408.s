.syntax unified
	.thumb
	.global Unnamed_080ed408
	.thumb_func
Unnamed_080ed408:
	.global BattleEffect_LoadWork
BattleEffect_LoadWork:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	mov r9, r1
	ldr r1, [sp, #48]
	mov r10, r1
	str r2, [sp, #12]
	mov r2, r10
	mov r8, r3
	movs r1, #3
	cmp r2, #3
	bne .L_080ed42c
	movs r1, #6
.L_080ed42c:
	movs r3, #12
	mov r2, r8
	ands r3, r2
	cmp r3, #4
	bne .L_080ed438
	adds r1, #3
.L_080ed438:
	cmp r3, #8
	bne .L_080ed43e
	adds r1, #4
.L_080ed43e:
	cmp r3, #12
	bne .L_080ed444
	adds r1, #3
.L_080ed444:
	cmp r3, #0
	bne .L_080ed44a
	adds r1, #1
.L_080ed44a:
	movs r3, #2
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed456
	adds r1, #7
.L_080ed456:
	movs r3, #1
	mov r2, r8
	ands r3, r2
	adds r1, #2
	cmp r3, #0
	bne .L_080ed464
	adds r1, #1
.L_080ed464:
	cmp r3, #0
	beq .L_080ed46e
	adds r1, #2
	adds r1, #2
	adds r1, #5
.L_080ed46e:
	movs r3, #1
	mov r2, r8
	ands r3, r2
	adds r1, #9
	cmp r3, #0
	bne .L_080ed47c
	adds r1, #1
.L_080ed47c:
	adds r1, #9
	cmp r3, #0
	bne .L_080ed484
	adds r1, #1
.L_080ed484:
	mov r3, r10
	adds r1, #3
	cmp r3, #1
	beq .L_080ed49c
	cmp r3, #1
	bcc .L_080ed498
	cmp r3, #2
	beq .L_080ed49c
	cmp r3, #3
	beq .L_080ed4a0
.L_080ed498:
	adds r1, #2
	b .L_080ed4a2
.L_080ed49c:
	adds r1, #4
	b .L_080ed4a2
.L_080ed4a0:
	adds r1, #6
.L_080ed4a2:
	movs r3, #1
	mov r2, r8
	ands r3, r2
	adds r1, #3
	cmp r3, #0
	bne .L_080ed4b0
	adds r1, #2
.L_080ed4b0:
	mov r3, r10
	adds r1, #2
	cmp r3, #1
	beq .L_080ed4d0
	cmp r3, #1
	bcc .L_080ed4c4
	cmp r3, #2
	beq .L_080ed4d4
	cmp r3, #3
	beq .L_080ed4d8
.L_080ed4c4:
	movs r3, #0
.L_080ed4c6:
	adds r3, #1
	adds r1, #2
	cmp r3, #7
	ble .L_080ed4c6
	b .L_080ed4da
.L_080ed4d0:
	adds r1, #25
	b .L_080ed4da
.L_080ed4d4:
	adds r1, #32
	b .L_080ed4da
.L_080ed4d8:
	adds r1, #28
.L_080ed4da:
	movs r3, #1
	mov r2, r8
	ands r3, r2
	adds r1, #1
	cmp r3, #0
	bne .L_080ed4e8
	adds r1, #2
.L_080ed4e8:
	mov r3, r10
	adds r1, #5
	cmp r3, #1
	beq .L_080ed500
	cmp r3, #1
	bcc .L_080ed4fc
	cmp r3, #2
	beq .L_080ed500
	cmp r3, #3
	beq .L_080ed504
.L_080ed4fc:
	adds r1, #2
	b .L_080ed506
.L_080ed500:
	adds r1, #4
	b .L_080ed506
.L_080ed504:
	adds r1, #6
.L_080ed506:
	adds r1, #8
	lsls r1, r1, #2
	bl Runtime_AllocateHeapBlock
	ldr r6, .L_080ed82c
	adds r5, r0, #0
	ldr r3, .L_080ed830
	ldr r2, .L_080ed834
	adds r0, r6, #0
	adds r1, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #12
	adds r6, #12
	mov r1, r10
	cmp r1, #3
	bne .L_080ed534
	ldr r0, .L_080ed838
	adds r1, r5, #0
	ldr r2, .L_080ed834
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #12
.L_080ed534:
	movs r4, #12
	mov r2, r8
	ands r4, r2
	cmp r4, #4
	bne .L_080ed54c
	ldr r3, .L_080ed830
	adds r0, r6, #0
	adds r1, r5, #0
	ldr r2, .L_080ed834
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #12
.L_080ed54c:
	adds r6, #12
	cmp r4, #8
	bne .L_080ed560
	ldr r3, .L_080ed830
	adds r0, r6, #0
	adds r1, r5, #0
	ldr r2, .L_080ed83c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #16
.L_080ed560:
	adds r6, #16
	cmp r4, #12
	bne .L_080ed574
	ldr r3, .L_080ed830
	adds r0, r6, #0
	adds r1, r5, #0
	ldr r2, .L_080ed834
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #12
.L_080ed574:
	adds r6, #12
	cmp r4, #0
	bne .L_080ed57e
	ldr r3, [r6]
	stmia r5!, {r3}
.L_080ed57e:
	movs r3, #2
	mov r1, r8
	ands r3, r1
	adds r6, #4
	cmp r3, #0
	beq .L_080ed5bc
	ldr r3, [r6]
	stmia r5!, {r3}
	ldr r3, [r6, #4]
	stmia r5!, {r3}
	movs r3, #8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed59e
	ldr r3, [r6, #8]
	b .L_080ed5a0
.L_080ed59e:
	ldr r3, [r6, #12]
.L_080ed5a0:
	stmia r5!, {r3}
	ldr r3, [r6, #16]
	stmia r5!, {r3}
	ldr r3, [r6, #20]
	stmia r5!, {r3}
	movs r2, #1
	ldr r3, [sp, #12]
	lsls r2, r3
	ldr r3, [r6, #24]
	adds r3, r3, r2
	stmia r5!, {r3}
	ldr r3, [r6, #28]
	adds r3, r3, r2
	stmia r5!, {r3}
.L_080ed5bc:
	adds r6, #32
	ldmia r6!, {r3}
	stmia r5!, {r3}
	str r5, [sp, #8]
	ldmia r6!, {r3}
	stmia r5!, {r3}
	movs r2, #1
	mov r1, r8
	ands r1, r2
	cmp r1, #0
	bne .L_080ed5de
	mov r3, r9
	lsls r2, r3
	ldr r3, [r6]
	adds r3, r3, r2
	subs r3, #1
	stmia r5!, {r3}
.L_080ed5de:
	adds r6, #4
	cmp r1, #0
	beq .L_080ed638
	ldr r3, [r6]
	stmia r5!, {r3}
	ldr r3, [r6, #4]
	stmia r5!, {r3}
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed5fe
	ldr r3, [r6, #8]
	stmia r5!, {r3}
	ldr r3, [r6, #12]
	b .L_080ed604
.L_080ed5fe:
	ldr r3, [r6, #16]
	stmia r5!, {r3}
	ldr r3, [r6, #20]
.L_080ed604:
	stmia r5!, {r3}
	ldr r3, [r6, #24]
	stmia r5!, {r3}
	ldr r3, [r6, #28]
	stmia r5!, {r3}
	ldr r3, .L_080ed840
	mov r2, r9
	lsls r1, r2, #1
	ldrh r2, [r3, r1]
	ldr r3, [r6, #32]
	adds r3, r3, r2
	stmia r5!, {r3}
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed62a
	ldr r3, [r6, #36]
	b .L_080ed62c
.L_080ed62a:
	ldr r3, [r6, #40]
.L_080ed62c:
	stmia r5!, {r3}
	ldr r3, .L_080ed840
	ldrh r2, [r3, r1]
	ldr r3, [r6, #44]
	adds r3, r3, r2
	stmia r5!, {r3}
.L_080ed638:
	adds r6, #48
	ldmia r6!, {r3}
	stmia r5!, {r3}
	mov r11, r5
	ldmia r6!, {r3}
	stmia r5!, {r3}
	ldr r3, .L_080ed830
	ldr r2, .L_080ed844
	adds r0, r6, #0
	adds r1, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #24
	adds r6, #24
	ldmia r6!, {r3}
	stmia r5!, {r3}
	movs r7, #1
	mov r4, r8
	ands r4, r7
	cmp r4, #0
	bne .L_080ed666
	ldr r3, [r6]
	stmia r5!, {r3}
.L_080ed666:
	adds r6, #4
	ldr r3, .L_080ed830
	ldr r2, .L_080ed848
	adds r0, r6, #0
	adds r1, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #20
	adds r6, #20
	str r5, [sp, #4]
	ldr r3, [r6]
	stmia r5!, {r3}
	ldr r3, [sp, #12]
	subs r3, #3
	adds r2, r7, #0
	lsls r2, r3
	ldr r3, [r6, #4]
	adds r3, r3, r2
	subs r3, #1
	stmia r5!, {r3}
	mov r2, r9
	ldr r3, [r6, #8]
	subs r2, #3
	lsls r2, r2, #7
	adds r3, r3, r2
	stmia r5!, {r3}
	ldr r3, [r6, #12]
	stmia r5!, {r3}
	adds r6, #16
	cmp r4, #0
	bne .L_080ed6a8
	ldr r3, [r6]
	stmia r5!, {r3}
.L_080ed6a8:
	adds r6, #4
	ldmia r6!, {r3}
	stmia r5!, {r3}
	mov r12, r5
	ldmia r6!, {r3}
	stmia r5!, {r3}
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed6c2
	ldr r3, [r6]
	b .L_080ed6c4
.L_080ed6c2:
	ldr r3, [r6, #4]
.L_080ed6c4:
	stmia r5!, {r3}
	mov r2, r10
	adds r6, #8
	adds r4, r5, #0
	cmp r2, #1
	beq .L_080ed6fc
	cmp r2, #1
	bcc .L_080ed6dc
	cmp r2, #2
	beq .L_080ed712
	cmp r2, #3
	beq .L_080ed732
.L_080ed6dc:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed6ec
	ldr r3, .L_080ed830
	ldr r0, .L_080ed84c
	b .L_080ed6f0
.L_080ed6ec:
	ldr r3, .L_080ed830
	ldr r0, .L_080ed850
.L_080ed6f0:
	adds r1, r5, #0
	ldr r2, .L_080ed854
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #8
	b .L_080ed750
.L_080ed6fc:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed70c
	ldr r3, .L_080ed830
	ldr r0, .L_080ed858
	b .L_080ed726
.L_080ed70c:
	ldr r3, .L_080ed830
	ldr r0, .L_080ed85c
	b .L_080ed726
.L_080ed712:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed722
	ldr r3, .L_080ed830
	ldr r0, .L_080ed860
	b .L_080ed726
.L_080ed722:
	ldr r3, .L_080ed830
	ldr r0, .L_080ed864
.L_080ed726:
	adds r1, r5, #0
	ldr r2, .L_080ed83c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #16
	b .L_080ed750
.L_080ed732:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed742
	ldr r3, .L_080ed830
	ldr r0, .L_080ed868
	b .L_080ed746
.L_080ed742:
	ldr r3, .L_080ed830
	ldr r0, .L_080ed86c
.L_080ed746:
	adds r1, r5, #0
	ldr r2, .L_080ed844
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #24
.L_080ed750:
	adds r6, #16
	ldmia r6!, {r3}
	stmia r5!, {r3}
	subs r3, r4, r5
	subs r3, #8
	ldr r1, .L_080ed870
	ldmia r6!, {r2}
	lsrs r3, r3, #2
	ands r3, r1
	adds r2, r2, r3
	stmia r5!, {r2}
	ldmia r6!, {r3}
	stmia r5!, {r3}
	movs r3, #1
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	bne .L_080ed77c
	ldr r3, [r6]
	stmia r5!, {r3}
	ldr r3, [r6, #4]
	stmia r5!, {r3}
.L_080ed77c:
	adds r6, #8
	mov r2, r12
	subs r3, r5, r2
	subs r3, #8
	ldr r2, [r2]
	lsrs r3, r3, #2
	ands r3, r1
	orrs r2, r3
	mov r3, r12
	str r2, [r3]
	ldmia r6!, {r3}
	stmia r5!, {r3}
	mov r12, r5
	ldmia r6!, {r3}
	stmia r5!, {r3}
	str r5, [sp, #0]
	mov r1, r10
	cmp r1, #1
	beq .L_080ed7de
	cmp r1, #1
	bcc .L_080ed7ae
	cmp r1, #2
	beq .L_080ed7fe
	cmp r1, #3
	beq .L_080ed88c
.L_080ed7ae:
	ldr r3, .L_080ed84c
	ldr r1, .L_080ed850
	movs r7, #4
	mov r2, r8
	movs r4, #0
	ands r7, r2
	mov r9, r3
	mov lr, r1
.L_080ed7be:
	cmp r7, #0
	beq .L_080ed7c8
	ldr r3, .L_080ed830
	mov r0, r9
	b .L_080ed7cc
.L_080ed7c8:
	ldr r3, .L_080ed830
	mov r0, lr
.L_080ed7cc:
	adds r1, r5, #0
	ldr r2, .L_080ed854
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #8
	adds r4, #1
	cmp r4, #7
	ble .L_080ed7be
	b .L_080ed8b6
.L_080ed7de:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed7ee
	ldr r3, .L_080ed830
	ldr r0, .L_080ed874
	b .L_080ed7f2
.L_080ed7ee:
	ldr r3, .L_080ed830
	ldr r0, .L_080ed878
.L_080ed7f2:
	adds r1, r5, #0
	ldr r2, .L_080ed87c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #100
	b .L_080ed8b6
.L_080ed7fe:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed80c
	ldr r4, .L_080ed880
	b .L_080ed80e
.L_080ed80c:
	ldr r4, .L_080ed884
.L_080ed80e:
	ldr r3, .L_080ed830
	adds r0, r4, #0
	adds r1, r5, #0
	ldr r2, .L_080ed888
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #64
	adds r0, r4, #0
	adds r1, r5, #0
	ldr r2, .L_080ed888
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #64
	b .L_080ed8b6
	.2byte 0x0000
.L_080ed82c:
	.4byte SentouKouka_Gousei
.L_080ed830:
	.4byte 0x040000d4
.L_080ed834:
	.4byte 0x84000003
.L_080ed838:
	.4byte SentouKouka_Mask
.L_080ed83c:
	.4byte 0x84000004
.L_080ed840:
	.4byte Data_080ef034
.L_080ed844:
	.4byte 0x84000006
.L_080ed848:
	.4byte 0x84000005
.L_080ed84c:
	.4byte SentouKouka_YomiGyaku
.L_080ed850:
	.4byte SentouKouka_YomiJun
.L_080ed854:
	.4byte 0x84000002
.L_080ed858:
	.4byte SentouKouka_NuriGyaku
.L_080ed85c:
	.4byte SentouKouka_NuriJun
.L_080ed860:
	.4byte SentouKouka_HikakuGyaku
.L_080ed864:
	.4byte SentouKouka_HikakuJun
.L_080ed868:
	.4byte SentouKouka_KasanGyaku
.L_080ed86c:
	.4byte SentouKouka_KasanJun
.L_080ed870:
	.4byte 0x00ffffff
.L_080ed874:
	.4byte SentouKouka_Nuri8Gyaku
.L_080ed878:
	.4byte SentouKouka_Nuri8Jun
.L_080ed87c:
	.4byte 0x84000019
.L_080ed880:
	.4byte SentouKouka_Hikaku4Gyaku
.L_080ed884:
	.4byte SentouKouka_Hikaku4Jun
.L_080ed888:
	.4byte 0x84000010
.L_080ed88c:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed89a
	ldr r4, .L_080eda30
	b .L_080ed89c
.L_080ed89a:
	ldr r4, .L_080eda34
.L_080ed89c:
	ldr r3, .L_080eda38
	adds r0, r4, #0
	adds r1, r5, #0
	ldr r2, .L_080eda3c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #56
	adds r0, r4, #0
	adds r1, r5, #0
	ldr r2, .L_080eda3c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #56
.L_080ed8b6:
	adds r6, #16
	ldmia r6!, {r3}
	stmia r5!, {r3}
	movs r3, #1
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	bne .L_080ed8ce
	ldr r3, [r6]
	stmia r5!, {r3}
	ldr r3, [r6, #4]
	stmia r5!, {r3}
.L_080ed8ce:
	adds r6, #8
	ldmia r6!, {r3}
	stmia r5!, {r3}
	ldr r2, [sp, #0]
	subs r3, r2, r5
	ldr r1, .L_080eda40
	subs r3, #8
	ldmia r6!, {r2}
	lsrs r3, r3, #2
	ands r3, r1
	adds r2, r2, r3
	stmia r5!, {r2}
	mov r2, r12
	subs r3, r5, r2
	subs r3, #8
	ldr r2, [r2]
	lsrs r3, r3, #2
	ands r3, r1
	orrs r2, r3
	mov r3, r12
	str r2, [r3]
	ldmia r6!, {r3}
	stmia r5!, {r3}
	mov r12, r5
	ldmia r6!, {r3}
	stmia r5!, {r3}
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed910
	ldr r3, [r6]
	b .L_080ed912
.L_080ed910:
	ldr r3, [r6, #4]
.L_080ed912:
	stmia r5!, {r3}
	mov r2, r10
	adds r6, #8
	adds r4, r5, #0
	cmp r2, #1
	beq .L_080ed94a
	cmp r2, #1
	bcc .L_080ed92a
	cmp r2, #2
	beq .L_080ed960
	cmp r2, #3
	beq .L_080ed980
.L_080ed92a:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed93a
	ldr r3, .L_080eda38
	ldr r0, .L_080eda44
	b .L_080ed93e
.L_080ed93a:
	ldr r3, .L_080eda38
	ldr r0, .L_080eda48
.L_080ed93e:
	adds r1, r5, #0
	ldr r2, .L_080eda4c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #8
	b .L_080ed99e
.L_080ed94a:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed95a
	ldr r3, .L_080eda38
	ldr r0, .L_080eda50
	b .L_080ed974
.L_080ed95a:
	ldr r3, .L_080eda38
	ldr r0, .L_080eda54
	b .L_080ed974
.L_080ed960:
	movs r3, #4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_080ed970
	ldr r3, .L_080eda38
	ldr r0, .L_080eda58
	b .L_080ed974
.L_080ed970:
	ldr r3, .L_080eda38
	ldr r0, .L_080eda5c
.L_080ed974:
	adds r1, r5, #0
	ldr r2, .L_080eda60
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #16
	b .L_080ed99e
.L_080ed980:
	movs r3, #4
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_080ed990
	ldr r3, .L_080eda38
	ldr r0, .L_080eda64
	b .L_080ed994
.L_080ed990:
	ldr r3, .L_080eda38
	ldr r0, .L_080eda68
.L_080ed994:
	adds r1, r5, #0
	ldr r2, .L_080eda6c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #24
.L_080ed99e:
	adds r6, #16
	ldmia r6!, {r3}
	stmia r5!, {r3}
	subs r3, r4, r5
	subs r3, #8
	ldr r4, .L_080eda40
	ldmia r6!, {r2}
	lsrs r3, r3, #2
	ands r3, r4
	adds r2, r2, r3
	stmia r5!, {r2}
	mov r1, r12
	subs r3, r5, r1
	subs r3, #8
	ldr r2, [r1]
	lsrs r3, r3, #2
	ands r3, r4
	orrs r2, r3
	str r2, [r1]
	ldr r3, .L_080eda38
	ldr r2, .L_080eda70
	adds r0, r6, #0
	adds r1, r5, #0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #12
	adds r6, #12
	ldr r2, [sp, #4]
	subs r3, r2, r5
	subs r3, #8
	ldmia r6!, {r2}
	lsrs r3, r3, #2
	ands r3, r4
	adds r2, r2, r3
	stmia r5!, {r2}
	ldr r1, [sp, #8]
	subs r3, r5, r1
	subs r3, #8
	ldr r2, [r1]
	lsrs r3, r3, #2
	ands r3, r4
	orrs r2, r3
	str r2, [r1]
	mov r2, r11
	subs r3, r5, r2
	subs r3, #8
	ldr r2, [r2]
	lsrs r3, r3, #2
	ands r3, r4
	orrs r2, r3
	mov r3, r11
	str r2, [r3]
	ldmia r6!, {r3}
	stmia r5!, {r3}
	ldmia r6!, {r3}
	str r3, [r5]
	ldr r3, .L_080eda74
	eors r3, r6
	negs r2, r3
	orrs r2, r3
	lsrs r2, r2, #31
	movs r0, #1
	subs r0, r0, r2
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080eda30:
	.4byte SentouKouka_IroGyaku
.L_080eda34:
	.4byte SentouKouka_IroJun
.L_080eda38:
	.4byte 0x040000d4
.L_080eda3c:
	.4byte 0x8400000e
.L_080eda40:
	.4byte 0x00ffffff
.L_080eda44:
	.4byte SentouKouka_YomiGyaku
.L_080eda48:
	.4byte SentouKouka_YomiJun
.L_080eda4c:
	.4byte 0x84000002
.L_080eda50:
	.4byte SentouKouka_NuriGyaku
.L_080eda54:
	.4byte SentouKouka_NuriJun
.L_080eda58:
	.4byte SentouKouka_HikakuGyaku
.L_080eda5c:
	.4byte SentouKouka_HikakuJun
.L_080eda60:
	.4byte 0x84000004
.L_080eda64:
	.4byte SentouKouka_KasanGyaku
.L_080eda68:
	.4byte SentouKouka_KasanJun
.L_080eda6c:
	.4byte 0x84000006
.L_080eda70:
	.4byte 0x84000003
.L_080eda74:
	.4byte ParticleStreams_CellOffsets
