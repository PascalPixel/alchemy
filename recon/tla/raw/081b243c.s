.syntax unified
	.thumb
	.global Func_081b243c
	.thumb_func
Func_081b243c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #180
	ldr r7, [r3]
	ldr r2, [r2, #92]
	sub sp, #40
	movs r0, #128
	movs r1, #168
	adds r1, r1, r7
	str r2, [sp, #36]
	lsls r0, r0, #3
	movs r2, #0
	str r0, [sp, #32]
	str r2, [r1]
	mov r11, r1
	mov r8, r2
	bl Random16
	ldr r3, .L_081b24d4
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
	mov r10, r3
	ands r1, r2
	mov r0, r10
	strh r1, [r0]
	ldrh r3, [r5]
	ands r2, r3
	cmp r2, r1
	bne .L_081b24d8
	adds r1, r7, #0
	adds r1, #162
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #12
	bls .L_081b24b0
	movs r3, #12
	strh r3, [r1]
	ldr r2, .L_081b24d0
.L_081b24b0:
	adds r3, r2, #0
	cmp r3, #0
	bne .L_081b24bc
	movs r3, #4
	strh r3, [r1]
	b .L_081b24e0
.L_081b24bc:
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	mov r2, r10
	mov r1, r8
	strh r1, [r2]
	b .L_081b24e0
	.2byte 0x0000
.L_081b24d0:
	.4byte 0x0000000c
.L_081b24d4:
	.4byte gInput
.L_081b24d8:
	adds r2, r7, #0
	adds r2, #162
	movs r3, #12
	strh r3, [r2]
.L_081b24e0:
	lsrs r3, r4, #16
	strh r3, [r5]
	ldr r3, .L_081b25b8
	ldrb r3, [r3]
	mov r9, r3
	cmp r3, #0
	beq .L_081b24f2
	bl .L_081b2db8
.L_081b24f2:
	adds r3, r7, #0
	adds r3, #140
	str r3, [sp, #16]
	str r3, [sp, #28]
	ldr r5, [r3]
	cmp r5, #0
	beq .L_081b2502
	b .L_081b2644
.L_081b2502:
	movs r0, #228
	bl PartyInventory_CountItemFar
	mov r10, r0
	adds r0, r7, #0
	adds r0, #152
	str r0, [sp, #12]
	movs r2, #128
	mov r8, r0
	lsls r2, r2, #3
	ldr r0, [r0]
	adds r2, #204
	mov r1, r10
	adds r5, r7, r2
	mov r3, r9
	subs r0, r1, r0
	ldr r2, [r5]
	movs r1, #2
	str r3, [sp, #0]
	movs r3, #64
	bl UiText_DrawNumberInWindowFar
	ldr r1, [sp, #12]
	movs r3, #8
	ldr r0, [r1]
	ldr r2, [r5]
	movs r1, #2
	str r3, [sp, #0]
	movs r3, #64
	bl UiText_DrawNumberInWindowFar
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_081b2574
	ldr r0, [sp, #16]
	ldr r2, .L_081b25bc
	movs r1, #160
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
	mov r10, r3
	bl .L_081b2e0c
.L_081b2574:
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_081b259a
	ldr r0, [sp, #12]
	ldr r3, [r0]
	cmp r3, #3
	bgt .L_081b2594
	cmp r10, r3
	ble .L_081b2594
	adds r3, #1
	str r3, [r0]
	movs r0, #111
	bl Audio_PlayCue
	b .L_081b259a
.L_081b2594:
	movs r0, #113
	bl Audio_PlayCue
.L_081b259a:
	ldrh r2, [r6]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_081b25c6
	mov r1, r8
	ldr r3, [r1]
	cmp r3, #1
	ble .L_081b25c0
	subs r3, #1
	str r3, [r1]
	movs r0, #111
	bl Audio_PlayCue
	b .L_081b25c6
.L_081b25b8:
	.4byte gDebugPaused
.L_081b25bc:
	.4byte Data_0200024c
.L_081b25c0:
	movs r0, #113
	bl Audio_PlayCue
.L_081b25c6:
	ldr r3, .L_081b25e8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_081b25ec
	adds r2, #2
	strh r3, [r2]
	movs r1, #1
	ldrh r2, [r6]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	bne .L_081b25e6
	bl .L_081b2dca
.L_081b25e6:
	b .L_081b25f0
.L_081b25e8:
	.4byte 0x00003fd0
.L_081b25ec:
	.4byte 0x00000010
.L_081b25f0:
	ldr r2, [sp, #28]
	movs r0, #238
	str r1, [r2]
	ldr r3, [sp, #36]
	lsls r0, r0, #7
	adds r0, #140
	movs r1, #153
	adds r2, r3, r0
	lsls r1, r1, #3
	movs r3, #0
	str r3, [r2]
	adds r3, r7, r1
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	mov r2, r8
	ldr r3, [r2]
	movs r5, #0
	cmp r3, #0
	beq .L_081b262a
.L_081b261a:
	movs r0, #228
	bl PartyInventory_RemoveFar
	mov r0, r8
	ldr r3, [r0]
	adds r5, #1
	cmp r5, r3
	bne .L_081b261a
.L_081b262a:
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #204
	adds r3, r7, r1
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r0, #152
	lsls r0, r0, #1
	bl Audio_PlayCue
	b .L_081b2dca
.L_081b2644:
	cmp r5, #5
	beq .L_081b264a
	b .L_081b28f4
.L_081b264a:
	movs r0, #164
	adds r0, r0, r7
	mov r8, r0
	movs r3, #0
	mov r1, r8
	mov r9, r3
	ldr r3, [r1]
	movs r5, #0
	adds r3, #1
	str r3, [r1]
	ldrb r3, [r7, #25]
	cmp r3, #0
	beq .L_081b2676
	adds r2, r7, #0
	adds r2, #25
.L_081b2668:
	adds r5, #1
	cmp r5, #5
	beq .L_081b267a
	adds r2, #28
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_081b2668
.L_081b2676:
	cmp r5, #5
	bne .L_081b267e
.L_081b267a:
	movs r2, #1
	mov r9, r2
.L_081b267e:
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081b2738
	movs r2, #0
	str r2, [r1]
	ldr r0, [sp, #36]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #140
	adds r3, r0, r1
	str r2, [r3]
	movs r3, #148
	adds r3, r3, r7
	mov r10, r3
	ldr r3, [r3]
	mov r8, r10
	cmp r3, #4
	bne .L_081b26d0
	adds r3, r7, #0
	mov r0, r10
	adds r3, #144
	str r2, [r0]
	str r2, [r3]
	ldr r1, [sp, #28]
	movs r6, #0
	str r2, [r1]
	adds r2, r7, #0
	movs r0, #0
	movs r1, #255
	adds r2, #24
.L_081b26be:
	ldrb r3, [r2, #2]
	adds r6, #1
	orrs r3, r1
	strb r0, [r2, #1]
	strb r3, [r2, #2]
	adds r2, #28
	cmp r6, #5
	bne .L_081b26be
	b .L_081b278a
.L_081b26d0:
	adds r5, r7, #0
	adds r5, #144
	ldr r3, [r5]
	cmp r3, #4
	bgt .L_081b26f8
	movs r0, #50
	adds r0, #255
	bl Audio_PlayCue
	ldr r2, [r5]
	movs r1, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #24
	adds r3, r7, r3
	ldrb r2, [r3, #1]
	eors r2, r1
	strb r2, [r3, #1]
	b .L_081b278a
.L_081b26f8:
	mov r2, r9
	cmp r2, #0
	bne .L_081b2730
	movs r0, #152
	lsls r0, r0, #1
	bl Audio_PlayCue
	ldr r0, [sp, #28]
	mov r1, r9
	movs r3, #1
	adds r2, r7, #0
	str r3, [r0]
	movs r6, #0
	str r1, [r5]
	adds r2, #24
	movs r1, #255
.L_081b2718:
	ldrb r3, [r2, #2]
	adds r6, #1
	orrs r3, r1
	strb r3, [r2, #2]
	adds r2, #28
	cmp r6, #5
	bne .L_081b2718
	mov r2, r8
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_081b278a
.L_081b2730:
	movs r0, #113
	bl Audio_PlayCue
	b .L_081b278a
.L_081b2738:
	mov r3, r10
	ldrh r2, [r3]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_081b275e
	adds r5, r7, #0
	adds r5, #144
	ldr r0, [r5]
	movs r1, #6
	adds r0, #1
	bl __modsi3
	str r0, [r5]
	movs r0, #111
	bl Audio_PlayCue
	mov r0, r10
	ldrh r2, [r0]
.L_081b275e:
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_081b2784
	adds r5, r7, #0
	adds r5, #144
	ldr r0, [r5]
	movs r1, #6
	adds r0, #5
	bl __modsi3
	str r0, [r5]
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #148
	adds r1, r1, r7
	mov r10, r1
	b .L_081b278a
.L_081b2784:
	movs r2, #148
	adds r2, r2, r7
	mov r10, r2
.L_081b278a:
	ldr r3, [sp, #28]
	ldr r3, [r3]
	mov r8, r3
	cmp r3, #5
	beq .L_081b2796
	b .L_081b28de
.L_081b2796:
	adds r3, r7, #0
	adds r3, #144
	ldr r2, [r3]
	cmp r2, #5
	bne .L_081b284e
	mov r0, r9
	cmp r0, #0
	beq .L_081b280a
	movs r1, #195
	lsls r1, r1, #3
	adds r6, r7, r1
	ldr r2, [r6]
	subs r3, r2, #1
	cmp r3, #1
	bls .L_081b27e6
	movs r2, #153
	lsls r2, r2, #3
	adds r5, r7, r2
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
	movs r3, #0
	str r1, [r5]
	ldr r0, .L_081b2960
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #1
	str r3, [r6]
	b .L_081b2dd2
.L_081b27e6:
	cmp r2, #1
	beq .L_081b27ec
	b .L_081b2dd2
.L_081b27ec:
	movs r1, #153
	lsls r1, r1, #3
	adds r3, r7, r1
	ldr r1, [r3]
	movs r2, #0
	movs r3, #8
	ldr r0, .L_081b2964
	bl UiText_DrawCharacterAtOffsetFar
	adds r2, r7, #0
	movs r3, #2
	adds r2, #152
	str r3, [r6]
	str r2, [sp, #12]
	b .L_081b2e0c
.L_081b280a:
	movs r3, #195
	lsls r3, r3, #3
	adds r6, r7, r3
	ldr r3, [r6]
	cmp r3, #3
	beq .L_081b2842
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
	ldr r0, .L_081b2968
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_081b2842:
	adds r1, r7, #0
	movs r3, #3
	adds r1, #152
	str r3, [r6]
	str r1, [sp, #12]
	b .L_081b2e0c
.L_081b284e:
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	ldrb r3, [r3, #25]
	cmp r3, #0
	bne .L_081b289a
	movs r2, #195
	lsls r2, r2, #3
	adds r6, r7, r2
	ldr r3, [r6]
	cmp r3, #4
	beq .L_081b2894
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
	ldr r0, .L_081b296c
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_081b2894:
	adds r0, r7, #0
	movs r3, #4
	b .L_081b28d6
.L_081b289a:
	movs r1, #195
	lsls r1, r1, #3
	adds r6, r7, r1
	ldr r3, [r6]
	cmp r3, #5
	beq .L_081b28d2
	movs r2, #153
	lsls r2, r2, #3
	adds r5, r7, r2
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
	ldr r0, .L_081b2970
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_081b28d2:
	adds r0, r7, #0
	mov r3, r8
.L_081b28d6:
	adds r0, #152
	str r3, [r6]
	str r0, [sp, #12]
	b .L_081b2e0c
.L_081b28de:
	movs r1, #153
	lsls r1, r1, #3
	adds r3, r7, r1
	ldr r0, [r3]
	movs r1, #1
	bl UiWork_FinalizeFar
	adds r2, r7, #0
	adds r2, #152
	str r2, [sp, #12]
	b .L_081b2e0c
.L_081b28f4:
	cmp r5, #2
	bne .L_081b2974
	movs r3, #164
	adds r3, r3, r7
	mov r8, r3
	ldr r3, [r3]
	mov r0, r8
	adds r3, #1
	movs r1, #0
	str r3, [r0]
	str r1, [sp, #32]
	cmp r3, #60
	beq .L_081b2910
	b .L_081b2dda
.L_081b2910:
	ldr r2, [sp, #16]
	movs r3, #3
	str r3, [r2]
	movs r0, #93
	bl Audio_PlayCue
	ldr r3, [sp, #32]
	mov r0, r8
	str r3, [r0]
	movs r2, #128
	ldr r3, .L_081b2958
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_081b295c
	adds r2, #2
	strh r3, [r2]
	ldr r1, [sp, #36]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r1, r2
	str r5, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	adds r0, r7, #0
	movs r1, #148
	movs r3, #75
	adds r0, #152
	adds r1, r1, r7
	str r3, [r2]
	mov r10, r1
	str r0, [sp, #12]
	b .L_081b2e0c
	.2byte 0x0000
.L_081b2958:
	.4byte 0x00003f44
.L_081b295c:
	.4byte 0x00001010
.L_081b2960:
	.4byte 0x00000d75
.L_081b2964:
	.4byte 0x00000d76
.L_081b2968:
	.4byte 0x00000d72
.L_081b296c:
	.4byte 0x00000d70
.L_081b2970:
	.4byte 0x00000d71
.L_081b2974:
	cmp r5, #3
	bne .L_081b299e
	movs r2, #164
	ldr r3, [r2, r7]
	adds r3, #1
	str r3, [r2, r7]
	movs r3, #0
	str r3, [sp, #32]
	movs r3, #1
	ldrh r2, [r6]
	ands r3, r2
	cmp r3, #0
	bne .L_081b2990
	b .L_081b2df2
.L_081b2990:
	ldr r0, [sp, #16]
	movs r3, #10
	str r3, [r0]
	movs r0, #112
	bl Audio_PlayCue
	b .L_081b29ee
.L_081b299e:
	cmp r5, #11
	bne .L_081b29fc
	movs r3, #195
	lsls r3, r3, #3
	adds r5, r7, r3
	ldr r3, [r5]
	cmp r3, #0
	bne .L_081b29c4
	movs r1, #153
	movs r3, #1
	lsls r1, r1, #3
	str r3, [r5]
	adds r3, r7, r1
	ldr r1, [r3]
	ldr r0, .L_081b2cac
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_081b29c4:
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_081b29d0
	b .L_081b2de6
.L_081b29d0:
	ldr r2, [sp, #16]
	movs r3, #5
	str r3, [r2]
	mov r3, r9
	str r3, [r5]
	movs r0, #112
	bl Audio_PlayCue
	movs r0, #153
	lsls r0, r0, #3
	adds r3, r7, r0
	movs r1, #1
	ldr r0, [r3]
	bl UiWork_FinalizeFar
.L_081b29ee:
	adds r1, r7, #0
	movs r2, #148
	adds r1, #152
	adds r2, r2, r7
	str r1, [sp, #12]
	mov r10, r2
	b .L_081b2e0c
.L_081b29fc:
	cmp r5, #20
	bne .L_081b2a32
	movs r3, #164
	adds r3, r3, r7
	mov r8, r3
	ldr r3, [r3]
	mov r0, r8
	adds r3, #1
	str r3, [r0]
	cmp r3, #45
	beq .L_081b2a14
	b .L_081b2df2
.L_081b2a14:
	ldr r1, [sp, #16]
	movs r3, #10
	str r3, [r1]
	b .L_081b2df2
.L_081b2a1c:
	bl Random16
	movs r3, #3
	ands r0, r3
	adds r0, #4
	strb r0, [r5, #2]
	movs r0, #52
	adds r0, #255
	bl Audio_PlayCue
	b .L_081b2ac2
.L_081b2a32:
	cmp r5, #10
	bne .L_081b2a38
	b .L_081b2e00
.L_081b2a38:
	movs r0, #164
	adds r0, r0, r7
	str r0, [sp, #24]
	mov r8, r0
	ldr r3, [r0]
	cmp r3, #4
	bne .L_081b2a6e
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #17
	movs r2, #12
	movs r3, #3
	movs r0, #18
	bl UiWindow_CreateFar
	movs r2, #153
	lsls r2, r2, #3
	adds r1, r0, #0
	adds r3, r7, r2
	str r1, [r3]
	ldr r0, .L_081b2cb0
	movs r3, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r0, r8
	ldr r3, [r0]
.L_081b2a6e:
	cmp r3, #16
	bne .L_081b2a7e
	movs r0, #153
	lsls r0, r0, #1
	bl Audio_PlayCue
	mov r1, r8
	ldr r3, [r1]
.L_081b2a7e:
	cmp r3, #56
	ble .L_081b2ac2
	ldr r2, [sp, #36]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #140
	adds r1, r2, r3
	ldr r3, [r1]
	cmp r3, #31
	bgt .L_081b2a9e
	ldrh r2, [r6]
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081b2ac2
.L_081b2a9e:
	mov r0, r9
	str r0, [r1]
	movs r2, #1
	adds r5, r7, #0
	movs r6, #0
	negs r2, r2
	adds r5, #24
.L_081b2aac:
	ldrb r3, [r5, #1]
	cmp r3, #0
	bne .L_081b2aba
	movs r3, #2
	ldrsb r3, [r5, r3]
	cmp r3, r2
	beq .L_081b2a1c
.L_081b2aba:
	adds r6, #1
	adds r5, #28
	cmp r6, #5
	bne .L_081b2aac
.L_081b2ac2:
	adds r2, r7, #0
	movs r6, #0
	adds r2, #24
.L_081b2ac8:
	movs r3, #2
	ldrsb r3, [r2, r3]
	ldrb r1, [r2, #2]
	cmp r3, #0
	ble .L_081b2ad6
	subs r3, r1, #1
	strb r3, [r2, #2]
.L_081b2ad6:
	adds r6, #1
	adds r2, #28
	cmp r6, #5
	bne .L_081b2ac8
	adds r2, r7, #0
	movs r1, #0
	movs r6, #0
	movs r4, #15
	adds r2, #24
	movs r0, #0
.L_081b2aea:
	ldrb r3, [r2, #1]
	cmp r3, #1
	beq .L_081b2b00
	movs r3, #2
	ldrsb r3, [r2, r3]
	cmp r3, #0
	bne .L_081b2b02
	ldr r3, [r0, r7]
	ands r3, r4
	cmp r3, #8
	bne .L_081b2b02
.L_081b2b00:
	adds r1, #1
.L_081b2b02:
	adds r6, #1
	adds r2, #28
	adds r0, #28
	cmp r6, #5
	bne .L_081b2aea
	cmp r1, #5
	beq .L_081b2b12
	b .L_081b2d4e
.L_081b2b12:
	adds r2, r7, #0
	movs r1, #0
	adds r2, #152
	movs r3, #172
	str r1, [sp, #20]
	str r2, [sp, #12]
	adds r3, r3, r7
	movs r4, #0
	mov r8, r3
.L_081b2b24:
	movs r1, #0
	mov r2, r8
	str r1, [r2]
	ldr r3, [sp, #12]
	movs r0, #1
	ldr r2, [r3]
	movs r3, #3
	negs r0, r0
	subs r3, r3, r2
	mov r10, r0
	mov r9, r1
	cmp r4, r3
	ble .L_081b2bd4
	adds r3, r2, #3
	cmp r4, r3
	bge .L_081b2bd4
	movs r6, #0
	movs r5, #0
.L_081b2b48:
	cmp r4, #0
	bne .L_081b2b5c
	ldr r0, [r5, r7]
	cmp r0, #0
	bge .L_081b2b54
	adds r0, #15
.L_081b2b54:
	asrs r0, r0, #4
	subs r0, r6, r0
	adds r0, #22
	b .L_081b2b80
.L_081b2b5c:
	cmp r4, #6
	bne .L_081b2b72
	ldr r0, [r5, r7]
	negs r3, r6
	cmp r0, #0
	bge .L_081b2b6a
	adds r0, #15
.L_081b2b6a:
	asrs r0, r0, #4
	subs r0, r3, r0
	adds r0, #26
	b .L_081b2b80
.L_081b2b72:
	ldr r0, [r5, r7]
	cmp r0, #0
	bge .L_081b2b7a
	adds r0, #15
.L_081b2b7a:
	asrs r0, r0, #4
	subs r0, r4, r0
	adds r0, #21
.L_081b2b80:
	movs r1, #21
	str r4, [sp, #4]
	bl __modsi3
	adds r0, r0, r5
	adds r0, #4
	ldrb r3, [r7, r0]
	ldr r4, [sp, #4]
	cmp r3, #5
	beq .L_081b2ba8
	movs r0, #1
	negs r0, r0
	cmp r10, r0
	bne .L_081b2ba0
	mov r10, r3
	b .L_081b2ba8
.L_081b2ba0:
	cmp r10, r3
	beq .L_081b2ba8
	movs r1, #1
	mov r9, r1
.L_081b2ba8:
	adds r6, #1
	adds r5, #28
	cmp r6, #5
	bne .L_081b2b48
	mov r2, r9
	cmp r2, #0
	bne .L_081b2bd4
	movs r3, #1
	mov r0, r8
	str r3, [r0]
	ldr r1, [sp, #20]
	movs r3, #160
	lsls r3, r3, #1
	adds r2, r1, r3
	ldr r3, .L_081b2cb4
	mov r0, r10
	ldrb r3, [r3, r0]
	ldr r1, .L_081b2cb8
	strb r3, [r1, r2]
	ldr r2, [sp, #20]
	adds r2, #1
	str r2, [sp, #20]
.L_081b2bd4:
	movs r3, #4
	adds r4, #1
	add r8, r3
	cmp r4, #7
	bne .L_081b2b24
	ldr r0, [sp, #24]
	movs r5, #0
	str r5, [r0]
	ldr r1, [sp, #20]
	cmp r1, #0
	beq .L_081b2c38
	movs r3, #160
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r0, #1
	ldr r1, .L_081b2cb8
	negs r0, r0
	adds r3, r0, #0
	strb r3, [r1, r2]
	ldr r2, [sp, #28]
	movs r3, #2
	str r3, [r2]
	movs r0, #171
	bl Audio_PlayCue
	ldr r3, [sp, #36]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #1
	str r3, [r2]
	ldr r1, [sp, #36]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r1, r2
	str r5, [r3]
	movs r3, #128
	lsls r3, r3, #19
	movs r0, #153
	adds r3, #80
	lsls r0, r0, #3
	strh r5, [r3]
	adds r3, r7, r0
	movs r1, #1
	ldr r0, [r3]
	bl UiWork_FinalizeFar
	movs r1, #148
	b .L_081b2d56
.L_081b2c38:
	ldr r2, [sp, #28]
	movs r3, #11
	str r3, [r2]
	ldr r1, [sp, #20]
	movs r0, #195
	movs r2, #153
	lsls r0, r0, #3
	lsls r2, r2, #3
	adds r3, r7, r0
	adds r6, r7, r2
	str r1, [r3]
	ldr r0, [r6]
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r3, #148
	adds r3, r3, r7
	mov r10, r3
	ldr r3, [r3]
	mov r8, r10
	cmp r3, #3
	bgt .L_081b2c96
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #24
	movs r3, #4
	movs r0, #3
	bl UiWindow_CreateFar
	ldr r5, .L_081b2cbc
	adds r1, r0, #0
	str r1, [r6]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	movs r3, #8
	ldr r1, [r6]
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r0, r10
	ldr r3, [r0]
.L_081b2c96:
	cmp r3, #4
	bne .L_081b2d5a
	movs r0, #228
	bl PartyInventory_CountItemFar
	cmp r0, #0
	ble .L_081b2cc0
	ldr r1, [sp, #28]
	movs r3, #20
	str r3, [r1]
	b .L_081b2cc6
.L_081b2cac:
	.4byte 0x00000d6f
.L_081b2cb0:
	.4byte 0x00000d6d
.L_081b2cb4:
	.4byte Data_081b489c
.L_081b2cb8:
	.4byte Data_0200024c
.L_081b2cbc:
	.4byte 0x00000d6e
.L_081b2cc0:
	ldr r2, [sp, #28]
	movs r3, #20
	str r3, [r2]
.L_081b2cc6:
	ldr r1, [sp, #12]
	ldr r3, [r1]
	cmp r3, r0
	ble .L_081b2cd0
	str r0, [r1]
.L_081b2cd0:
	movs r2, #0
	mov r3, r8
	str r2, [r3]
	adds r3, r7, #0
	adds r3, #144
	str r2, [r3]
	adds r2, r7, #0
	movs r6, #0
	movs r0, #0
	movs r1, #255
	adds r2, #24
.L_081b2ce6:
	ldrb r3, [r2, #2]
	adds r6, #1
	orrs r3, r1
	strb r0, [r2, #1]
	strb r3, [r2, #2]
	adds r2, #28
	cmp r6, #5
	bne .L_081b2ce6
	ldr r0, [sp, #36]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r0, r2
	movs r2, #0
	str r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #12
	movs r3, #4
	movs r0, #18
	bl UiWindow_CreateFar
	movs r3, #128
	ldr r5, .L_081b3050
	lsls r3, r3, #3
	adds r3, #204
	adds r1, r0, #0
	adds r6, r7, r3
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
	b .L_081b2d5a
.L_081b2d4e:
	adds r0, r7, #0
	adds r0, #152
	movs r1, #148
	str r0, [sp, #12]
.L_081b2d56:
	adds r1, r1, r7
	mov r10, r1
.L_081b2d5a:
	ldr r2, [sp, #28]
	ldr r3, [r2]
	cmp r3, #1
	bne .L_081b2d9e
	movs r6, #0
	adds r1, r7, #0
.L_081b2d66:
	ldrb r3, [r1, #25]
	cmp r3, #0
	bne .L_081b2d96
	movs r3, #26
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_081b2d78
	ldr r3, [r1]
	b .L_081b2d84
.L_081b2d78:
	ldr r2, [r1]
	movs r3, #15
	ands r3, r2
	cmp r3, #8
	beq .L_081b2d8a
	adds r3, r2, #0
.L_081b2d84:
	adds r3, #8
	str r3, [r1]
	adds r2, r3, #0
.L_081b2d8a:
	movs r3, #168
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_081b2d96
	movs r3, #0
	str r3, [r1]
.L_081b2d96:
	adds r6, #1
	adds r1, #28
	cmp r6, #5
	bne .L_081b2d66
.L_081b2d9e:
	ldr r0, [sp, #36]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #140
	adds r3, r0, r1
	ldr r2, [r3]
	adds r2, #1
	str r2, [r3]
	ldr r2, [sp, #24]
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_081b2e0c
.L_081b2db8:
	adds r3, r7, #0
	adds r0, r7, #0
	movs r1, #148
	adds r3, #140
	adds r0, #152
	adds r1, r1, r7
	str r3, [sp, #16]
	str r0, [sp, #12]
	b .L_081b2e0a
.L_081b2dca:
	movs r2, #148
	adds r2, r2, r7
	mov r10, r2
	b .L_081b2e0c
.L_081b2dd2:
	adds r3, r7, #0
	adds r3, #152
	str r3, [sp, #12]
	b .L_081b2e0c
.L_081b2dda:
	adds r0, r7, #0
	movs r1, #148
	adds r0, #152
	adds r1, r1, r7
	str r0, [sp, #12]
	b .L_081b2e0a
.L_081b2de6:
	adds r0, r7, #0
	movs r1, #148
	adds r0, #152
	adds r1, r1, r7
	str r0, [sp, #12]
	b .L_081b2e0a
.L_081b2df2:
	adds r2, r7, #0
	movs r3, #148
	adds r2, #152
	adds r3, r3, r7
	str r2, [sp, #12]
	mov r10, r3
	b .L_081b2e0c
.L_081b2e00:
	adds r0, r7, #0
	adds r0, #152
	movs r1, #148
	str r0, [sp, #12]
	adds r1, r1, r7
.L_081b2e0a:
	mov r10, r1
.L_081b2e0c:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	cmp r3, #5
	bne .L_081b2ec4
	adds r3, r7, #0
	adds r3, #144
	ldr r1, [r3]
	movs r2, #15
	lsls r3, r1, #3
	adds r3, r3, r1
	lsls r3, r3, #2
	adds r5, r3, #0
	movs r3, #164
	adds r3, r3, r7
	ldr r3, [r3]
	adds r5, #36
	ands r3, r2
	movs r6, #128
	movs r0, #0
	cmp r3, #7
	bgt .L_081b2e38
	movs r0, #1
.L_081b2e38:
	cmp r1, #5
	bne .L_081b2e40
	movs r5, #208
	movs r6, #32
.L_081b2e40:
	mov r2, r11
	ldr r1, [r2]
	adds r3, r5, #0
	ldr r2, [sp, #32]
	subs r3, #12
	lsls r3, r3, #16
	orrs r3, r2
	adds r4, r6, #0
	ldr r2, .L_081b3054
	adds r4, #8
	lsls r1, r1, #3
	orrs r3, r4
	orrs r3, r2
	adds r1, #200
	str r3, [r7, r1]
	mov r1, r11
	ldr r3, [r1]
	movs r2, #172
	lsls r2, r2, #2
	lsls r3, r3, #3
	lsls r0, r0, #4
	adds r0, r0, r2
	adds r3, #204
	str r0, [r7, r3]
	adds r3, r5, #0
	ldr r2, [r1]
	adds r3, #12
	adds r2, #1
	str r2, [r1]
	ldr r1, [sp, #32]
	lsls r3, r3, #16
	orrs r3, r1
	ldr r1, .L_081b3058
	lsls r2, r2, #3
	orrs r3, r4
	orrs r3, r1
	adds r2, #200
	str r3, [r7, r2]
	mov r2, r11
	ldr r3, [r2]
	ldr r1, .L_081b305c
	lsls r3, r3, #3
	adds r3, #204
	str r0, [r7, r3]
	mov r3, r11
	ldr r2, [r2]
	adds r2, #1
	str r2, [r3]
	ldr r0, [sp, #32]
	lsls r3, r5, #16
	orrs r3, r0
	lsls r2, r2, #3
	orrs r3, r6
	orrs r3, r1
	adds r2, #200
	str r3, [r7, r2]
	mov r1, r11
	ldr r3, [r1]
	movs r2, #248
	lsls r3, r3, #3
	adds r3, #204
	lsls r2, r2, #1
	str r2, [r7, r3]
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
.L_081b2ec4:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	cmp r3, #3
	bne .L_081b2f82
	ldr r0, .L_081b3060
	movs r3, #164
	ldr r4, [sp, #36]
	adds r3, r3, r7
	movs r6, #0
	mov r8, r3
	mov r5, r11
	mov r12, r0
.L_081b2edc:
	movs r1, #2
	ldrsh r2, [r4, r1]
	ldr r3, [sp, #32]
	lsls r2, r2, #16
	orrs r2, r3
	movs r1, #6
	ldrsh r3, [r4, r1]
	movs r1, #128
	lsls r1, r1, #1
	ldr r0, [r5]
	adds r3, r3, r1
	movs r1, #255
	ands r3, r1
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #24
	lsls r0, r0, #3
	orrs r2, r3
	adds r0, #200
	str r2, [r7, r0]
	mov r2, r12
	ldrb r3, [r2, r6]
	ldr r1, [r5]
	movs r0, #220
	lsls r3, r3, #4
	lsls r0, r0, #2
	movs r2, #240
	lsls r2, r2, #8
	lsls r1, r1, #3
	adds r3, r3, r0
	orrs r3, r2
	adds r1, #204
	str r3, [r7, r1]
	movs r1, #128
	ldr r2, [r4, #16]
	ldr r3, [r4, #4]
	lsls r1, r1, #7
	adds r3, r3, r2
	adds r2, r2, r1
	str r2, [r4, #16]
	str r3, [r4, #4]
	mov r2, r8
	ldr r3, [r2]
	adds r2, r3, #0
	cmp r3, #0
	bge .L_081b2f3a
	adds r2, #255
.L_081b2f3a:
	asrs r2, r2, #8
	lsls r2, r2, #8
	subs r2, r3, r2
	lsls r3, r6, #2
	adds r3, #200
	cmp r2, r3
	bne .L_081b2f52
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r4, #16]
	movs r3, #0
	str r3, [r4, #24]
.L_081b2f52:
	ldr r3, [r4, #4]
	movs r2, #128
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_081b2f74
	ldr r1, [r4, #24]
	str r2, [r4, #4]
	cmp r1, #1
	bgt .L_081b2f70
	ldr r3, [r4, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r4, #16]
.L_081b2f70:
	adds r3, r1, #1
	str r3, [r4, #24]
.L_081b2f74:
	ldr r3, [r5]
	adds r6, #1
	adds r3, #1
	str r3, [r5]
	adds r4, #28
	cmp r6, #8
	bne .L_081b2edc
.L_081b2f82:
	ldr r3, .L_081b3054
	ldr r6, .L_081b3064
	ldr r4, .L_081b3068
	movs r5, #0
	mov r0, r11
	mov r12, r3
.L_081b2f8e:
	ldrb r2, [r4]
	ldr r1, [sp, #32]
	lsls r2, r2, #16
	ldr r3, [r0]
	orrs r2, r1
	ldrb r1, [r6]
	lsls r3, r3, #3
	orrs r2, r1
	mov r1, r12
	adds r3, #200
	orrs r2, r1
	adds r4, #1
	adds r6, #1
	str r2, [r7, r3]
	cmp r5, #3
	bgt .L_081b2fb6
	mov r2, r11
	ldr r3, [r2]
	movs r2, #156
	b .L_081b2fbc
.L_081b2fb6:
	mov r1, r11
	ldr r3, [r1]
	movs r2, #157
.L_081b2fbc:
	lsls r3, r3, #3
	adds r3, #204
	lsls r2, r2, #3
	str r2, [r7, r3]
	ldr r3, [r0]
	adds r5, #1
	adds r3, #1
	str r3, [r0]
	cmp r5, #14
	bne .L_081b2f8e
	ldr r5, .L_081b306c
	movs r4, #128
	adds r0, r7, #0
	movs r6, #0
	mov r1, r11
	lsls r4, r4, #14
	adds r0, #25
.L_081b2fde:
	ldrb r3, [r0]
	adds r0, #28
	cmp r3, #0
	bne .L_081b2ff8
	ldr r2, [r1]
	ldr r3, [sp, #32]
	lsls r2, r2, #3
	orrs r3, r4
	adds r2, #200
	orrs r3, r5
	str r3, [r7, r2]
	movs r2, #140
	b .L_081b3008
.L_081b2ff8:
	ldr r2, [r1]
	ldr r3, [sp, #32]
	lsls r2, r2, #3
	orrs r3, r4
	adds r2, #200
	orrs r3, r5
	str r3, [r7, r2]
	movs r2, #144
.L_081b3008:
	ldr r3, [r1]
	lsls r2, r2, #3
	lsls r3, r3, #3
	adds r3, #204
	str r2, [r7, r3]
	ldr r3, [r1]
	movs r2, #144
	adds r3, #1
	lsls r2, r2, #14
	adds r6, #1
	str r3, [r1]
	adds r4, r4, r2
	cmp r6, #5
	bne .L_081b2fde
	movs r5, #128
	movs r4, #128
	movs r6, #0
	mov r0, r11
	lsls r5, r5, #3
	lsls r4, r4, #14
.L_081b3030:
	ldr r1, [r0]
	ldr r3, [sp, #32]
	ldr r2, .L_081b3070
	lsls r1, r1, #3
	orrs r3, r4
	adds r1, #200
	orrs r3, r2
	str r3, [r7, r1]
	mov r1, r10
	ldr r3, [r1]
	cmp r6, r3
	bne .L_081b3074
	mov r3, r11
	ldr r2, [r3]
	movs r1, #132
	b .L_081b307a
.L_081b3050:
	.4byte 0x00000d68
.L_081b3054:
	.4byte 0x80006000
.L_081b3058:
	.4byte 0x90006000
.L_081b305c:
	.4byte 0x80002000
.L_081b3060:
	.4byte Data_081b48a2
.L_081b3064:
	.4byte Data_081b48b8
.L_081b3068:
	.4byte Data_081b48aa
.L_081b306c:
	.4byte 0x8000207c
.L_081b3070:
	.4byte 0x80006003
.L_081b3074:
	mov r3, r11
	ldr r2, [r3]
	movs r1, #136
.L_081b307a:
	lsls r3, r6, #5
	lsls r1, r1, #2
	lsls r2, r2, #3
	adds r3, r3, r1
	adds r2, #204
	orrs r3, r5
	str r3, [r7, r2]
	ldr r3, [r0]
	movs r2, #128
	adds r3, #1
	lsls r2, r2, #13
	adds r6, #1
	str r3, [r0]
	adds r4, r4, r2
	cmp r6, #5
	bne .L_081b3030
	movs r4, #0
	mov r0, r11
	movs r5, #5
.L_081b30a0:
	movs r2, #1
	ands r2, r4
	movs r3, #129
	lsls r2, r2, #3
	lsls r3, r3, #2
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r3, r2
	ldr r2, [sp, #32]
	ldr r1, [r0]
	lsls r3, r3, #16
	orrs r3, r2
	ldr r2, .L_081b327c
	lsls r1, r1, #3
	orrs r3, r5
	orrs r3, r2
	adds r1, #200
	str r3, [r7, r1]
	ldr r3, [sp, #12]
	ldr r2, [r3]
	movs r3, #3
	subs r3, r3, r2
	cmp r4, r3
	ble .L_081b30e2
	adds r3, r2, #3
	cmp r4, r3
	bge .L_081b30e2
	mov r1, r11
	ldr r3, [r1]
	movs r2, #186
	b .L_081b30e6
.L_081b30e2:
	ldr r3, [r0]
	movs r2, #162
.L_081b30e6:
	lsls r3, r3, #3
	adds r3, #204
	lsls r2, r2, #3
	str r2, [r7, r3]
	ldr r3, [r0]
	adds r4, #1
	adds r3, #1
	str r3, [r0]
	adds r5, #16
	cmp r4, #7
	bne .L_081b30a0
	movs r2, #160
	lsls r2, r2, #14
	str r2, [sp, #8]
	movs r6, #0
	mov r10, r11
	adds r5, r7, #0
.L_081b3108:
	ldr r3, [sp, #8]
	movs r4, #0
	mov r9, r3
.L_081b310e:
	ldr r2, [r5]
	lsls r1, r4, #4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_081b311a
	adds r3, #15
.L_081b311a:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	mov r0, r10
	adds r3, r1, r3
	mov r2, r9
	ldr r1, [r0]
	adds r3, #4
	orrs r3, r2
	ldr r2, .L_081b3280
	lsls r1, r1, #3
	orrs r3, r2
	adds r1, #200
	str r3, [r7, r1]
	ldr r3, [r0]
	ldr r0, [r5]
	lsls r3, r3, #3
	adds r3, #204
	mov r8, r3
	cmp r0, #0
	bge .L_081b3146
	adds r0, #15
.L_081b3146:
	asrs r0, r0, #4
	subs r0, r4, r0
	movs r1, #21
	adds r0, #21
	str r4, [sp, #4]
	bl __modsi3
	adds r0, #4
	ldrb r3, [r5, r0]
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #4
	orrs r3, r2
	mov r0, r8
	str r3, [r7, r0]
	mov r1, r11
	ldr r3, [r1]
	ldr r4, [sp, #4]
	adds r3, #1
	adds r4, #1
	str r3, [r1]
	cmp r4, #7
	bne .L_081b310e
	ldr r2, [sp, #8]
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r2, r3
	adds r6, #1
	str r2, [sp, #8]
	adds r5, #28
	cmp r6, #5
	bne .L_081b3108
	ldr r0, [sp, #16]
	movs r1, #40
	ldr r3, [r0]
	cmp r3, #1
	bne .L_081b31c0
	adds r3, r7, #0
	adds r3, #164
	ldr r0, [r3]
	cmp r0, #47
	bgt .L_081b31aa
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #170
	muls r0, r3
	bl Trig_Sin
	lsls r0, r0, #6
	b .L_081b31ba
.L_081b31aa:
	cmp r0, #55
	bgt .L_081b31c0
	ldr r1, .L_081b3284
	lsls r0, r0, #12
	adds r0, r0, r1
	bl Trig_Sin
	lsls r0, r0, #2
.L_081b31ba:
	asrs r0, r0, #16
	adds r1, r0, #0
	adds r1, #40
.L_081b31c0:
	mov r2, r11
	ldr r3, [r2]
	ldr r0, [sp, #32]
	ldr r2, .L_081b3288
	orrs r1, r0
	lsls r3, r3, #3
	orrs r1, r2
	adds r3, #200
	str r1, [r7, r3]
	mov r1, r11
	ldr r3, [r1]
	movs r2, #160
	lsls r3, r3, #3
	adds r3, #204
	lsls r2, r2, #3
	str r2, [r7, r3]
	movs r5, #0
	ldr r3, [r1]
	mov r0, r11
	adds r3, #1
	str r3, [r1]
	movs r4, #12
.L_081b31ec:
	ldr r3, [r0]
	ldr r2, [sp, #32]
	ldr r1, .L_081b328c
	lsls r3, r3, #3
	orrs r2, r4
	adds r3, #200
	orrs r2, r1
	str r2, [r7, r3]
	cmp r5, #0
	bne .L_081b3208
	mov r2, r11
	ldr r3, [r2]
	movs r2, #168
	b .L_081b3232
.L_081b3208:
	cmp r5, #1
	bne .L_081b3214
	mov r1, r11
	ldr r3, [r1]
	movs r2, #170
	b .L_081b3232
.L_081b3214:
	cmp r5, #6
	bne .L_081b3220
	mov r2, r11
	ldr r3, [r2]
	movs r2, #174
	b .L_081b3232
.L_081b3220:
	cmp r5, #7
	bne .L_081b322c
	mov r1, r11
	ldr r3, [r1]
	movs r2, #176
	b .L_081b3232
.L_081b322c:
	mov r2, r11
	ldr r3, [r2]
	movs r2, #172
.L_081b3232:
	lsls r3, r3, #3
	adds r3, #204
	lsls r2, r2, #3
	str r2, [r7, r3]
	ldr r3, [r0]
	adds r5, #1
	adds r3, #1
	str r3, [r0]
	adds r4, #16
	cmp r5, #8
	bne .L_081b31ec
	mov r1, r11
	ldr r3, [r1]
	cmp r3, #128
	beq .L_081b326e
	ldr r0, .L_081b3290
	movs r2, #0
.L_081b3254:
	ldr r3, [r1]
	lsls r3, r3, #3
	adds r3, #200
	str r0, [r7, r3]
	ldr r3, [r1]
	lsls r3, r3, #3
	adds r3, #204
	str r2, [r7, r3]
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	cmp r3, #128
	bne .L_081b3254
.L_081b326e:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081b327c:
	.4byte 0x80002000
.L_081b3280:
	.4byte 0x80006000
.L_081b3284:
	.4byte 0xfffd0000
.L_081b3288:
	.4byte 0x80d06000
.L_081b328c:
	.4byte 0x80ce6000
.L_081b3290:
	.4byte 0x40f02000
