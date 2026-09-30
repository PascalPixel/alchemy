.syntax unified
	.thumb
	.global Func_0818ad40
	.thumb_func
Func_0818ad40:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #84
	str r0, [sp, #48]
	str r1, [sp, #44]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	ldr r0, [r3, #92]
	movs r5, #128
	str r1, [sp, #40]
	lsls r5, r5, #6
	adds r5, #1
	mov r10, r0
	adds r0, r5, #0
	ldr r6, [r3, #100]
	bl Func_08143a88
	ldr r2, [sp, #48]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0818ad7e
	adds r0, r5, #0
	bl BattleFx_BeginTiledCanvas
	b .L_0818ad84
.L_0818ad7e:
	adds r0, r5, #0
	bl Func_08143a88
.L_0818ad84:
	ldr r3, .L_0818adb0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #44]
	cmp r3, #3
	bne .L_0818adb4
	ldr r4, [sp, #48]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0818ada6
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	b .L_0818adce
.L_0818ada6:
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
	b .L_0818adce
.L_0818adb0:
	.4byte 0x00000210
.L_0818adb4:
	ldr r0, [sp, #48]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0818adc6
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_0818adce
.L_0818adc6:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_0818adce:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #0
	str r3, [sp, #32]
	str r1, [sp, #28]
	str r1, [sp, #24]
	str r1, [sp, #20]
	movs r2, #0
	ldr r0, .L_0818aeb8
	adds r1, r6, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #44]
	cmp r2, #0
	bne .L_0818ae10
	ldr r0, .L_0818aebc
	ldr r1, .L_0818aec0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	movs r3, #0
	ldr r0, .L_0818aec4
	add r1, r10
	movs r2, #0
	bl Func_08157cf4
	movs r3, #96
	b .L_0818af02
.L_0818ae10:
	ldr r4, [sp, #44]
	cmp r4, #1
	bne .L_0818ae38
	ldr r0, .L_0818aec8
	ldr r1, .L_0818aec0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0818aec4
	add r1, r10
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r6, #80
	str r6, [sp, #16]
	b .L_0818af04
.L_0818ae38:
	ldr r0, [sp, #44]
	cmp r0, #3
	bne .L_0818aed8
	ldr r0, .L_0818aecc
	ldr r1, .L_0818aec0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0818aed0
	add r1, r10
	movs r2, #1
	movs r3, #0
	movs r5, #178
	bl Func_08157cf4
	lsls r5, r5, #6
	mov r2, r10
	adds r1, r2, r5
	movs r3, #0
	ldr r0, .L_0818aed4
	movs r2, #0
	movs r6, #174
	bl Func_08157cf4
	lsls r6, r6, #2
	movs r3, #1
	mov r8, r3
	mov r12, r6
	movs r7, #6
	mov lr, r5
.L_0818ae7a:
	mov r1, r10
	adds r3, r6, r5
	movs r0, #0
	adds r4, r7, #0
	add r1, lr
	add r3, r10
.L_0818ae86:
	ldrb r2, [r1]
	adds r1, #1
	cmp r2, #0
	ble .L_0818ae96
	subs r2, r2, r4
	cmp r2, #0
	bgt .L_0818ae96
	movs r2, #1
.L_0818ae96:
	adds r0, #1
	strb r2, [r3]
	adds r3, #1
	cmp r0, r12
	bne .L_0818ae86
	movs r0, #1
	movs r4, #174
	add r8, r0
	lsls r4, r4, #2
	mov r1, r8
	adds r6, r6, r4
	adds r7, #6
	cmp r1, #8
	bne .L_0818ae7a
	movs r2, #116
	str r2, [sp, #16]
	b .L_0818af04
.L_0818aeb8:
	.4byte 0x00000134
.L_0818aebc:
	.4byte 0x000000e3
.L_0818aec0:
	.4byte Data_0201603e
.L_0818aec4:
	.4byte 0x000000cf
.L_0818aec8:
	.4byte 0x000000e2
.L_0818aecc:
	.4byte 0x000000e5
.L_0818aed0:
	.4byte 0x0000015f
.L_0818aed4:
	.4byte 0x00000160
.L_0818aed8:
	ldr r0, .L_0818af5c
	ldr r1, .L_0818af60
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0818af64
	add r1, r10
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r3, #0
	ldr r0, .L_0818af68
	ldr r1, .L_0818af6c
	movs r2, #1
	bl Func_08157cf4
	movs r3, #88
.L_0818af02:
	str r3, [sp, #16]
.L_0818af04:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r5, #200
	add r2, r10
	movs r3, #0
	lsls r5, r5, #4
	str r3, [r2]
	ldr r0, .L_0818af70
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
	ldr r4, [sp, #44]
	cmp r4, #1
	bne .L_0818af34
	ldr r0, .L_0818af74
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
.L_0818af34:
	movs r2, #128
	ldr r3, .L_0818af58
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r0, [sp, #16]
	movs r6, #0
	mov r11, r6
	cmp r0, #0
	bne .L_0818af4c
	bl .L_0818b80a
.L_0818af4c:
	movs r1, #48
	subs r0, #8
	negs r1, r1
	str r0, [sp, #12]
	str r1, [sp, #8]
	b .L_0818af78
.L_0818af58:
	.4byte 0x00000100
.L_0818af5c:
	.4byte 0x000000e4
.L_0818af60:
	.4byte Data_0201603e
.L_0818af64:
	.4byte 0x0000016f
.L_0818af68:
	.4byte 0x00000170
.L_0818af6c:
	.4byte gMapCellBuffer
.L_0818af70:
	.4byte Func_08143000
.L_0818af74:
	.4byte Func_08152474
.L_0818af78:
	mov r2, r11
	cmp r2, #0
	bne .L_0818afe4
	movs r0, #190
	bl Audio_PlayCue
	ldr r3, [sp, #48]
	add r6, sp, #72
	ldr r0, [r3, #8]
	adds r1, r6, #0
	bl Func_0815e1fc
	add r5, sp, #60
	adds r0, r5, #0
	bl Func_0815e22c
	ldr r4, [sp, #44]
	cmp r4, #2
	bne .L_0818afb2
	ldr r0, [sp, #48]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0818afac
	ldr r3, [r5]
	adds r3, #32
	b .L_0818afb0
.L_0818afac:
	ldr r3, [r5]
	subs r3, #32
.L_0818afb0:
	str r3, [r5]
.L_0818afb2:
	ldr r3, [r6]
	movs r2, #56
	movs r1, #128
	subs r3, r2, r3
	lsls r1, r1, #19
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	movs r1, #0
	ldr r3, [r6]
	str r1, [sp, #28]
	subs r2, r2, r3
	str r2, [sp, #24]
	mov r3, r10
	movs r2, #1
	mov r8, r1
	negs r2, r2
	adds r3, #24
.L_0818afd6:
	movs r4, #1
	add r8, r4
	mov r6, r8
	str r2, [r3]
	adds r3, #28
	cmp r6, #64
	bne .L_0818afd6
.L_0818afe4:
	ldr r0, [sp, #12]
	cmp r11, r0
	blt .L_0818b008
	ldr r3, [sp, #16]
	mov r1, r11
	subs r2, r1, r3
	adds r2, #8
	ldr r3, .L_0818b01c
	adds r1, r2, #0
	movs r0, #128
	adds r1, #8
	lsls r2, r2, #1
	lsls r0, r0, #19
	lsls r1, r1, #8
	subs r3, r3, r2
	adds r0, #82
	orrs r1, r3
	strh r1, [r0]
.L_0818b008:
	mov r4, r11
	cmp r4, #48
	bne .L_0818b020
	ldr r6, [sp, #44]
	cmp r6, #1
	bne .L_0818b020
	movs r0, #138
	bl Audio_PlayCue
	b .L_0818b020
.L_0818b01c:
	.4byte 0x00000010
.L_0818b020:
	mov r0, r11
	cmp r0, #47
	bgt .L_0818b07a
	add r6, sp, #72
	ldr r2, [r6]
	ldr r3, [sp, #60]
	movs r1, #48
	subs r3, r3, r2
	mov r0, r11
	muls r0, r3
	adds r5, r2, #0
	bl Math_Div
	subs r5, #64
	adds r3, r5, r0
	cmp r3, #0
	bge .L_0818b048
	str r3, [sp, #20]
	movs r3, #0
	b .L_0818b052
.L_0818b048:
	cmp r3, #112
	ble .L_0818b052
	subs r3, #112
	str r3, [sp, #20]
	movs r3, #112
.L_0818b052:
	movs r2, #128
	lsls r3, r3, #8
	lsls r2, r2, #19
	negs r3, r3
	adds r2, #40
	str r3, [r2]
	str r3, [sp, #24]
	movs r2, #128
	ldr r3, [r6, #4]
	lsls r2, r2, #3
	subs r3, #1
	str r3, [r6, #4]
	ldr r1, [sp, #28]
	movs r3, #128
	adds r1, r1, r2
	lsls r3, r3, #8
	str r1, [sp, #28]
	cmp r1, r3
	ble .L_0818b07a
	str r3, [sp, #28]
.L_0818b07a:
	ldr r4, [sp, #44]
	cmp r4, #0
	beq .L_0818b150
	cmp r4, #1
	bne .L_0818b0b4
	movs r6, #225
	lsls r6, r6, #7
	movs r0, #0
	mov r1, r11
	add r6, r10
	mov r8, r0
	lsls r5, r1, #12
.L_0818b092:
	adds r0, r5, #0
	bl Trig_Sin
	ldr r2, [sp, #24]
	lsls r0, r0, #2
	asrs r0, r0, #10
	movs r4, #1
	subs r0, r2, r0
	movs r3, #128
	add r8, r4
	stmia r6!, {r0}
	lsls r3, r3, #5
	mov r0, r8
	adds r5, r5, r3
	cmp r0, #160
	bne .L_0818b092
	b .L_0818b14a
.L_0818b0b4:
	ldr r3, [sp, #44]
	ldr r1, .L_0818b204
	subs r3, #2
	mov r9, r1
	cmp r3, #1
	bhi .L_0818b0ce
	mov r3, r11
	subs r3, #40
	cmp r3, #7
	bhi .L_0818b0ce
	ldr r0, .L_0818b208
	bl Func_0815f0a0
.L_0818b0ce:
	mov r2, r11
	cmp r2, #51
	ble .L_0818b0e8
	ldr r3, [sp, #44]
	cmp r3, #2
	bne .L_0818b0e2
	ldr r0, .L_0818b20c
	bl Func_0815f0a0
	b .L_0818b0e8
.L_0818b0e2:
	ldr r0, .L_0818b210
	bl Func_0815f0a0
.L_0818b0e8:
	mov r4, r11
	cmp r4, #48
	bne .L_0818b106
	movs r6, #128
	lsls r6, r6, #1
	movs r7, #15
	mov r5, r9
	add r6, r9
.L_0818b0f8:
	bl Random16
	ands r0, r7
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_0818b0f8
.L_0818b106:
	ldr r6, [sp, #8]
	cmp r6, #15
	bhi .L_0818b14a
	ldr r7, .L_0818b214
	movs r0, #0
	movs r1, #7
	mov r8, r0
	mov lr, r1
	mov r12, r6
	movs r5, #0
	movs r4, #0
.L_0818b11c:
	mov r2, lr
	mov r3, r8
	ands r3, r2
	lsls r3, r3, #5
	mov r6, r9
	movs r0, #0
	adds r1, r4, r7
	adds r2, r3, r6
.L_0818b12c:
	ldrb r3, [r2]
	adds r2, #1
	cmp r3, r12
	bne .L_0818b136
	strb r5, [r1]
.L_0818b136:
	adds r0, #1
	adds r1, #1
	cmp r0, #32
	bne .L_0818b12c
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r4, #32
	cmp r1, #64
	bne .L_0818b11c
.L_0818b14a:
	ldr r2, [sp, #44]
	cmp r2, #0
	bne .L_0818b154
.L_0818b150:
	movs r3, #120
	b .L_0818b15e
.L_0818b154:
	ldr r4, [sp, #44]
	movs r3, #72
	cmp r4, #3
	beq .L_0818b15e
	movs r3, #80
.L_0818b15e:
	cmp r11, r3
	blt .L_0818b164
	b .L_0818b27a
.L_0818b164:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0818b218
	ldr r3, [sp, #52]
	adds r5, r0, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0818b21c
	str r6, [r5, #12]
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #52]
	ldr r3, .L_0818b214
	add r2, sp, #52
	str r3, [r2, #4]
	movs r3, #8
	str r3, [r5]
	ldr r3, .L_0818b220
	str r2, [r5, #16]
	str r3, [r5, #8]
	bl Func_08014de4
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_080151e4
	ldr r1, [sp, #20]
	ldr r2, .L_0818b224
	lsls r0, r1, #16
	ldr r1, [sp, #76]
	lsls r1, r1, #16
	adds r1, r1, r2
	movs r2, #0
	bl Func_08015160
	mov r3, r11
	cmp r3, #47
	bgt .L_0818b1f4
	ldr r4, [sp, #48]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0818b1da
	ldr r2, .L_0818b228
	mov r1, r11
	lsls r0, r1, #10
	adds r0, r0, r2
	bl Func_080150e4
	b .L_0818b1e6
.L_0818b1da:
	movs r0, #52
	mov r3, r11
	subs r0, r0, r3
	lsls r0, r0, #10
	bl Func_080150e4
.L_0818b1e6:
	movs r0, #52
	mov r4, r11
	subs r0, r0, r4
	lsls r0, r0, #10
	bl SceneTransform_ApplyPitch
	b .L_0818b240
.L_0818b1f4:
	ldr r0, [sp, #48]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0818b230
	ldr r0, .L_0818b22c
	bl Func_080150e4
	b .L_0818b238
.L_0818b204:
	.4byte Data_0201683e
.L_0818b208:
	.4byte 0x00000161
.L_0818b20c:
	.4byte 0x00000170
.L_0818b210:
	.4byte 0x0000015f
.L_0818b214:
	.4byte Data_0201603e
.L_0818b218:
	.4byte 0xffffff00
.L_0818b21c:
	.4byte 0xffff00ff
.L_0818b220:
	.4byte Data_081992b0
.L_0818b224:
	.4byte 0xffc00000
.L_0818b228:
	.4byte 0xffff3000
.L_0818b22c:
	.4byte 0xfffff000
.L_0818b230:
	movs r0, #128
	lsls r0, r0, #5
	bl Func_080150e4
.L_0818b238:
	movs r0, #128
	lsls r0, r0, #5
	bl SceneTransform_ApplyPitch
.L_0818b240:
	movs r0, #184
	lsls r0, r0, #5
	adds r0, #112
	bl Func_08015068
	ldr r0, [sp, #28]
	bl Func_0801521c
	movs r2, #128
	lsls r2, r2, #9
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #10
	bl Func_080151e4
	adds r1, r6, #0
	movs r2, #4
	ldr r0, .L_0818b2cc
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r6, #0
	bl Sys_Free
.L_0818b27a:
	ldr r1, [sp, #44]
	cmp r1, #1
	bne .L_0818b372
	mov r2, r11
	cmp r2, #48
	bne .L_0818b2ac
	movs r0, #136
	bl Func_081180e8
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	str r3, [r2]
	movs r2, #128
	ldr r3, .L_0818b2c8
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0818b2ac:
	ldr r3, [sp, #8]
	cmp r3, #31
	bhi .L_0818b304
	lsls r2, r3, #1
	lsls r3, r3, #4
	subs r3, r3, r2
	movs r4, #0
	lsls r3, r3, #2
	mov r0, r10
	mov r8, r4
	movs r6, #127
	adds r5, r3, r0
	b .L_0818b2d0
	.2byte 0x0000
.L_0818b2c8:
	.4byte 0x00001010
.L_0818b2cc:
	.4byte Data_081991e0
.L_0818b2d0:
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #14
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	movs r1, #1
	ands r0, r6
	adds r0, #64
	add r8, r1
	lsls r0, r0, #11
	movs r3, #0
	mov r2, r8
	str r0, [r5, #16]
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #2
	bne .L_0818b2d0
.L_0818b304:
	movs r3, #0
	mov r8, r3
	mov r5, r10
.L_0818b30a:
	ldr r1, [r5, #24]
	cmp r1, #15
	bhi .L_0818b366
	cmp r1, #0
	bge .L_0818b316
	adds r1, #3
.L_0818b316:
	asrs r1, r1, #2
	lsls r1, r1, #10
	movs r4, #224
	movs r6, #2
	ldrsh r2, [r5, r6]
	lsls r4, r4, #3
	ldr r0, [sp, #20]
	add r1, r10
	adds r1, r1, r4
	movs r4, #6
	ldrsh r3, [r5, r4]
	adds r2, r0, r2
	movs r0, #32
	subs r3, #16
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #40]
	ldr r6, [sp, #32]
	mov lr, r6
	.2byte 0xf800
	ldr r2, .L_0818b55c
	adds r0, r5, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #4]
	movs r2, #192
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_0818b360
	movs r3, #0
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	str r2, [r5, #4]
	lsls r3, r3, #1
	str r3, [r5, #12]
.L_0818b360:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0818b366:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_0818b30a
.L_0818b372:
	ldr r2, [sp, #44]
	cmp r2, #0
	bne .L_0818b426
	mov r3, r11
	cmp r3, #48
	bne .L_0818b384
	movs r0, #212
	bl Audio_PlayCue
.L_0818b384:
	mov r4, r11
	cmp r4, #64
	bne .L_0818b392
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
.L_0818b392:
	ldr r6, [sp, #8]
	cmp r6, #31
	bhi .L_0818b3cc
	ldr r0, [sp, #8]
	movs r3, #128
	lsls r5, r0, #3
	subs r5, r5, r0
	lsls r5, r5, #2
	add r5, r10
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #14
	str r3, [r5, #4]
	bl Random16
	movs r6, #127
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #16]
	ldr r1, [sp, #44]
	str r1, [r5, #24]
.L_0818b3cc:
	movs r2, #0
	mov r8, r2
	mov r5, r10
.L_0818b3d2:
	ldr r1, [r5, #24]
	cmp r1, #63
	bhi .L_0818b41a
	cmp r1, #0
	bge .L_0818b3de
	adds r1, #3
.L_0818b3de:
	movs r3, #3
	asrs r1, r1, #2
	ands r1, r3
	ldr r3, .L_0818b560
	movs r4, #2
	ldrsh r2, [r5, r4]
	ldr r6, [sp, #20]
	lsls r1, r1, #10
	adds r1, r1, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	adds r2, r6, r2
	movs r0, #32
	subs r3, #16
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0818b41a:
	movs r6, #1
	add r8, r6
	mov r0, r8
	adds r5, #28
	cmp r0, #64
	bne .L_0818b3d2
.L_0818b426:
	ldr r1, [sp, #44]
	cmp r1, #3
	beq .L_0818b42e
	b .L_0818b5f2
.L_0818b42e:
	mov r2, r11
	cmp r2, #47
	bgt .L_0818b436
	b .L_0818b5f2
.L_0818b436:
	ldr r3, [sp, #8]
	mov r9, r3
	cmp r3, #23
	bgt .L_0818b474
	adds r0, r3, #0
	cmp r3, #0
	bge .L_0818b448
	mov r0, r11
	subs r0, #45
.L_0818b448:
	movs r1, #6
	asrs r0, r0, #2
	bl __modsi3
	lsls r1, r0, #4
	subs r1, r1, r0
	ldr r2, [sp, #20]
	movs r3, #24
	lsls r1, r1, #6
	movs r4, #224
	add r1, r10
	lsls r4, r4, #3
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	adds r1, r1, r4
	adds r2, #52
	ldr r0, [sp, #40]
	movs r3, #44
	ldr r6, [sp, #32]
	mov lr, r6
	.2byte 0xf800
.L_0818b474:
	mov r0, r9
	cmp r0, #24
	bne .L_0818b536
	movs r5, #1
	movs r0, #212
	negs r5, r5
	bl Audio_PlayCue
	adds r0, r5, #0
	bl Func_081180e8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #4
	str r3, [r2]
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_0818b564
	ldr r0, [sp, #40]
	ldr r2, .L_0818b568
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #48]
	movs r3, #16
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	adds r2, r5, #0
	bl Func_0814cd48
	movs r3, #0
	mov r8, r3
	mov r7, r10
.L_0818b4be:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r7]
	mov r3, r8
	adds r3, #48
	lsls r3, r3, #16
	movs r5, #127
	str r3, [r7, #4]
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Cos
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r4, #1
	movs r3, #255
	add r8, r4
	ands r3, r0
	mov r6, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r6, #64
	bne .L_0818b4be
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	str r3, [r2]
	movs r2, #128
	ldr r3, .L_0818b558
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
.L_0818b536:
	mov r3, r9
	subs r3, #24
	cmp r3, #36
	bhi .L_0818b5f2
	mov r0, r9
	movs r1, #0
	cmp r0, #28
	ble .L_0818b56c
	cmp r3, #0
	bge .L_0818b54c
	adds r3, #3
.L_0818b54c:
	asrs r1, r3, #2
	cmp r1, #7
	ble .L_0818b56c
	movs r1, #7
	b .L_0818b56c
	.2byte 0x0000
.L_0818b558:
	.4byte 0x00001010
.L_0818b55c:
	.4byte 0xfffff000
.L_0818b560:
	.4byte Data_0201683e
.L_0818b564:
	.4byte IwramFillWords
.L_0818b568:
	.4byte 0x3f3f3f3f
.L_0818b56c:
	movs r3, #174
	lsls r3, r3, #2
	adds r7, r1, #0
	muls r7, r3
	movs r2, #0
	mov r8, r2
	mov r6, r10
.L_0818b57a:
	mov r3, r8
	cmp r3, #0
	bge .L_0818b582
	adds r3, #3
.L_0818b582:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r4, r8
	subs r3, r4, r3
	lsls r2, r3, #1
	adds r5, r2, r3
	ldr r3, [r6, #24]
	mov r1, r9
	adds r0, r3, r1
	cmp r0, #0
	bge .L_0818b59a
	adds r0, #7
.L_0818b59a:
	movs r1, #3
	asrs r0, r0, #3
	bl __modsi3
	ldr r2, .L_0818b834
	adds r0, r5, r0
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	movs r2, #178
	adds r1, r7, r1
	lsls r2, r2, #6
	add r1, r10
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r6, r3]
	ldr r3, .L_0818b838
	ldr r4, [sp, #20]
	ldrb r5, [r3, r0]
	adds r2, r4, r2
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r6, r4]
	ldr r4, .L_0818b83c
	ldrb r4, [r4, r0]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #32
	bne .L_0818b57a
.L_0818b5f2:
	ldr r2, [sp, #44]
	cmp r2, #2
	beq .L_0818b5fa
	b .L_0818b7d8
.L_0818b5fa:
	mov r3, r11
	cmp r3, #60
	bne .L_0818b606
	movs r0, #212
	bl Audio_PlayCue
.L_0818b606:
	mov r4, r11
	cmp r4, #64
	bne .L_0818b654
	movs r0, #144
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
	ldr r1, [sp, #48]
	movs r3, #110
	movs r6, #36
	ldrsh r0, [r1, r6]
	movs r2, #128
	str r3, [sp, #4]
	movs r3, #128
	lsls r2, r2, #10
	movs r1, #1
	lsls r3, r3, #12
	str r2, [sp, #0]
	bl Func_0815f000
	ldr r3, [sp, #48]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #4
	str r3, [r2]
.L_0818b654:
	mov r4, r11
	cmp r4, #51
	bgt .L_0818b65c
	b .L_0818b7d8
.L_0818b65c:
	cmp r4, #63
	bgt .L_0818b68c
	ldr r6, [sp, #48]
	ldr r1, .L_0818b840
	ldr r2, [r6, #4]
	ldr r0, [sp, #20]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r1, r3]
	ldr r3, .L_0818b844
	movs r1, #57
	ldrb r3, [r3]
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	movs r1, #224
	lsls r1, r1, #3
	adds r2, r0, r2
	add r1, r10
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	b .L_0818b7d8
.L_0818b68c:
	mov r6, r11
	cmp r6, #67
	bgt .L_0818b6bc
	ldr r0, [sp, #48]
	ldr r1, .L_0818b840
	ldr r2, [r0, #4]
	ldr r4, [sp, #32]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r1, r3]
	ldr r1, [sp, #20]
	ldr r3, .L_0818b844
	adds r2, r1, r2
	movs r1, #57
	ldrb r3, [r3]
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, [sp, #40]
	add r1, r10
	mov lr, r4
	.2byte 0xf800
.L_0818b6bc:
	ldr r6, [sp, #48]
	ldr r7, .L_0818b840
	ldr r2, [r6, #4]
	ldr r6, .L_0818b844
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #1
	movs r1, #99
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #1]
	str r1, [sp, #0]
	movs r1, #69
	ldr r0, [sp, #20]
	str r1, [sp, #4]
	movs r1, #224
	lsls r1, r1, #5
	adds r1, #210
	adds r2, r0, r2
	ldr r5, [sp, #32]
	ldr r0, [sp, #40]
	add r1, r10
	mov lr, r5
	.2byte 0xf800
	mov r3, r11
	subs r3, #64
	cmp r3, #1
	bhi .L_0818b700
	movs r1, #128
	ldr r3, .L_0818b848
	ldr r0, [sp, #40]
	lsls r1, r1, #7
	ldr r2, .L_0818b84c
	mov lr, r3
	.2byte 0xf800
.L_0818b700:
	mov r3, r11
	subs r3, #66
	cmp r3, #1
	bhi .L_0818b730
	ldr r1, [sp, #48]
	ldr r0, [sp, #40]
	ldr r2, [r1, #4]
	movs r1, #128
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #2
	ldrb r2, [r7, r3]
	ldr r3, [sp, #20]
	adds r2, r3, r2
	ldrb r3, [r6, #2]
	str r1, [sp, #0]
	movs r1, #91
	str r1, [sp, #4]
	movs r1, #220
	lsls r1, r1, #6
	adds r1, #129
	add r1, r10
	mov lr, r5
	.2byte 0xf800
.L_0818b730:
	mov r3, r11
	subs r3, #68
	cmp r3, #1
	bhi .L_0818b75a
	ldr r4, [sp, #48]
	ldr r0, [sp, #20]
	ldr r2, [r4, #4]
	movs r1, #128
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #3
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #3]
	str r1, [sp, #0]
	movs r1, #91
	adds r2, r0, r2
	str r1, [sp, #4]
	ldr r0, [sp, #40]
	ldr r1, .L_0818b850
	mov lr, r5
	.2byte 0xf800
.L_0818b75a:
	mov r3, r11
	subs r3, #70
	cmp r3, #1
	bhi .L_0818b784
	ldr r1, [sp, #48]
	ldr r0, [sp, #40]
	ldr r2, [r1, #4]
	movs r1, #128
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #4
	ldrb r2, [r7, r3]
	ldr r3, [sp, #20]
	adds r2, r3, r2
	ldrb r3, [r6, #4]
	str r1, [sp, #0]
	movs r1, #59
	str r1, [sp, #4]
	ldr r1, .L_0818b854
	mov lr, r5
	.2byte 0xf800
.L_0818b784:
	mov r3, r11
	subs r3, #72
	cmp r3, #1
	bhi .L_0818b7ae
	ldr r4, [sp, #48]
	ldr r0, [sp, #20]
	ldr r2, [r4, #4]
	movs r1, #122
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #5
	ldrb r2, [r7, r3]
	ldrb r3, [r6, #5]
	str r1, [sp, #0]
	movs r1, #29
	adds r2, r0, r2
	str r1, [sp, #4]
	ldr r0, [sp, #40]
	ldr r1, .L_0818b858
	mov lr, r5
	.2byte 0xf800
.L_0818b7ae:
	mov r3, r11
	subs r3, #74
	cmp r3, #1
	bhi .L_0818b7d8
	ldr r1, [sp, #48]
	ldr r0, [sp, #40]
	ldr r2, [r1, #4]
	movs r1, #76
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #6
	ldrb r2, [r7, r3]
	ldr r3, [sp, #20]
	adds r2, r3, r2
	ldrb r3, [r6, #6]
	str r1, [sp, #0]
	movs r1, #25
	str r1, [sp, #4]
	ldr r1, .L_0818b85c
	mov lr, r5
	.2byte 0xf800
.L_0818b7d8:
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r4, [sp, #8]
	ldr r0, [sp, #16]
	movs r6, #1
	adds r4, #1
	add r11, r6
	str r4, [sp, #8]
	cmp r11, r0
	beq .L_0818b80a
	bl .L_0818af78
.L_0818b80a:
	ldr r1, [sp, #44]
	cmp r1, #1
	bne .L_0818b816
	ldr r0, .L_0818b860
	bl Scheduler_RemoveCallback
.L_0818b816:
	ldr r0, .L_0818b864
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0818b834:
	.4byte Data_081974dc
.L_0818b838:
	.4byte Data_081974f4
.L_0818b83c:
	.4byte Data_08197500
.L_0818b840:
	.4byte Data_0819750c
.L_0818b844:
	.4byte Data_0819751a
.L_0818b848:
	.4byte IwramFillWords
.L_0818b84c:
	.4byte 0x3f3f3f3f
.L_0818b850:
	.4byte gMapCellBuffer
.L_0818b854:
	.4byte Data_02012d80
.L_0818b858:
	.4byte Data_02014b00
.L_0818b85c:
	.4byte Data_020158d2
.L_0818b860:
	.4byte Func_08152474
.L_0818b864:
	.4byte Func_08143000
