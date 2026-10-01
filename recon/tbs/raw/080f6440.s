.syntax unified
	.thumb
	.global ReelGame_RunFrame
ReelGame_RunFrame:
	.global Unnamed_080f6440
	.thumb_func
Unnamed_080f6440:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080f64ec
	ldr r7, [r3]
	subs r3, #24
	ldr r3, [r3]
	movs r1, #128
	sub sp, #40
	lsls r1, r1, #3
	movs r0, #0
	str r3, [sp, #36]
	str r1, [sp, #28]
	str r0, [sp, #32]
	bl Random16
	ldr r3, .L_080f64f0
	ldr r2, .L_080f64f4
	ldrh r1, [r3, #10]
	ands r2, r1
	strh r2, [r3, #10]
	ldr r2, .L_080f64f8
	ldrh r1, [r3, #10]
	ands r2, r1
	strh r2, [r3, #10]
	ldrh r2, [r3, #10]
	movs r2, #155
	lsls r2, r2, #3
	adds r0, r7, r2
	ldr r1, .L_080f64fc
	ldr r2, .L_080f6500
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080f6504
	adds r5, r7, #0
	ldr r4, [r3]
	adds r5, #156
	ldrh r0, [r5]
	lsls r4, r4, #16
	ldr r1, [r3]
	adds r6, r7, #0
	lsrs r3, r4, #16
	bics r3, r0
	adds r6, #160
	strh r3, [r6]
	movs r3, #158
	adds r3, r3, r7
	movs r2, #240
	mov r8, r3
	ands r1, r2
	mov r0, r8
	strh r1, [r0]
	ldrh r3, [r5]
	ands r2, r3
	cmp r2, r1
	bne .L_080f650c
	adds r1, r7, #0
	adds r1, #162
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #12
	bls .L_080f64ca
	movs r3, #12
	strh r3, [r1]
	ldr r2, .L_080f64e8
.L_080f64ca:
	adds r3, r2, #0
	cmp r3, #0
	bne .L_080f64d6
	movs r3, #4
	strh r3, [r1]
	b .L_080f6514
.L_080f64d6:
	ldr r0, .L_080f6508
	adds r3, r2, r0
	strh r3, [r1]
	add r1, sp, #32
	ldrh r1, [r1]
	mov r2, r8
	strh r1, [r2]
	b .L_080f6514
	.2byte 0x0000
.L_080f64e8:
	.4byte 0x0000000c
.L_080f64ec:
	.4byte gTransitionWork + 0x4
.L_080f64f0:
	.4byte 0x040000b0
.L_080f64f4:
	.4byte 0x0000c5ff
.L_080f64f8:
	.4byte 0x00007fff
.L_080f64fc:
	.4byte 0x04000054
.L_080f6500:
	.4byte 0xa2600001
.L_080f6504:
	.4byte gKeysHeld
.L_080f6508:
	.4byte 0x0000ffff
.L_080f650c:
	adds r2, r7, #0
	adds r2, #162
	movs r3, #12
	strh r3, [r2]
.L_080f6514:
	lsrs r3, r4, #16
	strh r3, [r5]
	ldr r3, .L_080f65e8
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	beq .L_080f6526
	bl .L_080f6dc2
.L_080f6526:
	adds r2, r7, #0
	adds r2, #140
	str r2, [sp, #12]
	str r2, [sp, #24]
	ldr r5, [r2]
	cmp r5, #0
	beq .L_080f6536
	b .L_080f6670
.L_080f6536:
	movs r0, #228
	bl PartyInventory_CountItemFar
	movs r3, #152
	adds r3, r3, r7
	ldr r1, .L_080f65ec
	mov r9, r0
	ldr r0, [r3]
	mov r11, r3
	mov r4, r9
	adds r5, r7, r1
	mov r3, r10
	subs r0, r4, r0
	ldr r2, [r5]
	movs r1, #2
	str r3, [sp, #0]
	movs r3, #64
	bl UiNumber_DrawAt
	mov r4, r11
	movs r3, #8
	ldr r2, [r5]
	ldr r0, [r4]
	movs r1, #2
	str r3, [sp, #0]
	movs r3, #64
	bl UiNumber_DrawAt
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	mov r8, r11
	cmp r3, #0
	beq .L_080f65a4
	ldr r0, [sp, #12]
	ldr r2, .L_080f65f0
	movs r1, #144
	movs r3, #10
	lsls r1, r1, #1
	str r3, [r0]
	adds r2, r2, r1
	movs r3, #254
	strb r3, [r2]
	movs r2, #153
	lsls r2, r2, #3
	adds r3, r7, r2
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #148
	adds r3, r3, r7
	mov r8, r3
	bl .L_080f6e26
.L_080f65a4:
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080f65ca
	mov r4, r11
	ldr r3, [r4]
	cmp r3, #3
	bgt .L_080f65c4
	cmp r9, r3
	ble .L_080f65c4
	adds r3, #1
	str r3, [r4]
	movs r0, #111
	bl AudioCommand_PlayFar
	b .L_080f65ca
.L_080f65c4:
	movs r0, #113
	bl AudioCommand_PlayFar
.L_080f65ca:
	ldrh r2, [r6]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080f65fa
	mov r0, r8
	ldr r3, [r0]
	cmp r3, #1
	ble .L_080f65f4
	subs r3, #1
	str r3, [r0]
	movs r0, #111
	bl AudioCommand_PlayFar
	b .L_080f65fa
.L_080f65e8:
	.4byte gDebugPaused
.L_080f65ec:
	.4byte 0x000004cc
.L_080f65f0:
	.4byte gCell + 0xc
.L_080f65f4:
	movs r0, #113
	bl AudioCommand_PlayFar
.L_080f65fa:
	ldr r2, .L_080f663c
	ldr r3, .L_080f6634
	strh r3, [r2]
	ldr r3, .L_080f6638
	adds r2, #2
	strh r3, [r2]
	movs r1, #1
	ldrh r2, [r6]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080f6614
	b .L_080f6dd2
.L_080f6614:
	ldr r2, [sp, #24]
	str r1, [r2]
	ldr r3, [sp, #36]
	ldr r4, .L_080f6640
	movs r0, #153
	adds r2, r3, r4
	lsls r0, r0, #3
	movs r3, #0
	str r3, [r2]
	adds r3, r7, r0
	ldr r0, [r3]
	bl UiWork_FinalizeFar
	mov r1, r8
	ldr r3, [r1]
	b .L_080f6644
.L_080f6634:
	.4byte 0x00003fd0
.L_080f6638:
	.4byte 0x00000010
.L_080f663c:
	.4byte 0x04000050
.L_080f6640:
	.4byte 0x0000778c
.L_080f6644:
	movs r5, #0
	cmp r3, #0
	beq .L_080f665a
.L_080f664a:
	movs r0, #228
	bl PartyInventory_RemoveFar
	mov r2, r8
	ldr r3, [r2]
	adds r5, #1
	cmp r5, r3
	bne .L_080f664a
.L_080f665a:
	ldr r4, .L_080f6970
	adds r3, r7, r4
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r0, #152
	lsls r0, r0, #1
	bl AudioCommand_PlayFar
	b .L_080f6dd2
.L_080f6670:
	cmp r5, #5
	beq .L_080f6676
	b .L_080f691c
.L_080f6676:
	adds r4, r7, #0
	movs r1, #0
	adds r4, #168
	mov r9, r1
	adds r1, r4, #0
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	ldrb r3, [r7, #25]
	movs r5, #0
	cmp r3, #0
	beq .L_080f66a0
	adds r2, r7, #0
	adds r2, #25
.L_080f6692:
	adds r5, #1
	cmp r5, #5
	beq .L_080f66a4
	adds r2, #28
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_080f6692
.L_080f66a0:
	cmp r5, #5
	bne .L_080f66a8
.L_080f66a4:
	movs r2, #1
	mov r9, r2
.L_080f66a8:
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080f675a
	movs r2, #0
	str r2, [r1]
	ldr r4, [sp, #36]
	ldr r0, .L_080f6974
	adds r3, r4, r0
	str r2, [r3]
	movs r1, #148
	adds r1, r1, r7
	ldr r3, [r1]
	mov r8, r1
	mov r10, r8
	cmp r3, #4
	bne .L_080f66f4
	adds r3, r7, #0
	adds r3, #144
	str r2, [r1]
	str r2, [r3]
	ldr r3, [sp, #24]
	str r2, [r3]
	adds r2, r7, #0
	movs r6, #0
	movs r0, #0
	movs r1, #255
	adds r2, #24
.L_080f66e2:
	ldrb r3, [r2, #2]
	adds r6, #1
	orrs r3, r1
	strb r0, [r2, #1]
	strb r3, [r2, #2]
	adds r2, #28
	cmp r6, #5
	bne .L_080f66e2
	b .L_080f67ac
.L_080f66f4:
	adds r5, r7, #0
	adds r5, #144
	ldr r3, [r5]
	cmp r3, #4
	bgt .L_080f671a
	ldr r0, .L_080f6978
	bl AudioCommand_PlayFar
	ldr r2, [r5]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #24
	adds r3, r7, r3
	ldrb r2, [r3, #1]
	movs r1, #1
	eors r2, r1
	strb r2, [r3, #1]
	b .L_080f67ac
.L_080f671a:
	mov r4, r9
	cmp r4, #0
	bne .L_080f6752
	movs r0, #152
	lsls r0, r0, #1
	bl AudioCommand_PlayFar
	ldr r0, [sp, #24]
	mov r1, r9
	movs r3, #1
	adds r2, r7, #0
	str r3, [r0]
	movs r6, #0
	str r1, [r5]
	adds r2, #24
	movs r1, #255
.L_080f673a:
	ldrb r3, [r2, #2]
	adds r6, #1
	orrs r3, r1
	strb r3, [r2, #2]
	adds r2, #28
	cmp r6, #5
	bne .L_080f673a
	mov r2, r10
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_080f67ac
.L_080f6752:
	movs r0, #113
	bl AudioCommand_PlayFar
	b .L_080f67ac
.L_080f675a:
	mov r3, r8
	ldrh r2, [r3]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080f6780
	adds r5, r7, #0
	adds r5, #144
	ldr r0, [r5]
	movs r1, #6
	adds r0, #1
	bl Math_Mod
	str r0, [r5]
	movs r0, #111
	bl AudioCommand_PlayFar
	mov r4, r8
	ldrh r2, [r4]
.L_080f6780:
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080f67a6
	adds r5, r7, #0
	adds r5, #144
	ldr r0, [r5]
	movs r1, #6
	adds r0, #5
	bl Math_Mod
	str r0, [r5]
	movs r0, #111
	bl AudioCommand_PlayFar
	movs r0, #148
	adds r0, r0, r7
	mov r8, r0
	b .L_080f67ac
.L_080f67a6:
	movs r1, #148
	adds r1, r1, r7
	mov r8, r1
.L_080f67ac:
	ldr r2, [sp, #24]
	ldr r2, [r2]
	mov r10, r2
	cmp r2, #5
	beq .L_080f67b8
	b .L_080f6906
.L_080f67b8:
	adds r3, r7, #0
	adds r3, #144
	ldr r2, [r3]
	cmp r2, #5
	bne .L_080f6870
	mov r3, r9
	cmp r3, #0
	beq .L_080f682c
	movs r4, #195
	lsls r4, r4, #3
	adds r6, r7, r4
	ldr r2, [r6]
	subs r3, r2, #1
	cmp r3, #1
	bls .L_080f6808
	movs r0, #153
	lsls r0, r0, #3
	adds r5, r7, r0
	ldr r0, [r5]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #19
	movs r3, #4
	movs r0, #11
	bl UiWindow_CreateFar
	adds r1, r0, #0
	str r1, [r5]
	movs r3, #0
	ldr r0, .L_080f697c
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #152
	movs r3, #1
	b .L_080f6868
.L_080f6808:
	cmp r2, #1
	beq .L_080f680e
	b .L_080f6dda
.L_080f680e:
	movs r2, #153
	lsls r2, r2, #3
	adds r3, r7, r2
	ldr r1, [r3]
	ldr r0, .L_080f6980
	movs r3, #8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #2
	str r3, [r6]
	movs r3, #152
	adds r3, r3, r7
	mov r11, r3
	b .L_080f6e26
.L_080f682c:
	movs r4, #195
	lsls r4, r4, #3
	adds r6, r7, r4
	ldr r3, [r6]
	cmp r3, #3
	beq .L_080f6864
	movs r0, #153
	lsls r0, r0, #3
	adds r5, r7, r0
	ldr r0, [r5]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #14
	movs r3, #3
	movs r0, #16
	bl UiWindow_CreateFar
	adds r1, r0, #0
	str r1, [r5]
	ldr r0, .L_080f6984
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080f6864:
	movs r1, #152
	movs r3, #3
.L_080f6868:
	adds r1, r1, r7
	str r3, [r6]
	mov r11, r1
	b .L_080f6e26
.L_080f6870:
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrb r3, [r3, #25]
	cmp r3, #0
	bne .L_080f68c2
	movs r2, #195
	lsls r2, r2, #3
	adds r6, r7, r2
	ldr r3, [r6]
	cmp r3, #4
	beq .L_080f68b6
	movs r3, #153
	lsls r3, r3, #3
	adds r5, r7, r3
	ldr r0, [r5]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #7
	movs r3, #3
	movs r0, #23
	bl UiWindow_CreateFar
	adds r1, r0, #0
	str r1, [r5]
	ldr r0, .L_080f6988
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080f68b6:
	movs r4, #152
	movs r3, #4
	adds r4, r4, r7
	str r3, [r6]
	mov r11, r4
	b .L_080f6e26
.L_080f68c2:
	movs r0, #195
	lsls r0, r0, #3
	adds r6, r7, r0
	ldr r3, [r6]
	cmp r3, #5
	beq .L_080f68fa
	movs r1, #153
	lsls r1, r1, #3
	adds r5, r7, r1
	ldr r0, [r5]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #7
	movs r3, #3
	movs r0, #23
	bl UiWindow_CreateFar
	adds r1, r0, #0
	str r1, [r5]
	ldr r0, .L_080f698c
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080f68fa:
	movs r3, #152
	mov r2, r10
	adds r3, r3, r7
	str r2, [r6]
	mov r11, r3
	b .L_080f6e26
.L_080f6906:
	movs r4, #153
	lsls r4, r4, #3
	adds r3, r7, r4
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r0, #152
	adds r0, r0, r7
	mov r11, r0
	b .L_080f6e26
.L_080f691c:
	cmp r5, #2
	bne .L_080f6998
	adds r4, r7, #0
	adds r4, #168
	ldr r3, [r4]
	movs r1, #0
	adds r3, #1
	str r3, [r4]
	str r1, [sp, #28]
	cmp r3, #60
	beq .L_080f6934
	b .L_080f6de2
.L_080f6934:
	ldr r2, [sp, #12]
	movs r3, #3
	str r3, [r2]
	movs r0, #93
	str r4, [sp, #8]
	bl AudioCommand_PlayFar
	ldr r3, [sp, #28]
	ldr r4, [sp, #8]
	ldr r2, .L_080f6990
	str r3, [r4]
	ldr r3, .L_080f6968
	strh r3, [r2]
	ldr r3, .L_080f696c
	adds r2, #2
	strh r3, [r2]
	ldr r4, [sp, #36]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, .L_080f6994
	adds r3, r4, r0
	str r5, [r3]
	adds r2, r4, r1
	movs r3, #75
	str r3, [r2]
	b .L_080f6de2
.L_080f6968:
	.4byte 0x00003f44
.L_080f696c:
	.4byte 0x00001010
.L_080f6970:
	.4byte 0x000004cc
.L_080f6974:
	.4byte 0x0000778c
.L_080f6978:
	.4byte 0x00000131
.L_080f697c:
	.4byte 0x00000912
.L_080f6980:
	.4byte 0x00000913
.L_080f6984:
	.4byte 0x0000090f
.L_080f6988:
	.4byte 0x0000090d
.L_080f698c:
	.4byte 0x0000090e
.L_080f6990:
	.4byte 0x04000050
.L_080f6994:
	.4byte 0x00007784
.L_080f6998:
	cmp r5, #3
	bne .L_080f69c4
	adds r4, r7, #0
	adds r4, #168
	ldr r3, [r4]
	adds r3, #1
	str r3, [r4]
	movs r4, #0
	str r4, [sp, #28]
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080f69b6
	b .L_080f6df0
.L_080f69b6:
	ldr r0, [sp, #12]
	movs r3, #10
	str r3, [r0]
	movs r0, #112
	bl AudioCommand_PlayFar
	b .L_080f6dfe
.L_080f69c4:
	cmp r5, #11
	bne .L_080f6a16
	movs r3, #195
	lsls r3, r3, #3
	adds r5, r7, r3
	ldr r3, [r5]
	cmp r3, #0
	bne .L_080f69ea
	movs r4, #153
	movs r3, #1
	lsls r4, r4, #3
	str r3, [r5]
	adds r3, r7, r4
	ldr r1, [r3]
	ldr r0, .L_080f6d30
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_080f69ea:
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080f69f6
	b .L_080f6dfe
.L_080f69f6:
	ldr r0, [sp, #12]
	movs r3, #5
	mov r1, r10
	str r3, [r0]
	str r1, [r5]
	movs r0, #112
	bl AudioCommand_PlayFar
	movs r2, #153
	lsls r2, r2, #3
	adds r3, r7, r2
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	b .L_080f6e0c
.L_080f6a16:
	cmp r5, #20
	bne .L_080f6a4e
	adds r4, r7, #0
	adds r4, #168
	ldr r3, [r4]
	adds r3, #1
	str r3, [r4]
	cmp r3, #45
	beq .L_080f6a2a
	b .L_080f6e0c
.L_080f6a2a:
	ldr r0, [sp, #12]
	movs r1, #152
	movs r2, #148
	movs r3, #10
	adds r1, r1, r7
	adds r2, r2, r7
	str r3, [r0]
	b .L_080f6e06
.L_080f6a3a:
	bl Random16
	movs r3, #3
	ands r0, r3
	adds r0, #4
	strb r0, [r5, #2]
	ldr r0, .L_080f6d34
	bl AudioCommand_PlayFar
	b .L_080f6adc
.L_080f6a4e:
	cmp r5, #10
	bne .L_080f6a54
	b .L_080f6e1a
.L_080f6a54:
	adds r4, r7, #0
	adds r4, #168
	str r4, [sp, #20]
	ldr r3, [r4]
	cmp r3, #4
	bne .L_080f6a8a
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #17
	movs r2, #12
	movs r3, #3
	movs r0, #18
	str r4, [sp, #8]
	bl UiWindow_CreateFar
	adds r1, r0, #0
	movs r0, #153
	lsls r0, r0, #3
	adds r3, r7, r0
	str r1, [r3]
	ldr r0, .L_080f6d38
	movs r3, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r4, [sp, #8]
	ldr r3, [r4]
.L_080f6a8a:
	cmp r3, #16
	bne .L_080f6a9c
	movs r0, #153
	lsls r0, r0, #1
	str r4, [sp, #8]
	bl AudioCommand_PlayFar
	ldr r4, [sp, #8]
	ldr r3, [r4]
.L_080f6a9c:
	cmp r3, #56
	ble .L_080f6adc
	ldr r3, .L_080f6d3c
	ldr r2, [sp, #36]
	adds r1, r2, r3
	ldr r3, [r1]
	cmp r3, #31
	bgt .L_080f6ab8
	ldrh r2, [r6]
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080f6adc
.L_080f6ab8:
	mov r4, r10
	movs r2, #1
	adds r5, r7, #0
	str r4, [r1]
	movs r6, #0
	negs r2, r2
	adds r5, #24
.L_080f6ac6:
	ldrb r3, [r5, #1]
	cmp r3, #0
	bne .L_080f6ad4
	movs r3, #2
	ldrsb r3, [r5, r3]
	cmp r3, r2
	beq .L_080f6a3a
.L_080f6ad4:
	adds r6, #1
	adds r5, #28
	cmp r6, #5
	bne .L_080f6ac6
.L_080f6adc:
	adds r2, r7, #0
	movs r6, #0
	adds r2, #24
.L_080f6ae2:
	movs r3, #2
	ldrsb r3, [r2, r3]
	ldrb r1, [r2, #2]
	cmp r3, #0
	ble .L_080f6af0
	subs r3, r1, #1
	strb r3, [r2, #2]
.L_080f6af0:
	adds r6, #1
	adds r2, #28
	cmp r6, #5
	bne .L_080f6ae2
	adds r2, r7, #0
	movs r1, #0
	movs r6, #0
	movs r4, #15
	adds r2, #24
	movs r0, #0
.L_080f6b04:
	ldrb r3, [r2, #1]
	cmp r3, #1
	beq .L_080f6b1a
	movs r3, #2
	ldrsb r3, [r2, r3]
	cmp r3, #0
	bne .L_080f6b1c
	ldr r3, [r0, r7]
	ands r3, r4
	cmp r3, #8
	bne .L_080f6b1c
.L_080f6b1a:
	adds r1, #1
.L_080f6b1c:
	adds r6, #1
	adds r2, #28
	adds r0, #28
	cmp r6, #5
	bne .L_080f6b04
	cmp r1, #5
	beq .L_080f6b2c
	b .L_080f6d5c
.L_080f6b2c:
	movs r0, #0
	movs r1, #152
	movs r2, #172
	adds r1, r1, r7
	adds r2, r2, r7
	str r0, [sp, #16]
	mov r10, r0
	mov r11, r1
	mov r8, r2
.L_080f6b3e:
	movs r3, #0
	mov r0, r8
	str r3, [r0]
	mov r1, r11
	ldr r2, [r1]
	mov r9, r3
	movs r3, #3
	movs r4, #1
	subs r3, r3, r2
	negs r4, r4
	cmp r10, r3
	ble .L_080f6bf0
	adds r3, r2, #3
	cmp r10, r3
	bge .L_080f6bf0
	movs r6, #0
	movs r5, #0
.L_080f6b60:
	mov r2, r10
	cmp r2, #0
	bne .L_080f6b76
	ldr r0, [r5, r7]
	cmp r0, #0
	bge .L_080f6b6e
	adds r0, #15
.L_080f6b6e:
	asrs r0, r0, #4
	subs r0, r6, r0
	adds r0, #22
	b .L_080f6b9e
.L_080f6b76:
	mov r3, r10
	cmp r3, #6
	bne .L_080f6b8e
	ldr r0, [r5, r7]
	negs r3, r6
	cmp r0, #0
	bge .L_080f6b86
	adds r0, #15
.L_080f6b86:
	asrs r0, r0, #4
	subs r0, r3, r0
	adds r0, #26
	b .L_080f6b9e
.L_080f6b8e:
	ldr r0, [r5, r7]
	cmp r0, #0
	bge .L_080f6b96
	adds r0, #15
.L_080f6b96:
	mov r1, r10
	asrs r0, r0, #4
	subs r0, r1, r0
	adds r0, #21
.L_080f6b9e:
	movs r1, #21
	str r4, [sp, #8]
	bl Math_Mod
	adds r0, r0, r5
	adds r0, #4
	ldrb r3, [r7, r0]
	ldr r4, [sp, #8]
	cmp r3, #5
	beq .L_080f6bc6
	movs r2, #1
	negs r2, r2
	cmp r4, r2
	bne .L_080f6bbe
	adds r4, r3, #0
	b .L_080f6bc6
.L_080f6bbe:
	cmp r4, r3
	beq .L_080f6bc6
	movs r3, #1
	mov r9, r3
.L_080f6bc6:
	adds r6, #1
	adds r5, #28
	cmp r6, #5
	bne .L_080f6b60
	mov r0, r9
	cmp r0, #0
	bne .L_080f6bf0
	movs r3, #1
	mov r1, r8
	str r3, [r1]
	ldr r3, [sp, #16]
	movs r0, #144
	lsls r0, r0, #1
	adds r2, r3, r0
	ldr r3, .L_080f6d40
	ldr r1, .L_080f6d44
	ldrb r3, [r3, r4]
	strb r3, [r1, r2]
	ldr r2, [sp, #16]
	adds r2, #1
	str r2, [sp, #16]
.L_080f6bf0:
	movs r4, #1
	add r10, r4
	movs r3, #4
	mov r0, r10
	add r8, r3
	cmp r0, #7
	bne .L_080f6b3e
	ldr r1, [sp, #20]
	movs r5, #0
	str r5, [r1]
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_080f6c52
	movs r3, #144
	subs r4, #2
	ldr r0, .L_080f6d44
	lsls r3, r3, #1
	adds r2, r2, r3
	adds r3, r4, #0
	strb r3, [r0, r2]
	ldr r1, [sp, #24]
	movs r3, #2
	str r3, [r1]
	movs r0, #171
	bl AudioCommand_PlayFar
	movs r4, #239
	ldr r3, [sp, #36]
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r0, [sp, #36]
	ldr r1, .L_080f6d48
	adds r3, r0, r1
	str r5, [r3]
	movs r2, #153
	ldr r3, .L_080f6d4c
	lsls r2, r2, #3
	strh r5, [r3]
	adds r3, r7, r2
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #148
	adds r3, r3, r7
	mov r8, r3
	b .L_080f6d68
.L_080f6c52:
	ldr r4, [sp, #24]
	movs r3, #11
	str r3, [r4]
	movs r0, #195
	movs r2, #153
	ldr r1, [sp, #16]
	lsls r2, r2, #3
	lsls r0, r0, #3
	adds r5, r7, r2
	adds r3, r7, r0
	str r1, [r3]
	ldr r0, [r5]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #24
	movs r3, #4
	movs r0, #3
	bl UiWindow_CreateFar
	adds r1, r0, #0
	str r1, [r5]
	movs r3, #0
	ldr r0, .L_080f6d50
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #148
	adds r3, r3, r7
	mov r8, r3
	mov r5, r8
	ldr r3, [r5]
	cmp r3, #4
	bne .L_080f6d68
	movs r0, #228
	bl PartyInventory_CountItemFar
	cmp r0, #0
	ble .L_080f6cae
	ldr r4, [sp, #24]
	movs r3, #20
	str r3, [r4]
	b .L_080f6cb4
.L_080f6cae:
	ldr r1, [sp, #24]
	movs r3, #20
	str r3, [r1]
.L_080f6cb4:
	mov r2, r11
	ldr r3, [r2]
	cmp r3, r0
	ble .L_080f6cbe
	str r0, [r2]
.L_080f6cbe:
	adds r3, r7, #0
	movs r2, #0
	adds r3, #144
	str r2, [r5]
	str r2, [r3]
	adds r2, r7, #0
	movs r6, #0
	movs r0, #0
	movs r1, #255
	adds r2, #24
.L_080f6cd2:
	ldrb r3, [r2, #2]
	adds r6, #1
	orrs r3, r1
	strb r0, [r2, #1]
	strb r3, [r2, #2]
	adds r2, #28
	cmp r6, #5
	bne .L_080f6cd2
	ldr r3, [sp, #36]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r0, [sp, #36]
	ldr r1, .L_080f6d48
	movs r2, #0
	adds r3, r0, r1
	str r2, [r3]
	ldr r3, .L_080f6d4c
	strh r2, [r3]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #12
	movs r3, #4
	movs r0, #18
	bl UiWindow_CreateFar
	ldr r2, .L_080f6d54
	ldr r5, .L_080f6d58
	adds r1, r0, #0
	adds r6, r7, r2
	adds r0, r5, #0
	str r1, [r6]
	movs r2, #0
	movs r3, #8
	subs r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080f6d68
.L_080f6d30:
	.4byte 0x0000090c
.L_080f6d34:
	.4byte 0x00000133
.L_080f6d38:
	.4byte 0x0000090a
.L_080f6d3c:
	.4byte 0x0000778c
.L_080f6d40:
	.4byte Data_080f870c
.L_080f6d44:
	.4byte gCell + 0xc
.L_080f6d48:
	.4byte 0x00007784
.L_080f6d4c:
	.4byte 0x04000050
.L_080f6d50:
	.4byte 0x0000090b
.L_080f6d54:
	.4byte 0x000004cc
.L_080f6d58:
	.4byte 0x00000905
.L_080f6d5c:
	movs r3, #152
	movs r4, #148
	adds r3, r3, r7
	adds r4, r4, r7
	mov r11, r3
	mov r8, r4
.L_080f6d68:
	ldr r0, [sp, #24]
	ldr r3, [r0]
	cmp r3, #1
	bne .L_080f6dac
	movs r6, #0
	adds r1, r7, #0
.L_080f6d74:
	ldrb r3, [r1, #25]
	cmp r3, #0
	bne .L_080f6da4
	movs r3, #26
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_080f6d86
	ldr r3, [r1]
	b .L_080f6d92
.L_080f6d86:
	ldr r2, [r1]
	movs r3, #15
	ands r3, r2
	cmp r3, #8
	beq .L_080f6d98
	adds r3, r2, #0
.L_080f6d92:
	adds r3, #8
	str r3, [r1]
	adds r2, r3, #0
.L_080f6d98:
	movs r3, #168
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_080f6da4
	movs r3, #0
	str r3, [r1]
.L_080f6da4:
	adds r6, #1
	adds r1, #28
	cmp r6, #5
	bne .L_080f6d74
.L_080f6dac:
	ldr r4, [sp, #36]
	ldr r0, .L_080f7108
	adds r3, r4, r0
	ldr r2, [r3]
	adds r2, #1
	str r2, [r3]
	ldr r1, [sp, #20]
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	b .L_080f6e26
.L_080f6dc2:
	adds r2, r7, #0
	movs r3, #152
	movs r4, #148
	adds r2, #140
	adds r3, r3, r7
	adds r4, r4, r7
	str r2, [sp, #12]
	b .L_080f6e14
.L_080f6dd2:
	movs r0, #148
	adds r0, r0, r7
	mov r8, r0
	b .L_080f6e26
.L_080f6dda:
	movs r1, #152
	adds r1, r1, r7
	mov r11, r1
	b .L_080f6e26
.L_080f6de2:
	movs r2, #152
	movs r3, #148
	adds r2, r2, r7
	adds r3, r3, r7
	mov r11, r2
	mov r8, r3
	b .L_080f6e26
.L_080f6df0:
	movs r4, #152
	movs r0, #148
	adds r4, r4, r7
	adds r0, r0, r7
	mov r11, r4
	mov r8, r0
	b .L_080f6e26
.L_080f6dfe:
	movs r1, #152
	movs r2, #148
	adds r1, r1, r7
	adds r2, r2, r7
.L_080f6e06:
	mov r11, r1
	mov r8, r2
	b .L_080f6e26
.L_080f6e0c:
	movs r3, #152
	movs r4, #148
	adds r3, r3, r7
	adds r4, r4, r7
.L_080f6e14:
	mov r11, r3
	mov r8, r4
	b .L_080f6e26
.L_080f6e1a:
	movs r0, #152
	movs r1, #148
	adds r0, r0, r7
	adds r1, r1, r7
	mov r11, r0
	mov r8, r1
.L_080f6e26:
	ldr r2, [sp, #12]
	ldr r3, [r2]
	cmp r3, #5
	bne .L_080f6eb2
	adds r3, r7, #0
	adds r3, #144
	ldr r1, [r3]
	lsls r3, r1, #3
	adds r3, r3, r1
	adds r4, r7, #0
	lsls r3, r3, #2
	adds r4, #168
	adds r5, r3, #0
	ldr r3, [r4]
	movs r2, #15
	ands r3, r2
	adds r5, #36
	movs r6, #128
	movs r0, #0
	cmp r3, #7
	bgt .L_080f6e52
	movs r0, #1
.L_080f6e52:
	cmp r1, #5
	bne .L_080f6e5a
	movs r5, #208
	movs r6, #32
.L_080f6e5a:
	adds r3, r5, #0
	ldr r4, [sp, #28]
	subs r3, #12
	lsls r3, r3, #16
	orrs r3, r4
	adds r4, r6, #0
	adds r4, #8
	ldr r2, .L_080f710c
	orrs r3, r4
	orrs r3, r2
	movs r1, #200
	str r3, [r7, r1]
	lsls r1, r0, #4
	movs r0, #172
	lsls r0, r0, #2
	adds r1, r1, r0
	movs r3, #204
	str r1, [r7, r3]
	adds r3, r5, #0
	ldr r2, [sp, #28]
	adds r3, #12
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, .L_080f7110
	orrs r3, r4
	orrs r3, r2
	movs r0, #208
	str r3, [r7, r0]
	movs r3, #212
	str r1, [r7, r3]
	ldr r4, [sp, #28]
	lsls r3, r5, #16
	ldr r2, .L_080f7114
	orrs r3, r4
	orrs r3, r6
	orrs r3, r2
	movs r1, #216
	str r3, [r7, r1]
	movs r3, #248
	movs r2, #220
	lsls r3, r3, #1
	movs r0, #3
	str r3, [r7, r2]
	str r0, [sp, #32]
.L_080f6eb2:
	ldr r1, [sp, #12]
	ldr r3, [r1]
	cmp r3, #3
	bne .L_080f6f7e
	ldr r3, [sp, #36]
	ldr r0, [sp, #32]
	ldr r2, .L_080f7118
	mov lr, r3
	movs r1, #204
	lsls r3, r0, #3
	adds r4, r7, #0
	adds r1, r1, r3
	adds r5, r3, #0
	movs r6, #0
	adds r4, #168
	mov r10, r2
	mov r12, r1
	adds r5, #200
.L_080f6ed6:
	movs r0, #225
	lsls r0, r0, #7
	add r0, lr
	movs r3, #2
	ldrsh r2, [r0, r3]
	ldr r1, [sp, #28]
	lsls r2, r2, #16
	orrs r2, r1
	movs r1, #6
	ldrsh r3, [r0, r1]
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #255
	ands r3, r1
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #24
	orrs r2, r3
	str r2, [r7, r5]
	mov r2, r10
	ldrb r3, [r2, r6]
	movs r1, #220
	lsls r3, r3, #4
	lsls r1, r1, #2
	movs r2, #240
	lsls r2, r2, #8
	adds r3, r3, r1
	orrs r3, r2
	mov r2, r12
	str r3, [r7, r2]
	ldr r2, [r0, #16]
	ldr r3, [r0, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #7
	adds r2, r2, r3
	str r2, [r0, #16]
	ldr r3, [r4]
	adds r2, r3, #0
	cmp r3, #0
	bge .L_080f6f2e
	adds r2, #255
.L_080f6f2e:
	asrs r2, r2, #8
	lsls r2, r2, #8
	subs r2, r3, r2
	lsls r3, r6, #2
	adds r3, #200
	cmp r2, r3
	bne .L_080f6f46
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #16]
	movs r3, #0
	str r3, [r0, #24]
.L_080f6f46:
	movs r2, #128
	ldr r3, [r0, #4]
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_080f6f68
	ldr r1, [r0, #24]
	str r2, [r0, #4]
	cmp r1, #1
	bgt .L_080f6f64
	ldr r3, [r0, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r0, #16]
.L_080f6f64:
	adds r3, r1, #1
	str r3, [r0, #24]
.L_080f6f68:
	ldr r1, [sp, #32]
	movs r0, #8
	adds r1, #1
	movs r2, #28
	adds r6, #1
	add r12, r0
	adds r5, #8
	str r1, [sp, #32]
	add lr, r2
	cmp r6, #8
	bne .L_080f6ed6
.L_080f6f7e:
	ldr r3, .L_080f710c
	ldr r0, [sp, #32]
	mov r9, r3
	lsls r3, r0, #3
	ldr r4, .L_080f711c
	adds r1, r3, #0
	adds r0, r3, #0
	movs r2, #156
	movs r3, #157
	adds r1, #204
	lsls r2, r2, #3
	lsls r3, r3, #3
	ldr r6, .L_080f7120
	mov r12, r4
	movs r5, #0
	adds r4, r1, #0
	adds r0, #200
	mov r10, r2
	mov lr, r3
.L_080f6fa4:
	ldrb r3, [r6]
	ldr r2, [sp, #28]
	lsls r3, r3, #16
	orrs r3, r2
	mov r2, r12
	ldrb r2, [r2]
	str r2, [sp, #4]
	movs r2, #1
	add r12, r2
	ldr r2, [sp, #4]
	orrs r3, r2
	mov r2, r9
	orrs r3, r2
	adds r6, #1
	str r3, [r7, r0]
	cmp r5, #3
	bgt .L_080f6fcc
	mov r3, r10
	str r3, [r7, r4]
	b .L_080f6fd0
.L_080f6fcc:
	mov r2, lr
	str r2, [r7, r1]
.L_080f6fd0:
	ldr r3, [sp, #32]
	adds r5, #1
	adds r3, #1
	adds r1, #8
	adds r4, #8
	adds r0, #8
	str r3, [sp, #32]
	cmp r5, #14
	bne .L_080f6fa4
	lsls r3, r3, #3
	ldr r4, .L_080f7124
	movs r0, #25
	adds r1, r3, #0
	adds r2, r3, #0
	movs r5, #128
	adds r0, r0, r7
	adds r1, #204
	adds r2, #200
	mov lr, r4
	movs r6, #0
	lsls r5, r5, #14
	mov r12, r0
	adds r4, r1, #0
	mov r9, r2
.L_080f7000:
	mov r3, r12
	ldrb r3, [r3]
	mov r10, r3
	movs r0, #28
	mov r3, r10
	add r12, r0
	cmp r3, #0
	bne .L_080f7024
	ldr r3, [sp, #28]
	mov r0, lr
	orrs r3, r5
	orrs r3, r0
	mov r0, r9
	str r3, [r7, r0]
	movs r3, #140
	lsls r3, r3, #3
	str r3, [r7, r4]
	b .L_080f7034
.L_080f7024:
	ldr r3, [sp, #28]
	mov r0, lr
	orrs r3, r5
	orrs r3, r0
	str r3, [r7, r2]
	movs r3, #144
	lsls r3, r3, #3
	str r3, [r7, r1]
.L_080f7034:
	movs r3, #8
	ldr r0, [sp, #32]
	add r9, r3
	movs r3, #144
	adds r0, #1
	lsls r3, r3, #14
	adds r6, #1
	adds r1, #8
	adds r2, #8
	adds r4, #8
	str r0, [sp, #32]
	adds r5, r5, r3
	cmp r6, #5
	bne .L_080f7000
	ldr r1, [sp, #32]
	ldr r4, .L_080f7128
	lsls r3, r1, #3
	adds r2, r3, #0
	movs r0, #128
	lsls r0, r0, #3
	mov r10, r4
	movs r5, #128
	adds r2, #204
	adds r3, #200
	movs r4, #132
	mov r12, r0
	movs r6, #0
	lsls r5, r5, #14
	adds r0, r2, #0
	mov r9, r3
	mov lr, r8
	lsls r4, r4, #2
.L_080f7074:
	ldr r3, [sp, #28]
	mov r1, r10
	orrs r3, r5
	orrs r3, r1
	mov r1, r9
	str r3, [r7, r1]
	mov r1, lr
	ldr r3, [r1]
	cmp r6, r3
	bne .L_080f7092
	adds r3, r4, #0
	mov r1, r12
	orrs r3, r1
	str r3, [r7, r0]
	b .L_080f70a0
.L_080f7092:
	movs r1, #136
	lsls r1, r1, #2
	lsls r3, r6, #5
	adds r3, r3, r1
	mov r1, r12
	orrs r3, r1
	str r3, [r7, r2]
.L_080f70a0:
	movs r3, #8
	ldr r1, [sp, #32]
	add r9, r3
	movs r3, #128
	adds r1, #1
	lsls r3, r3, #13
	adds r6, #1
	adds r2, #8
	adds r0, #8
	str r1, [sp, #32]
	adds r5, r5, r3
	adds r4, #32
	cmp r6, #5
	bne .L_080f7074
	lsls r3, r1, #3
	movs r4, #0
	mov r10, r4
	adds r0, r3, #0
	adds r4, r3, #0
	movs r5, #5
	adds r4, #204
	adds r0, #200
	adds r1, r3, #0
.L_080f70ce:
	mov r3, r10
	movs r2, #1
	ands r2, r3
	movs r3, #129
	lsls r2, r2, #3
	lsls r3, r3, #2
	subs r3, r3, r2
	ldr r2, .L_080f712c
	ands r3, r2
	ldr r2, [sp, #28]
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, .L_080f7114
	orrs r3, r5
	orrs r3, r2
	str r3, [r7, r0]
	mov r3, r11
	ldr r2, [r3]
	movs r3, #3
	subs r3, r3, r2
	cmp r10, r3
	ble .L_080f7130
	adds r3, r2, #3
	cmp r10, r3
	bge .L_080f7130
	movs r3, #186
	lsls r3, r3, #3
	str r3, [r7, r4]
	b .L_080f713a
.L_080f7108:
	.4byte 0x0000778c
.L_080f710c:
	.4byte 0x80006000
.L_080f7110:
	.4byte 0x90006000
.L_080f7114:
	.4byte 0x80002000
.L_080f7118:
	.4byte Data_080f8712
.L_080f711c:
	.4byte Data_080f8728
.L_080f7120:
	.4byte Data_080f871a
.L_080f7124:
	.4byte 0x8000207c
.L_080f7128:
	.4byte 0x80006003
.L_080f712c:
	.4byte 0x000001ff
.L_080f7130:
	adds r2, r1, #0
	movs r3, #162
	adds r2, #204
	lsls r3, r3, #3
	str r3, [r7, r2]
.L_080f713a:
	ldr r2, [sp, #32]
	movs r3, #1
	adds r2, #1
	add r10, r3
	str r2, [sp, #32]
	mov r2, r10
	adds r4, #8
	adds r0, #8
	adds r1, #8
	adds r5, #16
	cmp r2, #7
	bne .L_080f70ce
	movs r3, #160
	lsls r3, r3, #14
	movs r6, #0
	mov r11, r3
	adds r5, r7, #0
.L_080f715c:
	ldr r0, [sp, #32]
	movs r4, #0
	lsls r3, r0, #3
	movs r1, #204
	mov r10, r4
	adds r1, r1, r3
	adds r4, r3, #0
	mov r9, r11
	mov r8, r1
	adds r4, #200
.L_080f7170:
	mov r2, r10
	lsls r1, r2, #4
	ldr r2, [r5]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080f717e
	adds r3, #15
.L_080f717e:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	adds r3, r1, r3
	mov r0, r9
	adds r3, #4
	ldr r2, .L_080f7300
	orrs r3, r0
	orrs r3, r2
	str r3, [r7, r4]
	ldr r0, [r5]
	cmp r0, #0
	bge .L_080f719a
	adds r0, #15
.L_080f719a:
	mov r1, r10
	asrs r0, r0, #4
	subs r0, r1, r0
	adds r0, #21
	movs r1, #21
	str r4, [sp, #8]
	bl Math_Mod
	adds r0, #4
	ldrb r3, [r5, r0]
	movs r2, #128
	lsls r2, r2, #4
	lsls r3, r3, #4
	orrs r3, r2
	mov r2, r8
	str r3, [r7, r2]
	ldr r0, [sp, #32]
	movs r1, #1
	ldr r4, [sp, #8]
	add r10, r1
	movs r3, #8
	adds r0, #1
	mov r2, r10
	add r8, r3
	adds r4, #8
	str r0, [sp, #32]
	cmp r2, #7
	bne .L_080f7170
	movs r3, #128
	lsls r3, r3, #14
	adds r6, #1
	add r11, r3
	adds r5, #28
	cmp r6, #5
	bne .L_080f715c
	ldr r0, [sp, #12]
	ldr r3, [r0]
	movs r4, #40
	cmp r3, #1
	bne .L_080f7216
	adds r3, r7, #0
	adds r3, #168
	ldr r0, [r3]
	cmp r0, #47
	bgt .L_080f7200
	ldr r3, .L_080f7304
	muls r0, r3
	bl Trig_Sin
	lsls r0, r0, #6
	b .L_080f7210
.L_080f7200:
	cmp r0, #55
	bgt .L_080f7216
	ldr r1, .L_080f7308
	lsls r0, r0, #12
	adds r0, r0, r1
	bl Trig_Sin
	lsls r0, r0, #2
.L_080f7210:
	asrs r0, r0, #16
	adds r4, r0, #0
	adds r4, #40
.L_080f7216:
	ldr r3, [sp, #32]
	ldr r0, [sp, #28]
	lsls r2, r3, #3
	ldr r3, .L_080f730c
	orrs r4, r0
	orrs r4, r3
	adds r1, r2, #0
	movs r3, #160
	adds r1, #200
	adds r2, #204
	lsls r3, r3, #3
	str r4, [r7, r1]
	str r3, [r7, r2]
	ldr r1, [sp, #32]
	adds r1, #1
	lsls r3, r1, #3
	adds r6, r3, #0
	adds r6, #204
	adds r4, r6, #0
	adds r0, r4, #0
	str r1, [sp, #32]
	movs r2, #12
	adds r1, r0, #0
	adds r3, #200
	movs r5, #0
	mov r8, r2
	mov lr, r1
	mov r12, r3
.L_080f724e:
	ldr r3, [sp, #28]
	mov r2, r8
	orrs r3, r2
	ldr r2, .L_080f7310
	orrs r3, r2
	mov r2, r12
	str r3, [r7, r2]
	cmp r5, #0
	bne .L_080f726a
	movs r3, #168
	lsls r3, r3, #3
	mov r2, lr
	str r3, [r7, r2]
	b .L_080f7294
.L_080f726a:
	cmp r5, #1
	bne .L_080f7276
	movs r3, #170
	lsls r3, r3, #3
	str r3, [r7, r1]
	b .L_080f7294
.L_080f7276:
	cmp r5, #6
	bne .L_080f7282
	movs r3, #174
	lsls r3, r3, #3
	str r3, [r7, r0]
	b .L_080f7294
.L_080f7282:
	cmp r5, #7
	bne .L_080f728e
	movs r3, #176
	lsls r3, r3, #3
	str r3, [r7, r4]
	b .L_080f7294
.L_080f728e:
	movs r3, #172
	lsls r3, r3, #3
	str r3, [r7, r6]
.L_080f7294:
	ldr r2, [sp, #32]
	movs r3, #8
	add lr, r3
	add r12, r3
	adds r2, #1
	movs r3, #16
	adds r5, #1
	adds r6, #8
	adds r4, #8
	adds r0, #8
	adds r1, #8
	str r2, [sp, #32]
	add r8, r3
	cmp r5, #8
	bne .L_080f724e
	cmp r2, #128
	beq .L_080f72d4
	lsls r3, r2, #3
	adds r2, r3, #0
	ldr r0, .L_080f7314
	movs r1, #0
	adds r2, #204
	adds r3, #200
.L_080f72c2:
	str r0, [r7, r3]
	str r1, [r7, r2]
	ldr r4, [sp, #32]
	adds r4, #1
	adds r2, #8
	adds r3, #8
	str r4, [sp, #32]
	cmp r4, #128
	bne .L_080f72c2
.L_080f72d4:
	ldr r0, [sp, #32]
	movs r4, #132
	lsls r2, r0, #1
	lsls r4, r4, #24
	adds r0, r7, #0
	movs r1, #224
	ldr r3, .L_080f7318
	adds r0, #200
	lsls r1, r1, #19
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080f7300:
	.4byte 0x80006000
.L_080f7304:
	.4byte 0x000002aa
.L_080f7308:
	.4byte 0xfffd0000
.L_080f730c:
	.4byte 0x80d06000
.L_080f7310:
	.4byte 0x80ce6000
.L_080f7314:
	.4byte 0x40f02000
.L_080f7318:
	.4byte 0x040000d4
