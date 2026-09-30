.syntax unified
	.thumb
	.global Func_08170364
	.thumb_func
Func_08170364:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #124
	str r0, [sp, #88]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	movs r6, #252
	str r0, [sp, #84]
	lsls r6, r6, #6
	ldr r1, [r3, #92]
	ldr r5, .L_081703d8
	str r1, [sp, #80]
	ldr r3, [r3, #100]
	str r3, [sp, #72]
	bl Func_0813ba50
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081703d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #100
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #68]
	bl Func_08144aac
	ldr r3, [sp, #80]
	ldr r0, .L_081703dc
	adds r1, r3, r6
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r7, [sp, #80]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r7, r2
	ldr r0, .L_081703e0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_081703e4
	ldr r1, [sp, #72]
	b .L_081703e8
.L_081703d4:
	.4byte 0x00001010
.L_081703d8:
	.4byte Data_02014000
.L_081703dc:
	.4byte 0x0000013e
.L_081703e0:
	.4byte 0x000000b7
.L_081703e4:
	.4byte 0x00000134
.L_081703e8:
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	adds r1, r5, #0
	ldr r0, .L_0817064c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r6, #184
	movs r3, #192
	lsls r3, r3, #4
	lsls r6, r6, #6
	adds r3, #86
	adds r6, #22
	adds r1, r5, r3
	ldr r0, .L_08170650
	movs r2, #1
	movs r3, #0
	adds r5, r5, r6
	bl Func_08157cf4
	ldr r0, .L_08170654
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08170658
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	adds r3, r7, #0
	mov r9, r2
	adds r3, #24
	subs r2, #1
.L_0817044c:
	movs r6, #1
	add r9, r6
	mov r7, r9
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_0817044c
	ldr r1, [sp, #80]
	movs r2, #168
	movs r0, #0
	lsls r2, r2, #2
	mov r9, r0
	adds r5, r1, r2
.L_08170466:
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
	add r9, r3
	mov r6, r9
	adds r5, #28
	cmp r6, #16
	bne .L_08170466
	ldr r5, [sp, #80]
	movs r7, #0
	mov r9, r7
.L_0817049a:
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #28
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r1, #48
	bl Math_ModU
	adds r0, #56
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl Random16
	movs r2, #127
	ands r2, r0
	movs r3, #16
	subs r3, r3, r2
	movs r1, #1
	lsls r3, r3, #10
	mov r0, r9
	add r9, r1
	str r3, [r5, #16]
	mov r2, r9
	mvns r3, r0
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #32
	bne .L_0817049a
	movs r3, #0
	mov r9, r3
	ldr r3, .L_0817065c
	movs r2, #128
	subs r1, #2
	lsls r2, r2, #2
.L_081704f4:
	movs r6, #1
	add r9, r6
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_081704f4
	ldr r0, .L_08170660
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_08170664
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r0, #240
	ldr r7, [sp, #80]
	lsls r0, r0, #7
	adds r0, #240
	adds r3, r7, r0
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	str r1, [sp, #76]
.L_0817052e:
	ldr r3, .L_08170668
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08170548
	ldr r2, [sp, #76]
	cmp r2, #10
	ble .L_08170548
	cmp r2, #107
	bgt .L_08170554
	movs r3, #107
	str r3, [sp, #76]
.L_08170548:
	ldr r6, [sp, #76]
	cmp r6, #107
	bne .L_08170554
	movs r0, #212
	bl Audio_PlayCue
.L_08170554:
	ldr r7, [sp, #76]
	cmp r7, #148
	bne .L_08170560
	movs r0, #134
	bl Func_081180e8
.L_08170560:
	ldr r0, [sp, #76]
	cmp r0, #0
	bne .L_0817058a
	movs r1, #128
	lsls r1, r1, #14
	str r1, [sp, #60]
	movs r2, #144
	ldr r6, .L_0817066c
	movs r1, #201
	lsls r2, r2, #15
	movs r3, #144
	lsls r1, r1, #1
	str r2, [sp, #64]
	lsls r3, r3, #12
	movs r0, #2
	adds r1, #255
	movs r2, #2
	str r3, [sp, #52]
	str r6, [sp, #56]
	bl Func_08152404
.L_0817058a:
	ldr r7, [sp, #76]
	cmp r7, #108
	bne .L_081705bc
	ldr r0, [sp, #80]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #220
	adds r3, r0, r1
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r6, #238
	ldr r2, [sp, #80]
	lsls r6, r6, #7
	adds r6, #224
	adds r3, r2, r6
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	movs r1, #164
	movs r0, #11
	lsls r1, r1, #2
	movs r2, #2
	bl Func_08152404
.L_081705bc:
	ldr r7, [sp, #76]
	cmp r7, #107
	bgt .L_081705c4
	b .L_081706e4
.L_081705c4:
	ldr r0, [sp, #60]
	ldr r3, [sp, #64]
	ldr r2, [sp, #52]
	ldr r7, [sp, #52]
	ldr r6, [sp, #56]
	adds r1, r0, r2
	adds r2, r3, r6
	adds r0, r7, #0
	lsls r3, r7, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	subs r3, r3, r0
	str r1, [sp, #60]
	str r2, [sp, #64]
	cmp r3, #0
	bge .L_081705e6
	adds r3, #63
.L_081705e6:
	ldr r6, [sp, #56]
	asrs r3, r3, #6
	str r3, [sp, #52]
	lsls r3, r6, #4
	subs r3, r3, r6
	lsls r3, r3, #2
	subs r3, r3, r6
	cmp r3, #0
	bge .L_081705fa
	adds r3, #63
.L_081705fa:
	asrs r3, r3, #6
	movs r0, #4
	str r3, [sp, #56]
	bl Func_0816442c
	ldr r7, [sp, #76]
	cmp r7, #108
	bne .L_08170624
	ldr r3, .L_0817065c
	movs r0, #0
	movs r1, #1
	movs r2, #128
	mov r9, r0
	negs r1, r1
	lsls r2, r2, #2
.L_08170618:
	movs r6, #1
	add r9, r6
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_08170618
.L_08170624:
	ldr r7, [sp, #76]
	subs r7, #108
	cmp r7, #23
	bhi .L_081706e4
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	movs r2, #10
	subs r2, r2, r3
	mov r8, r2
	cmp r2, #0
	ble .L_081706e4
	ldr r1, [sp, #60]
	movs r0, #0
	lsrs r3, r1, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	mov r9, r0
	mov r10, r3
	b .L_081706d8
.L_0817064c:
	.4byte 0x00000192
.L_08170650:
	.4byte 0x00000188
.L_08170654:
	.4byte 0x0000017e
.L_08170658:
	.4byte Func_08143000
.L_0817065c:
	.4byte Data_02010018
.L_08170660:
	.4byte 0x00000150
.L_08170664:
	.4byte IwramCopyWords
.L_08170668:
	.4byte gInput
.L_0817066c:
	.4byte 0xfffe0000
.L_08170670:
	movs r2, #128
	lsls r3, r7, #5
	lsls r2, r2, #1
	add r3, r9
	adds r2, #255
	ands r3, r2
	lsls r6, r3, #3
	ldr r2, .L_081709ac
	subs r6, r6, r3
	lsls r6, r6, #2
	adds r6, r6, r2
	bl Random16
	adds r2, r0, #0
	str r2, [sp, #16]
	bl Random16
	ldr r2, [sp, #16]
	movs r5, #31
	ands r5, r0
	adds r0, r2, #0
	bl Trig_Sin
	adds r5, #16
	adds r3, r5, #0
	muls r3, r0
	ldr r2, [sp, #16]
	movs r0, #128
	lsls r0, r0, #13
	add r3, r10
	adds r3, r3, r0
	str r3, [r6]
	adds r0, r2, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	ldr r1, [sp, #64]
	ldr r2, .L_081709b0
	adds r3, r3, r1
	adds r3, r3, r2
	str r3, [r6, #4]
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	str r3, [r6, #24]
	movs r3, #1
	add r9, r3
.L_081706d8:
	mov r0, r8
	movs r1, #5
	bl Math_Div
	cmp r9, r0
	bne .L_08170670
.L_081706e4:
	ldr r3, [sp, #76]
	subs r3, #64
	cmp r3, #3
	bhi .L_081706fa
	movs r1, #128
	ldr r3, .L_081709b4
	ldr r0, [sp, #84]
	lsls r1, r1, #7
	ldr r2, .L_081709b8
	mov lr, r3
	.2byte 0xf800
.L_081706fa:
	ldr r6, [sp, #76]
	cmp r6, #103
	ble .L_08170706
	ldr r0, .L_081709bc
	bl Func_0815f0a0
.L_08170706:
	ldr r7, [sp, #76]
	cmp r7, #64
	bne .L_08170720
	ldr r0, .L_081709c0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081709c4
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08170720:
	ldr r0, [sp, #76]
	cmp r0, #63
	bgt .L_08170728
	b .L_081708d6
.L_08170728:
	cmp r0, #111
	bgt .L_08170806
	cmp r0, #0
	bge .L_08170732
	adds r0, #3
.L_08170732:
	movs r1, #3
	asrs r0, r0, #2
	bl __modsi3
	ldr r2, [sp, #76]
	adds r1, r0, #0
	lsls r0, r2, #11
	str r1, [sp, #20]
	bl Trig_Sin
	ldr r6, [sp, #76]
	movs r3, #108
	eors r3, r6
	ldr r1, [sp, #20]
	negs r2, r3
	lsls r0, r0, #3
	asrs r0, r0, #16
	orrs r2, r3
	adds r7, r0, #0
	lsrs r2, r2, #31
	ldr r0, [sp, #68]
	movs r6, #1
	subs r6, r6, r2
	lsls r5, r1, #1
	adds r5, r5, r1
	lsls r6, r6, #2
	ldr r1, [sp, #80]
	adds r6, r6, r0
	movs r3, #96
	movs r0, #64
	movs r2, #224
	str r3, [sp, #0]
	str r0, [sp, #4]
	lsls r2, r2, #3
	lsls r5, r5, #11
	mov r9, r2
	adds r5, r1, r5
	subs r7, #16
	add r5, r9
	ldr r0, [sp, #84]
	mov r8, r3
	adds r1, r5, #0
	adds r2, r7, #0
	ldr r4, [r6]
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
	movs r2, #52
	mov r1, r8
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #84]
	ldr r4, [r6]
	adds r1, r5, #0
	mov r10, r2
	movs r3, #64
	adds r2, r7, #0
	mov lr, r4
	.2byte 0xf800
	ldr r3, [sp, #76]
	movs r1, #3
	lsrs r0, r3, #31
	adds r0, r3, r0
	asrs r0, r0, #1
	bl __modsi3
	ldr r7, [sp, #76]
	movs r1, #128
	adds r3, r0, #0
	lsls r1, r1, #8
	lsls r0, r7, #12
	adds r0, r0, r1
	str r3, [sp, #12]
	bl Trig_Sin
	ldr r3, [sp, #12]
	ldr r2, [sp, #80]
	lsls r0, r0, #3
	lsls r5, r3, #1
	asrs r7, r0, #16
	adds r5, r5, r3
	movs r0, #64
	mov r3, r8
	str r3, [sp, #0]
	str r0, [sp, #4]
	lsls r5, r5, #11
	adds r5, r2, r5
	add r5, r9
	ldr r4, [r6]
	adds r1, r5, #0
	adds r2, r7, #0
	movs r3, #0
	ldr r0, [sp, #84]
	mov lr, r4
	.2byte 0xf800
	mov r1, r8
	mov r2, r10
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #84]
	ldr r4, [r6]
	adds r1, r5, #0
	adds r2, r7, #0
	movs r3, #64
	mov lr, r4
	.2byte 0xf800
.L_08170806:
	ldr r5, [sp, #80]
	movs r3, #0
	mov r9, r3
.L_0817080c:
	ldr r3, [r5, #24]
	cmp r3, #15
	bhi .L_0817085a
	adds r1, r3, #0
	cmp r3, #0
	bge .L_0817081a
	adds r1, r3, #3
.L_0817081a:
	ldr r7, [sp, #80]
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r0, #158
	adds r1, r7, r1
	lsls r0, r0, #7
	adds r1, r1, r0
	mov r6, r9
	movs r0, #32
	movs r4, #1
	movs r7, #64
	movs r3, #2
	ldrsh r2, [r5, r3]
	ands r4, r6
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	str r7, [sp, #4]
	ldr r0, [sp, #68]
	lsls r4, r4, #2
	subs r3, #32
	ldr r4, [r4, r0]
	subs r2, #16
	ldr r0, [sp, #84]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_081709c8
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_0817085a:
	adds r3, #1
	str r3, [r5, #24]
	ldr r1, [sp, #76]
	cmp r1, #108
	bne .L_0817087a
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	cmp r3, #0
	bge .L_08170876
	adds r3, #3
.L_08170876:
	asrs r3, r3, #2
	str r3, [r5, #16]
.L_0817087a:
	ldr r3, [sp, #76]
	cmp r3, #107
	bgt .L_081708ca
	ldr r3, [r5, #24]
	cmp r3, #16
	bne .L_0817088a
	movs r3, #0
	str r3, [r5, #24]
.L_0817088a:
	cmp r3, #0
	bne .L_081708ca
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #28
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r1, #48
	bl Math_ModU
	adds r0, #56
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl Random16
	movs r2, #127
	ands r2, r0
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #10
	str r3, [r5, #16]
.L_081708ca:
	movs r6, #1
	add r9, r6
	mov r7, r9
	adds r5, #28
	cmp r7, #24
	bne .L_0817080c
.L_081708d6:
	ldr r0, [sp, #76]
	cmp r0, #64
	bne .L_081708e2
	movs r0, #163
	bl Audio_PlayCue
.L_081708e2:
	movs r1, #0
	mov r9, r1
	movs r5, #75
.L_081708e8:
	ldr r2, [sp, #76]
	cmp r2, r5
	bne .L_081708f4
	movs r0, #134
	bl Audio_PlayCue
.L_081708f4:
	movs r3, #1
	add r9, r3
	mov r6, r9
	adds r5, #4
	cmp r6, #8
	bne .L_081708e8
	ldr r7, [sp, #76]
	cmp r7, #10
	bne .L_0817093e
	ldr r0, [sp, #80]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r0, r1
	str r6, [r3]
	ldr r6, [sp, #88]
	movs r2, #0
	ldr r3, [r6, #20]
	mov r9, r2
	cmp r3, #0
	beq .L_0817093e
	movs r6, #16
	movs r5, #36
.L_08170922:
	ldr r7, [sp, #88]
	mov r3, r9
	ldrsh r0, [r5, r7]
	movs r2, #5
	movs r1, #7
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r3, [r7, #20]
	movs r2, #1
	add r9, r2
	adds r5, #2
	cmp r9, r3
	bne .L_08170922
.L_0817093e:
	ldr r3, [sp, #76]
	cmp r3, #107
	ble .L_08170a02
	ldr r5, .L_081709ac
	movs r6, #0
	mov r9, r6
.L_0817094a:
	ldr r3, [r5, #24]
	cmp r3, #35
	bhi .L_081709f4
	lsrs r0, r3, #31
	adds r0, r3, r0
	movs r1, #9
	asrs r0, r0, #1
	bl __modsi3
	ldr r2, .L_081709cc
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	movs r3, #2
	ldrsh r2, [r5, r3]
	ldr r3, .L_081709d0
	ldr r7, .L_081709d4
	ldrb r4, [r3, r0]
	movs r6, #6
	ldrsh r3, [r5, r6]
	lsrs r0, r4, #1
	subs r3, r3, r0
	adds r1, r1, r7
	subs r2, r2, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #84]
	ldr r4, [sp, #100]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #58
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	mov r7, r9
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_081709dc
	ldr r3, [r5, #12]
	ldr r0, .L_081709d8
	movs r1, #128
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	lsls r1, r1, #5
	adds r3, r3, r1
	b .L_081709ec
	.2byte 0x0000
.L_081709ac:
	.4byte gMapCellBuffer
.L_081709b0:
	.4byte 0xfff80000
.L_081709b4:
	.4byte IwramFillWords
.L_081709b8:
	.4byte 0x3f3f3f3f
.L_081709bc:
	.4byte 0x00000167
.L_081709c0:
	.4byte 0x00000148
.L_081709c4:
	.4byte IwramCopyWords
.L_081709c8:
	.4byte 0xffffc000
.L_081709cc:
	.4byte Data_0819744c
.L_081709d0:
	.4byte Data_0819745e
.L_081709d4:
	.4byte Data_02016e16
.L_081709d8:
	.4byte 0xfffff800
.L_081709dc:
	ldr r3, [r5, #12]
	ldr r2, .L_08170d48
	movs r6, #128
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	lsls r6, r6, #4
	adds r3, r3, r6
.L_081709ec:
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_081709f4:
	movs r7, #1
	movs r0, #128
	add r9, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r9, r0
	bne .L_0817094a
.L_08170a02:
	ldr r1, [sp, #76]
	cmp r1, #63
	ble .L_08170a0a
	b .L_08170cee
.L_08170a0a:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #238
	movs r6, #192
	lsls r0, r0, #7
	movs r2, #0
	movs r3, #12
	lsls r6, r6, #13
	movs r7, #10
	adds r0, #220
	movs r1, #24
	str r3, [sp, #44]
	str r6, [sp, #40]
	str r7, [sp, #36]
	str r0, [sp, #32]
	str r1, [sp, #28]
	str r2, [sp, #24]
	mov r9, r2
.L_08170a30:
	mov r2, r9
	lsls r2, r2, #4
	mov r8, r2
	ldr r6, [sp, #76]
	mov r3, r8
	adds r3, #5
	cmp r6, r3
	bne .L_08170a46
	movs r0, #212
	bl Audio_PlayCue
.L_08170a46:
	ldr r7, [sp, #76]
	mov r3, r8
	adds r3, #9
	cmp r7, r3
	bne .L_08170a72
	ldr r0, [sp, #80]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r0, r1
	movs r2, #8
	str r2, [r3]
	movs r1, #240
	ldr r3, .L_08170d4c
	ldr r0, [sp, #84]
	lsls r1, r1, #6
	ldr r2, .L_08170d50
	mov lr, r3
	.2byte 0xf800
	movs r0, #144
	bl Audio_PlayCue
.L_08170a72:
	ldr r2, [sp, #76]
	cmp r2, r8
	bge .L_08170a7a
	b .L_08170c1c
.L_08170a7a:
	mov r3, r8
	subs r5, r2, r3
	lsls r3, r5, #4
	adds r2, r3, #0
	ldr r3, .L_08170d54
	subs r2, #16
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #92]
	str r4, [sp, #96]
	cmp r2, #131
	ble .L_08170a94
	movs r2, #132
.L_08170a94:
	add r1, sp, #108
	movs r3, #0
	str r3, [r1, #12]
	str r3, [r1, #4]
	ldr r6, [sp, #28]
	lsls r3, r6, #17
	str r3, [r1]
	lsls r3, r2, #16
	str r3, [r1, #8]
	ldr r2, [sp, #80]
	ldr r7, [sp, #32]
	movs r3, #0
	ldr r0, [r7, r2]
	add r2, sp, #92
	bl Func_08020010
	cmp r5, #9
	bne .L_08170ad0
	ldr r6, [sp, #80]
	movs r1, #128
	ldr r3, [r7, r6]
	ldr r7, .L_08170d58
	ldrh r0, [r3, #8]
	lsls r1, r1, #2
	lsls r0, r0, #22
	lsrs r0, r0, #17
	adds r0, r0, r7
	ldr r3, .L_08170d5c
	mov lr, r3
	.2byte 0xf800
.L_08170ad0:
	ldr r0, [sp, #36]
	ldr r1, [sp, #76]
	str r0, [sp, #48]
	cmp r1, r0
	bne .L_08170b3a
	ldr r3, [sp, #40]
	ldr r6, [sp, #24]
	ldr r0, .L_08170d60
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r7, r6, r0
.L_08170ae8:
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r3, #255
	adds r5, r0, #0
	ands r5, r3
	movs r3, #224
	mov r1, r10
	lsls r3, r3, #15
	str r1, [r7]
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	negs r0, r0
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	movs r3, #0
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #128
	add r11, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r11, r3
	bne .L_08170ae8
.L_08170b3a:
	ldr r6, [sp, #76]
	ldr r7, [sp, #48]
	cmp r6, r7
	blt .L_08170c1c
	mov r3, r8
	adds r3, #18
	cmp r6, r3
	bge .L_08170b7c
	adds r2, r6, #0
	add r2, r9
	cmp r2, #0
	bge .L_08170b54
	adds r2, #3
.L_08170b54:
	movs r3, #3
	asrs r2, r2, #2
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r1, r3, #4
	ldr r0, .L_08170d64
	subs r1, r1, r3
	movs r3, #24
	lsls r1, r1, #6
	str r3, [sp, #0]
	movs r3, #112
	adds r1, r1, r0
	str r3, [sp, #4]
	ldr r4, [sp, #100]
	ldr r0, [sp, #84]
	ldr r2, [sp, #44]
	movs r3, #0
	mov lr, r4
	.2byte 0xf800
.L_08170b7c:
	ldr r1, [sp, #76]
	ldr r2, [sp, #48]
	cmp r1, r2
	blt .L_08170c1c
	movs r3, #0
	mov r11, r3
	movs r7, #3
.L_08170b8a:
	mov r4, r11
	ands r4, r7
	str r4, [sp, #8]
	bl Random16
	movs r5, #7
	ldr r4, [sp, #8]
	ands r5, r0
	ldr r0, .L_08170d68
	ldr r6, [sp, #28]
	ldrb r3, [r0, r4]
	adds r5, r6, r5
	lsrs r3, r3, #1
	subs r5, r5, r3
	mov r10, r0
	bl Random16
	ldr r1, .L_08170d6c
	ldr r4, [sp, #8]
	movs r6, #63
	ldrb r3, [r1, r4]
	ands r6, r0
	lsrs r3, r3, #1
	mov r8, r1
	subs r6, r6, r3
	bl Random16
	ldr r3, .L_08170d70
	ands r0, r7
	ldrb r2, [r3, r0]
	ldr r0, [sp, #88]
	adds r3, r7, #0
	orrs r3, r2
	ldr r1, .L_08170d74
	ldr r2, [r0, #24]
	movs r0, #188
	ldrb r2, [r1, r2]
	movs r1, #7
	str r2, [sp, #0]
	movs r2, #7
	bl Func_08196404
	ldr r4, [sp, #8]
	ldr r2, .L_08170d78
	lsls r3, r4, #1
	mov r0, r10
	ldrh r1, [r2, r3]
	ldrb r3, [r0, r4]
	ldr r2, .L_08170d7c
	str r3, [sp, #0]
	adds r1, r1, r2
	mov r2, r8
	ldrb r3, [r2, r4]
	adds r6, #40
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	subs r5, #4
	ldr r4, [r3]
	ldr r0, [sp, #84]
	adds r3, r6, #0
	adds r2, r5, #0
	movs r6, #1
	mov lr, r4
	.2byte 0xf800
	add r11, r6
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	mov r0, r11
	cmp r0, #2
	bne .L_08170b8a
.L_08170c1c:
	ldr r1, [sp, #44]
	ldr r2, [sp, #40]
	movs r3, #128
	adds r1, #32
	lsls r3, r3, #14
	ldr r6, [sp, #36]
	adds r2, r2, r3
	ldr r7, [sp, #32]
	ldr r0, [sp, #28]
	str r1, [sp, #44]
	ldr r1, [sp, #24]
	str r2, [sp, #40]
	movs r3, #1
	movs r2, #224
	adds r6, #16
	lsls r2, r2, #5
	add r9, r3
	str r6, [sp, #36]
	adds r7, #4
	adds r0, #32
	adds r1, r1, r2
	mov r6, r9
	str r7, [sp, #32]
	str r0, [sp, #28]
	str r1, [sp, #24]
	cmp r6, #2
	beq .L_08170c54
	b .L_08170a30
.L_08170c54:
	movs r0, #188
	movs r1, #3
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r7, [sp, #68]
	str r3, [r7, #4]
	ldr r0, [sp, #76]
	cmp r0, #69
	bgt .L_08170cee
	ldr r5, .L_08170d60
	movs r1, #0
	mov r9, r1
.L_08170c74:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_08170ce0
	mov r0, r9
	movs r1, #3
	bl __modsi3
	ldr r3, [r5, #16]
	adds r4, r0, #1
	movs r6, #4
	cmp r3, #0
	bgt .L_08170c8e
	movs r6, #0
.L_08170c8e:
	ldr r2, .L_08170d80
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #72]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #68]
	subs r3, r3, r4
	ldr r4, [r6, r0]
	ldr r0, [sp, #84]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #7
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5, #16]
	cmp r3, #0
	ble .L_08170ce0
	movs r1, #6
	ldrsh r3, [r5, r1]
	cmp r3, #104
	ble .L_08170ce0
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_08170ce0:
	movs r2, #1
	movs r3, #128
	add r9, r2
	lsls r3, r3, #2
	adds r5, #28
	cmp r9, r3
	bne .L_08170c74
.L_08170cee:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r7, #240
	ldr r6, [sp, #80]
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r6, r7
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #76]
	adds r0, #1
	str r0, [sp, #76]
	cmp r0, #170
	beq .L_08170d1c
	bl .L_0817052e
.L_08170d1c:
	ldr r0, .L_08170d84
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #60]
	ldr r2, [sp, #64]
	movs r0, #4
	bl Func_0816467c
	movs r2, #238
	lsls r2, r2, #7
	movs r1, #0
	adds r2, #220
	mov r9, r1
	adds r5, r6, r2
	b .L_08170d88
	.2byte 0x0000
.L_08170d48:
	.4byte 0xfffff000
.L_08170d4c:
	.4byte IwramFillWords
.L_08170d50:
	.4byte 0x3f3f3f3f
.L_08170d54:
	.4byte Data_08196e94
.L_08170d58:
	.4byte 0x06010e00
.L_08170d5c:
	.4byte IwramClearWords
.L_08170d60:
	.4byte gMapCellBuffer
.L_08170d64:
	.4byte Data_02014c56
.L_08170d68:
	.4byte Data_08197492
.L_08170d6c:
	.4byte Data_08197498
.L_08170d70:
	.4byte Data_08198bf4
.L_08170d74:
	.4byte Data_08198bf8
.L_08170d78:
	.4byte Data_08197486
.L_08170d7c:
	.4byte Data_02014000
.L_08170d80:
	.4byte Data_08197410
.L_08170d84:
	.4byte Func_08143000
.L_08170d88:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r9, r3
	mov r6, r9
	cmp r6, #11
	bne .L_08170d88
	bl Func_08143bb8
	add sp, #124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
