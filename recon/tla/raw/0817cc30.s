.syntax unified
	.thumb
	.global Func_0817cc30
	.thumb_func
Func_0817cc30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #32]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	ldr r5, .L_0817cca0
	str r0, [sp, #28]
	ldr r1, [r3, #92]
	str r1, [sp, #24]
	ldr r3, [r3, #100]
	str r3, [sp, #20]
	bl Func_0813ba50
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0817cc9c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #36
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #16]
	bl Func_08144aac
	ldr r0, .L_0817cca4
	adds r1, r5, #0
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #24]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_0817cca8
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #20]
	b .L_0817ccac
	.2byte 0x0000
.L_0817cc9c:
	.4byte 0x00001010
.L_0817cca0:
	.4byte Data_02014000
.L_0817cca4:
	.4byte 0x0000013e
.L_0817cca8:
	.4byte 0x000000b7
.L_0817ccac:
	movs r3, #0
	ldr r0, .L_0817d03c
	bl Resource_LoadAndDecompress
	ldr r0, .L_0817d040
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817d044
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #24]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0817d048
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #32]
	mov r2, sp
	adds r2, #44
	movs r4, #36
	ldrsh r0, [r1, r4]
	adds r1, r2, #0
	str r2, [sp, #12]
	bl Func_0815e21c
	movs r3, #0
	str r3, [sp, #8]
	mov r11, r3
.L_0817cd02:
	mov r4, r11
	cmp r4, #0
	bne .L_0817cd48
	ldr r3, .L_0817d04c
	movs r0, #0
	movs r1, #1
	movs r2, #128
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #2
.L_0817cd16:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0817cd16
	ldr r2, [sp, #24]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0817d050
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r0, #145
	bl Audio_PlayCue
	movs r0, #238
	ldr r4, [sp, #24]
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r4, r0
	movs r3, #4
	str r3, [r2]
.L_0817cd48:
	mov r1, r11
	cmp r1, #32
	bne .L_0817cd7c
	ldr r3, [sp, #32]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r1, [sp, #32]
	movs r3, #240
	lsls r3, r3, #12
	movs r4, #36
	ldrsh r0, [r1, r4]
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl Func_0815f000
.L_0817cd7c:
	mov r2, r11
	cmp r2, #38
	bne .L_0817cd96
	movs r0, #134
	bl Func_081180e8
	movs r4, #238
	ldr r3, [sp, #24]
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #16
	str r3, [r2]
.L_0817cd96:
	mov r3, r11
	subs r3, #32
	cmp r3, #11
	bhi .L_0817cdce
	movs r1, #3
	mov r0, r11
	bl __modsi3
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r0, [sp, #24]
	lsls r3, r3, #11
	movs r1, #224
	adds r3, r0, r3
	lsls r1, r1, #3
	adds r2, r3, r1
	movs r3, #0
	movs r1, #192
	mov r8, r3
	lsls r1, r1, #5
.L_0817cdbe:
	ldrb r3, [r2]
	movs r4, #1
	asrs r3, r3, #1
	add r8, r4
	strb r3, [r2]
	adds r2, #1
	cmp r8, r1
	bne .L_0817cdbe
.L_0817cdce:
	mov r0, r11
	cmp r0, #41
	bhi .L_0817ce62
	ldr r1, [sp, #8]
	movs r3, #120
	subs r6, r3, r1
	movs r1, #3
	bl __modsi3
	ldr r2, [sp, #12]
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r9, r3
	ldr r3, [sp, #8]
	cmp r3, #0
	bge .L_0817cdf4
	adds r3, #63
.L_0817cdf4:
	ldr r4, [sp, #8]
	asrs r7, r3, #6
	lsls r3, r7, #6
	subs r7, r4, r3
	cmp r6, #0
	bge .L_0817ce02
	movs r6, #0
.L_0817ce02:
	lsls r3, r0, #1
	ldr r2, [sp, #24]
	adds r3, r3, r0
	lsls r3, r3, #11
	movs r1, #0
	adds r2, r2, r3
	mov r8, r1
	mov r10, r2
.L_0817ce12:
	mov r4, r8
	lsls r3, r4, #6
	subs r5, r3, r7
	movs r3, #108
	subs r2, r3, r5
	cmp r2, #64
	ble .L_0817ce22
	movs r2, #64
.L_0817ce22:
	cmp r2, #0
	ble .L_0817ce58
	adds r3, r6, #0
	subs r3, #64
	cmp r5, r3
	blt .L_0817ce58
	cmp r5, r6
	bge .L_0817ce38
	subs r3, r6, r5
	subs r2, r2, r3
	adds r5, r6, #0
.L_0817ce38:
	movs r0, #96
	str r0, [sp, #0]
	str r2, [sp, #4]
	ldr r1, [sp, #16]
	mov r3, r9
	ldr r1, [r1, #4]
	movs r4, #48
	mov r12, r1
	movs r1, #224
	lsls r1, r1, #3
	subs r2, r3, r4
	ldr r0, [sp, #28]
	add r1, r10
	adds r3, r5, #0
	mov lr, r12
	.2byte 0xf800
.L_0817ce58:
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #3
	bne .L_0817ce12
.L_0817ce62:
	mov r2, r11
	cmp r2, #0
	bne .L_0817ce84
	movs r3, #0
	mov r8, r3
	ldr r3, [sp, #24]
	movs r2, #4
	adds r3, #24
	negs r2, r2
.L_0817ce74:
	movs r4, #1
	add r8, r4
	mov r0, r8
	str r2, [r3]
	adds r3, #28
	subs r2, #1
	cmp r0, #32
	bne .L_0817ce74
.L_0817ce84:
	ldr r5, [sp, #24]
	movs r1, #0
	mov r8, r1
.L_0817ce8a:
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_0817cec6
	movs r1, #3
	bl __divsi3
	ldr r2, .L_0817d054
	adds r1, r0, #0
	lsls r1, r1, #11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	subs r2, #16
	subs r3, #32
	ldr r4, [sp, #36]
	ldr r0, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_0817d058
	bl BattleFxKernels_IntegrateVector2
	ldr r0, [r5, #24]
.L_0817cec6:
	adds r6, r0, #1
	str r6, [r5, #24]
	cmp r6, #0
	bne .L_0817cf06
	bl Random16
	movs r2, #31
	ands r2, r0
	ldr r0, [sp, #12]
	ldr r3, [r0]
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	adds r2, r2, r3
	subs r2, #16
	lsls r2, r2, #16
	str r2, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #88
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #12]
	str r6, [r5, #16]
.L_0817cf06:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #24
	bne .L_0817ce8a
	mov r3, r11
	cmp r3, #28
	bne .L_0817cf9c
	ldr r7, .L_0817d05c
	movs r4, #0
	movs r0, #63
	mov r8, r4
	mov r10, r0
.L_0817cf22:
	ldr r3, [r7, #24]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_0817cf8e
	bl Random16
	mov r2, r10
	adds r6, r0, #0
	ands r6, r2
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
	movs r4, #128
	lsls r4, r4, #14
	asrs r3, r3, #3
	adds r3, r3, r4
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r0, #192
	lsls r0, r0, #15
	asrs r3, r3, #2
	adds r3, r3, r0
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
.L_0817cf8e:
	movs r3, #1
	movs r4, #128
	add r8, r3
	lsls r4, r4, #1
	adds r7, #28
	cmp r8, r4
	bne .L_0817cf22
.L_0817cf9c:
	mov r0, r11
	cmp r0, #31
	bhi .L_0817d006
	ldr r5, .L_0817d05c
	movs r6, #0
	mov r8, r6
	movs r7, #63
.L_0817cfaa:
	ldr r3, [r5, #24]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_0817cff8
	bl Random16
	ldr r4, [sp, #12]
	movs r2, #31
	ldr r3, [r4]
	ands r2, r0
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	adds r2, r2, r3
	subs r2, #16
	movs r3, #216
	lsls r3, r3, #15
	lsls r2, r2, #16
	str r3, [r5, #4]
	str r2, [r5]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	negs r0, r0
	subs r0, #8
	lsls r0, r0, #13
	movs r3, #0
	adds r6, #1
	str r0, [r5, #16]
	str r3, [r5, #24]
	cmp r6, #16
	beq .L_0817d006
.L_0817cff8:
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #2
	adds r5, #28
	cmp r8, r1
	bne .L_0817cfaa
.L_0817d006:
	ldr r5, .L_0817d05c
	movs r2, #0
	mov r8, r2
.L_0817d00c:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0817d0d2
	movs r1, #3
	mov r0, r8
	bl __modsi3
	ldr r2, .L_0817d060
	adds r6, r0, #3
	lsls r4, r6, #1
	mov r3, r8
	movs r0, #1
	ands r0, r3
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	lsls r0, r0, #2
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r3, r3, #1
	b .L_0817d064
.L_0817d03c:
	.4byte 0x00000134
.L_0817d040:
	.4byte 0x00000130
.L_0817d044:
	.4byte IwramCopyWords
.L_0817d048:
	.4byte Func_08143000
.L_0817d04c:
	.4byte Data_02010018
.L_0817d050:
	.4byte 0x000000b7
.L_0817d054:
	.4byte Data_02014000
.L_0817d058:
	.4byte 0xffffe000
.L_0817d05c:
	.4byte gMapCellBuffer
.L_0817d060:
	.4byte Data_08197410
.L_0817d064:
	subs r2, r2, r3
	mov lr, r2
	movs r3, #6
	ldrsh r2, [r5, r3]
	str r6, [sp, #0]
	subs r2, r2, r6
	mov r12, r2
	str r4, [sp, #4]
	ldr r2, [sp, #16]
	mov r3, r12
	ldr r4, [r0, r2]
	ldr r0, [sp, #28]
	mov r2, lr
	mov lr, r4
	.2byte 0xf800
	mov r3, r11
	movs r7, #3
	cmp r3, #31
	ble .L_0817d098
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #7
	bl BattleFxKernels_IntegrateVector2
	b .L_0817d0aa
.L_0817d098:
	ldr r3, .L_0817d16c
	mov r2, r8
	ands r2, r7
	lsls r2, r2, #2
	ldr r2, [r3, r2]
	adds r0, r5, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector2
.L_0817d0aa:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5, #16]
	cmp r3, #0
	ble .L_0817d0c6
	ldr r2, [r5, #4]
	asrs r3, r2, #16
	cmp r3, #104
	ble .L_0817d0c8
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
	b .L_0817d0c8
.L_0817d0c6:
	ldr r2, [r5, #4]
.L_0817d0c8:
	cmn r2, r6
	bge .L_0817d0d2
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_0817d0d2:
	movs r4, #1
	movs r0, #128
	add r8, r4
	lsls r0, r0, #2
	adds r5, #28
	cmp r8, r0
	bne .L_0817d00c
	mov r1, r11
	cmp r1, #37
	bgt .L_0817d0f4
	ldr r3, [sp, #24]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #16
	str r3, [r2]
.L_0817d0f4:
	mov r0, r11
	cmp r0, #7
	bgt .L_0817d104
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	b .L_0817d11c
.L_0817d104:
	mov r1, r11
	cmp r1, #37
	bgt .L_0817d114
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	b .L_0817d11c
.L_0817d114:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
.L_0817d11c:
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #24]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #8]
	movs r1, #1
	add r11, r1
	adds r0, #8
	mov r2, r11
	str r0, [sp, #8]
	cmp r2, #92
	beq .L_0817d146
	b .L_0817cd02
.L_0817d146:
	ldr r0, .L_0817d170
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0817d16c:
	.4byte Data_081994a8
.L_0817d170:
	.4byte Func_08143000
