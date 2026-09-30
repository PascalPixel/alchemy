.syntax unified
	.thumb
	.global Func_0813eb70
	.thumb_func
Func_0813eb70:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #246
	sub sp, #56
	lsls r1, r1, #7
	str r0, [sp, #52]
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	str r0, [sp, #48]
	lsls r1, r1, #7
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #44]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	adds r3, #176
	str r0, [sp, #28]
	ldr r1, [sp, #48]
	ldr r3, [r3]
	movs r2, #240
	ldr r4, [sp, #52]
	lsls r2, r2, #7
	movs r5, #240
	adds r2, #240
	lsls r5, r5, #7
	mov r8, r3
	adds r5, #228
	adds r3, r1, r2
	str r4, [r3]
	adds r3, r1, r5
	movs r5, #1
	str r5, [r3]
	bl Func_081434d8
	ldr r2, .L_0813ec28
	mov r7, r8
	movs r3, #32
	str r5, [r7, #12]
	strh r3, [r2, #6]
	ldr r0, [sp, #28]
	movs r1, #206
	lsls r1, r1, #3
	adds r3, r0, r1
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #0
	bl Func_08118038
	ldr r3, .L_0813ec20
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	movs r3, #100
	movs r0, #0
	bl Func_08118028
	movs r3, #128
	lsls r3, r3, #19
	movs r5, #0
	adds r3, #40
	str r5, [r7, #12]
	movs r2, #128
	str r5, [r3]
	ldr r3, .L_0813ec2c
	lsls r2, r2, #19
	adds r2, #44
	str r3, [r2]
	ldr r3, .L_0813ec24
	subs r2, #12
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #34
	strh r5, [r3]
	b .L_0813ec30
.L_0813ec20:
	.4byte 0x00000784
.L_0813ec24:
	.4byte 0x00000080
.L_0813ec28:
	.4byte Data_03001120
.L_0813ec2c:
	.4byte 0xfffff000
.L_0813ec30:
	adds r3, #2
	strh r5, [r3]
	ldr r3, .L_0813ec6c
	adds r2, #6
	strh r3, [r2]
	ldr r1, .L_0813ec70
	movs r3, #128
	ldr r2, .L_0813ec74
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_0813ec78
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_0813ec7c
	adds r2, #2
	strh r3, [r2]
	movs r6, #0
	ldr r2, .L_0813ec80
	movs r3, #128
	b .L_0813ec84
	.2byte 0x0000
.L_0813ec6c:
	.4byte 0x00000100
.L_0813ec70:
	.4byte 0x000000f0
.L_0813ec74:
	.4byte 0x00001088
.L_0813ec78:
	.4byte 0x00003537
.L_0813ec7c:
	.4byte 0x00003f21
.L_0813ec80:
	.4byte 0x06003800
.L_0813ec84:
	movs r4, #128
	str r6, [sp, #20]
	lsls r4, r4, #2
	lsls r3, r3, #1
	mov r10, r4
	mov r12, r2
	mov lr, r3
	movs r4, #0
.L_0813ec94:
	movs r7, #0
	str r7, [sp, #24]
	mov r1, lr
	adds r0, r5, r1
	lsls r1, r4, #1
.L_0813ec9e:
	adds r3, r0, #0
	orrs r3, r1
	lsls r3, r3, #16
	mov r7, r12
	adds r2, r6, r7
	asrs r3, r3, #16
	strh r3, [r2]
	ldr r2, [sp, #24]
	add r0, r10
	adds r2, #1
	adds r1, #2
	str r2, [sp, #24]
	adds r6, #2
	cmp r2, #8
	bne .L_0813ec9e
	ldr r7, [sp, #20]
	movs r3, #128
	lsls r3, r3, #5
	adds r7, #1
	adds r5, r5, r3
	adds r4, #8
	str r7, [sp, #20]
	cmp r7, #16
	bne .L_0813ec94
	movs r1, #128
	ldr r5, .L_0813ed60
	ldr r0, [sp, #44]
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_0813ed64
	mov lr, r5
	.2byte 0xf800
	ldr r1, .L_0813ed68
	ldr r0, .L_0813ed6c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0813ed16
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0813ed16:
	strh r4, [r0]
	movs r2, #128
	ldr r3, .L_0813ed58
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0813ed5c
	subs r2, #2
	strh r3, [r2]
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0813ed70
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r4, [sp, #48]
	movs r5, #239
	movs r7, #238
	lsls r5, r5, #7
	lsls r7, r7, #7
	adds r2, r4, r5
	movs r3, #1
	adds r7, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r4, r7
	movs r3, #0
	str r3, [r2]
	b .L_0813ed74
	.2byte 0x0000
.L_0813ed58:
	.4byte 0x00001010
.L_0813ed5c:
	.4byte 0x00000000
.L_0813ed60:
	.4byte IwramClearWords
.L_0813ed64:
	.4byte 0x06004000
.L_0813ed68:
	.4byte Data_020038e0
.L_0813ed6c:
	.4byte 0x04000208
.L_0813ed70:
	.4byte 0x00000106
.L_0813ed74:
	lsls r1, r1, #4
	ldr r0, .L_0813ef10
	bl Scheduler_AddOrUpdateCallback
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #104]
	movs r1, #19
	str r0, [sp, #32]
	movs r0, #188
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	movs r1, #0
	str r5, [sp, #36]
	str r1, [sp, #12]
	ldr r7, .L_0813ef14
	ldr r4, .L_0813ef18
	ldr r0, .L_0813ef1c
	ldr r1, [sp, #48]
	ldr r5, .L_0813ef20
	movs r6, #0
.L_0813edaa:
	ldrb r2, [r0]
	ldrb r3, [r4]
	lsls r2, r2, #16
	lsls r3, r3, #16
	str r2, [r1]
	str r3, [r1, #4]
	adds r2, r2, r7
	adds r3, r3, r5
	asrs r2, r2, #2
	asrs r3, r3, #2
	adds r6, #1
	str r2, [r1, #12]
	str r3, [r1, #16]
	adds r0, #1
	adds r4, #1
	adds r1, #28
	cmp r6, #33
	bne .L_0813edaa
	movs r2, #240
	ldr r3, .L_0813ef24
	ldr r1, .L_0813ef28
	lsls r2, r2, #7
	ldr r0, .L_0813ef2c
	mov lr, r3
	.2byte 0xf800
	movs r1, #240
	ldr r5, .L_0813ef30
	lsls r1, r1, #7
	ldr r2, .L_0813ef34
	ldr r0, .L_0813ef2c
	mov lr, r5
	.2byte 0xf800
	mov r2, r8
	movs r3, #1
	str r3, [r2, #16]
	ldr r3, [sp, #48]
	ldr r2, .L_0813ef38
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #160
	adds r1, r3, r4
	ldrh r3, [r2, #4]
	movs r0, #238
	str r3, [r1]
	ldr r7, [sp, #48]
	ldrh r3, [r2, #6]
	lsls r0, r0, #7
	adds r0, #164
	adds r1, r7, r0
	str r3, [r1]
	ldr r1, .L_0813ef3c
	movs r3, #0
	strh r3, [r2, #4]
	ldr r0, .L_0813ef40
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0813ee46
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #248
	adds r3, r3, r1
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #129
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0813ee46:
	strh r4, [r0]
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, .L_0813ef44
	adds r0, #192
	mov lr, r5
	.2byte 0xf800
	movs r0, #212
	bl Audio_PlayCue
	ldr r2, [sp, #52]
	movs r3, #30
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	movs r2, #3
	bl Func_0814cd48
	movs r3, #0
	str r3, [sp, #40]
.L_0813ee76:
	ldr r4, [sp, #40]
	cmp r4, #2
	bne .L_0813ee82
	movs r0, #212
	bl Audio_PlayCue
.L_0813ee82:
	ldr r5, [sp, #40]
	cmp r5, #3
	bne .L_0813ee8e
	movs r0, #212
	bl Audio_PlayCue
.L_0813ee8e:
	ldr r7, [sp, #40]
	cmp r7, #28
	bne .L_0813eeaa
	ldr r2, [sp, #52]
	movs r3, #1
	movs r1, #36
	ldrsh r0, [r2, r1]
	negs r3, r3
	movs r2, #0
	str r2, [sp, #0]
	adds r1, r3, #0
	movs r2, #3
	bl Func_0814cd48
.L_0813eeaa:
	ldr r3, [sp, #40]
	cmp r3, #32
	bne .L_0813eeb6
	movs r0, #149
	bl Audio_PlayCue
.L_0813eeb6:
	ldr r4, [sp, #40]
	cmp r4, #5
	bne .L_0813eee6
	movs r0, #145
	bl Audio_PlayCue
	ldr r5, [sp, #48]
	movs r7, #238
	lsls r7, r7, #7
	adds r7, #160
	adds r3, r5, r7
	ldr r2, .L_0813ef38
	ldr r3, [r3]
	movs r1, #206
	strh r3, [r2, #4]
	ldr r0, [sp, #28]
	lsls r1, r1, #3
	adds r3, r0, r1
	movs r2, #1
	ldrh r1, [r3]
	movs r0, #1
	negs r2, r2
	bl Func_08118040
.L_0813eee6:
	ldr r2, [sp, #40]
	cmp r2, #7
	ble .L_0813efba
	ldr r7, [sp, #28]
	movs r0, #160
	movs r3, #160
	ldr r4, .L_0813ef0c
	lsls r0, r0, #3
	lsls r3, r3, #19
	adds r0, #108
	adds r3, #192
	movs r5, #31
	adds r7, r7, r0
	mov lr, r3
	movs r6, #0
	mov r12, r4
	mov r8, r5
	mov r9, r7
	b .L_0813ef48
.L_0813ef0c:
	.4byte 0x0000001f
.L_0813ef10:
	.4byte Func_08143000
.L_0813ef14:
	.4byte 0xffe00000
.L_0813ef18:
	.4byte Data_081976b1
.L_0813ef1c:
	.4byte Data_08197690
.L_0813ef20:
	.4byte 0xffc40000
.L_0813ef24:
	.4byte IwramCopyWords
.L_0813ef28:
	.4byte 0x06008000
.L_0813ef2c:
	.4byte gMapCellBuffer
.L_0813ef30:
	.4byte IwramFillWords
.L_0813ef34:
	.4byte 0x01010101
.L_0813ef38:
	.4byte Data_03001120
.L_0813ef3c:
	.4byte Data_020038e0
.L_0813ef40:
	.4byte 0x04000208
.L_0813ef44:
	.4byte 0x7fff7fff
.L_0813ef48:
	mov r1, lr
	ldrh r3, [r1]
	mov r5, r8
	mov r7, r9
	ands r5, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r0, r3, #26
	ldrh r3, [r7]
	movs r1, #2
	add r9, r1
	mov r1, r8
	ands r1, r3
	lsls r3, r3, #16
	mov r4, r12
	mov r10, r3
	mov r7, r10
	ands r2, r4
	ands r0, r4
	lsrs r4, r3, #21
	mov r3, r12
	ands r4, r3
	lsrs r3, r7, #26
	mov r7, r12
	ands r3, r7
	cmp r5, r1
	bge .L_0813ef82
	adds r5, #1
	b .L_0813ef88
.L_0813ef82:
	cmp r5, r1
	ble .L_0813ef88
	subs r5, #1
.L_0813ef88:
	cmp r2, r4
	bge .L_0813ef90
	adds r2, #1
	b .L_0813ef96
.L_0813ef90:
	cmp r2, r4
	ble .L_0813ef96
	subs r2, #1
.L_0813ef96:
	cmp r0, r3
	bge .L_0813ef9e
	adds r0, #1
	b .L_0813efa4
.L_0813ef9e:
	cmp r0, r3
	ble .L_0813efa4
	subs r0, #1
.L_0813efa4:
	lsls r3, r0, #10
	lsls r2, r2, #5
	orrs r3, r2
	mov r0, lr
	orrs r3, r5
	movs r1, #2
	adds r6, #1
	strh r3, [r0]
	add lr, r1
	cmp r6, #128
	bne .L_0813ef48
.L_0813efba:
	ldr r2, [sp, #40]
	cmp r2, #4
	bne .L_0813efce
	movs r1, #240
	ldr r3, .L_0813f318
	ldr r0, .L_0813f31c
	lsls r1, r1, #7
	ldr r2, .L_0813f320
	mov lr, r3
	.2byte 0xf800
.L_0813efce:
	ldr r3, [sp, #40]
	cmp r3, #3
	ble .L_0813efd6
	b .L_0813f278
.L_0813efd6:
	lsls r1, r3, #2
	adds r1, #8
	lsls r4, r3, #5
	movs r0, #160
	lsls r3, r1, #10
	lsls r2, r1, #5
	lsls r0, r0, #19
	orrs r3, r2
	adds r0, #4
	orrs r3, r1
	str r4, [sp, #16]
	strh r3, [r0]
	ldr r5, [sp, #12]
	cmp r5, r4
	bne .L_0813eff6
	b .L_0813f26a
.L_0813eff6:
	ldr r7, [sp, #12]
	movs r0, #0
	adds r1, r7, #0
	mov r10, r7
	mov r11, r0
	str r7, [sp, #8]
	cmp r1, #0
	bge .L_0813f008
	b .L_0813f25c
.L_0813f008:
	ldr r3, [sp, #24]
	mov r4, r10
	ldr r5, [sp, #20]
	ldr r2, [sp, #24]
	subs r0, r3, r4
	ldr r4, [sp, #20]
	mov r7, r11
	subs r6, r5, r7
	add r2, r10
	add r4, r11
	cmp r6, #0
	bge .L_0813f022
	movs r6, #0
.L_0813f022:
	cmp r4, #119
	ble .L_0813f028
	movs r4, #119
.L_0813f028:
	cmp r0, #0
	bge .L_0813f02e
	movs r0, #0
.L_0813f02e:
	cmp r2, #255
	ble .L_0813f034
	movs r2, #255
.L_0813f034:
	movs r5, #7
	ands r5, r2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0813f040
	adds r3, r2, #7
.L_0813f040:
	movs r2, #7
	asrs r1, r3, #3
	ands r2, r4
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0813f04e
	adds r3, r4, #7
.L_0813f04e:
	asrs r3, r3, #3
	lsls r3, r3, #11
	mov r12, r3
	lsls r3, r1, #6
	lsls r7, r2, #3
	ldr r1, .L_0813f324
	adds r4, r3, r5
	adds r3, r7, r4
	add r3, r12
	adds r3, r3, r1
	movs r2, #2
	movs r1, #7
	strb r2, [r3]
	ands r1, r6
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0813f072
	adds r3, r6, #7
.L_0813f072:
	asrs r3, r3, #3
	lsls r5, r1, #3
	ldr r2, .L_0813f324
	lsls r6, r3, #11
	adds r3, r5, r4
	adds r3, r6, r3
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r3, #7
	ands r3, r0
	adds r1, r0, #0
	cmp r0, #0
	bge .L_0813f090
	adds r1, r0, #7
.L_0813f090:
	asrs r1, r1, #3
	lsls r1, r1, #6
	adds r1, r1, r3
	ldr r4, .L_0813f324
	adds r3, r7, r1
	add r3, r12
	adds r1, r5, r1
	adds r3, r3, r4
	movs r2, #2
	adds r1, r6, r1
	strb r2, [r3]
	adds r1, r1, r4
	movs r3, #2
	strb r3, [r1]
	movs r0, #96
	mov r1, r10
	subs r3, r0, r1
	mov r2, r10
	adds r0, r3, #1
	adds r2, #97
	cmp r0, #0
	bge .L_0813f0be
	movs r0, #0
.L_0813f0be:
	cmp r2, #255
	ble .L_0813f0c4
	movs r2, #255
.L_0813f0c4:
	movs r1, #7
	ands r1, r2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0813f0d0
	adds r3, r2, #7
.L_0813f0d0:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r3, r3, r1
	ldr r4, .L_0813f324
	adds r2, r7, r3
	adds r3, r5, r3
	add r2, r12
	adds r3, r6, r3
	movs r1, #2
	adds r3, r3, r4
	adds r2, r2, r4
	strb r1, [r2]
	strb r1, [r3]
	movs r3, #7
	ands r3, r0
	adds r1, r0, #0
	cmp r0, #0
	bge .L_0813f0f6
	adds r1, r0, #7
.L_0813f0f6:
	asrs r1, r1, #3
	lsls r1, r1, #6
	adds r1, r1, r3
	adds r3, r7, r1
	ldr r7, .L_0813f324
	ldr r0, .L_0813f324
	adds r1, r5, r1
	add r3, r12
	adds r3, r3, r7
	movs r2, #2
	adds r1, r6, r1
	strb r2, [r3]
	adds r1, r1, r0
	movs r3, #2
	strb r3, [r1]
	movs r1, #96
	mov r3, r11
	mov r2, r11
	subs r0, r1, r3
	mov r4, r10
	movs r5, #60
	mov r7, r10
	adds r2, #96
	mov r9, r0
	adds r4, #60
	subs r6, r5, r7
	cmp r0, #0
	bge .L_0813f130
	movs r0, #0
.L_0813f130:
	cmp r2, #255
	ble .L_0813f136
	movs r2, #255
.L_0813f136:
	cmp r6, #0
	bge .L_0813f13c
	movs r6, #0
.L_0813f13c:
	cmp r4, #119
	ble .L_0813f142
	movs r4, #119
.L_0813f142:
	movs r5, #7
	ands r5, r2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0813f14e
	adds r3, r2, #7
.L_0813f14e:
	movs r2, #7
	asrs r1, r3, #3
	ands r2, r4
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0813f15c
	adds r3, r4, #7
.L_0813f15c:
	asrs r3, r3, #3
	lsls r3, r3, #11
	mov r8, r3
	lsls r3, r1, #6
	lsls r2, r2, #3
	ldr r1, .L_0813f324
	adds r5, r3, r5
	adds r3, r2, r5
	add r3, r8
	adds r3, r3, r1
	mov lr, r2
	movs r1, #7
	movs r2, #2
	strb r2, [r3]
	ands r1, r6
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0813f182
	adds r3, r6, #7
.L_0813f182:
	asrs r3, r3, #3
	lsls r3, r3, #11
	lsls r7, r1, #3
	ldr r2, .L_0813f324
	mov r12, r3
	adds r3, r7, r5
	add r3, r12
	adds r3, r3, r2
	movs r5, #2
	strb r5, [r3]
	movs r3, #7
	ands r3, r0
	adds r1, r0, #0
	cmp r0, #0
	bge .L_0813f1a2
	adds r1, r0, #7
.L_0813f1a2:
	asrs r1, r1, #3
	lsls r1, r1, #6
	adds r1, r1, r3
	mov r0, lr
	ldr r2, .L_0813f324
	adds r3, r0, r1
	add r3, r8
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r3, .L_0813f324
	adds r1, r7, r1
	add r1, r12
	mov r0, r9
	adds r1, r1, r3
	mov r2, r11
	movs r3, #2
	adds r0, #1
	strb r3, [r1]
	adds r2, #97
	cmp r0, #0
	bge .L_0813f1d0
	movs r0, #0
.L_0813f1d0:
	cmp r2, #255
	ble .L_0813f1d6
	movs r2, #255
.L_0813f1d6:
	movs r5, #7
	adds r1, r2, #0
	mov r9, r5
	ands r1, r5
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0813f1e6
	adds r3, r2, #7
.L_0813f1e6:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r1, r3, r1
	mov r5, lr
	adds r3, r5, r1
	ldr r5, .L_0813f324
	add r3, r8
	adds r3, r3, r5
	movs r5, #2
	strb r5, [r3]
	adds r3, r7, r1
	ldr r1, .L_0813f324
	add r3, r12
	adds r3, r3, r1
	strb r5, [r3]
	adds r2, r0, #0
	mov r3, r9
	ands r2, r3
	adds r3, r0, #0
	cmp r0, #0
	bge .L_0813f212
	adds r3, r0, #7
.L_0813f212:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r1, r3, r2
	mov r4, lr
	ldr r2, .L_0813f324
	adds r3, r4, r1
	add r3, r8
	adds r3, r3, r2
	ldr r4, .L_0813f324
	strb r5, [r3]
	adds r3, r7, r1
	add r3, r12
	adds r3, r3, r4
	movs r5, #2
	strb r5, [r3]
	ldr r0, [sp, #8]
	mov r7, r11
	lsls r3, r7, #1
	subs r3, r0, r3
	subs r3, #1
	str r3, [sp, #8]
	cmp r3, #0
	bge .L_0813f252
	ldr r2, [sp, #8]
	mov r1, r10
	lsls r3, r1, #1
	adds r3, r2, r3
	subs r3, #2
	str r3, [sp, #8]
	movs r3, #1
	negs r3, r3
	add r10, r3
.L_0813f252:
	movs r4, #1
	add r11, r4
	cmp r10, r11
	blt .L_0813f25c
	b .L_0813f008
.L_0813f25c:
	ldr r5, [sp, #12]
	ldr r7, [sp, #16]
	adds r5, #1
	str r5, [sp, #12]
	cmp r5, r7
	beq .L_0813f26a
	b .L_0813eff6
.L_0813f26a:
	movs r2, #240
	ldr r3, .L_0813f328
	ldr r0, .L_0813f31c
	ldr r1, .L_0813f324
	lsls r2, r2, #7
	mov lr, r3
	.2byte 0xf800
.L_0813f278:
	ldr r0, [sp, #40]
	cmp r0, #50
	bgt .L_0813f2c8
	ldr r5, [sp, #48]
	movs r6, #0
.L_0813f282:
	ldr r2, .L_0813f32c
	ldr r0, .L_0813f330
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #48]
	ldrb r0, [r0, r6]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	ldr r0, .L_0813f334
	ldr r4, [sp, #32]
	ldrb r0, [r0, r6]
	str r0, [sp, #4]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	ldr r7, [sp, #40]
	cmp r7, #3
	ble .L_0813f2c0
	movs r2, #128
	adds r0, r5, #0
	movs r1, #64
	lsls r2, r2, #7
	bl BattleFxKernels_IntegrateVector2
.L_0813f2c0:
	adds r6, #1
	adds r5, #28
	cmp r6, #33
	bne .L_0813f282
.L_0813f2c8:
	ldr r1, [sp, #40]
	subs r1, #8
	cmp r1, #42
	bhi .L_0813f2e8
	adds r0, r1, #0
	cmp r0, #31
	ble .L_0813f2d8
	movs r0, #31
.L_0813f2d8:
	movs r3, #160
	lsls r2, r0, #10
	lsls r1, r0, #5
	lsls r3, r3, #19
	orrs r2, r1
	adds r3, #2
	orrs r2, r0
	strh r2, [r3]
.L_0813f2e8:
	ldr r0, [sp, #40]
	cmp r0, #51
	bne .L_0813f3c6
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0813f338
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r0, #160
	lsls r0, r0, #19
	movs r6, #1
	adds r0, #2
.L_0813f308:
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r1, r3, #1
	cmp r1, #0
	bge .L_0813f33c
	movs r1, #0
	b .L_0813f33c
	.2byte 0x0000
.L_0813f318:
	.4byte IwramFillWords
.L_0813f31c:
	.4byte 0x06008000
.L_0813f320:
	.4byte Data_02020202
.L_0813f324:
	.4byte gMapCellBuffer
.L_0813f328:
	.4byte IwramCopyWords
.L_0813f32c:
	.4byte Data_0819764c
.L_0813f330:
	.4byte Data_08197609
.L_0813f334:
	.4byte Data_0819762a
.L_0813f338:
	.4byte 0x0000013e
.L_0813f33c:
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	lsls r2, r1, #10
	lsls r3, r3, #5
	orrs r2, r3
	orrs r2, r1
	adds r6, #1
	strh r2, [r0]
	adds r0, #2
	cmp r6, #64
	bne .L_0813f308
	movs r2, #128
	ldr r3, .L_0813f394
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r5, [sp, #48]
	movs r4, #31
	movs r6, #0
	mov r8, r4
	movs r7, #0
.L_0813f368:
	bl Random16
	mov r1, r8
	ands r0, r1
	adds r0, #32
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	mov r2, r8
	ands r0, r2
	adds r0, #80
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #128
	lsls r3, r3, #1
	ldr r4, .L_0813f398
	adds r3, #255
	ands r3, r0
	b .L_0813f39c
.L_0813f394:
	.4byte 0x00003f44
.L_0813f398:
	.4byte 0xffffff00
.L_0813f39c:
	adds r3, r3, r4
	lsls r3, r3, #12
	adds r6, #1
	str r3, [r5, #12]
	str r7, [r5, #16]
	str r7, [r5, #24]
	adds r5, #28
	cmp r6, #32
	bne .L_0813f368
	ldr r5, [sp, #48]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r5, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	adds r2, r5, r0
	movs r3, #50
	str r3, [r2]
.L_0813f3c6:
	ldr r1, [sp, #40]
	cmp r1, #52
	ble .L_0813f438
	ldr r5, [sp, #48]
	movs r6, #0
.L_0813f3d0:
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0813f3d8
	adds r3, r6, #3
.L_0813f3d8:
	ldr r2, [sp, #40]
	asrs r3, r3, #2
	adds r3, #52
	cmp r2, r3
	blt .L_0813f430
	ldr r3, [r5, #24]
	cmp r3, #39
	bgt .L_0813f430
	adds r1, r3, #0
	cmp r1, #0
	bge .L_0813f3f0
	adds r1, #3
.L_0813f3f0:
	asrs r1, r1, #2
	cmp r1, #5
	ble .L_0813f3f8
	movs r1, #5
.L_0813f3f8:
	ldr r3, [sp, #48]
	lsls r1, r1, #11
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r3, r1
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r4, #224
	movs r0, #32
	str r0, [sp, #0]
	lsls r4, r4, #3
	movs r0, #64
	adds r1, r1, r4
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #44]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_0813f504
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0813f430:
	adds r6, #1
	adds r5, #28
	cmp r6, #32
	bne .L_0813f3d0
.L_0813f438:
	bl Func_081434f8
	movs r7, #240
	ldr r5, [sp, #48]
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r5, r7
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #40]
	adds r0, #1
	str r0, [sp, #40]
	cmp r0, #128
	beq .L_0813f45c
	b .L_0813ee76
.L_0813f45c:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0813f508
	bl Scheduler_RemoveCallback
	ldr r2, [sp, #52]
	movs r3, #1
	movs r1, #36
	ldrsh r0, [r2, r1]
	negs r3, r3
	movs r2, #0
	str r2, [sp, #0]
	adds r1, r3, #0
	movs r2, #1
	bl Func_0814cd48
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #160
	adds r3, r5, r4
	ldr r2, .L_0813f50c
	ldr r3, [r3]
	movs r7, #206
	strh r3, [r2, #4]
	movs r3, #32
	strh r3, [r2, #6]
	ldr r5, [sp, #28]
	lsls r7, r7, #3
	adds r3, r5, r7
	ldrh r1, [r3]
	movs r2, #0
	movs r0, #2
	bl Func_08118038
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0813f510
	ldr r0, .L_0813f514
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0813f4e0
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #234
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0813f4e0:
	strh r4, [r0]
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813f504:
	.4byte 0xfffff000
.L_0813f508:
	.4byte Func_08143000
.L_0813f50c:
	.4byte Data_03001120
.L_0813f510:
	.4byte Data_020038e0
.L_0813f514:
	.4byte 0x04000208
