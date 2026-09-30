.syntax unified
	.thumb
	.global BattleEffect_RunCirclingFallingScene
	.thumb_func
BattleEffect_RunCirclingFallingScene:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080e82b0
	ldr r1, [r3]
	sub sp, #84
	str r1, [sp, #48]
	subs r2, r3, #4
	ldr r2, [r2]
	str r2, [sp, #44]
	ldr r3, [r3, #4]
	str r3, [sp, #36]
	ldr r3, .L_080e82b4
	adds r5, r2, r3
	str r0, [r5]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080e82ac
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r6, [sp, #44]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r6, r7
	movs r3, #0
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080e82b8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #1
	bl BattleEffect_WipeCanvas
	ldr r0, [r5]
	bl BattleFx_SelectLivingTargets
	ldr r1, .L_080e82bc
	movs r0, #9
	movs r2, #2
	bl BattleFx_SpawnObjects
	movs r1, #13
	b .L_080e82c0
	.2byte 0x0000
.L_080e82ac:
	.4byte 0x00000000
.L_080e82b0:
	.4byte Data_03001ef0
.L_080e82b4:
	.4byte 0x00007828
.L_080e82b8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e82bc:
	.4byte 0x0000017b
.L_080e82c0:
	movs r0, #0
	negs r1, r1
	ldr r6, .L_080e838c
	mov r8, r0
	adds r7, r1, #0
.L_080e82ca:
	movs r0, #195
	lsls r0, r0, #1
	bl GetBattleEffectObject
	ldr r2, [sp, #44]
	adds r5, r0, #0
	str r5, [r6, r2]
	cmp r5, #0
	beq .L_080e8302
	adds r2, r5, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
	movs r1, #3
	mov r0, r8
	bl Func_080022fc
	adds r1, r0, #0
	adds r0, r5, #0
	bl Object_InitializeMode
	ldr r3, [sp, #44]
	ldr r1, [r6, r3]
	ldrb r3, [r1, #9]
	movs r2, #4
	ands r3, r7
	orrs r3, r2
	strb r3, [r1, #9]
.L_080e8302:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #4
	cmp r1, #6
	bne .L_080e82ca
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl Unnamed_080ed408
	ldr r5, .L_080e8390
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #60]
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r3, [sp, #0]
	bl Unnamed_080ed408
	adds r5, #188
	mov r2, sp
	ldr r3, [r5]
	adds r2, #60
	str r2, [sp, #16]
	str r3, [r2, #4]
	ldr r2, .L_080e8394
	ldr r3, .L_080e8380
	strh r3, [r2]
	ldr r3, .L_080e8384
	subs r2, #8
	strh r3, [r2]
	ldr r3, .L_080e8388
	adds r2, #6
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldr r1, .L_080e8398
	movs r0, #1
	bl BattleBackground_LoadFar
	movs r0, #1
	movs r1, #1
	bl BattleEffect_WipeCanvas
	ldr r0, .L_080e839c
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e83a0
	ldr r1, [sp, #44]
	b .L_080e83a4
	.2byte 0x0000
.L_080e8380:
	.4byte 0x00002737
.L_080e8384:
	.4byte 0x000000f0
.L_080e8388:
	.4byte 0x00001088
.L_080e838c:
	.4byte 0x000077fc
.L_080e8390:
	.4byte Data_03001e50
.L_080e8394:
	.4byte 0x04000048
.L_080e8398:
	.4byte 0x0000003c
.L_080e839c:
	.4byte 0x00000073
.L_080e83a0:
	.4byte 0x000000c0
.L_080e83a4:
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r3, .L_080e83e4
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_080e83e8
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_080e83ec
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_080e83f0
	subs r2, #2
	strh r3, [r2]
	ldr r7, [sp, #44]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, .L_080e83f4
	adds r3, r7, r0
	str r6, [r3]
	adds r2, r7, r1
	movs r3, #50
	str r3, [r2]
	movs r2, #188
	movs r3, #184
	lsls r3, r3, #15
	lsls r2, r2, #16
	movs r6, #160
	b .L_080e83f8
.L_080e83e4:
	.4byte 0x00007741
.L_080e83e8:
	.4byte 0x00000080
.L_080e83ec:
	.4byte 0x00001010
.L_080e83f0:
	.4byte 0x00003f44
.L_080e83f4:
	.4byte 0x00007784
.L_080e83f8:
	ldr r0, [sp, #44]
	movs r1, #225
	lsls r6, r6, #16
	movs r7, #0
	lsls r1, r1, #7
	str r2, [sp, #28]
	str r3, [sp, #32]
	str r3, [sp, #24]
	str r6, [sp, #20]
	mov r8, r7
	movs r6, #0
	adds r5, r0, r1
.L_080e8410:
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #1
	ldr r2, .L_080e8734
	add r8, r3
	mov r0, r8
	str r7, [r5, #4]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #24]
	adds r7, r7, r2
	adds r5, #28
	cmp r0, #6
	bne .L_080e8410
	ldr r6, [sp, #44]
	ldr r7, .L_080e8738
	movs r1, #0
	mov r8, r1
	movs r2, #24
	adds r3, r6, r7
.L_080e8440:
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #58
	bne .L_080e8440
	movs r2, #0
	mov r8, r2
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080e873c
	negs r1, r1
	lsls r2, r2, #3
.L_080e845c:
	movs r6, #1
	add r8, r6
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080e845c
	ldr r7, [sp, #44]
	ldr r0, .L_080e8740
	ldr r1, .L_080e8744
	adds r2, r7, r0
	movs r3, #24
	str r3, [r2]
	movs r3, #0
	adds r2, r7, r1
	str r3, [r2]
	str r3, [sp, #40]
	ldr r3, .L_080e8748
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080e848a
	b .L_080e8968
.L_080e848a:
	mov r2, sp
	mov r3, sp
	adds r2, #68
	adds r3, #52
	str r2, [sp, #8]
	str r3, [sp, #12]
.L_080e8496:
	ldr r6, [sp, #40]
	cmp r6, #94
	bne .L_080e84a2
	movs r0, #156
	bl Func_080f9010
.L_080e84a2:
	ldr r7, [sp, #40]
	cmp r7, #136
	bne .L_080e84ae
	movs r0, #156
	bl Func_080f9010
.L_080e84ae:
	ldr r0, [sp, #40]
	cmp r0, #178
	bne .L_080e84ba
	movs r0, #156
	bl Func_080f9010
.L_080e84ba:
	movs r2, #130
	ldr r1, [sp, #40]
	lsls r2, r2, #1
	cmp r1, r2
	bne .L_080e84ca
	movs r0, #145
	bl Func_080f9010
.L_080e84ca:
	ldr r3, .L_080e874c
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #52]
	str r4, [sp, #56]
	ldr r3, [sp, #40]
	subs r3, #96
	cmp r3, #155
	bhi .L_080e84e4
	ldr r3, [sp, #44]
	ldr r6, .L_080e8750
	adds r2, r3, r6
	b .L_080e84f4
.L_080e84e4:
	ldr r7, [sp, #40]
	ldr r0, .L_080e8754
	adds r3, r7, r0
	cmp r3, #3
	bhi .L_080e84f8
	ldr r1, [sp, #44]
	ldr r3, .L_080e8750
	adds r2, r1, r3
.L_080e84f4:
	movs r3, #1
	str r3, [r2]
.L_080e84f8:
	movs r3, #0
	ldr r0, [sp, #44]
	ldr r1, .L_080e8758
	str r3, [sp, #80]
	str r3, [sp, #72]
	ldr r5, [sp, #8]
	ldr r7, .L_080e875c
	mov r8, r3
	adds r6, r0, r1
.L_080e850a:
	ldr r3, .L_080e8760
	mov r2, r8
	ldrb r3, [r3, r2]
	ldr r0, [sp, #28]
	lsls r3, r3, #16
	adds r3, r3, r0
	adds r3, r3, r7
	str r3, [r5]
	ldr r3, .L_080e8764
	ldrb r3, [r3, r2]
	ldr r1, [sp, #32]
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r3, r3, r7
	str r3, [r5, #8]
	ldr r2, [sp, #12]
	movs r3, #0
	ldmia r6!, {r0}
	adds r1, r5, #0
	bl Object_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #7
	bne .L_080e850a
	ldr r6, [sp, #40]
	cmp r6, #90
	bgt .L_080e8566
	lsls r5, r6, #9
	adds r0, r5, #0
	bl Trig_Sin
	movs r7, #156
	lsls r0, r0, #4
	lsls r7, r7, #16
	adds r7, r0, r7
	adds r0, r5, #0
	str r7, [sp, #20]
	bl Func_0800231c
	movs r1, #184
	lsls r0, r0, #4
	lsls r1, r1, #15
	adds r1, r0, r1
	str r1, [sp, #24]
.L_080e8566:
	ldr r2, [sp, #40]
	cmp r2, #196
	bgt .L_080e8600
	ldr r7, [sp, #44]
	movs r3, #0
	mov r8, r3
	movs r6, #91
	mov r10, r7
.L_080e8576:
	ldr r0, [sp, #40]
	cmp r0, r6
	blt .L_080e858c
	adds r3, r6, #4
	cmp r0, r3
	bge .L_080e858c
	ldr r1, [sp, #24]
	movs r2, #128
	lsls r2, r2, #12
	adds r2, r1, r2
	str r2, [sp, #24]
.L_080e858c:
	ldr r7, [sp, #40]
	adds r3, r6, #3
	cmp r7, r3
	bne .L_080e85d8
	ldr r5, .L_080e8768
	movs r0, #255
	movs r7, #0
	mov r9, r0
	add r5, r10
.L_080e859e:
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	mov r2, r9
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #4
	bne .L_080e859e
.L_080e85d8:
	adds r3, r6, #0
	ldr r7, [sp, #40]
	adds r3, #20
	cmp r7, r3
	blt .L_080e85f0
	adds r3, #16
	cmp r7, r3
	bge .L_080e85f0
	ldr r0, [sp, #24]
	ldr r1, .L_080e876c
	adds r1, r0, r1
	str r1, [sp, #24]
.L_080e85f0:
	movs r3, #1
	add r8, r3
	movs r2, #224
	mov r7, r8
	adds r6, #40
	add r10, r2
	cmp r7, #3
	bne .L_080e8576
.L_080e8600:
	ldr r3, [sp, #40]
	subs r3, #244
	cmp r3, #7
	bhi .L_080e8610
	ldr r0, [sp, #20]
	ldr r1, .L_080e8770
	adds r1, r0, r1
	str r1, [sp, #20]
.L_080e8610:
	ldr r3, [sp, #40]
	subs r3, #252
	cmp r3, #23
	bhi .L_080e8624
	ldr r3, [sp, #40]
	ldr r2, [sp, #20]
	subs r3, #250
	lsls r3, r3, #16
	subs r3, r2, r3
	str r3, [sp, #20]
.L_080e8624:
	ldr r3, [sp, #40]
	ldr r6, .L_080e8774
	cmp r3, r6
	bgt .L_080e866e
	ldr r0, [sp, #24]
	movs r3, #255
	ldr r1, [sp, #44]
	lsls r3, r3, #24
	ldr r2, .L_080e8778
	str r3, [sp, #72]
	adds r3, r0, r3
	str r3, [sp, #76]
	adds r3, r1, r2
	ldr r7, [sp, #20]
	ldr r0, [r3]
	add r3, sp, #68
	adds r1, r3, #0
	ldr r2, [sp, #12]
	movs r3, #0
	str r7, [sp, #68]
	bl Object_ApplyProjectedPlacementFar
	movs r7, #128
	ldr r6, [sp, #20]
	ldr r0, [sp, #44]
	ldr r1, .L_080e877c
	lsls r7, r7, #14
	adds r3, r6, r7
	add r2, sp, #68
	str r3, [sp, #68]
	adds r3, r0, r1
	ldr r0, [r3]
	adds r1, r2, #0
	movs r3, #0
	ldr r2, [sp, #12]
	bl Object_ApplyProjectedPlacementFar
.L_080e866e:
	ldr r6, [sp, #8]
	movs r3, #0
	str r3, [r6, #4]
	ldr r7, [sp, #44]
	movs r0, #225
	lsls r0, r0, #7
	mov r8, r3
	mov r11, r6
	adds r5, r7, r0
	mov r9, r7
.L_080e8682:
	ldr r3, [r5, #24]
	cmp r3, #2
	bne .L_080e868a
	b .L_080e8796
.L_080e868a:
	ldr r3, [r5]
	mov r1, r11
	str r3, [r1]
	ldr r3, [r5, #4]
	mov r2, r8
	str r3, [r1, #8]
	ldr r6, .L_080e8780
	lsls r3, r2, #2
	ldr r7, [sp, #44]
	adds r3, r3, r6
	ldr r0, [r7, r3]
	ldr r2, [sp, #12]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r0, [sp, #40]
	cmp r0, #96
	ble .L_080e86c6
	movs r1, #128
	lsls r1, r1, #7
	adds r3, r2, r1
	str r3, [r5, #16]
.L_080e86c6:
	movs r2, #240
	ldr r3, [r5, #4]
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_080e8796
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #1
	bne .L_080e8788
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r6, .L_080e8784
	str r3, [r5, #16]
	movs r3, #255
	movs r7, #0
	mov r10, r3
	add r6, r9
.L_080e86f0:
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6]
	ldr r0, .L_080e875c
	ldr r3, [r5, #4]
	adds r3, r3, r0
	str r3, [r6, #4]
	bl Random16
	mov r1, r10
	ands r0, r1
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r6, #12]
	bl Random16
	mov r2, r10
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r6, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r7, #1
	str r3, [r6, #24]
	adds r6, #28
	cmp r7, #2
	bne .L_080e86f0
	b .L_080e8796
	.2byte 0x0000
.L_080e8734:
	.4byte 0xfff00000
.L_080e8738:
	.4byte 0x00007140
.L_080e873c:
	.4byte Data_02010018
.L_080e8740:
	.4byte 0x000077b4
.L_080e8744:
	.4byte 0x000077b8
.L_080e8748:
	.4byte gKeysRepeat
.L_080e874c:
	.4byte Data_080edac8
.L_080e8750:
	.4byte 0x000077a8
.L_080e8754:
	.4byte 0xfffffefc
.L_080e8758:
	.4byte 0x000077d8
.L_080e875c:
	.4byte 0xffe00000
.L_080e8760:
	.4byte Data_080eeed8
.L_080e8764:
	.4byte Data_080eeee1
.L_080e8768:
	.4byte 0x00007128
.L_080e876c:
	.4byte 0xfffe0000
.L_080e8770:
	.4byte 0xffff0000
.L_080e8774:
	.4byte 0x00000103
.L_080e8778:
	.4byte 0x000077f4
.L_080e877c:
	.4byte 0x000077f8
.L_080e8780:
	.4byte 0x000077fc
.L_080e8784:
	.4byte 0x000073c8
.L_080e8788:
	ldr r3, [sp, #40]
	cmp r3, #199
	bgt .L_080e8796
	movs r3, #0
	str r3, [r5, #4]
	str r3, [r5, #16]
	str r3, [r5, #24]
.L_080e8796:
	movs r7, #1
	add r8, r7
	movs r6, #56
	mov r0, r8
	adds r5, #28
	add r9, r6
	cmp r0, #6
	beq .L_080e87a8
	b .L_080e8682
.L_080e87a8:
	ldr r2, [sp, #44]
	ldr r3, .L_080e89b4
	movs r1, #0
	mov r8, r1
	adds r5, r2, r3
.L_080e87b2:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_080e87fc
	cmp r0, #23
	bhi .L_080e87ec
	movs r1, #6
	bl FixedPoint_Ratio
	ldr r3, .L_080e89b8
	adds r0, #3
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r3, .L_080e89bc
	ldr r6, [sp, #44]
	ldrh r4, [r3, r0]
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r6, r1
	movs r6, #6
	ldrsh r3, [r5, r6]
	lsrs r0, r4, #1
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #60]
	bl _call_via_r4
.L_080e87ec:
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_080e89c0
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_080e87fc:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #56
	bne .L_080e87b2
	movs r2, #130
	ldr r1, [sp, #40]
	lsls r2, r2, #1
	cmp r1, r2
	bne .L_080e88c8
	movs r3, #0
	mov r8, r3
	ldr r6, [sp, #44]
	ldr r3, .L_080e89c4
	ldr r3, [r6, r3]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080e8854
	ldr r7, .L_080e89c4
	adds r5, r6, r7
	movs r6, #36
.L_080e8828:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r1, #4
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #8
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	negs r2, r2
	bl ObjectGroup_UpdateMembers
	movs r3, #1
	add r8, r3
	ldr r3, [r5]
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r8, r3
	bne .L_080e8828
.L_080e8854:
	ldr r6, [sp, #44]
	ldr r7, .L_080e89c8
	movs r3, #8
	adds r2, r6, r7
	str r3, [r2]
	movs r1, #130
	ldr r0, [sp, #40]
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_080e88c8
	movs r2, #0
	ldr r7, .L_080e89cc
	mov r8, r2
.L_080e886e:
	bl Random16
	ldr r5, .L_080e89d0
	ands r5, r0
	bl Random16
	ldr r3, .L_080e89d4
	adds r6, r0, #0
	ands r6, r3
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r7]
	movs r3, #184
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Func_0800231c
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r6, #128
	movs r3, #1
	add r8, r3
	lsls r6, r6, #2
	adds r7, #28
	cmp r8, r6
	bne .L_080e886e
.L_080e88c8:
	ldr r0, .L_080e89d8
	movs r7, #0
	ldr r6, .L_080e89cc
	mov r8, r7
	mov r10, r0
.L_080e88d2:
	ldr r0, [r6, #24]
	cmp r0, #0
	blt .L_080e8922
	asrs r0, r0, #3
	adds r0, #1
	lsls r5, r0, #1
	mov r1, r8
	subs r3, r5, #2
	mov r2, r10
	movs r4, #1
	ands r4, r1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #36]
	adds r1, r3, r1
	lsrs r3, r0, #31
	movs r7, #2
	ldrsh r2, [r6, r7]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r6, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r5, [sp, #4]
	ldr r0, [sp, #16]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #48]
	bl _call_via_r4
	movs r2, #128
	adds r0, r6, #0
	movs r1, #62
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_080e8922:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #2
	adds r6, #28
	cmp r8, r2
	bne .L_080e88d2
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r6, .L_080e89dc
	ldr r3, [sp, #44]
	adds r2, r3, r6
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r7, [sp, #40]
	movs r0, #160
	adds r7, #1
	lsls r0, r0, #1
	str r7, [sp, #40]
	cmp r7, r0
	beq .L_080e8968
	ldr r3, .L_080e89e0
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080e8968
	b .L_080e8496
.L_080e8968:
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	bl BattleEffect_SetupBlendedDisplay
	ldr r3, .L_080e89e4
	ldr r2, [sp, #44]
	movs r1, #0
	mov r8, r1
	adds r5, r2, r3
.L_080e897c:
	movs r6, #1
	add r8, r6
	ldmia r5!, {r0}
	mov r7, r8
	bl ResourceObject_ReleaseFar
	cmp r7, #15
	bne .L_080e897c
	ldr r0, .L_080e89e8
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e89b4:
	.4byte 0x00007128
.L_080e89b8:
	.4byte Data_080eeeea
.L_080e89bc:
	.4byte Data_080eeef8
.L_080e89c0:
	.4byte 0xffffc000
.L_080e89c4:
	.4byte 0x00007828
.L_080e89c8:
	.4byte 0x000077a8
.L_080e89cc:
	.4byte gMapCellBuffer
.L_080e89d0:
	.4byte 0x000003ff
.L_080e89d4:
	.4byte 0x0000ffff
.L_080e89d8:
	.4byte ParticleStreams_CellOffsets
.L_080e89dc:
	.4byte 0x00007824
.L_080e89e0:
	.4byte gKeysRepeat
.L_080e89e4:
	.4byte 0x000077d8
.L_080e89e8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
