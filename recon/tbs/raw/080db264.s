.syntax unified
	.thumb
	.global Unnamed_080db264
	.thumb_func
Unnamed_080db264:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080db2a4
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #44
	str r3, [sp, #32]
	ldr r5, .L_080db2a8
	mov r10, r1
	ldr r2, [r2, #8]
	add r5, r10
	str r2, [sp, #24]
	str r0, [r5]
	movs r0, #1
	bl BattleFx_BeginTiledCanvas
	ldr r3, [r5]
	ldr r3, [r3, #24]
	cmp r3, #2
	bne .L_080db2b0
	ldr r2, .L_080db2ac
	ldr r3, .L_080db2a0
	b .L_080db2b4
	.2byte 0x0000
.L_080db2a0:
	.4byte 0x00000080
.L_080db2a4:
	.4byte gBattleFxWork
.L_080db2a8:
	.4byte 0x00007828
.L_080db2ac:
	.4byte 0x04000020
.L_080db2b0:
	ldr r2, .L_080db2f0
	ldr r3, .L_080db2ec
.L_080db2b4:
	strh r3, [r2]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r5, .L_080db2f4
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	str r3, [sp, #36]
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl Unnamed_080ed408
	adds r5, #188
	ldr r3, [r5]
	mov r2, sp
	ldr r1, .L_080db2f8
	adds r2, #36
	b .L_080db2fc
.L_080db2ec:
	.4byte 0x00000100
.L_080db2f0:
	.4byte 0x04000020
.L_080db2f4:
	.4byte gWorkSlot
.L_080db2f8:
	.4byte 0x0000060e
.L_080db2fc:
	str r2, [sp, #12]
	ldr r0, .L_080db670
	str r3, [r2, #4]
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #0
	movs r3, #0
	ldr r0, .L_080db674
	ldr r1, [sp, #24]
	bl Resource_LoadAndDecompress
	ldr r3, .L_080db678
	add r3, r10
	ldr r2, [r3]
	ldr r3, [r2, #24]
	cmp r3, #2
	bne .L_080db338
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080db330
	ldr r2, .L_080db67c
	ldr r3, .L_080db680
	b .L_080db342
.L_080db330:
	ldr r2, .L_080db67c
	movs r3, #128
	lsls r3, r3, #5
	b .L_080db342
.L_080db338:
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_080db344
	ldr r2, .L_080db67c
	ldr r3, .L_080db684
.L_080db342:
	str r3, [r2]
.L_080db344:
	movs r3, #0
	ldr r7, .L_080db688
	mov r8, r3
.L_080db34a:
	bl Random16
	ldr r6, .L_080db68c
	movs r1, #128
	lsls r1, r1, #1
	ands r6, r0
	adds r6, r6, r1
	bl Random16
	ldr r5, .L_080db690
	movs r3, #128
	ldr r2, .L_080db694
	lsls r3, r3, #7
	str r3, [r7]
	ands r5, r0
	movs r3, #224
	adds r5, r5, r2
	lsls r3, r3, #7
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #16
	str r3, [r7, #20]
	movs r3, #0
	str r3, [r7, #24]
	movs r1, #128
	movs r3, #1
	add r8, r3
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne .L_080db34a
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080db698
	movs r3, #75
	add r2, r10
	str r3, [r2]
	adds r1, #128
	ldr r0, .L_080db69c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #138
	bl AudioCommand_PlayFar
	movs r2, #0
	str r2, [sp, #28]
.L_080db3c6:
	ldr r3, [sp, #28]
	cmp r3, #20
	bne .L_080db3d2
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080db3d2:
	ldr r1, [sp, #28]
	cmp r1, #15
	ble .L_080db3da
	b .L_080db55a
.L_080db3da:
	adds r0, r1, #0
	movs r1, #5
	bl __modsi3
	cmp r0, #2
	bne .L_080db3f4
	movs r1, #128
	ldr r3, .L_080db6a0
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_080db6a4
	bl _call_via_r3
.L_080db3f4:
	ldr r3, [sp, #28]
	movs r2, #0
	lsls r3, r3, #11
	str r2, [sp, #20]
	str r3, [sp, #8]
.L_080db3fe:
	movs r1, #0
	ldr r2, [sp, #8]
	str r1, [sp, #16]
	movs r1, #128
	lsls r1, r1, #7
	adds r3, r2, r1
	ldr r2, [sp, #20]
	adds r5, r2, #0
	muls r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, [sp, #28]
	movs r3, #32
	subs r3, r3, r1
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #64
	adds r0, r5, #0
	mov r9, r3
	bl Trig_Cos
	ldr r3, .L_080db678
	add r3, r10
	lsls r0, r0, #3
	ldr r3, [r3]
	asrs r0, r0, #16
	negs r0, r0
	ldr r3, [r3, #24]
	adds r6, r0, #0
	subs r6, #8
	cmp r3, #0
	bne .L_080db47a
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r5, r3, #4
	subs r5, r5, r3
	ldr r2, .L_080db6a8
	lsls r5, r5, #6
	add r5, r10
	adds r5, r5, r2
	bl Random16
	movs r2, #7
	ands r2, r0
	movs r3, #24
	add r2, r9
	str r3, [sp, #0]
	movs r3, #120
	str r3, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	adds r1, r5, #0
	adds r3, r6, #0
	bl _call_via_r4
	b .L_080db4bc
.L_080db47a:
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r5, r3, #4
	subs r5, r5, r3
	lsls r5, r5, #6
	ldr r3, .L_080db6a8
	add r5, r10
	adds r5, r5, r3
	bl Random16
	ldr r3, [sp, #20]
	movs r1, #1
	ands r1, r3
	movs r3, #24
	str r3, [sp, #0]
	movs r2, #7
	movs r3, #120
	ands r2, r0
	str r3, [sp, #4]
	ldr r3, [sp, #12]
	lsls r1, r1, #2
	add r2, r9
	ldr r4, [r1, r3]
	subs r2, #16
	ldr r0, [sp, #32]
	adds r1, r5, #0
	adds r3, r6, #0
	bl _call_via_r4
.L_080db4bc:
	adds r3, r6, #0
	adds r3, #112
	movs r1, #0
	lsls r3, r3, #16
	ldr r7, .L_080db688
	mov r8, r1
	mov r11, r3
.L_080db4ca:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080db538
	bl Random16
	ldr r6, .L_080db6ac
	ands r6, r0
	bl Random16
	ldr r5, .L_080db690
	ldr r2, .L_080db694
	mov r1, r9
	ands r5, r0
	lsls r3, r1, #16
	adds r5, r5, r2
	mov r2, r11
	str r2, [r7, #4]
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #128
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	ldr r3, [sp, #16]
	adds r3, #1
	str r3, [sp, #16]
	ldr r3, .L_080db678
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #24]
	ldr r1, .L_080db6b0
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r1, r3]
	ldr r2, [sp, #16]
	cmp r2, r3
	beq .L_080db546
.L_080db538:
	movs r3, #1
	movs r1, #128
	add r8, r3
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne .L_080db4ca
.L_080db546:
	ldr r2, [sp, #20]
	adds r2, #1
	str r2, [sp, #20]
	cmp r2, #4
	beq .L_080db552
	b .L_080db3fe
.L_080db552:
	ldr r2, .L_080db6b4
	movs r3, #1
	add r2, r10
	str r3, [r2]
.L_080db55a:
	movs r3, #0
	ldr r5, .L_080db688
	mov r8, r3
.L_080db560:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_080db5ca
	subs r3, #1
	str r3, [r5, #24]
	movs r1, #60
	adds r0, r5, #0
	ldr r2, .L_080db6b8
	bl EffectStep_AdvanceWithGravity2D
	movs r1, #240
	ldr r3, [r5, #4]
	lsls r1, r1, #15
	cmp r3, r1
	ble .L_080db58c
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_080db5ca
.L_080db58c:
	ldr r2, [r5]
	ldr r1, .L_080db6bc
	cmp r2, r1
	bhi .L_080db5ca
	cmp r3, #0
	blt .L_080db5ca
	ldr r0, [r5, #24]
	asrs r6, r2, #16
	asrs r7, r3, #16
	cmp r0, #0
	bge .L_080db5a4
	adds r0, #7
.L_080db5a4:
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, .L_080db6c0
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #24]
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r3, r7, r0
	str r0, [sp, #0]
	str r4, [sp, #4]
	subs r2, r6, r2
	ldr r4, [sp, #36]
	ldr r0, [sp, #32]
	bl _call_via_r4
.L_080db5ca:
	movs r3, #1
	movs r1, #128
	add r8, r3
	lsls r1, r1, #3
	adds r5, #28
	cmp r8, r1
	bne .L_080db560
	ldr r3, [sp, #28]
	subs r3, #4
	cmp r3, #91
	bhi .L_080db622
	movs r2, #0
	mov r8, r2
	ldr r2, .L_080db678
	mov r1, r10
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080db622
	movs r6, #36
	movs r5, #4
.L_080db5f4:
	ldr r3, [sp, #28]
	cmp r3, r5
	bne .L_080db60e
	mov r1, r10
	ldr r3, [r1, r2]
	ldrsh r0, [r3, r6]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
.L_080db60e:
	ldr r2, .L_080db678
	movs r3, #1
	mov r1, r10
	add r8, r3
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	adds r6, #2
	adds r5, #4
	cmp r8, r3
	bne .L_080db5f4
.L_080db622:
	movs r0, #2
	movs r1, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080db6c4
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	adds r2, #1
	str r2, [sp, #28]
	cmp r2, #64
	beq .L_080db648
	b .L_080db3c6
.L_080db648:
	ldr r0, .L_080db69c
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080db670:
	.4byte 0x000000c4
.L_080db674:
	.4byte 0x00000073
.L_080db678:
	.4byte 0x00007828
.L_080db67c:
	.4byte 0x04000028
.L_080db680:
	.4byte 0xfffff000
.L_080db684:
	.4byte 0xffff8000
.L_080db688:
	.4byte gMapCellBuffer
.L_080db68c:
	.4byte 0x000003ff
.L_080db690:
	.4byte 0x00007fff
.L_080db694:
	.4byte 0xffffc000
.L_080db698:
	.4byte 0x00007784
.L_080db69c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080db6a0:
	.4byte IwramFillWords
.L_080db6a4:
	.4byte 0x10101010
.L_080db6a8:
	.4byte 0x0000060e
.L_080db6ac:
	.4byte 0x000001ff
.L_080db6b0:
	.4byte Data_080eeadc
.L_080db6b4:
	.4byte 0x000077a8
.L_080db6b8:
	.4byte 0xfffff800
.L_080db6bc:
	.4byte 0x007effff
.L_080db6c0:
	.4byte ParticleStreams_CellOffsets
.L_080db6c4:
	.4byte 0x00007824
