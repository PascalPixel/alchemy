.syntax unified
	.thumb
	.global Func_08163e14
	.thumb_func
Func_08163e14:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	str r0, [sp, #44]
	movs r0, #0
	ldr r1, [r3, #92]
	str r1, [sp, #40]
	ldr r3, [r3, #100]
	str r3, [sp, #36]
	bl Func_081435e0
	ldr r3, .L_08163e78
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #52
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #32]
	bl Func_08144aac
	ldr r0, .L_08163e7c
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [sp, #40]
	movs r6, #224
	b .L_08163e80
	.2byte 0x0000
.L_08163e78:
	.4byte 0x00001010
.L_08163e7c:
	.4byte 0x0000012f
.L_08163e80:
	adds r4, #128
	lsls r6, r6, #3
	adds r1, r3, r6
	adds r0, r4, #0
	bl Func_0801587c
	ldr r0, .L_08164210
	bl Resource_GetTableEntry
	ldr r7, [sp, #40]
	adds r4, r0, #0
	movs r0, #208
	lsls r0, r0, #4
	adds r4, #128
	adds r0, #228
	adds r1, r7, r0
	adds r0, r4, #0
	bl Func_0801587c
	ldr r0, .L_08164214
	bl Resource_GetTableEntry
	ldr r1, [sp, #36]
	bl Func_0801587c
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r7, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r7, r3
	movs r1, #200
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08164218
	bl Func_080145a8
	ldr r1, .L_0816421c
	ldr r2, .L_08164220
	movs r7, #128
	movs r0, #176
	movs r6, #0
	lsls r7, r7, #17
	lsls r0, r0, #15
	ldr r3, [sp, #40]
	str r6, [sp, #12]
	str r7, [sp, #24]
	str r0, [sp, #28]
	str r1, [sp, #16]
	str r2, [sp, #20]
	movs r2, #1
	mov r8, r6
	negs r2, r2
	adds r3, #24
.L_08163ef4:
	movs r6, #1
	add r8, r6
	mov r7, r8
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_08163ef4
	ldr r1, [sp, #40]
	movs r2, #168
	movs r0, #0
	lsls r2, r2, #2
	mov r8, r0
	adds r5, r1, r2
.L_08163f0e:
	bl Random16
	movs r3, #127
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #56
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	negs r3, r3
	str r3, [r5, #24]
	movs r3, #1
	add r8, r3
	mov r6, r8
	adds r5, #28
	cmp r6, #16
	bne .L_08163f0e
	ldr r3, .L_08164224
	movs r7, #0
	movs r1, #1
	movs r2, #128
	mov r8, r7
	negs r1, r1
	lsls r2, r2, #3
.L_08163f4a:
	movs r0, #1
	add r8, r0
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08163f4a
	ldr r1, [sp, #40]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #240
	adds r3, r1, r2
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	movs r1, #197
	lsls r1, r1, #1
	adds r1, #255
	movs r0, #12
	movs r2, #2
	bl Func_08152404
	movs r3, #0
	mov r9, r3
.L_08163f7e:
	ldr r3, .L_08164228
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08163f98
	mov r6, r9
	cmp r6, #32
	ble .L_08163f98
	cmp r6, #97
	bgt .L_08163f98
	movs r7, #98
	mov r9, r7
.L_08163f98:
	mov r0, r9
	cmp r0, #120
	bne .L_08163fa4
	movs r0, #134
	bl Func_08118088 + 0x60
.L_08163fa4:
	mov r1, r9
	cmp r1, #15
	bgt .L_08163fb0
	ldr r2, [sp, #12]
	adds r2, #2
	str r2, [sp, #12]
.L_08163fb0:
	mov r3, r9
	cmp r3, #99
	bgt .L_08163ffa
	ldr r2, [sp, #16]
	ldr r7, [sp, #24]
	ldr r1, [sp, #28]
	ldr r6, [sp, #16]
	ldr r0, [sp, #20]
	movs r3, #58
	muls r3, r2
	adds r6, r6, r7
	adds r0, r0, r1
	str r6, [sp, #24]
	str r0, [sp, #28]
	cmp r3, #0
	bge .L_08163fd2
	adds r3, #63
.L_08163fd2:
	ldr r6, [sp, #20]
	asrs r3, r3, #6
	str r3, [sp, #16]
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_08163fe4
	adds r3, #63
.L_08163fe4:
	ldr r7, [sp, #24]
	ldr r0, .L_0816422c
	asrs r3, r3, #6
	str r3, [sp, #20]
	cmp r7, r0
	bgt .L_08163ffa
	ldr r1, [sp, #16]
	movs r2, #128
	lsls r2, r2, #8
	adds r2, r1, r2
	str r2, [sp, #16]
.L_08163ffa:
	movs r0, #1
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	bl Func_0816442c
	mov r3, r9
	cmp r3, #28
	bne .L_0816408e
	movs r7, #63
	mov r10, r7
	ldr r7, .L_08164230
	movs r6, #0
	mov r8, r6
.L_08164014:
	ldr r3, [r7, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_08164080
	bl Random16
	mov r1, r10
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	movs r2, #128
	lsls r2, r2, #14
	asrs r3, r3, #3
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r6, #192
	asrs r3, r3, #2
	lsls r6, r6, #15
	adds r3, r3, r6
	str r3, [r7, #4]
	bl Random16
	mov r1, r10
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r2, r10
	ands r0, r2
	negs r0, r0
	subs r0, #8
	lsls r0, r0, #13
	movs r3, #0
	str r0, [r7, #16]
	str r3, [r7, #24]
.L_08164080:
	movs r3, #1
	movs r6, #128
	add r8, r3
	lsls r6, r6, #1
	adds r7, #28
	cmp r8, r6
	bne .L_08164014
.L_0816408e:
	mov r7, r9
	subs r7, #32
	str r7, [sp, #8]
	cmp r7, #47
	bhi .L_08164128
	ldr r7, .L_08164230
	movs r0, #0
	movs r1, #63
	mov r11, r0
	mov r8, r0
	mov r10, r1
.L_081640a4:
	ldr r3, [r7, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_0816411a
	bl Random16
	mov r3, r10
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	movs r0, #128
	lsls r0, r0, #14
	asrs r3, r3, #3
	adds r3, r3, r0
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r1, #192
	lsls r1, r1, #15
	asrs r3, r3, #2
	adds r3, r3, r1
	str r3, [r7, #4]
	bl Random16
	mov r2, r10
	ands r0, r2
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r3, r10
	ands r0, r3
	negs r0, r0
	subs r0, #8
	movs r6, #1
	lsls r0, r0, #13
	add r11, r6
	str r0, [r7, #16]
	movs r3, #0
	mov r0, r11
	str r3, [r7, #24]
	cmp r0, #16
	beq .L_08164128
.L_0816411a:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r7, #28
	cmp r8, r2
	bne .L_081640a4
.L_08164128:
	mov r3, r9
	cmp r3, #0
	bne .L_08164134
	movs r0, #164
	bl Audio_PlayCue
.L_08164134:
	mov r6, r9
	cmp r6, #32
	bne .L_08164140
	movs r0, #145
	bl Audio_PlayCue
.L_08164140:
	mov r7, r9
	cmp r7, #80
	bne .L_0816414c
	movs r0, #144
	bl Audio_PlayCue
.L_0816414c:
	ldr r0, [sp, #8]
	cmp r0, #47
	bhi .L_081641bc
	ldr r6, [sp, #40]
	movs r7, #208
	lsls r7, r7, #4
	movs r1, #0
	adds r7, #228
	adds r6, r6, r7
	mov r8, r1
	ldr r1, .L_08164234
	mov r2, r9
	mov r10, r6
	ldr r6, .L_08164238
	lsls r3, r2, #4
	movs r0, #34
	mov r11, r0
	adds r7, r3, r1
.L_08164170:
	adds r0, r7, #0
	movs r1, #104
	bl Math_Mod
	ldrb r3, [r6, #1]
	ldrb r2, [r6]
	adds r5, r0, #0
	movs r1, #104
	mov r0, r11
	subs r3, r3, r5
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #52]
	subs r2, #17
	subs r3, #104
	ldr r0, [sp, #44]
	mov r1, r10
	mov lr, r4
	.2byte 0xf800
	ldrb r2, [r6]
	ldrb r3, [r6, #1]
	mov r0, r11
	subs r2, #17
	str r0, [sp, #0]
	mov r1, r10
	subs r3, r3, r5
	str r5, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #2
	adds r7, #25
	cmp r2, #3
	bne .L_08164170
.L_081641bc:
	mov r3, r9
	cmp r3, #95
	bgt .L_08164206
	ldr r7, [sp, #40]
	movs r0, #224
	lsls r0, r0, #3
	adds r7, r7, r0
	movs r6, #0
	mov r10, r7
	mov r8, r6
	movs r5, #32
	movs r7, #120
.L_081641d4:
	mov r2, r8
	lsls r1, r2, #5
	mov r2, r9
	cmp r2, #0
	bge .L_081641e0
	adds r2, #3
.L_081641e0:
	movs r3, #31
	ldr r6, [sp, #12]
	asrs r2, r2, #2
	ands r2, r3
	adds r2, r1, r2
	subs r2, #32
	mov r1, r10
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #44]
	subs r3, r7, r6
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #5
	bne .L_081641d4
.L_08164206:
	ldr r5, .L_08164230
	movs r2, #0
	mov r8, r2
	b .L_0816423c
	.2byte 0x0000
.L_08164210:
	.4byte 0x00000146
.L_08164214:
	.4byte 0x00000134
.L_08164218:
	.4byte Func_08143000
.L_0816421c:
	.4byte 0xfff00000
.L_08164220:
	.4byte 0xfffc0000
.L_08164224:
	.4byte Data_02010018
.L_08164228:
	.4byte gInput
.L_0816422c:
	.4byte 0x0077ffff
.L_08164230:
	.4byte gMapCellBuffer
.L_08164234:
	.4byte 0xffffff00
.L_08164238:
	.4byte Data_0819896a
.L_0816423c:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_08164328
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	ldr r3, [r5, #16]
	adds r4, r0, #2
	cmp r3, #0
	ble .L_08164254
	adds r4, #2
.L_08164254:
	mov r6, r9
	cmp r6, #68
	ble .L_08164260
	cmp r4, #5
	bgt .L_08164260
	movs r4, #6
.L_08164260:
	mov r7, r9
	cmp r7, #70
	ble .L_0816426c
	cmp r4, #6
	bgt .L_0816426c
	movs r4, #7
.L_0816426c:
	mov r0, r9
	cmp r0, #72
	ble .L_08164278
	cmp r4, #7
	bgt .L_08164278
	movs r4, #8
.L_08164278:
	mov r1, r9
	cmp r1, #74
	ble .L_08164284
	cmp r4, #8
	bgt .L_08164284
	movs r4, #9
.L_08164284:
	mov r2, r9
	cmp r2, #76
	ble .L_0816428c
	movs r4, #10
.L_0816428c:
	movs r6, #4
	cmp r3, #0
	bgt .L_08164294
	movs r6, #0
.L_08164294:
	ldr r2, .L_0816441c
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #36]
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r3, r1
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #32]
	subs r3, r3, r4
	ldr r4, [r6, r0]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	ldr r1, [r5, #16]
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r5, #4]
	mov r2, r9
	adds r3, r3, r1
	str r3, [r5, #4]
	cmp r2, #80
	ble .L_081642dc
	ldr r6, .L_08164420
	adds r3, r1, r6
	b .L_081642ea
.L_081642dc:
	ldr r3, .L_08164424
	movs r2, #3
	mov r7, r8
	ands r2, r7
	lsls r2, r2, #2
	ldr r3, [r3, r2]
	adds r3, r1, r3
.L_081642ea:
	str r3, [r5, #16]
	ldr r2, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_081642fa
	adds r3, #63
.L_081642fa:
	ldr r2, [r5, #16]
	asrs r3, r3, #6
	str r3, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r2, r3, #1
	cmp r2, #0
	bge .L_0816430c
	adds r2, #63
.L_0816430c:
	ldr r3, [r5, #24]
	asrs r2, r2, #6
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r2, #0
	ble .L_08164328
	movs r0, #6
	ldrsh r3, [r5, r0]
	cmp r3, #104
	ble .L_08164328
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_08164328:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r5, #28
	cmp r8, r2
	bne .L_0816423c
	mov r3, r9
	cmp r3, #79
	bgt .L_081643b0
	ldr r7, [sp, #48]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r8, r6
	cmp r3, #0
	beq .L_081643b0
	adds r7, #36
.L_0816434a:
	mov r0, r9
	cmp r0, #29
	ble .L_081643a6
	movs r1, #12
	bl Math_Mod
	adds r6, r0, #0
	cmp r6, #0
	bne .L_08164386
	movs r1, #0
	ldrsh r0, [r7, r1]
	bl Func_08118088 + 0x10
	movs r3, #1
	ldr r5, [r0]
	negs r3, r3
	movs r2, #0
	ldrsh r0, [r7, r2]
	movs r1, #7
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r3, #144
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r5, #72]
.L_08164386:
	cmp r6, #6
	bne .L_081643a2
	movs r3, #0
	ldrsh r0, [r7, r3]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	subs r3, #1
	movs r2, #5
	bl Func_0814cd48
	ldr r6, [sp, #48]
	ldr r3, [r6, #20]
	b .L_081643a6
.L_081643a2:
	ldr r0, [sp, #48]
	ldr r3, [r0, #20]
.L_081643a6:
	movs r1, #1
	add r8, r1
	adds r7, #2
	cmp r8, r3
	bne .L_0816434a
.L_081643b0:
	ldr r3, [sp, #40]
	movs r6, #240
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r3, r6
	movs r7, #1
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	add r9, r7
	bl WaitFrames
	mov r0, r9
	cmp r0, #124
	beq .L_081643d0
	b .L_08163f7e
.L_081643d0:
	ldr r0, .L_08164428
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r0, #1
	bl Func_0816467c
	movs r3, #238
	ldr r2, [sp, #40]
	lsls r3, r3, #7
	movs r1, #0
	adds r3, #220
	mov r8, r1
	adds r5, r2, r3
.L_081643fa:
	movs r6, #1
	add r8, r6
	ldmia r5!, {r0}
	mov r7, r8
	bl Func_08020040 + 0x8
	cmp r7, #12
	bne .L_081643fa
	bl Func_08143bb8
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816441c:
	.4byte Data_08197410
.L_08164420:
	.4byte 0xffff8000
.L_08164424:
	.4byte Data_08198974
.L_08164428:
	.4byte Func_08143000
