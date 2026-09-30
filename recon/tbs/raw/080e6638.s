.syntax unified
	.thumb
	.global Func_080e6638
	.thumb_func
Func_080e6638:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080e66c0
	ldmia r3!, {r2}
	ldr r5, .L_080e66c4
	mov r11, r2
	ldr r3, [r3]
	sub sp, #40
	add r5, r11
	adds r6, r0, #0
	movs r0, #128
	str r3, [sp, #20]
	lsls r0, r0, #6
	str r6, [r5]
	bl BattleFx_BeginCanvasLayer
	ldr r3, [r5]
	ldr r2, [r3, #4]
	add r3, sp, #36
	str r3, [sp, #0]
	add r3, sp, #32
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r1, #6
	movs r3, #2
	bl BattleFx_PrepareCanvasEffect
	ldr r2, .L_080e66c8
	ldr r3, .L_080e66b4
	strh r3, [r2]
	ldr r3, .L_080e66b8
	adds r2, #70
	strh r3, [r2]
	ldr r3, .L_080e66bc
	subs r2, #50
	strh r3, [r2]
	ldr r3, [r5]
	add r1, sp, #24
	ldr r0, [r3, #4]
	bl BattleFx_FetchRectangleBlitters
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e66cc
	movs r3, #75
	add r2, r11
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080e66d0
	bl Scheduler_AddOrUpdateCallback
	movs r3, #254
	b .L_080e66d4
.L_080e66b4:
	.4byte 0x00002784
.L_080e66b8:
	.4byte 0x00001000
.L_080e66bc:
	.4byte 0x000000aa
.L_080e66c0:
	.4byte gBattleFxWork
.L_080e66c4:
	.4byte 0x00007828
.L_080e66c8:
	.4byte 0x0400000c
.L_080e66cc:
	.4byte 0x00007784
.L_080e66d0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e66d4:
	lsls r3, r3, #6
	movs r1, #0
	mov r9, r3
	mov r5, r11
	add r5, r9
	str r1, [sp, #16]
	mov r10, r5
.L_080e66e2:
	ldr r3, [sp, #16]
	movs r2, #127
	add r3, r11
	add r2, r10
	mov r7, r9
	adds r6, r3, #0
	movs r4, #0
	mov r8, r2
	add r7, r11
	adds r6, #127
	adds r5, r3, #0
.L_080e66f8:
	adds r2, r1, #0
	cmp r1, #0
	bge .L_080e6700
	adds r2, r1, #7
.L_080e6700:
	asrs r2, r2, #3
	adds r3, r4, #0
	adds r2, #64
	subs r2, r1, r2
	subs r3, #64
	adds r0, r3, #0
	muls r0, r3
	adds r3, r2, #0
	muls r3, r2
	str r1, [sp, #12]
	adds r0, r0, r3
	str r4, [sp, #8]
	ldr r3, .L_080e6810
	bl _call_via_r3
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r0, r3, #1
	ldr r1, [sp, #12]
	ldr r4, [sp, #8]
	cmp r0, #0
	bne .L_080e672e
	movs r0, #1
.L_080e672e:
	cmp r0, #63
	ble .L_080e6734
	movs r0, #63
.L_080e6734:
	movs r2, #1
	mov r3, r8
	negs r2, r2
	adds r4, #1
	strb r0, [r5]
	add r8, r2
	strb r0, [r6]
	adds r5, #1
	strb r0, [r7]
	subs r6, #1
	adds r7, #1
	strb r0, [r3]
	cmp r4, #64
	bne .L_080e66f8
	ldr r3, [sp, #16]
	movs r5, #128
	negs r5, r5
	adds r3, #128
	adds r1, #1
	str r3, [sp, #16]
	add r9, r5
	add r10, r5
	cmp r1, #64
	bne .L_080e66e2
	ldr r4, .L_080e6814
	movs r7, #1
.L_080e6768:
	cmp r7, #31
	ble .L_080e6772
	movs r3, #64
	subs r2, r3, r7
	b .L_080e6774
.L_080e6772:
	adds r2, r7, #0
.L_080e6774:
	lsls r3, r2, #3
	adds r0, r3, r2
	subs r3, r3, r2
	adds r1, r3, #0
	adds r2, r3, #0
	subs r1, #42
	subs r2, #56
	cmp r0, #0
	bge .L_080e6788
	movs r0, #0
.L_080e6788:
	cmp r1, #0
	bge .L_080e678e
	movs r1, #0
.L_080e678e:
	cmp r2, #0
	bge .L_080e6794
	movs r2, #0
.L_080e6794:
	cmp r0, #255
	ble .L_080e679a
	movs r0, #255
.L_080e679a:
	cmp r1, #255
	ble .L_080e67a0
	movs r1, #255
.L_080e67a0:
	cmp r2, #250
	ble .L_080e67a6
	movs r2, #250
.L_080e67a6:
	asrs r1, r1, #3
	asrs r2, r2, #3
	movs r5, #160
	lsls r2, r2, #10
	lsls r1, r1, #5
	lsls r3, r7, #1
	asrs r0, r0, #3
	lsls r5, r5, #19
	orrs r2, r1
	orrs r2, r0
	adds r3, r3, r5
	adds r7, #1
	strh r2, [r3]
	strh r2, [r4]
	adds r4, #2
	cmp r7, #64
	bne .L_080e6768
	movs r3, #128
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #24]
	ldr r0, [sp, #20]
	mov r1, r11
	movs r2, #0
	movs r3, #0
	bl _call_via_r4
	ldr r2, .L_080e6818
	movs r3, #1
	add r2, r11
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e681c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080e6820
	ldr r3, .L_080e6824
	movs r4, #0
	mov r9, r2
	mov r10, r3
	mov r8, r4
.L_080e67fa:
	cmp r4, #8
	bgt .L_080e682c
	ldr r2, .L_080e680c
	mov r3, r8
	ldr r5, .L_080e6828
	orrs r3, r2
	mov r1, r8
	strh r3, [r5]
	b .L_080e682e
.L_080e680c:
	.4byte 0x00001000
.L_080e6810:
	.4byte IwramSqrt
.L_080e6814:
	.4byte Data_02010002
.L_080e6818:
	.4byte 0x00007824
.L_080e681c:
	.4byte BattleFx_ArmBg2AffineHBlankDma
.L_080e6820:
	.4byte Data_0201007e
.L_080e6824:
	.4byte 0x04000208
.L_080e6828:
	.4byte 0x04000052
.L_080e682c:
	lsls r1, r4, #1
.L_080e682e:
	cmp r4, #88
	ble .L_080e6840
	ldr r3, .L_080e6850
	mov r2, r8
	subs r3, r3, r2
	ldr r2, .L_080e6854
	ldr r5, .L_080e6858
	orrs r3, r2
	strh r3, [r5]
.L_080e6840:
	movs r6, #211
	lsls r6, r6, #7
	lsls r3, r1, #9
	add r6, r11
	movs r7, #0
	negs r5, r3
	b .L_080e685c
	.2byte 0x0000
.L_080e6850:
	.4byte 0x000000c0
.L_080e6854:
	.4byte 0x00001000
.L_080e6858:
	.4byte 0x04000052
.L_080e685c:
	adds r0, r5, #0
	str r4, [sp, #8]
	bl Trig_Sin
	lsls r3, r7, #18
	lsls r0, r0, #7
	movs r2, #128
	subs r3, r3, r0
	lsls r2, r2, #11
	adds r3, r3, r2
	asrs r3, r3, #10
	stmia r6!, {r3}
	movs r3, #128
	lsls r3, r3, #2
	adds r7, #1
	adds r5, r5, r3
	ldr r4, [sp, #8]
	cmp r7, #160
	bne .L_080e685c
	cmp r4, #127
	ble .L_080e6890
	ldr r2, .L_080e691c
	movs r3, #1
	add r2, r11
	str r3, [r2]
	b .L_080e68d6
.L_080e6890:
	mov r5, r9
	ldrh r3, [r5]
	ldr r2, .L_080e6920
	ldr r0, .L_080e6924
	strh r3, [r2, #2]
	mov r1, r9
	ldr r3, .L_080e6928
	ldr r2, .L_080e692c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_080e6930
	mov r5, r10
	ldrh r3, [r5]
	adds r0, r3, #0
	mov r2, r10
	mov r3, r10
	strh r2, [r3]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080e68d2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_080e6934
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_080e6938
	stmia r3!, {r2}
	ldr r2, .L_080e693c
	str r2, [r3]
.L_080e68d2:
	mov r5, r10
	strh r0, [r5]
.L_080e68d6:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
	ldr r4, [sp, #8]
	movs r2, #2
	adds r4, #1
	add r8, r2
	cmp r4, #96
	beq .L_080e68ec
	b .L_080e67fa
.L_080e68ec:
	ldr r0, .L_080e6940
	bl Scheduler_RemoveCallback
	ldr r0, .L_080e6944
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080e691c:
	.4byte 0x00007824
.L_080e6920:
	.4byte gMapCellBuffer
.L_080e6924:
	.4byte Data_0201007c
.L_080e6928:
	.4byte 0x040000d4
.L_080e692c:
	.4byte 0x80a0003e
.L_080e6930:
	.4byte gIoWriteQueue
.L_080e6934:
	.4byte Data_02010002
.L_080e6938:
	.4byte 0x05000002
.L_080e693c:
	.4byte 0x8000003f
.L_080e6940:
	.4byte BattleFx_ArmBg2AffineHBlankDma
.L_080e6944:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
