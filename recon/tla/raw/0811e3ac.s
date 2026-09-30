.syntax unified
	.thumb
	.global Func_0811e3ac
	.thumb_func
Func_0811e3ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r0, [sp, #8]
	adds r7, r1, #0
	ldr r3, [r0, #84]
	subs r3, #15
	cmp r3, #2
	bhi .L_0811e3ce
	ldr r5, [r0, #92]
	cmp r5, #0
	beq .L_0811e3d2
.L_0811e3ce:
	movs r0, #0
	b .L_0811e7cc
.L_0811e3d2:
	ldr r1, [sp, #8]
	movs r0, #56
	ldrb r2, [r1, #3]
	ldrb r3, [r1]
	eors r3, r2
	movs r2, #128
	ands r3, r2
	lsls r3, r3, #24
	lsrs r3, r3, #24
	negs r2, r3
	orrs r2, r3
	lsrs r2, r2, #31
	str r2, [sp, #4]
	bl Runtime_BumpAllocateAlternatePool
	add r2, sp, #4
	mov r9, r0
	ldrb r2, [r2]
	mov r3, r9
	mov r6, r9
	adds r3, #49
	adds r6, #50
	strb r2, [r3]
	strb r5, [r6]
	ldr r1, [sp, #8]
	ldr r2, .L_0811e5cc
	ldr r3, [r1, #76]
	adds r3, r3, r2
	cmp r3, #31
	bls .L_0811e410
	b .L_0811e528
.L_0811e410:
	ldr r2, .L_0811e5d0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0811e418:
	.4byte .L_0811e498
	.4byte .L_0811e49c
	.4byte .L_0811e4a0
	.4byte .L_0811e4a4
	.4byte .L_0811e4aa
	.4byte .L_0811e4b0
	.4byte .L_0811e4b6
	.4byte .L_0811e4bc
	.4byte .L_0811e4c2
	.4byte .L_0811e4c6
	.4byte .L_0811e4ca
	.4byte .L_0811e4ce
	.4byte .L_0811e4d4
	.4byte .L_0811e4d8
	.4byte .L_0811e4de
	.4byte .L_0811e4e2
	.4byte .L_0811e4e6
	.4byte .L_0811e4ea
	.4byte .L_0811e4ee
	.4byte .L_0811e4f4
	.4byte .L_0811e4f8
	.4byte .L_0811e4fc
	.4byte .L_0811e500
	.4byte .L_0811e504
	.4byte .L_0811e508
	.4byte .L_0811e50e
	.4byte .L_0811e512
	.4byte .L_0811e518
	.4byte .L_0811e528
	.4byte .L_0811e51c
	.4byte .L_0811e520
	.4byte .L_0811e524
.L_0811e498:
	movs r3, #117
	b .L_0811e52a
.L_0811e49c:
	movs r3, #137
	b .L_0811e52a
.L_0811e4a0:
	movs r3, #182
	b .L_0811e52a
.L_0811e4a4:
	movs r3, #44
	adds r3, #255
	b .L_0811e52a
.L_0811e4aa:
	movs r3, #148
	lsls r3, r3, #1
	b .L_0811e52a
.L_0811e4b0:
	movs r3, #139
	lsls r3, r3, #1
	b .L_0811e52a
.L_0811e4b6:
	movs r3, #158
	lsls r3, r3, #1
	b .L_0811e52a
.L_0811e4bc:
	movs r3, #62
	adds r3, #255
	b .L_0811e52a
.L_0811e4c2:
	movs r3, #185
	b .L_0811e52a
.L_0811e4c6:
	movs r3, #158
	b .L_0811e52a
.L_0811e4ca:
	movs r3, #150
	b .L_0811e52a
.L_0811e4ce:
	movs r3, #56
	adds r3, #255
	b .L_0811e52a
.L_0811e4d4:
	movs r3, #212
	b .L_0811e52a
.L_0811e4d8:
	movs r3, #68
	adds r3, #255
	b .L_0811e52a
.L_0811e4de:
	movs r3, #191
	b .L_0811e52a
.L_0811e4e2:
	movs r3, #159
	b .L_0811e52a
.L_0811e4e6:
	movs r3, #162
	b .L_0811e52a
.L_0811e4ea:
	movs r3, #155
	b .L_0811e52a
.L_0811e4ee:
	movs r3, #8
	adds r3, #255
	b .L_0811e52a
.L_0811e4f4:
	movs r3, #197
	b .L_0811e52a
.L_0811e4f8:
	movs r3, #215
	b .L_0811e52a
.L_0811e4fc:
	movs r3, #99
	b .L_0811e52a
.L_0811e500:
	movs r3, #196
	b .L_0811e52a
.L_0811e504:
	movs r3, #160
	b .L_0811e52a
.L_0811e508:
	movs r3, #26
	adds r3, #255
	b .L_0811e52a
.L_0811e50e:
	movs r3, #163
	b .L_0811e52a
.L_0811e512:
	movs r3, #151
	lsls r3, r3, #1
	b .L_0811e52a
.L_0811e518:
	movs r3, #188
	b .L_0811e52a
.L_0811e51c:
	movs r3, #242
	b .L_0811e52a
.L_0811e520:
	movs r3, #203
	b .L_0811e52a
.L_0811e524:
	movs r3, #218
	b .L_0811e52a
.L_0811e528:
	movs r3, #9
.L_0811e52a:
	mov r1, r9
	strh r3, [r1, #52]
	ldr r2, [sp, #4]
	cmp r2, #0
	bne .L_0811e570
	ldr r1, [sp, #8]
	movs r2, #128
	ldrb r3, [r1]
	eors r3, r2
	ands r3, r2
	movs r2, #48
	add r2, r9
	strb r3, [r2]
	mov r8, r2
	ldrb r0, [r2]
	bl GetBattleObjectSlot
	ldr r3, [r0]
	cmp r3, #0
	bne .L_0811e57c
	movs r3, #1
	strb r3, [r6]
	mov r3, r8
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	bl Func_0811a720
	mov r1, r8
	ldrb r0, [r1]
	bl GetBattleObjectSlot
	ldr r3, .L_0811e5d4
	str r3, [r0, #24]
	b .L_0811e57c
.L_0811e570:
	ldr r2, [sp, #8]
	movs r1, #48
	ldrb r3, [r2]
	add r1, r9
	strb r3, [r1]
	mov r8, r1
.L_0811e57c:
	mov r2, r8
	ldrb r3, [r2]
	str r3, [r7, #8]
	ldr r3, [sp, #8]
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	ldr r5, [r0]
	movs r1, #3
	adds r0, r5, #0
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #128
	lsls r0, r0, #19
	movs r1, #253
	lsls r1, r1, #6
	adds r0, #80
	bl Func_08013ba4
	movs r0, #154
	bl Audio_PlayCue
	movs r0, #10
	bl WaitFrames
	ldr r1, [sp, #4]
	cmp r1, #0
	beq .L_0811e5d8
	mov r2, r8
	ldrb r0, [r2]
	movs r1, #1
	bl BattlePres_SetActorRecordMode
	add r3, sp, #12
	mov r11, r3
	b .L_0811e608
.L_0811e5cc:
	.4byte 0xfffffda8
.L_0811e5d0:
	.4byte .L_0811e418
.L_0811e5d4:
	.4byte 0x00013333
.L_0811e5d8:
	mov r1, r8
	ldrb r3, [r1]
	movs r2, #12
	negs r0, r3
	orrs r0, r3
	add r2, sp
	lsrs r0, r0, #31
	mov r11, r2
	adds r0, #1
	mov r1, r11
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_0811e608
	mov r6, r11
	adds r5, r0, #0
.L_0811e5f8:
	ldrh r0, [r6]
	movs r1, #1
	subs r5, #1
	adds r6, #2
	bl BattlePres_SetActorRecordMode
	cmp r5, #0
	bne .L_0811e5f8
.L_0811e608:
	movs r7, #128
	ldr r6, .L_0811e614
	lsls r7, r7, #19
	movs r5, #0
	adds r7, #82
	b .L_0811e618
.L_0811e614:
	.4byte 0x00000010
.L_0811e618:
	subs r2, r6, r5
	lsls r3, r5, #8
	orrs r3, r2
	strh r3, [r7]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #16
	bne .L_0811e618
	mov r3, r8
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	str r0, [sp, #0]
	ldr r2, [sp, #4]
	ldr r1, [r0]
	mov r10, r1
	cmp r2, #0
	beq .L_0811e64a
	mov r3, r8
	ldrb r0, [r3]
	bl Func_0811b724
	b .L_0811e67a
.L_0811e64a:
	mov r1, r8
	ldrb r3, [r1]
	mov r1, r11
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	adds r0, #1
	bl BattleParty_ListActorIds
	cmp r0, #0
	ble .L_0811e67a
	mov r7, r11
	movs r6, #0
	adds r5, r0, #0
.L_0811e666:
	ldrh r0, [r6, r7]
	bl Func_0811b724
	subs r5, #1
	ldrh r0, [r6, r7]
	bl Func_0811a5fc
	adds r6, #2
	cmp r5, #0
	bne .L_0811e666
.L_0811e67a:
	mov r2, r8
	ldrb r0, [r2]
	bl GetBattleObjectSlot
	movs r2, #44
	adds r1, r0, #0
	ldr r3, .L_0811e708
	mov r0, r9
	mov lr, r3
	.2byte 0xf800
	mov r3, r8
	ldrb r0, [r3]
	bl GetBattleObjectSlot
	ldr r1, [sp, #4]
	bl Func_0811a720
	mov r1, r8
	ldrb r0, [r1]
	bl GetBattleObjectSlot
	ldr r3, .L_0811e70c
	str r3, [r0, #24]
	ldr r2, [sp, #4]
	cmp r2, #0
	beq .L_0811e6bc
	movs r0, #128
	lsls r0, r0, #8
	bl Runtime_BumpAllocateAlternatePool
	mov r3, r9
	str r0, [r3, #44]
	b .L_0811e6c2
.L_0811e6bc:
	ldr r3, .L_0811e710
	mov r1, r9
	str r3, [r1, #44]
.L_0811e6c2:
	mov r2, r9
	ldrh r0, [r2, #52]
	bl Func_081280bc
	mov r3, r9
	adds r5, r0, #0
	ldrh r0, [r3, #52]
	bl Func_081280d8
	mov r2, r9
	adds r3, r0, #0
	ldr r1, [r2, #44]
	movs r0, #7
	adds r2, r5, #0
	bl Func_080202b8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0811e714
	mov r3, r9
	ldrh r0, [r3, #52]
	bl Func_081280bc
	movs r1, #224
	ldr r2, [sp, #0]
	lsls r1, r1, #7
	adds r0, r0, r1
	strh r0, [r2, #4]
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_0811e732
	mov r1, r9
	str r3, [r1, #44]
	b .L_0811e732
	.2byte 0x0000
.L_0811e708:
	.4byte IwramCopyWords
.L_0811e70c:
	.4byte 0x00013333
.L_0811e710:
	.4byte Data_02018000
.L_0811e714:
	mov r2, r9
	ldrh r0, [r2, #52]
	bl Func_081280bc
	ldr r3, [sp, #0]
	strh r0, [r3, #4]
	ldr r1, [sp, #4]
	cmp r1, #0
	beq .L_0811e72e
	mov r2, r9
	ldr r0, [r2, #44]
	bl Sys_Free
.L_0811e72e:
	mov r3, r9
	str r5, [r3, #44]
.L_0811e732:
	mov r1, r8
	ldrb r3, [r1]
	mov r2, r11
	strh r3, [r2]
	mov r1, r11
	movs r3, #255
	strh r3, [r1, #2]
	movs r2, #0
	movs r1, #1
	mov r0, r11
	bl Func_0811b75c
	movs r3, #160
	lsls r3, r3, #14
	mov r2, r10
	str r3, [r2, #12]
	ldr r1, [sp, #8]
	ldr r2, .L_0811e778
	ldrb r3, [r1, #3]
	cmp r3, #7
	bhi .L_0811e75e
	ldr r2, .L_0811e77c
.L_0811e75e:
	mov r3, r10
	mov r1, r8
	ldrb r0, [r1]
	strh r2, [r3, #6]
	movs r1, #1
	bl BattlePres_SetActorRecordMode
	movs r6, #128
	lsls r6, r6, #19
	movs r5, #0
	adds r6, #82
	b .L_0811e780
	.2byte 0x0000
.L_0811e778:
	.4byte 0x00008000
.L_0811e77c:
	.4byte 0x00000000
.L_0811e780:
	movs r2, #29
	subs r2, r2, r5
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #16
	mov r2, r10
	str r3, [r2, #12]
	movs r3, #0
	str r3, [r2, #40]
	movs r1, #136
	ldrh r3, [r2, #6]
	lsls r1, r1, #6
	adds r1, #34
	adds r3, r3, r1
	strh r3, [r2, #6]
	cmp r5, #15
	bgt .L_0811e7ac
	ldr r3, .L_0811e7c8
	subs r3, r3, r5
	lsls r3, r3, #8
	orrs r3, r5
	strh r3, [r6]
.L_0811e7ac:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #29
	ble .L_0811e780
	mov r3, r8
	ldrb r0, [r3]
	movs r1, #0
	bl BattlePres_SetActorRecordMode
	mov r0, r9
	b .L_0811e7cc
	.2byte 0x0000
.L_0811e7c8:
	.4byte 0x00000010
.L_0811e7cc:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
