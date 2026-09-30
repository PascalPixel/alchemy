.syntax unified
	.thumb
	.global Func_08122d10
	.thumb_func
Func_08122d10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	sub sp, #32
	movs r0, #192
	movs r1, #128
	str r3, [sp, #8]
	lsls r0, r0, #3
	lsls r1, r1, #4
	adds r0, #228
	adds r1, #44
	adds r7, r3, r0
	adds r3, r3, r1
	ldr r3, [r3]
	cmp r3, #0
	bne .L_08122d40
	b .L_08123494
.L_08122d40:
	movs r2, #164
	lsls r2, r2, #1
	adds r5, r7, r2
	ldr r3, [r5]
	cmp r3, #4
	bne .L_08122d4e
	b .L_08123494
.L_08122d4e:
	cmp r3, #1
	bne .L_08122da8
	ldr r4, [sp, #8]
	movs r0, #192
	movs r3, #160
	lsls r0, r0, #3
	lsls r3, r3, #1
	adds r0, #125
	adds r6, r7, r3
	adds r3, r4, r0
	movs r2, #0
	ldrsb r2, [r3, r2]
	ldr r3, [r6]
	cmp r3, r2
	bge .L_08122da2
	movs r1, #162
	lsls r1, r1, #1
	movs r4, #166
	movs r2, #0
	adds r3, r7, r1
	lsls r4, r4, #1
	movs r0, #168
	str r2, [r3]
	lsls r0, r0, #1
	adds r3, r7, r4
	str r2, [r3]
	adds r3, r7, r0
	str r2, [r3]
	ldr r1, [sp, #8]
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #124
	adds r0, r1, r2
	ldr r1, [r6]
	bl Battle_ResolveTargetAction
	ldr r3, [r6]
	adds r3, #1
	str r3, [r6]
	movs r3, #2
	str r3, [r5]
	b .L_08122d40
.L_08122da2:
	movs r3, #4
	str r3, [r5]
	b .L_08122d40
.L_08122da8:
	cmp r3, #2
	beq .L_08122dae
	b .L_0812302c
.L_08122dae:
	movs r4, #166
	movs r0, #162
	lsls r4, r4, #1
	lsls r0, r0, #1
	adds r3, r7, r4
	adds r2, r7, r0
	ldr r5, [r3]
	ldr r3, [r2]
	cmp r5, r3
	blt .L_08122dc4
	b .L_08123018
.L_08122dc4:
	adds r6, r5, #0
.L_08122dc6:
	movs r1, #168
	lsls r1, r1, #1
	adds r2, r7, r1
	ldr r3, [r2]
	cmp r3, #0
	beq .L_08122dd8
	subs r3, #1
	str r3, [r2]
	b .L_08123494
.L_08122dd8:
	ldrb r3, [r7, r6]
	cmp r3, #15
	bls .L_08122de0
	b .L_08122ff0
.L_08122de0:
	ldr r2, .L_0812306c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08122de8:
	.4byte .L_08122e42
	.4byte .L_08122e50
	.4byte .L_08122e5e
	.4byte .L_08122e74
	.4byte .L_08122e98
	.4byte .L_08122eb8
	.4byte .L_08122e8a
	.4byte .L_08122ed2
	.4byte .L_08122ee4
	.4byte .L_08122f26
	.4byte .L_08122fa6
	.4byte .L_08122fb6
	.4byte .L_08122ed8
	.4byte .L_08122e34
	.4byte .L_08122e28
	.4byte .L_08122fe6
.L_08122e28:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	bl Audio_PlayCue
	b .L_08122ff0
.L_08122e34:
	lsls r3, r6, #2
	adds r3, #64
	ldr r1, [r7, r3]
	adds r0, r7, #0
	bl Battle_SetRuntimeFlagBit0
	b .L_08122ff0
.L_08122e42:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r1, #1
	bl UiText_DrawQuantity
	b .L_08122ff0
.L_08122e50:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r1, #5
	bl UiText_DrawQuantity
	b .L_08122ff0
.L_08122e5e:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r1, #2
	bl UiText_DrawQuantity
	b .L_08122ff0
.L_08122e74:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r0, r3
	movs r1, #4
	bl UiText_DrawQuantity
	b .L_08122ff0
.L_08122e8a:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r2, [r3]
	movs r3, #1
	str r3, [r2, #8]
	b .L_08122ff0
.L_08122e98:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	cmp r0, #0
	blt .L_08122ea6
	bl Func_080381c0 + 0x10
.L_08122ea6:
	movs r3, #164
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #3
	str r3, [r2]
	ldr r2, .L_08123070
	movs r3, #0
	str r3, [r2, #28]
	b .L_08122ff0
.L_08122eb8:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	cmp r0, #0
	blt .L_08122ec6
	bl Func_080381c0 + 0x10
.L_08122ec6:
	movs r4, #164
	lsls r4, r4, #1
	adds r2, r7, r4
	movs r3, #13
	str r3, [r2]
	b .L_08122ff0
.L_08122ed2:
	bl Func_08038118
	b .L_08122ff0
.L_08122ed8:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	bl Func_08120178
	b .L_08122ff0
.L_08122ee4:
	movs r0, #180
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	cmp r0, #0
	ble .L_08122ef6
	bl Audio_PlayCue
.L_08122ef6:
	movs r3, #178
	lsls r3, r3, #1
	adds r2, r7, r3
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	str r0, [r2]
	bl GetBattleObjectSlot
	movs r1, #5
	ldr r0, [r0]
	bl Object_SetMode
	movs r4, #164
	lsls r4, r4, #1
	movs r0, #168
	adds r2, r7, r4
	movs r3, #10
	lsls r0, r0, #1
	str r3, [r2]
	adds r2, r7, r0
	movs r3, #0
	str r3, [r2]
	b .L_08122ff0
.L_08122f26:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	movs r1, #178
	lsls r1, r1, #1
	adds r5, r7, r1
	str r0, [r5]
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r1, [r3]
	bl Func_0812824c
	ldr r0, [r5]
	bl Func_0811fe3c
	ldr r0, [r5]
	bl Owner_GetState
	movs r5, #0
	adds r6, r0, #0
	b .L_08122f6e
.L_08122f52:
	movs r4, #149
	lsls r4, r4, #1
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_08122f66
	movs r1, #4
	bl Animation_ApplyChildArgumentFar
	b .L_08122f6c
.L_08122f66:
	movs r1, #5
	bl Animation_ApplyChildArgumentFar
.L_08122f6c:
	adds r5, #1
.L_08122f6e:
	movs r0, #178
	lsls r0, r0, #1
	adds r3, r7, r0
	ldr r0, [r3]
	bl GetBattleObjectSlot
	adds r1, r5, #0
	ldr r0, [r0]
	bl GetMotionRecord
	cmp r0, #0
	bne .L_08122f52
	movs r1, #149
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_08122ff0
	movs r2, #164
	lsls r2, r2, #1
	movs r4, #168
	adds r3, r7, r2
	lsls r4, r4, #1
	movs r2, #11
	str r2, [r3]
	adds r3, r7, r4
	str r0, [r3]
	b .L_08122ff0
.L_08122fa6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_DrawPartyStatusContentsFar
	b .L_08122ff0
.L_08122fb6:
	lsls r5, r6, #2
	adds r5, #64
	ldr r0, [r7, r5]
	bl GetBattleObjectSlot
	adds r1, r0, #0
	ldr r0, [r7, r5]
	bl Func_0811b4d8
	ldr r0, [r7, r5]
	bl GetBattleObjectSlot
	adds r6, r0, #0
	ldr r0, [r7, r5]
	bl Func_0811a484
	adds r1, r0, #0
	ldr r0, [r6]
	bl Func_0811f030
	ldr r0, [r7, r5]
	bl BattlePres_SetActorModeAndAction
	b .L_08122ff0
.L_08122fe6:
	lsls r3, r6, #2
	adds r3, #64
	ldr r0, [r7, r3]
	bl Func_0811f3b8
.L_08122ff0:
	movs r0, #166
	lsls r0, r0, #1
	adds r2, r7, r0
	ldr r3, [r2]
	movs r1, #162
	adds r5, r3, #1
	str r5, [r2]
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r3, [r3]
	cmp r5, r3
	bge .L_08123018
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r3, [r3]
	adds r6, r5, #0
	cmp r3, #2
	bne .L_08123018
	b .L_08122dc6
.L_08123018:
	movs r3, #164
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	cmp r3, #2
	beq .L_08123026
	b .L_08122d40
.L_08123026:
	movs r3, #1
	str r3, [r2]
	b .L_08122d40
.L_0812302c:
	cmp r3, #3
	beq .L_08123034
	cmp r3, #13
	bne .L_08123078
.L_08123034:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	bne .L_0812303e
	b .L_08123494
.L_0812303e:
	ldr r3, [r5]
	cmp r3, #13
	bne .L_08123050
	movs r4, #168
	movs r3, #2
	lsls r4, r4, #1
	str r3, [r5]
	adds r2, r7, r4
	b .L_081231c0
.L_08123050:
	movs r0, #176
	movs r3, #5
	lsls r0, r0, #1
	str r3, [r5]
	adds r2, r7, r0
	subs r3, #6
	str r3, [r2]
	ldr r3, .L_08123074
	movs r1, #168
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r7, r1
	str r3, [r2]
	b .L_08122d40
.L_0812306c:
	.4byte .L_08122de8
.L_08123070:
	.4byte gInput
.L_08123074:
	.4byte gFrameTick
.L_08123078:
	cmp r3, #5
	beq .L_0812307e
	b .L_081231d0
.L_0812307e:
	ldr r4, .L_08123130
	movs r2, #170
	ldr r3, [r4]
	lsls r2, r2, #1
	adds r2, r2, r7
	ldr r1, .L_08123134
	mov r9, r2
	lsrs r3, r3, #2
	movs r2, #7
	ands r3, r2
	lsls r3, r3, #7
	adds r3, r3, r1
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r3, [r3]
	movs r2, #176
	ldr r0, [r3]
	ldr r3, [r3, #4]
	lsls r2, r2, #1
	str r3, [sp, #4]
	adds r6, r7, r2
	ldr r3, [r6]
	movs r4, #1
	movs r1, #0
	negs r4, r4
	mov r11, r0
	mov r8, r1
	cmp r3, r4
	bne .L_081230c2
	ldr r0, [sp, #8]
	ldr r3, [r0, #84]
	str r3, [r6]
.L_081230c2:
	movs r5, #128
	lsls r5, r5, #19
	adds r5, #74
	bl Func_08038118
	adds r0, r5, #0
	movs r1, #4
	bl Func_08013d0c
	adds r0, r5, #0
	movs r1, #16
	bl Func_08013c58
	movs r3, #160
	mov r1, r9
	lsls r3, r3, #8
	mov r2, r8
	str r3, [r1, #4]
	str r2, [r1, #8]
	mov r1, r10
	ldr r0, [r6]
	bl Resource_GetBuffer
	ldr r3, .L_08123128
	mov r4, r9
	ands r0, r3
	mov r3, r9
	ldrh r2, [r3, #8]
	ldr r3, .L_08123138
	mov r1, r11
	ands r3, r2
	orrs r3, r0
	strh r3, [r4, #8]
	ldr r4, [sp, #4]
	movs r0, #12
	ldrsh r2, [r1, r0]
	ldrh r3, [r4, #4]
	lsls r2, r2, #3
	lsrs r3, r3, #8
	adds r2, r2, r3
	adds r2, #4
	ldr r3, .L_0812312c
	mov r8, r2
	mov r0, r8
	mov r1, r9
	ands r0, r3
	ldrh r2, [r1, #6]
	ldr r3, .L_0812313c
	ands r3, r2
	b .L_08123140
	.2byte 0x0000
.L_08123128:
	.4byte 0x000003ff
.L_0812312c:
	.4byte 0x000001ff
.L_08123130:
	.4byte Data_0300122c
.L_08123134:
	.4byte Data_0812996c
.L_08123138:
	.4byte 0xfffffc00
.L_0812313c:
	.4byte 0xfffffe00
.L_08123140:
	orrs r3, r0
	mov r2, r9
	strh r3, [r2, #6]
	ldr r3, .L_08123478
	ldr r0, [r3]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_0812315c
	movs r4, #254
	lsls r4, r4, #7
	adds r4, #255
	adds r0, r0, r4
.L_0812315c:
	mov r1, r11
	asrs r2, r0, #15
	movs r0, #14
	ldrsh r3, [r1, r0]
	ldr r4, [sp, #4]
	lsls r3, r3, #3
	adds r2, r2, r3
	ldrh r3, [r4, #6]
	ldr r1, .L_0812347c
	lsrs r3, r3, #8
	adds r3, r3, r2
	adds r3, #6
	mov r0, r9
	strb r3, [r0, #4]
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_081231aa
	ldr r3, [r1, #28]
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	ands r3, r0
	cmp r3, #0
	bne .L_081231aa
	ldr r3, .L_08123480
	movs r4, #168
	lsls r4, r4, #1
	adds r2, r7, r4
	ldr r3, [r3]
	ldr r2, [r2]
	subs r3, r3, r2
	cmp r3, #10
	bls .L_081231c6
	ldr r3, [r1]
	ands r3, r0
	cmp r3, #0
	beq .L_081231c6
.L_081231aa:
	movs r0, #111
	bl Audio_PlayCue
	movs r0, #164
	lsls r0, r0, #1
	movs r1, #168
	adds r2, r7, r0
	movs r3, #2
	lsls r1, r1, #1
	str r3, [r2]
	adds r2, r7, r1
.L_081231c0:
	movs r3, #0
	str r3, [r2]
	b .L_08122d40
.L_081231c6:
	mov r0, r9
	movs r1, #240
	bl Func_08014128
	b .L_08123494
.L_081231d0:
	cmp r3, #10
	bne .L_08123276
	ldr r3, .L_08123484
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_081231ea
	movs r2, #168
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r2, [r3]
	movs r1, #1
	orrs r2, r1
	str r2, [r3]
.L_081231ea:
	movs r4, #168
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08123254
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0812322a
	add r0, sp, #28
	movs r2, #178
	lsls r2, r2, #1
	mov r9, r0
	mov r1, r9
	adds r5, r7, r2
	movs r3, #255
	strh r3, [r1]
	ldr r0, [r5]
	bl GetBattleObjectSlot
	adds r6, r0, #0
	ldr r0, [r5]
	bl Func_0811a484
	adds r1, r0, #0
	ldr r0, [r6]
	bl Func_0811f030
	b .L_0812324e
.L_0812322a:
	movs r3, #28
	movs r4, #178
	add r3, sp
	lsls r4, r4, #1
	mov r9, r3
	adds r3, r7, r4
	ldr r0, [r3]
	mov r1, r9
	movs r3, #255
	mov r2, r9
	strh r0, [r1]
	strh r3, [r2, #2]
	bl GetBattleObjectSlot
	movs r1, #7
	ldr r0, [r0]
	bl Func_0811f030
.L_0812324e:
	mov r0, r9
	bl Func_080382a0
.L_08123254:
	movs r3, #168
	lsls r3, r3, #1
	adds r1, r7, r3
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	cmp r3, #7
	bgt .L_08123266
	b .L_08123494
.L_08123266:
	movs r4, #164
	lsls r4, r4, #1
	adds r3, r7, r4
	movs r2, #2
	str r2, [r3]
	movs r3, #0
	str r3, [r1]
	b .L_08122d40
.L_08123276:
	cmp r3, #11
	beq .L_0812327c
	b .L_08122d40
.L_0812327c:
	movs r0, #168
	lsls r0, r0, #1
	adds r5, r7, r0
	ldr r3, [r5]
	cmp r3, #0
	beq .L_08123290
	movs r1, #128
	lsls r1, r1, #3
	cmp r3, r1
	blt .L_0812338c
.L_08123290:
	movs r2, #6
	mov r10, r2
	cmp r3, #0
	bne .L_081232da
	movs r4, #182
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r3, [r3]
	cmp r3, #0
	beq .L_081232da
	movs r0, #178
	lsls r0, r0, #1
	adds r3, r7, r0
	ldr r0, [r3]
	bl Owner_GetState
	movs r1, #165
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrh r0, [r3]
	bl Func_081280a0
	cmp r0, #0
	blt .L_081232ce
	subs r0, #1
	cmp r0, #0
	bge .L_081232c8
	movs r0, #0
.L_081232c8:
	adds r0, #146
	bl Audio_PlayCue
.L_081232ce:
	movs r3, #168
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r2]
.L_081232da:
	movs r4, #168
	lsls r4, r4, #1
	adds r2, r7, r4
	movs r0, #128
	ldr r3, [r2]
	lsls r0, r0, #3
	adds r0, #29
	cmp r3, r0
	ble .L_081232f0
	movs r3, #0
	str r3, [r2]
.L_081232f0:
	cmp r3, #0
	bne .L_08123316
	movs r1, #178
	lsls r1, r1, #1
	adds r3, r7, r1
	ldr r0, [r3]
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	bl Func_081280a0
	cmp r0, #0
	blt .L_08123316
	adds r0, #146
	bl Audio_PlayCue
.L_08123316:
	movs r4, #168
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r3, [r3]
	movs r0, #128
	lsls r0, r0, #3
	cmp r3, r0
	blt .L_0812333e
	ldr r1, .L_08123488
	adds r0, r3, r1
	cmp r0, #0
	bge .L_08123332
	ldr r2, .L_0812348c
	adds r0, r3, r2
.L_08123332:
	asrs r0, r0, #3
	movs r1, #5
	bl Math_Mod
	adds r0, #1
	mov r10, r0
.L_0812333e:
	mov r3, r10
	cmp r3, #6
	beq .L_08123356
	movs r4, #168
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r3, [r3]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	beq .L_08123356
	b .L_08123468
.L_08123356:
	movs r0, #255
	movs r6, #0
	add r5, sp, #12
	mov r8, r0
	b .L_08123372
.L_08123360:
	ldr r2, [r0, #40]
	mov r4, r8
	ldrb r3, [r2, #22]
	mov r1, r10
	orrs r3, r4
	stmia r5!, {r0}
	strb r1, [r2, #5]
	strb r3, [r2, #22]
	adds r6, #1
.L_08123372:
	movs r0, #178
	lsls r0, r0, #1
	adds r3, r7, r0
	ldr r0, [r3]
	bl GetBattleObjectSlot
	adds r1, r6, #0
	ldr r0, [r0]
	bl GetMotionRecord
	cmp r0, #0
	bne .L_08123360
	b .L_08123468
.L_0812338c:
	cmp r3, #4
	bne .L_081233a0
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r0, [r3]
	bl Func_0811f3b8
	ldr r3, [r5]
	b .L_08123490
.L_081233a0:
	cmp r3, #4
	ble .L_08123490
	movs r4, #178
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r0, [r3]
	bl GetBattleObjectSlot
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #42
	movs r3, #1
	strb r3, [r2]
	add r5, sp, #12
	movs r2, #0
	b .L_081233c4
.L_081233c0:
	stmia r5!, {r0}
	adds r2, #1
.L_081233c4:
	adds r1, r2, #0
	ldr r0, [r6]
	str r2, [sp, #0]
	bl GetMotionRecord
	ldr r2, [sp, #0]
	cmp r0, #0
	bne .L_081233c0
	movs r0, #168
	lsls r0, r0, #1
	adds r3, r7, r0
	ldr r3, [r3]
	movs r1, #20
	lsls r3, r3, #2
	negs r1, r1
	adds r1, r1, r3
	mov r8, r1
	cmp r1, #127
	ble .L_08123426
	cmp r2, #0
	ble .L_08123404
	add r6, sp, #12
	adds r5, r2, #0
.L_081233f2:
	add r2, sp, #32
	ldmia r6!, {r0}
	mov r9, r2
	movs r1, #0
	subs r5, #1
	bl Func_08122cd0
	cmp r5, #0
	bne .L_081233f2
.L_08123404:
	movs r4, #178
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r0, [r3]
	bl Func_0811bc64
	movs r0, #164
	lsls r0, r0, #1
	movs r1, #168
	adds r2, r7, r0
	movs r3, #2
	lsls r1, r1, #1
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #0
	str r3, [r2]
	b .L_08123494
.L_08123426:
	cmp r2, #0
	ble .L_08123468
	movs r4, #19
	movs r0, #18
	negs r4, r4
	negs r0, r0
	adds r4, r4, r3
	adds r0, r0, r3
	subs r3, #17
	mov r11, r4
	mov r9, r0
	mov r10, r3
	adds r6, r2, #0
	add r5, sp, #12
.L_08123442:
	ldr r0, [r5]
	mov r1, r8
	bl Object_SetPositionAndResetMotionFar + 0x8
	ldr r0, [r5]
	mov r1, r11
	bl Object_SetPositionAndResetMotionFar + 0x8
	ldr r0, [r5]
	mov r1, r9
	bl Object_SetPositionAndResetMotionFar + 0x8
	subs r6, #1
	ldmia r5!, {r0}
	mov r1, r10
	bl Object_SetPositionAndResetMotionFar + 0x8
	cmp r6, #0
	bne .L_08123442
.L_08123468:
	movs r1, #168
	lsls r1, r1, #1
	adds r2, r7, r1
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_08123494
	.2byte 0x0000
.L_08123478:
	.4byte Data_0300122c
.L_0812347c:
	.4byte gInput
.L_08123480:
	.4byte gFrameTick
.L_08123484:
	.4byte Data_030011d8
.L_08123488:
	.4byte 0xfffffc00
.L_0812348c:
	.4byte 0xfffffc07
.L_08123490:
	adds r3, #1
	str r3, [r5]
.L_08123494:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
