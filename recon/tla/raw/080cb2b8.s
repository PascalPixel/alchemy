.syntax unified
	.thumb
	.global Func_080cb2b8
	.thumb_func
Func_080cb2b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	str r2, [sp, #24]
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r0
	ldr r0, [r3, #108]
	ldr r3, [r3, #32]
	mov r8, r1
	mov r9, r0
	str r3, [sp, #20]
	bl Func_080cdf5c
	str r0, [sp, #16]
	bl Func_080cb8e8
	movs r1, #0
	mov r2, r9
	str r0, [sp, #12]
	str r1, [sp, #8]
	cmp r2, #0
	bne .L_080cb2f4
	b .L_080cb6b8
.L_080cb2f4:
	bl Party_CountActiveOwnersFar
	movs r6, #0
	str r0, [sp, #4]
	cmp r6, r0
	bcs .L_080cb320
	ldr r3, .L_080cb504
	movs r4, #134
	lsls r4, r4, #2
	add r5, sp, #28
	adds r7, r3, r4
.L_080cb30a:
	ldrb r0, [r7]
	bl Owner_GetState
	ldrh r3, [r0, #56]
	adds r6, #1
	strh r3, [r5]
	ldr r0, [sp, #4]
	adds r7, #1
	adds r5, #2
	cmp r6, r0
	bcc .L_080cb30a
.L_080cb320:
	movs r3, #197
	lsls r3, r3, #1
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cb35e
	mov r3, r8
	cmp r3, #0
	bge .L_080cb33a
	ldr r3, .L_080cb508
	add r3, r8
.L_080cb33a:
	asrs r2, r3, #21
	movs r1, #31
	mov r3, r10
	ands r2, r1
	cmp r3, #0
	bge .L_080cb34a
	ldr r3, .L_080cb508
	add r3, r10
.L_080cb34a:
	asrs r3, r3, #21
	ands r3, r1
	lsls r3, r3, #5
	ldr r1, .L_080cb50c
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r0, r3, r1
	ldrb r5, [r0, #2]
	movs r7, #0
	b .L_080cb39c
.L_080cb35e:
	mov r2, r11
	cmp r2, #2
	bhi .L_080cb376
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r4, #156
	ldr r1, [sp, #20]
	lsls r3, r3, #3
	lsls r4, r4, #1
	adds r3, r3, r4
	ldr r0, [r1, r3]
	b .L_080cb378
.L_080cb376:
	ldr r0, .L_080cb510
.L_080cb378:
	mov r3, r8
	cmp r3, #0
	bge .L_080cb382
	ldr r3, .L_080cb514
	add r3, r8
.L_080cb382:
	asrs r2, r3, #20
	mov r3, r10
	cmp r3, #0
	bge .L_080cb38e
	ldr r3, .L_080cb514
	add r3, r10
.L_080cb38e:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r0, r0, r3
	ldrb r5, [r0, #2]
	ldrb r7, [r0, #3]
.L_080cb39c:
	movs r2, #210
	lsls r2, r2, #1
	add r2, r9
	ldr r3, [r2]
	movs r1, #212
	lsls r1, r1, #1
	add r1, r9
	str r3, [r1]
	str r0, [r2]
	cmp r7, #0
	beq .L_080cb3bc
	mov r0, r8
	ldr r1, [sp, #24]
	mov r2, r10
	bl Func_080cb1dc
.L_080cb3bc:
	subs r3, r5, #1
	cmp r3, #229
	bhi .L_080cb3ca
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r5, [r3]
.L_080cb3ca:
	movs r3, #226
	ands r3, r7
	cmp r3, #0
	beq .L_080cb3da
	movs r3, #171
	lsls r3, r3, #1
	add r3, r9
	strh r7, [r3]
.L_080cb3da:
	ldr r4, .L_080cb504
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r4, r2
	ldrb r3, [r3]
	cmp r3, #1
	bls .L_080cb3f4
	cmp r3, #6
	beq .L_080cb3f4
	cmp r3, #7
	beq .L_080cb3f4
	b .L_080cb57a
.L_080cb3f4:
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_080cb3fc
	b .L_080cb57a
.L_080cb3fc:
	ldr r3, [r3, #56]
	movs r0, #128
	lsls r0, r0, #24
	cmp r3, r0
	bne .L_080cb408
	b .L_080cb57a
.L_080cb408:
	ldr r1, [sp, #12]
	movs r0, #104
	adds r0, #255
	movs r6, #0
	ldr r5, [r1, #48]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cb41c
	lsls r5, r5, #1
.L_080cb41c:
	movs r3, #197
	lsls r3, r3, #1
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cb438
	ldr r0, [sp, #12]
	adds r1, r5, #0
	adds r0, #8
	bl BattleFx_ApplyLookupResult
	b .L_080cb44a
.L_080cb438:
	movs r3, #4
	ands r3, r7
	cmp r3, #0
	beq .L_080cb442
	movs r6, #1
.L_080cb442:
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectRuntime_LookupByTableEntry
.L_080cb44a:
	movs r3, #178
	lsls r3, r3, #1
	add r3, r9
	strh r0, [r3]
	movs r3, #206
	ldr r2, [sp, #12]
	lsls r3, r3, #1
	add r3, r9
	ldr r0, [r3]
	ldr r1, [r2, #48]
	ldr r3, .L_080cb518
	mov lr, r3
	.2byte 0xf800
	cmp r6, #0
	bne .L_080cb46e
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r0, r3, #1
.L_080cb46e:
	movs r2, #208
	lsls r2, r2, #1
	add r2, r9
	ldr r3, [r2]
	adds r0, r3, r0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	str r0, [r2]
	cmp r0, r3
	ble .L_080cb4b2
	adds r1, r0, #0
	cmp r0, #0
	bge .L_080cb48c
	adds r1, r0, r3
.L_080cb48c:
	ands r0, r3
	asrs r1, r1, #16
	str r0, [r2]
	adds r0, r1, #0
	movs r1, #0
	bl Func_080cb6c8
	bl BattlePlacement_UpdateTimedEntriesFar
	cmp r0, #0
	beq .L_080cb4a8
	movs r0, #139
	bl Audio_PlayCue
.L_080cb4a8:
	bl Event_ClearValidPackedIds
	bl BattleParty_ApplyStatusDamage
	str r0, [sp, #8]
.L_080cb4b2:
	ldr r4, .L_080cb504
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #78
	adds r3, r4, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080cb538
	movs r1, #8
	adds r3, r1, #0
	ands r3, r7
	cmp r3, #0
	beq .L_080cb538
	movs r3, #212
	lsls r3, r3, #1
	add r3, r9
	ldr r3, [r3]
	ldrb r2, [r3, #3]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080cb51c
	movs r2, #128
	ldr r3, [sp, #12]
	lsls r2, r2, #2
	adds r2, #82
	adds r1, r4, r2
	ldr r2, [r3, #48]
	cmp r2, #0
	bge .L_080cb4f8
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r2, r2, r0
.L_080cb4f8:
	ldrh r3, [r1]
	asrs r2, r2, #16
	adds r3, r3, r2
	strh r3, [r1]
	b .L_080cb538
	.2byte 0x0000
.L_080cb504:
	.4byte gPartyState
.L_080cb508:
	.4byte 0x001fffff
.L_080cb50c:
	.4byte gMapBlocks
.L_080cb510:
	.4byte gMapCellBuffer
.L_080cb514:
	.4byte 0x000fffff
.L_080cb518:
	.4byte IwramMulQ16
.L_080cb51c:
	movs r1, #147
	lsls r1, r1, #2
	adds r3, r4, r1
	ldrh r3, [r3]
	movs r0, #128
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsls r0, r0, #2
	lsrs r3, r3, #31
	adds r2, r2, r3
	adds r0, #82
	asrs r2, r2, #1
	adds r3, r4, r0
	strh r2, [r3]
.L_080cb538:
	movs r2, #153
	lsls r2, r2, #2
	adds r1, r4, r2
	ldr r2, [r1]
	cmp r2, #0
	beq .L_080cb57a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #94
	adds r3, r4, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	beq .L_080cb57a
	ldr r0, [sp, #12]
	ldr r3, [r0, #48]
	subs r3, r2, r3
	str r3, [r1]
	cmp r3, #0
	bgt .L_080cb57a
	movs r3, #1
	str r3, [r1]
	movs r2, #179
	lsls r2, r2, #1
	add r2, r9
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bne .L_080cb57a
	movs r3, #128
	lsls r3, r3, #6
	adds r3, #150
	strh r3, [r2]
.L_080cb57a:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #78
	adds r3, r4, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	bne .L_080cb5da
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #82
	adds r5, r4, r1
	ldrh r3, [r5]
	subs r2, #2
	adds r3, #1
	strh r3, [r5]
	adds r6, r4, r2
	ldrh r0, [r6]
	lsls r3, r3, #16
	lsls r2, r0, #16
	asrs r1, r2, #16
	lsrs r2, r2, #31
	adds r1, r1, r2
	asrs r3, r3, #16
	asrs r1, r1, #1
	cmp r3, r1
	bne .L_080cb5c0
	movs r1, #2
	ldr r0, [sp, #16]
	adds r1, #255
	str r4, [sp, #0]
	bl Func_080d489c
	ldrh r0, [r6]
	ldr r4, [sp, #0]
.L_080cb5c0:
	movs r3, #0
	ldrsh r2, [r5, r3]
	lsls r3, r0, #16
	asrs r3, r3, #16
	cmp r2, r3
	bne .L_080cb5da
	movs r1, #128
	lsls r1, r1, #1
	ldr r0, [sp, #16]
	str r4, [sp, #0]
	bl Func_080d489c
	ldr r4, [sp, #0]
.L_080cb5da:
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #147
	adds r1, #82
	lsls r2, r2, #2
	adds r0, r4, r1
	adds r3, r4, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r1, #0
	ldrsh r2, [r0, r1]
	cmp r2, r3
	blt .L_080cb61c
	movs r2, #148
	lsls r2, r2, #2
	adds r3, r4, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	strh r3, [r0]
	movs r0, #255
	movs r3, #128
	lsls r3, r3, #1
	ands r0, r1
	negs r0, r0
	ands r1, r3
	str r4, [sp, #0]
	bl BattleParty_ApplyHealthDelta
	ldr r3, [sp, #8]
	ldr r4, [sp, #0]
	adds r3, #1
	str r3, [sp, #8]
.L_080cb61c:
	ldr r0, [sp, #8]
	cmp r0, #0
	beq .L_080cb6b8
	movs r3, #182
	lsls r3, r3, #1
	movs r2, #0
	add r3, r9
	strh r2, [r3]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
	movs r1, #129
	lsls r1, r1, #1
	ldr r0, [sp, #16]
	str r4, [sp, #0]
	bl Func_080d489c
	ldr r1, [sp, #4]
	movs r6, #0
	ldr r4, [sp, #0]
	cmp r6, r1
	bcs .L_080cb6b8
	movs r2, #181
	lsls r2, r2, #1
	movs r7, #183
	movs r3, #134
	add r2, r9
	lsls r7, r7, #1
	lsls r3, r3, #2
	mov r8, r2
	add r7, r9
	adds r5, r4, r3
.L_080cb65e:
	ldrb r0, [r5]
	bl Owner_GetState
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_080cb674
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
	b .L_080cb6ae
.L_080cb674:
	add r3, sp, #28
	lsls r2, r6, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080cb6ae
	movs r1, #182
	lsls r1, r1, #1
	add r1, r9
	ldrh r3, [r1]
	mov r4, r9
	adds r2, r3, #1
	strh r2, [r1]
	lsls r3, r3, #16
	movs r2, #184
	lsls r2, r2, #1
	asrs r3, r3, #15
	adds r3, r3, r2
	ldrb r2, [r5]
	mov r1, r8
	strh r2, [r4, r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_080cb6ae:
	ldr r4, [sp, #4]
	adds r6, #1
	adds r5, #1
	cmp r6, r4
	bcc .L_080cb65e
.L_080cb6b8:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
