.syntax unified
	.thumb
	.global Ui_RunIconMonitor
	.thumb_func
Ui_RunIconMonitor:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	movs r0, #144
	movs r1, #96
	movs r2, #1
	str r0, [sp, #24]
	str r1, [sp, #20]
	movs r3, #0
	movs r1, #160
	movs r0, #9
	mov r10, r3
	str r2, [sp, #16]
	str r2, [sp, #12]
	bl Runtime_AllocateBlock
	ldr r2, .L_080125b8
	str r0, [sp, #8]
	add r4, sp, #28
	mov r0, r10
	movs r3, #3
	strb r3, [r2]
	str r0, [r4]
	ldr r3, .L_080125bc
	adds r0, r4, #0
	ldr r1, [sp, #8]
	ldr r2, .L_080125c0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r1, r10
	str r1, [r4]
	adds r0, r4, #0
	add r1, sp, #32
	ldr r2, .L_080125c4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	movs r1, #1
	negs r0, r0
	bl Ui_FindNextNumberWithMetadata
	add r2, sp, #32
	mov r8, r2
	ldr r1, .L_080125ac
	movs r7, #0
	movs r2, #1
	mov r3, r8
.L_08012580:
	adds r7, #1
	strh r2, [r3, #2]
	strb r1, [r3, #5]
	strh r0, [r3]
	adds r3, #8
	cmp r7, #3
	bls .L_08012580
	movs r3, #1
	mov r0, r8
	ldr r2, .L_080125c8
	strb r3, [r0, #4]
	movs r3, #2
	strb r3, [r2]
	ldr r2, .L_080125cc
	ldr r3, .L_080125b0
	strh r3, [r2]
	ldr r3, .L_080125b4
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	b .L_080125d0
	.2byte 0x0000
.L_080125ac:
	.4byte 0x00000001
.L_080125b0:
	.4byte 0x00003f42
.L_080125b4:
	.4byte 0x000001e0
.L_080125b8:
	.4byte gDecodeFillByte
.L_080125bc:
	.4byte 0x040000d4
.L_080125c0:
	.4byte 0x85000001
.L_080125c4:
	.4byte 0x85000008
.L_080125c8:
	.4byte gDebugMode
.L_080125cc:
	.4byte 0x04000050
.L_080125d0:
	movs r2, #128
	ldr r3, .L_08012610
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #1
	bl Blend_SetDarkenTarget0
.L_080125de:
	bl Runtime_InitializeHeap
	bl Scheduler_ResetTaskTable
	movs r1, #160
	movs r0, #9
	bl Runtime_AllocateBlock
	str r0, [sp, #8]
	bl Resource_InitializeTable
	movs r0, #2
	bl ObjectSystem_Initialize
	mov r0, r8
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r1, .L_08012614
	movs r3, #0
	movs r0, #0
	bl ResourceSlot_Load
	mov r2, r8
	b .L_08012618
	.2byte 0x0000
.L_08012610:
	.4byte 0x00001140
.L_08012614:
	.4byte gMapCellBuffer
.L_08012618:
	movs r1, #0
	ldrsh r0, [r2, r1]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r0, #4]
	cmp r3, #20
	bne .L_08012638
	mov r0, r8
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r1, .L_0801293c
	adds r2, #1
	movs r0, #1
	movs r3, #0
	bl ResourceSlot_Load
.L_08012638:
	movs r7, #0
	mov r6, r8
.L_0801263c:
	movs r1, #0
	ldrsh r0, [r6, r1]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r0, #4]
	movs r5, #0
	cmp r3, #20
	bne .L_08012656
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_08012656
	movs r5, #1
.L_08012656:
	movs r2, #0
	ldrsh r0, [r6, r2]
	lsls r3, r5, #12
	adds r0, r0, r5
	adds r0, r0, r3
	bl ResourceObject_Create
	movs r3, #8
	ldrsh r1, [r6, r3]
	adds r5, r0, #0
	bl ResourceMetadata_Register
	movs r0, #16
	ldrsh r1, [r6, r0]
	adds r0, r5, #0
	bl ResourceMetadata_Register
	adds r0, r5, #0
	movs r2, #24
	ldrsh r1, [r6, r2]
	bl ResourceMetadata_Register
	add r3, sp, #12
	ldrb r3, [r3]
	adds r5, #38
	adds r7, #1
	strb r3, [r5]
	cmp r7, #9
	bls .L_0801263c
	mov r6, r8
	movs r7, #0
	mov r5, r8
	adds r6, #4
	movs r2, #4
.L_0801269a:
	mov r0, r8
	ldrb r3, [r2, r0]
	cmp r3, #0
	beq .L_080126ac
	movs r1, #1
	ldrsb r1, [r6, r1]
	adds r0, r7, #0
	str r2, [sp, #4]
	b .L_080126b2
.L_080126ac:
	adds r0, r7, #0
	movs r1, #8
	str r2, [sp, #4]
.L_080126b2:
	bl Ui_SetGridColumnByte6
	ldr r2, [sp, #4]
	movs r1, #6
	ldrsb r1, [r5, r1]
	adds r0, r7, #0
	str r2, [sp, #4]
	bl Ui_SetGridColumnByte5
	movs r3, #2
	ldrsh r1, [r5, r3]
	adds r0, r7, #0
	bl Ui_FillGridColumnFromMetadata
	ldr r2, [sp, #4]
	adds r7, #1
	adds r5, #8
	adds r6, #8
	adds r2, #8
	cmp r7, #3
	bls .L_0801269a
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	ldr r2, [sp, #8]
	bl Map_BuildProbeRing
	movs r1, #200
	ldr r0, .L_08012940
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_080126f0:
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_08012944
	ldr r1, .L_08012948
	mov r11, r0
	mov r9, r1
.L_080126fe:
	mov r3, r11
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_0801275e
	mov r0, r11
	ldr r2, [r0]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_0801271c
	ldr r1, [sp, #24]
	subs r1, #1
	str r1, [sp, #24]
.L_0801271c:
	mov r3, r11
	ldr r2, [r3]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_0801272e
	ldr r0, [sp, #24]
	adds r0, #1
	str r0, [sp, #24]
.L_0801272e:
	mov r1, r11
	ldr r2, [r1]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_08012740
	ldr r2, [sp, #20]
	subs r2, #1
	str r2, [sp, #20]
.L_08012740:
	mov r3, r11
	ldr r2, [r3]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_08012752
	ldr r0, [sp, #20]
	adds r0, #1
	str r0, [sp, #20]
.L_08012752:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	ldr r2, [sp, #8]
	bl Map_BuildProbeRing
	b .L_080127be
.L_0801275e:
	ldr r1, .L_08012948
	ldr r3, [r1]
	mov r3, r9
	ldr r2, [r3]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_0801277c
	movs r0, #1
	negs r0, r0
	add r10, r0
	mov r2, r10
	movs r3, #3
	ands r2, r3
	mov r10, r2
.L_0801277c:
	ldr r2, [r1]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_08012792
	movs r3, #1
	add r10, r3
	mov r0, r10
	movs r3, #3
	ands r0, r3
	mov r10, r0
.L_08012792:
	ldr r2, [r1]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_080127a6
	ldr r2, [sp, #16]
	movs r3, #3
	subs r2, #1
	ands r2, r3
	str r2, [sp, #16]
.L_080127a6:
	ldr r2, [r1]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_080127be
	ldr r3, [sp, #16]
	adds r3, #1
	str r3, [sp, #16]
	ldr r0, [sp, #16]
	movs r3, #3
	ands r0, r3
	str r0, [sp, #16]
.L_080127be:
	ldr r3, .L_0801294c
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_080127e8
	ldr r3, .L_08012950
	ldr r1, [sp, #12]
	ldr r2, [r3]
	movs r3, #1
	eors r1, r3
	str r1, [sp, #12]
	movs r7, #0
	adds r2, #38
.L_080127da:
	add r3, sp, #12
	ldrb r3, [r3]
	adds r7, #1
	strb r3, [r2]
	adds r2, #56
	cmp r7, #9
	bls .L_080127da
.L_080127e8:
	ldr r0, [sp, #16]
	cmp r0, #1
	beq .L_0801287c
	cmp r0, #1
	bcc .L_08012800
	cmp r0, #2
	bne .L_080127f8
	b .L_08012954
.L_080127f8:
	cmp r0, #3
	bne .L_080127fe
	b .L_080129ca
.L_080127fe:
	b .L_08012a46
.L_08012800:
	mov r1, r10
	cmp r1, #1
	bne .L_08012808
	b .L_08012a46
.L_08012808:
	ldr r1, .L_08012948
	movs r3, #128
	ldr r2, [r1]
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_08012840
	mov r2, r10
	mov r3, r8
	lsls r6, r2, #3
	adds r2, r3, r6
	ldrh r3, [r2, #2]
	subs r3, #1
	strh r3, [r2, #2]
	lsls r3, r3, #16
	movs r1, #0
	cmp r3, #0
	bge .L_0801282e
	strh r1, [r2, #2]
.L_0801282e:
	mov r0, r10
	cmp r0, #0
	beq .L_08012836
	b .L_080126f0
.L_08012836:
	mov r1, r8
	ldrh r3, [r1, #2]
	mov r2, r8
	strh r3, [r2, #10]
	b .L_080126f0
.L_08012840:
	ldr r2, [r1]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_0801284e
	b .L_08012a46
.L_0801284e:
	mov r3, r10
	lsls r6, r3, #3
	mov r0, r8
	adds r2, r0, r6
	ldrh r3, [r2, #2]
	movs r1, #198
	adds r3, #1
	strh r3, [r2, #2]
	lsls r1, r1, #15
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_0801286a
	movs r3, #99
	strh r3, [r2, #2]
.L_0801286a:
	mov r2, r10
	cmp r2, #0
	beq .L_08012872
	b .L_080126f0
.L_08012872:
	mov r0, r8
	ldrh r3, [r0, #2]
	mov r1, r8
	strh r3, [r1, #10]
	b .L_080126f0
.L_0801287c:
	mov r3, r11
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	movs r1, #0
	movs r4, #1
	cmp r2, #0
	beq .L_0801288e
	movs r4, #10
.L_0801288e:
	mov r0, r9
	ldr r2, [r0]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080128c0
	movs r7, #0
	cmp r1, r4
	bcs .L_080128be
	mov r1, r10
	mov r6, r8
	lsls r5, r1, #3
.L_080128a8:
	movs r1, #1
	ldrsh r0, [r6, r5]
	negs r1, r1
	str r4, [sp, #0]
	bl Ui_FindNextNumberWithMetadata
	ldr r4, [sp, #0]
	adds r7, #1
	strh r0, [r6, r5]
	cmp r7, r4
	bcc .L_080128a8
.L_080128be:
	movs r1, #1
.L_080128c0:
	mov r3, r9
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080128f0
	movs r7, #0
	cmp r7, r4
	bcs .L_080128ee
	mov r0, r10
	mov r6, r8
	lsls r5, r0, #3
.L_080128da:
	ldrsh r0, [r6, r5]
	movs r1, #1
	str r4, [sp, #0]
	bl Ui_FindNextNumberWithMetadata
	ldr r4, [sp, #0]
	adds r7, #1
	strh r0, [r6, r5]
	cmp r7, r4
	bcc .L_080128da
.L_080128ee:
	movs r1, #1
.L_080128f0:
	cmp r1, #0
	bne .L_080128f6
	b .L_08012a46
.L_080128f6:
	mov r2, r10
	cmp r2, #0
	bne .L_080128fe
	b .L_080125de
.L_080128fe:
	lsls r6, r2, #3
	adds r5, r6, #4
	mov r0, r8
	ldrb r3, [r0, r5]
	cmp r3, #0
	bne .L_0801290c
	b .L_080126f0
.L_0801290c:
	ldrsh r1, [r0, r6]
	add r5, r8
	mov r0, r10
	bl Ui_SetGridColumnNumber
	movs r1, #1
	ldrsb r1, [r5, r1]
	mov r0, r10
	bl Ui_SetGridColumnByte6
	movs r1, #2
	ldrsb r1, [r5, r1]
	mov r0, r10
	bl Ui_SetGridColumnByte5
	mov r0, r8
	adds r3, r0, r6
	movs r2, #2
	ldrsh r1, [r3, r2]
	mov r0, r10
	bl Ui_FillGridColumnFromMetadata
	b .L_080126f0
	.2byte 0x0000
.L_0801293c:
	.4byte gActorSpriteSlots
.L_08012940:
	.4byte Battle_PlaceActorsByFormationKind
.L_08012944:
	.4byte Data_03001ae8
.L_08012948:
	.4byte gKeysRepeat
.L_0801294c:
	.4byte gKeyState
.L_08012950:
	.4byte gSpriteObjects
.L_08012954:
	mov r3, r9
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	movs r1, #0
	cmp r2, #0
	beq .L_08012980
	mov r0, r10
	lsls r6, r0, #3
	adds r3, r6, #4
	mov r1, r8
	adds r2, r1, r3
	ldrb r3, [r2, #1]
	subs r3, #1
	strb r3, [r2, #1]
	lsls r3, r3, #24
	cmp r3, #0
	bge .L_0801297e
	movs r3, #3
	strb r3, [r2, #1]
.L_0801297e:
	movs r1, #1
.L_08012980:
	mov r3, r9
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080129ae
	mov r0, r10
	lsls r6, r0, #3
	adds r3, r6, #4
	mov r1, r8
	adds r2, r1, r3
	ldrb r3, [r2, #1]
	movs r0, #192
	adds r3, #1
	strb r3, [r2, #1]
	lsls r0, r0, #18
	lsls r3, r3, #24
	movs r1, #0
	cmp r3, r0
	ble .L_080129ac
	strb r1, [r2, #1]
.L_080129ac:
	movs r1, #1
.L_080129ae:
	cmp r1, #0
	beq .L_08012a46
	mov r1, r10
	lsls r6, r1, #3
	adds r2, r6, #4
	mov r0, r8
	ldrb r3, [r0, r2]
	cmp r3, #0
	bne .L_080129c2
	b .L_080126f0
.L_080129c2:
	adds r3, r0, r2
	movs r1, #1
	ldrsb r1, [r3, r1]
	b .L_08012ab0
.L_080129ca:
	mov r3, r9
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	movs r1, #0
	cmp r2, #0
	beq .L_080129f6
	mov r0, r10
	lsls r6, r0, #3
	adds r3, r6, #4
	mov r1, r8
	adds r2, r1, r3
	ldrb r3, [r2, #2]
	subs r3, #1
	strb r3, [r2, #2]
	lsls r3, r3, #24
	cmp r3, #0
	bge .L_080129f4
	movs r3, #15
	strb r3, [r2, #2]
.L_080129f4:
	movs r1, #1
.L_080129f6:
	mov r3, r9
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_08012a24
	mov r0, r10
	lsls r6, r0, #3
	adds r3, r6, #4
	mov r1, r8
	adds r2, r1, r3
	ldrb r3, [r2, #2]
	movs r0, #240
	adds r3, #1
	strb r3, [r2, #2]
	lsls r0, r0, #20
	lsls r3, r3, #24
	movs r1, #0
	cmp r3, r0
	ble .L_08012a22
	strb r1, [r2, #2]
.L_08012a22:
	movs r1, #1
.L_08012a24:
	cmp r1, #0
	beq .L_08012a46
	mov r1, r10
	lsls r6, r1, #3
	adds r2, r6, #4
	mov r0, r8
	ldrb r3, [r0, r2]
	cmp r3, #0
	bne .L_08012a38
	b .L_080126f0
.L_08012a38:
	adds r3, r0, r2
	movs r1, #2
	ldrsb r1, [r3, r1]
	mov r0, r10
	bl Ui_SetGridColumnByte5
	b .L_080126f0
.L_08012a46:
	mov r1, r9
	ldr r2, [r1]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_08012ab8
	ldr r2, [sp, #16]
	cmp r2, #0
	bne .L_08012a8e
	mov r3, r10
	cmp r3, #1
	beq .L_08012ab8
	lsls r6, r3, #3
	adds r3, r6, #4
	mov r0, r8
	ldrb r3, [r0, r3]
	cmp r3, #0
	bne .L_08012a6c
	b .L_080126f0
.L_08012a6c:
	adds r3, r0, r6
	movs r2, #2
	ldrsh r1, [r3, r2]
	mov r0, r10
	bl Ui_FillGridColumnFromMetadata
	mov r3, r10
	cmp r3, #0
	beq .L_08012a80
	b .L_080126f0
.L_08012a80:
	mov r2, r8
	movs r0, #10
	ldrsh r1, [r2, r0]
	movs r0, #1
	bl Ui_FillGridColumnFromMetadata
	b .L_080126f0
.L_08012a8e:
	mov r3, r10
	cmp r3, #0
	beq .L_08012ab8
	lsls r3, r3, #3
	adds r1, r3, #4
	mov r0, r8
	ldrb r2, [r0, r1]
	movs r3, #1
	eors r2, r3
	strb r2, [r0, r1]
	cmp r2, #0
	beq .L_08012aae
	adds r3, r0, r1
	movs r1, #1
	ldrsb r1, [r3, r1]
	b .L_08012ab0
.L_08012aae:
	movs r1, #8
.L_08012ab0:
	mov r0, r10
	bl Ui_SetGridColumnByte6
	b .L_080126f0
.L_08012ab8:
	mov r1, r9
	ldr r2, [r1]
	movs r3, #4
	ands r2, r3
	cmp r2, #0
	beq .L_08012ae2
	bl Scheduler_ResetTaskTable
	mov r2, r11
	ldr r3, [r2]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08012ad8
	ldr r0, .L_08012aec
	b .L_08012ada
.L_08012ad8:
	ldr r0, .L_08012af0
.L_08012ada:
	ldr r1, .L_08012af4
	bl RuntimeDispatch_ReturnZero
	b .L_080125de
.L_08012ae2:
	movs r0, #1
	bl WaitFrames
	b .L_080126fe
	.2byte 0x0000
.L_08012aec:
	.4byte 0x00000011
.L_08012af0:
	.4byte 0x00000012
.L_08012af4:
	.4byte FarCall_ResourceTable
