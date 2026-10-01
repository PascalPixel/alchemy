.syntax unified
	.thumb
	.global Func_081548d0
	.thumb_func
Func_081548d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #40]
	str r0, [sp, #44]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #36]
	movs r0, #1
	ldr r3, [r3, #96]
	str r3, [sp, #32]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08154920
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08154924
	adds r2, #48
	strh r3, [r2]
	ldr r1, [sp, #40]
	cmp r1, #1
	bne .L_0815492c
	ldr r2, [sp, #36]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08154928
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_0815493e
.L_08154920:
	.4byte 0x00000100
.L_08154924:
	.4byte 0x00000000
.L_08154928:
	.4byte 0x00000144
.L_0815492c:
	ldr r5, [sp, #36]
	movs r6, #224
	lsls r6, r6, #3
	ldr r0, .L_08154a88
	adds r1, r5, r6
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_0815493e:
	ldr r0, [sp, #44]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_08154950
	movs r2, #128
	ldr r3, .L_08154a8c
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
.L_08154950:
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #48]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r1, sp
	adds r1, #48
	str r1, [sp, #24]
	str r3, [r1, #4]
	ldr r5, [sp, #44]
	ldr r2, .L_08154a90
	ldr r3, [r5, #24]
	movs r6, #0
	ldrb r3, [r2, r3]
	movs r1, #1
	lsls r3, r3, #2
	adds r3, #56
	str r3, [sp, #28]
	ldr r3, .L_08154a94
	movs r2, #128
	mov r10, r6
	negs r1, r1
	lsls r2, r2, #3
.L_08154990:
	movs r0, #1
	add r10, r0
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_08154990
	ldr r7, .L_08154a98
	ldr r6, [sp, #36]
	movs r1, #0
	mov r10, r1
.L_081549a4:
	bl Random16
	ldr r3, .L_08154a9c
	mov r5, r10
	ldrb r2, [r3, r5]
	movs r3, #7
	ands r3, r0
	adds r2, r2, r3
	lsrs r3, r5, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #108
	subs r2, #4
	str r3, [r6, #4]
	str r2, [r6]
	bl Random16
	movs r5, #63
	ands r5, r0
	adds r5, #55
	str r5, [r6, #16]
	mov r0, r10
	movs r1, #3
	bl Math_Mod
	ldrb r3, [r7, r0]
	cmp r3, r5
	bge .L_081549de
	str r3, [r6, #16]
.L_081549de:
	mov r0, r10
	movs r1, #1
	lsls r3, r0, #2
	add r10, r1
	adds r3, #8
	mov r2, r10
	str r3, [r6, #24]
	adds r6, #28
	cmp r2, #16
	bne .L_081549a4
	ldr r3, [sp, #36]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	ldr r6, [sp, #36]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r6, r0
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08154aa0
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #28]
	movs r1, #0
	mov r11, r1
	cmp r2, #0
	bne .L_08154a22
	b .L_08154d0c
.L_08154a22:
	ldr r3, [sp, #28]
	ldr r5, [sp, #28]
	subs r2, #64
	subs r3, #20
	subs r5, #4
	str r2, [sp, #12]
	str r3, [sp, #20]
	str r5, [sp, #16]
.L_08154a32:
	ldr r6, [sp, #12]
	cmp r11, r6
	bne .L_08154a3e
	movs r0, #133
	bl Func_081180e8
.L_08154a3e:
	ldr r0, [sp, #20]
	cmp r11, r0
	blt .L_08154a6a
	ldr r1, [sp, #16]
	cmp r11, r1
	blt .L_08154a4c
	b .L_08154c1e
.L_08154a4c:
	ldr r3, .L_08154a80
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r2, [sp, #28]
	mov r5, r11
	subs r3, r2, r5
	ldr r2, .L_08154a84
	movs r1, #128
	lsls r1, r1, #19
	subs r3, #5
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
.L_08154a6a:
	ldr r6, [sp, #16]
	cmp r11, r6
	blt .L_08154a72
	b .L_08154c1e
.L_08154a72:
	ldr r1, [sp, #44]
	ldr r3, .L_08154a90
	ldr r2, [r1, #24]
	movs r0, #0
	ldrb r3, [r3, r2]
	mov r10, r0
	b .L_08154aa4
.L_08154a80:
	.4byte 0x00003f44
.L_08154a84:
	.4byte 0x00001000
.L_08154a88:
	.4byte 0x00000145
.L_08154a8c:
	.4byte 0xffff9000
.L_08154a90:
	.4byte Data_081983ea
.L_08154a94:
	.4byte Data_02010018
.L_08154a98:
	.4byte Data_081983d7
.L_08154a9c:
	.4byte Data_081983ed
.L_08154aa0:
	.4byte Func_08143000
.L_08154aa4:
	cmp r3, #0
	bne .L_08154aaa
	b .L_08154c1e
.L_08154aaa:
	ldr r3, [sp, #36]
	movs r2, #8
	mov r9, r2
	mov r8, r3
.L_08154ab2:
	mov r5, r10
	lsls r3, r5, #2
	adds r3, #9
	cmp r11, r3
	bne .L_08154aca
	ldr r6, [sp, #36]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r6, r0
	movs r3, #2
	str r3, [r2]
.L_08154aca:
	cmp r11, r9
	ble .L_08154b6c
	movs r1, #3
	mov r0, r10
	bl Math_Mod
	mov r1, r11
	mov r2, r9
	subs r3, r1, r2
	mov r6, r8
	lsls r5, r3, #3
	ldr r3, [r6, #16]
	cmp r5, r3
	ble .L_08154ae8
	adds r5, r3, #0
.L_08154ae8:
	ldr r1, [sp, #40]
	cmp r1, #0
	bne .L_08154b26
	mov r2, r10
	movs r4, #1
	ands r4, r2
	ldr r2, .L_08154d30
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #36]
	movs r6, #224
	adds r1, r3, r1
	ldr r3, .L_08154d34
	lsls r6, r6, #3
	ldrb r0, [r3, r0]
	adds r1, r1, r6
	mov r6, r8
	ldr r2, [r6]
	lsrs r3, r0, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	subs r3, r3, r5
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	b .L_08154b6c
.L_08154b26:
	ldr r2, .L_08154d38
	movs r3, #7
	mov r1, r10
	ands r3, r1
	ldrsb r3, [r2, r3]
	cmp r5, r3
	ble .L_08154b36
	adds r5, r3, #0
.L_08154b36:
	mov r2, r10
	movs r4, #1
	ands r4, r2
	ldr r2, .L_08154d3c
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #36]
	movs r6, #224
	adds r1, r3, r1
	ldr r3, .L_08154d40
	lsls r6, r6, #3
	ldrb r0, [r3, r0]
	adds r1, r1, r6
	mov r6, r8
	ldr r2, [r6]
	lsrs r3, r0, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	subs r3, r3, r5
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_08154b6c:
	ldr r1, [sp, #44]
	movs r5, #0
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_08154bb8
	mov r6, r9
	movs r4, #1
	mov r2, r10
	adds r6, #4
	ands r4, r2
	movs r7, #36
.L_08154b82:
	cmp r11, r6
	bne .L_08154bae
	cmp r4, #0
	bne .L_08154b94
	movs r0, #133
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r4, [sp, #8]
.L_08154b94:
	ldr r3, [sp, #44]
	movs r2, #5
	ldrsh r0, [r7, r3]
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r5, #0
	str r4, [sp, #8]
	bl Func_0814cd48
	ldr r2, [sp, #44]
	ldr r4, [sp, #8]
	ldr r3, [r2, #20]
.L_08154bae:
	adds r5, #1
	adds r7, #2
	cmp r5, r3
	bne .L_08154b82
	b .L_08154bbc
.L_08154bb8:
	mov r6, r9
	adds r6, #4
.L_08154bbc:
	cmp r11, r6
	beq .L_08154bc8
	mov r3, r9
	adds r3, #8
	cmp r11, r3
	bne .L_08154c04
.L_08154bc8:
	ldr r6, .L_08154d44
	movs r5, #0
	movs r7, #15
	b .L_08154bd4
.L_08154bd0:
	adds r6, #28
	adds r5, #1
.L_08154bd4:
	movs r3, #128
	lsls r3, r3, #2
	cmp r5, r3
	beq .L_08154c04
	ldr r3, [r6, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_08154bd0
	bl Random16
	mov r1, r8
	ldr r3, [r1]
	ands r0, r7
	adds r0, r0, r3
	subs r0, #8
	str r0, [r6]
	bl Random16
	ands r0, r7
	adds r0, #80
	movs r3, #0
	str r0, [r6, #4]
	str r3, [r6, #24]
.L_08154c04:
	ldr r6, [sp, #44]
	movs r2, #4
	movs r3, #28
	add r9, r2
	add r8, r3
	ldr r2, [r6, #24]
	ldr r3, .L_08154d48
	movs r5, #1
	ldrb r3, [r3, r2]
	add r10, r5
	cmp r10, r3
	beq .L_08154c1e
	b .L_08154ab2
.L_08154c1e:
	ldr r1, .L_08154d44
	movs r0, #0
	mov r10, r0
	mov r8, r1
.L_08154c26:
	mov r3, r8
	ldr r2, [r3, #24]
	cmp r2, #0
	blt .L_08154cd0
	movs r5, #240
	ldr r6, [sp, #40]
	lsrs r3, r2, #31
	lsls r5, r5, #5
	adds r3, r2, r3
	adds r5, #89
	asrs r7, r3, #1
	mov r9, r5
	cmp r6, #0
	beq .L_08154c4a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #255
	mov r9, r0
.L_08154c4a:
	ldr r2, .L_08154d4c
	lsls r6, r7, #1
	ldrh r1, [r2, r6]
	ldr r3, [sp, #36]
	add r1, r9
	adds r1, r3, r1
	mov r3, r8
	ldr r2, [r3]
	ldr r0, .L_08154d50
	ldr r3, .L_08154d54
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r1, r5
	ldrsb r5, [r0, r7]
	ldrb r0, [r3, r7]
	mov r3, r8
	lsls r0, r0, #24
	asrs r4, r0, #24
	ldr r3, [r3, #4]
	lsrs r0, r0, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r2, r2, r5
	subs r3, r3, r0
	str r5, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #32]
	ldr r4, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	ldr r5, .L_08154d4c
	movs r0, #224
	ldrh r1, [r5, r6]
	ldr r6, [sp, #36]
	ldr r5, .L_08154d54
	add r1, r9
	adds r1, r6, r1
	lsls r0, r0, #3
	adds r1, r1, r0
	ldrb r0, [r5, r7]
	mov r3, r8
	lsls r0, r0, #24
	ldr r2, [r3]
	asrs r4, r0, #24
	ldr r3, [r3, #4]
	ldr r6, .L_08154d50
	lsrs r0, r0, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	subs r3, r3, r0
	ldrsb r0, [r6, r7]
	str r4, [sp, #4]
	str r0, [sp, #0]
	ldr r0, [sp, #24]
	ldr r4, [r0, #4]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	mov r1, r8
	ldr r3, [r1, #24]
	adds r3, #1
	str r3, [r1, #24]
	cmp r3, #14
	bne .L_08154cd0
	movs r3, #1
	negs r3, r3
	str r3, [r1, #24]
.L_08154cd0:
	movs r3, #1
	movs r5, #128
	movs r2, #28
	add r10, r3
	lsls r5, r5, #2
	add r8, r2
	cmp r10, r5
	bne .L_08154c26
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r0, #240
	ldr r6, [sp, #36]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r6, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	movs r1, #1
	add r11, r1
	cmp r11, r2
	beq .L_08154d0c
	b .L_08154a32
.L_08154d0c:
	ldr r0, .L_08154d58
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08154d30:
	.4byte Data_081983da
.L_08154d34:
	.4byte Data_081983d4
.L_08154d38:
	.4byte Data_081983fd
.L_08154d3c:
	.4byte Data_081983e4
.L_08154d40:
	.4byte Data_081983e0
.L_08154d44:
	.4byte gMapCellBuffer
.L_08154d48:
	.4byte Data_081983ea
.L_08154d4c:
	.4byte Data_08198414
.L_08154d50:
	.4byte Data_08198405
.L_08154d54:
	.4byte Data_0819840c
.L_08154d58:
	.4byte Func_08143000
