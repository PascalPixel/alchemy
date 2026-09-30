.syntax unified
	.thumb
	.global Func_08148e9c
	.thumb_func
Func_08148e9c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #52
	str r0, [sp, #40]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	movs r7, #0
	str r0, [sp, #36]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #32]
	bl Func_081435e0
	ldr r3, .L_08148f00
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r2, [sp, #36]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08148f04
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r0, #188
	movs r1, #23
	str r3, [sp, #44]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r0, sp
	adds r0, #44
	str r0, [sp, #24]
	str r3, [r0, #4]
	b .L_08148f08
.L_08148f00:
	.4byte 0x00001010
.L_08148f04:
	.4byte 0x00000192
.L_08148f08:
	ldr r5, [sp, #36]
.L_08148f0a:
	bl Random16
	movs r3, #31
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	negs r3, r3
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #16
	bne .L_08148f0a
	ldr r1, [sp, #36]
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
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_08148f94
	bl Func_080145a8
	ldr r3, .L_08148f90
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #141
	bl Audio_PlayCue
	movs r1, #0
	mov r11, r1
.L_08148f70:
	mov r2, r11
	lsls r0, r2, #10
	bl Trig_Sin
	mov r3, r11
	lsls r0, r0, #4
	str r0, [sp, #28]
	cmp r3, #32
	bne .L_08148f88
	movs r0, #133
	bl Func_08118088 + 0x60
.L_08148f88:
	ldr r6, .L_08148f98
	movs r0, #0
	mov r9, r0
	b .L_08148f9c
.L_08148f90:
	.4byte 0x00001000
.L_08148f94:
	.4byte Func_08143000
.L_08148f98:
	.4byte IwramFillWords
.L_08148f9c:
	movs r5, #16
.L_08148f9e:
	cmp r11, r5
	bne .L_08148fae
	movs r1, #128
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_08148fd0
	mov lr, r6
	.2byte 0xf800
.L_08148fae:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #8
	cmp r2, #7
	bne .L_08148f9e
	ldr r0, [sp, #40]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_08148fd4
	ldr r1, [sp, #28]
	movs r2, #128
	lsls r2, r2, #14
	adds r1, r1, r2
	str r1, [sp, #28]
	b .L_08148fdc
	.2byte 0x0000
.L_08148fd0:
	.4byte Text_MessageContexts + 0x1fbd8
.L_08148fd4:
	ldr r3, [sp, #28]
	ldr r0, .L_08149020
	adds r3, r3, r0
	str r3, [sp, #28]
.L_08148fdc:
	mov r1, r11
	cmp r1, #16
	bgt .L_08148ff0
	ldr r2, .L_08149018
	movs r1, #128
	lsls r1, r1, #19
	mov r3, r11
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
.L_08148ff0:
	mov r2, r11
	cmp r2, #63
	ble .L_08149008
	ldr r2, .L_0814901c
	ldr r1, .L_08149018
	movs r3, #128
	mov r0, r11
	lsls r3, r3, #19
	subs r2, r2, r0
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08149008:
	ldr r2, [sp, #40]
	ldr r0, .L_08149024
	ldr r3, [r2, #24]
	movs r1, #0
	lsls r2, r3, #1
	adds r2, r2, r3
	ldrb r3, [r0, r2]
	b .L_08149028
.L_08149018:
	.4byte 0x00001000
.L_0814901c:
	.4byte 0x0000004f
.L_08149020:
	.4byte 0xffe00000
.L_08149024:
	.4byte Data_081979ae
.L_08149028:
	mov r9, r1
	cmp r3, #0
	bne .L_08149030
	b .L_08149190
.L_08149030:
	mov r1, r11
	mov r2, r11
	asrs r1, r1, #31
	lsls r2, r2, #11
	movs r3, #0
	str r1, [sp, #20]
	str r2, [sp, #16]
	str r3, [sp, #12]
.L_08149040:
	ldr r0, [sp, #16]
	bl Trig_Sin
	ldr r1, [sp, #40]
	movs r7, #0
	ldr r2, [r1, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_08149254
	adds r3, #1
	ldrb r3, [r2, r3]
	muls r3, r0
	ldr r0, [sp, #28]
	adds r3, r3, r0
	asrs r3, r3, #16
	ldr r0, [sp, #16]
	adds r3, #40
	mov r10, r3
	bl Trig_Cos
	ldr r1, [sp, #20]
	lsls r0, r0, #1
	asrs r0, r0, #16
	mov r8, r0
	lsrs r0, r1, #31
	add r0, r11
	movs r1, #3
	asrs r0, r0, #1
	bl Math_Mod
	ldr r2, [sp, #36]
	lsls r5, r0, #2
	adds r5, r5, r0
	movs r3, #152
	lsls r6, r5, #9
	lsls r3, r3, #5
	adds r6, r2, r6
	adds r3, #86
	adds r1, r6, r3
	movs r0, #40
	movs r2, #32
	mov r3, r8
	str r0, [sp, #0]
	str r2, [sp, #4]
	ldr r4, [sp, #44]
	ldr r0, [sp, #32]
	adds r3, #16
	mov r2, r10
	mov lr, r4
	.2byte 0xf800
	ldr r3, [sp, #36]
	movs r0, #196
	lsls r5, r5, #8
	lsls r0, r0, #6
	adds r5, r3, r5
	adds r0, #86
	adds r5, r5, r0
	movs r1, #40
	movs r2, #32
	mov r3, r8
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r4, [sp, #44]
	adds r3, #48
	ldr r0, [sp, #32]
	adds r1, r5, #0
	mov r2, r10
	mov lr, r4
	.2byte 0xf800
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #86
	adds r6, r6, r3
	movs r0, #40
	movs r1, #32
	mov r3, r8
	adds r3, #80
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r2, r10
	adds r1, r6, #0
	ldr r4, [sp, #44]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [sp, #12]
	ldr r3, [sp, #36]
	adds r6, r2, r3
.L_081490f0:
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_08149140
	lsrs r2, r7, #31
	adds r2, r7, r2
	asrs r2, r2, #1
	lsrs r4, r3, #31
	adds r4, r3, r4
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r0, #1
	ldr r2, .L_08149258
	asrs r4, r4, #1
	adds r5, r7, #0
	adds r4, r4, r3
	ands r5, r0
	ldr r0, .L_0814925c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #36]
	ldrb r0, [r0, r4]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	ldr r2, [r6]
	adds r1, r1, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	ldr r0, .L_08149260
	lsls r5, r5, #2
	ldrb r0, [r0, r4]
	add r3, r8
	str r0, [sp, #4]
	ldr r0, [sp, #24]
	add r2, r10
	ldr r4, [r5, r0]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
.L_08149140:
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #6
	bne .L_08149162
	bl Random16
	movs r3, #31
	ands r3, r0
	str r3, [r6]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #16
	str r3, [r6, #4]
	movs r3, #0
	str r3, [r6, #24]
.L_08149162:
	adds r7, #1
	adds r6, #28
	cmp r7, #4
	bne .L_081490f0
	ldr r1, [sp, #16]
	ldr r3, [sp, #12]
	movs r2, #128
	lsls r2, r2, #7
	adds r1, r1, r2
	adds r3, #112
	str r1, [sp, #16]
	str r3, [sp, #12]
	ldr r1, [sp, #40]
	movs r0, #1
	ldr r3, [r1, #24]
	add r9, r0
	ldr r0, .L_08149254
	lsls r2, r3, #1
	adds r2, r2, r3
	ldrb r3, [r0, r2]
	cmp r9, r3
	beq .L_08149190
	b .L_08149040
.L_08149190:
	ldr r1, [sp, #40]
	movs r7, #0
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_081491ea
	movs r2, #0
	mov r8, r2
	movs r4, #36
.L_081491a0:
	movs r3, #0
	mov r5, r8
	mov r9, r3
	adds r6, r4, #0
	adds r5, #16
.L_081491aa:
	cmp r11, r5
	bne .L_081491ce
	ldr r1, [sp, #40]
	movs r3, #4
	ldrsh r0, [r1, r6]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	adds r3, r7, #0
	str r4, [sp, #8]
	bl Func_0814cd48
	ldr r3, [sp, #40]
	ldrsh r0, [r3, r6]
	movs r1, #6
	bl Func_08118088
	ldr r4, [sp, #8]
.L_081491ce:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r5, #8
	cmp r3, #7
	bne .L_081491aa
	ldr r1, [sp, #40]
	movs r0, #3
	ldr r3, [r1, #20]
	adds r7, #1
	add r8, r0
	adds r4, #2
	cmp r7, r3
	bne .L_081491a0
.L_081491ea:
	ldr r2, [sp, #36]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r2, r0
	movs r1, #1
	str r1, [r3]
	ldr r3, [sp, #40]
	ldr r1, .L_08149254
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, #2
	ldrb r0, [r1, r3]
	lsls r1, r0, #1
	bl Func_08158ce0
	bl Func_081434f8
	movs r0, #240
	ldr r2, [sp, #36]
	lsls r0, r0, #7
	adds r0, #232
	adds r3, r2, r0
	movs r1, #1
	str r1, [r3]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r11, r2
	mov r3, r11
	cmp r3, #80
	beq .L_08149230
	b .L_08148f70
.L_08149230:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08149264
	bl Func_08014644
	bl Func_08143bb8
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08149254:
	.4byte Data_081979ae
.L_08149258:
	.4byte Data_08197486
.L_0814925c:
	.4byte Data_08197492
.L_08149260:
	.4byte Data_08197498
.L_08149264:
	.4byte Func_08143000
