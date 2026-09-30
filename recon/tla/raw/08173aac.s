.syntax unified
	.thumb
	.global Func_08173aac
	.thumb_func
Func_08173aac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #272
	str r0, [sp, #56]
	str r1, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #48]
	movs r0, #0
	ldr r1, [r3, #96]
	str r1, [sp, #44]
	ldr r2, [r3, #100]
	str r2, [sp, #28]
	ldr r3, [r3, #48]
	str r3, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08173b14
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_08173b48
	ldr r4, [sp, #56]
	movs r3, #1
	ldr r1, [r4, #4]
	add r2, sp, #260
	eors r1, r3
	lsls r1, r1, #4
	movs r3, #35
	adds r0, r4, #0
	orrs r1, r3
	add r3, sp, #248
	bl Func_0815585c
	movs r6, #216
	ldr r5, [sp, #48]
	lsls r6, r6, #7
	adds r6, #192
	movs r7, #224
	ldr r0, .L_08173b18
	adds r1, r5, r6
	movs r2, #1
	b .L_08173b1c
.L_08173b14:
	.4byte 0x00001010
.L_08173b18:
	.4byte 0x00000153
.L_08173b1c:
	movs r3, #1
	lsls r7, r7, #3
	bl Func_08157cf4
	ldr r0, .L_08173b44
	movs r2, #0
	movs r3, #0
	adds r1, r5, r7
	bl Func_08157cf4
	movs r2, #128
	ldr r3, .L_08173b40
	lsls r2, r2, #19
	adds r2, #12
	movs r0, #84
	strh r3, [r2]
	str r0, [sp, #32]
	b .L_08174094
.L_08173b40:
	.4byte 0x00000785
.L_08173b44:
	.4byte 0x000000d8
.L_08173b48:
	ldr r1, [sp, #52]
	cmp r1, #1
	bne .L_08173ba6
	ldr r2, [sp, #56]
	ldr r3, [sp, #52]
	ldr r1, [r2, #4]
	ldr r0, [sp, #56]
	eors r1, r3
	lsls r1, r1, #4
	movs r3, #33
	add r2, sp, #260
	orrs r1, r3
	add r3, sp, #248
	bl Func_0815585c
	ldr r4, [sp, #48]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r4, r5
	ldr r0, .L_08173d7c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r7, #216
	ldr r6, [sp, #48]
	lsls r7, r7, #7
	adds r7, #192
	adds r1, r6, r7
	movs r2, #1
	movs r3, #1
	ldr r0, .L_08173d80
	bl Func_08157cf4
	ldr r0, .L_08173d84
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08173d88
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r0, #48
	str r0, [sp, #32]
	b .L_08174094
.L_08173ba6:
	ldr r1, [sp, #52]
	cmp r1, #4
	bne .L_08173c38
	ldr r2, [sp, #56]
	movs r3, #1
	ldr r1, [r2, #4]
	ldr r0, [sp, #56]
	eors r1, r3
	movs r3, #144
	lsls r3, r3, #1
	lsls r1, r1, #4
	add r2, sp, #260
	orrs r1, r3
	add r3, sp, #248
	bl Func_0815585c
	movs r4, #216
	ldr r3, [sp, #48]
	lsls r4, r4, #7
	adds r4, #192
	adds r1, r3, r4
	ldr r0, .L_08173d8c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r6, [sp, #48]
	movs r5, #184
	lsls r5, r5, #5
	adds r1, r6, r5
	ldr r0, .L_08173d90
	movs r2, #1
	movs r3, #0
	movs r7, #224
	bl Func_08157cf4
	lsls r7, r7, #3
	movs r1, #128
	adds r0, r6, r7
	ldr r3, .L_08173d94
	lsls r1, r1, #5
	mov lr, r3
	.2byte 0xf800
	movs r0, #160
	lsls r0, r0, #4
	ldr r4, [sp, #48]
	adds r0, #12
	movs r6, #0
	movs r7, #0
	mov r12, r0
.L_08173c0a:
	mov r2, r12
	adds r1, r4, r2
	ldr r2, [sp, #48]
	adds r3, r7, r6
	lsls r3, r3, #3
	adds r3, r3, r2
	movs r0, #0
	adds r2, r3, r5
.L_08173c1a:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #40
	bne .L_08173c1a
	adds r6, #1
	adds r7, #4
	adds r4, #64
	cmp r6, #40
	bne .L_08173c0a
	movs r3, #42
	str r3, [sp, #32]
	b .L_08174094
.L_08173c38:
	ldr r4, [sp, #52]
	cmp r4, #6
	bne .L_08173c86
	ldr r5, [sp, #56]
	movs r3, #1
	ldr r1, [r5, #4]
	add r2, sp, #260
	eors r1, r3
	lsls r1, r1, #4
	movs r3, #131
	orrs r1, r3
	adds r0, r5, #0
	add r3, sp, #248
	bl Func_0815585c
	movs r7, #216
	ldr r6, [sp, #48]
	lsls r7, r7, #7
	adds r7, #192
	ldr r0, .L_08173d98
	adds r1, r6, r7
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #224
	lsls r2, r2, #3
	ldr r0, .L_08173d9c
	adds r1, r6, r2
	movs r3, #0
	movs r2, #0
	bl Func_08157cf4
	movs r3, #80
	movs r0, #84
	str r3, [sp, #32]
	bl Audio_PlayCue
	b .L_08174094
.L_08173c86:
	ldr r4, [sp, #52]
	cmp r4, #11
	bne .L_08173cb8
	ldr r5, [sp, #48]
	movs r6, #224
	lsls r6, r6, #3
	adds r1, r5, r6
	movs r2, #0
	movs r3, #0
	ldr r0, .L_08173d9c
	bl Func_08157cf4
	ldr r0, .L_08173da0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08173d88
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r7, #64
	str r7, [sp, #32]
	b .L_08174094
.L_08173cb8:
	ldr r0, [sp, #52]
	cmp r0, #14
	bne .L_08173d04
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08173d9c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_08173d80
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08173d88
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r5, #238
	ldr r4, [sp, #48]
	lsls r5, r5, #7
	movs r6, #238
	adds r5, #180
	lsls r6, r6, #7
	adds r2, r4, r5
	movs r3, #24
	adds r6, #184
	str r3, [r2]
	movs r7, #64
	adds r2, r4, r6
	movs r3, #0
	str r3, [r2]
	str r7, [sp, #32]
	b .L_08174094
.L_08173d04:
	ldr r0, [sp, #52]
	cmp r0, #13
	bne .L_08173d22
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08173da4
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r4, #40
	str r4, [sp, #32]
	b .L_08174094
.L_08173d22:
	ldr r5, [sp, #52]
	cmp r5, #12
	beq .L_08173d2e
	ldr r5, [sp, #52]
	cmp r5, #15
	bne .L_08173db4
.L_08173d2e:
	ldr r0, .L_08173da8
	ldr r1, .L_08173dac
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r6, [sp, #48]
	movs r7, #142
	lsls r7, r7, #7
	adds r5, r6, r7
	ldr r0, .L_08173db0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r0, #224
	lsls r0, r0, #3
	adds r1, r6, r0
	movs r2, #64
	movs r3, #64
	adds r0, r5, #0
	bl Func_0816ae40
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #180
	adds r2, r6, r1
	movs r3, #24
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #184
	adds r2, r6, r3
	movs r4, #48
	movs r3, #0
	str r3, [r2]
	str r4, [sp, #32]
	b .L_08174094
.L_08173d7c:
	.4byte 0x000000d4
.L_08173d80:
	.4byte 0x00000155
.L_08173d84:
	.4byte 0x00000148
.L_08173d88:
	.4byte IwramCopyWords
.L_08173d8c:
	.4byte 0x00000157
.L_08173d90:
	.4byte 0x00000156
.L_08173d94:
	.4byte IwramClearWords
.L_08173d98:
	.4byte 0x00000153
.L_08173d9c:
	.4byte 0x000000d7
.L_08173da0:
	.4byte 0x00000151
.L_08173da4:
	.4byte 0x000000f4
.L_08173da8:
	.4byte 0x000000c2
.L_08173dac:
	.4byte gMapCellBuffer
.L_08173db0:
	.4byte 0x000000d6
.L_08173db4:
	ldr r5, [sp, #52]
	cmp r5, #5
	bne .L_08173e46
	ldr r6, [sp, #56]
	movs r3, #226
	ldr r1, [r6, #4]
	add r2, sp, #260
	lsls r1, r1, #4
	orrs r1, r3
	adds r0, r6, #0
	add r3, sp, #248
	bl Func_0815585c
	ldr r3, .L_08173e0c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r7, [sp, #48]
	movs r2, #216
	lsls r2, r2, #7
	adds r2, #192
	adds r1, r7, r2
	ldr r0, .L_08173e10
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r7, r3
	ldr r0, .L_08173e14
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r4, #142
	lsls r4, r4, #7
	adds r1, r7, r4
	ldr r0, .L_08173e18
	movs r2, #0
	movs r3, #0
	b .L_08173e1c
	.2byte 0x0000
.L_08173e0c:
	.4byte 0x00000785
.L_08173e10:
	.4byte 0x00000151
.L_08173e14:
	.4byte 0x000000c1
.L_08173e18:
	.4byte 0x000000c9
.L_08173e1c:
	bl Func_08157cf4
	ldr r1, .L_08173f3c
	movs r2, #0
	movs r3, #0
	ldr r0, .L_08173f40
	bl Func_08157cf4
	ldr r0, .L_08173f44
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08173f48
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r5, #82
	str r5, [sp, #32]
	b .L_08174094
.L_08173e46:
	ldr r6, [sp, #52]
	cmp r6, #10
	bne .L_08173ecc
	movs r2, #1
	ldr r0, .L_08173f4c
	ldr r1, .L_08173f3c
	movs r3, #1
	bl Func_08157cf4
	ldr r7, [sp, #48]
	movs r1, #224
	lsls r1, r1, #3
	adds r0, r7, r1
	movs r1, #128
	ldr r3, .L_08173f50
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r3, #224
	lsls r3, r3, #3
	movs r2, #0
	adds r3, #4
	mov r8, r2
	movs r7, #0
	mov r12, r3
.L_08173e78:
	ldr r0, [sp, #48]
	mov r4, r8
	lsls r3, r4, #12
	movs r6, #0
	lsls r5, r7, #6
	adds r4, r3, r0
.L_08173e84:
	ldr r3, .L_08173f3c
	mov r2, r12
	adds r1, r4, r2
	movs r0, #0
	adds r2, r5, r3
.L_08173e8e:
	ldrb r3, [r2]
	adds r0, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r0, #24
	bne .L_08173e8e
	adds r6, #1
	adds r5, #24
	adds r4, #32
	cmp r6, #120
	bne .L_08173e84
	movs r4, #1
	add r8, r4
	mov r5, r8
	adds r7, #45
	cmp r5, #4
	bne .L_08173e78
	ldr r0, .L_08173f54
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08173f48
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r6, #60
	str r6, [sp, #32]
	b .L_08174094
.L_08173ecc:
	ldr r7, [sp, #52]
	cmp r7, #7
	bne .L_08173efe
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08173f58
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_08173f5c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08173f48
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r4, #76
	str r4, [sp, #32]
	b .L_08174094
.L_08173efe:
	ldr r5, [sp, #52]
	cmp r5, #8
	bne .L_08173f64
	ldr r6, [sp, #48]
	movs r7, #142
	lsls r7, r7, #7
	adds r5, r6, r7
	ldr r0, .L_08173f60
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r0, #224
	lsls r0, r0, #3
	adds r1, r6, r0
	movs r2, #64
	movs r3, #64
	adds r0, r5, #0
	bl Func_0816ae40
	movs r2, #128
	ldr r3, .L_08173f38
	lsls r2, r2, #19
	adds r2, #12
	movs r1, #16
	strh r3, [r2]
	str r1, [sp, #32]
	b .L_08174094
.L_08173f38:
	.4byte 0x00000785
.L_08173f3c:
	.4byte gMapCellBuffer
.L_08173f40:
	.4byte 0x000000c2
.L_08173f44:
	.4byte 0x00000148
.L_08173f48:
	.4byte IwramCopyWords
.L_08173f4c:
	.4byte 0x00000188
.L_08173f50:
	.4byte IwramClearWords
.L_08173f54:
	.4byte 0x00000149
.L_08173f58:
	.4byte 0x000000b4
.L_08173f5c:
	.4byte 0x00000155
.L_08173f60:
	.4byte 0x000000d6
.L_08173f64:
	ldr r2, [sp, #52]
	cmp r2, #9
	bne .L_08174018
	ldr r3, .L_08173fa8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, [sp, #48]
	movs r4, #222
	lsls r4, r4, #6
	adds r1, r3, r4
	ldr r0, .L_08173fac
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r6, [sp, #48]
	movs r7, #224
	lsls r7, r7, #3
	adds r5, r6, r7
	ldr r0, .L_08173fb0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r0, #0
	movs r1, #216
	mov r8, r0
	lsls r1, r1, #5
	movs r0, #63
	b .L_08173fb4
	.2byte 0x0000
.L_08173fa8:
	.4byte 0x00000785
.L_08173fac:
	.4byte 0x000000c2
.L_08173fb0:
	.4byte 0x00000161
.L_08173fb4:
	ldrb r2, [r5]
	lsls r3, r2, #1
	strb r3, [r5]
	movs r3, #252
	lsls r2, r2, #25
	lsls r3, r3, #22
	cmp r2, r3
	bls .L_08173fc6
	strb r0, [r5]
.L_08173fc6:
	movs r4, #1
	add r8, r4
	adds r5, #1
	cmp r8, r1
	bne .L_08173fb4
	ldr r5, [sp, #56]
	ldr r3, [r5, #24]
	cmp r3, #2
	bne .L_08173fee
	ldr r0, .L_081741ec
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081741f0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_08174002
.L_08173fee:
	ldr r0, .L_081741f4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081741f0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08174002:
	ldr r6, [sp, #56]
	movs r2, #10
	ldr r0, [r6, #8]
	movs r7, #36
	ldrsh r1, [r6, r7]
	movs r3, #80
	bl Func_08157530
	movs r0, #41
	str r0, [sp, #32]
	b .L_08174094
.L_08174018:
	ldr r1, [sp, #52]
	cmp r1, #2
	bne .L_0817403c
	ldr r2, [sp, #56]
	movs r3, #1
	ldr r1, [r2, #4]
	ldr r0, [sp, #56]
	eors r1, r3
	ldr r3, [sp, #52]
	lsls r1, r1, #4
	orrs r1, r3
	add r2, sp, #260
	add r3, sp, #248
	bl Func_0815585c
	movs r4, #76
	str r4, [sp, #32]
	b .L_0817405a
.L_0817403c:
	ldr r5, [sp, #48]
	movs r6, #238
	lsls r6, r6, #7
	movs r7, #238
	adds r6, #180
	lsls r7, r7, #7
	adds r2, r5, r6
	movs r3, #24
	adds r7, #184
	str r3, [r2]
	movs r0, #70
	adds r2, r5, r7
	movs r3, #0
	str r3, [r2]
	str r0, [sp, #32]
.L_0817405a:
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_081741f8
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r5, #216
	ldr r4, [sp, #48]
	lsls r5, r5, #7
	adds r5, #192
	adds r1, r4, r5
	movs r2, #1
	movs r3, #1
	ldr r0, .L_081741fc
	bl Func_08157cf4
	ldr r0, .L_081741ec
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081741f0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08174094:
	ldr r0, .L_08174200
	ldr r1, [sp, #28]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r6, [sp, #48]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r6, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	adds r2, r6, r0
	movs r3, #50
	str r3, [r2]
	ldr r1, [sp, #52]
	cmp r1, #0
	beq .L_081740c2
	cmp r1, #6
	bne .L_081740cc
.L_081740c2:
	movs r1, #200
	ldr r0, .L_08174204
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_081740cc:
	movs r1, #200
	ldr r0, .L_08174208
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #52]
	cmp r2, #2
	beq .L_081740e4
	cmp r2, #7
	beq .L_081740e4
	cmp r2, #4
	bne .L_081740ee
.L_081740e4:
	ldr r4, [sp, #56]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0817410a
	b .L_08174100
.L_081740ee:
	ldr r5, [sp, #52]
	cmp r5, #3
	beq .L_081740f8
	cmp r5, #5
	bne .L_08174114
.L_081740f8:
	ldr r6, [sp, #56]
	ldr r3, [r6, #4]
	cmp r3, #1
	bne .L_0817410a
.L_08174100:
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
	b .L_0817412e
.L_0817410a:
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	b .L_0817412e
.L_08174114:
	ldr r7, [sp, #56]
	ldr r3, [r7, #4]
	cmp r3, #0
	bne .L_08174126
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
	b .L_0817412e
.L_08174126:
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
.L_0817412e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r1, [sp, #32]
	movs r0, #0
	str r3, [sp, #36]
	mov r11, r0
	cmp r1, #0
	bne .L_08174144
	bl .L_08175f2e
.L_08174144:
	mov r2, sp
	adds r2, #60
	str r2, [sp, #8]
.L_0817414a:
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_08174174
	ldr r4, [sp, #48]
	movs r6, #225
	lsls r6, r6, #7
	movs r7, #0
	adds r5, r4, r6
	mov r8, r7
	movs r6, #31
.L_0817415e:
	bl Random16
	ands r0, r6
	lsls r0, r0, #4
	stmia r5!, {r0}
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #160
	bne .L_0817415e
	b .L_08174248
.L_08174174:
	ldr r2, [sp, #52]
	cmp r2, #6
	bne .L_081741b2
	ldr r3, [sp, #48]
	movs r4, #225
	movs r5, #0
	lsls r4, r4, #7
	mov r7, r11
	mov r8, r5
	adds r6, r3, r4
	lsls r5, r7, #11
.L_0817418a:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r2, r0, #1
	adds r2, r2, r0
	movs r3, #192
	lsls r2, r2, #1
	lsls r3, r3, #11
	movs r1, #1
	subs r3, r3, r2
	movs r0, #128
	add r8, r1
	asrs r3, r3, #10
	lsls r0, r0, #4
	mov r2, r8
	stmia r6!, {r3}
	adds r5, r5, r0
	cmp r2, #160
	bne .L_0817418a
	b .L_08174248
.L_081741b2:
	ldr r3, [sp, #52]
	cmp r3, #1
	bne .L_08174248
	mov r4, r11
	cmp r4, #31
	ble .L_08174248
	movs r0, #160
	ldr r4, .L_081741e8
	lsls r0, r0, #19
	movs r5, #0
	adds r0, #2
	mov r8, r5
.L_081741ca:
	ldrh r3, [r0]
	movs r1, #31
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	mov r6, r11
	lsrs r3, r3, #26
	ands r2, r4
	ands r3, r4
	cmp r6, #40
	ble .L_0817420c
	adds r1, #4
	adds r2, #4
	adds r3, #4
	b .L_08174212
.L_081741e8:
	.4byte 0x0000001f
.L_081741ec:
	.4byte 0x00000148
.L_081741f0:
	.4byte IwramCopyWords
.L_081741f4:
	.4byte 0x00000184
.L_081741f8:
	.4byte 0x000000b4
.L_081741fc:
	.4byte 0x00000151
.L_08174200:
	.4byte 0x00000134
.L_08174204:
	.4byte Func_08152474
.L_08174208:
	.4byte Func_08143000
.L_0817420c:
	adds r1, #2
	adds r2, #2
	adds r3, #2
.L_08174212:
	cmp r1, #31
	ble .L_08174218
	movs r1, #31
.L_08174218:
	cmp r2, #31
	ble .L_0817421e
	movs r2, #31
.L_0817421e:
	cmp r3, #31
	ble .L_08174224
	movs r3, #31
.L_08174224:
	lsls r3, r3, #10
	lsls r2, r2, #5
	movs r7, #1
	orrs r3, r2
	add r8, r7
	orrs r3, r1
	mov r1, r8
	strh r3, [r0]
	adds r0, #2
	cmp r1, #63
	bne .L_081741ca
	movs r1, #128
	ldr r3, .L_08174480
	ldr r0, [sp, #44]
	lsls r1, r1, #7
	ldr r2, .L_08174484
	mov lr, r3
	.2byte 0xf800
.L_08174248:
	ldr r2, [sp, #52]
	cmp r2, #3
	beq .L_08174250
	b .L_081743f6
.L_08174250:
	ldr r3, [sp, #56]
	add r6, sp, #224
	ldr r0, [r3, #8]
	adds r1, r6, #0
	bl Func_0815e20c
	movs r7, #236
	ldr r5, [sp, #56]
	add r7, sp
	mov r10, r7
	movs r4, #36
	ldrsh r0, [r5, r4]
	mov r1, r10
	bl Func_0815e20c
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6]
	ldr r3, [r7]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	mov r3, r11
	subs r3, #32
	str r0, [r7]
	cmp r3, #8
	bhi .L_081742fe
	mov r1, r11
	subs r1, #31
	cmp r1, #8
	ble .L_08174294
	movs r1, #8
.L_08174294:
	ldr r5, .L_08174488
	ldr r2, [r6]
	ldrsb r3, [r5, r1]
	subs r0, r0, r2
	muls r3, r0
	cmp r3, #0
	bge .L_081742a4
	adds r3, #15
.L_081742a4:
	asrs r3, r3, #4
	adds r4, r2, r3
	subs r3, r1, #1
	ldrsb r3, [r5, r3]
	muls r3, r0
	cmp r3, #0
	bge .L_081742b4
	adds r3, #15
.L_081742b4:
	asrs r3, r3, #4
	adds r7, r2, r3
	movs r0, #0
	subs r4, r4, r7
	mov r8, r0
	mov r9, r4
	movs r5, #0
.L_081742c2:
	adds r0, r5, #0
	movs r1, #3
	bl Math_Div
	ldr r3, [r6, #4]
	movs r4, #216
	subs r3, #20
	mov r12, r3
	ldr r3, [sp, #48]
	adds r2, r0, #0
	movs r1, #20
	lsls r4, r4, #7
	str r1, [sp, #0]
	adds r4, #192
	movs r1, #40
	adds r2, r7, r2
	str r1, [sp, #4]
	subs r2, #10
	adds r1, r3, r4
	ldr r0, [sp, #44]
	mov r3, r12
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r1, r8
	add r5, r9
	cmp r1, #4
	bne .L_081742c2
.L_081742fe:
	mov r2, r11
	cmp r2, #37
	bgt .L_08174306
	b .L_081747f2
.L_08174306:
	cmp r2, #38
	bne .L_0817439c
	ldr r7, [sp, #48]
	movs r3, #0
	mov r8, r3
.L_08174310:
	mov r4, r10
	ldr r3, [r4]
	movs r5, #128
	lsls r3, r3, #16
	str r3, [r7]
	lsls r5, r5, #1
	ldr r3, [r4, #4]
	adds r5, #255
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #1
	adds r5, r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	movs r1, #1
	ands r3, r0
	add r8, r1
	adds r3, #16
	mov r2, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #63
	bne .L_08174310
	movs r0, #134
	bl Func_081180e8
	ldr r4, [sp, #56]
	movs r1, #4
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_08118088
	movs r6, #238
	ldr r5, [sp, #48]
	lsls r6, r6, #7
	adds r6, #168
	movs r2, #8
	adds r3, r5, r6
	str r2, [r3]
	ldr r1, [sp, #56]
	movs r3, #0
	movs r7, #36
	ldrsh r0, [r1, r7]
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #5
	bl Func_0814cd48
.L_0817439c:
	ldr r6, .L_0817448c
	ldr r5, [sp, #48]
	movs r2, #0
	mov r8, r2
.L_081743a4:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_081743e8
	asrs r0, r0, #3
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r3, [sp, #28]
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r3, r1
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #3
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_081743e8:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #63
	bne .L_081743a4
	b .L_081747f2
.L_081743f6:
	ldr r1, [sp, #52]
	cmp r1, #6
	bne .L_08174472
	mov r2, r11
	lsls r6, r2, #11
	adds r0, r6, #0
	bl Trig_Sin
	add r5, sp, #260
	ldr r3, [r5]
	lsls r2, r0, #2
	adds r2, r2, r0
	lsrs r1, r3, #31
	adds r3, r3, r1
	lsls r2, r2, #2
	asrs r3, r3, #1
	asrs r2, r2, #16
	adds r2, r2, r3
	adds r0, r6, #0
	adds r7, r2, #0
	bl Trig_Cos
	ldr r3, [r5, #4]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r4, r0, #0
	mov r3, r11
	subs r7, #10
	subs r4, #24
	cmp r3, #44
	ble .L_0817443e
	lsls r0, r3, #1
	subs r3, r4, r0
	adds r4, r3, #0
	adds r4, #88
.L_0817443e:
	ldr r5, [sp, #48]
	movs r6, #216
	movs r3, #20
	lsls r6, r6, #7
	str r3, [sp, #0]
	adds r6, #192
	movs r3, #40
	adds r2, r7, #0
	str r3, [sp, #4]
	adds r1, r5, r6
	adds r3, r4, #0
	ldr r0, [sp, #44]
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
	movs r1, #170
	movs r2, #170
	lsls r1, r1, #8
	lsls r2, r2, #7
	mov r0, r11
	adds r1, #171
	adds r2, #85
	movs r3, #0
	bl Func_081496c8
	b .L_081747f2
.L_08174472:
	ldr r0, [sp, #52]
	cmp r0, #2
	beq .L_08174490
	cmp r0, #7
	beq .L_0817447e
	b .L_081745e8
.L_0817447e:
	b .L_081744f6
.L_08174480:
	.4byte IwramFillWords
.L_08174484:
	.4byte 0x01010101
.L_08174488:
	.4byte Data_08198c28
.L_0817448c:
	.4byte Data_08197410
.L_08174490:
	add r1, sp, #260
	ldr r3, [r1]
	mov r5, r11
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r7, r3, #0
	ldr r3, [r1, #4]
	subs r7, #10
	adds r6, r3, #0
	subs r6, #20
	cmp r5, #12
	ble .L_081744ac
	movs r5, #12
.L_081744ac:
	mov r1, r11
	lsls r1, r1, #9
	mov r9, r1
	mov r0, r9
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	asrs r3, r3, #16
	mov r0, r9
	subs r7, r7, r3
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	subs r3, r3, r5
	mov r2, r11
	subs r6, r6, r3
	cmp r2, #61
	bgt .L_081744f6
	movs r3, #20
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	movs r4, #216
	ldr r3, [sp, #48]
	lsls r4, r4, #7
	adds r4, #192
	adds r1, r3, r4
	ldr r0, [sp, #44]
	adds r2, r7, #0
	adds r3, r6, #0
	ldr r5, [sp, #36]
	mov lr, r5
	.2byte 0xf800
.L_081744f6:
	mov r6, r11
	cmp r6, #0
	bne .L_08174502
	movs r0, #142
	bl Audio_PlayCue
.L_08174502:
	mov r7, r11
	cmp r7, #62
	bne .L_0817450e
	movs r0, #139
	bl Audio_PlayCue
.L_0817450e:
	mov r0, r11
	cmp r0, #0
	bne .L_08174542
	ldr r5, [sp, #48]
	movs r1, #0
	mov r8, r1
.L_0817451a:
	bl Random16
	str r0, [r5, #8]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #64
	str r3, [r5, #20]
	bl Random16
	mov r3, r8
	movs r2, #1
	adds r3, #32
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #63
	bne .L_0817451a
.L_08174542:
	ldr r5, [sp, #56]
	movs r6, #0
	movs r4, #36
	ldrsh r0, [r5, r4]
	add r5, sp, #212
	adds r1, r5, #0
	bl Func_0815e20c
	mov r8, r6
	ldr r6, [sp, #48]
	mov r9, r5
.L_08174558:
	ldr r3, [r6, #24]
	mov r10, r6
	cmp r3, #31
	bhi .L_081745d4
	adds r2, r3, #0
	cmp r3, #0
	bge .L_08174568
	adds r2, r3, #7
.L_08174568:
	asrs r3, r2, #3
	ldr r0, [r6, #8]
	adds r7, r3, #2
	bl Trig_Sin
	ldr r3, [r6, #20]
	adds r1, r3, #0
	muls r1, r0
	mov r0, r9
	lsrs r3, r1, #31
	adds r1, r1, r3
	ldr r3, [r0]
	asrs r1, r1, #17
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r0, [r6, #8]
	adds r5, r1, r3
	bl Trig_Cos
	ldr r1, [r6, #20]
	mov r4, r9
	adds r3, r1, #0
	muls r3, r0
	ldr r2, [r4, #4]
	asrs r3, r3, #16
	adds r4, r3, r2
	ldr r3, [r6, #8]
	movs r0, #128
	lsls r0, r0, #2
	adds r3, r3, r0
	subs r1, #4
	str r3, [r6, #8]
	str r1, [r6, #20]
	cmp r1, #0
	ble .L_081745d2
	ldr r2, .L_08174780
	lsls r0, r7, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	subs r3, r4, r7
	adds r1, r2, r1
	lsrs r2, r7, #31
	adds r2, r7, r2
	asrs r2, r2, #1
	str r0, [sp, #4]
	subs r2, r5, r2
	str r7, [sp, #0]
	ldr r0, [sp, #44]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
.L_081745d2:
	ldr r3, [r6, #24]
.L_081745d4:
	movs r7, #1
	add r8, r7
	subs r3, #1
	mov r5, r10
	mov r0, r8
	str r3, [r5, #24]
	adds r6, #28
	cmp r0, #48
	bne .L_08174558
	b .L_081747f2
.L_081745e8:
	ldr r1, [sp, #52]
	cmp r1, #5
	beq .L_081745f0
	b .L_08174784
.L_081745f0:
	ldr r2, [sp, #48]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #228
	mov r4, r11
	adds r7, r2, r3
	cmp r4, #0
	bne .L_0817460e
	add r2, sp, #248
	ldr r3, [r2]
	str r3, [r7, #12]
	ldr r3, [r2, #4]
	str r3, [r7, #16]
	ldr r3, [r2, #8]
	str r3, [r7, #20]
.L_0817460e:
	mov r5, r11
	cmp r5, #47
	bgt .L_0817465a
	ldr r6, [sp, #56]
	ldr r0, [r6, #8]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	add r5, sp, #248
	ldr r3, [r5]
	ldr r0, [r6, #8]
	movs r1, #40
	subs r0, r0, r3
	bl Math_Div
	ldr r3, [r7, #12]
	movs r1, #40
	adds r3, r3, r0
	str r3, [r7, #12]
	movs r0, #240
	ldr r3, [r5, #4]
	lsls r0, r0, #14
	subs r0, r0, r3
	bl Math_Div
	ldr r3, [r7, #16]
	movs r1, #40
	adds r3, r3, r0
	str r3, [r7, #16]
	ldr r3, [r5, #8]
	ldr r0, [r6, #16]
	subs r0, r0, r3
	bl Math_Div
	ldr r3, [r7, #20]
	adds r3, r3, r0
	str r3, [r7, #20]
	b .L_081746da
.L_0817465a:
	ldr r2, [sp, #56]
	mov r5, r11
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #48]
	movs r4, #217
	lsls r4, r4, #3
	ldr r0, [r0]
	adds r1, r3, r4
	cmp r5, #48
	bne .L_081746aa
	movs r3, #0
	str r3, [r1]
	str r3, [r1, #4]
	str r3, [r1, #8]
	ldr r3, [r7, #12]
	ldr r2, [r0, #8]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_08174688
	adds r2, #31
.L_08174688:
	asrs r3, r2, #5
	str r3, [r1, #12]
	ldr r3, [r7, #16]
	negs r3, r3
	cmp r3, #0
	bge .L_08174696
	adds r3, #31
.L_08174696:
	asrs r3, r3, #5
	str r3, [r1, #16]
	ldr r3, [r7, #20]
	ldr r2, [r0, #16]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_081746a6
	adds r0, #31
.L_081746a6:
	asrs r3, r0, #5
	str r3, [r1, #20]
.L_081746aa:
	ldr r2, [r1]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	ldr r2, [r1, #8]
	adds r3, r3, r2
	str r3, [r7, #20]
	ldr r3, [r1]
	ldr r2, [r1, #12]
	adds r3, r3, r2
	str r3, [r1]
	ldr r2, [r1, #16]
	ldr r3, [r1, #4]
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r2, [r1, #20]
	ldr r3, [r1, #8]
	adds r3, r3, r2
	str r3, [r1, #8]
.L_081746da:
	bl Func_08014de4
	ldr r0, [sp, #24]
	adds r1, r0, #0
	adds r1, #12
	bl Func_080156e8
	adds r0, r7, #0
	add r5, sp, #200
	adds r1, r5, #0
	adds r0, #12
	bl Func_0815e1ec
	ldr r2, [r5]
	movs r1, #20
	asrs r2, r2, #1
	str r2, [r5]
	str r2, [r7]
	ldr r4, [sp, #36]
	ldr r3, [r5, #4]
	subs r2, #10
	str r3, [r7, #4]
	ldr r6, [sp, #48]
	movs r7, #216
	lsls r7, r7, #7
	str r1, [sp, #0]
	adds r7, #192
	movs r1, #40
	str r1, [sp, #4]
	subs r3, #20
	adds r1, r6, r7
	ldr r0, [sp, #44]
	mov r6, r11
	mov lr, r4
	.2byte 0xf800
	cmp r6, #0
	bne .L_0817472a
	movs r0, #140
	bl Audio_PlayCue
.L_0817472a:
	mov r7, r11
	cmp r7, #55
	bne .L_08174776
	ldr r0, [sp, #48]
	movs r1, #192
	ldr r3, [r5]
	lsls r1, r1, #3
	adds r1, #172
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #145
	ldr r3, [r5, #4]
	movs r7, #238
	str r3, [r2, #4]
	bl Func_081180e8
	ldr r3, [sp, #56]
	movs r1, #4
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118088
	ldr r5, [sp, #56]
	movs r3, #24
	movs r4, #36
	ldrsh r0, [r5, r4]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	ldr r6, [sp, #48]
	lsls r7, r7, #7
	adds r7, #168
	adds r2, r6, r7
	movs r3, #16
	str r3, [r2]
.L_08174776:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	b .L_081747f2
.L_08174780:
	.4byte Data_08197410
.L_08174784:
	ldr r0, [sp, #52]
	cmp r0, #0
	bne .L_081747f2
	mov r1, r11
	lsls r6, r1, #11
	adds r0, r6, #0
	bl Trig_Sin
	add r5, sp, #260
	ldr r3, [r5]
	lsls r2, r0, #2
	adds r2, r2, r0
	lsrs r1, r3, #31
	adds r3, r3, r1
	lsls r2, r2, #2
	asrs r3, r3, #1
	asrs r2, r2, #16
	adds r2, r2, r3
	adds r0, r6, #0
	adds r7, r2, #0
	bl Trig_Cos
	ldr r3, [r5, #4]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r4, r0, #0
	mov r2, r11
	subs r7, #10
	subs r4, #24
	cmp r2, #48
	ble .L_081747cc
	lsls r0, r2, #1
	subs r3, r4, r0
	adds r4, r3, #0
	adds r4, #96
.L_081747cc:
	movs r3, #40
	negs r3, r3
	cmp r4, r3
	ble .L_081747f2
	ldr r5, [sp, #48]
	movs r6, #216
	movs r3, #20
	lsls r6, r6, #7
	str r3, [sp, #0]
	adds r6, #192
	movs r3, #40
	str r3, [sp, #4]
	adds r2, r7, #0
	ldr r0, [sp, #44]
	adds r1, r5, r6
	adds r3, r4, #0
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
.L_081747f2:
	bl Func_08014de4
	ldr r2, .L_08174b08
	movs r3, #104
	movs r0, #128
	str r3, [r2, #16]
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #20]
	movs r0, #1
	bl Func_081969f8
	ldr r1, [sp, #8]
	mov r10, r0
	str r1, [r0, #16]
	ldr r2, [sp, #20]
	str r2, [r0, #12]
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_0817481e
	b .L_0817495c
.L_0817481e:
	mov r4, r11
	cmp r4, #0
	bne .L_08174870
	movs r5, #0
	mov r8, r5
	ldr r5, [sp, #48]
	movs r6, #0
.L_0817482c:
	mov r7, r8
	lsls r3, r7, #15
	str r3, [r5]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
	bl Random16
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r0
	movs r0, #128
	lsls r0, r0, #2
	adds r3, r3, r0
	str r3, [r5, #12]
	bl Random16
	movs r3, #127
	movs r1, #153
	ands r3, r0
	lsls r1, r1, #2
	adds r3, r3, r1
	negs r3, r3
	movs r2, #1
	lsls r3, r3, #7
	add r8, r2
	str r3, [r5, #16]
	mov r3, r8
	str r6, [r5, #24]
	subs r6, #8
	adds r5, #28
	cmp r3, #16
	bne .L_0817482c
.L_08174870:
	mov r4, r11
	cmp r4, #4
	bne .L_0817487c
	movs r0, #147
	bl Audio_PlayCue
.L_0817487c:
	mov r5, r11
	cmp r5, #48
	bne .L_0817488a
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
.L_0817488a:
	ldr r6, [sp, #8]
	mov r7, sp
	movs r3, #5
	adds r7, #60
	strb r3, [r6]
	str r7, [sp, #8]
	strb r3, [r7, #1]
	ldr r3, .L_08174b0c
	mov r1, r10
	movs r0, #7
	str r0, [r1]
	str r3, [r1, #8]
	ldr r5, [sp, #48]
	movs r2, #0
	mov r9, r7
	mov r8, r2
.L_081748aa:
	ldr r2, [r5, #24]
	adds r7, r5, #0
	cmp r2, #0
	blt .L_08174948
	movs r6, #234
	lsls r6, r6, #7
	movs r3, #16
	adds r6, #48
	negs r3, r3
	cmp r2, #6
	bgt .L_081748c4
	lsls r3, r2, #3
	subs r3, #64
.L_081748c4:
	cmp r2, #31
	ble .L_081748d6
	movs r3, #32
	subs r3, r3, r2
	movs r4, #64
	lsls r3, r3, #2
	negs r4, r4
	cmp r3, r4
	blt .L_0817494c
.L_081748d6:
	mov r1, r8
	mov r0, r10
	str r3, [r0, #20]
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	ldr r2, [sp, #48]
	lsls r3, r3, #1
	subs r3, r1, r3
	lsls r3, r3, #10
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r3
	adds r3, r3, r4
	mov r0, r9
	str r3, [r0, #4]
	bl Func_08014de4
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	ldr r0, [r5]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #12]
	adds r0, r0, r3
	str r0, [r5]
	bl Trig_Sin
	ldr r3, [sp, #260]
	lsls r0, r0, #4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, #64
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r1, [r5, #4]
	movs r2, #0
	bl Func_08015160
	asrs r2, r6, #1
	adds r0, r2, #0
	adds r1, r6, #0
	bl Func_080151e4
	ldr r0, .L_08174b10
	bl SceneTransform_ApplyPitch
	movs r2, #4
	ldr r0, .L_08174b14
	ldr r1, [sp, #20]
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	ldr r2, [r5, #24]
.L_08174948:
	adds r3, r2, #1
	str r3, [r7, #24]
.L_0817494c:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #6
	bne .L_081748aa
	bl .L_08175edc
.L_0817495c:
	ldr r3, [sp, #52]
	cmp r3, #6
	bne .L_08174a4e
	ldr r4, [sp, #8]
	mov r5, sp
	adds r5, #60
	movs r3, #5
	strb r3, [r4]
	str r5, [sp, #8]
	strb r3, [r5, #1]
	ldr r6, [sp, #52]
	ldr r3, .L_08174b0c
	mov r7, r10
	str r6, [r7]
	str r5, [r7, #16]
	str r3, [r7, #8]
	ldr r0, [sp, #20]
	mov r9, r5
	str r0, [r7, #12]
	movs r7, #0
.L_08174984:
	lsls r3, r7, #5
	cmp r11, r3
	blt .L_08174a44
	mov r1, r11
	subs r2, r1, r3
	movs r4, #128
	lsls r3, r2, #10
	lsls r4, r4, #5
	adds r6, r3, r4
	movs r3, #0
	cmp r2, #63
	ble .L_081749aa
	movs r3, #64
	subs r3, r3, r2
	movs r5, #64
	lsls r3, r3, #2
	negs r5, r5
	cmp r3, r5
	blt .L_08174a44
.L_081749aa:
	mov r0, r10
	str r3, [r0, #20]
	asrs r5, r6, #1
	bl Func_08014de4
	adds r1, r6, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl Func_080151e4
	mov r1, r11
	movs r2, #192
	lsls r0, r1, #8
	lsls r2, r2, #8
	adds r0, r0, r2
	bl Func_080150e4
	ldr r4, .L_08174b18
	mov r3, r11
	lsls r0, r3, #6
	adds r0, r0, r4
	bl SceneTransform_ApplyPitch
	movs r3, #178
	movs r6, #0
	lsls r3, r3, #1
	mov r8, r6
	mov r6, r11
	muls r6, r3
.L_081749e4:
	mov r0, r8
	movs r1, #3
	bl __modsi3
	ldr r1, [sp, #48]
	lsls r0, r0, #10
	movs r2, #224
	adds r0, r1, r0
	lsls r2, r2, #3
	adds r0, r0, r2
	mov r3, r9
	str r0, [r3, #4]
	bl Func_08014e38
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #85
	mov r0, r8
	muls r0, r3
	adds r0, r6, r0
	bl Func_080150e4
	movs r1, #232
	movs r0, #0
	lsls r1, r1, #15
	movs r2, #0
	bl Func_08015160
	adds r0, r5, #0
	adds r1, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_08174b14
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	bl Func_08014ea8
	mov r0, r10
	bl Func_08196a7c
	movs r4, #1
	add r8, r4
	mov r0, r8
	cmp r0, #12
	bne .L_081749e4
.L_08174a44:
	adds r7, #1
	cmp r7, #1
	bne .L_08174984
	bl .L_08175edc
.L_08174a4e:
	ldr r1, [sp, #52]
	cmp r1, #1
	bne .L_08174b24
	movs r3, #0
	mov r2, r10
	str r3, [r2, #20]
	ldr r3, [sp, #8]
	movs r2, #6
	strb r2, [r3]
	ldr r3, .L_08174b1c
	movs r5, #61
	mov r6, r10
	add r4, sp, #60
	add r5, sp
	str r4, [sp, #8]
	strb r2, [r5]
	str r3, [r6, #8]
	str r2, [r6]
	ldr r7, [sp, #48]
	movs r0, #224
	ldr r2, [sp, #56]
	lsls r0, r0, #3
	adds r3, r7, r0
	str r3, [sp, #64]
	ldr r3, [r2, #20]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	bne .L_08174a8c
	bl .L_08175edc
.L_08174a8c:
	mov r3, r11
	lsls r3, r3, #9
	mov r9, r3
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #36
	mov r4, r11
	muls r4, r3
	adds r3, r4, #0
	add r6, sp, #188
	movs r7, #36
	asrs r5, r3, #1
.L_08174aa4:
	ldr r1, [sp, #56]
	ldrsh r0, [r7, r1]
	adds r1, r6, #0
	bl Func_0815e20c
	bl Func_08014de4
	ldr r0, [r6]
	ldr r1, [r6, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r1, r1, #16
	movs r2, #0
	lsls r0, r0, #16
	bl Func_08015160
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	ldr r0, .L_08174b20
	adds r7, #2
	add r0, r9
	bl Func_08015068
	adds r0, r5, #0
	adds r1, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, .L_08174b14
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	ldr r4, [sp, #56]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	cmp r8, r3
	bne .L_08174aa4
	bl .L_08175edc
	.2byte 0x0000
.L_08174b08:
	.4byte gCameraSceneParameters
.L_08174b0c:
	.4byte Data_08199244
.L_08174b10:
	.4byte 0xffffc000
.L_08174b14:
	.4byte Data_08199210
.L_08174b18:
	.4byte 0xffffb890
.L_08174b1c:
	.4byte Data_08199340
.L_08174b20:
	.4byte 0xfffff000
.L_08174b24:
	ldr r5, [sp, #52]
	cmp r5, #5
	beq .L_08174b2c
	b .L_08174d26
.L_08174b2c:
	mov r6, r11
	cmp r6, #47
	ble .L_08174b34
	b .L_08174c58
.L_08174b34:
	movs r3, #16
	mov r7, r10
	negs r3, r3
	str r3, [r7, #20]
	ldr r0, [sp, #8]
	movs r2, #61
	add r1, sp, #60
	movs r3, #2
	add r2, sp
	strb r5, [r0]
	str r1, [sp, #8]
	strb r3, [r2]
	ldr r4, [sp, #48]
	movs r5, #142
	lsls r5, r5, #7
	adds r3, r4, r5
	str r3, [sp, #64]
	ldr r3, .L_08174ed8
	movs r6, #7
	str r1, [r7, #16]
	str r6, [r7]
	str r3, [r7, #8]
	ldr r0, [sp, #20]
	mov r1, r11
	str r0, [r7, #12]
	cmp r1, #0
	bne .L_08174bae
	ldr r5, [sp, #48]
	movs r2, #0
	mov r8, r2
	movs r6, #0
.L_08174b72:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	str r3, [r5, #4]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_08174b9e
	ldr r3, [r5, #4]
	negs r3, r3
	str r3, [r5, #4]
.L_08174b9e:
	movs r3, #1
	add r8, r3
	mov r4, r8
	str r6, [r5, #24]
	subs r6, #2
	adds r5, #28
	cmp r4, #16
	bne .L_08174b72
.L_08174bae:
	movs r5, #0
	ldr r6, [sp, #48]
	movs r0, #192
	mov r8, r5
	lsls r0, r0, #3
	movs r1, #128
	ldr r5, [sp, #48]
	adds r0, #228
	lsls r1, r1, #9
	mov r2, r11
	adds r7, r6, r0
	mov r9, r1
	lsls r6, r2, #1
.L_08174bc8:
	ldr r3, [r5, #24]
	ldr r2, [r5, #4]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r5]
	movs r2, #31
	adds r3, r6, #0
	ands r3, r2
	mov r4, r10
	strb r3, [r4, #25]
	ldr r3, [r5, #24]
	cmp r3, #31
	bhi .L_08174c4a
	bl Func_08014de4
	ldr r0, [r7]
	ldr r1, .L_08174edc
	lsls r0, r0, #16
	adds r0, r0, r1
	ldr r1, [r7, #4]
	ldr r2, .L_08174edc
	lsls r1, r1, #16
	adds r1, r1, r2
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	mov r1, r9
	mov r2, r9
	lsls r0, r0, #8
	bl Func_080151e4
	ldr r0, [r5]
	bl Func_080150e4
	ldr r2, [r5, #24]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #170
	adds r0, r2, #0
	muls r0, r3
	bl Trig_Sin
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #128
	adds r0, r0, r3
	lsls r1, r1, #10
	mov r2, r9
	bl Func_080151e4
	ldr r0, .L_08174ee0
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08174c4a:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r6, #7
	adds r5, #28
	cmp r0, #8
	bne .L_08174bc8
.L_08174c58:
	ldr r2, [sp, #56]
	add r5, sp, #176
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r4, [sp, #8]
	movs r7, #61
	movs r3, #7
	add r6, sp, #60
	add r7, sp
	strb r3, [r4]
	str r6, [sp, #8]
	strb r3, [r7]
	ldr r3, .L_08174ee4
	mov r2, r10
	movs r1, #7
	str r1, [r2]
	str r6, [r2, #16]
	str r3, [r2, #8]
	ldr r0, .L_08174ee8
	ldr r3, [sp, #20]
	str r0, [sp, #64]
	str r3, [r2, #12]
	movs r4, #0
	movs r7, #128
	mov r8, r4
	lsls r7, r7, #8
.L_08174c92:
	mov r6, r8
	lsls r3, r6, #2
	mov r0, r11
	subs r3, r0, r3
	adds r2, r3, #0
	subs r2, #55
	cmp r2, #0
	blt .L_08174d18
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #128
	lsls r3, r3, #12
	lsls r1, r1, #7
	adds r6, r3, r1
	movs r3, #0
	cmp r2, #7
	ble .L_08174cba
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #3
.L_08174cba:
	movs r4, #64
	mov r2, r10
	negs r4, r4
	str r3, [r2, #20]
	cmp r3, r4
	ble .L_08174d18
	bl Func_08014de4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	movs r1, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	adds r2, r7, #0
	bl Func_080151e4
	mov r1, r8
	movs r2, #128
	lsls r0, r1, #14
	lsls r2, r2, #6
	adds r0, r0, r2
	bl Func_080150e4
	ldr r0, .L_08174eec
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	bl Func_0801521c
	ldr r0, .L_08174ef0
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08174d18:
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #3
	bne .L_08174c92
	bl .L_08175edc
.L_08174d26:
	ldr r5, [sp, #52]
	cmp r5, #4
	beq .L_08174d2e
	b .L_08174e30
.L_08174d2e:
	mov r6, r11
	subs r6, #16
	cmp r6, #64
	bhi .L_08174e00
	cmp r6, #23
	bgt .L_08174d48
	ldr r7, [sp, #48]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	movs r3, #75
	b .L_08174d54
.L_08174d48:
	ldr r1, [sp, #48]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #50
.L_08174d54:
	str r3, [r2]
	ldr r4, [sp, #8]
	movs r7, #61
	add r5, sp, #60
	movs r3, #6
	add r7, sp
	strb r3, [r4]
	str r5, [sp, #8]
	strb r3, [r7]
	ldr r3, .L_08174ef4
	mov r0, r10
	movs r1, #7
	str r3, [r0, #8]
	str r1, [r0]
	ldr r7, [sp, #56]
	ldr r2, [sp, #48]
	movs r4, #224
	movs r5, #36
	ldrsh r0, [r7, r5]
	lsls r4, r4, #3
	add r5, sp, #176
	adds r3, r2, r4
	adds r1, r5, #0
	str r3, [sp, #64]
	bl Func_0815e20c
	lsls r3, r6, #3
	adds r3, r3, r6
	lsls r3, r3, #5
	subs r3, r3, r6
	lsls r3, r3, #4
	movs r1, #128
	movs r0, #0
	negs r3, r3
	lsls r1, r1, #9
	mov r8, r0
	adds r6, r3, r1
.L_08174d9e:
	cmp r6, #0
	ble .L_08174df0
	mov r2, r8
	lsls r3, r2, #4
	negs r3, r3
	cmp r3, #0
	ble .L_08174dae
	movs r3, #0
.L_08174dae:
	mov r4, r10
	str r3, [r4, #20]
	bl Func_08014de4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r1, r1, #16
	movs r2, #0
	lsls r0, r0, #16
	bl Func_08015160
	movs r0, #192
	lsls r0, r0, #8
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	adds r1, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	ldr r0, .L_08174ef0
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08174df0:
	movs r0, #1
	movs r7, #128
	add r8, r0
	lsls r7, r7, #7
	mov r1, r8
	adds r6, r6, r7
	cmp r1, #2
	bne .L_08174d9e
.L_08174e00:
	mov r2, r11
	cmp r2, #14
	bne .L_08174e0c
	movs r0, #212
	bl Audio_PlayCue
.L_08174e0c:
	mov r3, r11
	cmp r3, #24
	beq .L_08174e16
	bl .L_08175edc
.L_08174e16:
	ldr r5, [sp, #56]
	movs r3, #12
	movs r2, #1
	movs r4, #36
	ldrsh r0, [r5, r4]
	movs r1, #7
	str r3, [sp, #0]
	negs r2, r2
	movs r3, #0
	bl Func_0814cd48
	bl .L_08175edc
.L_08174e30:
	ldr r6, [sp, #52]
	cmp r6, #9
	beq .L_08174e38
	b .L_08175178
.L_08174e38:
	ldr r7, [sp, #56]
	mov r1, sp
	ldr r0, [r7, #8]
	adds r1, #176
	str r1, [sp, #16]
	bl Func_0815e21c
	mov r2, r11
	cmp r2, #0
	bne .L_08174f42
	ldr r3, [sp, #48]
	movs r4, #192
	movs r6, #36
	ldrsh r0, [r7, r6]
	lsls r4, r4, #3
	add r6, sp, #164
	adds r4, #228
	adds r1, r6, #0
	adds r5, r3, r4
	bl Func_0815e20c
	ldr r3, [r6]
	movs r7, #0
	str r3, [r5]
	movs r2, #1
	ldr r3, [r6, #4]
	mov r8, r7
	str r3, [r5, #4]
	ldr r3, [sp, #48]
	negs r2, r2
	adds r3, #24
.L_08174e76:
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #32
	bne .L_08174e76
	ldr r7, .L_08174ef8
	movs r2, #0
	mov r8, r2
	mov r9, r6
.L_08174e8c:
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	bl Random16
	adds r6, r0, #0
	bl Trig_Sin
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	muls r5, r0
	asrs r1, r5, #4
	str r1, [r7, #16]
	ldr r4, [sp, #56]
	ldr r3, [r4, #24]
	cmp r3, #0
	bne .L_08174efc
	ldr r3, [r7, #12]
	cmp r3, #0
	bge .L_08174ec8
	adds r3, #3
.L_08174ec8:
	asrs r3, r3, #2
	str r3, [r7, #12]
	adds r3, r1, #0
	cmp r1, #0
	bge .L_08174ed4
	adds r3, r1, #3
.L_08174ed4:
	asrs r3, r3, #2
	b .L_08174f10
.L_08174ed8:
	.4byte Data_08198c34
.L_08174edc:
	.4byte 0xffc00000
.L_08174ee0:
	.4byte Data_081991b0
.L_08174ee4:
	.4byte Data_08199364
.L_08174ee8:
	.4byte gMapCellBuffer
.L_08174eec:
	.4byte 0xfffff000
.L_08174ef0:
	.4byte Data_08199210
.L_08174ef4:
	.4byte Data_08199340
.L_08174ef8:
	.4byte Data_02014000
.L_08174efc:
	cmp r3, #1
	bne .L_08174f12
	ldr r3, [r7, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #12]
	lsrs r3, r5, #31
	adds r3, r1, r3
	asrs r3, r3, #1
.L_08174f10:
	str r3, [r7, #16]
.L_08174f12:
	mov r5, r9
	ldr r3, [r5]
	movs r6, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	add r8, r6
	ldr r3, [r5, #4]
	adds r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r0, #128
	adds r3, #8
	lsls r0, r0, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r8, r0
	bne .L_08174e8c
.L_08174f42:
	mov r1, r11
	cmp r1, #9
	bgt .L_08174fa2
	ldr r7, [sp, #16]
	ldr r4, [sp, #48]
	lsls r0, r1, #1
	lsls r3, r1, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r5, r3, r4
	ldr r3, [r7]
	movs r2, #0
	mov r8, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r7, #4]
	movs r6, #255
	adds r3, #8
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r1, #128
	ands r0, r6
	lsls r1, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #8
	str r0, [r5, #12]
	ldr r2, [sp, #56]
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08174f8c
	negs r3, r0
	str r3, [r5, #12]
.L_08174f8c:
	bl Random16
	movs r3, #128
	ands r0, r6
	lsls r3, r3, #1
	adds r0, r0, r3
	negs r0, r0
	lsls r0, r0, #4
	mov r4, r8
	str r0, [r5, #16]
	str r4, [r5, #24]
.L_08174fa2:
	movs r5, #0
	mov r8, r5
	ldr r5, [sp, #48]
.L_08174fa8:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_08174ff2
	cmp r3, #0
	bge .L_08174fb4
	adds r3, #3
.L_08174fb4:
	asrs r3, r3, #2
	ldr r6, [sp, #48]
	lsls r1, r3, #3
	movs r0, #2
	ldrsh r2, [r5, r0]
	adds r1, r1, r3
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #24
	lsls r1, r1, #7
	movs r7, #224
	str r0, [sp, #0]
	adds r1, r6, r1
	movs r0, #48
	lsls r7, r7, #3
	subs r3, #24
	str r0, [sp, #4]
	adds r1, r1, r7
	subs r2, #12
	ldr r0, [sp, #44]
	ldr r6, [sp, #36]
	mov lr, r6
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_081752f0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08174ff2:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #32
	bne .L_08174fa8
	mov r1, r11
	cmp r1, #9
	bgt .L_08175006
	b .L_0817512a
.L_08175006:
	ldr r3, [sp, #56]
	movs r2, #0
	mov r8, r2
	ldr r1, .L_081752f4
	ldr r2, [r3, #24]
	ldrb r3, [r1, r2]
	cmp r3, #0
	beq .L_0817506e
	ldr r5, .L_081752f8
	ldr r6, .L_081752fc
.L_0817501a:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08175062
	asrs r0, r0, #2
	adds r0, #3
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #28]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	movs r1, #62
	movs r2, #0
	adds r0, r5, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	ldr r1, .L_081752f4
	subs r3, #1
	str r3, [r5, #24]
	ldr r7, [sp, #56]
	ldr r2, [r7, #24]
.L_08175062:
	ldrb r3, [r1, r2]
	movs r0, #1
	add r8, r0
	adds r5, #28
	cmp r8, r3
	bne .L_0817501a
.L_0817506e:
	mov r1, r11
	cmp r1, #9
	ble .L_0817512a
	ldr r3, [sp, #48]
	movs r4, #192
	lsls r4, r4, #3
	adds r4, #228
	adds r6, r3, r4
	mov r3, r11
	subs r3, #10
	movs r7, #128
	lsls r3, r3, #15
	lsls r7, r7, #7
	adds r5, r3, r7
	movs r1, #0
	cmp r2, #0
	bne .L_081750a2
	adds r2, r5, #0
	cmp r5, #0
	bge .L_0817509e
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #3
	adds r2, r3, r0
.L_0817509e:
	asrs r5, r2, #2
	b .L_081750ac
.L_081750a2:
	cmp r2, #1
	bne .L_081750ac
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r5, r3, #1
.L_081750ac:
	mov r2, r11
	cmp r2, #15
	ble .L_081750b8
	movs r3, #16
	subs r3, r3, r2
	lsls r1, r3, #3
.L_081750b8:
	movs r3, #64
	negs r3, r3
	cmp r1, r3
	ble .L_0817512a
	mov r4, r10
	str r1, [r4, #20]
	ldr r0, [sp, #8]
	movs r3, #61
	add r1, sp, #60
	add r3, sp
	movs r2, #7
	movs r7, #7
	strb r7, [r0]
	str r1, [sp, #8]
	strb r2, [r3]
	ldr r3, .L_08175300
	movs r7, #7
	str r3, [r4, #8]
	str r7, [r4]
	ldr r0, [sp, #48]
	movs r1, #222
	lsls r1, r1, #6
	adds r3, r0, r1
	str r3, [sp, #64]
	bl Func_08014de4
	ldr r0, [r6]
	ldr r1, [r6, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	adds r0, r5, #0
	cmp r5, #0
	bge .L_0817510c
	adds r0, r5, #3
.L_0817510c:
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r0, r0, #2
	asrs r2, r2, #1
	adds r1, r5, #0
	bl Func_080151e4
	ldr r0, .L_08175304
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_0817512a:
	mov r2, r11
	cmp r2, #10
	bne .L_08175166
	movs r0, #134
	bl Func_081180e8
	movs r4, #238
	ldr r5, [sp, #56]
	ldr r3, [sp, #48]
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	ldr r3, [r5, #24]
	movs r1, #0
	lsls r3, r3, #1
	adds r3, #8
	str r3, [r2]
	movs r6, #36
	ldrsh r0, [r5, r6]
	bl Func_08118088
	movs r3, #12
	movs r7, #36
	ldrsh r0, [r5, r7]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_08175166:
	ldr r0, [sp, #56]
	ldr r1, [r0, #24]
	lsls r1, r1, #1
	adds r1, #8
	adds r0, r1, #0
	bl Func_08158ce0
	bl .L_08175edc
.L_08175178:
	ldr r1, [sp, #52]
	cmp r1, #11
	beq .L_08175180
	b .L_08175310
.L_08175180:
	mov r2, r11
	cmp r2, #0
	bne .L_08175236
	ldr r4, [sp, #56]
	add r5, sp, #152
	movs r3, #36
	ldrsh r0, [r4, r3]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r7, [sp, #48]
	movs r6, #0
	mov r8, r6
	mov r9, r5
.L_0817519c:
	bl Random16
	movs r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #7
	adds r5, r5, r0
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #128
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r7, #20]
	mov r1, r9
	ldr r3, [r1]
	movs r5, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	bl Random16
	mov r4, r9
	ldr r3, [r4, #4]
	movs r2, #31
	ands r2, r0
	subs r3, r3, r2
	adds r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	add r8, r5
	mov r6, r8
	str r0, [r7, #8]
	adds r7, #28
	cmp r6, #32
	bne .L_0817519c
	movs r0, #212
	bl Audio_PlayCue
	movs r0, #238
	ldr r7, [sp, #48]
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r7, r0
	movs r2, #16
	str r2, [r3]
	ldr r2, [sp, #56]
	movs r3, #0
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r2, #1
	movs r1, #10
	negs r2, r2
	str r6, [sp, #0]
	bl Func_0814cd48
.L_08175236:
	ldr r4, [sp, #8]
	mov r5, sp
	adds r5, #60
	movs r3, #5
	strb r3, [r4]
	mov r7, r10
	str r5, [sp, #8]
	strb r3, [r5, #1]
	movs r3, #6
	str r3, [r7]
	ldr r3, .L_08175308
	str r5, [r7, #16]
	str r3, [r7, #8]
	ldr r0, [sp, #20]
	adds r6, r5, #0
	str r0, [r7, #12]
	ldr r5, [sp, #48]
	movs r1, #0
	mov r8, r1
.L_0817525c:
	mov r2, r8
	lsls r3, r2, #1
	cmp r11, r3
	blt .L_081752e0
	mov r4, r11
	subs r2, r4, r3
	movs r3, #0
	cmp r2, #23
	ble .L_0817527c
	movs r3, #24
	subs r3, r3, r2
	movs r7, #64
	lsls r3, r3, #2
	negs r7, r7
	cmp r3, r7
	blt .L_081752e0
.L_0817527c:
	mov r0, r10
	str r3, [r0, #20]
	movs r1, #3
	mov r0, r8
	bl __modsi3
	ldr r1, [sp, #48]
	lsls r0, r0, #10
	movs r2, #224
	adds r0, r1, r0
	lsls r2, r2, #3
	adds r0, r0, r2
	str r0, [r6, #4]
	bl Func_08014de4
	ldr r3, .L_0817530c
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #128
	adds r1, r2, #0
	lsls r0, r0, #8
	bl Func_080151e4
	ldr r0, [r5, #8]
	bl Func_080150e4
	movs r0, #128
	lsls r0, r0, #8
	bl Func_0801521c
	movs r2, #4
	ldr r1, [sp, #20]
	ldr r0, .L_08175304
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
.L_081752e0:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #28
	cmp r4, #12
	bne .L_0817525c
	b .L_081758ea
	.2byte 0x0000
.L_081752f0:
	.4byte 0xfffff000
.L_081752f4:
	.4byte Data_08198c64
.L_081752f8:
	.4byte Data_02014000
.L_081752fc:
	.4byte Data_08197410
.L_08175300:
	.4byte Data_08199364
.L_08175304:
	.4byte Data_081991e0
.L_08175308:
	.4byte Data_08199244
.L_0817530c:
	.4byte 0xffc00000
.L_08175310:
	ldr r5, [sp, #52]
	cmp r5, #14
	beq .L_08175318
	b .L_08175468
.L_08175318:
	mov r6, r11
	cmp r6, #0
	bne .L_081753ae
	ldr r7, [sp, #56]
	add r5, sp, #140
	ldr r0, [r7, #8]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r7, [sp, #48]
	movs r0, #0
	mov r8, r0
	mov r9, r5
.L_08175332:
	bl Random16
	movs r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	movs r1, #128
	lsls r1, r1, #7
	ands r5, r0
	adds r5, r5, r1
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #128
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r7, #20]
	mov r2, r9
	ldr r3, [r2]
	movs r5, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	bl Random16
	mov r4, r9
	ldr r3, [r4, #4]
	movs r2, #31
	ands r2, r0
	subs r3, r3, r2
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	add r8, r5
	mov r6, r8
	str r0, [r7, #8]
	adds r7, #28
	cmp r6, #32
	bne .L_08175332
	movs r0, #182
	lsls r0, r0, #1
	adds r0, #255
	bl Audio_PlayCue
.L_081753ae:
	ldr r7, [sp, #8]
	mov r0, sp
	adds r0, #60
	movs r3, #5
	strb r3, [r7]
	mov r1, r10
	str r0, [sp, #8]
	strb r3, [r0, #1]
	movs r3, #6
	str r3, [r1]
	ldr r3, .L_08175728
	str r0, [r1, #16]
	str r3, [r1, #8]
	ldr r2, [sp, #20]
	movs r3, #0
	str r2, [r1, #12]
	ldr r5, [sp, #48]
	adds r6, r0, #0
	mov r8, r3
.L_081753d4:
	mov r4, r8
	lsls r3, r4, #1
	cmp r11, r3
	blt .L_08175458
	mov r7, r11
	subs r2, r7, r3
	movs r3, #0
	cmp r2, #23
	ble .L_081753f4
	movs r3, #24
	subs r3, r3, r2
	movs r0, #64
	lsls r3, r3, #2
	negs r0, r0
	cmp r3, r0
	blt .L_08175458
.L_081753f4:
	mov r1, r10
	str r3, [r1, #20]
	mov r0, r8
	movs r1, #3
	bl __modsi3
	ldr r2, [sp, #48]
	lsls r0, r0, #10
	movs r3, #224
	adds r0, r2, r0
	lsls r3, r3, #3
	adds r0, r0, r3
	str r0, [r6, #4]
	bl Func_08014de4
	ldr r3, .L_0817572c
	ldr r0, [r5]
	ldr r1, [r5, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #128
	adds r1, r2, #0
	lsls r0, r0, #8
	bl Func_080151e4
	ldr r0, [r5, #8]
	bl Func_080150e4
	movs r0, #128
	lsls r0, r0, #8
	bl Func_0801521c
	movs r2, #4
	ldr r1, [sp, #20]
	ldr r0, .L_08175730
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
.L_08175458:
	movs r4, #1
	add r8, r4
	mov r7, r8
	adds r5, #28
	cmp r7, #12
	bne .L_081753d4
	bl .L_08175edc
.L_08175468:
	ldr r0, [sp, #52]
	cmp r0, #10
	beq .L_08175470
	b .L_081757cc
.L_08175470:
	mov r1, r11
	cmp r1, #0
	bne .L_0817549e
	ldr r2, [sp, #56]
	ldr r0, [r2, #8]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	movs r1, #0
	adds r0, r5, #0
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #212
	bl Audio_PlayCue
.L_0817549e:
	mov r3, r11
	cmp r3, #14
	bne .L_081754b4
	ldr r4, [sp, #56]
	ldr r0, [r4, #8]
	bl GetBattleObjectSlotFar
	movs r1, #16
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_081754b4:
	mov r5, r11
	cmp r5, #22
	bne .L_081754c0
	movs r0, #212
	bl Audio_PlayCue
.L_081754c0:
	mov r6, r11
	cmp r6, #26
	bne .L_08175508
	movs r0, #144
	bl Func_081180e8
	movs r0, #238
	ldr r7, [sp, #48]
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r7, r0
	movs r3, #8
	str r3, [r2]
	ldr r2, [sp, #56]
	movs r3, #12
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	movs r1, #7
	bl Func_0814cd48
	ldr r4, [sp, #56]
	movs r1, #4
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_08118088
	movs r1, #128
	ldr r3, .L_08175734
	ldr r0, [sp, #44]
	lsls r1, r1, #7
	ldr r2, .L_08175738
	mov lr, r3
	.2byte 0xf800
.L_08175508:
	add r5, sp, #128
	mov r6, r11
	mov r9, r5
	cmp r6, #27
	bgt .L_08175514
	b .L_08175616
.L_08175514:
	cmp r6, #28
	bne .L_08175590
	ldr r1, [sp, #56]
	movs r7, #36
	ldrsh r0, [r1, r7]
	mov r1, r9
	ldr r7, .L_0817573c
	bl Func_0815e20c
	movs r3, #0
	mov r8, r3
.L_0817552a:
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	bl Random16
	adds r6, r0, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r1, r5, #0
	muls r1, r0
	asrs r1, r1, #6
	str r1, [r7, #16]
	ldr r3, [sp, #128]
	lsls r1, r1, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [r7, #12]
	asrs r3, r3, #1
	lsls r2, r2, #1
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [sp, #132]
	adds r3, #16
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r7, #4]
	bl Random16
	movs r3, #15
	movs r4, #1
	ands r3, r0
	add r8, r4
	adds r3, #16
	mov r5, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r5, #128
	bne .L_0817552a
	b .L_08175594
.L_08175590:
	add r6, sp, #128
	mov r9, r6
.L_08175594:
	ldr r5, .L_0817573c
	movs r7, #0
	mov r8, r7
.L_0817559a:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0817560a
	movs r6, #0
	b .L_081755a6
.L_081755a4:
	ldr r0, [r5, #24]
.L_081755a6:
	cmp r0, #0
	bge .L_081755ac
	adds r0, #7
.L_081755ac:
	asrs r0, r0, #3
	adds r0, #1
	ldr r2, .L_08175740
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #4
	adds r0, r5, #0
	movs r1, #63
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #4]
	movs r2, #208
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_081755fe
	ldr r3, [r5, #16]
	str r2, [r5, #4]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
.L_081755fe:
	adds r6, #1
	cmp r6, #4
	bne .L_081755a4
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0817560a:
	movs r6, #1
	add r8, r6
	mov r7, r8
	adds r5, #28
	cmp r7, #64
	bne .L_0817559a
.L_08175616:
	ldr r1, [sp, #56]
	mov r7, r10
	ldr r0, [r1, #8]
	mov r1, r9
	bl Func_0815e20c
	ldr r2, [sp, #8]
	movs r3, #5
	strb r3, [r2]
	add r3, sp, #60
	str r3, [sp, #8]
	str r3, [r7, #16]
	ldr r3, .L_08175744
	movs r5, #61
	movs r4, #7
	add r5, sp
	movs r6, #7
	str r3, [r7, #8]
	strb r4, [r5]
	str r6, [r7]
	ldr r0, [sp, #20]
	str r0, [r7, #12]
	bl Func_08014de4
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #128
	adds r1, r2, #0
	lsls r0, r0, #8
	bl Func_080151e4
	ldr r1, [sp, #56]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08175672
	mov r2, r9
	ldr r0, [r2]
	ldr r1, [r2, #4]
	subs r0, #136
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	b .L_08175686
.L_08175672:
	mov r3, r9
	ldr r0, [r3]
	ldr r1, [r3, #4]
	subs r0, #108
	subs r1, #56
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
.L_08175686:
	movs r4, #0
	mov r8, r4
.L_0817568a:
	bl Func_08014e38
	bl Random16
	movs r3, #3
	ldr r5, [sp, #48]
	ands r3, r0
	ldr r2, [sp, #8]
	lsls r3, r3, #12
	movs r6, #224
	adds r3, r5, r3
	lsls r6, r6, #3
	adds r3, r3, r6
	add r7, sp, #60
	str r3, [r2, #4]
	adds r0, r7, #0
	movs r3, #5
	movs r1, #7
	str r7, [sp, #8]
	strb r3, [r0]
	strb r1, [r2, #1]
	mov r2, r11
	cmp r2, #23
	ble .L_0817574c
	mov r4, r8
	ldr r6, .L_08175748
	lsls r3, r2, #4
	lsls r2, r4, #1
	subs r3, r3, r2
	adds r5, r3, r6
	cmp r5, #0
	blt .L_081757be
	cmp r5, #80
	ble .L_081756d0
	movs r5, #80
.L_081756d0:
	ldr r7, [sp, #56]
	ldr r3, [r7, #4]
	cmp r3, #0
	bne .L_081756ec
	lsls r0, r5, #1
	lsls r1, r5, #3
	adds r0, r0, r5
	subs r1, r1, r5
	lsls r0, r0, #13
	lsls r1, r1, #11
	movs r2, #0
	bl Func_08015160
	b .L_08175700
.L_081756ec:
	lsls r0, r5, #1
	adds r0, r0, r5
	lsls r1, r5, #3
	lsls r0, r0, #13
	subs r1, r1, r5
	negs r0, r0
	lsls r1, r1, #11
	movs r2, #0
	bl Func_08015160
.L_08175700:
	movs r3, #188
	lsls r3, r3, #8
	adds r3, #24
	adds r0, r5, #0
	muls r0, r3
	movs r1, #80
	bl Math_Div
	ldr r2, [sp, #56]
	movs r1, #192
	ldr r3, [r2, #4]
	lsls r1, r1, #7
	adds r0, r0, r1
	cmp r3, #0
	bne .L_08175720
	negs r0, r0
.L_08175720:
	bl Func_080150e4
	b .L_08175764
	.2byte 0x0000
.L_08175728:
	.4byte Data_08199244
.L_0817572c:
	.4byte 0xffc00000
.L_08175730:
	.4byte Data_081991e0
.L_08175734:
	.4byte IwramFillWords
.L_08175738:
	.4byte 0x3f3f3f3f
.L_0817573c:
	.4byte Data_02014000
.L_08175740:
	.4byte Data_08197410
.L_08175744:
	.4byte Data_08199268
.L_08175748:
	.4byte 0xfffffea0
.L_0817574c:
	mov r3, r8
	cmp r3, #0
	bne .L_081757be
	ldr r4, [sp, #56]
	ldr r0, .L_08175a1c
	ldr r3, [r4, #4]
	cmp r3, #0
	beq .L_08175760
	movs r0, #192
	lsls r0, r0, #7
.L_08175760:
	bl Func_080150e4
.L_08175764:
	mov r5, r8
	negs r3, r5
	lsls r3, r3, #3
	mov r6, r10
	mov r7, r11
	str r3, [r6, #20]
	cmp r7, #23
	bgt .L_08175780
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #170
	mov r2, r11
	muls r2, r3
	b .L_08175798
.L_08175780:
	mov r0, r11
	cmp r0, #31
	ble .L_08175794
	lsls r3, r0, #1
	add r3, r11
	movs r2, #128
	lsls r3, r3, #11
	lsls r2, r2, #11
	subs r2, r2, r3
	b .L_08175798
.L_08175794:
	movs r2, #128
	lsls r2, r2, #9
.L_08175798:
	cmp r2, #0
	ble .L_081757be
	lsls r1, r2, #1
	adds r0, r1, r2
	movs r2, #128
	lsls r0, r0, #1
	lsls r2, r2, #9
	bl Func_080151e4
	ldr r0, .L_08175a20
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	bl Func_08014ea8
.L_081757be:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #4
	beq .L_081757ca
	b .L_0817568a
.L_081757ca:
	b .L_081758ea
.L_081757cc:
	ldr r3, [sp, #52]
	cmp r3, #8
	beq .L_081757d4
	b .L_081758f4
.L_081757d4:
	ldr r4, [sp, #48]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #132
	adds r1, r4, r5
	movs r3, #50
	str r3, [r1]
	ldr r7, [sp, #8]
	movs r3, #61
	movs r6, #7
	add r2, sp, #60
	add r3, sp
	strb r6, [r7]
	str r2, [sp, #8]
	strb r6, [r3]
	ldr r3, .L_08175a24
	mov r4, r10
	str r3, [r4, #8]
	movs r3, #6
	str r3, [r4]
	ldr r5, [sp, #48]
	movs r6, #239
	lsls r6, r6, #7
	adds r2, r5, r6
	movs r0, #0
	movs r3, #1
	str r3, [r2]
	str r0, [r1]
	ldr r1, [sp, #56]
	movs r7, #224
	lsls r7, r7, #3
	adds r3, r5, r7
	add r5, sp, #116
	ldr r0, [r1, #8]
	adds r1, r5, #0
	str r3, [sp, #64]
	bl Func_0815e20c
	mov r2, r11
	cmp r2, #15
	bgt .L_08175894
	movs r3, #0
	mov r8, r3
	lsls r6, r2, #11
	adds r7, r5, #0
.L_0817582e:
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #2
	adds r5, r3, r0
	cmp r5, #0
	ble .L_0817588a
	mov r4, r10
	movs r3, #0
	str r3, [r4, #20]
	bl Func_08014de4
	ldr r0, [r7]
	mov r1, r8
	lsrs r3, r0, #31
	adds r0, r0, r3
	lsls r3, r1, #4
	ldr r1, [r7, #4]
	asrs r0, r0, #1
	adds r0, r0, r3
	subs r0, #72
	subs r1, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	adds r0, r2, #0
	adds r1, r5, #0
	bl Func_080151e4
	movs r0, #128
	lsls r0, r0, #6
	bl Func_080150e4
	ldr r0, .L_08175a28
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_0817588a:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #2
	bne .L_0817582e
.L_08175894:
	mov r4, r11
	cmp r4, #8
	bne .L_081758ea
	movs r0, #212
	bl Audio_PlayCue
	movs r6, #238
	ldr r5, [sp, #48]
	lsls r6, r6, #7
	adds r6, #168
	adds r3, r5, r6
	mov r7, r11
	str r7, [r3]
	movs r1, #128
	ldr r3, .L_08175a2c
	lsls r1, r1, #7
	ldr r0, [sp, #44]
	ldr r2, .L_08175a30
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #56]
	movs r0, #0
	ldr r3, [r1, #20]
	mov r8, r0
	cmp r3, #0
	beq .L_081758ea
	movs r6, #7
	movs r5, #36
.L_081758cc:
	ldr r2, [sp, #56]
	movs r1, #7
	ldrsh r0, [r5, r2]
	mov r3, r8
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r7, [sp, #56]
	movs r4, #1
	ldr r3, [r7, #20]
	add r8, r4
	adds r5, #2
	cmp r8, r3
	bne .L_081758cc
.L_081758ea:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	b .L_08175edc
.L_081758f4:
	ldr r0, [sp, #52]
	cmp r0, #13
	beq .L_081758fc
	b .L_08175a3c
.L_081758fc:
	mov r1, r11
	cmp r1, #0
	bne .L_08175908
	movs r0, #104
	bl Audio_PlayCue
.L_08175908:
	ldr r3, [sp, #56]
	mov r4, sp
	adds r4, #104
	movs r2, #36
	ldrsh r0, [r3, r2]
	adds r1, r4, #0
	mov r5, r11
	str r4, [sp, #12]
	movs r6, #16
	bl Func_0815e21c
	lsls r3, r5, #2
	subs r3, #64
	negs r6, r6
	cmp r3, r6
	blt .L_0817592c
	movs r3, #16
	negs r3, r3
.L_0817592c:
	mov r7, r10
	str r3, [r7, #20]
	ldr r2, [sp, #8]
	movs r3, #6
	str r2, [r7, #16]
	ldr r0, [sp, #20]
	str r3, [r7]
	ldr r3, .L_08175a34
	str r0, [r7, #12]
	str r3, [r7, #8]
	ldr r1, [sp, #48]
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r1, r4
	str r3, [r2, #4]
	add r5, sp, #60
	movs r6, #7
	movs r3, #4
	mov r7, r11
	str r5, [sp, #8]
	strb r6, [r5]
	strb r3, [r2, #1]
	cmp r7, #26
	bne .L_0817596e
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r1, r0
	movs r3, #8
	str r3, [r2]
	movs r0, #134
	bl Func_081180e8
.L_0817596e:
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	mov r3, r11
	mov r8, r1
	mov r9, r2
	lsls r7, r3, #9
	mov r6, r11
.L_0817597e:
	mov r4, r8
	lsls r3, r4, #1
	movs r5, #128
	adds r3, #16
	lsls r5, r5, #10
	cmp r11, r3
	blt .L_08175994
	mov r0, r11
	subs r3, r0, r3
	lsls r3, r3, #13
	subs r5, r5, r3
.L_08175994:
	movs r1, #128
	lsls r1, r1, #8
	cmp r5, r1
	ble .L_08175a06
	movs r2, #7
	adds r3, r6, #0
	ands r3, r2
	mov r4, r10
	lsls r3, r3, #4
	strb r3, [r4, #24]
	bl Func_08014de4
	ldr r1, [sp, #12]
	movs r2, #0
	ldr r0, [r1]
	movs r1, #128
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #60
	lsls r0, r0, #16
	lsls r1, r1, #12
	bl Func_08015160
	adds r1, r5, #0
	asrs r0, r5, #1
	mov r2, r9
	bl Func_080151e4
	mov r2, r8
	movs r3, #250
	lsls r3, r3, #4
	lsls r0, r2, #13
	adds r0, r0, r3
	bl SceneTransform_ApplyPitch
	mov r4, r11
	lsls r0, r4, #9
	bl Func_08015068
	adds r0, r7, #0
	bl Func_080150e4
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #8
	mov r2, r9
	bl Func_080151e4
	ldr r0, .L_08175a38
	ldr r1, [sp, #20]
	movs r2, #32
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08175a06:
	movs r0, #1
	movs r5, #128
	add r8, r0
	lsls r5, r5, #6
	mov r1, r8
	adds r7, r7, r5
	adds r6, #5
	cmp r1, #8
	bne .L_0817597e
	b .L_08175edc
	.2byte 0x0000
.L_08175a1c:
	.4byte 0xffffa000
.L_08175a20:
	.4byte Data_081991d0
.L_08175a24:
	.4byte Data_08199364
.L_08175a28:
	.4byte Data_081991e0
.L_08175a2c:
	.4byte IwramFillWords
.L_08175a30:
	.4byte 0x3f3f3f3f
.L_08175a34:
	.4byte Data_08198ec4
.L_08175a38:
	.4byte Data_08198cac
.L_08175a3c:
	ldr r2, [sp, #52]
	cmp r2, #12
	beq .L_08175a44
	b .L_08175c42
.L_08175a44:
	ldr r4, [sp, #8]
	mov r6, sp
	adds r6, #60
	movs r3, #7
	strb r3, [r4]
	str r6, [sp, #8]
	strb r3, [r6, #1]
	ldr r3, .L_08175d9c
	ldr r7, .L_08175da0
	mov r0, r10
	str r3, [r0, #8]
	movs r3, #6
	str r3, [r0]
	str r7, [r6, #4]
	ldr r1, [sp, #56]
	add r2, sp, #92
	mov r9, r2
	ldr r0, [r1, #8]
	mov r1, r9
	bl Func_0815e20c
	bl Func_08014de4
	mov r3, r9
	ldr r0, [r3]
	mov r4, r9
	lsrs r3, r0, #31
	ldr r1, [r4, #4]
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	mov r6, r11
	movs r5, #0
	bl Func_08015160
	cmp r6, #15
	bgt .L_08175abe
	lsls r3, r6, #1
	add r3, r11
	movs r1, #192
	lsls r3, r3, #12
	lsls r1, r1, #10
	subs r1, r1, r3
	asrs r2, r1, #1
	mov r7, r10
	adds r0, r2, #0
	str r5, [r7, #20]
	bl Func_080151e4
	ldr r0, .L_08175da4
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08175abe:
	bl Func_08014de4
	mov r1, r9
	ldr r0, [r1]
	ldr r1, [r1, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	mov r3, r11
	subs r3, #12
	cmp r3, #31
	bhi .L_08175b2a
	ldr r3, .L_08175da8
	mov r2, r11
	lsls r0, r2, #10
	adds r0, r0, r3
	bl Trig_Sin
	lsls r1, r0, #1
	adds r1, r1, r0
	ldr r4, [sp, #48]
	ldr r6, [sp, #8]
	lsrs r2, r1, #31
	movs r5, #224
	adds r2, r1, r2
	lsls r5, r5, #3
	adds r3, r4, r5
	asrs r2, r2, #1
	adds r0, r2, #0
	str r3, [r6, #4]
	bl Func_080151e4
	mov r7, r11
	movs r1, #128
	lsls r1, r1, #6
	lsls r0, r7, #9
	adds r0, r0, r1
	bl Func_080150e4
	ldr r0, .L_08175da4
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08175b2a:
	mov r2, r11
	cmp r2, #0
	bne .L_08175b76
	ldr r7, .L_08175dac
	ldr r5, [sp, #48]
	movs r3, #0
	movs r6, #192
	mov r8, r3
	lsls r6, r6, #2
.L_08175b3c:
	bl Random16
	str r6, [r5]
	adds r0, r7, #0
	movs r1, #48
	bl Math_Div
	str r0, [r5, #12]
	bl Random16
	movs r4, #1
	add r8, r4
	str r0, [r5, #4]
	mov r0, r8
	subs r7, #24
	adds r6, #24
	adds r5, #28
	cmp r0, #64
	bne .L_08175b3c
	ldr r0, .L_08175db0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08175db4
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08175b76:
	mov r1, r11
	cmp r1, #15
	bgt .L_08175be2
	ldr r6, [sp, #48]
	movs r2, #0
	mov r8, r2
.L_08175b82:
	movs r7, #0
.L_08175b84:
	ldr r0, [r6, #4]
	bl Trig_Sin
	ldr r3, [r6]
	mov r4, r9
	adds r5, r3, #0
	muls r5, r0
	ldr r3, [r4]
	asrs r5, r5, #21
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r0, [r6, #4]
	adds r5, r5, r3
	bl Trig_Cos
	ldr r3, [r6]
	ldr r2, .L_08175db8
	muls r3, r0
	ldrh r1, [r2, #6]
	ldr r0, [sp, #28]
	subs r5, #2
	movs r2, #4
	movs r4, #8
	asrs r3, r3, #20
	adds r1, r0, r1
	adds r3, #56
	str r2, [sp, #0]
	str r4, [sp, #4]
	adds r2, r5, #0
	ldr r0, [sp, #44]
	ldr r5, [sp, #36]
	mov lr, r5
	.2byte 0xf800
	ldr r2, [r6]
	ldr r3, [r6, #12]
	adds r7, #1
	adds r2, r2, r3
	str r2, [r6]
	cmp r7, #3
	bne .L_08175b84
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r6, #28
	cmp r0, #64
	bne .L_08175b82
.L_08175be2:
	movs r1, #0
	mov r8, r1
.L_08175be6:
	mov r2, r11
	cmp r2, #7
	ble .L_08175c0c
	cmp r2, #15
	bgt .L_08175bf8
	ldr r0, .L_08175dbc
	bl Func_0815f0a0
	b .L_08175c0c
.L_08175bf8:
	mov r3, r11
	cmp r3, #23
	bgt .L_08175c06
	ldr r0, .L_08175dc0
	bl Func_0815f0a0
	b .L_08175c0c
.L_08175c06:
	ldr r0, .L_08175dc4
	bl Func_0815f0a0
.L_08175c0c:
	movs r4, #1
	add r8, r4
	mov r5, r8
	cmp r5, #3
	bne .L_08175be6
	mov r6, r11
	cmp r6, #16
	bne .L_08175c38
	movs r0, #212
	bl Audio_PlayCue
	ldr r1, [sp, #56]
	movs r3, #30
	movs r2, #1
	movs r7, #36
	ldrsh r0, [r1, r7]
	negs r2, r2
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
.L_08175c38:
	mov r2, r11
	cmp r2, #28
	beq .L_08175c40
	b .L_08175edc
.L_08175c40:
	b .L_08175d90
.L_08175c42:
	ldr r3, [sp, #52]
	cmp r3, #15
	beq .L_08175c4a
	b .L_08175dd4
.L_08175c4a:
	ldr r5, [sp, #8]
	ldr r3, .L_08175d9c
	mov r6, sp
	ldr r7, .L_08175da0
	movs r4, #7
	adds r6, #60
	mov r0, r10
	strb r4, [r5]
	str r6, [sp, #8]
	str r3, [r0, #8]
	movs r3, #6
	strb r4, [r6, #1]
	str r7, [r6, #4]
	str r3, [r0]
	ldr r1, [sp, #56]
	add r2, sp, #80
	mov r9, r2
	ldr r0, [r1, #8]
	mov r1, r9
	bl Func_0815e20c
	mov r4, r11
	movs r3, #0
	mov r8, r3
	lsls r7, r4, #3
	lsls r6, r4, #14
	mov r5, r11
.L_08175c80:
	bl Func_08014de4
	ldr r0, [sp, #80]
	ldr r1, [sp, #84]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	cmp r5, #15
	bhi .L_08175ce4
	movs r3, #128
	lsls r3, r3, #11
	subs r1, r3, r6
	movs r3, #0
	cmp r5, #7
	bgt .L_08175cb0
	adds r3, r7, #0
	subs r3, #64
.L_08175cb0:
	lsrs r2, r1, #31
	adds r2, r1, r2
	asrs r2, r2, #1
	mov r0, r10
	str r3, [r0, #20]
	adds r0, r2, #0
	bl Func_080151e4
	mov r1, r8
	movs r2, #128
	lsls r0, r1, #14
	lsls r2, r2, #6
	adds r0, r0, r2
	bl Func_080150e4
	ldr r0, .L_08175dc8
	bl SceneTransform_ApplyPitch
	ldr r0, .L_08175dcc
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08175ce4:
	ldr r3, .L_08175dd0
	movs r4, #1
	add r8, r4
	mov r0, r8
	subs r7, #64
	adds r6, r6, r3
	subs r5, #8
	cmp r0, #2
	bne .L_08175c80
	bl Func_08014de4
	mov r1, r9
	ldr r0, [r1]
	ldr r1, [r1, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	mov r3, r11
	subs r3, #12
	cmp r3, #31
	bhi .L_08175d5c
	ldr r3, .L_08175da8
	mov r2, r11
	lsls r0, r2, #10
	adds r0, r0, r3
	bl Trig_Sin
	ldr r4, [sp, #48]
	adds r1, r0, #0
	lsls r1, r1, #1
	movs r5, #224
	asrs r2, r1, #1
	lsls r5, r5, #3
	adds r3, r4, r5
	adds r0, r2, #0
	mov r6, r11
	movs r7, #128
	str r3, [sp, #64]
	lsls r7, r7, #6
	bl Func_080151e4
	lsls r0, r6, #9
	adds r0, r0, r7
	bl Func_080150e4
	ldr r0, .L_08175da4
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08175d5c:
	mov r0, r11
	cmp r0, #0
	bne .L_08175d66
	bl Func_0815b410
.L_08175d66:
	mov r1, r11
	cmp r1, #16
	bne .L_08175d88
	movs r0, #212
	bl Audio_PlayCue
	ldr r3, [sp, #56]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #30
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	movs r3, #0
	bl Func_0814cd48
.L_08175d88:
	mov r4, r11
	cmp r4, #28
	beq .L_08175d90
	b .L_08175edc
.L_08175d90:
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
	b .L_08175edc
	.2byte 0x0000
.L_08175d9c:
	.4byte Data_08199364
.L_08175da0:
	.4byte gMapCellBuffer
.L_08175da4:
	.4byte Data_081991e0
.L_08175da8:
	.4byte 0xffffd000
.L_08175dac:
	.4byte 0xfffffd00
.L_08175db0:
	.4byte 0x00000184
.L_08175db4:
	.4byte IwramCopyWords
.L_08175db8:
	.4byte Data_08197410
.L_08175dbc:
	.4byte 0x00000154
.L_08175dc0:
	.4byte 0x00000150
.L_08175dc4:
	.4byte 0x00000152
.L_08175dc8:
	.4byte 0xfffff000
.L_08175dcc:
	.4byte Data_08199210
.L_08175dd0:
	.4byte 0xfffe0000
.L_08175dd4:
	mov r5, r10
	movs r3, #0
	str r3, [r5, #20]
	ldr r6, [sp, #8]
	movs r0, #61
	movs r3, #6
	add r7, sp, #60
	add r0, sp
	strb r3, [r6]
	str r7, [sp, #8]
	strb r3, [r0]
	ldr r3, .L_08175f5c
	movs r1, #7
	str r3, [r5, #8]
	str r1, [r5]
	ldr r2, [sp, #48]
	movs r4, #224
	ldr r5, [sp, #52]
	lsls r4, r4, #3
	adds r3, r2, r4
	str r3, [sp, #64]
	cmp r5, #2
	beq .L_08175e06
	cmp r5, #7
	bne .L_08175e22
.L_08175e06:
	mov r6, r11
	cmp r6, #31
	ble .L_08175e28
	mov r7, r11
	cmp r7, #59
	ble .L_08175e4e
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #184
	mov r2, r11
	muls r2, r3
	ldr r3, .L_08175f60
	subs r6, r3, r2
	b .L_08175e54
.L_08175e22:
	mov r0, r11
	cmp r0, #31
	bgt .L_08175e34
.L_08175e28:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	mov r6, r11
	muls r6, r3
	b .L_08175e54
.L_08175e34:
	mov r1, r11
	cmp r1, #39
	ble .L_08175e4e
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #184
	mov r2, r11
	muls r2, r3
	movs r3, #164
	lsls r3, r3, #10
	adds r3, #64
	subs r6, r3, r2
	b .L_08175e54
.L_08175e4e:
	movs r6, #187
	lsls r6, r6, #8
	adds r6, #128
.L_08175e54:
	cmp r6, #0
	ble .L_08175edc
	ldr r2, [sp, #52]
	cmp r2, #2
	beq .L_08175e62
	cmp r2, #7
	bne .L_08175e72
.L_08175e62:
	ldr r4, [sp, #56]
	add r5, sp, #68
	movs r3, #36
	ldrsh r0, [r4, r3]
	adds r1, r5, #0
	bl Func_0815e20c
	b .L_08175e7e
.L_08175e72:
	ldr r5, [sp, #56]
	ldr r0, [r5, #8]
	add r5, sp, #68
	adds r1, r5, #0
	bl Func_0815e20c
.L_08175e7e:
	bl Func_08014de4
	ldr r0, [r5]
	ldr r1, [r5, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #48
	movs r2, #0
	lsls r1, r1, #16
	lsls r0, r0, #16
	bl Func_08015160
	movs r0, #128
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	adds r0, r6, #0
	movs r1, #3
	bl Math_Div
	adds r1, r6, #0
	adds r2, r6, #0
	bl Func_080151e4
	ldr r6, [sp, #52]
	cmp r6, #2
	bne .L_08175ec4
	mov r7, r11
	negs r0, r7
	lsls r0, r0, #10
	bl Func_08015068
	b .L_08175ecc
.L_08175ec4:
	mov r1, r11
	lsls r0, r1, #10
	bl Func_08015068
.L_08175ecc:
	ldr r0, .L_08175f64
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08175edc:
	mov r0, r10
	bl Sys_Free
	ldr r0, [sp, #20]
	bl Sys_Free
	ldr r2, .L_08175f68
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	bl Func_081434f8
	ldr r2, [sp, #52]
	cmp r2, #1
	bne .L_08175f0c
	ldr r3, [sp, #48]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #228
	adds r2, r3, r4
.L_08175f04:
	ldr r3, [r2]
	ldr r3, [r2]
	cmp r3, #1
	bls .L_08175f04
.L_08175f0c:
	ldr r5, [sp, #48]
	movs r6, #240
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r5, r6
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #32]
	movs r7, #1
	add r11, r7
	cmp r11, r0
	beq .L_08175f2e
	bl .L_0817414a
.L_08175f2e:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #52]
	cmp r1, #0
	beq .L_08175f3e
	cmp r1, #6
	bne .L_08175f44
.L_08175f3e:
	ldr r0, .L_08175f6c
	bl Scheduler_RemoveCallback
.L_08175f44:
	ldr r0, .L_08175f70
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #272
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08175f5c:
	.4byte Data_08199340
.L_08175f60:
	.4byte 0x00037aa0
.L_08175f64:
	.4byte Data_08199210
.L_08175f68:
	.4byte gCameraSceneParameters
.L_08175f6c:
	.4byte Func_08152474
.L_08175f70:
	.4byte Func_08143000
