.syntax unified
	.thumb
	.global Func_08179994
	.thumb_func
Func_08179994:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	sub sp, #120
	mov r11, r0
	ldr r0, [r3, #92]
	str r1, [sp, #60]
	mov r8, r0
	ldr r3, [r3, #100]
	movs r0, #1
	str r3, [sp, #52]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081799f4
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #224
	adds r2, #82
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_081799f8
	add r1, r8
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #0
	movs r3, #0
	ldr r0, .L_081799fc
	ldr r1, [sp, #52]
	bl Func_08157cf4
	mov r2, r11
	ldr r3, [r2, #24]
	cmp r3, #2
	bne .L_08179a10
	ldr r0, .L_08179a00
	bl Resource_GetTableEntry
	adds r1, r0, #0
	b .L_08179a04
.L_081799f4:
	.4byte 0x00001010
.L_081799f8:
	.4byte 0x000000eb
.L_081799fc:
	.4byte 0x00000134
.L_08179a00:
	.4byte 0x00000148
.L_08179a04:
	movs r0, #160
	ldr r3, .L_08179d60
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08179a10:
	mov r3, sp
	adds r3, #64
	adds r1, r3, #0
	movs r0, #0
	str r3, [sp, #48]
	bl Func_08144aac
	movs r2, #239
	lsls r2, r2, #7
	add r2, r8
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r8
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08179d64
	bl Scheduler_AddOrUpdateCallback
	movs r7, #0
	str r7, [sp, #44]
	ldr r3, .L_08179d68
	movs r0, #0
	movs r1, #1
	movs r2, #192
	mov r9, r0
	negs r1, r1
	lsls r2, r2, #2
.L_08179a50:
	movs r7, #1
	add r9, r7
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_08179a50
	mov r1, sp
	movs r0, #0
	adds r1, #108
	str r0, [sp, #56]
	str r1, [sp, #36]
	str r0, [sp, #16]
.L_08179a68:
	ldr r2, [sp, #56]
	cmp r2, #16
	bne .L_08179a74
	movs r0, #104
	bl Audio_PlayCue
.L_08179a74:
	ldr r3, [sp, #56]
	cmp r3, #0
	bne .L_08179a7e
	movs r7, #0
	str r7, [sp, #44]
.L_08179a7e:
	mov r1, r11
	ldr r0, [r1, #8]
	ldr r1, [sp, #36]
	bl Func_0815e21c
	ldr r2, [sp, #56]
	cmp r2, #7
	ble .L_08179aa2
	mov r3, r11
	ldr r1, [r3, #24]
	ldr r4, .L_08179d6c
	lsls r3, r1, #3
	subs r3, r3, r1
	ldrb r3, [r4, r3]
	ldr r7, [sp, #44]
	adds r7, r7, r3
	str r7, [sp, #44]
	b .L_08179aa8
.L_08179aa2:
	mov r0, r11
	ldr r1, [r0, #24]
	ldr r4, .L_08179d6c
.L_08179aa8:
	lsls r3, r1, #3
	subs r3, r3, r1
	adds r3, #1
	ldrb r2, [r4, r3]
	ldr r1, [sp, #44]
	cmp r1, r2
	ble .L_08179ab8
	str r2, [sp, #44]
.L_08179ab8:
	ldr r2, [sp, #56]
	cmp r2, #63
	bgt .L_08179ad6
	ldr r7, [sp, #36]
	mov r0, r8
	ldr r3, [r7]
	lsls r3, r3, #16
	str r3, [r0]
	ldr r3, [r7, #4]
	ldr r1, [sp, #44]
	subs r3, r3, r1
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r0, #4]
	b .L_08179b6a
.L_08179ad6:
	ldr r2, [sp, #56]
	cmp r2, #64
	bne .L_08179b28
	add r5, sp, #96
	adds r0, r5, #0
	bl Func_0815e22c
	ldr r3, [sp, #36]
	mov r7, r8
	ldr r2, [r3]
	lsls r2, r2, #16
	str r2, [r7]
	ldr r0, [sp, #36]
	ldr r1, [sp, #44]
	ldr r3, [r0, #4]
	subs r3, r3, r1
	subs r3, #32
	lsls r0, r3, #16
	movs r3, #0
	str r0, [r7, #4]
	str r3, [r7, #12]
	str r3, [r7, #16]
	ldr r3, [r5]
	lsls r3, r3, #16
	subs r3, r3, r2
	cmp r3, #0
	bge .L_08179b0e
	adds r3, #63
.L_08179b0e:
	asrs r3, r3, #6
	mov r2, r8
	str r3, [r2, #8]
	ldr r3, [r5, #4]
	subs r3, #32
	lsls r3, r3, #16
	subs r0, r3, r0
	cmp r0, #0
	bge .L_08179b22
	adds r0, #63
.L_08179b22:
	asrs r3, r0, #6
	mov r7, r8
	str r3, [r7, #20]
.L_08179b28:
	mov r0, r8
	ldr r2, [r0, #12]
	ldr r3, [r0]
	ldr r1, [r0, #16]
	adds r3, r3, r2
	str r3, [r0]
	ldr r3, [r0, #4]
	adds r3, r3, r1
	str r3, [r0, #4]
	ldr r3, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #20]
	str r2, [r0, #12]
	adds r1, r1, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	str r1, [r0, #16]
	cmp r3, #0
	bge .L_08179b52
	adds r3, #63
.L_08179b52:
	asrs r3, r3, #6
	mov r2, r8
	str r3, [r2, #12]
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r1, r3, #4
	cmp r1, #0
	bge .L_08179b64
	adds r1, #63
.L_08179b64:
	asrs r3, r1, #6
	mov r7, r8
	str r3, [r7, #16]
.L_08179b6a:
	mov r1, r11
	ldr r3, [r1, #24]
	ldr r7, .L_08179d6c
	lsls r2, r3, #3
	subs r2, r2, r3
	adds r2, #2
	ldrb r3, [r7, r2]
	movs r0, #0
	mov r9, r0
	cmp r3, #0
	beq .L_08179c64
	ldr r0, [sp, #56]
	ldr r1, [sp, #56]
	ldr r2, [sp, #44]
	asrs r0, r0, #31
	lsls r1, r1, #2
	str r0, [sp, #28]
	str r1, [sp, #32]
	lsls r2, r2, #16
	mov r10, r2
	add r5, sp, #84
.L_08179b94:
	ldr r0, [sp, #28]
	ldr r1, [sp, #56]
	lsrs r3, r0, #31
	adds r3, r1, r3
	mov r0, r9
	lsls r2, r0, #2
	asrs r3, r3, #1
	adds r6, r3, r2
	movs r3, #3
	ands r6, r3
	bl Func_08014de4
	mov r1, r8
	ldr r3, .L_08179d70
	ldr r0, [r1]
	ldr r2, .L_08179d74
	ldr r1, [r1, #4]
	adds r0, r0, r2
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	ldr r0, [sp, #16]
	bl Func_080150e4
	ldr r1, [sp, #16]
	movs r2, #250
	lsls r2, r2, #1
	adds r0, r1, r2
	bl SceneTransform_ApplyPitch
	mov r3, r9
	cmp r3, #7
	ble .L_08179be0
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
.L_08179be0:
	mov r0, r11
	ldr r2, [r0, #24]
	ldr r1, [sp, #32]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #3
	ldrb r3, [r7, r3]
	mov r0, r9
	muls r0, r3
	adds r0, r1, r0
	lsls r0, r0, #8
	bl Func_08015068
	movs r3, #0
	mov r2, r10
	str r3, [r5]
	str r3, [r5, #4]
	str r2, [r5, #8]
	ldr r3, .L_08179d78
	adds r1, r5, #0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	ldr r2, [r5]
	asrs r2, r2, #17
	adds r3, r2, #0
	adds r3, #64
	str r3, [r5]
	movs r0, #6
	ldrsh r3, [r5, r0]
	adds r2, #56
	adds r1, r3, #0
	adds r1, #64
	str r1, [r5, #4]
	mov r1, r11
	ldr r0, [r1, #24]
	adds r3, #16
	lsls r1, r0, #3
	subs r1, r1, r0
	adds r1, #6
	ldrb r4, [r7, r1]
	movs r0, #224
	lsls r1, r6, #10
	lsls r0, r0, #3
	add r1, r8
	adds r1, r1, r0
	movs r0, #16
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	ldr r0, [sp, #48]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	mov r3, r11
	ldr r2, [r3, #24]
	movs r1, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #2
	ldrb r3, [r7, r3]
	add r9, r1
	cmp r9, r3
	bne .L_08179b94
.L_08179c64:
	ldr r7, [sp, #56]
	cmp r7, #88
	beq .L_08179c6c
	b .L_08179d98
.L_08179c6c:
	movs r0, #134
	bl Func_081180e8
	mov r1, r11
	ldr r3, [r1, #20]
	movs r0, #0
	mov r9, r0
	cmp r3, #0
	bne .L_08179c80
	b .L_08179d80
.L_08179c80:
	mov r2, sp
	adds r2, #72
	movs r3, #36
	str r2, [sp, #24]
	str r3, [sp, #20]
	str r0, [sp, #12]
.L_08179c8c:
	ldr r7, [sp, #20]
	mov r2, r11
	ldrsh r0, [r7, r2]
	ldr r1, [sp, #24]
	bl Func_0815e1fc
	mov r1, r11
	ldrsh r0, [r7, r1]
	movs r1, #4
	bl Func_08118088
	mov r3, r11
	ldrsh r0, [r7, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r9
	movs r2, #5
	bl Func_0814cd48
	movs r7, #0
	str r7, [sp, #40]
	mov r0, r11
	ldr r1, [r0, #24]
	ldr r4, .L_08179d6c
	lsls r3, r1, #3
	subs r3, r3, r1
	adds r3, #4
	ldrb r3, [r4, r3]
	cmp r3, #0
	beq .L_08179d40
	ldr r1, [sp, #24]
	ldr r2, [sp, #12]
	ldr r3, .L_08179d7c
	mov r10, r1
	adds r7, r2, r3
.L_08179cd4:
	mov r0, r10
	ldr r3, [r0]
	str r4, [sp, #8]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	movs r5, #255
	ldr r3, [r0, #4]
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #24
	str r3, [r7, #24]
	ldr r1, [sp, #40]
	mov r2, r11
	adds r1, #1
	str r1, [sp, #40]
	ldr r0, .L_08179d6c
	ldr r1, [r2, #24]
	ldr r2, [sp, #40]
	lsls r3, r1, #3
	subs r3, r3, r1
	adds r3, #4
	ldrb r3, [r0, r3]
	adds r7, #28
	ldr r4, [sp, #8]
	cmp r2, r3
	bne .L_08179cd4
.L_08179d40:
	ldr r3, [sp, #20]
	ldr r7, [sp, #12]
	movs r0, #224
	lsls r0, r0, #4
	adds r3, #2
	adds r7, r7, r0
	str r7, [sp, #12]
	str r3, [sp, #20]
	mov r7, r11
	ldr r3, [r7, #20]
	movs r2, #1
	add r9, r2
	cmp r9, r3
	bne .L_08179c8c
	b .L_08179d86
	.2byte 0x0000
.L_08179d60:
	.4byte IwramCopyWords
.L_08179d64:
	.4byte Func_08143000
.L_08179d68:
	.4byte Data_02010018
.L_08179d6c:
	.4byte Data_0819940c
.L_08179d70:
	.4byte 0xffc00000
.L_08179d74:
	.4byte 0xff800000
.L_08179d78:
	.4byte IwramTransformVector
.L_08179d7c:
	.4byte gMapCellBuffer
.L_08179d80:
	mov r0, r11
	ldr r1, [r0, #24]
	ldr r4, .L_08179e58
.L_08179d86:
	lsls r2, r1, #3
	subs r2, r2, r1
	movs r3, #238
	adds r2, #5
	lsls r3, r3, #7
	ldrb r2, [r4, r2]
	adds r3, #168
	add r3, r8
	str r2, [r3]
.L_08179d98:
	ldr r2, .L_08179e5c
	ldr r6, .L_08179e60
	movs r1, #0
	mov r9, r1
	mov r10, r2
.L_08179da2:
	ldr r0, [r6, #24]
	cmp r0, #0
	blt .L_08179df0
	asrs r0, r0, #3
	adds r0, #1
	lsls r5, r0, #1
	mov r3, r9
	movs r4, #1
	ands r4, r3
	mov r7, r10
	subs r3, r5, #2
	ldrh r1, [r7, r3]
	ldr r2, [sp, #52]
	lsls r4, r4, #2
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r6, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r6, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r5, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [r4, r0]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	ldr r2, .L_08179e64
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_08179df0:
	movs r1, #1
	movs r2, #192
	add r9, r1
	lsls r2, r2, #2
	adds r6, #28
	cmp r9, r2
	bne .L_08179da2
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r8
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #16]
	ldr r0, [sp, #56]
	movs r7, #128
	lsls r7, r7, #1
	adds r3, r3, r7
	adds r0, #1
	str r3, [sp, #16]
	str r0, [sp, #56]
	cmp r0, #122
	beq .L_08179e32
	b .L_08179a68
.L_08179e32:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08179e68
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08179e58:
	.4byte Data_0819940c
.L_08179e5c:
	.4byte Data_08197410
.L_08179e60:
	.4byte gMapCellBuffer
.L_08179e64:
	.4byte 0xffffc000
.L_08179e68:
	.4byte Func_08143000
