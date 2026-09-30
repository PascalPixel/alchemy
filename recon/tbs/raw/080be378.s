.syntax unified
	.thumb
	.global BattleCommand_BuildPlan
	.thumb_func
BattleCommand_BuildPlan:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	mov r3, sp
	add r2, sp, #32
	adds r3, #44
	str r0, [r2]
	str r3, [sp, #8]
	str r1, [r3]
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	mov r10, r2
	bl Func_08077008
	mov r1, sp
	adds r1, #28
	str r1, [sp, #12]
	ldr r3, .L_080be738
	ldr r3, [r3]
	add r7, sp, #36
	mov r2, r10
	str r3, [r7]
	ldr r3, [r2]
	str r0, [r1]
	movs r4, #10
	ldrsh r0, [r3, r4]
	bl Battle_GetTaggedSlotValue
	str r0, [sp, #40]
	bl BattleEventRuntime_Reset
	mov r4, r10
	ldr r0, [sp, #8]
	ldr r3, [r4]
	ldr r1, [r0]
	ldrh r3, [r3]
	movs r2, #0
	movs r5, #4
	strb r3, [r1]
	str r2, [r1, #96]
	strb r2, [r1, #1]
	str r2, [r1, #88]
	str r2, [r1, #92]
	str r5, [r1, #80]
	bl UiWork_ClearValueNameTablesFar
	ldr r0, [sp, #12]
	ldr r3, [r0]
	movs r1, #56
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080be3f0
	bl .L_080bec5c
.L_080be3f0:
	ldr r3, .L_080be73c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080be46a
	ldr r0, .L_080be740
	bl Func_080770c0
	cmp r0, #0
	beq .L_080be46a
	ldr r1, .L_080be744
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080be46a
	ldr r3, [r1]
	movs r2, #1
	ands r3, r5
	mov r8, r2
	cmp r3, #0
	beq .L_080be420
	movs r3, #0
	mov r8, r3
.L_080be420:
	movs r6, #100
	b .L_080be448
.L_080be424:
	cmp r5, #254
	beq .L_080be446
	movs r1, #192
	adds r0, r5, #0
	lsls r1, r1, #24
	bl Func_08077118
	cmp r0, #0
	bne .L_080be446
	adds r1, r5, #0
	movs r0, #8
	bl BattleEv_Push
	movs r0, #9
	adds r1, r5, #0
	bl BattleEv_Push
.L_080be446:
	adds r6, #2
.L_080be448:
	mov r4, r8
	cmp r4, #0
	beq .L_080be456
	ldr r3, [r7]
	adds r3, #2
	ldrsh r5, [r3, r6]
	b .L_080be45e
.L_080be456:
	ldr r2, [r7]
	adds r3, r6, #0
	subs r3, #12
	ldrsh r5, [r2, r3]
.L_080be45e:
	cmp r5, #255
	bne .L_080be424
	bl BattleEv_DispatchQueued
	bl .L_080bec5c
.L_080be46a:
	bl UiWork_ClearValueNameTablesFar
	ldr r3, [sp, #12]
	ldr r4, .L_080be748
	ldr r2, [r3]
	adds r1, r2, r4
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_080be498
	movs r3, #0
	mov r0, r10
	strb r3, [r1]
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_080be74c
	bl UiText_ShowMessageAndWaitCoreFar
	bl .L_080bec8a
.L_080be498:
	movs r4, #158
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080be4bc
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_080be750
	bl UiText_ShowMessageAndWaitCoreFar
	bl .L_080bec8a
.L_080be4bc:
	ldr r4, .L_080be754
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080be4dc
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_080be758
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080bec8a
.L_080be4dc:
	movs r4, #152
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080be51a
	mov r0, r10
	ldr r3, [r0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #3
	beq .L_080be51a
	bl Func_080771a0
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_080be51a
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Func_08015120
	ldr r0, .L_080be75c
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080bec8a
.L_080be51a:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #6
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .L_080be528
	b .L_080bec5c
.L_080be528:
	ldr r4, [sp, #8]
	ldr r3, [r4]
	movs r2, #1
	mov r11, r2
	movs r1, #0
	adds r3, #44
	movs r2, #13
.L_080be536:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .L_080be536
	ldr r0, [sp, #8]
	movs r2, #1
	ldr r3, [r0]
	negs r2, r2
	adds r1, r2, #0
	adds r3, #58
	movs r2, #13
.L_080be54e:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .L_080be54e
	mov r4, r10
	ldr r3, [r4]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #99
	bls .L_080be568
	bl .L_080bee00
.L_080be568:
	ldr r2, .L_080be760
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080be570:
	.4byte .L_080be76c
	.4byte .L_080be7d0
	.4byte .L_080be888
	.4byte .L_080be96e
	.4byte .L_080be984
	.4byte .L_080beb08
	.4byte .L_080becea
	.4byte .L_080be96e
	.4byte .L_080bec5c
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080bee00
	.4byte .L_080be700
.L_080be700:
	mov r1, r10
	ldr r3, [r1]
	ldrh r3, [r3]
	movs r2, #224
	lsls r0, r3, #16
	lsls r2, r2, #11
	cmp r0, r2
	bhi .L_080be718
	ldr r0, .L_080be764
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080be726
.L_080be718:
	asrs r0, r0, #16
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_080be768
	bl UiText_ShowMessageAndWaitCoreFar
.L_080be726:
	bl BattlePresentation_WaitForAdvance
	ldr r3, [sp, #8]
	ldr r2, [r3]
	movs r3, #7
	str r3, [r2, #84]
	bl .L_080bf1d4
	.2byte 0x0000
.L_080be738:
	.4byte Data_03001e74_a
.L_080be73c:
	.4byte gDebugMode
.L_080be740:
	.4byte 0x0000016d
.L_080be744:
	.4byte Data_03001ae8
.L_080be748:
	.4byte 0x00000145
.L_080be74c:
	.4byte 0x00000880
.L_080be750:
	.4byte 0x00000858
.L_080be754:
	.4byte 0x0000013b
.L_080be758:
	.4byte 0x00000857
.L_080be75c:
	.4byte 0x00000859
.L_080be760:
	.4byte .L_080be570
.L_080be764:
	.4byte 0x00000843
.L_080be768:
	.4byte 0x00000846
.L_080be76c:
	ldr r4, [sp, #12]
	ldr r0, [r4]
	bl RollWeaponUnleashFar
	mov r11, r0
	add r0, sp, #48
	mov r9, r0
	mov r0, r11
	bl BattleCommand_SelectTargets
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_080be78c
	bl .L_080bf1d6
.L_080be78c:
	mov r2, r11
	cmp r2, #1
	bne .L_080be794
	b .L_080bee08
.L_080be794:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	ldr r2, [sp, #12]
	movs r1, #1
	ldr r0, [r2]
	bl Inventory_GetEquippedItemFar
	movs r1, #2
	bl Func_08015120
	ldr r5, .L_080bea9c
	adds r0, r5, #0
	bl UiText_ShowMessageAndWaitCoreFar
	adds r5, #1
	bl BattleEv_SetRuntimeField8
	mov r0, r11
	movs r1, #4
	bl Func_08015120
	adds r0, r5, #0
.L_080be7ca:
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080bee00
.L_080be7d0:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #8
	ldrsh r0, [r3, r1]
	mov r11, r0
	bl Func_08077080
	add r2, sp, #48
	adds r6, r0, #0
	mov r9, r2
	mov r0, r11
	bl BattleCommand_SelectTargets
	movs r3, #1
	negs r3, r3
	movs r5, #1
	cmp r0, r3
	bne .L_080be7f8
	bl .L_080bf1d6
.L_080be7f8:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	movs r1, #4
	mov r0, r11
	bl Func_08015120
	ldr r0, .L_080beaa0
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r2, [sp, #12]
	ldr r1, [r2]
	movs r3, #58
	ldrsh r2, [r1, r3]
	ldrb r3, [r6, #9]
	cmp r2, r3
	bge .L_080be82c
	ldr r4, [sp, #8]
	ldr r2, [r4]
	movs r3, #2
	str r3, [r2, #92]
	movs r5, #0
.L_080be82c:
	ldr r0, .L_080beaa4
	adds r3, r1, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080be840
	ldr r1, [sp, #8]
	ldr r2, [r1]
	movs r3, #1
	str r3, [r2, #92]
	movs r5, #0
.L_080be840:
	cmp r5, #0
	bne .L_080be846
	b .L_080bee00
.L_080be846:
	ldr r2, [sp, #8]
	ldr r3, [r2]
	movs r5, #0
	str r5, [r3, #92]
	ldr r3, [sp, #12]
	ldr r1, [r3]
	ldrb r2, [r6, #9]
	ldrh r3, [r1, #58]
	mov r4, r10
	subs r3, r3, r2
	strh r3, [r1, #58]
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Owner_RecalculateRatiosFar
	ldr r2, [sp, #12]
	ldr r1, [r2]
	movs r4, #58
	ldrsh r3, [r1, r4]
	cmp r3, #0
	bge .L_080be874
	strh r5, [r1, #58]
.L_080be874:
	movs r0, #58
	ldrsh r2, [r1, r0]
	movs r4, #54
	ldrsh r3, [r1, r4]
	ldrh r0, [r1, #54]
	cmp r2, r3
	bgt .L_080be884
	b .L_080bee00
.L_080be884:
	strh r0, [r1, #58]
	b .L_080bee00
.L_080be888:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #8
	ldrsh r2, [r3, r1]
	cmp r2, #0
	bge .L_080be8a6
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_080beaa8
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080bec8a
.L_080be8a6:
	ldr r4, [sp, #12]
	lsls r2, r2, #1
	ldr r3, [r4]
	adds r2, #216
	ldrh r0, [r3, r2]
	bl Item_Get
	adds r5, r0, #0
	ldrh r0, [r5, #40]
	mov r11, r0
	cmp r0, #0
	beq .L_080be8dc
	ldr r1, [sp, #12]
	mov r3, r10
	ldr r2, [r1]
	ldr r1, [r3]
	movs r4, #8
	ldrsh r3, [r1, r4]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r2, [r2, r3]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080be908
	b .L_080be8e0
.L_080be8dc:
	mov r0, r10
	ldr r1, [r0]
.L_080be8e0:
	movs r2, #0
	ldrsh r0, [r1, r2]
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_080beaac
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r4, [sp, #12]
	ldr r0, .L_080beab0
	ldr r3, [r4]
	adds r2, r3, r0
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080be902
	b .L_080bec8a
.L_080be902:
	movs r3, #1
	strb r3, [r2]
	b .L_080bec8a
.L_080be908:
	add r1, sp, #48
	mov r9, r1
	mov r0, r11
	bl BattleCommand_SelectTargets
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_080be91e
	bl .L_080bf1d6
.L_080be91e:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	ldr r3, [sp, #12]
	mov r4, r10
	ldr r2, [r3]
	ldr r3, [r4]
	movs r0, #8
	ldrsh r3, [r3, r0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r2, r3]
	movs r1, #2
	bl Func_08015120
	ldrb r3, [r5, #12]
	cmp r3, #2
	beq .L_080be94e
	cmp r3, #0
	bne .L_080be96a
.L_080be94e:
	ldrb r0, [r5, #2]
	cmp r0, #3
	beq .L_080be966
	cmp r0, #3
	bgt .L_080be95e
	cmp r0, #1
	beq .L_080be966
	b .L_080be96a
.L_080be95e:
	cmp r0, #8
	bgt .L_080be96a
	cmp r0, #6
	blt .L_080be96a
.L_080be966:
	ldr r0, .L_080beab4
	b .L_080be7ca
.L_080be96a:
	ldr r0, .L_080beab8
	b .L_080be7ca
.L_080be96e:
	mov r1, r10
	ldr r3, [r1]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_08015120
	ldr r0, .L_080beaac
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080bec8a
.L_080be984:
	mov r4, r10
	ldr r3, [r4]
	add r2, sp, #48
	movs r1, #8
	ldrsh r0, [r3, r1]
	mov r9, r2
	mov r11, r0
	bl BattleCommand_SelectTargets
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080be9a2
	bl .L_080bf1d6
.L_080be9a2:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	mov r0, r11
	movs r1, #4
	bl Func_08015120
	mov r0, r11
	bl Func_08077080
	ldrb r2, [r0, #1]
	movs r3, #15
	ands r3, r2
	cmp r3, #6
	bne .L_080be9cc
	ldr r0, .L_080beabc
	b .L_080be9ce
.L_080be9cc:
	ldr r0, .L_080beac0
.L_080be9ce:
	movs r3, #244
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_080bea84
	cmp r11, r3
	bgt .L_080bea12
	ldr r2, .L_080beac4
	cmp r11, r2
	bgt .L_080bea00
	subs r3, #52
	cmp r11, r3
	bgt .L_080bea78
	mov r4, r11
	cmp r4, #224
	beq .L_080bea64
	cmp r4, #224
	bge .L_080be9f2
	b .L_080be7ca
.L_080be9f2:
	movs r1, #217
	lsls r1, r1, #1
	cmp r11, r1
	bgt .L_080be9fc
	b .L_080be7ca
.L_080be9fc:
	ldr r0, .L_080beac8
	b .L_080be7ca
.L_080bea00:
	movs r2, #222
	lsls r2, r2, #1
	cmp r11, r2
	ble .L_080bea7c
	movs r3, #236
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_080bea80
	b .L_080be7ca
.L_080bea12:
	movs r3, #250
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_080bea68
	cmp r11, r3
	bgt .L_080bea40
	subs r3, #6
	cmp r11, r3
	beq .L_080bea74
	cmp r11, r3
	bgt .L_080bea32
	movs r4, #246
	lsls r4, r4, #1
	cmp r11, r4
	beq .L_080bea88
	b .L_080be7ca
.L_080bea32:
	ldr r1, .L_080beacc
	cmp r11, r1
	beq .L_080bea8c
	ldr r2, .L_080bead0
	cmp r11, r2
	beq .L_080bea70
	b .L_080be7ca
.L_080bea40:
	ldr r3, .L_080bead4
	cmp r11, r3
	beq .L_080bea90
	cmp r11, r3
	bgt .L_080bea52
	subs r3, #2
	cmp r11, r3
	beq .L_080bea6c
	b .L_080be7ca
.L_080bea52:
	movs r4, #252
	lsls r4, r4, #1
	cmp r11, r4
	beq .L_080bea94
	movs r1, #254
	lsls r1, r1, #1
	cmp r11, r1
	beq .L_080bea98
	b .L_080be7ca
.L_080bea64:
	ldr r0, .L_080beaa0
	b .L_080be7ca
.L_080bea68:
	ldr r0, .L_080bead8
	b .L_080be7ca
.L_080bea6c:
	ldr r0, .L_080beadc
	b .L_080be7ca
.L_080bea70:
	ldr r0, .L_080beae0
	b .L_080be7ca
.L_080bea74:
	ldr r0, .L_080beae4
	b .L_080be7ca
.L_080bea78:
	ldr r0, .L_080beae8
	b .L_080be7ca
.L_080bea7c:
	ldr r0, .L_080beac0
	b .L_080be7ca
.L_080bea80:
	ldr r0, .L_080beaec
	b .L_080be7ca
.L_080bea84:
	ldr r0, .L_080beaf0
	b .L_080be7ca
.L_080bea88:
	ldr r0, .L_080beaf4
	b .L_080be7ca
.L_080bea8c:
	ldr r0, .L_080beaf8
	b .L_080be7ca
.L_080bea90:
	ldr r0, .L_080beafc
	b .L_080be7ca
.L_080bea94:
	ldr r0, .L_080beb00
	b .L_080be7ca
.L_080bea98:
	ldr r0, .L_080beb04
	b .L_080be7ca
.L_080bea9c:
	.4byte 0x00000819
.L_080beaa0:
	.4byte 0x0000083e
.L_080beaa4:
	.4byte 0x0000013d
.L_080beaa8:
	.4byte 0x0000081b
.L_080beaac:
	.4byte 0x00000816
.L_080beab0:
	.4byte 0x0000012b
.L_080beab4:
	.4byte 0x00000818
.L_080beab8:
	.4byte 0x00000817
.L_080beabc:
	.4byte 0x000008f1
.L_080beac0:
	.4byte 0x000008f0
.L_080beac4:
	.4byte 0x000001b9
.L_080beac8:
	.4byte 0x000008f2
.L_080beacc:
	.4byte 0x000001ef
.L_080bead0:
	.4byte 0x000001f3
.L_080bead4:
	.4byte 0x000001f7
.L_080bead8:
	.4byte 0x000008f7
.L_080beadc:
	.4byte 0x000008f8
.L_080beae0:
	.4byte 0x000008f9
.L_080beae4:
	.4byte 0x000008fa
.L_080beae8:
	.4byte 0x000008fb
.L_080beaec:
	.4byte 0x000008fc
.L_080beaf0:
	.4byte 0x000008fd
.L_080beaf4:
	.4byte 0x000008ff
.L_080beaf8:
	.4byte 0x000008fe
.L_080beafc:
	.4byte 0x00000900
.L_080beb00:
	.4byte 0x00000901
.L_080beb04:
	.4byte 0x00000902
.L_080beb08:
	mov r2, r10
	ldr r3, [r2]
	ldrh r3, [r3, #8]
	ldr r6, .L_080beb44
	lsls r0, r3, #16
	movs r5, #255
	asrs r0, r0, #24
	adds r1, r5, #0
	ands r1, r3
	ands r0, r6
	bl Func_080771e8
	mov r4, r10
	ldr r3, [r4]
	mov r11, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_080beb40
	b .L_080bec90
.L_080beb40:
	b .L_080beb48
	.2byte 0x0000
.L_080beb44:
	.4byte 0x0000000f
.L_080beb48:
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	bne .L_080beb66
	b .L_080bec62
.L_080beb66:
	mov r0, r11
	bl Func_08077080
	movs r1, #0
	movs r0, #0
	bl BattlePres_SetActorModes
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r2, r3
	ands r1, r6
	bl Djinn_ActivateFar
	mov r2, r10
	ldr r3, [r2]
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl Trade_RemoveOfferFar
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Owner_RecalculateStatsFar
	bl BattleEventRuntime_Reset
	movs r0, #30
	bl BattleEventRuntime_SchedulePhase
	mov r2, r10
	ldr r3, [r2]
	movs r0, #0
	movs r4, #0
	ldrsh r1, [r3, r4]
	bl BattleEv_Push
	mov r0, r10
	ldr r3, [r0]
	ldrh r2, [r3, #8]
	lsls r3, r2, #16
	asrs r3, r3, #24
	ands r3, r6
	lsls r1, r3, #2
	adds r1, r1, r3
	adds r3, r5, #0
	ands r3, r2
	lsls r1, r1, #2
	movs r2, #150
	lsls r2, r2, #1
	adds r1, r1, r3
	adds r1, r1, r2
	movs r0, #3
	bl BattleEv_Push
	movs r1, #175
	movs r0, #14
	bl BattleEv_Push
	movs r1, #0
	movs r0, #10
	bl BattleEv_Push
	ldr r1, .L_080bef58
	movs r0, #4
	bl BattleEv_Push
	mov r4, r10
	ldr r3, [r4]
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #11
	bl BattleEv_Push
	movs r0, #212
	bl Func_080f9010
	mov r1, r10
	ldr r3, [r1]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Func_08009080
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r2, r10
	ldr r3, [r2]
	ldrh r1, [r3, #8]
	lsls r1, r1, #16
	asrs r1, r1, #24
	movs r4, #0
	ldrsh r0, [r3, r4]
	ands r1, r6
	movs r2, #3
	movs r3, #0
	bl BattleFx_PlayUnitElementEffect
	bl BattleEventRuntime_WaitForReady
.L_080bec5c:
	movs r0, #2
	negs r0, r0
	b .L_080bf1d6
.L_080bec62:
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	movs r1, #4
	mov r0, r11
	bl Func_08015120
	movs r0, #114
	bl Func_080f9010
	ldr r0, .L_080bef5c
	bl UiText_ShowMessageAndWaitCoreFar
	movs r0, #60
	bl WaitFrames
.L_080bec8a:
	movs r0, #1
	negs r0, r0
	b .L_080bf1d6
.L_080bec90:
	add r2, sp, #48
	mov r9, r2
	mov r0, r11
	bl BattleCommand_SelectTargets
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080beca4
	b .L_080bf1d6
.L_080beca4:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	adds r2, r5, #0
	asrs r1, r1, #24
	ands r2, r3
	ands r1, r6
	bl Trade_AddOfferFar
	mov r0, r11
	bl Func_08077080
	mov r2, r10
	ldr r3, [r2]
	adds r5, r0, #0
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Func_08015120
	mov r0, r11
	movs r1, #4
	bl Func_08015120
	ldr r0, .L_080bef60
	bl UiText_ShowMessageAndWaitCoreFar
	ldr r0, [sp, #8]
	ldrb r3, [r5, #2]
	ldr r2, [r0]
	str r3, [r2, #80]
	b .L_080bee00
.L_080becea:
	mov r1, r10
	ldr r3, [r1]
	movs r2, #8
	ldrsh r0, [r3, r2]
	bl SummonDefinition_Get
	mov r4, r10
	movs r2, #24
	ldr r3, [r4]
	add r2, sp
	mov r8, r2
	mov r9, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	mov r1, r8
	bl BattlePlacement_CountValidEntries
	mov r4, r10
	ldr r3, [r4]
	ldrh r3, [r3]
	movs r0, #0
	cmp r3, #7
	bls .L_080bed1a
	movs r0, #1
.L_080bed1a:
	bl Func_08077000
	adds r0, #8
	str r0, [sp, #4]
	mov r1, r9
	adds r1, #4
	mov r0, r8
	ldrb r2, [r0]
	ldrb r3, [r1]
	movs r7, #0
	cmp r2, r3
	bcc .L_080bed56
	movs r5, #4
	mov r6, r8
	movs r4, #4
.L_080bed38:
	mov r2, r9
	ldrb r3, [r2, r5]
	adds r7, #1
	strb r3, [r0]
	adds r4, #1
	adds r0, #1
	cmp r7, #3
	bgt .L_080bed56
	adds r6, #1
	adds r1, #1
	ldrb r2, [r6]
	ldrb r3, [r1]
	adds r5, r4, #0
	cmp r2, r3
	bcs .L_080bed38
.L_080bed56:
	mov r3, r9
	ldrh r3, [r3]
	add r4, sp, #48
	mov r11, r3
	mov r9, r4
	mov r0, r11
	bl BattleCommand_SelectTargets
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_080bed70
	b .L_080bf1d6
.L_080bed70:
	cmp r7, #4
	beq .L_080bed94
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl Func_08015120
	mov r0, r11
	movs r1, #4
	bl Func_08015120
	ldr r0, .L_080bef64
	bl UiText_ShowMessageAndWaitCoreFar
	adds r0, r5, #0
	b .L_080bf1d6
.L_080bed94:
	mov r2, r10
	ldr r3, [r2]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Func_08015120
	movs r1, #4
	mov r0, r11
	bl Func_08015120
	ldr r0, .L_080bef68
	bl UiText_ShowMessageAndWaitCoreFar
	movs r1, #128
	ldr r0, [sp, #4]
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	movs r7, #0
	cmp r3, #0
	beq .L_080bee00
	mov r9, r5
	adds r5, r0, #0
.L_080bedc4:
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r9
	bne .L_080bedee
	ldrb r0, [r5, #2]
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_080bedee
	ldrb r1, [r5]
	mov r3, r8
	ldrb r2, [r3, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080bedee
	movs r3, #254
	strb r3, [r5, #3]
	adds r3, r2, #0
	adds r3, #255
	mov r4, r8
	strb r3, [r4, r1]
.L_080bedee:
	ldr r0, [sp, #4]
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	adds r7, #1
	adds r5, #4
	cmp r7, r3
	bne .L_080bedc4
.L_080bee00:
	mov r2, r11
	cmp r2, #1
	beq .L_080bee08
	b .L_080befb4
.L_080bee08:
	ldr r4, [sp, #8]
	ldr r3, [r4]
	ldrb r0, [r3, #2]
	bl Func_08077008
	adds r6, r0, #0
	ldr r0, [sp, #8]
	ldr r2, [r0]
	mov r1, r10
	movs r3, #1
	str r3, [r2, #76]
	ldr r3, [r1]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Item_GetEquippedElementFar
	ldr r3, [sp, #8]
	ldr r1, [r3]
	movs r3, #2
	str r0, [r1, #80]
	str r3, [r1, #84]
	ldr r4, [sp, #12]
	ldr r0, .L_080bef6c
	ldr r2, [r4]
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080bee58
	movs r1, #148
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r0, [r3]
	bl Battle_GetEntryField2LowBits
	ldr r3, [sp, #8]
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #7
	orrs r3, r0
	b .L_080beea6
.L_080bee58:
	movs r3, #0
	movs r4, #148
	str r3, [r1, #88]
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #5
	bhi .L_080beea8
	ldr r2, .L_080bef70
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080bee70:
	.4byte .L_080beea0
	.4byte .L_080bee88
	.4byte .L_080bee90
	.4byte .L_080bee98
	.4byte .L_080beea8
	.4byte .L_080beea0
.L_080bee88:
	ldr r1, [sp, #8]
	ldr r3, .L_080bef74
	ldr r2, [r1]
	b .L_080beea6
.L_080bee90:
	ldr r3, [sp, #8]
	ldr r2, [r3]
	ldr r3, .L_080bef78
	b .L_080beea6
.L_080bee98:
	ldr r4, [sp, #8]
	ldr r3, .L_080bef78
	ldr r2, [r4]
	b .L_080beea6
.L_080beea0:
	ldr r0, [sp, #8]
	ldr r3, .L_080bef74
	ldr r2, [r0]
.L_080beea6:
	str r3, [r2, #88]
.L_080beea8:
	mov r1, r10
	ldr r3, [r1]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_08015120
	ldr r0, .L_080bef7c
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080beef4
.L_080beebe:
	ldr r4, [sp, #12]
	movs r0, #156
	ldr r3, [r4]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080beee0
	bl Func_080771a0
	movs r3, #255
	ands r0, r3
	cmp r0, #152
	bgt .L_080beee0
	ldr r1, [sp, #8]
	ldr r3, [r1]
	strb r5, [r3, #30]
.L_080beee0:
	bl Func_080771a0
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_080bef28
	ldr r2, [sp, #8]
	ldr r3, [r2]
	strb r0, [r3, #30]
	b .L_080bef28
.L_080beef4:
	movs r4, #56
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_080bef28
	movs r0, #158
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080bef28
	ldr r1, .L_080bef80
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080bef28
	ldr r2, .L_080bef84
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080bef28
	movs r4, #157
	lsls r4, r4, #1
	adds r3, r6, r4
	ldrb r5, [r3]
	cmp r5, #0
	beq .L_080beebe
.L_080bef28:
	movs r0, #183
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	beq .L_080bef3c
	ldr r0, [sp, #8]
	ldr r2, [r0]
	movs r3, #0
	strb r3, [r2, #30]
.L_080bef3c:
	movs r1, #56
	ldrsh r3, [r6, r1]
	cmp r3, #0
	bne .L_080bef46
	b .L_080bf1a8
.L_080bef46:
	bl Func_080771a0
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_080bef88
	ldr r2, [sp, #8]
	ldr r3, [r2]
	b .L_080befac
.L_080bef58:
	.4byte 0x00000897
.L_080bef5c:
	.4byte 0x0000085b
.L_080bef60:
	.4byte 0x0000083f
.L_080bef64:
	.4byte 0x00000842
.L_080bef68:
	.4byte 0x00000841
.L_080bef6c:
	.4byte 0x00000129
.L_080bef70:
	.4byte .L_080bee70
.L_080bef74:
	.4byte 0x00004001
.L_080bef78:
	.4byte 0x00004004
.L_080bef7c:
	.4byte 0x00000814
.L_080bef80:
	.4byte 0x0000013b
.L_080bef84:
	.4byte 0x00000145
.L_080bef88:
	ldr r3, [sp, #12]
	ldr r0, [r3]
	bl Equipment_GetUnleashRateBonusFar
	movs r1, #200
	lsls r0, r0, #16
	bl FixedPoint_Ratio
	adds r5, r0, #0
	bl Func_080771a0
	ldr r3, .L_080bf1e8
	ands r0, r3
	cmp r5, r0
	bgt .L_080befa8
	b .L_080bf1a8
.L_080befa8:
	ldr r4, [sp, #8]
	ldr r3, [r4]
.L_080befac:
	movs r2, #1
	adds r3, #44
	strb r2, [r3]
	b .L_080bf1a8
.L_080befb4:
	mov r0, r11
	bl Func_08077080
	adds r7, r0, #0
	ldr r0, [sp, #8]
	ldrb r2, [r7, #2]
	ldr r3, [r0]
	mov r1, r11
	str r2, [r3, #80]
	movs r2, #0
	str r2, [r3, #88]
	str r1, [r3, #76]
	ldrb r3, [r7, #3]
	adds r2, r3, #0
	cmp r2, #65
	beq .L_080beff2
	cmp r2, #41
	beq .L_080befe8
	cmp r2, #42
	beq .L_080befe8
	cmp r2, #43
	beq .L_080befe8
	cmp r2, #44
	beq .L_080befe8
	cmp r2, #68
	bne .L_080bf044
.L_080befe8:
	adds r2, r3, #0
	cmp r2, #65
	beq .L_080beff2
	cmp r2, #68
	bne .L_080beff6
.L_080beff2:
	movs r5, #153
	b .L_080bf002
.L_080beff6:
	cmp r2, #41
	beq .L_080bf000
	movs r5, #64
	cmp r2, #43
	bne .L_080bf002
.L_080bf000:
	movs r5, #32
.L_080bf002:
	cmp r3, #65
	beq .L_080bf010
	cmp r3, #41
	beq .L_080bf010
	movs r6, #2
	cmp r3, #42
	bne .L_080bf012
.L_080bf010:
	movs r6, #1
.L_080bf012:
	bl Func_080771a0
	movs r3, #255
	ands r0, r3
	cmp r0, r5
	bge .L_080bf0f8
	ldr r3, [sp, #8]
	ldr r2, [r3]
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .L_080bf0f8
	adds r1, r2, #0
	adds r2, #30
.L_080bf030:
	ldrb r3, [r2]
	adds r3, r3, r6
	strb r3, [r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r0, #1
	adds r2, #1
	cmp r0, r3
	blt .L_080bf030
	b .L_080bf0f8
.L_080bf044:
	adds r3, #220
	movs r4, #128
	lsls r3, r3, #24
	lsls r4, r4, #19
	cmp r3, r4
	bhi .L_080bf0b4
	ldrb r3, [r7, #3]
	subs r3, #36
	cmp r3, #4
	bhi .L_080bf084
	ldr r2, .L_080bf1ec
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080bf060:
	.4byte .L_080bf074
	.4byte .L_080bf078
	.4byte .L_080bf07c
	.4byte .L_080bf080
	.4byte .L_080bf084
.L_080bf074:
	movs r5, #63
	b .L_080bf086
.L_080bf078:
	movs r5, #31
	b .L_080bf086
.L_080bf07c:
	movs r5, #15
	b .L_080bf086
.L_080bf080:
	movs r5, #7
	b .L_080bf086
.L_080bf084:
	movs r5, #3
.L_080bf086:
	bl Func_080771a0
	ands r0, r5
	cmp r0, #0
	bne .L_080bf0f8
	ldr r1, [sp, #8]
	ldr r2, [r1]
	movs r3, #1
	ldrsb r3, [r2, r3]
	movs r0, #0
	cmp r0, r3
	bge .L_080bf0f8
	adds r1, r2, #0
	movs r4, #2
	adds r2, #44
.L_080bf0a4:
	strb r4, [r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r0, #1
	adds r2, #1
	cmp r0, r3
	blt .L_080bf0a4
	b .L_080bf0f8
.L_080bf0b4:
	mov r2, r11
	cmp r2, #178
	bne .L_080bf0f8
	ldr r5, [sp, #8]
	ldr r3, [r5]
	ldrb r3, [r3, #1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r6, #0
	cmp r6, r3
	bge .L_080bf0f8
.L_080bf0ca:
	mov r4, r10
	ldr r3, [r4]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldr r3, [r5]
	adds r3, #2
	ldrb r1, [r3, r6]
	ldrb r2, [r7, #2]
	ldrb r3, [r7, #3]
	movs r4, #100
	str r4, [sp, #0]
	bl Battle_HitCheck
	ldr r1, [r5]
	adds r2, r6, #0
	adds r3, r1, #2
	adds r2, #56
	strb r0, [r3, r2]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r6, #1
	cmp r6, r3
	blt .L_080bf0ca
.L_080bf0f8:
	ldr r2, .L_080bf1f0
	cmp r11, r2
	bhi .L_080bf11e
	ldr r3, [sp, #8]
	ldr r2, .L_080bf1f4
	ldr r1, [r3]
	mov r4, r11
	lsls r3, r4, #2
	ldr r2, [r2, r3]
	movs r3, #30
	ldrsb r3, [r1, r3]
	str r2, [r1, #88]
	cmp r3, #1
	ble .L_080bf11e
	lsls r3, r3, #12
	ldr r0, .L_080bf1f8
	adds r3, r2, r3
	adds r3, r3, r0
	str r3, [r1, #88]
.L_080bf11e:
	ldr r1, .L_080bf1fc
	cmp r11, r1
	bhi .L_080bf138
	ldr r1, .L_080bf200
	mov r2, r11
	ldrb r3, [r1, r2]
	cmp r3, #0
	beq .L_080bf138
	ldr r3, [sp, #8]
	mov r4, r11
	ldr r2, [r3]
	ldrb r3, [r1, r4]
	b .L_080bf16c
.L_080bf138:
	mov r0, r11
	bl Ability_CheckStatusOrSpecialId
	cmp r0, #0
	beq .L_080bf14a
	ldr r0, [sp, #8]
	ldr r2, [r0]
	movs r3, #3
	b .L_080bf16c
.L_080bf14a:
	ldr r1, [sp, #8]
	ldr r2, [r1]
	ldr r3, [r2, #88]
	cmp r3, #0
	beq .L_080bf16a
	ldr r4, [sp, #12]
	ldr r0, .L_080bf204
	ldr r3, [r4]
	adds r3, r3, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080bf166
	movs r3, #8
	b .L_080bf16c
.L_080bf166:
	movs r3, #3
	b .L_080bf16c
.L_080bf16a:
	movs r3, #1
.L_080bf16c:
	str r3, [r2, #84]
	ldrb r0, [r7, #3]
	bl Func_080772b8
	cmp r0, #0
	beq .L_080bf186
	ldr r1, [sp, #8]
	ldr r3, [r1]
	movs r1, #128
	ldr r2, [r3, #88]
	lsls r1, r1, #9
	orrs r2, r1
	str r2, [r3, #88]
.L_080bf186:
	mov r2, r11
	cmp r2, #178
	bne .L_080bf1a8
	ldr r3, [sp, #8]
	ldr r1, [r3]
	adds r3, r1, #0
	adds r3, #58
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080bf1a8
	ldr r3, [r1, #88]
	movs r2, #128
	lsls r2, r2, #5
	orrs r3, r2
	str r3, [r1, #88]
.L_080bf1a8:
	mov r4, r10
	ldr r3, [r4]
	movs r0, #6
	ldrsh r3, [r3, r0]
	cmp r3, #2
	bne .L_080bf1c6
	ldr r1, [sp, #8]
	ldr r2, [r1]
	ldr r3, [r2, #84]
	cmp r3, #5
	beq .L_080bf1c6
	cmp r3, #9
	beq .L_080bf1c6
	movs r3, #4
	str r3, [r2, #84]
.L_080bf1c6:
	ldr r2, [sp, #8]
	mov r4, r10
	ldr r3, [r2]
	ldr r2, [r4]
	ldrh r2, [r2, #6]
	adds r3, #72
	strh r2, [r3]
.L_080bf1d4:
	movs r0, #0
.L_080bf1d6:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080bf1e8:
	.4byte 0x0000ffff
.L_080bf1ec:
	.4byte .L_080bf060
.L_080bf1f0:
	.4byte 0x00000206
.L_080bf1f4:
	.4byte Battle_ActionFlags
.L_080bf1f8:
	.4byte 0xfffff000
.L_080bf1fc:
	.4byte 0x00000205
.L_080bf200:
	.4byte Battle_ActionStatus
.L_080bf204:
	.4byte 0x00000129
