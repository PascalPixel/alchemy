.syntax unified
	.thumb
	.global Func_0815c970
	.thumb_func
Func_0815c970:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r8, r0
	ldr r0, [r5, #92]
	ldr r2, [r5, #96]
	sub sp, #28
	mov r10, r0
	movs r0, #0
	str r2, [sp, #12]
	mov r11, r1
	bl BattleFx_BeginCanvasLayer
	movs r3, #128
	ldr r2, .L_0815c9b0
	lsls r3, r3, #19
	adds r3, #32
	mov r0, r8
	strh r2, [r3]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_0815c9b4
	movs r0, #104
	movs r1, #35
	b .L_0815c9b8
	.2byte 0x0000
.L_0815c9b0:
	.4byte 0x00000100
.L_0815c9b4:
	movs r0, #104
	movs r1, #39
.L_0815c9b8:
	bl Func_081963ec
	ldr r6, [r5, #104]
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0815cba4
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #1
	ldr r0, .L_0815cba8
	ldr r1, .L_0815cbac
	movs r3, #0
	bl Func_08157cf4
	mov r2, r11
	cmp r2, #0
	bne .L_0815c9f4
	ldr r0, .L_0815cbb0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0815cbb4
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0815c9f4:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #75
	add r2, r10
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815cbb8
	bl Scheduler_AddOrUpdateCallback
	mov r2, r8
	movs r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r2, r8
	add r5, sp, #16
	mov r9, r0
	adds r1, r5, #0
	movs r3, #36
	ldrsh r0, [r2, r3]
	bl Func_0815e20c
	mov r0, r8
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0815ca40
	ldr r2, [r5]
	movs r1, #128
	movs r3, #16
	b .L_0815ca46
.L_0815ca40:
	ldr r2, [r5]
	movs r1, #128
	movs r3, #112
.L_0815ca46:
	lsls r1, r1, #19
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	movs r2, #74
	mov r3, r11
	str r2, [sp, #8]
	cmp r3, #1
	beq .L_0815ca5e
	movs r0, #48
	str r0, [sp, #8]
.L_0815ca5e:
	ldr r2, [sp, #8]
	movs r5, #0
	cmp r2, #0
	bne .L_0815ca68
	b .L_0815cb86
.L_0815ca68:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_0815ca70
	adds r3, r5, #3
.L_0815ca70:
	asrs r4, r3, #2
	cmp r4, #5
	bgt .L_0815cae4
	cmp r4, #3
	bgt .L_0815cab2
	ldr r0, .L_0815cbbc
	lsls r3, r4, #1
	ldrh r1, [r0, r3]
	movs r2, #224
	lsls r2, r2, #3
	mov r3, r8
	add r1, r10
	adds r1, r1, r2
	ldr r2, [r3, #4]
	ldr r0, .L_0815cbc0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, r4, r3
	ldrb r2, [r0, r3]
	ldr r0, .L_0815cbc4
	ldr r3, .L_0815cbc8
	ldrb r0, [r0, r4]
	ldrsb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_0815cbcc
	adds r3, #32
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #12]
	mov lr, r6
	.2byte 0xf800
	b .L_0815cae4
.L_0815cab2:
	ldr r2, .L_0815cbbc
	lsls r3, r4, #1
	mov r0, r8
	ldrh r1, [r2, r3]
	ldr r2, [r0, #4]
	ldr r3, .L_0815cbac
	ldr r0, .L_0815cbc0
	adds r1, r1, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, r4, r3
	ldrb r2, [r0, r3]
	ldr r0, .L_0815cbc4
	ldr r3, .L_0815cbc8
	ldrb r0, [r0, r4]
	ldrsb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_0815cbcc
	adds r3, #32
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #12]
	mov lr, r6
	.2byte 0xf800
.L_0815cae4:
	cmp r5, #8
	bne .L_0815cb28
	mov r2, r11
	cmp r2, #0
	bne .L_0815cb02
	movs r0, #133
	bl Func_081180e8
	mov r2, r8
	movs r3, #36
	ldrsh r0, [r2, r3]
	movs r1, #1
	bl Func_08118088
	b .L_0815cb1c
.L_0815cb02:
	movs r0, #134
	bl Audio_PlayCue
	mov r2, r8
	movs r3, #36
	ldrsh r0, [r2, r3]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_0815cb1c:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #8
	str r3, [r2]
.L_0815cb28:
	mov r3, r11
	cmp r3, #1
	bne .L_0815cb5e
	cmp r5, #13
	bne .L_0815cb48
	movs r3, #192
	mov r0, r9
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #81
	str r3, [r0, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r0, #68]
.L_0815cb48:
	cmp r5, #65
	bne .L_0815cb5e
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #4
	str r3, [r2]
	movs r0, #134
	bl Func_081180e8
.L_0815cb5e:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #8]
	adds r5, #1
	cmp r5, r2
	beq .L_0815cb86
	b .L_0815ca68
.L_0815cb86:
	ldr r0, .L_0815cbb8
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0815cba4:
	.4byte 0x00000132
.L_0815cba8:
	.4byte 0x00000133
.L_0815cbac:
	.4byte gMapCellBuffer
.L_0815cbb0:
	.4byte 0x00000163
.L_0815cbb4:
	.4byte IwramCopyWords
.L_0815cbb8:
	.4byte Func_08143000
.L_0815cbbc:
	.4byte Data_081986d4
.L_0815cbc0:
	.4byte Data_081986e0
.L_0815cbc4:
	.4byte Data_081986c8
.L_0815cbc8:
	.4byte Data_081986ec
.L_0815cbcc:
	.4byte Data_081986ce
