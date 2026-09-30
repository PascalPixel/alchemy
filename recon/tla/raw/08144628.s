.syntax unified
	.thumb
	.global Func_08144628
	.thumb_func
Func_08144628:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	str r1, [sp, #68]
	movs r3, #192
	lsls r3, r3, #18
	mov r9, r0
	ldr r0, [r3, #92]
	str r0, [sp, #64]
	movs r0, #1
	ldr r1, [r3, #96]
	str r1, [sp, #60]
	ldr r3, [r3, #48]
	str r3, [sp, #52]
	bl BattleFx_BeginCanvasLayer
	ldr r2, [sp, #64]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08144994
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, [sp, #68]
	cmp r0, #0
	beq .L_0814466e
	cmp r0, #3
	bne .L_08144672
.L_0814466e:
	ldr r0, .L_08144998
	b .L_0814467e
.L_08144672:
	ldr r1, [sp, #68]
	cmp r1, #1
	bne .L_0814467c
	ldr r0, .L_0814499c
	b .L_0814467e
.L_0814467c:
	ldr r0, .L_081449a0
.L_0814467e:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_081449a4
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #68]
	cmp r2, #3
	bne .L_0814469a
	bl Func_0815b634
.L_0814469a:
	mov r3, r9
	ldr r0, [r3, #4]
	add r1, sp, #72
	bl Func_08144aac
	ldr r3, .L_081449a8
	movs r1, #1
	movs r2, #128
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #3
.L_081446b0:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_081446b0
	movs r0, #0
	str r0, [sp, #56]
	mov r1, r9
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_0814473c
	movs r2, #255
	mov r8, r2
	mov r11, r0
.L_081446cc:
	mov r3, r9
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	mov r1, r9
	ldr r6, [r0]
	ldr r0, [r1, #8]
	bl Func_08118070
	ldr r5, .L_081449ac
	mov r10, r0
	movs r7, #0
	add r5, r11
.L_081446e6:
	ldr r3, [r6, #8]
	mov r2, r10
	str r2, [r5, #4]
	str r3, [r5]
	adds r7, #1
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	mov r3, r8
	ands r0, r3
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	mov r1, r8
	ands r0, r1
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	mov r2, r8
	ands r0, r2
	subs r0, #128
	lsls r0, r0, #10
	movs r3, #0
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #128
	bne .L_081446e6
	ldr r0, [sp, #56]
	movs r3, #224
	adds r0, #1
	str r0, [sp, #56]
	lsls r3, r3, #4
	mov r1, r9
	add r11, r3
	ldr r3, [r1, #20]
	cmp r0, r3
	bne .L_081446cc
.L_0814473c:
	movs r5, #200
	lsls r5, r5, #4
	adds r1, r5, #0
	ldr r0, .L_081449b0
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #64]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #64]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #75
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_081449b4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #146
	bl Audio_PlayCue
	ldr r1, [sp, #68]
	movs r0, #0
	lsls r1, r1, #1
	str r1, [sp, #36]
	mov r11, r0
	ldr r0, .L_081449b8
	adds r3, r1, #0
	adds r3, #1
	ldrb r1, [r0, r3]
	mov r3, r9
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	cmn r1, r3
	bne .L_08144792
	b .L_08144a74
.L_08144792:
	ldr r1, [sp, #52]
	adds r1, #12
	str r1, [sp, #32]
.L_08144798:
	mov r2, r11
	cmp r2, #80
	bne .L_081447b8
	ldr r3, [sp, #68]
	cmp r3, #0
	beq .L_081447aa
	ldr r0, [sp, #68]
	cmp r0, #3
	bne .L_081447b2
.L_081447aa:
	movs r0, #134
	bl Func_081180e8
	b .L_081447b8
.L_081447b2:
	movs r0, #133
	bl Func_081180e8
.L_081447b8:
	bl Func_08014de4
	ldr r0, [sp, #52]
	ldr r1, [sp, #32]
	bl Func_080156e8
	ldr r1, [sp, #64]
	mov r3, r11
	movs r2, #225
	lsls r2, r2, #7
	lsls r0, r3, #15
	ldr r3, [sp, #36]
	adds r6, r1, r2
	ldr r2, .L_081449b8
	adds r3, #1
	ldrb r1, [r2, r3]
	mov r3, r9
	ldr r2, [r3, #20]
	movs r7, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r1, r1, r3
	bl Math_Div
	bl Trig_Sin
	ldr r3, .L_081449bc
	lsls r0, r0, #4
	ldrh r1, [r3, #4]
	asrs r4, r0, #16
	mov r0, r11
	lsls r5, r0, #10
.L_081447fa:
	adds r0, r5, #0
	str r1, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Sin
	ldr r4, [sp, #8]
	ldr r1, [sp, #12]
	adds r3, r4, #0
	muls r3, r0
	asrs r3, r3, #16
	adds r2, r3, r1
	cmp r2, #0
	bge .L_0814481c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_0814481c:
	movs r3, #128
	lsls r3, r3, #14
	movs r0, #128
	orrs r2, r3
	lsls r0, r0, #3
	adds r7, #1
	stmia r6!, {r2}
	adds r5, r5, r0
	cmp r7, #160
	bne .L_081447fa
	movs r1, #0
	str r1, [sp, #56]
	mov r2, r9
	ldr r3, [r2, #20]
	cmp r3, #0
	bne .L_0814483e
	b .L_08144a3e
.L_0814483e:
	movs r3, #36
	str r3, [sp, #28]
	str r1, [sp, #24]
	str r1, [sp, #20]
	str r1, [sp, #16]
.L_08144848:
	ldr r1, [sp, #28]
	mov r3, r9
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r1, [sp, #28]
	mov r3, r9
	mov r10, r0
	ldrsh r0, [r1, r3]
	bl Func_08118070
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [sp, #20]
	asrs r0, r0, #1
	adds r3, #71
	str r0, [sp, #48]
	cmp r11, r3
	bne .L_0814488a
	ldr r0, [sp, #68]
	cmp r0, #0
	bne .L_0814487e
	movs r0, #134
	bl Audio_PlayCue
	b .L_0814488a
.L_0814487e:
	ldr r1, [sp, #68]
	cmp r1, #3
	beq .L_0814488a
	movs r0, #133
	bl Audio_PlayCue
.L_0814488a:
	ldr r3, [sp, #20]
	adds r3, #70
	cmp r11, r3
	bne .L_081448a6
	ldr r2, [sp, #28]
	mov r1, r9
	ldrsh r0, [r2, r1]
	movs r3, #26
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #56]
	bl Func_0814cd48
.L_081448a6:
	ldr r2, [sp, #20]
	cmp r11, r2
	bgt .L_081448ae
	b .L_08144a12
.L_081448ae:
	ldr r2, .L_081449b8
	ldr r0, [sp, #36]
	movs r7, #0
	ldrb r3, [r2, r0]
	cmp r3, #0
	bne .L_081448bc
	b .L_08144a12
.L_081448bc:
	ldr r1, [sp, #24]
	movs r3, #80
	ldr r0, [sp, #20]
	add r3, sp
	str r1, [sp, #44]
	mov r8, r3
	ldr r1, [sp, #16]
	ldr r3, .L_081449ac
	str r0, [sp, #40]
	adds r6, r1, r3
.L_081448d0:
	ldr r0, [sp, #44]
	lsls r3, r0, #1
	adds r3, r3, r7
	lsls r3, r3, #1
	cmp r11, r3
	bgt .L_081448de
	b .L_08144a04
.L_081448de:
	ldr r3, [r6, #24]
	cmp r3, #0
	bge .L_081448e6
	b .L_08144a04
.L_081448e6:
	mov r1, r8
	adds r0, r6, #0
	bl Func_0815e1ec
	mov r1, r8
	ldr r5, [r1]
	adds r0, r7, #0
	asrs r5, r5, #1
	str r5, [r1]
	movs r1, #3
	bl __modsi3
	ldr r2, [sp, #64]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #7
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	mov r0, r8
	adds r1, r1, r3
	ldr r3, [r0, #4]
	movs r2, #20
	str r2, [sp, #0]
	subs r5, #10
	movs r2, #32
	subs r3, #16
	str r2, [sp, #4]
	ldr r4, [sp, #72]
	ldr r0, [sp, #60]
	adds r2, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r1, #62
	adds r0, r6, #0
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	ldr r1, [sp, #40]
	adds r3, r1, r7
	adds r3, #30
	cmp r11, r3
	ble .L_08144a02
	mov r2, r10
	ldr r0, [r2, #8]
	ldr r3, [sp, #48]
	ldr r2, [r2, #12]
	ldr r4, [r6]
	adds r2, r2, r3
	ldr r3, [r6, #4]
	subs r0, r0, r4
	subs r2, r2, r3
	mov r3, r10
	ldr r1, [r3, #16]
	ldr r3, [r6, #8]
	asrs r0, r0, #8
	subs r1, r1, r3
	ldr r3, [r6, #12]
	asrs r2, r2, #9
	adds r3, r3, r0
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	asrs r1, r1, #8
	adds r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #20]
	mov r0, r10
	adds r3, r3, r1
	str r3, [r6, #20]
	ldr r3, [r0, #8]
	adds r1, r3, #0
	cmp r3, #0
	bge .L_0814497a
	negs r1, r3
.L_0814497a:
	adds r2, r4, #0
	cmp r2, #0
	bge .L_08144982
	negs r2, r2
.L_08144982:
	cmp r1, r2
	bgt .L_08144a02
	cmp r4, #0
	bge .L_081449c0
	lsrs r3, r3, #31
	cmp r3, #0
	bne .L_081449c8
	b .L_08144a02
	.2byte 0x0000
.L_08144994:
	.4byte 0x0000012a
.L_08144998:
	.4byte 0x0000017f
.L_0814499c:
	.4byte 0x00000150
.L_081449a0:
	.4byte 0x00000154
.L_081449a4:
	.4byte IwramCopyWords
.L_081449a8:
	.4byte Data_02010018
.L_081449ac:
	.4byte gMapCellBuffer
.L_081449b0:
	.4byte Func_081524b4
.L_081449b4:
	.4byte Func_08143000
.L_081449b8:
	.4byte Data_08197864
.L_081449bc:
	.4byte Data_03001120
.L_081449c0:
	mvns r3, r3
	lsrs r3, r3, #31
	cmp r3, #0
	beq .L_08144a02
.L_081449c8:
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
	ldr r1, [sp, #68]
	cmp r1, #3
	bne .L_08144a02
	ldr r2, [sp, #28]
	mov r1, r9
	ldrsh r0, [r2, r1]
	movs r3, #128
	lsls r2, r7, #3
	lsls r3, r3, #10
	adds r2, #100
	str r2, [sp, #4]
	movs r1, #1
	adds r2, r3, #0
	str r3, [sp, #0]
	bl Func_0815f000
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_08144a02
	mov r2, r11
	cmp r2, #80
	beq .L_08144a02
	movs r0, #134
	bl Audio_PlayCue
.L_08144a02:
	ldr r2, .L_08144aa0
.L_08144a04:
	ldr r0, [sp, #36]
	adds r7, #1
	ldrb r3, [r2, r0]
	adds r6, #28
	cmp r7, r3
	beq .L_08144a12
	b .L_081448d0
.L_08144a12:
	ldr r2, [sp, #24]
	ldr r1, [sp, #28]
	adds r2, #5
	adds r1, #2
	str r2, [sp, #24]
	ldr r3, [sp, #20]
	ldr r0, [sp, #16]
	ldr r2, [sp, #56]
	str r1, [sp, #28]
	movs r1, #224
	lsls r1, r1, #4
	adds r3, #20
	adds r0, r0, r1
	adds r2, #1
	str r0, [sp, #16]
	str r3, [sp, #20]
	str r2, [sp, #56]
	mov r0, r9
	ldr r3, [r0, #20]
	cmp r2, r3
	beq .L_08144a3e
	b .L_08144848
.L_08144a3e:
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #64]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #36]
	ldr r2, .L_08144aa0
	adds r3, #1
	ldrb r1, [r2, r3]
	mov r3, r9
	ldr r2, [r3, #20]
	movs r0, #1
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	add r11, r0
	adds r1, r1, r3
	cmp r11, r1
	beq .L_08144a74
	b .L_08144798
.L_08144a74:
	ldr r0, .L_08144aa4
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08144aa8
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08144aa0:
	.4byte Data_08197864
.L_08144aa4:
	.4byte Func_08143000
.L_08144aa8:
	.4byte Func_081524b4
