.syntax unified
	.thumb
	.global Func_0813f518
	.thumb_func
Func_0813f518:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #246
	sub sp, #40
	lsls r1, r1, #7
	str r0, [sp, #24]
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	mov r11, r0
	lsls r1, r1, #7
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	movs r1, #192
	lsls r1, r1, #3
	str r0, [sp, #20]
	adds r1, #14
	movs r0, #100
	bl Runtime_AllocateHeapBlock
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	add r2, r11
	movs r3, #1
	str r0, [sp, #8]
	str r3, [r2]
	movs r3, #240
	ldr r0, [sp, #24]
	lsls r3, r3, #7
	adds r3, #240
	add r3, r11
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #180
	add r2, r11
	movs r3, #24
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #184
	add r2, r11
	movs r3, #0
	str r3, [r2]
	ldr r3, .L_0813f5c0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0813f5c4
	movs r1, #224
	subs r2, #50
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0813f5c8
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r1, [sp, #8]
	ldr r0, .L_0813f5cc
	movs r2, #0
	bl Resource_LoadAndDecompress
	ldr r1, [sp, #24]
	ldr r3, [r1]
	cmp r3, #1
	beq .L_0813f5e4
	b .L_0813f5d0
	.2byte 0x0000
.L_0813f5c0:
	.4byte 0x0000100c
.L_0813f5c4:
	.4byte 0x00000100
.L_0813f5c8:
	.4byte 0x00000107
.L_0813f5cc:
	.4byte 0x00000137
.L_0813f5d0:
	cmp r3, #1
	bgt .L_0813f5da
	cmp r3, #0
	beq .L_0813f5e0
	b .L_0813f5f8
.L_0813f5da:
	cmp r3, #2
	beq .L_0813f5e8
	b .L_0813f5f8
.L_0813f5e0:
	ldr r0, .L_0813f5ec
	b .L_0813f5fa
.L_0813f5e4:
	ldr r0, .L_0813f5f0
	b .L_0813f5fa
.L_0813f5e8:
	ldr r0, .L_0813f5f4
	b .L_0813f5fa
.L_0813f5ec:
	.4byte 0x0000010a
.L_0813f5f0:
	.4byte 0x00000119
.L_0813f5f4:
	.4byte 0x00000109
.L_0813f5f8:
	ldr r0, .L_0813f714
.L_0813f5fa:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0813f718
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	ldr r5, .L_0813f71c
	movs r7, #0
.L_0813f610:
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	movs r2, #128
	ands r3, r0
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r5, #8]
	negs r3, r7
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #128
	bne .L_0813f610
	movs r7, #0
	mov r5, r11
.L_0813f648:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #16
	str r3, [r5, #4]
	movs r3, #15
	ands r3, r7
	adds r3, #16
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne .L_0813f648
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0813f720
	bl Scheduler_AddOrUpdateCallback
	movs r1, #7
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r0, #140
	str r3, [sp, #12]
	bl Audio_PlayCue
	movs r4, #28
	movs r3, #0
	add r4, sp
	mov r10, r3
	mov r9, r4
.L_0813f6b2:
	ldr r5, [sp, #24]
	mov r1, r9
	ldr r0, [r5, #8]
	bl Func_0815e20c
	mov r0, r9
	ldr r2, [r0]
	movs r1, #128
	movs r3, #64
	lsls r1, r1, #19
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	mov r1, r10
	cmp r1, #49
	ble .L_0813f6e8
	mov r3, r10
	lsls r2, r3, #1
	ldr r3, .L_0813f70c
	movs r1, #128
	subs r3, r3, r2
	ldr r2, .L_0813f710
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
.L_0813f6e8:
	mov r4, r10
	cmp r4, #26
	bne .L_0813f724
	movs r0, #212
	bl Audio_PlayCue
	ldr r1, [sp, #24]
	movs r3, #20
	movs r2, #1
	movs r5, #36
	ldrsh r0, [r1, r5]
	negs r2, r2
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	b .L_0813f724
.L_0813f70c:
	.4byte 0x00000070
.L_0813f710:
	.4byte 0x00001000
.L_0813f714:
	.4byte 0x00000108
.L_0813f718:
	.4byte IwramCopyWords
.L_0813f71c:
	.4byte gMapCellBuffer
.L_0813f720:
	.4byte Func_08143000
.L_0813f724:
	mov r0, r10
	subs r0, #28
	cmp r0, #20
	bhi .L_0813f756
	movs r1, #3
	bl Math_Div
	lsls r1, r0, #3
	adds r1, r1, r0
	mov r4, r9
	ldr r3, [r4, #4]
	lsls r1, r1, #8
	movs r2, #216
	lsls r2, r2, #5
	add r1, r11
	adds r1, r1, r2
	movs r2, #48
	str r2, [sp, #0]
	str r2, [sp, #4]
	subs r3, #24
	ldr r0, [sp, #20]
	movs r2, #40
	ldr r5, [sp, #12]
	mov lr, r5
	.2byte 0xf800
.L_0813f756:
	mov r0, r10
	cmp r0, #14
	bhi .L_0813f7c0
	movs r1, #3
	bl Math_Div
	movs r1, #5
	bl __modsi3
	lsls r0, r0, #10
	add r0, r11
	movs r7, #0
	movs r6, #1
	mov r8, r0
.L_0813f772:
	ldr r3, .L_0813f888
	movs r1, #7
	ldrb r2, [r3, r7]
	movs r3, #3
	orrs r3, r2
	movs r0, #188
	movs r2, #7
	str r6, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	ldr r3, .L_0813f88c
	mov r5, r9
	ldrsb r2, [r3, r7]
	ldr r3, .L_0813f890
	str r4, [sp, #16]
	ldrsb r1, [r3, r7]
	ldr r3, [r5, #4]
	adds r2, #32
	adds r3, r3, r1
	movs r1, #32
	str r1, [sp, #0]
	str r1, [sp, #4]
	movs r1, #224
	lsls r1, r1, #3
	subs r3, #32
	ldr r0, [sp, #20]
	add r1, r8
	mov lr, r4
	.2byte 0xf800
	adds r7, #1
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	cmp r7, #4
	bne .L_0813f772
.L_0813f7c0:
	mov r0, r10
	cmp r0, #0
	blt .L_0813f834
	movs r7, #0
	mov r5, r11
.L_0813f7ca:
	ldr r2, [r5, #24]
	cmp r2, #0
	blt .L_0813f82c
	ldr r3, [r5, #4]
	cmp r3, #0
	ble .L_0813f82c
	asrs r3, r2, #3
	ldr r0, [r5]
	adds r6, r3, #1
	bl Trig_Sin
	ldr r3, [r5, #4]
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #64
	ldr r0, [r5]
	mov r8, r3
	bl Trig_Cos
	ldr r3, [r5, #4]
	mov r1, r9
	muls r3, r0
	ldr r2, [r1, #4]
	asrs r3, r3, #16
	adds r4, r3, r2
	cmp r6, #0
	bgt .L_0813f802
	movs r6, #1
.L_0813f802:
	ldr r2, .L_0813f894
	lsls r0, r6, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #8]
	mov r3, r8
	adds r1, r2, r1
	str r0, [sp, #0]
	subs r2, r3, r6
	str r0, [sp, #4]
	subs r3, r4, r6
	ldr r0, [sp, #20]
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #4]
	subs r3, #2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0813f82c:
	adds r7, #1
	adds r5, #28
	cmp r7, #64
	bne .L_0813f7ca
.L_0813f834:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	movs r5, #1
	movs r0, #1
	str r3, [r2]
	add r10, r5
	bl WaitFrames
	mov r0, r10
	cmp r0, #56
	beq .L_0813f856
	b .L_0813f6b2
.L_0813f856:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0813f898
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813f888:
	.4byte Data_081976da
.L_0813f88c:
	.4byte Data_081976d2
.L_0813f890:
	.4byte Data_081976d6
.L_0813f894:
	.4byte Data_08197424
.L_0813f898:
	.4byte Func_08143000
