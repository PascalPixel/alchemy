.syntax unified
	.thumb
	.global MusicPlayer_Tick
	.thumb_func
MusicPlayer_Tick:
	ldr r2, .L_081c0c14
	ldr r3, [r0, #52]
	cmp r2, r3
	beq .L_081c09da
	bx lr
.L_081c09da:
	adds r3, #1
	str r3, [r0, #52]
	push {r0, lr}
	ldr r3, [r0, #56]
	cmp r3, #0
	beq .L_081c09ec
	ldr r0, [r0, #60]
	mov lr, r3
	.2byte 0xf800
.L_081c09ec:
	pop {r0}
	push {r4, r5, r6, r7}
	mov r4, r8
	mov r5, r9
	mov r6, r10
	mov r7, r11
	push {r4, r5, r6, r7}
	adds r7, r0, #0
	ldr r0, [r7, #4]
	cmp r0, #0
	bge .L_081c0a04
	b .L_081c0bfe
.L_081c0a04:
	ldr r0, .L_081c0c10
	ldr r0, [r0]
	mov r8, r0
	adds r0, r7, #0
	bl MusicPlayer_StepFade
	ldr r0, [r7, #4]
	cmp r0, #0
	bge .L_081c0a18
	b .L_081c0bfe
.L_081c0a18:
	ldrh r0, [r7, #34]
	ldrh r1, [r7, #32]
	adds r0, r0, r1
	b .L_081c0b56
.L_081c0a20:
	ldrb r6, [r7, #8]
	ldr r5, [r7, #44]
	movs r3, #1
	movs r4, #0
.L_081c0a28:
	ldrb r0, [r5]
	movs r1, #128
	tst r1, r0
	bne .L_081c0a32
	b .L_081c0b34
.L_081c0a32:
	mov r10, r3
	orrs r4, r3
	mov r11, r4
	ldr r4, [r5, #32]
	cmp r4, #0
	beq .L_081c0a64
.L_081c0a3e:
	ldrb r1, [r4]
	movs r0, #199
	tst r0, r1
	beq .L_081c0a58
	ldrb r0, [r4, #16]
	subs r0, #1
	bcc .L_081c0a5e
	strb r0, [r4, #16]
	bne .L_081c0a5e
	movs r0, #64
	orrs r1, r0
	strb r1, [r4]
	b .L_081c0a5e
.L_081c0a58:
	adds r0, r4, #0
	bl SoundChannel_Unlink
.L_081c0a5e:
	ldr r4, [r4, #52]
	cmp r4, #0
	bne .L_081c0a3e
.L_081c0a64:
	ldrb r3, [r5]
	lsrs r0, r3, #7
	.global Func_081c0a68
	.thumb_func
Func_081c0a68:
	bcc .L_081c0ad8
	adds r0, r5, #0
	bl SoundDriver_ClearBuffer
	movs r0, #128
	strb r0, [r5]
	movs r0, #2
	strb r0, [r5, #15]
	movs r0, #64
	strb r0, [r5, #19]
	movs r0, #22
	strb r0, [r5, #25]
	movs r0, #1
	adds r1, r5, #6
	strb r0, [r1, #30]
	b .L_081c0ad8
.L_081c0a88:
	ldrb r1, [r5, #7]
	b .L_081c0a9e
.L_081c0a8c:
	ldr r2, [r5, #64]
	ldrb r1, [r2]
	cmp r1, #128
	bcc .L_081c0a88
	adds r2, #1
	str r2, [r5, #64]
	cmp r1, #189
	bcc .L_081c0abc
	strb r1, [r5, #7]
.L_081c0a9e:
	cmp r1, #207
	bcc .L_081c0abc
	adds r0, r1, #0
	subs r0, #207
	adds r1, r7, #0
	adds r2, r5, #0
	bl MusicTrack_HandleNote
	b .L_081c0ad8
.L_081c0ab0:
	ldr r0, .L_081c0ab8
	ldrb r0, [r1, r0]
	strb r0, [r5, #1]
	b .L_081c0ada
.L_081c0ab8:
	.4byte Data_081c36e1
.L_081c0abc:
	subs r1, #177
	bcc .L_081c0ab0
	strb r1, [r7, #10]
	mov r3, r8
	ldr r3, [r3, #52]
	lsls r1, r1, #2
	ldr r3, [r3, r1]
	adds r0, r7, #0
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
	ldrb r0, [r5]
	cmp r0, #0
	beq .L_081c0b30
.L_081c0ad8:
	ldrb r0, [r5, #1]
.L_081c0ada:
	subs r0, #1
	bcc .L_081c0a8c
	strb r0, [r5, #1]
	ldrb r0, [r5, #23]
	cmp r0, #0
	beq .L_081c0b30
	ldrb r0, [r5, #28]
	subs r0, #1
	bcc .L_081c0af0
	strb r0, [r5, #28]
	b .L_081c0b30
.L_081c0af0:
	ldrb r1, [r5, #25]
	cmp r1, #0
	beq .L_081c0b30
	ldrb r0, [r5, #26]
	adds r0, r0, r1
	strb r0, [r5, #26]
	adds r1, r0, #0
	subs r0, #64
	lsls r0, r0, #24
	bpl .L_081c0b0a
	lsls r2, r1, #24
	asrs r2, r2, #24
	b .L_081c0b0e
.L_081c0b0a:
	movs r0, #128
	subs r2, r0, r1
.L_081c0b0e:
	ldrb r0, [r5, #23]
	muls r0, r2
	asrs r2, r0, #6
	ldrb r0, [r5, #22]
	eors r0, r2
	lsls r0, r0, #24
	beq .L_081c0b30
	strb r2, [r5, #22]
	ldrb r0, [r5]
	ldrb r1, [r5, #24]
	cmp r1, #0
	bne .L_081c0b2a
	movs r1, #12
	b .L_081c0b2c
.L_081c0b2a:
	movs r1, #3
.L_081c0b2c:
	orrs r0, r1
	strb r0, [r5]
.L_081c0b30:
	mov r3, r10
	mov r4, r11
.L_081c0b34:
	subs r6, #1
	ble .L_081c0b3e
	adds r5, #80
	lsls r3, r3, #1
	b .L_081c0a28
.L_081c0b3e:
	ldr r0, [r7, #12]
	adds r0, #1
	str r0, [r7, #12]
	cmp r4, #0
	bne .L_081c0b50
	movs r0, #128
	lsls r0, r0, #24
	str r0, [r7, #4]
	b .L_081c0bfe
.L_081c0b50:
	str r4, [r7, #4]
	ldrh r0, [r7, #34]
	subs r0, #150
.L_081c0b56:
	strh r0, [r7, #34]
	cmp r0, #150
	bcc .L_081c0b5e
	b .L_081c0a20
.L_081c0b5e:
	ldrb r2, [r7, #8]
	ldr r5, [r7, #44]
.L_081c0b62:
	ldrb r0, [r5]
	lsls r1, r0, #24
	bpl .L_081c0bf8
	lsls r1, r0, #28
	beq .L_081c0bf8
	mov r9, r2
	adds r0, r7, #0
	adds r1, r5, #0
	bl MusicTrack_CalcOutput
	ldr r4, [r5, #32]
	cmp r4, #0
	beq .L_081c0bee
.L_081c0b7c:
	ldrb r1, [r4]
	movs r0, #199
	tst r0, r1
	bne .L_081c0b8c
	adds r0, r4, #0
	bl SoundChannel_Unlink
	b .L_081c0be8
.L_081c0b8c:
	ldrb r0, [r4, #1]
	movs r6, #7
	ands r6, r0
	ldrb r3, [r5]
	movs r0, #3
	tst r0, r3
	beq .L_081c0baa
	bl SoundDriver_CalculateStereoVolume
	cmp r6, #0
	beq .L_081c0baa
	ldrb r0, [r4, #29]
	movs r1, #1
	orrs r0, r1
	strb r0, [r4, #29]
.L_081c0baa:
	ldrb r3, [r5]
	movs r0, #12
	tst r0, r3
	beq .L_081c0be8
	ldrb r1, [r4, #8]
	movs r0, #8
	ldrsb r0, [r5, r0]
	adds r2, r1, r0
	bpl .L_081c0bbe
	movs r2, #0
.L_081c0bbe:
	cmp r6, #0
	beq .L_081c0bdc
	mov r0, r8
	ldr r3, [r0, #48]
	adds r1, r2, #0
	ldrb r2, [r5, #9]
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	str r0, [r4, #32]
	ldrb r0, [r4, #29]
	movs r1, #2
	orrs r0, r1
	strb r0, [r4, #29]
	b .L_081c0be8
.L_081c0bdc:
	adds r1, r2, #0
	ldrb r2, [r5, #9]
	ldr r0, [r4, #36]
	bl SoundDriver_MidiKeyToFrequency
	str r0, [r4, #32]
.L_081c0be8:
	ldr r4, [r4, #52]
	cmp r4, #0
	bne .L_081c0b7c
.L_081c0bee:
	ldrb r0, [r5]
	movs r1, #240
	ands r0, r1
	strb r0, [r5]
	mov r2, r9
.L_081c0bf8:
	adds r5, #80
	subs r2, #1
	bgt .L_081c0b62
.L_081c0bfe:
	ldr r0, .L_081c0c14
	str r0, [r7, #52]
	pop {r0, r1, r2, r3, r4, r5, r6, r7}
	mov r8, r0
	mov r9, r1
	mov r10, r2
	mov r11, r3
	.global Func_081c0c0c
	.thumb_func
Func_081c0c0c:
	pop {pc}
	.2byte 0x4718
.L_081c0c10:
	.4byte Data_03007ff0
.L_081c0c14:
	.4byte 0x68736d53
