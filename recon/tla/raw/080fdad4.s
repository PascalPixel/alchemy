.syntax unified
	.thumb
	.global Func_080fdad4
	.thumb_func
Func_080fdad4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	str r0, [sp, #52]
	ldr r1, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	lsls r1, r1, #2
	movs r0, #0
	adds r3, r1, #0
	str r0, [sp, #44]
	str r0, [sp, #32]
	str r0, [sp, #28]
	str r1, [sp, #24]
	adds r3, #20
	ldr r2, [r7, r3]
	movs r3, #13
	strb r3, [r2, #5]
	adds r5, r7, #0
	movs r3, #14
	str r3, [sp, #0]
	adds r5, #56
	movs r3, #2
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r1, #13
	movs r2, #3
	movs r3, #17
	bl UiWindow_UpdateOrCreate
	ldr r5, [r5]
	movs r2, #0
	mov r8, r5
	str r2, [sp, #36]
	adds r3, r7, #2
	ldr r0, [sp, #52]
	str r3, [sp, #16]
	lsls r0, r0, #1
	add r4, sp, #56
	str r0, [sp, #12]
	b .L_080fe0d8
.L_080fdb34:
	ldr r1, [sp, #52]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r1, r2
	ldr r1, [sp, #16]
	str r4, [sp, #8]
	ldrb r0, [r1, r3]
	bl Owner_GetState
	movs r2, #155
	str r0, [sp, #48]
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	ldr r4, [sp, #8]
	cmp r3, #0
	beq .L_080fdbc8
	movs r3, #226
	lsls r3, r3, #1
	adds r1, r7, r3
	movs r2, #1
	b .L_080fdbd4
.L_080fdb60:
	movs r0, #130
	b .L_080fde92
.L_080fdb64:
	movs r0, #113
	str r4, [sp, #8]
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
	movs r1, #1
	str r0, [sp, #44]
	str r1, [sp, #36]
	ldr r4, [sp, #8]
	b .L_080fe0d8
.L_080fdb7a:
	movs r0, #130
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r4, [sp, #8]
	movs r2, #226
	ldr r3, [r4, #24]
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	str r3, [sp, #44]
	movs r3, #155
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	str r3, [sp, #36]
	b .L_080fe0d8
.L_080fdba0:
	movs r0, #130
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r4, [sp, #8]
	movs r0, #226
	ldr r3, [r4, #24]
	lsls r0, r0, #1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r7, r3]
	movs r1, #155
	lsls r1, r1, #2
	str r3, [sp, #44]
	adds r2, r7, r1
	movs r3, #2
	strb r3, [r2]
	movs r2, #1
	str r2, [sp, #36]
	b .L_080fe0d8
.L_080fdbc8:
	movs r3, #226
	lsls r3, r3, #1
	adds r1, r7, r3
	ldr r0, [sp, #48]
	movs r2, #2
	str r4, [sp, #8]
.L_080fdbd4:
	bl Func_080fd6f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r7, r1
	strb r0, [r3]
	ldr r4, [sp, #8]
	movs r2, #226
	lsls r2, r2, #1
	adds r0, r7, r2
	str r4, [sp, #8]
	bl Func_080fd6b0
	ldr r4, [sp, #8]
	ldr r1, [sp, #52]
	adds r0, r4, #0
	bl Func_080fd80c
	movs r3, #1
	str r3, [sp, #40]
	mov r11, r3
	ldr r3, [sp, #24]
	ldr r1, .L_080fdd80
	adds r3, #20
	ldr r3, [r7, r3]
	movs r2, #4
	mov r0, r11
	mov r10, r1
	mov r9, r2
	strb r0, [r3, #5]
	ldr r4, [sp, #8]
	b .L_080fe0c4
.L_080fdc14:
	ldr r1, [r4, #16]
	movs r0, #88
	lsls r1, r1, #4
	adds r1, #36
	str r4, [sp, #8]
	bl Func_080f8a44
	mov r3, r11
	ldr r4, [sp, #8]
	cmp r3, #0
	beq .L_080fdce4
	ldr r1, [sp, #32]
	movs r2, #226
	lsls r3, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	movs r0, #0
	mov r11, r0
	cmp r3, #0
	beq .L_080fdc4a
	lsls r3, r1, #2
	adds r3, #76
	ldr r0, [r7, r3]
	bl UiIcon_PrepareObject
	ldr r4, [sp, #8]
.L_080fdc4a:
	ldr r3, [sp, #40]
	cmp r3, #0
	beq .L_080fdc6a
	movs r0, #0
	str r0, [sp, #40]
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
	ldr r4, [sp, #8]
	mov r0, r8
	adds r2, r4, #0
	movs r1, #0
	bl Func_080fd968
	ldr r4, [sp, #8]
.L_080fdc6a:
	adds r2, r4, #0
	add r1, sp, #84
	mov r0, r8
	str r4, [sp, #8]
	bl Func_080fd8a0
	ldr r1, [sp, #12]
	ldr r4, [sp, #8]
	movs r3, #182
	lsls r3, r3, #1
	adds r2, r1, r3
	ldr r3, [r4, #24]
	movs r0, #226
	lsls r0, r0, #1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r7, r3]
	movs r1, #134
	strh r3, [r7, r2]
	lsls r1, r1, #2
	adds r3, r7, r1
	ldr r2, [r3]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r2, [r4, #24]
	lsls r3, r2, #1
	adds r3, r3, r0
	ldrh r3, [r7, r3]
	cmp r3, #0
	beq .L_080fdcb6
	lsls r3, r2, #2
	adds r3, #76
	ldr r0, [r7, r3]
	movs r3, #9
	strb r3, [r0, #5]
	movs r3, #250
	strh r6, [r0, #12]
	strb r3, [r0, #15]
.L_080fdcb6:
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r7, r3
	ldrb r3, [r2]
	movs r5, #0
	cmp r6, r3
	bcs .L_080fdce4
	adds r6, r2, #0
.L_080fdcc8:
	lsls r3, r5, #2
	adds r3, #248
	ldr r0, [r7, r3]
	movs r1, #1
	str r4, [sp, #8]
	bl Animation_ApplyChildArgumentFar
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	ldrb r3, [r6]
	ldr r4, [sp, #8]
	cmp r5, r3
	bcc .L_080fdcc8
.L_080fdce4:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
	ldr r4, [sp, #8]
	mov r1, r10
	ldr r0, [r4, #24]
	mov r2, r9
	str r0, [sp, #32]
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	bne .L_080fdd14
	add r3, sp, #64
	ldr r1, [r4, #20]
	movs r0, #0
	str r3, [sp, #0]
	movs r2, #5
	add r3, sp, #72
	bl Func_080f8f9c
	ldr r1, .L_080fdd80
	ldr r4, [sp, #8]
	b .L_080fdd18
.L_080fdd14:
	movs r0, #1
	negs r0, r0
.L_080fdd18:
	cmp r0, #1
	bne .L_080fdd22
	movs r3, #1
	str r3, [sp, #40]
	mov r11, r3
.L_080fdd22:
	cmp r0, #0
	bne .L_080fdd2a
	movs r2, #1
	mov r11, r2
.L_080fdd2a:
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080fdd36
	movs r0, #0
	mov r11, r0
.L_080fdd36:
	movs r2, #155
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080fde20
	mov r0, r10
	ldr r3, [r0, #4]
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	beq .L_080fddcc
	ldr r3, [sp, #28]
	cmp r3, #0
	bne .L_080fddcc
	ldr r3, [r4, #24]
	movs r0, #226
	lsls r0, r0, #1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r7, r3]
	ldr r0, .L_080fdd7c
	str r4, [sp, #8]
	ands r0, r3
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	ldr r4, [sp, #8]
	cmp r3, #0
	bne .L_080fdd84
	movs r0, #114
	bl Audio_PlayCue
	b .L_080fddba
	.2byte 0x0000
.L_080fdd7c:
	.4byte 0x00003fff
.L_080fdd80:
	.4byte gInput
.L_080fdd84:
	movs r0, #174
	str r4, [sp, #8]
	bl Audio_PlayCue
	movs r1, #1
	str r1, [sp, #28]
	movs r2, #135
	lsls r2, r2, #2
	adds r1, r7, r2
	ldrh r2, [r1]
	ldr r3, .L_080fddc0
	mov r0, r8
	orrs r3, r2
	strh r3, [r1]
	movs r3, #96
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #88
	movs r3, #120
	bl UiWindow_ClearInteriorTilesFar
	mov r1, r8
	ldr r0, .L_080fddc4
	movs r2, #0
	movs r3, #88
	bl UiText_DrawCharacterAtOffsetFar
.L_080fddba:
	ldr r1, .L_080fddc8
	ldr r4, [sp, #8]
	b .L_080fddcc
.L_080fddc0:
	.4byte 0x00000002
.L_080fddc4:
	.4byte 0x00001010
.L_080fddc8:
	.4byte gInput
.L_080fddcc:
	mov r0, r10
	ldr r3, [r0]
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	bne .L_080fde20
	ldr r3, [sp, #28]
	cmp r3, #1
	bne .L_080fde20
	movs r0, #0
	str r0, [sp, #28]
	movs r2, #135
	lsls r2, r2, #2
	adds r1, r7, r2
	ldrh r2, [r1]
	ldr r3, .L_080fde14
	mov r0, r8
	ands r2, r3
	movs r3, #96
	strh r2, [r1]
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #88
	movs r3, #120
	str r4, [sp, #8]
	bl UiWindow_ClearInteriorTilesFar
	mov r1, r8
	ldr r0, .L_080fde18
	movs r2, #0
	movs r3, #88
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, .L_080fde1c
	ldr r4, [sp, #8]
	b .L_080fde20
.L_080fde14:
	.4byte 0x0000fffd
.L_080fde18:
	.4byte 0x000010ba
.L_080fde1c:
	.4byte gInput
.L_080fde20:
	mov r3, r10
	ldr r2, [r3, #4]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080fdeae
	movs r0, #155
	lsls r0, r0, #2
	adds r3, r7, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080fde3a
	b .L_080fdb60
.L_080fde3a:
	ldr r3, [r4, #24]
	subs r0, #168
	lsls r3, r3, #1
	adds r2, r3, r0
	ldrh r3, [r7, r2]
	cmp r3, #0
	beq .L_080fdeae
	adds r0, r3, #0
	str r4, [sp, #8]
	bl Func_080fe164
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080fde58
	b .L_080fe088
.L_080fde58:
	ldr r3, [r4, #24]
	movs r1, #226
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r3, [r7, r3]
	ldr r0, .L_080fde88
	str r4, [sp, #8]
	ands r0, r3
	bl BattleAction_Get
	ldr r1, [sp, #48]
	ldrb r2, [r0, #9]
	movs r0, #58
	ldrsh r3, [r1, r0]
	ldr r4, [sp, #8]
	cmp r2, r3
	ble .L_080fde90
	movs r0, #114
	bl Audio_PlayCue
	ldr r1, .L_080fde8c
	ldr r4, [sp, #8]
	b .L_080fdeae
.L_080fde88:
	.4byte 0x00003fff
.L_080fde8c:
	.4byte gInput
.L_080fde90:
	movs r0, #173
.L_080fde92:
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r4, [sp, #8]
	movs r2, #226
	ldr r3, [r4, #24]
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	str r3, [sp, #44]
	movs r3, #1
	str r3, [sp, #36]
	b .L_080fe0d8
.L_080fdeae:
	mov r0, r10
	ldr r2, [r0, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080fdebc
	b .L_080fdb64
.L_080fdebc:
	ldr r2, [r1, #12]
	adds r3, #254
	ands r2, r3
	cmp r2, #0
	bne .L_080fded4
	ldr r2, [r1, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	bne .L_080fded4
	b .L_080fdfdc
.L_080fded4:
	ldr r3, [r1]
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	beq .L_080fdee0
	b .L_080fdfdc
.L_080fdee0:
	ldr r3, [sp, #52]
	movs r0, #139
	adds r3, #28
	str r3, [sp, #20]
	lsls r0, r0, #1
	adds r0, #255
	ldrsb r6, [r7, r3]
	adds r3, r7, r0
	ldrb r3, [r3]
	movs r1, #155
	lsls r1, r1, #2
	mov r11, r3
	adds r3, r7, r1
	ldrb r2, [r3]
	movs r0, #111
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	mov r9, r3
	mov r2, r9
	movs r3, #2
	subs r2, r3, r2
	mov r9, r2
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r0, [sp, #52]
	ldr r2, [sp, #16]
	movs r1, #133
	lsls r1, r1, #2
	ldr r4, [sp, #8]
	adds r3, r0, r1
	ldrb r3, [r2, r3]
	movs r0, #153
	ldr r2, [r4, #24]
	lsls r0, r0, #2
	adds r3, r3, r0
	strb r2, [r7, r3]
.L_080fdf2c:
	mov r1, r10
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080fdf3e
	adds r6, #1
	b .L_080fdf40
.L_080fdf3e:
	subs r6, #1
.L_080fdf40:
	mov r2, r11
	adds r0, r6, r2
	mov r1, r11
	str r4, [sp, #8]
	bl __modsi3
	adds r6, r0, #0
	movs r0, #129
	lsls r3, r6, #1
	lsls r0, r0, #2
	adds r5, r3, r0
	ldrh r3, [r7, r5]
	movs r1, #128
	str r3, [r7, #8]
	ldrh r2, [r7, r5]
	lsls r1, r1, #2
	adds r1, #22
	adds r3, r7, r1
	strb r2, [r3]
	ldrb r0, [r3]
	bl Owner_GetState
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	mov r2, r9
	bl Func_080fd6f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r7, r1
	strb r0, [r3]
	lsls r0, r0, #24
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080fdf2c
	ldr r2, [sp, #20]
	adds r0, r6, #0
	strb r6, [r7, r2]
	cmp r6, #0
	bge .L_080fdf94
	adds r0, r6, #3
.L_080fdf94:
	asrs r0, r0, #2
	lsls r0, r0, #2
	str r4, [sp, #8]
	bl Func_08104ef8
	ldr r0, [r7, #16]
	adds r1, r6, #0
	mov r2, r11
	bl Func_08104d5c
	movs r0, #188
	lsls r0, r0, #1
	adds r3, r7, r0
	ldr r3, [r3]
	movs r1, #190
	movs r2, #13
	lsls r1, r1, #1
	strb r2, [r3, #5]
	adds r3, r7, r1
	ldr r3, [r3]
	movs r0, #1
	strb r2, [r3, #5]
	bl WaitFrames
	ldr r0, [r7, #40]
	ldrh r1, [r7, r5]
	movs r2, #0
	movs r3, #0
	bl Func_080f8170
	ldrh r1, [r7, r5]
	adds r0, r7, #0
	bl Func_080f88c4
	ldr r4, [sp, #8]
	b .L_080fe0d8
.L_080fdfdc:
	mov r3, r10
	ldr r2, [r3, #4]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080fe052
	ldr r3, [r1]
	mov r0, r9
	ands r3, r0
	cmp r3, #0
	beq .L_080fe052
	ldr r3, [r4, #24]
	movs r1, #226
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r3, [r7, r3]
	ldr r0, .L_080fe020
	str r4, [sp, #8]
	ands r0, r3
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	ldr r4, [sp, #8]
	cmp r3, #0
	bne .L_080fe028
	movs r0, #114
	bl Audio_PlayCue
	ldr r1, .L_080fe024
	ldr r4, [sp, #8]
	b .L_080fe052
	.2byte 0x0000
.L_080fe020:
	.4byte 0x00003fff
.L_080fe024:
	.4byte gInput
.L_080fe028:
	ldr r2, [sp, #52]
	ldr r1, [sp, #16]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r2, r0
	ldrb r0, [r1, r3]
	ldr r3, [r4, #24]
	movs r2, #226
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r1, [r7, r3]
	movs r2, #0
	str r4, [sp, #8]
	bl Func_080fd52c
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080fe050
	b .L_080fdb7a
.L_080fe050:
	ldr r1, .L_080fe098
.L_080fe052:
	mov r3, r10
	ldr r2, [r3, #4]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080fe0c4
	ldr r3, [r1]
	mov r0, r9
	ands r3, r0
	cmp r3, #0
	beq .L_080fe0c4
	ldr r3, [r4, #24]
	movs r1, #226
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r3, [r7, r3]
	ldr r0, .L_080fe094
	str r4, [sp, #8]
	ands r0, r3
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	ldr r4, [sp, #8]
	cmp r3, #0
	bne .L_080fe09c
.L_080fe088:
	movs r0, #114
	bl Audio_PlayCue
	ldr r4, [sp, #8]
	b .L_080fe0c4
	.2byte 0x0000
.L_080fe094:
	.4byte 0x00003fff
.L_080fe098:
	.4byte gInput
.L_080fe09c:
	ldr r2, [sp, #52]
	ldr r1, [sp, #16]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r2, r0
	ldrb r0, [r1, r3]
	ldr r3, [r4, #24]
	movs r2, #226
	lsls r2, r2, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r1, [r7, r3]
	movs r2, #1
	str r4, [sp, #8]
	bl Func_080fd52c
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_080fe0c4
	b .L_080fdba0
.L_080fe0c4:
	movs r0, #168
	lsls r0, r0, #1
	str r4, [sp, #8]
	bl GameFlag_Test
	adds r6, r0, #0
	ldr r4, [sp, #8]
	cmp r6, #0
	bne .L_080fe0d8
	b .L_080fdc14
.L_080fe0d8:
	ldr r3, [sp, #36]
	cmp r3, #0
	bne .L_080fe0f0
	movs r0, #168
	lsls r0, r0, #1
	str r4, [sp, #8]
	bl GameFlag_Test
	ldr r4, [sp, #8]
	cmp r0, #0
	bne .L_080fe0f0
	b .L_080fdb34
.L_080fe0f0:
	movs r0, #135
	lsls r0, r0, #2
	adds r1, r7, r0
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #253
	ands r3, r2
	strh r3, [r1]
	ldr r0, [r7, #72]
	str r4, [sp, #8]
	bl UiIcon_PrepareObject
	ldr r1, [sp, #12]
	ldr r4, [sp, #8]
	movs r3, #180
	lsls r3, r3, #1
	adds r2, r1, r3
	ldr r3, [r4, #24]
	movs r1, #133
	strh r3, [r7, r2]
	ldr r0, [sp, #52]
	ldr r2, [sp, #16]
	lsls r1, r1, #2
	adds r3, r0, r1
	ldrb r3, [r2, r3]
	movs r0, #153
	ldr r2, [r4, #24]
	lsls r0, r0, #2
	adds r3, r3, r0
	strb r2, [r7, r3]
	ldr r1, [sp, #12]
	add r0, sp, #44
	ldrh r0, [r0]
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r0, [r7, r3]
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fe14e
	movs r1, #1
	negs r1, r1
	str r1, [sp, #44]
.L_080fe14e:
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #44]
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
