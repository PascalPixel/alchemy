.syntax unified
	.thumb
	.global Func_08182aa8
	.thumb_func
Func_08182aa8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #164
	str r0, [sp, #80]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	movs r7, #0
	str r0, [sp, #76]
	movs r5, #80
	ldr r1, [r3, #92]
	mov r11, r7
	str r1, [sp, #72]
	negs r5, r5
	ldr r3, [r3, #100]
	movs r7, #1
	mov r10, r3
	bl Func_0813ba50
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	bl Func_08179e6c
	ldr r3, [sp, #72]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r1, #200
	movs r3, #0
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08182c28
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	ldr r1, [sp, #72]
	movs r2, #240
	lsls r0, r0, #19
	lsls r2, r2, #7
	adds r0, #40
	adds r2, #232
	mov r8, r0
	adds r6, r1, r2
.L_08182b0a:
	adds r5, #4
	lsls r3, r5, #8
	mov r4, r8
	str r3, [r4]
	movs r0, #1
	str r7, [r6]
	bl WaitFrames
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #27
	bne .L_08182b0a
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	mov r1, r10
	str r3, [sp, #64]
	ldr r0, .L_08182c2c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #72]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r7, [sp, #72]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	movs r3, #75
	movs r1, #236
	str r3, [r2]
	lsls r1, r1, #7
	movs r2, #8
	adds r1, #64
	negs r2, r2
	movs r3, #0
	adds r4, r7, r1
	mov r10, r2
	mov r8, r3
.L_08182b6c:
	movs r7, #1
	add r8, r7
	mov r0, r8
	strh r3, [r4]
	adds r4, #2
	cmp r0, #15
	bne .L_08182b6c
.L_08182b7a:
	mov r5, r8
	subs r5, #15
	movs r1, #15
	adds r0, r5, #0
	str r4, [sp, #12]
	bl Math_Div
	mov r1, r10
	subs r0, r1, r0
	adds r7, r0, #0
	adds r7, #32
	adds r3, r5, #0
	ldr r4, [sp, #12]
	cmp r5, #0
	bge .L_08182b9a
	mov r3, r8
.L_08182b9a:
	mov r2, r10
	asrs r3, r3, #4
	subs r3, r2, r3
	movs r1, #17
	adds r0, r5, #0
	adds r6, r3, #0
	str r4, [sp, #12]
	bl Math_Div
	mov r3, r10
	adds r6, #16
	subs r1, r3, r0
	ldr r4, [sp, #12]
	cmp r7, #0
	bge .L_08182bba
	movs r7, #0
.L_08182bba:
	cmp r7, #31
	ble .L_08182bc0
	movs r7, #31
.L_08182bc0:
	cmp r6, #0
	bge .L_08182bc6
	movs r6, #0
.L_08182bc6:
	cmp r6, #31
	ble .L_08182bcc
	movs r6, #31
.L_08182bcc:
	cmp r1, #0
	bge .L_08182bd2
	movs r1, #0
.L_08182bd2:
	cmp r1, #31
	ble .L_08182bd8
	movs r1, #31
.L_08182bd8:
	lsls r3, r7, #10
	lsls r2, r6, #5
	movs r7, #1
	orrs r3, r2
	add r8, r7
	orrs r3, r1
	mov r0, r8
	strh r3, [r4]
	adds r4, #2
	cmp r0, #135
	bne .L_08182b7a
	ldr r3, .L_08182c24
.L_08182bf0:
	movs r1, #1
	add r8, r1
	mov r2, r8
	strh r3, [r4]
	adds r4, #2
	cmp r2, #160
	bne .L_08182bf0
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #254
	ldr r0, .L_08182c30
	bl Scheduler_AddOrUpdateCallback
	movs r2, #32
	movs r3, #0
	ldr r0, .L_08182c34
	ldr r1, .L_08182c38
	str r3, [sp, #52]
	bl Func_081885f0
	movs r3, #255
	add r0, sp, #136
	strh r3, [r0]
	movs r1, #0
	b .L_08182c3c
	.2byte 0x0000
.L_08182c24:
	.4byte 0x00000000
.L_08182c28:
	.4byte Func_08143000
.L_08182c2c:
	.4byte 0x00000134
.L_08182c30:
	.4byte Func_08164bb4
.L_08182c34:
	.4byte Data_02014400
.L_08182c38:
	.4byte Data_02014800
.L_08182c3c:
	bl BattleActor_SpawnObjectsForListFar
	ldr r0, .L_08182db0
	bl Resource_GetTableEntry
	movs r2, #128
	adds r5, r0, #0
	ldr r6, .L_08182db4
	adds r1, r5, #0
	lsls r2, r2, #2
	ldr r0, .L_08182db8
	mov lr, r6
	.2byte 0xf800
	movs r4, #128
	lsls r4, r4, #2
	adds r5, r5, r4
	adds r0, r5, #0
	ldr r1, .L_08182dbc
	bl Resource_DecodeType01
	movs r7, #0
	ldr r0, .L_08182dc0
	ldr r1, [sp, #72]
	movs r2, #238
	mov r8, r7
	lsls r2, r2, #7
	movs r3, #13
	ldr r7, .L_08182dbc
	adds r2, #220
	negs r3, r3
	mov r10, r6
	mov r9, r0
	adds r5, r1, r2
	adds r6, r3, #0
.L_08182c80:
	movs r1, #32
	ldr r2, .L_08182dc4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #8
	ands r3, r6
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r9
	ldrh r0, [r3, #2]
	ldr r4, .L_08182dc8
	movs r2, #128
	adds r1, r7, #0
	lsls r2, r2, #3
	adds r0, r0, r4
	mov lr, r10
	.2byte 0xf800
	movs r1, #1
	movs r0, #128
	add r8, r1
	lsls r0, r0, #3
	mov r2, r8
	adds r7, r7, r0
	cmp r2, #12
	bne .L_08182c80
	bl Func_0815b410
	ldr r0, .L_08182dcc
	ldr r1, .L_08182dbc
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #72]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_08182dbc
	movs r2, #64
	movs r3, #64
	bl Func_0816ae40
	ldr r7, [sp, #72]
	movs r2, #142
	lsls r2, r2, #7
	adds r1, r7, r2
	ldr r0, .L_08182dd0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #158
	lsls r3, r3, #7
	adds r5, r7, r3
	ldr r0, .L_08182dd4
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r0, #180
	movs r1, #176
	lsls r0, r0, #7
	movs r4, #0
	lsls r1, r1, #4
	adds r0, #184
	mov r8, r4
	adds r1, #184
	adds r2, r7, r0
.L_08182d16:
	ldrb r3, [r5]
	adds r5, #1
	lsrs r3, r3, #1
	strb r3, [r2]
	movs r3, #1
	add r8, r3
	adds r2, #1
	cmp r8, r1
	bne .L_08182d16
	ldr r0, .L_08182dd8
	ldr r1, .L_08182dbc
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r1, .L_08182ddc
	ldr r0, .L_08182de0
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08182d66
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
.L_08182d66:
	strh r4, [r0]
	ldr r2, .L_08182de4
	movs r3, #240
	str r3, [r2, #16]
	ldr r3, .L_08182da4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	adds r2, #8
	movs r3, #0
	str r3, [r2]
	ldr r3, .L_08182da8
	adds r2, #40
	strh r3, [r2]
	ldr r7, .L_08182de8
	ldr r3, .L_08182dac
	movs r4, #0
	adds r2, #2
	strh r3, [r2]
	str r7, [sp, #20]
	str r4, [sp, #16]
	mov r11, r4
.L_08182d94:
	ldr r3, .L_08182dec
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08182e04
	b .L_08182df0
	.2byte 0x0000
.L_08182da4:
	.4byte 0x00000080
.L_08182da8:
	.4byte 0x00003f44
.L_08182dac:
	.4byte 0x00001010
.L_08182db0:
	.4byte 0x000000a7
.L_08182db4:
	.4byte IwramCopyWords
.L_08182db8:
	.4byte 0x05000200
.L_08182dbc:
	.4byte gMapCellBuffer
.L_08182dc0:
	.4byte ResourceTableEntries
.L_08182dc4:
	.4byte 0x80002000
.L_08182dc8:
	.4byte 0x06010000
.L_08182dcc:
	.4byte 0x000000fb
.L_08182dd0:
	.4byte 0x000000fc
.L_08182dd4:
	.4byte 0x000000fd
.L_08182dd8:
	.4byte 0x000000c2
.L_08182ddc:
	.4byte gIoWriteQueue
.L_08182de0:
	.4byte 0x04000208
.L_08182de4:
	.4byte gCameraSceneParameters
.L_08182de8:
	.4byte 0xde500000
.L_08182dec:
	.4byte gInput
.L_08182df0:
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	cmp r11, r0
	bgt .L_08182e04
	ldr r1, [sp, #52]
	cmp r1, #0
	bne .L_08182e04
	movs r2, #1
	str r2, [sp, #52]
.L_08182e04:
	ldr r3, [sp, #52]
	cmp r3, #0
	ble .L_08182e44
	ldr r0, [sp, #52]
	lsls r3, r3, #8
	adds r3, r3, r0
	ldr r4, [sp, #72]
	lsls r2, r3, #16
	movs r7, #238
	adds r3, r3, r2
	lsls r7, r7, #7
	ldr r2, .L_08182fd0
	adds r7, #132
	adds r1, r4, r7
	lsls r3, r3, #2
	str r3, [r1]
	cmp r3, r2
	ble .L_08182e2a
	str r2, [r1]
.L_08182e2a:
	ldr r1, [sp, #72]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	ldr r4, [sp, #52]
	adds r4, #1
	str r4, [sp, #52]
	cmp r4, #19
	bne .L_08182e44
	bl .L_08183ff0
.L_08182e44:
	mov r7, r11
	cmp r7, #0
	bne .L_08182ea2
	ldr r3, .L_08182fd4
	movs r0, #0
	movs r1, #1
	movs r2, #192
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #1
.L_08182e58:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08182e58
	ldr r3, [sp, #72]
	movs r7, #0
	movs r2, #1
	mov r8, r7
	negs r2, r2
	adds r3, #24
.L_08182e70:
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #64
	bne .L_08182e70
	ldr r4, [sp, #72]
	movs r2, #176
	movs r3, #144
	movs r7, #239
	movs r0, #238
	lsls r2, r2, #15
	lsls r3, r3, #14
	lsls r7, r7, #7
	lsls r0, r0, #7
	str r2, [sp, #56]
	str r3, [sp, #60]
	adds r2, r4, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	adds r2, r4, r0
	movs r3, #50
	str r3, [r2]
.L_08182ea2:
	mov r1, r11
	cmp r1, #82
	bne .L_08182ebc
	movs r1, #128
	ldr r3, .L_08182fd8
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	ldr r2, .L_08182fdc
	mov lr, r3
	.2byte 0xf800
	movs r0, #130
	bl Audio_PlayCue
.L_08182ebc:
	mov r2, r11
	cmp r2, #210
	bne .L_08182ed6
	movs r1, #128
	ldr r3, .L_08182fd8
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	ldr r2, .L_08182fdc
	mov lr, r3
	.2byte 0xf800
	movs r0, #130
	bl Audio_PlayCue
.L_08182ed6:
	movs r3, #137
	lsls r3, r3, #1
	cmp r11, r3
	bne .L_08182ee4
	movs r0, #139
	bl Audio_PlayCue
.L_08182ee4:
	movs r4, #155
	lsls r4, r4, #1
	cmp r11, r4
	bne .L_08182ef2
	movs r0, #206
	bl Audio_PlayCue
.L_08182ef2:
	movs r7, #158
	lsls r7, r7, #1
	cmp r11, r7
	bne .L_08182f00
	movs r0, #154
	bl Audio_PlayCue
.L_08182f00:
	movs r0, #233
	lsls r0, r0, #1
	cmp r11, r0
	bne .L_08182f0e
	movs r0, #142
	bl Audio_PlayCue
.L_08182f0e:
	movs r1, #249
	lsls r1, r1, #1
	cmp r11, r1
	bne .L_08182f1c
	movs r0, #154
	bl Audio_PlayCue
.L_08182f1c:
	movs r2, #248
	adds r2, #255
	cmp r11, r2
	bne .L_08182f2a
	movs r0, #154
	bl Audio_PlayCue
.L_08182f2a:
	movs r3, #254
	lsls r3, r3, #1
	cmp r11, r3
	bne .L_08182f38
	movs r0, #154
	bl Audio_PlayCue
.L_08182f38:
	movs r4, #130
	lsls r4, r4, #2
	cmp r11, r4
	bne .L_08182f46
	movs r0, #212
	bl Audio_PlayCue
.L_08182f46:
	movs r7, #135
	lsls r7, r7, #2
	cmp r11, r7
	bne .L_08182f54
	movs r0, #186
	bl Audio_PlayCue
.L_08182f54:
	ldr r0, .L_08182fe0
	cmp r11, r0
	ble .L_08182f5c
	b .L_08183094
.L_08182f5c:
	ldr r1, [sp, #72]
	movs r2, #236
	lsls r2, r2, #7
	adds r2, #64
	adds r1, r1, r2
	mov r3, r11
	mov r10, r1
	cmp r3, #0
	bge .L_08182f70
	adds r3, #31
.L_08182f70:
	asrs r3, r3, #5
	adds r4, r3, #0
	subs r4, #8
	cmp r4, #8
	ble .L_08182f7c
	movs r4, #8
.L_08182f7c:
	movs r3, #0
	mov r8, r3
.L_08182f80:
	movs r1, #1
	add r8, r1
	mov r7, r10
	movs r0, #2
	mov r2, r8
	strh r3, [r7]
	add r10, r0
	cmp r2, #15
	bne .L_08182f80
	movs r3, #142
	lsls r3, r3, #1
	adds r3, #255
	cmp r11, r3
	ble .L_08182fac
	ldr r3, .L_08182fe4
	add r3, r11
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r3, #0
	adds r4, #8
	b .L_0818300a
.L_08182fac:
	movs r7, #56
	adds r7, #255
	cmp r11, r7
	ble .L_08182ff0
	ldr r2, .L_08182fe8
	add r2, r11
	cmp r2, #0
	bge .L_08182fc0
	ldr r2, .L_08182fec
	add r2, r11
.L_08182fc0:
	asrs r2, r2, #2
	movs r3, #14
	subs r4, r3, r2
	cmp r4, #7
	bgt .L_0818300a
	movs r4, #8
	b .L_0818300a
	.2byte 0x0000
.L_08182fd0:
	.4byte 0x3f3f3f3f
.L_08182fd4:
	.4byte Data_02015018
.L_08182fd8:
	.4byte IwramFillWords
.L_08182fdc:
	.4byte 0x2f2f2f2f
.L_08182fe0:
	.4byte 0x001e847f
.L_08182fe4:
	.4byte 0xfffffde4
.L_08182fe8:
	.4byte 0xfffffec8
.L_08182fec:
	.4byte 0xfffffecb
.L_08182ff0:
	movs r0, #44
	adds r0, #255
	cmp r11, r0
	ble .L_0818300a
	ldr r3, .L_081830b8
	add r3, r11
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r4, r3
	cmp r4, #14
	ble .L_0818300a
	movs r4, #14
.L_0818300a:
	movs r1, #15
	mov r8, r1
.L_0818300e:
	mov r6, r8
	subs r6, #15
	adds r0, r6, #0
	movs r1, #15
	str r4, [sp, #12]
	bl Math_Div
	ldr r4, [sp, #12]
	adds r3, r6, #0
	subs r0, r4, r0
	adds r7, r0, #0
	adds r7, #32
	cmp r6, #0
	bge .L_0818302c
	mov r3, r8
.L_0818302c:
	asrs r3, r3, #4
	subs r3, r4, r3
	movs r1, #17
	adds r0, r6, #0
	adds r5, r3, #0
	str r4, [sp, #12]
	bl Math_Div
	ldr r4, [sp, #12]
	adds r5, #16
	subs r1, r4, r0
	cmp r7, #0
	bge .L_08183048
	movs r7, #0
.L_08183048:
	cmp r7, #31
	ble .L_0818304e
	movs r7, #31
.L_0818304e:
	cmp r5, #0
	bge .L_08183054
	movs r5, #0
.L_08183054:
	cmp r5, #31
	ble .L_0818305a
	movs r5, #31
.L_0818305a:
	cmp r1, #0
	bge .L_08183060
	movs r1, #0
.L_08183060:
	cmp r1, #31
	ble .L_08183066
	movs r1, #31
.L_08183066:
	lsls r3, r7, #10
	lsls r2, r5, #5
	orrs r3, r2
	movs r7, #1
	orrs r3, r1
	mov r2, r10
	add r8, r7
	strh r3, [r2]
	mov r0, r8
	movs r3, #2
	add r10, r3
	cmp r0, #135
	bne .L_0818300e
	ldr r3, .L_081830b4
.L_08183082:
	movs r4, #1
	add r8, r4
	mov r1, r10
	movs r2, #2
	mov r7, r8
	strh r3, [r1]
	add r10, r2
	cmp r7, #160
	bne .L_08183082
.L_08183094:
	ldr r2, .L_081830bc
	add r2, r11
	cmp r2, #17
	bhi .L_081830e4
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	mov r2, r11
	adds r3, #14
	ldr r1, .L_081830c0
	subs r2, #253
	cmp r3, #31
	ble .L_081830c4
	movs r3, #31
	b .L_081830c4
	.2byte 0x0000
.L_081830b4:
	.4byte 0x00000000
.L_081830b8:
	.4byte 0xfffffed4
.L_081830bc:
	.4byte 0xfffffee6
.L_081830c0:
	.4byte 0x05000200
.L_081830c4:
	cmp r2, #31
	ble .L_081830ca
	movs r2, #31
.L_081830ca:
	lsls r3, r3, #5
	lsls r2, r2, #10
	movs r0, #0
	orrs r2, r3
	movs r3, #128
	mov r8, r0
	lsls r3, r3, #1
.L_081830d8:
	movs r4, #1
	add r8, r4
	strh r2, [r1]
	adds r1, #2
	cmp r8, r3
	bne .L_081830d8
.L_081830e4:
	movs r7, #44
	adds r7, #255
	cmp r11, r7
	bgt .L_081830ee
	b .L_081831f6
.L_081830ee:
	ldr r0, .L_08183138
	bl Resource_GetTableEntry
	ldr r1, .L_0818313c
	movs r4, #60
	movs r2, #31
	movs r3, #15
	adds r4, #255
	mov lr, r1
	str r2, [sp, #48]
	str r2, [sp, #44]
	mov r10, r3
	cmp r11, r4
	ble .L_08183116
	movs r7, #4
	movs r1, #12
	str r7, [sp, #48]
	str r1, [sp, #44]
	movs r2, #16
	mov r10, r2
.L_08183116:
	movs r3, #82
	adds r3, #255
	cmp r11, r3
	ble .L_08183126
	movs r4, #0
	str r4, [sp, #48]
	str r4, [sp, #44]
	mov r10, r4
.L_08183126:
	ldr r1, .L_08183134
	movs r7, #0
	movs r2, #31
	mov r8, r7
	mov r12, r1
	mov r9, r2
	b .L_08183140
.L_08183134:
	.4byte 0x0000001f
.L_08183138:
	.4byte 0x000000a7
.L_0818313c:
	.4byte 0x05000200
.L_08183140:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r1, [r0]
	lsls r3, r2, #16
	mov r5, r9
	lsrs r6, r3, #26
	mov r4, r12
	ands r5, r2
	lsls r2, r1, #16
	mov r7, r12
	ands r6, r4
	lsrs r4, r3, #21
	lsrs r3, r2, #26
	str r2, [sp, #8]
	ands r3, r7
	mov r2, r10
	ands r4, r7
	adds r7, r3, r2
	ldr r3, [sp, #8]
	lsrs r2, r3, #21
	mov r3, r12
	ands r2, r3
	ldr r3, [sp, #44]
	adds r2, r2, r3
	str r2, [sp, #8]
	ldr r2, [sp, #48]
	mov r3, r9
	ands r3, r1
	adds r1, r3, r2
	subs r3, r7, #1
	cmp r6, r3
	bge .L_08183184
	adds r6, #2
	b .L_0818318c
.L_08183184:
	adds r3, r7, #1
	cmp r6, r3
	ble .L_0818318c
	subs r6, #2
.L_0818318c:
	ldr r3, [sp, #8]
	subs r3, #1
	cmp r4, r3
	bge .L_08183198
	adds r4, #2
	b .L_081831a2
.L_08183198:
	ldr r3, [sp, #8]
	adds r3, #1
	cmp r4, r3
	ble .L_081831a2
	subs r4, #2
.L_081831a2:
	subs r3, r1, #1
	cmp r5, r3
	bge .L_081831ac
	adds r5, #2
	b .L_081831b4
.L_081831ac:
	adds r3, r1, #1
	cmp r5, r3
	ble .L_081831b4
	subs r5, #2
.L_081831b4:
	cmp r6, #31
	ble .L_081831ba
	movs r6, #31
.L_081831ba:
	cmp r4, #31
	ble .L_081831c0
	movs r4, #31
.L_081831c0:
	cmp r5, #31
	ble .L_081831c6
	movs r5, #31
.L_081831c6:
	cmp r6, #0
	bge .L_081831cc
	movs r6, #0
.L_081831cc:
	cmp r4, #0
	bge .L_081831d2
	movs r4, #0
.L_081831d2:
	cmp r5, #0
	bge .L_081831d8
	movs r5, #0
.L_081831d8:
	lsls r2, r4, #5
	lsls r3, r6, #10
	orrs r3, r2
	movs r7, #1
	movs r2, #128
	mov r4, lr
	orrs r3, r5
	movs r1, #2
	add r8, r7
	lsls r2, r2, #1
	strh r3, [r4]
	adds r0, #2
	add lr, r1
	cmp r8, r2
	bne .L_08183140
.L_081831f6:
	movs r3, #132
	lsls r3, r3, #1
	adds r3, #255
	cmp r11, r3
	ble .L_0818327a
	ldr r5, .L_08183248
	ldr r0, .L_0818324c
	add r5, r11
	bl Resource_GetTableEntry
	lsrs r3, r5, #31
	adds r3, r5, r3
	ldr r6, .L_08183250
	ldr r7, .L_08183244
	movs r4, #0
	asrs r3, r3, #1
	mov r8, r4
	mov r12, r3
.L_0818321a:
	ldrh r3, [r0]
	movs r2, #31
	ands r2, r3
	lsls r3, r3, #16
	adds r4, r2, r5
	lsrs r2, r3, #21
	lsrs r1, r3, #26
	ands r2, r7
	adds r3, r5, #0
	add r2, r12
	ands r1, r7
	cmp r3, #0
	bge .L_08183236
	adds r3, #3
.L_08183236:
	asrs r3, r3, #2
	adds r3, r1, r3
	cmp r4, #31
	ble .L_08183254
	movs r4, #31
	b .L_08183254
	.2byte 0x0000
.L_08183244:
	.4byte 0x0000001f
.L_08183248:
	.4byte 0xfffffdf8
.L_0818324c:
	.4byte 0x000000a7
.L_08183250:
	.4byte 0x05000200
.L_08183254:
	cmp r2, #31
	ble .L_0818325a
	movs r2, #31
.L_0818325a:
	cmp r3, #31
	ble .L_08183260
	movs r3, #31
.L_08183260:
	lsls r2, r2, #5
	lsls r3, r3, #10
	orrs r3, r2
	movs r1, #1
	movs r2, #128
	orrs r3, r4
	add r8, r1
	lsls r2, r2, #1
	strh r3, [r6]
	adds r0, #2
	adds r6, #2
	cmp r8, r2
	bne .L_0818321a
.L_0818327a:
	ldr r3, .L_08183580
	add r3, r11
	cmp r3, #253
	bhi .L_08183314
	ldr r3, .L_08183584
	add r1, sp, #120
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #100]
	str r4, [sp, #104]
	movs r3, #255
	lsls r3, r3, #16
	movs r4, #180
	str r3, [r1, #4]
	lsls r4, r4, #1
	movs r3, #0
	str r3, [r1, #12]
	cmp r11, r4
	bne .L_081832a6
	movs r7, #144
	lsls r7, r7, #14
	str r7, [sp, #60]
.L_081832a6:
	ldr r3, .L_08183588
	add r3, r11
	cmp r3, #111
	bhi .L_081832b8
	ldr r0, [sp, #60]
	movs r2, #128
	lsls r2, r2, #7
	adds r2, r0, r2
	str r2, [sp, #60]
.L_081832b8:
	movs r3, #142
	lsls r3, r3, #1
	adds r3, #255
	cmp r11, r3
	ble .L_081832cc
	ldr r4, [sp, #60]
	movs r7, #128
	lsls r7, r7, #13
	adds r7, r4, r7
	str r7, [sp, #60]
.L_081832cc:
	adds r6, r1, #0
	movs r2, #238
	ldr r1, [sp, #72]
	lsls r2, r2, #7
	movs r0, #0
	adds r2, #220
	mov r8, r0
	add r7, sp, #100
	adds r5, r1, r2
.L_081832de:
	mov r0, r8
	movs r1, #3
	bl __modsi3
	ldr r3, [sp, #56]
	lsls r0, r0, #21
	adds r0, r0, r3
	str r0, [r6]
	movs r1, #3
	mov r0, r8
	bl Math_Div
	ldr r4, [sp, #60]
	lsls r0, r0, #21
	adds r0, r0, r4
	str r0, [r6, #8]
	adds r1, r6, #0
	ldmia r5!, {r0}
	adds r2, r7, #0
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #12
	bne .L_081832de
.L_08183314:
	ldr r3, .L_0818358c
	add r3, r11
	cmp r3, #23
	bhi .L_08183366
	ldr r2, .L_08183590
	lsls r5, r3, #3
	subs r5, r5, r3
	lsls r5, r5, #2
	adds r5, r5, r2
	bl Random16
	movs r6, #15
	ands r0, r6
	adds r0, #52
	movs r3, #200
	lsls r3, r3, #15
	lsls r0, r0, #16
	str r3, [r5, #4]
	str r0, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #8
	negs r0, r0
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #5
	str r3, [r5, #8]
	movs r3, #32
	str r3, [r5, #24]
.L_08183366:
	movs r3, #155
	lsls r3, r3, #1
	cmp r11, r3
	bne .L_081833ca
	ldr r5, .L_08183594
	movs r4, #0
	mov r8, r4
	movs r6, #31
.L_08183376:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #28
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r1, #80
	bl Math_ModU
	adds r0, #56
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	movs r3, #8
	ands r0, r6
	subs r3, r3, r0
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r7, #1
	adds r3, #5
	add r8, r7
	str r3, [r5, #8]
	mov r0, r8
	movs r3, #32
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #80
	bne .L_08183376
.L_081833ca:
	movs r1, #0
	mov r10, r1
.L_081833ce:
	ldr r3, .L_08183598
	mov r4, r10
	lsls r2, r4, #1
	ldrh r3, [r3, r2]
	cmp r11, r3
	bne .L_08183466
	movs r7, #0
	mov r8, r7
	movs r6, #3
.L_081833e0:
	movs r1, #9
	mov r0, r10
	bl __modsi3
	lsls r3, r0, #3
	subs r3, r3, r0
	add r3, r8
	ldr r0, [sp, #72]
	lsls r5, r3, #3
	subs r5, r5, r3
	lsls r5, r5, #2
	adds r5, r0, r5
	bl Random16
	ldr r3, .L_0818359c
	mov r2, r10
	ldrb r1, [r3, r2]
	bl Math_ModU
	ldr r3, .L_081835a0
	mov r4, r10
	ldrb r3, [r3, r4]
	mov r7, r10
	adds r0, r0, r3
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ldr r3, .L_081835a4
	ands r0, r6
	ldrb r3, [r3, r7]
	mov r1, r8
	muls r1, r3
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_081835a8
	ldrb r3, [r3, r7]
	adds r0, r0, r3
	ldr r3, .L_081835ac
	lsls r0, r0, #16
	ldrsb r3, [r3, r7]
	str r0, [r5, #4]
	lsls r3, r3, #12
	str r3, [r5, #12]
	ldr r3, .L_081835b0
	ldrsb r3, [r3, r7]
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	ldr r3, .L_081835b4
	ands r0, r6
	ldrsb r3, [r3, r7]
	movs r2, #0
	adds r0, r0, r3
	ldr r3, .L_081835b8
	lsls r0, r0, #16
	ldrsb r3, [r3, r7]
	str r0, [r5, #8]
	lsls r3, r3, #10
	str r3, [r5, #20]
	movs r3, #1
	add r8, r3
	mov r4, r8
	str r2, [r5, #24]
	cmp r4, #7
	bne .L_081833e0
.L_08183466:
	movs r7, #1
	add r10, r7
	mov r0, r10
	cmp r0, #9
	bne .L_081833ce
	movs r1, #104
	adds r1, #255
	cmp r11, r1
	bgt .L_081834ea
	ldr r3, .L_081835bc
	ldr r7, .L_08183594
	movs r2, #0
	mov r8, r2
	mov r10, r3
.L_08183482:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_081834da
	ldr r6, [r7, #8]
	ldr r2, .L_081835c0
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	movs r3, #2
	ldrsh r2, [r7, r3]
	ldr r3, .L_081835c4
	ldr r4, [sp, #72]
	ldrb r5, [r3, r6]
	movs r0, #158
	adds r1, r4, r1
	lsls r0, r0, #7
	adds r1, r1, r0
	lsrs r3, r5, #1
	mov r0, r10
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r7, r4]
	ldrb r4, [r0, r6]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #76]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	ldr r2, .L_081835c8
	movs r1, #63
	bl BattleFxKernels_IntegrateVector2
	mov r0, r10
	ldrb r3, [r0, r6]
	ldr r2, [r7, #4]
	lsls r3, r3, #16
	cmn r2, r3
	bge .L_081834da
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_081834da:
	movs r1, #1
	movs r2, #192
	add r8, r1
	lsls r2, r2, #1
	adds r7, #28
	cmp r8, r2
	bne .L_08183482
	b .L_08183820
.L_081834ea:
	bl Func_08014de4
	movs r2, #128
	movs r0, #0
	movs r1, #0
	lsls r2, r2, #17
	bl Func_08015160
	movs r3, #142
	lsls r3, r3, #1
	adds r3, #255
	cmp r11, r3
	ble .L_0818351e
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	mov r0, r11
	muls r0, r3
	ldr r4, .L_081835cc
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r4
	adds r1, r2, #0
	bl Func_080151e4
	b .L_08183550
.L_0818351e:
	movs r7, #131
	lsls r7, r7, #1
	adds r7, #255
	cmp r11, r7
	ble .L_08183536
	movs r2, #128
	movs r0, #0
	movs r1, #0
	lsls r2, r2, #9
	bl Func_080151e4
	b .L_08183550
.L_08183536:
	movs r0, #230
	adds r0, #255
	cmp r11, r0
	ble .L_08183550
	mov r1, r11
	lsls r3, r1, #11
	ldr r1, .L_081835d0
	movs r2, #128
	subs r1, r1, r3
	adds r0, r1, #0
	lsls r2, r2, #9
	bl Func_080151e4
.L_08183550:
	movs r2, #142
	lsls r2, r2, #1
	adds r2, #255
	cmp r11, r2
	ble .L_08183564
	mov r3, r11
	lsls r0, r3, #10
	bl Func_08015068
	b .L_081835f4
.L_08183564:
	movs r4, #131
	lsls r4, r4, #1
	adds r4, #255
	cmp r11, r4
	ble .L_081835d4
	movs r0, #128
	lsls r0, r0, #9
	bl Func_08015068
	movs r0, #128
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	b .L_081835f4
.L_08183580:
	.4byte 0xfffffedc
.L_08183584:
	.4byte Data_08196ef0
.L_08183588:
	.4byte 0xfffffe74
.L_0818358c:
	.4byte 0xfffffeee
.L_08183590:
	.4byte Data_020158c0
.L_08183594:
	.4byte Data_02015000
.L_08183598:
	.4byte Data_081996e8
.L_0818359c:
	.4byte Data_0819970c
.L_081835a0:
	.4byte Data_081996fa
.L_081835a4:
	.4byte Data_08199715
.L_081835a8:
	.4byte Data_08199703
.L_081835ac:
	.4byte Data_0819971e
.L_081835b0:
	.4byte Data_08199727
.L_081835b4:
	.4byte Data_08199730
.L_081835b8:
	.4byte Data_08199739
.L_081835bc:
	.4byte Data_081996bb
.L_081835c0:
	.4byte Data_081996ca
.L_081835c4:
	.4byte Data_081996ac
.L_081835c8:
	.4byte 0xffffc000
.L_081835cc:
	.4byte 0xfff3b3f0
.L_081835d0:
	.4byte 0x00103000
.L_081835d4:
	movs r7, #198
	adds r7, #255
	cmp r11, r7
	ble .L_081835f4
	ldr r5, .L_081837a0
	add r5, r11
	lsls r5, r5, #10
	adds r0, r5, #0
	bl Func_08015068
	adds r0, r5, #0
	bl Trig_Sin
	asrs r0, r0, #3
	bl SceneTransform_ApplyPitch
.L_081835f4:
	movs r0, #216
	movs r1, #177
	movs r2, #216
	lsls r0, r0, #8
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r0, #240
	adds r1, #224
	adds r2, #120
	bl Func_080151e4
	movs r0, #180
	lsls r0, r0, #1
	cmp r11, r0
	bne .L_081836a6
	ldr r1, [sp, #52]
	cmp r1, #0
	bne .L_08183632
	ldr r3, [sp, #72]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r7, [sp, #72]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	movs r3, #75
	str r3, [r2]
.L_08183632:
	ldr r5, .L_081837a4
	movs r1, #0
	mov r8, r1
.L_08183638:
	bl Random16
	movs r3, #3
	ands r3, r0
	str r3, [r5, #24]
	bl Random16
	movs r1, #160
	bl Math_ModU
	subs r0, #80
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r1, #160
	bl Math_ModU
	adds r0, #48
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r1, #160
	bl Math_ModU
	subs r0, #80
	movs r2, #0
	lsls r0, r0, #16
	str r2, [r5, #12]
	str r0, [r5, #8]
	bl Random16
	ldr r3, [r5, #24]
	movs r2, #63
	ands r2, r0
	adds r2, #32
	movs r1, #16
	subs r1, r1, r3
	negs r2, r2
	movs r3, #1
	lsls r3, r1
	lsls r2, r2, #10
	subs r2, r2, r3
	str r2, [r5, #16]
	bl Random16
	movs r3, #1
	movs r4, #192
	add r8, r3
	lsls r4, r4, #1
	str r0, [r5, #20]
	adds r5, #28
	cmp r8, r4
	bne .L_08183638
.L_081836a6:
	movs r7, #135
	lsls r7, r7, #2
	cmp r11, r7
	bne .L_081836d6
	ldr r5, .L_081837a4
	movs r0, #0
	mov r8, r0
	movs r6, #63
.L_081836b6:
	movs r1, #0
	str r1, [r5, #4]
	bl Random16
	ands r0, r6
	adds r0, #32
	negs r0, r0
	movs r2, #1
	movs r3, #192
	lsls r0, r0, #15
	add r8, r2
	lsls r3, r3, #1
	str r0, [r5, #16]
	adds r5, #28
	cmp r8, r3
	bne .L_081836b6
.L_081836d6:
	ldr r7, .L_081837a4
	movs r4, #0
	mov r8, r4
.L_081836dc:
	ldr r3, [r7, #24]
	cmp r3, #0
	bge .L_081836e4
	b .L_08183812
.L_081836e4:
	add r6, sp, #108
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	movs r0, #10
	ldrsh r3, [r6, r0]
	movs r1, #50
	subs r3, #228
	lsls r0, r3, #1
	adds r0, r0, r3
	str r3, [r6, #8]
	lsls r0, r0, #1
	bl Math_Div
	ldr r2, .L_081837a8
	ldr r3, [r7, #24]
	ldrb r3, [r2, r3]
	adds r0, r0, r3
	cmp r0, #0
	bge .L_08183710
	movs r0, #0
.L_08183710:
	cmp r0, #14
	ble .L_08183716
	movs r0, #14
.L_08183716:
	movs r1, #2
	ldrsh r3, [r6, r1]
	movs r4, #6
	ldrsh r2, [r6, r4]
	movs r1, #142
	lsls r1, r1, #1
	adds r3, #60
	adds r1, #255
	str r2, [r6, #4]
	str r3, [r6]
	cmp r11, r1
	ble .L_0818373a
	mov r4, r11
	ldr r1, .L_081837ac
	lsls r3, r4, #4
	adds r3, r2, r3
	adds r3, r3, r1
	b .L_08183764
.L_0818373a:
	movs r3, #131
	lsls r3, r3, #1
	adds r3, #255
	cmp r11, r3
	ble .L_0818374a
	adds r3, r2, #0
	adds r3, #16
	b .L_08183764
.L_0818374a:
	movs r4, #198
	adds r4, #255
	cmp r11, r4
	bgt .L_08183758
	adds r3, r2, #0
	adds r3, #80
	b .L_08183764
.L_08183758:
	mov r1, r11
	subs r3, r2, r1
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r3, r3, r2
.L_08183764:
	str r3, [r6, #4]
	ldr r3, [r7, #24]
	cmp r3, #1
	bgt .L_081837bc
	ldr r2, .L_081837b0
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #72]
	ldr r2, [r6]
	adds r1, r3, r1
	ldr r3, .L_081837b4
	movs r4, #158
	ldrb r5, [r3, r0]
	lsls r4, r4, #7
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_081837b8
	adds r1, r1, r4
	ldrb r4, [r3, r0]
	ldr r3, [r6, #4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	str r5, [sp, #0]
	ldr r0, [sp, #76]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	b .L_081837ee
	.2byte 0x0000
.L_081837a0:
	.4byte 0xfffffe3a
.L_081837a4:
	.4byte Data_02015000
.L_081837a8:
	.4byte Data_08199742
.L_081837ac:
	.4byte 0xffffde60
.L_081837b0:
	.4byte Data_081996ca
.L_081837b4:
	.4byte Data_081996ac
.L_081837b8:
	.4byte Data_081996bb
.L_081837bc:
	ldr r2, .L_08183b38
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #72]
	movs r3, #180
	lsls r3, r3, #7
	adds r1, r2, r1
	adds r3, #184
	adds r1, r1, r3
	ldr r3, .L_08183b3c
	ldr r2, [r6]
	ldrb r5, [r3, r0]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_08183b40
	ldrb r4, [r3, r0]
	ldr r3, [r6, #4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	str r5, [sp, #0]
	ldr r0, [sp, #76]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
.L_081837ee:
	movs r0, #198
	adds r0, #255
	cmp r11, r0
	ble .L_0818380a
	mov r3, r8
	cmp r3, #0
	bge .L_081837fe
	adds r3, #15
.L_081837fe:
	movs r1, #135
	asrs r3, r3, #4
	lsls r1, r1, #2
	adds r3, r3, r1
	cmp r11, r3
	blt .L_08183812
.L_0818380a:
	ldr r3, [r7, #4]
	ldr r2, [r7, #16]
	adds r3, r3, r2
	str r3, [r7, #4]
.L_08183812:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #28
	cmp r3, #80
	beq .L_08183820
	b .L_081836dc
.L_08183820:
	movs r4, #0
	str r4, [sp, #24]
	mov r10, r4
.L_08183826:
	ldr r1, [sp, #24]
	movs r7, #0
	lsls r3, r1, #3
	ldr r2, [sp, #72]
	subs r3, r3, r1
	str r7, [sp, #40]
	movs r0, #0
	lsls r3, r3, #2
	mov r8, r0
	adds r6, r3, r2
.L_0818383a:
	movs r3, #196
	mov r4, r10
	muls r4, r3
	ldr r7, [sp, #72]
	adds r3, r4, #0
	adds r3, #192
	ldr r3, [r7, r3]
	cmp r3, #0
	blt .L_081838ee
	movs r0, #10
	ldrsh r2, [r6, r0]
	movs r3, #14
	movs r1, #7
	mov r0, r8
	subs r7, r3, r2
	bl __modsi3
	mov r1, r11
	lsls r0, r0, #3
	subs r0, r1, r0
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r3, [r6]
	lsls r0, r0, #2
	adds r3, r3, r0
	mov r2, r8
	asrs r0, r3, #16
	cmp r2, #0
	bne .L_0818387a
	ldr r3, [r6, #4]
	str r3, [sp, #40]
.L_0818387a:
	cmp r7, #0
	bge .L_08183880
	movs r7, #0
.L_08183880:
	ldr r2, .L_08183b38
	lsls r3, r7, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_08183b3c
	ldr r4, [sp, #72]
	ldrb r5, [r3, r7]
	movs r2, #180
	lsls r2, r2, #7
	adds r1, r4, r1
	adds r2, #184
	adds r1, r1, r2
	lsrs r2, r5, #1
	ldr r4, [sp, #40]
	subs r2, r0, r2
	ldr r0, .L_08183b40
	asrs r3, r4, #16
	ldrb r4, [r0, r7]
	mov r9, r0
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	str r5, [sp, #0]
	ldr r0, [sp, #76]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_081838c8
	movs r0, #0
	str r0, [r6, #8]
.L_081838c8:
	mov r1, r8
	cmp r1, #6
	bne .L_081838e0
	mov r3, r9
	ldrb r2, [r3, r7]
	ldr r3, [r6, #4]
	lsls r2, r2, #16
	cmn r3, r2
	bge .L_081838e0
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
.L_081838e0:
	ldr r3, [r6, #8]
	ldr r4, [sp, #40]
	movs r7, #128
	adds r3, r4, r3
	lsls r7, r7, #12
	adds r7, r3, r7
	str r7, [sp, #40]
.L_081838ee:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #7
	bne .L_0818383a
	ldr r2, [sp, #24]
	add r10, r0
	adds r2, #7
	mov r3, r10
	str r2, [sp, #24]
	cmp r3, #9
	bne .L_08183826
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #36]
	movs r0, #1
	bl Func_081969f8
	mov r4, r8
	add r3, sp, #92
	strb r4, [r3]
	str r3, [r0, #16]
	ldr r3, .L_08183b44
	movs r7, #93
	add r7, sp
	strb r4, [r7]
	str r3, [r0, #8]
	ldr r1, [sp, #36]
	mov r2, r11
	subs r2, #210
	str r1, [r0, #12]
	mov r10, r0
	str r2, [sp, #32]
	cmp r2, #107
	bls .L_0818393c
	b .L_08183a5c
.L_0818393c:
	mov r4, r11
	lsls r3, r4, #1
	ldr r0, .L_08183b48
	add r3, r11
	lsls r3, r3, #13
	movs r1, #250
	movs r2, #192
	lsls r1, r1, #4
	adds r7, r3, r0
	lsls r2, r2, #13
	mov r8, r1
	cmp r7, r2
	ble .L_0818395a
	movs r7, #192
	lsls r7, r7, #13
.L_0818395a:
	movs r3, #178
	lsls r3, r3, #1
	adds r3, #255
	mov r4, r11
	muls r4, r3
	ldr r0, .L_08183b4c
	movs r1, #216
	adds r3, r4, #0
	lsls r1, r1, #8
	adds r6, r3, r0
	adds r1, #240
	cmp r6, r1
	ble .L_0818397a
	movs r6, #216
	lsls r6, r6, #8
	adds r6, #240
.L_0818397a:
	movs r2, #18
	adds r2, #255
	cmp r11, r2
	ble .L_081839d8
	ldr r0, .L_08183b50
	mov r4, r11
	lsls r3, r4, #15
	movs r1, #128
	adds r7, r3, r0
	lsls r1, r1, #14
	cmp r7, r1
	ble .L_08183996
	movs r7, #128
	lsls r7, r7, #14
.L_08183996:
	mov r3, r11
	lsls r2, r3, #6
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #32
	movs r4, #176
	subs r3, r3, r2
	lsls r4, r4, #4
	mov r8, r3
	adds r4, #183
	cmp r8, r4
	bgt .L_081839b6
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #184
	mov r8, r0
.L_081839b6:
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #83
	mov r1, r11
	muls r1, r3
	ldr r2, .L_08183b54
	adds r3, r1, #0
	adds r3, r6, r3
	adds r6, r3, r2
	movs r3, #167
	lsls r3, r3, #9
	adds r3, #32
	cmp r6, r3
	ble .L_081839d8
	movs r6, #167
	lsls r6, r6, #9
	adds r6, #32
.L_081839d8:
	movs r3, #6
	mov r4, r10
	str r3, [r4]
	ldr r0, [sp, #72]
	movs r1, #224
	lsls r1, r1, #3
	ldr r4, .L_08183b58
	adds r3, r0, r1
	mov r2, r11
	str r3, [sp, #96]
	lsls r3, r2, #3
	adds r5, r3, r4
	cmp r5, #0
	ble .L_081839f6
	movs r5, #0
.L_081839f6:
	movs r0, #54
	adds r0, #255
	cmp r11, r0
	ble .L_08183a08
	movs r3, #155
	lsls r3, r3, #1
	mov r1, r11
	subs r3, r3, r1
	lsls r5, r3, #3
.L_08183a08:
	mov r2, r10
	str r5, [r2, #20]
	bl Func_08014de4
	movs r0, #0
	adds r1, r7, #0
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	adds r0, r6, #0
	bl Func_0801521c
	movs r3, #18
	adds r3, #255
	cmp r11, r3
	bgt .L_08183a40
	ldr r4, [sp, #16]
	ldr r7, .L_08183b5c
	adds r0, r4, r7
	bl Func_080150e4
.L_08183a40:
	mov r0, r8
	bl SceneTransform_ApplyPitch
	ldr r0, [sp, #16]
	bl Func_08015068
	ldr r0, .L_08183b60
	ldr r1, [sp, #36]
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08183a5c:
	ldr r0, .L_08183b64
	add r0, r11
	cmp r0, #7
	bhi .L_08183a90
	ldr r1, [sp, #52]
	cmp r1, #0
	bne .L_08183a90
	ldr r3, [sp, #72]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r7, [sp, #72]
	movs r2, #238
	lsls r2, r2, #7
	lsls r3, r0, #8
	adds r2, #132
	adds r3, r3, r0
	adds r1, r7, r2
	lsls r2, r3, #16
	adds r3, r3, r2
	ldr r2, .L_08183b68
	lsls r3, r3, #2
	subs r2, r2, r3
	str r2, [r1]
.L_08183a90:
	movs r3, #26
	adds r3, #255
	cmp r11, r3
	bne .L_08183ab6
	ldr r4, [sp, #52]
	cmp r4, #0
	bne .L_08183ab6
	ldr r7, [sp, #72]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #50
	str r3, [r2]
.L_08183ab6:
	ldr r3, .L_08183b6c
	add r3, r11
	cmp r3, #27
	bhi .L_08183b1a
	ldr r4, .L_08183b70
	mov r2, r11
	ldr r7, .L_08183b74
	lsls r3, r2, #11
	movs r1, #38
	adds r6, r3, r4
	mov r0, r10
	movs r3, #7
	adds r1, #255
	str r7, [sp, #96]
	str r3, [r0]
	movs r5, #0
	cmp r11, r1
	ble .L_08183ae2
	movs r3, #147
	lsls r3, r3, #1
	subs r3, r3, r2
	lsls r5, r3, #3
.L_08183ae2:
	mov r2, r10
	str r5, [r2, #20]
	bl Func_08014de4
	movs r1, #224
	lsls r1, r1, #13
	movs r2, #0
	movs r0, #0
	bl Func_08015160
	adds r0, r6, #0
	bl Func_0801521c
	movs r0, #250
	lsls r0, r0, #4
	bl SceneTransform_ApplyPitch
	ldr r0, [sp, #16]
	bl Func_08015068
	ldr r0, .L_08183b60
	ldr r1, [sp, #36]
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08183b1a:
	mov r3, r11
	cmp r3, #80
	bgt .L_08183b22
	b .L_08183d52
.L_08183b22:
	ldr r7, [sp, #20]
	movs r4, #0
	mov r8, r4
	mov r9, r7
.L_08183b2a:
	ldr r2, .L_08183b78
	mov r0, r8
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	cmp r11, r1
	ble .L_08183bf0
	b .L_08183b7c
.L_08183b38:
	.4byte Data_081996ca
.L_08183b3c:
	.4byte Data_081996ac
.L_08183b40:
	.4byte Data_081996bb
.L_08183b44:
	.4byte Data_08199364
.L_08183b48:
	.4byte 0xffb14000
.L_08183b4c:
	.4byte 0xfffe4aca
.L_08183b50:
	.4byte 0xff8f0000
.L_08183b54:
	.4byte 0xfff8292a
.L_08183b58:
	.4byte 0xfffff930
.L_08183b5c:
	.4byte 0xfffedc00
.L_08183b60:
	.4byte Data_08199210
.L_08183b64:
	.4byte 0xfffffeef
.L_08183b68:
	.4byte 0x3f3f3f3f
.L_08183b6c:
	.4byte 0xfffffeee
.L_08183b70:
	.4byte 0xfff7b000
.L_08183b74:
	.4byte gMapCellBuffer
.L_08183b78:
	.4byte Data_08199746
.L_08183b7c:
	adds r3, r1, #0
	adds r3, #10
	cmp r11, r3
	bge .L_08183bf0
	ldr r3, .L_08183ea8
	movs r5, #16
	mov r4, r10
	str r3, [sp, #96]
	negs r5, r5
	movs r3, #6
	mov r2, r11
	str r3, [r4]
	str r5, [r4, #20]
	subs r7, r2, r1
	bl Func_08014de4
	mov r0, r8
	lsls r6, r7, #13
	cmp r0, #1
	ble .L_08183bc6
	movs r2, #142
	lsls r2, r2, #1
	adds r2, #255
	ldr r1, .L_08183eac
	cmp r11, r2
	ble .L_08183bb2
	add r1, r9
.L_08183bb2:
	ldr r0, .L_08183eb0
	movs r2, #0
	bl Func_08015160
	adds r0, r6, #0
	adds r1, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	b .L_08183bd0
.L_08183bc6:
	lsls r1, r7, #14
	adds r0, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
.L_08183bd0:
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	mov r3, r11
	lsls r0, r3, #9
	bl Func_08015068
	ldr r0, .L_08183eb4
	ldr r1, [sp, #36]
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08183bf0:
	movs r4, #1
	add r8, r4
	mov r7, r8
	cmp r7, #4
	bne .L_08183b2a
	mov r0, r11
	cmp r0, #80
	bgt .L_08183c02
	b .L_08183d52
.L_08183c02:
	ldr r2, .L_08183eb8
	movs r1, #0
	mov r8, r1
	mov r9, r2
	movs r7, #0
.L_08183c0c:
	mov r4, r9
	ldrh r3, [r7, r4]
	cmp r11, r3
	bne .L_08183c26
	movs r1, #128
	ldr r3, .L_08183ebc
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	ldr r2, .L_08183ec0
	mov lr, r3
	.2byte 0xf800
	mov r4, r9
	ldrh r3, [r7, r4]
.L_08183c26:
	cmp r11, r3
	ble .L_08183c9c
	adds r2, r3, #0
	adds r2, #10
	cmp r11, r2
	bge .L_08183c9c
	mov r0, r11
	subs r1, r0, r3
	movs r3, #160
	lsls r2, r1, #13
	lsls r3, r3, #9
	subs r6, r3, r2
	lsls r1, r1, #4
	ldr r2, .L_08183ea8
	adds r5, r1, #0
	movs r3, #7
	mov r4, r10
	subs r5, #56
	str r2, [sp, #96]
	str r3, [r4]
	cmp r5, #0
	ble .L_08183c54
	movs r5, #0
.L_08183c54:
	mov r0, r10
	str r5, [r0, #20]
	bl Func_08014de4
	ldr r0, .L_08183eb0
	ldr r1, .L_08183eac
	movs r2, #0
	bl Func_08015160
	lsls r1, r6, #1
	adds r0, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	mov r1, r8
	movs r2, #128
	lsls r0, r1, #14
	lsls r2, r2, #6
	adds r0, r0, r2
	bl Func_080150e4
	ldr r0, .L_08183ec4
	bl SceneTransform_ApplyPitch
	mov r3, r11
	lsls r0, r3, #9
	bl Func_08015068
	ldr r0, .L_08183eb4
	ldr r1, [sp, #36]
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08183c9c:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r7, #2
	cmp r0, #3
	bne .L_08183c0c
	mov r1, r11
	cmp r1, #80
	ble .L_08183d52
	ldr r3, [sp, #20]
	movs r2, #0
	mov r8, r2
	mov r9, r3
.L_08183cb6:
	ldr r2, .L_08183ec8
	mov r4, r8
	lsls r3, r4, #1
	ldrh r2, [r2, r3]
	cmp r11, r2
	ble .L_08183d48
	adds r3, r2, #0
	adds r3, #100
	cmp r11, r3
	bge .L_08183d48
	movs r3, #176
	mov r0, r11
	lsls r3, r3, #4
	subs r2, r0, r2
	adds r3, #162
	movs r1, #255
	adds r6, r2, #0
	muls r6, r3
	lsls r1, r1, #8
	adds r1, #255
	ldr r7, .L_08183eac
	cmp r6, r1
	ble .L_08183ce8
	movs r6, #128
	lsls r6, r6, #9
.L_08183ce8:
	ldr r2, [sp, #72]
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r4
	movs r5, #16
	mov r0, r10
	str r3, [sp, #96]
	negs r5, r5
	movs r3, #6
	str r3, [r0]
	str r5, [r0, #20]
	bl Func_08014de4
	movs r1, #142
	lsls r1, r1, #1
	adds r1, #255
	cmp r11, r1
	ble .L_08183d10
	ldr r7, .L_08183eac
	add r7, r9
.L_08183d10:
	ldr r0, .L_08183eb0
	adds r1, r7, #0
	movs r2, #0
	bl Func_08015160
	lsrs r1, r6, #31
	adds r1, r6, r1
	asrs r1, r1, #1
	adds r2, r6, #0
	adds r0, r6, #0
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	mov r2, r11
	lsls r0, r2, #11
	bl Func_08015068
	ldr r0, .L_08183eb4
	ldr r1, [sp, #36]
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08183d48:
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #3
	bne .L_08183cb6
.L_08183d52:
	add r7, sp, #92
	adds r2, r7, #0
	movs r1, #7
	movs r3, #4
	strb r1, [r7]
	strb r3, [r2, #1]
	ldr r0, [sp, #72]
	movs r4, #142
	lsls r4, r4, #7
	adds r3, r0, r4
	ldr r0, .L_08183ecc
	mov r7, r10
	str r3, [r2, #4]
	str r1, [r7]
	str r0, [r7, #8]
	ldr r1, [sp, #36]
	movs r4, #128
	movs r2, #0
	movs r3, #0
	lsls r4, r4, #8
	str r1, [r7, #12]
	strb r2, [r7, #24]
	strb r2, [r7, #25]
	mov r8, r3
	mov r9, r4
	movs r7, #0
.L_08183d86:
	ldr r0, .L_08183ed0
	mov r1, r8
	ldrb r2, [r0, r1]
	cmp r11, r2
	bge .L_08183d92
	b .L_08183f8c
.L_08183d92:
	movs r3, #38
	adds r3, #255
	cmp r11, r3
	ble .L_08183d9c
	b .L_08183ef0
.L_08183d9c:
	ldr r3, .L_08183ed4
	mov r4, r11
	ldrh r1, [r3, r7]
	ldr r3, .L_08183ed8
	subs r2, r4, r2
	ldrh r4, [r3, r7]
	adds r3, r4, #0
	muls r3, r2
	adds r6, r1, r3
	ldr r3, .L_08183edc
	ldrh r3, [r3, r7]
	cmp r6, r3
	ble .L_08183db8
	adds r6, r3, #0
.L_08183db8:
	mov r0, r11
	cmp r0, #209
	ble .L_08183dc6
	ldr r1, [sp, #32]
	adds r3, r4, #0
	muls r3, r1
	subs r6, r6, r3
.L_08183dc6:
	bl Func_08014de4
	movs r1, #128
	mov r2, r9
	mov r0, r9
	lsls r1, r1, #9
	bl Func_080151e4
	movs r2, #18
	adds r2, #255
	cmp r11, r2
	ble .L_08183de8
	mov r4, r11
	ldr r0, .L_08183ee0
	lsls r3, r4, #13
	adds r3, r6, r3
	adds r6, r3, r0
.L_08183de8:
	adds r0, r6, #0
	bl Func_0801521c
	mov r1, r8
	cmp r1, #0
	bne .L_08183e20
	mov r2, r11
	cmp r2, #209
	ble .L_08183e0c
	ldr r1, [sp, #32]
	cmp r1, #48
	ble .L_08183e02
	movs r1, #48
.L_08183e02:
	lsls r1, r1, #16
	movs r0, #0
	movs r2, #0
	bl Func_08015160
.L_08183e0c:
	ldr r3, .L_08183ee4
	mov r4, r11
	ldrh r0, [r3, r7]
	lsls r3, r4, #8
	subs r0, r0, r3
	movs r5, #16
	bl Func_080150e4
	negs r5, r5
	b .L_08183e74
.L_08183e20:
	movs r0, #48
	mov r1, r11
	negs r0, r0
	cmp r1, #127
	ble .L_08183e2e
	mov r0, r11
	subs r0, #176
.L_08183e2e:
	movs r6, #16
	negs r6, r6
	cmp r0, r6
	ble .L_08183e38
	adds r0, r6, #0
.L_08183e38:
	lsls r0, r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_08015160
	ldr r3, .L_08183ee4
	mov r2, r11
	ldrh r0, [r3, r7]
	lsls r5, r2, #8
	adds r0, r0, r5
	bl Func_080150e4
	mov r3, r11
	cmp r3, #209
	ble .L_08183e5e
	ldr r4, .L_08183ee8
	adds r0, r5, r4
	bl Func_080150e4
.L_08183e5e:
	ldr r0, .L_08183ed0
	mov r1, r8
	ldrb r3, [r0, r1]
	mov r2, r11
	subs r3, r2, r3
	lsls r3, r3, #2
	adds r5, r3, #0
	subs r5, #64
	cmp r5, r6
	ble .L_08183e74
	adds r5, r6, #0
.L_08183e74:
	movs r3, #18
	adds r3, #255
	cmp r11, r3
	ble .L_08183f4c
	mov r4, r11
	movs r3, #135
	lsls r2, r4, #2
	lsls r3, r3, #3
	mov r0, r8
	subs r5, r3, r2
	cmp r0, #0
	bne .L_08183e9a
	movs r0, #137
	lsls r0, r0, #1
	subs r0, r0, r4
	lsls r0, r0, #10
	bl Func_080150e4
	b .L_08183f4c
.L_08183e9a:
	ldr r1, [sp, #16]
	ldr r2, .L_08183eec
	adds r0, r1, r2
	bl Func_080150e4
	b .L_08183f4c
	.2byte 0x0000
.L_08183ea8:
	.4byte gMapCellBuffer
.L_08183eac:
	.4byte 0xffd40000
.L_08183eb0:
	.4byte 0xfffc0000
.L_08183eb4:
	.4byte Data_08199210
.L_08183eb8:
	.4byte Data_0819974e
.L_08183ebc:
	.4byte IwramFillWords
.L_08183ec0:
	.4byte 0x2f2f2f2f
.L_08183ec4:
	.4byte 0xfffff000
.L_08183ec8:
	.4byte Data_08199754
.L_08183ecc:
	.4byte Data_02014800
.L_08183ed0:
	.4byte Data_08199756
.L_08183ed4:
	.4byte Data_08199758
.L_08183ed8:
	.4byte Data_08199760
.L_08183edc:
	.4byte Data_08199764
.L_08183ee0:
	.4byte 0xffddc000
.L_08183ee4:
	.4byte Data_0819975c
.L_08183ee8:
	.4byte 0xffff2e00
.L_08183eec:
	.4byte 0xfffddc00
.L_08183ef0:
	mov r4, r11
	mov r0, r8
	ldr r1, .L_08184248
	lsls r2, r0, #3
	lsls r3, r4, #2
	subs r3, r3, r2
	movs r2, #16
	adds r5, r3, r1
	negs r2, r2
	cmp r5, r2
	ble .L_08183f0a
	movs r5, #16
	negs r5, r5
.L_08183f0a:
	movs r3, #206
	lsls r3, r3, #7
	adds r3, #16
	mov r4, r8
	muls r4, r3
	mov r0, r11
	lsls r2, r0, #12
	adds r3, r4, #0
	ldr r1, .L_0818424c
	subs r3, r3, r2
	movs r2, #252
	lsls r2, r2, #6
	adds r6, r3, r1
	adds r2, #255
	cmp r6, r2
	ble .L_08183f8c
	bl Func_08014de4
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #9
	mov r2, r9
	bl Func_080151e4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #14
	movs r2, #0
	bl Func_08015160
	adds r0, r6, #0
	bl Func_0801521c
.L_08183f4c:
	movs r3, #64
	negs r3, r3
	cmp r5, r3
	ble .L_08183f8c
	movs r3, #82
	mov r0, r8
	muls r0, r3
	mov r1, r11
	adds r3, r0, #0
	subs r3, r1, r3
	movs r2, #22
	adds r0, r3, #0
	muls r0, r2
	ldr r2, .L_08184250
	mov r4, r10
	adds r0, r0, r2
	str r5, [r4, #20]
	bl SceneTransform_ApplyPitch
	mov r3, r11
	negs r0, r3
	lsls r0, r0, #10
	bl Func_08015068
	ldr r0, .L_08184254
	ldr r1, [sp, #36]
	movs r2, #64
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08183f8c:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r7, #2
	cmp r0, #2
	beq .L_08183f9a
	b .L_08183d86
.L_08183f9a:
	mov r0, r10
	bl Sys_Free
	ldr r0, [sp, #36]
	bl Sys_Free
	movs r3, #240
	ldr r1, [sp, #72]
	lsls r3, r3, #7
	adds r3, #228
	adds r2, r1, r3
.L_08183fb0:
	ldr r3, [r2]
	cmp r3, #1
	bls .L_08183fb0
	ldr r4, [sp, #72]
	movs r7, #240
	lsls r7, r7, #7
	adds r7, #232
	adds r2, r4, r7
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #20]
	ldr r2, [sp, #16]
	movs r1, #128
	movs r3, #128
	movs r7, #128
	lsls r1, r1, #13
	lsls r3, r3, #2
	movs r4, #1
	lsls r7, r7, #2
	adds r0, r0, r1
	adds r2, r2, r3
	add r11, r4
	adds r7, #90
	str r0, [sp, #20]
	str r2, [sp, #16]
	cmp r11, r7
	beq .L_08183ff0
	bl .L_08182d94
.L_08183ff0:
	movs r1, #128
	ldr r3, .L_08184258
	lsls r1, r1, #7
	ldr r2, .L_0818425c
	ldr r0, .L_08184260
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_08184264
	ldr r0, .L_08184268
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08184030
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
.L_08184030:
	strh r4, [r0]
	movs r0, #0
	ldr r1, [sp, #72]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #220
	mov r8, r0
	adds r5, r1, r2
.L_08184040:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #12
	bne .L_08184040
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0818426c
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08184270
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	ldr r7, [sp, #72]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #240
	adds r3, r7, r0
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_08184274
	bl Scheduler_RemoveCallback
	ldr r2, .L_08184278
	movs r3, #120
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0818427c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08184280
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r1, #128
	ldr r3, .L_08184258
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	ldr r2, .L_0818425c
	mov lr, r3
	.2byte 0xf800
	movs r1, #0
	movs r2, #11
	movs r0, #0
	bl Func_08164b2c
	mov r2, sp
	adds r2, #84
	str r2, [sp, #28]
	movs r1, #0
	mov r11, r1
.L_081840d6:
	mov r3, r11
	cmp r3, #16
	bne .L_081840f4
	ldr r4, [sp, #72]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r4, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	adds r2, r4, r0
	movs r3, #75
	str r3, [r2]
.L_081840f4:
	mov r1, r11
	cmp r1, #52
	ble .L_08184106
	movs r2, #4
	negs r2, r2
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_08164a4c
.L_08184106:
	mov r2, r11
	cmp r2, #48
	ble .L_0818411e
	mov r3, r11
	subs r3, #48
	negs r1, r3
	movs r2, #11
	lsls r3, r3, #1
	subs r2, r2, r3
	adds r0, r1, #0
	bl Func_08164b2c
.L_0818411e:
	mov r3, r11
	cmp r3, #0
	bne .L_08184194
	ldr r4, [sp, #72]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r4, r7
	movs r3, #3
	movs r0, #238
	str r3, [r2]
	lsls r0, r0, #7
	ldr r3, .L_08184284
	adds r0, #132
	adds r2, r4, r0
	str r3, [r2]
	ldr r5, .L_08184288
	movs r1, #0
	mov r8, r1
	movs r6, #31
.L_08184144:
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #120
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	movs r3, #8
	ands r0, r6
	subs r3, r3, r0
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #5
	str r3, [r5, #8]
	movs r2, #1
	movs r3, #32
	str r3, [r5, #24]
	add r8, r2
	adds r3, #224
	adds r5, #28
	cmp r8, r3
	bne .L_08184144
.L_08184194:
	ldr r7, .L_0818428c
	movs r4, #0
	mov r10, r7
	ldr r7, .L_08184288
	mov r8, r4
.L_0818419e:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_081841f6
	ldr r6, [r7, #8]
	ldr r2, .L_08184290
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r0, [sp, #72]
	movs r2, #158
	adds r1, r0, r1
	lsls r2, r2, #7
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r7, r3]
	ldr r3, .L_08184294
	mov r0, r10
	ldrb r5, [r3, r6]
	lsrs r3, r5, #1
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r7, r4]
	ldrb r4, [r0, r6]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #76]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	ldr r2, .L_08184298
	movs r1, #63
	bl BattleFxKernels_IntegrateVector2
	mov r0, r10
	ldrb r3, [r0, r6]
	ldr r2, [r7, #4]
	lsls r3, r3, #16
	cmn r2, r3
	bge .L_081841f6
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_081841f6:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #1
	adds r7, #28
	cmp r8, r2
	bne .L_0818419e
	ldr r4, [sp, #80]
	movs r3, #0
	ldr r2, [r4, #20]
	mov r8, r3
	cmp r2, #0
	beq .L_081842c2
	movs r6, #16
	movs r5, #36
.L_08184214:
	mov r7, r8
	lsls r3, r7, #3
	adds r3, #18
	cmp r11, r3
	bne .L_081842b8
	cmp r7, #0
	bne .L_0818429c
	ldr r2, [sp, #80]
	movs r3, #0
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r2, #1
	negs r2, r2
	movs r1, #7
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r0, #126
	bl Func_081180e8
	movs r0, #126
	bl Audio_PlayCue
	ldr r3, [sp, #80]
	ldr r2, [r3, #20]
	b .L_081842b8
.L_08184248:
	.4byte 0xfffffb28
.L_0818424c:
	.4byte 0x00138710
.L_08184250:
	.4byte 0xfffff448
.L_08184254:
	.4byte Data_02014400
.L_08184258:
	.4byte IwramFillWords
.L_0818425c:
	.4byte 0x3f3f3f3f
.L_08184260:
	.4byte 0x06004000
.L_08184264:
	.4byte gIoWriteQueue
.L_08184268:
	.4byte 0x04000208
.L_0818426c:
	.4byte 0x05000200
.L_08184270:
	.4byte 0x050001e8
.L_08184274:
	.4byte Func_08164bb4
.L_08184278:
	.4byte gCameraSceneParameters
.L_0818427c:
	.4byte 0x00000130
.L_08184280:
	.4byte IwramCopyWords
.L_08184284:
	.4byte 0x04040404
.L_08184288:
	.4byte Data_02015000
.L_0818428c:
	.4byte Data_081996bb
.L_08184290:
	.4byte Data_081996ca
.L_08184294:
	.4byte Data_081996ac
.L_08184298:
	.4byte 0xffffc000
.L_0818429c:
	movs r0, #126
	bl Audio_PlayCue
	ldr r4, [sp, #80]
	movs r2, #1
	ldrsh r0, [r5, r4]
	negs r2, r2
	movs r1, #7
	mov r3, r8
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r0, [sp, #80]
	ldr r2, [r0, #20]
.L_081842b8:
	movs r1, #1
	add r8, r1
	adds r5, #2
	cmp r8, r2
	bne .L_08184214
.L_081842c2:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	movs r0, #1
	bl Func_081969f8
	ldr r1, [sp, #28]
	adds r6, r0, #0
	movs r2, #7
	add r3, sp, #84
	str r1, [r6, #16]
	str r3, [sp, #28]
	strb r2, [r3]
	ldr r3, .L_081843e4
	strb r2, [r1, #1]
	str r3, [r6, #8]
	mov r2, r9
	movs r3, #6
	str r2, [r6, #12]
	str r3, [r6]
	ldr r4, [sp, #72]
	movs r7, #224
	lsls r7, r7, #3
	adds r3, r4, r7
	str r3, [r1, #4]
	mov r1, r11
	lsls r3, r1, #3
	ldr r2, .L_081843e8
	subs r3, #128
	movs r0, #0
	mov r10, r3
	mov r5, r11
	lsls r3, r1, #13
	mov r8, r0
	adds r7, r3, r2
	subs r5, #16
.L_0818430e:
	cmp r5, #0
	blt .L_0818437e
	movs r3, #0
	cmp r5, #15
	ble .L_0818431e
	movs r3, #16
	subs r3, r3, r5
	lsls r3, r3, #3
.L_0818431e:
	movs r4, #64
	negs r4, r4
	str r3, [r6, #20]
	cmp r3, r4
	ble .L_0818437e
	bl Func_08014de4
	mov r0, r10
	movs r1, #112
	subs r1, r1, r0
	lsls r1, r1, #16
	movs r0, #0
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	lsls r0, r0, #8
	bl Func_080151e4
	adds r0, r7, #0
	bl Func_0801521c
	mov r1, r8
	movs r2, #128
	lsls r0, r1, #14
	lsls r2, r2, #6
	adds r0, r0, r2
	bl Func_080150e4
	ldr r0, .L_081843ec
	bl SceneTransform_ApplyPitch
	mov r3, r11
	lsls r0, r3, #10
	bl Func_08015068
	ldr r0, .L_081843f0
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0818437e:
	ldr r0, .L_081843f4
	movs r1, #1
	movs r4, #64
	add r8, r1
	negs r4, r4
	mov r2, r8
	add r10, r4
	adds r7, r7, r0
	subs r5, #8
	cmp r2, #3
	bne .L_0818430e
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r9
	bl Sys_Free
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #72]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r7, #1
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	add r11, r7
	bl WaitFrames
	mov r0, r11
	cmp r0, #60
	beq .L_081843c4
	b .L_081840d6
.L_081843c4:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_081843f8
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #164
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081843e4:
	.4byte Data_08199364
.L_081843e8:
	.4byte 0xfffe0000
.L_081843ec:
	.4byte 0xfffff000
.L_081843f0:
	.4byte Data_08199210
.L_081843f4:
	.4byte 0xffff0000
.L_081843f8:
	.4byte Func_08143000
