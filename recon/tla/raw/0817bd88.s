.syntax unified
	.thumb
	.global Func_0817bd88
	.thumb_func
Func_0817bd88:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	str r0, [sp, #68]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #64]
	movs r0, #0
	ldr r1, [r3, #96]
	str r1, [sp, #60]
	ldr r2, [r3, #48]
	str r2, [sp, #52]
	ldr r5, [r3, #100]
	bl BattleFx_BeginCanvasLayer
	ldr r0, [sp, #68]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0817bdc4
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_0817bdcc
.L_0817bdc4:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_0817bdcc:
	ldr r3, .L_0817be0c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #64]
	movs r3, #239
	movs r0, #238
	lsls r3, r3, #7
	lsls r0, r0, #7
	adds r2, r1, r3
	adds r0, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r0
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0817be10
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #68]
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl GetBattleObjectSlotFar
	movs r3, #0
	ldr r1, [r0]
	str r3, [sp, #48]
	b .L_0817be14
	.2byte 0x0000
.L_0817be0c:
	.4byte 0x00001010
.L_0817be10:
	.4byte Func_08143000
.L_0817be14:
	str r3, [sp, #44]
	ldr r0, [sp, #64]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #228
	adds r2, r0, r3
	ldr r3, [r1, #8]
	str r3, [r2]
	movs r3, #221
	lsls r3, r3, #3
	adds r2, r0, r3
	ldr r3, [r1, #12]
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #236
	adds r2, r0, r3
	ldr r3, [r1, #16]
	str r3, [r2]
	movs r3, #222
	lsls r3, r3, #3
	adds r2, r0, r3
	ldr r3, [r1, #24]
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #244
	adds r2, r0, r3
	ldr r3, [r1, #28]
	ldr r0, .L_0817bebc
	str r3, [r2]
	ldr r2, [sp, #64]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0817bec0
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #64]
	movs r3, #142
	lsls r3, r3, #7
	adds r1, r2, r3
	ldr r0, .L_0817bec4
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0817bec8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817becc
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0817beb8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r0, .L_0817bed0
	movs r1, #4
	movs r2, #32
	movs r3, #32
	bl Func_08178680
	ldr r1, [sp, #52]
	movs r0, #0
	adds r1, #12
	str r0, [sp, #56]
	str r1, [sp, #28]
	b .L_0817bed4
	.2byte 0x0000
.L_0817beb8:
	.4byte 0x00000080
.L_0817bebc:
	.4byte 0x000000d3
.L_0817bec0:
	.4byte 0x00000134
.L_0817bec4:
	.4byte 0x000000f2
.L_0817bec8:
	.4byte 0x00000154
.L_0817becc:
	.4byte IwramCopyWords
.L_0817bed0:
	.4byte Data_02012000
.L_0817bed4:
	bl Func_08014de4
	ldr r0, [sp, #52]
	ldr r1, [sp, #28]
	bl Func_080156e8
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_0817befa
	ldr r3, [sp, #68]
	movs r1, #6
	ldr r0, [r3, #8]
	movs r3, #1
	negs r3, r3
	str r3, [sp, #0]
	movs r2, #4
	movs r3, #0
	bl Func_0814cd48
.L_0817befa:
	ldr r0, [sp, #56]
	cmp r0, #8
	bne .L_0817bf06
	movs r0, #103
	bl Audio_PlayCue
.L_0817bf06:
	ldr r1, [sp, #56]
	cmp r1, #100
	bne .L_0817bf20
	ldr r2, [sp, #68]
	movs r3, #1
	negs r3, r3
	ldr r0, [r2, #8]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #0
	movs r3, #0
	bl Func_0814cd48
.L_0817bf20:
	ldr r3, [sp, #56]
	cmp r3, #54
	bne .L_0817bf96
	movs r1, #128
	ldr r3, .L_0817c24c
	lsls r1, r1, #7
	ldr r2, .L_0817c250
	ldr r0, [sp, #60]
	mov lr, r3
	.2byte 0xf800
	movs r0, #212
	bl Audio_PlayCue
	movs r0, #212
	bl Func_081180e8
	movs r1, #238
	ldr r0, [sp, #64]
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #8
	str r3, [r2]
	ldr r0, [sp, #68]
	movs r2, #0
	ldr r3, [r0, #20]
	mov r10, r2
	cmp r3, #0
	beq .L_0817bf96
	movs r6, #128
	lsls r6, r6, #11
	movs r5, #36
.L_0817bf60:
	ldr r1, [sp, #68]
	movs r3, #150
	ldrsh r0, [r5, r1]
	movs r2, #128
	str r3, [sp, #4]
	movs r1, #1
	lsls r2, r2, #10
	adds r3, r6, #0
	str r6, [sp, #0]
	bl Func_0815f000
	ldr r3, [sp, #68]
	movs r2, #5
	ldrsh r0, [r5, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r10
	bl Func_0814cd48
	ldr r0, [sp, #68]
	movs r2, #1
	ldr r3, [r0, #20]
	add r10, r2
	adds r5, #2
	cmp r10, r3
	bne .L_0817bf60
.L_0817bf96:
	ldr r1, [sp, #56]
	cmp r1, #15
	bgt .L_0817bf9e
	b .L_0817c202
.L_0817bf9e:
	movs r3, #44
	adds r2, r1, #0
	muls r2, r3
	ldr r3, .L_0817c254
	movs r0, #178
	adds r3, r2, r3
	lsls r0, r0, #3
	str r3, [sp, #40]
	cmp r3, r0
	ble .L_0817bfb4
	str r0, [sp, #40]
.L_0817bfb4:
	ldr r1, [sp, #56]
	cmp r1, #73
	ble .L_0817bfc4
	movs r3, #144
	lsls r3, r3, #5
	adds r3, #72
	subs r3, r3, r2
	str r3, [sp, #40]
.L_0817bfc4:
	ldr r3, [sp, #40]
	movs r2, #0
	lsls r3, r3, #5
	str r3, [sp, #32]
	str r3, [sp, #36]
	str r2, [sp, #24]
	str r2, [sp, #20]
	mov r9, r2
.L_0817bfd4:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #94
	mov r0, r9
	muls r0, r3
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #214
	adds r0, r0, r1
	bl Trig_Sin
	movs r3, #44
	muls r3, r0
	ldr r0, [sp, #56]
	asrs r3, r3, #16
	movs r2, #50
	add r0, r9
	subs r2, r2, r3
	lsls r0, r0, #11
	str r2, [sp, #12]
	bl Trig_Sin
	lsls r0, r0, #4
	asrs r0, r0, #16
	mov r3, r9
	muls r3, r0
	ldr r2, [sp, #12]
	cmp r3, #0
	bge .L_0817c010
	adds r3, #31
.L_0817c010:
	asrs r1, r3, #5
	adds r0, r2, r1
	movs r3, #0
	mov r8, r0
	mov r10, r3
	lsrs r3, r0, #31
	add r3, r8
	asrs r3, r3, #1
	mov r0, r9
	mov r11, r3
	adds r3, r0, r1
	adds r7, r3, #0
	ldr r0, .L_0817c258
	ldr r3, [sp, #20]
	ldr r4, [sp, #24]
	adds r6, r1, #0
	adds r7, #32
	adds r6, #16
	adds r5, r3, r0
.L_0817c036:
	adds r0, r4, #0
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	ldr r4, [sp, #8]
	mov r0, r11
	asrs r3, r3, #16
	subs r3, r3, r0
	subs r3, #16
	strb r3, [r5]
	adds r0, r4, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #16
	negs r3, r3
	ldr r2, [sp, #12]
	movs r0, #1
	strb r3, [r5, #1]
	add r10, r0
	movs r3, #0
	strb r3, [r5, #2]
	mov r3, r10
	adds r7, r7, r2
	adds r6, r6, r2
	adds r5, #4
	ldr r1, [sp, #16]
	ldr r4, [sp, #8]
	cmp r3, #2
	bne .L_0817c036
	mov r0, r9
	cmp r0, #32
	bne .L_0817c0c0
	ldr r0, [sp, #36]
	bl Trig_Cos
	ldr r2, [sp, #12]
	ldr r1, [sp, #16]
	lsrs r5, r2, #31
	adds r5, r2, r5
	asrs r5, r5, #1
	adds r3, r5, r1
	adds r3, #16
	adds r2, r3, #0
	muls r2, r0
	mov r0, r8
	lsrs r3, r0, #31
	add r3, r8
	asrs r3, r3, #1
	asrs r2, r2, #16
	subs r2, r2, r3
	subs r2, #16
	ldr r0, [sp, #36]
	str r2, [sp, #48]
	bl Trig_Sin
	ldr r1, [sp, #16]
	adds r5, r5, r1
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	negs r3, r3
	str r3, [sp, #44]
.L_0817c0c0:
	ldr r1, [sp, #24]
	ldr r2, [sp, #40]
	ldr r3, [sp, #20]
	movs r0, #1
	adds r1, r1, r2
	add r9, r0
	str r1, [sp, #24]
	adds r3, #8
	mov r1, r9
	str r3, [sp, #20]
	cmp r1, #33
	beq .L_0817c0da
	b .L_0817bfd4
.L_0817c0da:
	bl Func_081969f8
	movs r2, #0
	ldr r3, [sp, #72]
	mov r8, r2
	ldr r2, .L_0817c25c
	adds r6, r0, #0
	ands r3, r2
	movs r2, #7
	orrs r3, r2
	ldr r2, .L_0817c260
	ldr r0, [sp, #64]
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	movs r1, #224
	orrs r3, r2
	lsls r1, r1, #3
	str r3, [sp, #72]
	add r7, sp, #72
	adds r3, r0, r1
	str r3, [r7, #4]
	ldr r0, .L_0817c264
	ldr r3, .L_0817c268
	movs r2, #6
	mov r1, r8
	str r3, [r6, #8]
	str r0, [r6, #12]
	str r1, [r6, #20]
	str r2, [r6]
	str r7, [r6, #16]
	mov r10, r2
	ldr r2, [sp, #68]
	add r5, sp, #80
	adds r1, r5, #0
	ldr r0, [r2, #8]
	bl Func_0815e20c
	mov r3, r8
	strb r3, [r6, #25]
	ldr r0, [sp, #56]
	movs r3, #127
	lsls r2, r0, #2
	bics r3, r2
	strb r3, [r6, #24]
	bl Func_08014de4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r2, .L_0817c26c
	asrs r0, r0, #1
	subs r0, #64
	lsls r1, r1, #16
	adds r1, r1, r2
	lsls r0, r0, #16
	movs r2, #0
	movs r5, #128
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, [sp, #68]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_0817c170
	adds r0, r5, #0
	bl Func_08015068
.L_0817c170:
	movs r0, #128
	lsls r0, r0, #7
	bl Func_080150e4
	ldr r1, [sp, #56]
	cmp r1, #73
	ble .L_0817c18a
	movs r0, #212
	lsls r3, r1, #11
	lsls r0, r0, #10
	subs r0, r0, r3
	bl Func_0801521c
.L_0817c18a:
	ldr r1, .L_0817c264
	movs r2, #66
	ldr r0, .L_0817c258
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	ldr r3, .L_0817c270
	mov r2, r10
	str r3, [r6, #8]
	ldr r3, .L_0817c264
	mov r0, r8
	strb r2, [r7]
	strb r2, [r7, #1]
	str r3, [r6, #12]
	strb r0, [r6, #24]
	strb r0, [r6, #25]
	ldr r1, [sp, #64]
	movs r2, #142
	lsls r2, r2, #7
	adds r3, r1, r2
	str r3, [r7, #4]
	mov r3, r8
	str r3, [r6, #20]
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
	lsls r0, r1, #16
	lsls r1, r2, #16
	movs r2, #0
	bl Func_08015160
	movs r0, #174
	ldr r3, [sp, #32]
	lsls r0, r0, #7
	adds r0, #112
	subs r0, r0, r3
	bl Func_080150e4
	ldr r1, [sp, #40]
	movs r2, #128
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #3
	subs r0, r0, r1
	lsls r2, r2, #8
	adds r0, r0, r2
	bl Func_0801521c
	ldr r0, .L_0817c274
	ldr r1, .L_0817c264
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	adds r0, r6, #0
	bl Sys_Free
.L_0817c202:
	bl Func_081434f8
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	movs r0, #240
	ldr r3, [sp, #64]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r3, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #56]
	adds r1, #1
	str r1, [sp, #56]
	cmp r1, #103
	beq .L_0817c22e
	b .L_0817bed4
.L_0817c22e:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0817c278
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817c24c:
	.4byte IwramFillWords
.L_0817c250:
	.4byte 0x3f3f3f3f
.L_0817c254:
	.4byte 0xfffffd40
.L_0817c258:
	.4byte gMapCellBuffer
.L_0817c25c:
	.4byte 0xffffff00
.L_0817c260:
	.4byte 0xffff00ff
.L_0817c264:
	.4byte Data_02011000
.L_0817c268:
	.4byte Data_02012000
.L_0817c26c:
	.4byte 0xffd60000
.L_0817c270:
	.4byte Data_08199340
.L_0817c274:
	.4byte Data_081991e0
.L_0817c278:
	.4byte Func_08143000
