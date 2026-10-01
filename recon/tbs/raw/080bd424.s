.syntax unified
	.thumb
	.global BattleCommand_SelectAutomatic
	.thumb_func
BattleCommand_SelectAutomatic:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	adds r7, r0, #0
	movs r1, #0
	ldrsh r0, [r7, r1]
	bl Owner_GetStateFar
	movs r3, #0
	movs r2, #1
	str r3, [sp, #12]
	ldr r3, .L_080bd678
	mov r11, r0
	str r2, [sp, #16]
	str r2, [sp, #8]
	add r3, r11
	movs r0, #1
	ldrb r3, [r3]
	negs r0, r0
	mov r10, r0
	cmp r3, #0
	beq .L_080bd45e
	b .L_080bd792
.L_080bd45e:
	ldr r1, [sp, #24]
	cmp r1, #0
	beq .L_080bd46e
	movs r2, #6
	ldrsh r3, [r7, r2]
	cmp r3, #4
	beq .L_080bd46e
	b .L_080bd792
.L_080bd46e:
	movs r3, #148
	lsls r3, r3, #1
	add r3, r11
	ldrb r0, [r3]
	bl Owner_GetRecordFar
	str r0, [sp, #20]
	ldr r1, [sp, #20]
	movs r3, #144
	lsls r3, r3, #1
	adds r0, #54
	adds r1, #55
	add r3, r11
	str r0, [sp, #4]
	str r1, [sp, #0]
	mov r8, r3
.L_080bd48e:
	ldr r2, [sp, #4]
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #6
	bhi .L_080bd534
	ldr r2, .L_080bd67c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080bd4a0:
	.4byte .L_080bd4bc
	.4byte .L_080bd4c4
	.4byte .L_080bd4cc
	.4byte .L_080bd4da
	.4byte .L_080bd504
	.4byte .L_080bd530
	.4byte .L_080bd534
.L_080bd4bc:
	add r3, sp, #28
	ldr r0, .L_080bd680
	mov r9, r3
	b .L_080bd4d2
.L_080bd4c4:
	add r1, sp, #28
	ldr r0, .L_080bd684
	mov r9, r1
	b .L_080bd4d2
.L_080bd4cc:
	add r2, sp, #28
	ldr r0, .L_080bd688
	mov r9, r2
.L_080bd4d2:
	bl Battle_SelectWeightedIndex
	mov r10, r0
	b .L_080bd534
.L_080bd4da:
	mov r3, r8
	ldr r2, [r3]
	lsls r3, r2, #31
	cmp r3, #0
	bne .L_080bd508
	bl BattleRandom16Far
	mov r1, r8
	movs r3, #7
	ldrb r2, [r1]
	ands r0, r3
	movs r3, #15
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #1
	movs r2, #1
	orrs r3, r0
	orrs r3, r2
	strb r3, [r1]
	ldr r2, [r1]
	b .L_080bd508
.L_080bd504:
	mov r3, r8
	ldr r2, [r3]
.L_080bd508:
	lsls r3, r2, #28
	ldr r0, [sp, #24]
	lsrs r3, r3, #29
	mov r10, r3
	cmp r0, #0
	beq .L_080bd534
	mov r2, r10
	movs r3, #7
	adds r2, #1
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3]
	movs r3, #15
	negs r3, r3
	lsls r2, r2, #1
	ands r3, r1
	orrs r3, r2
	mov r0, r8
	strb r3, [r0]
	b .L_080bd534
.L_080bd530:
	movs r1, #1
	add r10, r1
.L_080bd534:
	ldr r2, [sp, #0]
	ldrb r6, [r2]
	mov r3, r10
	asrs r6, r3
	mov r1, r10
	movs r3, #1
	ands r6, r3
	ldr r2, [sp, #20]
	lsls r3, r1, #1
	adds r3, #56
	ldr r0, [sp, #16]
	ldrh r3, [r2, r3]
	ands r6, r0
	mov r9, r3
	movs r3, #4
	strh r3, [r7, #6]
	cmp r6, #0
	beq .L_080bd5be
	ldr r3, [sp, #24]
	cmp r3, #0
	beq .L_080bd5be
	mov r1, r11
	adds r1, #216
	ldrh r2, [r1]
	ldr r3, .L_080bd68c
	ands r3, r2
	cmp r3, #0
	bne .L_080bd586
	ldr r3, [sp, #20]
	adds r3, #53
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r6, #0
	cmp r3, #0
	bne .L_080bd586
	movs r3, #2
	strh r3, [r7, #6]
	ldr r3, .L_080bd690
	strh r3, [r7, #8]
	b .L_080bd792
.L_080bd586:
	cmp r6, #0
	beq .L_080bd5ba
	ldrh r0, [r1]
	bl Item_Get
	adds r5, r0, #0
	ldrb r3, [r5, #12]
	cmp r3, #1
	bne .L_080bd5b4
	ldrh r0, [r5, #40]
	bl Ability_GetData
	movs r3, #2
	ldrh r5, [r5, #40]
	strh r3, [r7, #6]
	ldrb r3, [r0, #1]
	movs r2, #0
	mov r9, r5
	strh r2, [r7, #8]
	cmp r3, #2
	bgt .L_080bd5b4
	cmp r3, #1
	bge .L_080bd5b6
.L_080bd5b4:
	movs r6, #0
.L_080bd5b6:
	cmp r6, #0
	bne .L_080bd5be
.L_080bd5ba:
	movs r0, #0
	str r0, [sp, #16]
.L_080bd5be:
	ldr r1, [sp, #8]
	cmp r1, #0
	bne .L_080bd5c6
	b .L_080bd766
.L_080bd5c6:
	mov r0, r9
	bl Ability_GetData
	adds r5, r0, #0
	ldrb r3, [r5, #3]
	cmp r3, #47
	beq .L_080bd5ee
	cmp r3, #47
	bgt .L_080bd5de
	cmp r3, #46
	beq .L_080bd5e4
	b .L_080bd606
.L_080bd5de:
	cmp r3, #49
	beq .L_080bd5f8
	b .L_080bd606
.L_080bd5e4:
	movs r3, #3
	strh r3, [r7, #6]
	movs r2, #0
	ldrsh r0, [r7, r2]
	b .L_080bd600
.L_080bd5ee:
	movs r3, #7
	strh r3, [r7, #6]
	movs r3, #0
	ldrsh r0, [r7, r3]
	b .L_080bd600
.L_080bd5f8:
	movs r3, #99
	strh r3, [r7, #6]
	movs r1, #0
	ldrsh r0, [r7, r1]
.L_080bd600:
	bl Battle_FindTaggedSlotByValue
	strh r0, [r7, #10]
.L_080bd606:
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_080bd61e
	movs r0, #6
	ldrsh r3, [r7, r0]
	ldrh r2, [r7, #6]
	cmp r3, #3
	beq .L_080bd620
	cmp r3, #7
	beq .L_080bd61c
	b .L_080bd792
.L_080bd61c:
	b .L_080bd620
.L_080bd61e:
	ldrh r2, [r7, #6]
.L_080bd620:
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	beq .L_080bd69a
	mov r0, r9
	bl Ability_CheckStatusOrSpecialId
	cmp r0, #0
	beq .L_080bd674
	movs r3, #1
	mov r2, r9
	mov r1, r11
	strh r3, [r7, #6]
	strh r2, [r7, #8]
	movs r0, #58
	ldrsh r3, [r1, r0]
	ldrb r2, [r5, #9]
	cmp r2, r3
	ble .L_080bd658
	ldr r3, [sp, #20]
	adds r3, #53
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080bd658
	b .L_080bd770
.L_080bd658:
	ldr r3, .L_080bd694
	add r3, r11
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080bd698
	ldr r3, [sp, #20]
	adds r3, #53
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_080bd672
	b .L_080bd770
.L_080bd672:
	b .L_080bd698
.L_080bd674:
	ldrh r2, [r7, #6]
	b .L_080bd69a
.L_080bd678:
	.4byte 0x00000129
.L_080bd67c:
	.4byte .L_080bd4a0
.L_080bd680:
	.4byte HpDmgFalloff + 0x18
.L_080bd684:
	.4byte HpDmgFalloff + 0x20
.L_080bd688:
	.4byte HpDmgFalloff + 0x28
.L_080bd68c:
	.4byte 0x000001ff
.L_080bd690:
	.4byte 0x000001fd
.L_080bd694:
	.4byte 0x0000013d
.L_080bd698:
	ldr r2, .L_080bd6c0
.L_080bd69a:
	lsls r3, r2, #16
	asrs r2, r3, #16
	cmp r2, #99
	bne .L_080bd6ae
	movs r3, #164
	lsls r3, r3, #1
	add r3, r11
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080bd770
.L_080bd6ae:
	ldr r3, [sp, #24]
	cmp r3, #0
	beq .L_080bd6c4
	cmp r2, #3
	beq .L_080bd770
	cmp r2, #7
	beq .L_080bd770
	b .L_080bd6c4
	.2byte 0x0000
.L_080bd6c0:
	.4byte 0x00000001
.L_080bd6c4:
	cmp r2, #4
	bne .L_080bd6d6
	mov r0, r9
	mov r1, r9
	movs r3, #0
	strh r0, [r7, #8]
	cmp r1, #1
	bne .L_080bd6d6
	strh r3, [r7, #6]
.L_080bd6d6:
	ldrb r3, [r5, #8]
	strh r3, [r7, #12]
	ldrb r3, [r5]
	cmp r3, #2
	beq .L_080bd6f2
	cmp r3, #2
	bgt .L_080bd6ea
	cmp r3, #1
	beq .L_080bd722
	b .L_080bd758
.L_080bd6ea:
	cmp r3, #3
	beq .L_080bd74c
	cmp r3, #4
	bne .L_080bd758
.L_080bd6f2:
	movs r2, #0
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	bl BattleTarget_SelectForAction
	movs r3, #2
	negs r3, r3
	cmp r0, r3
	bne .L_080bd712
	ldrh r3, [r7]
	movs r0, #0
	cmp r3, #7
	bhi .L_080bd70e
	movs r0, #1
.L_080bd70e:
	bl BattleTarget_SelectRandomPosition
.L_080bd712:
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_080bd766
	movs r2, #0
	strh r0, [r7, #10]
	str r2, [sp, #8]
	b .L_080bd766
.L_080bd722:
	adds r1, r5, #0
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl BattleTarget_SelectForAction
	movs r1, #2
	negs r1, r1
	cmp r0, r1
	bne .L_080bd742
	ldrh r3, [r7]
	movs r0, #0
	cmp r3, #7
	bhi .L_080bd73e
	movs r0, #1
.L_080bd73e:
	bl BattleTarget_SelectRandomPosition
.L_080bd742:
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_080bd766
	b .L_080bd760
.L_080bd74c:
	movs r1, #0
	ldrsh r0, [r7, r1]
	bl Battle_FindTaggedSlotByValue
	strh r0, [r7, #10]
	b .L_080bd766
.L_080bd758:
	movs r2, #0
	ldrsh r0, [r7, r2]
	bl Battle_FindTaggedSlotByValue
.L_080bd760:
	movs r3, #0
	strh r0, [r7, #10]
	str r3, [sp, #8]
.L_080bd766:
	ldr r0, [sp, #24]
	cmp r0, #0
	bne .L_080bd770
	movs r1, #0
	str r1, [sp, #8]
.L_080bd770:
	ldr r2, [sp, #8]
	cmp r2, #0
	beq .L_080bd784
	ldr r3, [sp, #12]
	cmp r3, #16
	ble .L_080bd784
	movs r3, #3
	movs r0, #0
	strh r3, [r7, #6]
	str r0, [sp, #8]
.L_080bd784:
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	adds r1, #1
	str r1, [sp, #12]
	cmp r2, #0
	beq .L_080bd792
	b .L_080bd48e
.L_080bd792:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
