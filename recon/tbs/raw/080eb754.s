.syntax unified
	.thumb
	.global Unnamed_080eb754
	.thumb_func
Unnamed_080eb754:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080eb7c4
	ldr r1, [r5]
	sub sp, #176
	str r1, [sp, #80]
	subs r3, r5, #4
	ldr r3, [r3]
	str r3, [sp, #76]
	ldr r4, .L_080eb7c8
	ldr r2, [r5, #4]
	adds r6, r3, r4
	str r2, [sp, #68]
	str r0, [r6]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080eb7cc
	ldr r3, .L_080eb7bc
	strh r3, [r2]
	ldr r2, .L_080eb7c0
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r7, [sp, #76]
	movs r0, #239
	lsls r0, r0, #7
	movs r3, #0
	adds r7, r7, r0
	movs r1, #144
	str r3, [r7]
	lsls r1, r1, #3
	ldr r0, .L_080eb7d0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	movs r1, #0
	bl BattleEffect_WipeCanvas
	ldr r1, .L_080eb7d4
	movs r0, #9
	movs r2, #1
	b .L_080eb7d8
.L_080eb7bc:
	.4byte 0x00000784
.L_080eb7c0:
	.4byte 0x00000000
.L_080eb7c4:
	.4byte Data_03001ef0
.L_080eb7c8:
	.4byte 0x00007828
.L_080eb7cc:
	.4byte 0x0400000c
.L_080eb7d0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080eb7d4:
	.4byte 0x00000175
.L_080eb7d8:
	bl BattleFx_SpawnObjects
	ldr r2, .L_080eb82c
	movs r3, #240
	str r3, [r2, #16]
	ldr r0, [r6]
	bl BattleFx_SelectLivingTargets
	ldr r2, .L_080eb830
	ldr r3, .L_080eb824
	strh r3, [r2]
	ldr r3, .L_080eb828
	subs r2, #8
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldr r1, .L_080eb834
	movs r0, #1
	bl BattleBackground_LoadFar
	movs r0, #1
	movs r1, #1
	bl BattleEffect_WipeCanvas
	ldr r0, .L_080eb838
	ldr r1, [sp, #68]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080eb83c
	ldr r1, [sp, #76]
	movs r2, #1
	movs r3, #1
	b .L_080eb840
	.2byte 0x0000
.L_080eb824:
	.4byte 0x00002737
.L_080eb828:
	.4byte 0x000000ca
.L_080eb82c:
	.4byte gProjection
.L_080eb830:
	.4byte 0x04000048
.L_080eb834:
	.4byte 0x0000003a
.L_080eb838:
	.4byte 0x00000073
.L_080eb83c:
	.4byte 0x00000095
.L_080eb840:
	bl Resource_LoadAndDecompress
	ldr r3, .L_080eb878
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_080eb87c
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_080eb880
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_080eb884
	movs r1, #0
	subs r2, #2
	strh r3, [r2]
	str r1, [sp, #64]
	str r1, [sp, #60]
	ldr r3, .L_080eb888
	ldrh r3, [r3, #4]
	str r3, [sp, #56]
	ldr r5, [r5, #16]
	movs r2, #1
	str r5, [sp, #52]
	str r1, [sp, #48]
	str r2, [r7]
	b .L_080eb88c
	.2byte 0x0000
.L_080eb878:
	.4byte 0x00007741
.L_080eb87c:
	.4byte 0x00000080
.L_080eb880:
	.4byte 0x0000100e
.L_080eb884:
	.4byte 0x00003f44
.L_080eb888:
	.4byte gBgScroll
.L_080eb88c:
	ldr r4, [sp, #76]
	ldr r7, .L_080eb908
	ldr r0, [sp, #64]
	adds r3, r4, r7
	str r0, [r3]
	ldr r1, [sp, #52]
	movs r3, #225
	str r2, [r1, #16]
	lsls r3, r3, #7
	movs r2, #0
	mov r8, r2
	movs r6, #31
	adds r5, r4, r3
.L_080eb8a6:
	bl Random16
	ands r0, r6
	adds r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r6
	adds r0, #48
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #16
	str r0, [r5, #16]
	bl Random16
	movs r1, #48
	bl IwramUnsignedRemainderEntry
	movs r4, #1
	add r8, r4
	adds r0, #2
	mov r7, r8
	str r0, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne .L_080eb8a6
	movs r3, #3
	movs r2, #7
	movs r0, #46
	movs r1, #7
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, .L_080eb90c
	adds r3, #184
	ldr r3, [r3]
	ldr r2, .L_080eb910
	str r3, [sp, #72]
	movs r0, #0
	ldr r3, .L_080eb904
	mov r11, r0
	strh r3, [r2]
	b .L_080ebc56
.L_080eb904:
	.4byte 0x00000786
.L_080eb908:
	.4byte 0x00007784
.L_080eb90c:
	.4byte Data_03001e50
.L_080eb910:
	.4byte 0x0400000c
.L_080eb914:
	mov r3, r11
	subs r3, #24
	cmp r3, #31
	bhi .L_080eb922
	ldr r1, [sp, #48]
	adds r1, #1
	str r1, [sp, #48]
.L_080eb922:
	ldr r2, [sp, #48]
	cmp r2, #24
	ble .L_080eb92c
	movs r3, #24
	str r3, [sp, #48]
.L_080eb92c:
	mov r4, r11
	cmp r4, #135
	bgt .L_080eb944
	ldr r7, .L_080ebb0c
	ldr r0, [sp, #48]
	ldrh r3, [r7, #4]
	adds r1, r7, #0
	subs r3, r3, r0
	strh r3, [r1, #4]
	ldr r2, [sp, #60]
	adds r2, r2, r0
	str r2, [sp, #60]
.L_080eb944:
	mov r3, r11
	cmp r3, #149
	bgt .L_080eba18
	ldr r3, .L_080ebb10
	ldr r4, [r3, #4]
	ldr r3, [r3]
	mov r7, r11
	str r3, [sp, #92]
	str r4, [sp, #96]
	movs r4, #0
	mov r10, r4
	cmp r7, #103
	ble .L_080eb966
	ldr r0, .L_080ebb14
	lsls r3, r7, #4
	adds r0, r0, r3
	mov r10, r0
.L_080eb966:
	mov r3, r11
	subs r3, #8
	cmp r3, #23
	bhi .L_080eb978
	ldr r1, [sp, #64]
	ldr r2, [sp, #48]
	adds r3, r1, r2
	subs r3, #8
	str r3, [sp, #64]
.L_080eb978:
	mov r3, r11
	cmp r3, #7
	ble .L_080eb9be
	movs r5, #96
	cmp r3, #104
	bgt .L_080eb986
	movs r5, #32
.L_080eb986:
	mov r4, r11
	ldr r7, .L_080ebb18
	lsls r3, r4, #10
	adds r0, r3, r7
	ldr r3, .L_080ebb1c
	movs r1, #128
	ands r0, r3
	lsls r1, r1, #8
	cmp r0, r1
	ble .L_080eb99e
	ldr r2, .L_080ebb20
	adds r0, r0, r2
.L_080eb99e:
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [sp, #44]
	mov r4, r11
	movs r3, #31
	ands r3, r4
	cmp r3, #8
	bne .L_080eb9be
	ldr r7, [sp, #76]
	ldr r0, .L_080ebb24
	movs r3, #4
	adds r2, r7, r0
	str r3, [r2]
.L_080eb9be:
	add r3, sp, #160
	movs r2, #0
	str r2, [r3, #12]
	movs r2, #255
	lsls r2, r2, #16
	str r2, [r3, #4]
	adds r6, r3, #0
	ldr r2, [sp, #76]
	ldr r3, .L_080ebb28
	movs r1, #0
	mov r8, r1
	add r7, sp, #92
	adds r5, r2, r3
.L_080eb9d8:
	ldr r3, .L_080ebb2c
	mov r4, r8
	ldr r0, [sp, #64]
	ldrb r3, [r3, r4]
	mov r1, r10
	adds r3, r0, r3
	subs r3, r3, r1
	movs r2, #224
	lsls r2, r2, #16
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, .L_080ebb30
	ldrb r3, [r3, r4]
	ldr r4, [sp, #44]
	movs r0, #144
	subs r3, r3, r4
	lsls r0, r0, #15
	lsls r3, r3, #16
	adds r3, r3, r0
	str r3, [r6, #8]
	adds r1, r6, #0
	adds r2, r7, #0
	ldmia r5!, {r0}
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	bne .L_080eb9d8
.L_080eba18:
	mov r3, r11
	cmp r3, #26
	bgt .L_080ebab0
	ldr r7, [sp, #60]
	lsls r3, r3, #3
	adds r7, #4
	mov r10, r3
	cmp r7, #10
	ble .L_080eba2c
	movs r7, #10
.L_080eba2c:
	mov r4, r10
	cmp r4, #64
	ble .L_080eba36
	movs r0, #64
	mov r10, r0
.L_080eba36:
	movs r1, #0
	mov r2, r10
	mov r8, r1
	cmp r2, #0
	beq .L_080ebab0
	ldr r3, [sp, #60]
	ldr r4, [sp, #60]
	lsls r3, r3, #1
	str r3, [sp, #40]
	adds r3, r3, r4
	lsls r3, r3, #2
	adds r3, #48
	str r3, [sp, #36]
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	lsls r0, r7, #1
	str r3, [sp, #32]
	mov r9, r0
.L_080eba5c:
	mov r1, r8
	lsls r6, r1, #10
	adds r0, r6, #0
	bl Trig_Sin
	ldr r3, [sp, #40]
	adds r3, #8
	adds r5, r3, #0
	muls r5, r0
	ldr r2, [sp, #60]
	asrs r5, r5, #16
	adds r0, r6, #0
	adds r5, r5, r2
	bl Func_0800231c
	ldr r4, [sp, #36]
	adds r3, r4, #0
	muls r3, r0
	mov r2, r9
	ldr r0, .L_080ebb34
	ldr r4, [sp, #32]
	subs r2, #2
	ldrh r1, [r0, r2]
	adds r5, #96
	ldr r2, [sp, #68]
	asrs r3, r3, #16
	subs r5, r5, r4
	mov r0, r9
	adds r3, #64
	adds r1, r2, r1
	str r0, [sp, #4]
	subs r3, r3, r7
	str r7, [sp, #0]
	ldr r0, [sp, #80]
	adds r2, r5, #0
	ldr r4, [sp, #72]
	bl _call_via_r4
	movs r0, #1
	add r8, r0
	cmp r8, r10
	bne .L_080eba5c
.L_080ebab0:
	mov r1, r11
	cmp r1, #24
	bne .L_080ebacc
	ldr r3, [sp, #76]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r7, [sp, #76]
	ldr r0, .L_080ebb38
	movs r3, #50
	adds r2, r7, r0
	str r3, [r2]
.L_080ebacc:
	mov r1, r11
	cmp r1, #28
	bne .L_080ebad8
	ldr r2, .L_080ebb3c
	ldr r3, .L_080ebb08
	strh r3, [r2]
.L_080ebad8:
	mov r2, r11
	cmp r2, #17
	ble .L_080ebbbe
	ldr r4, [sp, #76]
	movs r7, #225
	movs r3, #0
	lsls r7, r7, #7
	mov r8, r3
	movs r6, #31
	adds r5, r4, r7
.L_080ebaec:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080ebb78
	movs r1, #3
	mov r0, r8
	bl Func_080022fc
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, .L_080ebb34
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	b .L_080ebb40
	.2byte 0x0000
.L_080ebb08:
	.4byte 0x00000784
.L_080ebb0c:
	.4byte gBgScroll
.L_080ebb10:
	.4byte Data_080edad8
.L_080ebb14:
	.4byte 0xfffff980
.L_080ebb18:
	.4byte 0xffffe000
.L_080ebb1c:
	.4byte 0x0000ffff
.L_080ebb20:
	.4byte 0xffff8000
.L_080ebb24:
	.4byte 0x000077a8
.L_080ebb28:
	.4byte 0x000077d8
.L_080ebb2c:
	.4byte Data_080eef56
.L_080ebb30:
	.4byte Data_080eef5f
.L_080ebb34:
	.4byte ParticleStreams_CellOffsets
.L_080ebb38:
	.4byte 0x00007784
.L_080ebb3c:
	.4byte 0x0400000c
.L_080ebb40:
	movs r7, #6
	ldrsh r3, [r5, r7]
	ldr r2, [sp, #68]
	subs r3, r3, r0
	adds r1, r2, r1
	ldr r2, [r5]
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #80]
	ldr r4, [sp, #72]
	bl _call_via_r4
	ldr r3, [r5]
	adds r3, #2
	ldr r2, [r5, #16]
	str r3, [r5]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_080ebb72
	adds r3, #63
.L_080ebb72:
	asrs r3, r3, #6
	str r3, [r5, #16]
	b .L_080ebb7c
.L_080ebb78:
	subs r3, #1
	str r3, [r5, #24]
.L_080ebb7c:
	ldr r3, [r5]
	cmp r3, #128
	bgt .L_080ebb88
	ldr r3, [r5, #24]
	cmp r3, #1
	bne .L_080ebbb2
.L_080ebb88:
	bl Random16
	ldr r7, [sp, #64]
	ands r0, r6
	adds r0, r0, r7
	adds r0, #172
	str r0, [r5]
	bl Random16
	ldr r1, [sp, #44]
	ands r0, r6
	subs r0, r0, r1
	adds r0, #56
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #15
	str r0, [r5, #16]
.L_080ebbb2:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #48
	bne .L_080ebaec
.L_080ebbbe:
	mov r4, r11
	cmp r4, #31
	ble .L_080ebc0e
	mov r3, r11
	subs r3, #32
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r5, r3, #1
	cmp r5, #40
	ble .L_080ebbd4
	movs r5, #40
.L_080ebbd4:
	movs r7, #0
	mov r8, r7
	movs r6, #0
	movs r7, #120
.L_080ebbdc:
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r1, r0, #1
	adds r1, r1, r0
	movs r3, #48
	ldr r0, [sp, #76]
	str r3, [sp, #0]
	lsls r1, r1, #9
	movs r3, #32
	adds r1, r0, r1
	str r3, [sp, #4]
	ldr r0, [sp, #80]
	adds r3, r6, #0
	subs r2, r7, r5
	ldr r4, [sp, #72]
	bl _call_via_r4
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #18
	cmp r1, #6
	bne .L_080ebbdc
.L_080ebc0e:
	ldr r3, [sp, #76]
	ldr r4, .L_080ebc34
	adds r2, r3, r4
	ldr r3, [r2]
	cmp r3, #0
	ble .L_080ebc3c
	subs r3, #1
	str r3, [r2]
	bl Random16
	ldr r3, .L_080ebc30
	ldr r7, .L_080ebc38
	ands r0, r3
	adds r0, #28
	strh r0, [r7, #6]
	b .L_080ebc42
	.2byte 0x0000
.L_080ebc30:
	.4byte 0x00000007
.L_080ebc34:
	.4byte 0x000077a8
.L_080ebc38:
	.4byte gBgScroll
.L_080ebc3c:
	ldr r0, .L_080ebd00
	movs r3, #32
	strh r3, [r0, #6]
.L_080ebc42:
	ldr r1, [sp, #76]
	ldr r3, .L_080ebd04
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r4, #1
	add r11, r4
.L_080ebc56:
	mov r7, r11
	cmp r7, #120
	beq .L_080ebcb0
	movs r0, #0
	str r0, [sp, #44]
	cmp r7, #0
	bne .L_080ebc6a
	movs r0, #136
	bl Func_080f9010
.L_080ebc6a:
	mov r1, r11
	cmp r1, #26
	bne .L_080ebc76
	movs r0, #141
	bl Func_080f9010
.L_080ebc76:
	mov r2, r11
	cmp r2, #40
	bne .L_080ebc82
	movs r0, #154
	bl Func_080f9010
.L_080ebc82:
	mov r3, r11
	cmp r3, #72
	bne .L_080ebc8e
	movs r0, #154
	bl Func_080f9010
.L_080ebc8e:
	mov r4, r11
	cmp r4, #104
	bne .L_080ebc9a
	movs r0, #154
	bl Func_080f9010
.L_080ebc9a:
	ldr r3, .L_080ebd08
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080ebca8
	b .L_080eb914
.L_080ebca8:
	mov r7, r11
	cmp r7, #16
	bgt .L_080ebcb0
	b .L_080eb914
.L_080ebcb0:
	add r0, sp, #56
	ldr r3, .L_080ebd00
	ldrh r0, [r0]
	strh r0, [r3, #4]
	ldr r1, [sp, #52]
	movs r2, #0
	str r2, [r1, #16]
	bl BattleEffect_SetupBlendedDisplay
	ldr r2, .L_080ebd0c
	ldr r3, .L_080ebcfc
	strh r3, [r2]
	ldr r3, [sp, #76]
	ldr r4, .L_080ebd10
	movs r2, #0
	mov r8, r2
	movs r0, #12
	adds r1, r3, r4
.L_080ebcd4:
	ldmia r1!, {r2}
	ldrb r3, [r2, #9]
	movs r7, #1
	orrs r3, r0
	add r8, r7
	strb r3, [r2, #9]
	mov r2, r8
	cmp r2, #9
	bne .L_080ebcd4
	mov r4, sp
	adds r4, #116
	movs r3, #224
	mov r2, sp
	str r4, [sp, #24]
	str r3, [sp, #28]
	movs r1, #0
	adds r3, r4, #0
	adds r2, #130
	b .L_080ebd14
	.2byte 0x0000
.L_080ebcfc:
	.4byte 0x000000f0
.L_080ebd00:
	.4byte gBgScroll
.L_080ebd04:
	.4byte 0x00007824
.L_080ebd08:
	.4byte gKeysRepeat
.L_080ebd0c:
	.4byte 0x04000040
.L_080ebd10:
	.4byte 0x000077d8
.L_080ebd14:
	strb r1, [r3]
	adds r3, #1
	cmp r3, r2
	bne .L_080ebd14
	mov r7, sp
	adds r7, #132
	str r7, [sp, #20]
	ldr r5, [sp, #20]
	movs r7, #31
	add r6, sp, #148
.L_080ebd28:
	bl Random16
	ands r0, r7
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_080ebd28
	movs r0, #0
	movs r2, #160
	ldr r3, .L_080ebd9c
	mov r8, r0
	movs r1, #0
	lsls r2, r2, #1
.L_080ebd42:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080ebd42
	ldr r7, [sp, #76]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, .L_080ebda0
	adds r2, r7, r0
	movs r3, #2
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #75
	str r3, [r2]
	ldr r2, .L_080ebda4
	ldr r3, .L_080ebd94
	strh r3, [r2]
	ldr r3, .L_080ebd98
	adds r2, #70
	strh r3, [r2]
	ldr r3, .L_080ebda8
	movs r2, #0
	str r3, [sp, #16]
	mov r11, r2
.L_080ebd76:
	mov r4, r11
	cmp r4, #23
	bgt .L_080ebe3c
	ldr r3, .L_080ebdac
	ldr r7, [sp, #28]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	subs r7, #16
	mov r0, r11
	str r3, [sp, #84]
	str r4, [sp, #88]
	str r7, [sp, #28]
	cmp r0, #8
	bgt .L_080ebdcc
	b .L_080ebdb0
.L_080ebd94:
	.4byte 0x00000784
.L_080ebd98:
	.4byte 0x00001010
.L_080ebd9c:
	.4byte Data_02010018
.L_080ebda0:
	.4byte 0x00007784
.L_080ebda4:
	.4byte 0x0400000c
.L_080ebda8:
	.4byte 0xfffffe20
.L_080ebdac:
	.4byte Data_080edae0
.L_080ebdb0:
	movs r1, #128
	lsls r3, r0, #11
	lsls r1, r1, #7
	movs r2, #128
	adds r0, r3, r1
	lsls r2, r2, #8
	cmp r0, r2
	ble .L_080ebdc4
	ldr r4, .L_080ec0b4
	adds r0, r3, r4
.L_080ebdc4:
	bl Trig_Sin
	lsls r0, r0, #6
	b .L_080ebde8
.L_080ebdcc:
	mov r7, r11
	movs r1, #128
	lsls r3, r7, #11
	lsls r1, r1, #7
	movs r2, #128
	adds r0, r3, r1
	lsls r2, r2, #8
	cmp r0, r2
	ble .L_080ebde2
	ldr r4, .L_080ec0b4
	adds r0, r3, r4
.L_080ebde2:
	bl Trig_Sin
	lsls r0, r0, #5
.L_080ebde8:
	asrs r4, r0, #16
	add r3, sp, #100
	movs r2, #0
	str r2, [r3, #12]
	movs r2, #255
	lsls r2, r2, #16
	str r2, [r3, #4]
	ldr r0, [sp, #76]
	ldr r1, .L_080ec0b8
	movs r7, #0
	mov r8, r7
	adds r6, r3, #0
	add r7, sp, #84
	adds r5, r0, r1
.L_080ebe04:
	ldr r3, .L_080ec0bc
	mov r2, r8
	ldr r0, [sp, #28]
	ldrb r3, [r3, r2]
	adds r3, r0, r3
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, .L_080ec0c0
	ldrb r3, [r3, r2]
	movs r1, #144
	subs r3, r3, r4
	lsls r1, r1, #15
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r6, #8]
	adds r2, r7, #0
	movs r3, #0
	ldmia r5!, {r0}
	adds r1, r6, #0
	str r4, [sp, #8]
	bl Object_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	ldr r4, [sp, #8]
	cmp r3, #9
	bne .L_080ebe04
.L_080ebe3c:
	mov r4, r11
	cmp r4, #8
	bne .L_080ebe50
	ldr r7, [sp, #76]
	ldr r0, .L_080ec0c4
	adds r3, r7, r0
	str r4, [r3]
	movs r0, #145
	bl Func_080f9010
.L_080ebe50:
	mov r1, r11
	cmp r1, #11
	bne .L_080ebe5c
	movs r0, #145
	bl Func_080f9010
.L_080ebe5c:
	mov r2, r11
	cmp r2, #46
	bne .L_080ebe68
	movs r0, #137
	bl Func_080f9010
.L_080ebe68:
	ldr r2, .L_080ec0c8
	ldr r4, [sp, #76]
	movs r3, #0
	mov r8, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080ebf3a
	movs r0, #0
	movs r7, #36
	str r0, [sp, #12]
	mov r9, r7
.L_080ebe80:
	ldr r1, [sp, #24]
	mov r4, r8
	ldrb r3, [r1, r4]
	cmp r3, #0
	bne .L_080ebf1c
	ldr r7, [sp, #76]
	ldr r3, [r7, r2]
	add r5, sp, #148
	mov r1, r9
	ldrsh r0, [r3, r1]
	adds r1, r5, #0
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r3, [r5]
	ldr r4, [sp, #28]
	cmp r3, r4
	ble .L_080ebf1c
	ldr r7, [sp, #24]
	movs r3, #1
	mov r0, r8
	strb r3, [r7, r0]
	ldr r1, [sp, #12]
	ldr r2, .L_080ec0cc
	mov r10, r5
	movs r6, #0
	movs r7, #255
	adds r5, r1, r2
.L_080ebeb6:
	mov r4, r10
	ldr r3, [r4]
	lsls r3, r3, #15
	str r3, [r5]
	ldr r3, [r4, #4]
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	subs r0, #192
	lsls r3, r0, #11
	ldr r2, [r5, #12]
	str r3, [r5, #16]
	ldr r3, [r5]
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r5, #4]
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	adds r6, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r6, #32
	bne .L_080ebeb6
	ldr r7, [sp, #76]
	ldr r0, .L_080ec0c8
	adds r3, r7, r0
	ldr r3, [r3]
	mov r1, r9
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl BattleMotion_ApplyVariantMotionFar
	movs r0, #134
	bl Func_080f9010
.L_080ebf1c:
	ldr r4, [sp, #12]
	movs r7, #224
	lsls r7, r7, #2
	adds r4, r4, r7
	str r4, [sp, #12]
	ldr r2, .L_080ec0c8
	ldr r1, [sp, #76]
	movs r3, #2
	add r9, r3
	ldr r3, [r1, r2]
	movs r0, #1
	ldr r3, [r3, #20]
	add r8, r0
	cmp r8, r3
	bne .L_080ebe80
.L_080ebf3a:
	movs r2, #0
	ldr r5, .L_080ec0cc
	mov r8, r2
	movs r7, #3
	movs r6, #6
.L_080ebf44:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_080ebf80
	ldr r3, .L_080ec0d0
	ldrh r1, [r3, #4]
	ldr r3, [sp, #68]
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r3, r1
	movs r0, #6
	ldrsh r3, [r5, r0]
	subs r2, #1
	subs r3, #3
	str r7, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #80]
	ldr r4, [sp, #72]
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
.L_080ebf80:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #192
	bne .L_080ebf44
	mov r2, r11
	cmp r2, #48
	bne .L_080ebf98
	movs r0, #136
	bl Func_080f9010
.L_080ebf98:
	mov r3, r11
	cmp r3, #40
	ble .L_080ebff6
	ldr r4, [sp, #76]
	movs r7, #239
	lsls r7, r7, #7
	ldr r0, .L_080ec0d4
	adds r2, r4, r7
	movs r3, #0
	ldr r6, [sp, #16]
	str r3, [r2]
	adds r2, r4, r0
	movs r3, #75
	movs r1, #0
	movs r5, #8
	str r3, [r2]
	mov r8, r1
	negs r5, r5
.L_080ebfbc:
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r1, r0, #1
	ldr r2, [sp, #76]
	ldr r3, [sp, #20]
	adds r1, r1, r0
	mov r4, r8
	lsls r1, r1, #9
	adds r1, r2, r1
	ldrb r2, [r3, r4]
	movs r3, #48
	str r3, [sp, #0]
	subs r2, r2, r6
	movs r3, #32
	str r3, [sp, #4]
	adds r2, #120
	adds r3, r5, #0
	ldr r0, [sp, #80]
	ldr r7, [sp, #72]
	bl _call_via_r7
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #8
	cmp r1, #16
	bne .L_080ebfbc
.L_080ebff6:
	mov r2, r11
	cmp r2, #64
	ble .L_080ec008
	ldr r3, [sp, #76]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
.L_080ec008:
	mov r7, r11
	cmp r7, #58
	bne .L_080ec044
	ldr r3, .L_080ec0c8
	ldr r1, [sp, #76]
	ldr r3, [r1, r3]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	beq .L_080ec044
	ldr r2, .L_080ec0c8
	movs r6, #36
	adds r5, r1, r2
.L_080ec024:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #14
	subs r3, #1
	movs r2, #5
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r7, #1
	ldr r3, [r3, #20]
	add r8, r7
	adds r6, #2
	cmp r8, r3
	bne .L_080ec024
.L_080ec044:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r1, .L_080ec0d8
	ldr r0, [sp, #76]
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	ldr r2, [sp, #16]
	add r11, r3
	adds r2, #12
	mov r4, r11
	str r2, [sp, #16]
	cmp r4, #96
	beq .L_080ec072
	b .L_080ebd76
.L_080ec072:
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	ldr r1, .L_080ec0b8
	ldr r0, [sp, #76]
	movs r7, #0
	mov r8, r7
	adds r6, r0, r1
.L_080ec082:
	ldmia r6!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #9
	bne .L_080ec082
	ldr r0, .L_080ec0dc
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #176
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080ec0b4:
	.4byte 0xffffc000
.L_080ec0b8:
	.4byte 0x000077d8
.L_080ec0bc:
	.4byte Data_080eef56
.L_080ec0c0:
	.4byte Data_080eef5f
.L_080ec0c4:
	.4byte 0x000077a8
.L_080ec0c8:
	.4byte 0x00007828
.L_080ec0cc:
	.4byte gMapCellBuffer
.L_080ec0d0:
	.4byte ParticleStreams_CellOffsets
.L_080ec0d4:
	.4byte 0x00007784
.L_080ec0d8:
	.4byte 0x00007824
.L_080ec0dc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
