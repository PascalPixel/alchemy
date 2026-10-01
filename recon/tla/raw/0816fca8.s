.syntax unified
	.thumb
	.global Func_0816fca8
	.thumb_func
Func_0816fca8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #96
	str r0, [sp, #60]
	str r1, [sp, #56]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	ldr r0, [r3, #92]
	str r1, [sp, #52]
	mov r11, r0
	ldr r3, [r3, #100]
	movs r0, #1
	str r3, [sp, #36]
	bl BattleFx_BeginCanvasLayer
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_0816fce4
	movs r2, #128
	ldr r3, .L_0816fce0
	b .L_0816fce8
	.2byte 0x0000
.L_0816fce0:
	.4byte 0x00001010
.L_0816fce4:
	movs r2, #128
	ldr r3, .L_0816fd24
.L_0816fce8:
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #240
	lsls r1, r1, #4
	str r3, [sp, #40]
	ldr r0, .L_0816fd28
	movs r3, #0
	add r1, r11
	movs r2, #1
	str r3, [sp, #32]
	bl Resource_LoadAndDecompress
	ldr r0, .L_0816fd2c
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0816fd30
	ldr r1, .L_0816fd34
	movs r2, #1
	b .L_0816fd38
.L_0816fd24:
	.4byte 0x00000810
.L_0816fd28:
	.4byte 0x00000192
.L_0816fd2c:
	.4byte 0x00000134
.L_0816fd30:
	.4byte 0x00000188
.L_0816fd34:
	.4byte gMapCellBuffer
.L_0816fd38:
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r7, [sp, #56]
	cmp r7, #1
	bne .L_0816fd58
	ldr r0, .L_0816fff4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816fff8
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0816fd58:
	movs r0, #216
	lsls r0, r0, #5
	adds r0, #86
	movs r1, #128
	lsls r1, r1, #7
	add r0, r11
	ldr r3, .L_0816fffc
	mov lr, r3
	.2byte 0xf800
	movs r1, #216
	lsls r1, r1, #5
	movs r0, #0
	adds r1, #90
	mov r9, r0
	movs r7, #0
	mov r12, r1
.L_0816fd78:
	mov r2, r9
	lsls r3, r2, #12
	mov r0, r11
	movs r6, #0
	lsls r5, r7, #6
	adds r4, r3, r0
.L_0816fd84:
	ldr r3, .L_08170000
	mov r2, r12
	adds r1, r4, r2
	movs r0, #0
	adds r2, r5, r3
.L_0816fd8e:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #24
	bne .L_0816fd8e
	adds r6, #1
	adds r5, #24
	adds r4, #32
	cmp r6, #120
	bne .L_0816fd84
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r7, #45
	cmp r1, #4
	bne .L_0816fd78
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_0816fdce
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	b .L_0816fde2
.L_0816fdce:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #0
.L_0816fde2:
	str r3, [r2]
	movs r1, #200
	ldr r0, .L_08170004
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #56]
	cmp r3, #0
	bne .L_0816fdfa
	movs r7, #24
	str r7, [sp, #28]
	b .L_0816fdfe
.L_0816fdfa:
	movs r0, #40
	str r0, [sp, #28]
.L_0816fdfe:
	movs r1, #0
	str r1, [sp, #48]
.L_0816fe02:
	ldr r2, [sp, #48]
	cmp r2, #0
	bne .L_0816fef4
	ldr r3, [sp, #60]
	add r5, sp, #72
	ldr r0, [r3, #8]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r1, [sp, #60]
	add r6, sp, #84
	movs r7, #36
	ldrsh r0, [r1, r7]
	adds r1, r6, #0
	bl Func_0815e21c
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_0816fe44
	ldr r2, [r5]
	mov r3, r11
	lsls r2, r2, #16
	str r2, [r3]
	mov r7, r11
	ldr r3, [r5, #4]
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r7, #4]
	ldr r1, [sp, #28]
	ldr r0, [r6]
	lsls r0, r0, #16
	subs r0, r0, r2
	b .L_0816fe78
.L_0816fe44:
	ldr r0, [sp, #60]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0816fe58
	ldr r3, [r5]
	mov r1, r11
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r1]
	b .L_0816fe62
.L_0816fe58:
	ldr r3, [r5]
	mov r2, r11
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r2]
.L_0816fe62:
	ldr r3, [r5, #4]
	mov r7, r11
	subs r3, #80
	lsls r3, r3, #16
	str r3, [r7, #4]
	ldr r3, [r7]
	ldr r0, [r6]
	ldr r1, [sp, #28]
	subs r0, #32
	lsls r0, r0, #16
	subs r0, r0, r3
.L_0816fe78:
	bl __divsi3
	str r0, [r7, #12]
	ldr r0, [r6, #4]
	mov r1, r11
	ldr r3, [r1, #4]
	subs r0, #32
	lsls r0, r0, #16
	subs r0, r0, r3
	ldr r1, [sp, #28]
	bl __divsi3
	mov r2, r11
	str r0, [r2, #16]
	movs r0, #221
	bl Audio_PlayCue
	mov r5, r11
	movs r3, #0
	mov r9, r3
	adds r5, #28
.L_0816fea2:
	bl Random16
	str r0, [r5, #8]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	str r3, [r5, #20]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_0816fec6
	ldr r3, [r5, #20]
	negs r3, r3
	str r3, [r5, #20]
.L_0816fec6:
	bl Random16
	movs r7, #1
	movs r3, #3
	add r9, r7
	ands r3, r0
	mov r0, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #63
	bne .L_0816fea2
	ldr r3, .L_08170008
	movs r1, #0
	movs r2, #128
	mov r9, r1
	lsls r2, r2, #2
	subs r1, #1
.L_0816fee8:
	movs r7, #1
	add r9, r7
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_0816fee8
.L_0816fef4:
	ldr r0, [sp, #48]
	ldr r1, [sp, #28]
	cmp r0, r1
	bne .L_0816ffb0
	ldr r7, .L_0817000c
	movs r2, #0
	mov r8, r2
.L_0816ff02:
	mov r0, r11
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7]
	ldr r3, [r0, #4]
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	movs r1, #200
	lsls r1, r1, #1
	bl __umodsi3
	adds r5, r0, #0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	movs r1, #1
	ands r3, r0
	add r8, r1
	adds r3, #16
	mov r2, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #128
	bne .L_0816ff02
	movs r1, #240
	ldr r0, [sp, #52]
	ldr r3, .L_08170010
	lsls r1, r1, #6
	ldr r2, .L_08170014
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	movs r3, #16
	add r2, r11
	str r3, [r2]
	ldr r7, [sp, #60]
	movs r1, #4
	movs r3, #36
	ldrsh r0, [r7, r3]
	bl Func_08118088
	movs r3, #8
	movs r1, #36
	ldrsh r0, [r7, r1]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_0816ffa2
	movs r0, #145
	bl Func_081180e8
	b .L_0816ffb0
.L_0816ffa2:
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
	movs r0, #145
	bl Audio_PlayCue
.L_0816ffb0:
	ldr r3, [sp, #48]
	ldr r7, [sp, #28]
	cmp r3, r7
	ble .L_0816ffba
	b .L_08170124
.L_0816ffba:
	mov r0, r11
	ldr r3, [r0, #24]
	adds r3, #1
	str r3, [r0, #24]
	ldr r3, [r0]
	asrs r3, r3, #17
	str r3, [sp, #24]
	ldr r3, [sp, #56]
	movs r2, #6
	ldrsh r1, [r0, r2]
	str r1, [sp, #20]
	cmp r3, #0
	bne .L_0816ffe0
	ldr r7, [sp, #48]
	lsls r3, r7, #1
	adds r3, #16
	str r3, [sp, #32]
	movs r3, #48
	b .L_0816ffea
.L_0816ffe0:
	ldr r0, [sp, #48]
	lsls r3, r0, #1
	adds r3, #16
	str r3, [sp, #32]
	movs r3, #64
.L_0816ffea:
	ldr r1, [sp, #32]
	cmp r1, r3
	ble .L_08170018
	str r3, [sp, #32]
	b .L_0817005c
.L_0816fff4:
	.4byte 0x00000165
.L_0816fff8:
	.4byte IwramCopyWords
.L_0816fffc:
	.4byte IwramClearWords
.L_08170000:
	.4byte gMapCellBuffer
.L_08170004:
	.4byte Func_08143000
.L_08170008:
	.4byte Data_02014018
.L_0817000c:
	.4byte Data_02014000
.L_08170010:
	.4byte IwramFillWords
.L_08170014:
	.4byte 0x3f3f3f3f
.L_08170018:
	movs r2, #167
	lsls r2, r2, #9
	adds r2, #32
	ldr r0, .L_0817032c
	ldr r1, [sp, #32]
	bl Func_0815b434
	ldr r2, [sp, #56]
	cmp r2, #1
	bne .L_0817005c
	ldr r7, [sp, #32]
	movs r3, #0
	mov r9, r3
	adds r3, r7, #0
	muls r3, r7
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	cmp r3, #0
	beq .L_0817005c
	ldr r1, .L_0817032c
	movs r0, #64
	mov r12, r3
.L_08170046:
	ldrb r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08170052
	subs r3, r0, r2
	strb r3, [r1]
.L_08170052:
	movs r2, #1
	add r9, r2
	adds r1, #1
	cmp r9, r12
	bne .L_08170046
.L_0817005c:
	ldr r3, [sp, #32]
	ldr r0, .L_0817032c
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	bl Func_0818caa8
	ldr r7, [sp, #32]
	movs r3, #0
	asrs r7, r7, #31
	str r7, [sp, #16]
	mov r9, r3
	movs r7, #3
.L_08170074:
	ldr r0, [sp, #16]
	ldr r1, [sp, #32]
	mov r4, r9
	ands r4, r7
	lsrs r5, r0, #31
	str r4, [sp, #12]
	adds r5, r1, r5
	bl Random16
	adds r6, r0, #0
	bl Trig_Sin
	asrs r5, r5, #1
	adds r2, r5, #0
	muls r2, r0
	ldr r3, [sp, #24]
	ldr r4, [sp, #12]
	ldr r0, .L_08170330
	asrs r2, r2, #16
	mov r8, r2
	add r8, r3
	ldrb r3, [r0, r4]
	mov r1, r8
	lsrs r3, r3, #1
	subs r1, r1, r3
	mov r10, r0
	adds r0, r6, #0
	mov r8, r1
	bl Trig_Cos
	ldr r6, .L_08170334
	ldr r4, [sp, #12]
	muls r5, r0
	ldr r2, [sp, #20]
	ldrb r3, [r6, r4]
	asrs r5, r5, #16
	adds r5, r2, r5
	lsrs r3, r3, #1
	subs r5, r5, r3
	bl Random16
	ldr r3, .L_08170338
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #1
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r4, [sp, #12]
	ldr r2, .L_0817033c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	movs r3, #240
	lsls r3, r3, #4
	mov r0, r10
	add r1, r11
	adds r1, r1, r3
	ldrb r3, [r0, r4]
	movs r2, #192
	str r3, [sp, #0]
	lsls r2, r2, #18
	ldrb r3, [r6, r4]
	adds r2, #188
	str r3, [sp, #4]
	ldr r0, [sp, #52]
	ldr r4, [r2]
	adds r3, r5, #0
	mov r2, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #4
	bne .L_08170074
	mov r0, r11
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
.L_08170124:
	ldr r1, [sp, #56]
	cmp r1, #0
	bne .L_081701c2
	ldr r2, [sp, #48]
	ldr r3, [sp, #28]
	cmp r2, r3
	blt .L_081701cc
	ldr r6, .L_08170340
	movs r7, #0
	mov r9, r7
.L_08170138:
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_081701b4
	asrs r3, r3, #3
	adds r5, r3, #2
	ldr r0, .L_08170344
	lsls r1, r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	str r1, [sp, #8]
	asrs r3, r3, #1
	movs r4, #0
	mov r10, r0
	mov r8, r3
.L_08170154:
	ldr r3, [sp, #8]
	mov r2, r10
	subs r3, #2
	ldrh r1, [r2, r3]
	movs r7, #2
	ldrsh r2, [r6, r7]
	ldr r3, [sp, #36]
	mov r0, r8
	adds r1, r3, r1
	subs r2, r2, r0
	movs r7, #6
	ldrsh r3, [r6, r7]
	ldr r0, [sp, #8]
	str r4, [sp, #12]
	str r0, [sp, #4]
	subs r3, r3, r5
	str r5, [sp, #0]
	ldr r0, [sp, #52]
	ldr r7, [sp, #40]
	mov lr, r7
	.2byte 0xf800
	movs r2, #128
	adds r0, r6, #0
	movs r1, #62
	lsls r2, r2, #2
	bl BattleFxKernels_IntegrateVector2
	ldr r4, [sp, #12]
	adds r4, #1
	cmp r4, #8
	bne .L_08170154
	movs r0, #6
	ldrsh r3, [r6, r0]
	cmp r3, #111
	ble .L_081701ae
	ldr r2, [r6, #16]
	negs r2, r2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_081701aa
	adds r3, #63
.L_081701aa:
	asrs r3, r3, #6
	str r3, [r6, #16]
.L_081701ae:
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_081701b4:
	movs r1, #1
	movs r2, #128
	add r9, r1
	lsls r2, r2, #2
	adds r6, #28
	cmp r9, r2
	bne .L_08170138
.L_081701c2:
	ldr r3, [sp, #48]
	ldr r7, [sp, #28]
	cmp r3, r7
	ble .L_081701cc
	b .L_081702e2
.L_081701cc:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08170348
	ldr r3, [sp, #64]
	adds r6, r0, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0817034c
	ldr r0, [sp, #56]
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #64]
	cmp r0, #0
	bne .L_081701fc
	movs r3, #6
	b .L_081701fe
.L_081701fc:
	movs r3, #9
.L_081701fe:
	str r3, [r6]
	movs r3, #6
	str r3, [r6]
	ldr r3, .L_08170350
	add r1, sp, #64
	str r1, [r6, #16]
	str r3, [r6, #8]
	str r7, [r6, #12]
	movs r2, #0
	mov r5, r11
	mov r8, r1
	mov r9, r2
	adds r5, #28
.L_08170218:
	ldr r3, [sp, #48]
	cmp r3, r9
	blt .L_081702ca
	ldr r0, [sp, #48]
	mov r3, r9
	adds r3, #8
	cmp r0, r3
	bge .L_081702ca
	movs r2, #0
	cmp r0, r9
	bne .L_08170232
	subs r2, #16
	b .L_08170246
.L_08170232:
	ldr r1, [sp, #48]
	mov r3, r9
	adds r3, #4
	cmp r1, r3
	blt .L_08170246
	mov r2, r9
	subs r3, r1, r2
	lsls r3, r3, #4
	subs r3, #64
	negs r2, r3
.L_08170246:
	ldr r3, [r5, #24]
	movs r0, #216
	lsls r3, r3, #12
	lsls r0, r0, #5
	adds r0, #86
	add r3, r11
	adds r3, r3, r0
	mov r1, r8
	str r3, [r1, #4]
	str r2, [r6, #20]
	bl Func_08014de4
	mov r2, r11
	ldr r0, [r2]
	ldr r1, [r2, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, .L_08170354
	asrs r0, r0, #1
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	ldr r0, [r5, #8]
	bl Func_080150e4
	ldr r3, [r5, #8]
	ldr r2, [r5, #20]
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [sp, #56]
	cmp r3, #0
	bne .L_081702a4
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #128
	adds r1, r2, #0
	lsls r0, r0, #11
	bl Func_080151e4
	ldr r0, .L_08170358
	adds r1, r7, #0
	movs r2, #4
	bl Func_08196958
	b .L_081702c4
.L_081702a4:
	ldr r0, [sp, #32]
	movs r2, #128
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #3
	subs r1, r1, r0
	lsls r1, r1, #3
	lsls r0, r0, #11
	lsls r2, r2, #9
	bl Func_080151e4
	ldr r0, .L_0817035c
	adds r1, r7, #0
	movs r2, #4
	bl Func_08196958
.L_081702c4:
	adds r0, r6, #0
	bl Func_08196a7c
.L_081702ca:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #40
	bne .L_08170218
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r7, #0
	bl Sys_Free
.L_081702e2:
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #48]
	adds r3, #1
	str r3, [sp, #48]
	cmp r3, #50
	beq .L_0817030c
	b .L_0816fe02
.L_0817030c:
	ldr r0, .L_08170360
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0817032c:
	.4byte gMapCellBuffer
.L_08170330:
	.4byte Data_08197492
.L_08170334:
	.4byte Data_08197498
.L_08170338:
	.4byte Data_08198bee
.L_0817033c:
	.4byte Data_08197486
.L_08170340:
	.4byte Data_02014000
.L_08170344:
	.4byte Data_08197410
.L_08170348:
	.4byte 0xffffff00
.L_0817034c:
	.4byte 0xffff00ff
.L_08170350:
	.4byte Data_08199268
.L_08170354:
	.4byte 0xffc00000
.L_08170358:
	.4byte Data_081991b0
.L_0817035c:
	.4byte Data_081991c0
.L_08170360:
	.4byte Func_08143000
