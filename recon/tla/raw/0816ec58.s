.syntax unified
	.thumb
	.global Func_0816ec58
	.thumb_func
Func_0816ec58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	ldr r3, [r3, #96]
	sub sp, #56
	mov r10, r0
	movs r0, #1
	str r3, [sp, #20]
	mov r9, r1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816ecb8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0816ecbc
	movs r1, #224
	adds r2, #50
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0816ecc0
	add r1, r9
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r1, #220
	lsls r1, r1, #6
	adds r1, #96
	add r1, r9
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0816ecc4
	bl Func_08157cf4
	ldr r0, .L_0816ecc8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	b .L_0816eccc
.L_0816ecb8:
	.4byte 0x00000100
.L_0816ecbc:
	.4byte 0x00001010
.L_0816ecc0:
	.4byte 0x000000ce
.L_0816ecc4:
	.4byte 0x00000184
.L_0816ecc8:
	.4byte 0x00000130
.L_0816eccc:
	movs r0, #160
	ldr r3, .L_0816ef30
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816ef34
	bl Scheduler_AddOrUpdateCallback
	mov r2, r10
	ldr r3, [r2, #24]
	add r5, sp, #32
	lsls r6, r3, #2
	adds r3, r6, #0
	adds r3, #32
	str r3, [sp, #16]
	adds r1, r5, #0
	movs r4, #36
	ldrsh r0, [r2, r4]
	bl Func_0815e21c
	mov r1, r10
	ldr r3, [r1, #20]
	lsls r3, r3, #1
	adds r3, #34
	ldrsh r0, [r1, r3]
	add r1, sp, #44
	bl Func_0815e21c
	ldr r1, [r5]
	ldr r3, [r5, #12]
	movs r4, #54
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	movs r2, #128
	movs r3, #64
	lsls r2, r2, #19
	subs r3, r3, r1
	lsls r3, r3, #8
	adds r2, #40
	str r1, [r5]
	negs r4, r4
	str r3, [r2]
	movs r3, #0
	mov r11, r3
	cmp r6, r4
	bne .L_0816ed4c
	b .L_0816f058
.L_0816ed4c:
	mov r1, sp
	adds r1, #24
	adds r6, #54
	str r1, [sp, #8]
	str r6, [sp, #12]
.L_0816ed56:
	mov r2, r11
	cmp r2, #0
	bne .L_0816edbe
	movs r3, #0
	mov r8, r3
	mov r7, r9
.L_0816ed62:
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r7]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r4, #1
	asrs r3, r3, #8
	add r8, r4
	str r3, [r7, #16]
	mov r1, r8
	movs r3, #8
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #16
	bne .L_0816ed62
.L_0816edbe:
	mov r2, r11
	cmp r2, #32
	bne .L_0816edca
	movs r0, #134
	bl Func_081180e8
.L_0816edca:
	mov r3, r11
	cmp r3, #0
	bge .L_0816edd2
	b .L_0816ef8c
.L_0816edd2:
	movs r5, #1
	cmp r3, #23
	bgt .L_0816ede2
	mov r4, r10
	ldr r3, [r4, #24]
	cmp r3, #0
	ble .L_0816ede2
	movs r5, #0
.L_0816ede2:
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #104
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #7
	str r3, [sp, #24]
	movs r2, #7
	movs r3, #7
	movs r0, #188
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r1, [sp, #8]
	movs r2, #0
	str r3, [r1, #4]
	mov r3, r10
	mov r8, r2
	ldr r2, [r3, #24]
	movs r4, #4
	lsls r3, r2, #1
	adds r3, r3, r2
	negs r4, r4
	cmp r3, r4
	bne .L_0816ee28
	b .L_0816ef80
.L_0816ee28:
	ldr r3, .L_0816ef38
	mov r1, r8
	ldrb r3, [r3, r1]
	mov r2, r11
	subs r7, r2, r3
	ldr r2, .L_0816ef3c
	lsls r3, r1, #1
	ldrsh r6, [r2, r3]
	cmp r7, #2
	bne .L_0816ee78
	mov r2, r10
	ldr r1, [r2, #20]
	mov r0, r8
	bl __modsi3
	adds r3, r0, #0
	lsls r5, r3, #1
	mov r4, r10
	adds r5, #36
	movs r2, #3
	ldrsh r0, [r4, r5]
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #5
	bl Func_0814cd48
	mov r3, r10
	ldrsh r0, [r3, r5]
	movs r1, #0
	bl Func_08118088
	movs r0, #212
	bl Audio_PlayCue
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r9
	movs r1, #3
	str r1, [r3]
.L_0816ee78:
	ldr r0, .L_0816ef40
	mov r2, r8
	ldrb r3, [r0, r2]
	movs r1, #60
	cmp r3, #1
	bls .L_0816ee86
	movs r1, #35
.L_0816ee86:
	ldr r3, .L_0816ef44
	mov r4, r8
	ldrb r3, [r3, r4]
	muls r3, r1
	cmp r3, #0
	bge .L_0816ee94
	adds r3, #63
.L_0816ee94:
	ldr r2, [sp, #16]
	asrs r1, r3, #6
	cmp r11, r2
	bge .L_0816eea4
	lsls r3, r7, #2
	adds r3, r3, r7
	lsls r4, r3, #1
	b .L_0816eeb2
.L_0816eea4:
	ldr r4, [sp, #16]
	mov r3, r11
	subs r2, r3, r4
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	subs r4, r1, r3
.L_0816eeb2:
	cmp r4, r1
	ble .L_0816eeb8
	adds r4, r1, #0
.L_0816eeb8:
	mov r1, r8
	ldrb r3, [r0, r1]
	cmp r3, #1
	beq .L_0816eede
	cmp r3, #1
	bgt .L_0816eeca
	cmp r3, #0
	beq .L_0816eed4
	b .L_0816ef00
.L_0816eeca:
	cmp r3, #2
	beq .L_0816eee6
	cmp r3, #3
	beq .L_0816eef4
	b .L_0816ef00
.L_0816eed4:
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r6, r6, r3
	b .L_0816ef00
.L_0816eede:
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	b .L_0816eefe
.L_0816eee6:
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0816eeee
	adds r3, r4, #3
.L_0816eeee:
	asrs r3, r3, #2
	subs r6, r6, r3
	b .L_0816ef00
.L_0816eef4:
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0816eefc
	adds r3, r4, #3
.L_0816eefc:
	asrs r3, r3, #2
.L_0816eefe:
	adds r6, r6, r3
.L_0816ef00:
	mov r3, r8
	ldrb r2, [r0, r3]
	adds r3, r2, #0
	cmp r3, #1
	bhi .L_0816ef48
	movs r1, #36
	str r1, [sp, #0]
	movs r0, #1
	ldr r1, [sp, #8]
	str r4, [sp, #4]
	ands r0, r2
	lsls r0, r0, #2
	movs r3, #112
	subs r3, r3, r4
	ldr r4, [r0, r1]
	movs r1, #224
	adds r2, r6, #0
	lsls r1, r1, #3
	subs r2, #18
	ldr r0, [sp, #20]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
	b .L_0816ef6c
.L_0816ef30:
	.4byte IwramCopyWords
.L_0816ef34:
	.4byte Func_08143000
.L_0816ef38:
	.4byte Data_08198bb0
.L_0816ef3c:
	.4byte Data_08198b9c
.L_0816ef40:
	.4byte Data_08198bc4
.L_0816ef44:
	.4byte Data_08198bba
.L_0816ef48:
	movs r1, #17
	str r1, [sp, #0]
	movs r0, #1
	ldr r1, [sp, #8]
	str r4, [sp, #4]
	ands r0, r2
	lsls r0, r0, #2
	movs r3, #108
	subs r3, r3, r4
	ldr r4, [r0, r1]
	movs r1, #247
	adds r2, r6, #0
	lsls r1, r1, #4
	subs r2, #9
	ldr r0, [sp, #20]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
.L_0816ef6c:
	movs r2, #1
	mov r3, r10
	add r8, r2
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #4
	cmp r8, r3
	beq .L_0816ef80
	b .L_0816ee28
.L_0816ef80:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_0816ef8c:
	mov r4, r10
	ldr r0, [r4, #4]
	ldr r1, [sp, #8]
	bl Func_08144aac
	mov r3, r10
	ldr r2, [r3, #24]
	movs r4, #8
	movs r1, #0
	lsls r3, r2, #2
	negs r4, r4
	mov r8, r1
	cmp r3, r4
	beq .L_0816f022
	mov r5, r9
.L_0816efaa:
	mov r1, r8
	cmp r1, #15
	ble .L_0816efb6
	ldr r3, [sp, #16]
	cmp r11, r3
	blt .L_0816f014
.L_0816efb6:
	mov r4, r8
	lsls r3, r4, #1
	adds r3, #8
	cmp r11, r3
	blt .L_0816f014
	ldr r0, [r5, #24]
	cmp r0, #28
	bgt .L_0816f014
	movs r1, #3
	bl Math_Div
	movs r1, #2
	ldrsh r4, [r5, r1]
	movs r2, #6
	ldrsh r6, [r5, r2]
	cmp r0, #6
	ble .L_0816efda
	movs r0, #6
.L_0816efda:
	ldr r3, .L_0816f070
	lsls r2, r0, #1
	ldrh r1, [r3, r2]
	movs r3, #220
	lsls r3, r3, #6
	adds r3, #96
	add r1, r9
	adds r1, r1, r3
	ldr r3, .L_0816f074
	ldrh r0, [r3, r2]
	lsrs r3, r0, #1
	subs r2, r4, r3
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #24]
	subs r3, r6, r3
	ldr r0, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
	ldr r2, .L_0816f078
	adds r3, #1
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector2
	mov r4, r10
	ldr r2, [r4, #24]
.L_0816f014:
	movs r1, #1
	lsls r3, r2, #2
	add r8, r1
	adds r3, #8
	adds r5, #28
	cmp r8, r3
	bne .L_0816efaa
.L_0816f022:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #12]
	movs r2, #1
	add r11, r2
	cmp r11, r3
	beq .L_0816f058
	b .L_0816ed56
.L_0816f058:
	ldr r0, .L_0816f07c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816f070:
	.4byte Data_08198bce
.L_0816f074:
	.4byte Data_08198bdc
.L_0816f078:
	.4byte 0xffffe000
.L_0816f07c:
	.4byte Func_08143000
