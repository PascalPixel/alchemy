.syntax unified
	.thumb
	.global BattleEffect_RunEmberColumns
	.thumb_func
BattleEffect_RunEmberColumns:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080d2e0c
	ldr r1, [r3]
	sub sp, #60
	str r1, [sp, #48]
	subs r2, r3, #4
	ldr r2, [r2]
	str r2, [sp, #44]
	ldr r7, .L_080d2e10
	ldr r3, [r3, #4]
	str r3, [sp, #40]
	adds r3, r2, r7
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d2e14
	mov r0, sp
	ldr r3, .L_080d2e08
	adds r0, #52
	strh r3, [r2]
	str r0, [sp, #36]
	ldr r1, [sp, #36]
	movs r0, #0
	bl BattleFx_FetchRectangleBlitters
	ldr r0, .L_080d2e18
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080d2e1c
	movs r2, #128
	adds r1, r5, #0
	lsls r0, r0, #19
	bl _call_via_r3
	adds r5, #128
	ldr r1, [sp, #44]
	adds r0, r5, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d2e20
	bl Resource_GetTableEntry
	ldr r3, .L_080d2e24
	ldr r2, [sp, #44]
	adds r5, r0, #0
	b .L_080d2e28
	.2byte 0x0000
.L_080d2e08:
	.4byte 0x00001010
.L_080d2e0c:
	.4byte Data_03001ef0
.L_080d2e10:
	.4byte 0x00007828
.L_080d2e14:
	.4byte 0x04000052
.L_080d2e18:
	.4byte 0x0000006e
.L_080d2e1c:
	.4byte IwramCopyWords
.L_080d2e20:
	.4byte 0x00000085
.L_080d2e24:
	.4byte 0x000006e4
.L_080d2e28:
	adds r5, #128
	adds r1, r2, r3
	adds r0, r5, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d31ac
	bl Resource_GetTableEntry
	ldr r1, [sp, #40]
	bl Resource_DecodeType01
	movs r0, #239
	ldr r7, [sp, #44]
	lsls r0, r0, #7
	ldr r1, .L_080d31b0
	adds r2, r7, r0
	movs r3, #2
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d31b4
	bl Scheduler_AddOrUpdateCallback
	movs r7, #176
	ldr r0, .L_080d31b8
	lsls r7, r7, #15
	ldr r1, .L_080d31bc
	movs r3, #128
	str r7, [sp, #32]
	str r0, [sp, #20]
	ldr r7, [sp, #44]
	ldr r0, .L_080d31c0
	movs r2, #0
	lsls r3, r3, #17
	str r1, [sp, #24]
	str r2, [sp, #16]
	str r3, [sp, #28]
	mov r8, r2
	adds r3, r7, r0
	subs r2, #1
.L_080d2e7e:
	movs r1, #1
	add r8, r1
	mov r7, r8
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_080d2e7e
	ldr r1, [sp, #44]
	ldr r2, .L_080d31c4
	movs r0, #0
	mov r8, r0
	adds r5, r1, r2
.L_080d2e96:
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
	mov r7, r8
	adds r5, #28
	cmp r7, #16
	bne .L_080d2e96
	movs r0, #0
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080d31c8
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #3
.L_080d2ed2:
	movs r7, #1
	add r8, r7
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080d2ed2
	ldr r0, [sp, #44]
	ldr r1, .L_080d31cc
	adds r3, r0, r1
	ldr r0, [r3]
	bl BattleFx_SelectLivingTargets
	movs r0, #1
	bl WaitFrames
	movs r1, #190
	movs r2, #2
	lsls r1, r1, #1
	movs r0, #12
	bl BattleFx_SpawnObjects
	movs r2, #0
	mov r9, r2
.L_080d2f00:
	ldr r3, .L_080d31d0
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080d2f1a
	mov r3, r9
	cmp r3, #32
	ble .L_080d2f1a
	cmp r3, #97
	bgt .L_080d2f1a
	movs r7, #98
	mov r9, r7
.L_080d2f1a:
	mov r0, r9
	cmp r0, #120
	bne .L_080d2f26
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080d2f26:
	mov r1, r9
	cmp r1, #15
	bgt .L_080d2f32
	ldr r2, [sp, #16]
	adds r2, #2
	str r2, [sp, #16]
.L_080d2f32:
	mov r3, r9
	cmp r3, #99
	bgt .L_080d2f7c
	ldr r0, [sp, #28]
	ldr r7, [sp, #20]
	adds r7, r7, r0
	str r7, [sp, #28]
	ldr r7, [sp, #20]
	movs r3, #58
	muls r3, r7
	ldr r2, [sp, #32]
	ldr r1, [sp, #24]
	adds r1, r1, r2
	str r1, [sp, #32]
	cmp r3, #0
	bge .L_080d2f54
	adds r3, #63
.L_080d2f54:
	ldr r0, [sp, #24]
	asrs r3, r3, #6
	str r3, [sp, #20]
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080d2f66
	adds r3, #63
.L_080d2f66:
	ldr r1, [sp, #28]
	ldr r2, .L_080d31d4
	asrs r3, r3, #6
	str r3, [sp, #24]
	cmp r1, r2
	bgt .L_080d2f7c
	ldr r3, [sp, #20]
	movs r7, #128
	lsls r7, r7, #8
	adds r7, r3, r7
	str r7, [sp, #20]
.L_080d2f7c:
	movs r0, #1
	ldr r1, [sp, #28]
	ldr r2, [sp, #32]
	bl BattleFx_PlaceFormationObjects
	mov r0, r9
	cmp r0, #28
	bne .L_080d300c
	movs r1, #0
	movs r2, #63
	ldr r7, .L_080d31d8
	mov r8, r1
	mov r10, r2
.L_080d2f96:
	movs r0, #1
	ldr r3, [r7, #24]
	negs r0, r0
	cmp r3, r0
	bne .L_080d2ffe
	bl Random16
	mov r1, r10
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	ldr r3, .L_080d31dc
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
	bl Func_0800231c
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
.L_080d2ffe:
	movs r3, #1
	movs r0, #128
	add r8, r3
	lsls r0, r0, #1
	adds r7, #28
	cmp r8, r0
	bne .L_080d2f96
.L_080d300c:
	mov r1, r9
	subs r1, #32
	str r1, [sp, #12]
	cmp r1, #47
	bhi .L_080d30a2
	movs r2, #0
	movs r3, #63
	ldr r7, .L_080d31d8
	mov r11, r2
	mov r8, r2
	mov r10, r3
.L_080d3022:
	movs r0, #1
	ldr r3, [r7, #24]
	negs r0, r0
	cmp r3, r0
	bne .L_080d3094
	bl Random16
	mov r1, r10
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	ldr r3, .L_080d31dc
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
	bl Func_0800231c
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
	movs r3, #0
	negs r0, r0
	subs r0, #8
	str r3, [r7, #24]
	movs r3, #1
	lsls r0, r0, #13
	add r11, r3
	str r0, [r7, #16]
	mov r0, r11
	cmp r0, #16
	beq .L_080d30a2
.L_080d3094:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r7, #28
	cmp r8, r2
	bne .L_080d3022
.L_080d30a2:
	mov r3, r9
	cmp r3, #0
	bne .L_080d30ae
	movs r0, #164
	bl Func_080f9010
.L_080d30ae:
	mov r7, r9
	cmp r7, #32
	bne .L_080d30ba
	movs r0, #145
	bl Func_080f9010
.L_080d30ba:
	mov r0, r9
	cmp r0, #80
	bne .L_080d30c6
	movs r0, #144
	bl Func_080f9010
.L_080d30c6:
	ldr r1, [sp, #12]
	cmp r1, #47
	bhi .L_080d3132
	ldr r0, [sp, #44]
	ldr r1, .L_080d31e0
	adds r0, r0, r1
	movs r2, #0
	mov r7, r9
	mov r10, r0
	ldr r0, .L_080d31e4
	mov r8, r2
	lsls r3, r7, #4
	movs r2, #34
	ldr r6, .L_080d31e8
	mov r11, r2
	adds r7, r3, r0
.L_080d30e6:
	adds r0, r7, #0
	movs r1, #104
	bl Func_080022fc
	ldrb r3, [r6, #1]
	ldrb r2, [r6]
	adds r5, r0, #0
	mov r1, r11
	movs r0, #104
	subs r3, r3, r5
	str r1, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #52]
	subs r2, #17
	subs r3, #104
	ldr r0, [sp, #48]
	mov r1, r10
	bl _call_via_r4
	ldrb r2, [r6]
	ldrb r3, [r6, #1]
	mov r1, r11
	subs r2, #17
	subs r3, r3, r5
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #48]
	mov r1, r10
	bl _call_via_r4
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r6, #2
	adds r7, #25
	cmp r3, #3
	bne .L_080d30e6
.L_080d3132:
	mov r7, r9
	cmp r7, #95
	bgt .L_080d3172
	movs r0, #0
	mov r8, r0
	movs r5, #32
	movs r6, #120
.L_080d3140:
	mov r2, r8
	lsls r1, r2, #5
	mov r2, r9
	cmp r2, #0
	bge .L_080d314c
	adds r2, #3
.L_080d314c:
	movs r3, #31
	asrs r2, r2, #2
	ands r2, r3
	ldr r7, [sp, #16]
	adds r2, r1, r2
	subs r2, #32
	ldr r1, [sp, #44]
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #48]
	subs r3, r6, r7
	bl _call_via_r4
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #5
	bne .L_080d3140
.L_080d3172:
	movs r2, #0
	ldr r5, .L_080d31d8
	mov r8, r2
.L_080d3178:
	ldr r3, [r5, #24]
	cmp r3, #0
	bge .L_080d3180
	b .L_080d32a8
.L_080d3180:
	mov r0, r8
	movs r1, #3
	bl Func_080022fc
	ldr r3, [r5, #16]
	adds r4, r0, #2
	cmp r3, #0
	ble .L_080d3192
	adds r4, #2
.L_080d3192:
	mov r7, r9
	cmp r7, #68
	ble .L_080d319e
	cmp r4, #5
	bgt .L_080d319e
	movs r4, #6
.L_080d319e:
	mov r0, r9
	cmp r0, #70
	ble .L_080d31ec
	cmp r4, #6
	bgt .L_080d31ec
	movs r4, #7
	b .L_080d31ec
.L_080d31ac:
	.4byte 0x00000073
.L_080d31b0:
	.4byte 0x00007784
.L_080d31b4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d31b8:
	.4byte 0xfff00000
.L_080d31bc:
	.4byte 0xfffc0000
.L_080d31c0:
	.4byte 0x00007098
.L_080d31c4:
	.4byte 0x00007320
.L_080d31c8:
	.4byte Data_02010018
.L_080d31cc:
	.4byte 0x00007828
.L_080d31d0:
	.4byte gKeysRepeat
.L_080d31d4:
	.4byte 0x0077ffff
.L_080d31d8:
	.4byte gMapCellBuffer
.L_080d31dc:
	.4byte 0x0000ffff
.L_080d31e0:
	.4byte 0x000006e4
.L_080d31e4:
	.4byte 0xffffff00
.L_080d31e8:
	.4byte Data_080ee1ac
.L_080d31ec:
	mov r1, r9
	cmp r1, #72
	ble .L_080d31f8
	cmp r4, #7
	bgt .L_080d31f8
	movs r4, #8
.L_080d31f8:
	mov r2, r9
	cmp r2, #74
	ble .L_080d3204
	cmp r4, #8
	bgt .L_080d3204
	movs r4, #9
.L_080d3204:
	mov r7, r9
	cmp r7, #76
	ble .L_080d320c
	movs r4, #10
.L_080d320c:
	movs r6, #4
	cmp r3, #0
	bgt .L_080d3214
	movs r6, #0
.L_080d3214:
	lsls r0, r4, #1
	ldr r2, .L_080d33a4
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #40]
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
	ldr r0, [sp, #36]
	subs r3, r3, r4
	ldr r4, [r6, r0]
	ldr r0, [sp, #48]
	bl _call_via_r4
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r1, [r5, #16]
	ldr r3, [r5, #4]
	mov r2, r9
	adds r3, r3, r1
	str r3, [r5, #4]
	cmp r2, #80
	ble .L_080d325c
	ldr r7, .L_080d33a8
	adds r3, r1, r7
	b .L_080d326a
.L_080d325c:
	movs r2, #3
	mov r0, r8
	ldr r3, .L_080d33ac
	ands r2, r0
	lsls r2, r2, #2
	ldr r3, [r3, r2]
	adds r3, r1, r3
.L_080d326a:
	str r3, [r5, #16]
	ldr r2, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_080d327a
	adds r3, #63
.L_080d327a:
	ldr r2, [r5, #16]
	asrs r3, r3, #6
	str r3, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r2, r3, #1
	cmp r2, #0
	bge .L_080d328c
	adds r2, #63
.L_080d328c:
	ldr r3, [r5, #24]
	asrs r2, r2, #6
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r2, #0
	ble .L_080d32a8
	movs r1, #6
	ldrsh r3, [r5, r1]
	cmp r3, #104
	ble .L_080d32a8
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080d32a8:
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #3
	adds r5, #28
	cmp r8, r3
	beq .L_080d32b8
	b .L_080d3178
.L_080d32b8:
	mov r7, r9
	cmp r7, #79
	bgt .L_080d333c
	ldr r3, .L_080d33b0
	ldr r1, [sp, #44]
	adds r2, r1, r3
	ldr r3, [r2]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	beq .L_080d333c
	adds r7, r2, #0
	movs r4, #36
.L_080d32d4:
	mov r0, r9
	cmp r0, #29
	ble .L_080d332e
	movs r1, #12
	str r4, [sp, #8]
	bl Func_080022fc
	adds r6, r0, #0
	ldr r4, [sp, #8]
	cmp r6, #0
	bne .L_080d3314
	ldr r3, [r7]
	ldrsh r0, [r3, r4]
	bl GetBattleObjectSlotFar
	ldr r3, [r7]
	ldr r4, [sp, #8]
	ldr r5, [r0]
	ldrsh r0, [r3, r4]
	movs r3, #1
	negs r3, r3
	movs r1, #7
	movs r2, #5
	str r6, [sp, #0]
	bl ObjectGroup_UpdateMembers
	movs r3, #144
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r3, .L_080d33b4
	ldr r4, [sp, #8]
	str r3, [r5, #72]
.L_080d3314:
	cmp r6, #6
	bne .L_080d332e
	ldr r3, [r7]
	ldrsh r0, [r3, r4]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #5
	subs r3, #1
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r4, [sp, #8]
.L_080d332e:
	ldr r3, [r7]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r4, #2
	cmp r8, r3
	bne .L_080d32d4
.L_080d333c:
	ldr r3, [sp, #44]
	ldr r7, .L_080d33b8
	adds r2, r3, r7
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r9, r0
	mov r1, r9
	cmp r1, #124
	beq .L_080d3358
	b .L_080d2f00
.L_080d3358:
	ldr r0, .L_080d33bc
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #32]
	movs r0, #1
	ldr r1, [sp, #28]
	bl BattleEffect_RunImpactBurst
	ldr r3, [sp, #44]
	movs r2, #0
	subs r7, #76
	mov r8, r2
	adds r5, r3, r7
.L_080d337e:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #12
	bne .L_080d337e
	bl BattleFx_EndCanvasLayer
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080d33a4:
	.4byte ParticleStreams_CellOffsets
.L_080d33a8:
	.4byte 0xffff8000
.L_080d33ac:
	.4byte Data_080ee1b4
.L_080d33b0:
	.4byte 0x00007828
.L_080d33b4:
	.4byte 0x0000ab85
.L_080d33b8:
	.4byte 0x00007824
.L_080d33bc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
