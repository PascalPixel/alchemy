.syntax unified
	.thumb
	.global BattleEffect_RunRisingMotes
BattleEffect_RunRisingMotes:
	.global Unnamed_080dab74
	.thumb_func
Unnamed_080dab74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080dabe0
	adds r3, r2, #0
	ldmia r3!, {r1}
	sub sp, #96
	str r1, [sp, #48]
	ldr r3, [r3]
	str r3, [sp, #44]
	ldr r2, [r2, #8]
	str r2, [sp, #36]
	ldr r2, .L_080dabe4
	adds r3, r1, r2
	str r0, [r3]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080dabe8
	ldr r3, .L_080dabdc
	strh r3, [r2]
	ldr r1, [sp, #48]
	ldr r0, .L_080dabec
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_080dabf0
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	mov r3, sp
	adds r3, #52
	movs r0, #0
	adds r1, r3, #0
	str r3, [sp, #32]
	bl BattleFx_FetchRectangleBlitters
	movs r1, #225
	ldr r0, [sp, #48]
	movs r6, #0
	lsls r1, r1, #7
	mov r10, r6
	adds r5, r0, r1
	b .L_080dabf4
	.2byte 0x0000
.L_080dabdc:
	.4byte 0x00000100
.L_080dabe0:
	.4byte gBattleFxWork
.L_080dabe4:
	.4byte 0x00007828
.L_080dabe8:
	.4byte 0x04000020
.L_080dabec:
	.4byte 0x000000b8
.L_080dabf0:
	.4byte 0x000000ba
.L_080dabf4:
	bl Random16
	ldr r3, .L_080daf24
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #56
	str r3, [r5, #8]
	bl Random16
	movs r3, #31
	ands r3, r0
	subs r3, #64
	movs r2, #1
	lsls r3, r3, #16
	add r10, r2
	str r3, [r5, #4]
	mov r3, r10
	adds r5, #28
	cmp r3, #64
	bne .L_080dabf4
	ldr r6, [sp, #48]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, .L_080daf28
	adds r2, r6, r0
	movs r3, #2
	str r3, [r2]
	adds r2, r6, r1
	movs r3, #50
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080daf2c
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080daf30
	adds r3, r6, r2
	ldr r1, [r3]
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_080dac54
	ldr r2, .L_080daf34
	ldr r3, .L_080daf38
	str r3, [r2]
.L_080dac54:
	movs r3, #0
	str r3, [sp, #40]
	ldr r3, [r1, #24]
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, .L_080daf3c
	adds r2, #2
	movs r6, #75
	ldrb r3, [r0, r2]
	negs r6, r6
	cmp r3, r6
	bne .L_080dac6e
	b .L_080db214
.L_080dac6e:
	ldr r3, .L_080daf30
	ldr r2, [sp, #48]
	mov r1, sp
	adds r1, #60
	adds r3, r2, r3
	str r1, [sp, #8]
	str r3, [sp, #16]
.L_080dac7c:
	movs r6, #240
	lsls r6, r6, #15
	movs r1, #0
	str r6, [sp, #24]
	str r1, [sp, #20]
	ldr r2, [sp, #16]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r3, [r0, r3]
	ldr r6, [sp, #40]
	adds r3, #11
	cmp r6, r3
	bne .L_080daca2
	movs r0, #132
	bl BattleEventRuntime_BeginPhaseFar
.L_080daca2:
	ldr r0, [sp, #20]
	ldr r1, [sp, #8]
	movs r3, #128
	lsls r3, r3, #18
	str r0, [r1]
	str r0, [r1, #4]
	str r3, [r1, #8]
	bl Render_ResetTransformState
	ldr r0, [sp, #8]
	bl SceneTransform_ApplyPosition
	ldr r2, [sp, #40]
	subs r2, #36
	str r2, [sp, #12]
	cmp r2, #27
	bhi .L_080dacd4
	ldr r6, [sp, #40]
	movs r3, #3
	ands r3, r6
	cmp r3, #0
	bne .L_080dacd4
	movs r0, #115
	bl AudioCommand_PlayFar
.L_080dacd4:
	ldr r0, [sp, #40]
	cmp r0, #85
	bne .L_080dace0
	movs r0, #136
	bl AudioCommand_PlayFar
.L_080dace0:
	ldr r2, .L_080daf30
	ldr r6, [sp, #48]
	ldr r3, [r6, r2]
	ldr r3, [r3, #20]
	movs r1, #0
	mov r10, r1
	cmp r3, #0
	beq .L_080dad22
	movs r6, #36
	movs r5, #40
.L_080dacf4:
	ldr r0, [sp, #40]
	cmp r0, r5
	bne .L_080dad0e
	ldr r1, [sp, #48]
	ldr r3, [r1, r2]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #9
	movs r2, #5
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080dad0e:
	ldr r2, .L_080daf30
	ldr r0, [sp, #48]
	movs r3, #1
	add r10, r3
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	adds r6, #2
	adds r5, #4
	cmp r10, r3
	bne .L_080dacf4
.L_080dad22:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	ldr r1, .L_080daf3c
	adds r2, r3, r2
	adds r3, r2, #2
	ldrb r3, [r1, r3]
	ldr r0, [sp, #40]
	movs r6, #16
	mov r9, r1
	str r6, [sp, #28]
	cmp r0, r3
	bge .L_080dad42
	ldrb r2, [r1, r2]
	str r2, [sp, #28]
.L_080dad42:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r3, [r1, r3]
	ldr r6, [sp, #40]
	adds r3, #35
	cmp r6, r3
	blt .L_080dad5a
	b .L_080dae90
.L_080dad5a:
	ldr r1, [sp, #28]
	movs r0, #0
	mov r10, r0
	cmp r1, #0
	bne .L_080dad66
	b .L_080dae90
.L_080dad66:
	ldr r6, [sp, #48]
	movs r0, #225
	movs r2, #84
	movs r3, #72
	lsls r0, r0, #7
	add r2, sp
	add r3, sp
	adds r6, r6, r0
	mov r11, r2
	mov r9, r3
	mov r8, r6
.L_080dad7c:
	ldr r1, [sp, #40]
	cmp r1, r10
	ble .L_080dae7c
	mov r1, r10
	cmp r1, #0
	bge .L_080dad8a
	adds r1, #7
.L_080dad8a:
	asrs r7, r1, #3
	mov r2, r10
	lsls r3, r7, #3
	subs r7, r2, r3
	lsrs r3, r2, #31
	add r3, r10
	asrs r3, r3, #1
	mov r6, r8
	movs r2, #48
	subs r2, r2, r3
	ldr r3, [r6, #4]
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_080dae10
	ldr r0, .L_080daf40
	cmp r3, r0
	ble .L_080dae10
	ldr r0, [r6]
	bl Trig_Sin
	ldr r3, [r6, #8]
	muls r3, r0
	mov r1, r11
	str r3, [r1]
	ldr r3, [r6, #4]
	str r3, [r1, #4]
	ldr r0, [r6]
	bl Trig_Cos
	ldr r3, [r6, #8]
	muls r3, r0
	mov r2, r11
	str r3, [r2, #8]
	mov r1, r9
	mov r0, r11
	bl EffectPosition_ApplyBaseAndYOffset
	mov r3, r9
	ldr r2, [r3]
	asrs r2, r2, #17
	adds r2, #64
	str r2, [r3]
	movs r0, #6
	ldrsh r3, [r3, r0]
	mov r1, r9
	adds r3, #60
	ldr r0, .L_080daf44
	str r3, [r1, #4]
	lsls r1, r7, #1
	ldrh r1, [r0, r1]
	ldr r0, [sp, #48]
	adds r1, r0, r1
	ldr r0, .L_080daf48
	ldrb r5, [r0, r7]
	lsrs r0, r5, #1
	subs r2, r2, r0
	ldr r0, .L_080daf4c
	ldrb r4, [r0, r7]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r5, [sp, #0]
	ldr r0, [sp, #32]
	str r4, [sp, #4]
	ldr r4, [r0, #4]
	ldr r0, [sp, #44]
	bl _call_via_r4
.L_080dae10:
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_080daf3c
	adds r3, #2
	ldrb r3, [r2, r3]
	ldr r0, [sp, #40]
	cmp r0, r3
	bge .L_080dae54
	mov r3, r10
	adds r3, #16
	cmp r0, r3
	ble .L_080dae7c
	ldr r3, [r6, #8]
	cmp r3, #4
	ble .L_080dae38
	subs r3, #2
	str r3, [r6, #8]
.L_080dae38:
	ldr r3, [r6, #4]
	ldr r1, .L_080daf50
	cmp r3, r1
	bgt .L_080dae48
	movs r2, #160
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #4]
.L_080dae48:
	ldr r3, [r6]
	movs r0, #128
	lsls r0, r0, #2
	adds r3, r3, r0
	str r3, [r6]
	b .L_080dae7c
.L_080dae54:
	ldr r3, [r6, #8]
	adds r3, #8
	str r3, [r6, #8]
	movs r1, #5
	mov r0, r10
	bl __modsi3
	ldr r3, [r6, #4]
	adds r0, #2
	lsls r0, r0, #16
	subs r3, r3, r0
	str r3, [r6, #4]
	ldr r1, [sp, #24]
	cmp r1, r3
	ble .L_080dae74
	str r3, [sp, #24]
.L_080dae74:
	ldr r2, [sp, #20]
	cmp r2, r3
	bge .L_080dae7c
	str r3, [sp, #20]
.L_080dae7c:
	movs r6, #1
	ldr r0, [sp, #28]
	movs r3, #28
	add r10, r6
	add r8, r3
	cmp r10, r0
	beq .L_080dae8c
	b .L_080dad7c
.L_080dae8c:
	ldr r1, .L_080daf3c
	mov r9, r1
.L_080dae90:
	ldr r2, [sp, #24]
	ldr r6, [sp, #20]
	movs r3, #128
	lsls r3, r3, #15
	adds r2, r2, r3
	adds r6, r6, r3
	str r6, [sp, #20]
	str r2, [sp, #24]
	ldr r0, [sp, #16]
	ldr r3, [r0]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r2, r3, r2
	adds r3, r2, #2
	mov r1, r9
	ldrb r3, [r1, r3]
	ldr r6, [sp, #40]
	cmp r6, r3
	bge .L_080dafa0
	adds r3, r2, #1
	ldrb r3, [r1, r3]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq .L_080dafa0
	ldr r5, .L_080daf54
	ldr r7, .L_080daf58
	mov r8, r5
	movs r6, #0
.L_080daeca:
	ldr r0, [sp, #12]
	movs r1, #3
	bl __divsi3
	cmp r10, r0
	bge .L_080daf84
	movs r1, #3
	mov r0, r10
	bl __modsi3
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	mov r2, r9
	ldrb r3, [r2, r3]
	adds r4, r0, #0
	ldr r0, [sp, #40]
	subs r3, #7
	cmp r0, r3
	blt .L_080daf60
	ldr r0, .L_080daf5c
	lsls r3, r4, #1
	ldrh r1, [r7, r3]
	ldrb r4, [r0, r4]
	ldr r2, [sp, #48]
	mov r3, r8
	movs r0, #32
	adds r1, r2, r1
	ldrb r2, [r6, r3]
	ldrb r3, [r5, #1]
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #32]
	subs r3, r3, r4
	ldr r4, [r0, #4]
	ldr r0, [sp, #44]
	bl _call_via_r4
	ldr r1, .L_080daf3c
	mov r9, r1
	b .L_080daf84
	.2byte 0x0000
.L_080daf24:
	.4byte 0x0000ffff
.L_080daf28:
	.4byte 0x00007784
.L_080daf2c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080daf30:
	.4byte 0x00007828
.L_080daf34:
	.4byte 0x04000028
.L_080daf38:
	.4byte 0xffff9000
.L_080daf3c:
	.4byte Data_080eea88
.L_080daf40:
	.4byte 0xffd00000
.L_080daf44:
	.4byte Data_080eeaa2
.L_080daf48:
	.4byte Data_080eea91
.L_080daf4c:
	.4byte Data_080eea99
.L_080daf50:
	.4byte 0x002fffff
.L_080daf54:
	.4byte Data_080eea62
.L_080daf58:
	.4byte Data_080eeab2
.L_080daf5c:
	.4byte Data_080eeab8
.L_080daf60:
	ldr r0, .L_080db23c
	lsls r3, r4, #1
	ldrh r1, [r7, r3]
	ldrb r4, [r0, r4]
	ldrb r3, [r5, #1]
	ldr r2, [sp, #48]
	movs r0, #32
	subs r3, r3, r4
	adds r1, r2, r1
	ldrb r2, [r5]
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #52]
	bl _call_via_r4
	ldr r3, .L_080db240
	mov r9, r3
.L_080daf84:
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #1
	mov r2, r9
	movs r0, #1
	ldrb r3, [r2, r3]
	add r10, r0
	adds r5, #2
	adds r6, #2
	cmp r10, r3
	bne .L_080daeca
.L_080dafa0:
	ldr r6, [sp, #16]
	ldr r3, [r6]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	mov r0, r9
	ldrb r3, [r0, r3]
	ldr r1, [sp, #40]
	cmp r1, r3
	bne .L_080db00c
	movs r2, #0
	ldr r5, .L_080db244
	mov r10, r2
	movs r6, #15
.L_080dafbe:
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	ands r0, r6
	adds r0, #80
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #63
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #12
	str r3, [r5, #8]
	bl Random16
	negs r0, r0
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	movs r3, #1
	ands r0, r6
	adds r0, #16
	add r10, r3
	str r0, [r5, #24]
	mov r0, r10
	adds r5, #28
	cmp r0, #32
	bne .L_080dafbe
	ldr r1, .L_080db240
	mov r9, r1
.L_080db00c:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	mov r6, r9
	ldrb r3, [r6, r3]
	ldr r0, [sp, #40]
	cmp r0, r3
	blt .L_080db09e
	movs r1, #0
	ldr r5, .L_080db244
	mov r10, r1
.L_080db028:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080db07c
	mov r3, r10
	cmp r3, #0
	bge .L_080db036
	adds r3, #7
.L_080db036:
	asrs r4, r3, #3
	lsls r3, r4, #3
	mov r2, r10
	subs r4, r2, r3
	ldr r2, .L_080db248
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #48]
	adds r1, r3, r1
	movs r0, #6
	ldrsh r3, [r5, r0]
	ldr r0, .L_080db24c
	ldrb r0, [r0, r4]
	movs r6, #2
	ldrsh r2, [r5, r6]
	str r0, [sp, #0]
	ldr r0, .L_080db250
	ldrb r0, [r0, r4]
	ldr r6, [sp, #32]
	str r0, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [r6, #4]
	bl _call_via_r4
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080db07c:
	ldr r3, [r5, #4]
	ldr r0, [sp, #24]
	cmp r0, r3
	ble .L_080db086
	str r3, [sp, #24]
.L_080db086:
	ldr r1, [sp, #20]
	cmp r1, r3
	bge .L_080db08e
	str r3, [sp, #20]
.L_080db08e:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r5, #28
	cmp r3, #24
	bne .L_080db028
	ldr r6, .L_080db240
	mov r9, r6
.L_080db09e:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	asrs r0, r0, #16
	asrs r1, r1, #16
	str r0, [sp, #24]
	str r1, [sp, #20]
	cmp r1, r0
	bgt .L_080db0b2
	adds r0, #1
	str r0, [sp, #20]
.L_080db0b2:
	ldr r2, [sp, #16]
	ldr r3, [r2]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	mov r6, r9
	ldrb r3, [r6, r3]
	ldr r0, [sp, #40]
	cmp r0, r3
	bne .L_080db120
	ldr r2, [sp, #48]
	movs r3, #225
	movs r1, #0
	lsls r3, r3, #7
	mov r10, r1
	adds r5, r2, r3
.L_080db0d4:
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5, #12]
	ldr r6, [sp, #20]
	ldr r0, [sp, #24]
	cmp r6, r0
	bne .L_080db0ee
	lsls r3, r0, #16
	str r3, [r5, #16]
	b .L_080db104
.L_080db0ee:
	bl Random16
	ldr r2, [sp, #20]
	ldr r3, [sp, #24]
	subs r1, r2, r3
	bl __umodsi3
	ldr r6, [sp, #24]
	adds r0, r0, r6
	lsls r0, r0, #16
	str r0, [r5, #16]
.L_080db104:
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r0, #1
	add r10, r0
	adds r3, #20
	mov r1, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #32
	bne .L_080db0d4
	ldr r2, .L_080db240
	mov r9, r2
.L_080db120:
	ldr r6, [sp, #16]
	ldr r3, [r6]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	mov r0, r9
	ldrb r3, [r0, r3]
	ldr r1, [sp, #40]
	cmp r1, r3
	blt .L_080db1e4
	subs r3, r1, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r8, r3
	movs r6, #225
	ldr r3, [sp, #48]
	movs r2, #0
	lsls r6, r6, #7
	ldr r7, .L_080db240
	mov r10, r2
	adds r5, r3, r6
.L_080db14e:
	ldr r3, [r5, #24]
	cmp r3, #17
	bhi .L_080db18e
	movs r0, #17
	subs r0, r0, r3
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	ldr r2, .L_080db254
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #36]
	adds r1, r2, r1
	movs r3, #14
	ldrsh r2, [r5, r3]
	ldr r3, .L_080db258
	ldrb r4, [r3, r0]
	movs r6, #18
	ldrsh r3, [r5, r6]
	lsrs r0, r4, #1
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r6, [sp, #32]
	subs r2, r2, r0
	subs r3, r3, r0
	mov r0, r8
	subs r3, r3, r0
	ldr r4, [r6, #4]
	ldr r0, [sp, #44]
	bl _call_via_r4
	ldr r3, [r5, #24]
.L_080db18e:
	movs r0, #1
	subs r3, #1
	negs r0, r0
	str r3, [r5, #24]
	cmp r3, r0
	beq .L_080db19e
	cmp r3, #17
	bne .L_080db1d8
.L_080db19e:
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r3, [r7, r3]
	ldr r2, [sp, #40]
	adds r3, #35
	cmp r2, r3
	bge .L_080db1d8
	movs r3, #17
	str r3, [r5, #24]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5, #12]
	bl Random16
	ldr r6, [sp, #24]
	ldr r3, [sp, #20]
	subs r1, r3, r6
	bl __umodsi3
	adds r0, r0, r6
	lsls r0, r0, #16
	str r0, [r5, #16]
.L_080db1d8:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r5, #28
	cmp r1, #32
	bne .L_080db14e
.L_080db1e4:
	ldr r3, [sp, #48]
	ldr r6, .L_080db25c
	adds r2, r3, r6
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #40]
	adds r0, #1
	str r0, [sp, #40]
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r0, .L_080db240
	adds r3, #2
	ldrb r3, [r0, r3]
	ldr r2, [sp, #40]
	adds r3, #75
	cmp r2, r3
	beq .L_080db214
	b .L_080dac7c
.L_080db214:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080db260
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080db23c:
	.4byte Data_080eeab8
.L_080db240:
	.4byte Data_080eea88
.L_080db244:
	.4byte gMapCellBuffer
.L_080db248:
	.4byte Data_080eeacc
.L_080db24c:
	.4byte Data_080eeabb
.L_080db250:
	.4byte Data_080eeac3
.L_080db254:
	.4byte BattleFx_PuffCells
.L_080db258:
	.4byte BattleFx_PuffSizes
.L_080db25c:
	.4byte 0x00007824
.L_080db260:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
