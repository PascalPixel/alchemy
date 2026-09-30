.syntax unified
	.thumb
	.global Func_080e3d04
	.thumb_func
Func_080e3d04:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #5
	adds r1, #168
	movs r0, #92
	sub sp, #32
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	ldr r2, [r2, #108]
	adds r3, #224
	ldr r3, [r3]
	str r2, [sp, #16]
	mov r8, r0
	ldr r0, [r3, #16]
	movs r1, #0
	str r0, [sp, #12]
	mov r10, r3
	ldr r5, [r3, #20]
	mov r11, r1
	adds r0, r5, #0
	bl Func_08020330
	movs r2, #209
	lsls r2, r2, #1
	cmp r0, r2
	bne .L_080e3d68
	ldr r3, [r5, #80]
	ldr r3, [r3, #40]
	ldrb r0, [r3, #5]
	cmp r0, #4
	bne .L_080e3d58
	movs r3, #1
	mov r11, r3
.L_080e3d58:
	cmp r0, #1
	bne .L_080e3d60
	movs r1, #2
	mov r11, r1
.L_080e3d60:
	cmp r0, #3
	bne .L_080e3d68
	movs r2, #3
	mov r11, r2
.L_080e3d68:
	ldr r2, .L_080e4000
	mov r0, r11
	lsls r3, r0, #2
	ldr r1, [r2, r3]
	movs r0, #8
	bl Func_080dc1b0
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #164
	movs r3, #0
	add r2, r8
	strb r3, [r2]
	mov r3, r10
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e3e74
	mov r1, r10
	ldr r3, [r1, #4]
	movs r2, #196
	lsls r2, r2, #5
	add r2, r8
	str r3, [r2]
	movs r2, #192
	ldr r3, [r1, #8]
	lsls r2, r2, #5
	movs r0, #128
	adds r2, #132
	lsls r0, r0, #13
	add r2, r8
	adds r3, r3, r0
	str r3, [r2]
	movs r2, #192
	ldr r3, [r1, #12]
	lsls r2, r2, #5
	adds r2, #136
	add r2, r8
	str r3, [r2]
	ldr r1, [sp, #16]
	movs r4, #156
	lsls r4, r4, #6
	adds r1, #20
	adds r4, #15
	movs r7, #0
	mov r9, r1
.L_080e3dc8:
	mov r2, r9
	ldr r6, [r2]
	ldr r3, [r6, #80]
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r6, #0
	beq .L_080e3e68
	ldr r3, [r6]
	cmp r3, #0
	beq .L_080e3e68
	movs r1, #205
	lsls r1, r1, #1
	cmp r2, r1
	beq .L_080e3dfc
	movs r3, #152
	lsls r3, r3, #1
	cmp r2, r3
	beq .L_080e3dfc
	movs r0, #193
	lsls r0, r0, #1
	cmp r2, r0
	beq .L_080e3dfc
	adds r1, #8
	cmp r2, r1
	bne .L_080e3e68
.L_080e3dfc:
	mov r3, r10
	movs r2, #30
	ldrsh r0, [r3, r2]
	movs r2, #24
	ldrsh r1, [r3, r2]
	adds r2, r7, #0
	str r4, [sp, #4]
	bl Func_080cdac0
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	ldr r4, [sp, #4]
	cmp r5, r3
	beq .L_080e3e68
	cmp r4, r5
	ble .L_080e3e68
	ldr r3, [r6, #80]
	ldr r3, [r3, #40]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_08020000
	movs r2, #192
	movs r3, #8
	ldrsb r3, [r0, r3]
	lsls r2, r2, #5
	adds r2, #164
	lsls r1, r3, #16
	add r2, r8
	movs r3, #1
	strb r3, [r2]
	movs r2, #196
	ldr r3, [r6, #8]
	lsls r2, r2, #5
	add r2, r8
	str r3, [r2]
	movs r2, #192
	ldr r3, [r6, #12]
	lsls r2, r2, #5
	adds r2, #132
	add r2, r8
	adds r3, r3, r1
	str r3, [r2]
	movs r2, #192
	ldr r3, [r6, #16]
	lsls r2, r2, #5
	adds r2, #136
	add r2, r8
	str r3, [r2]
	mov r2, r10
	strh r7, [r2, #26]
	str r6, [r2, #20]
	adds r4, r5, #0
.L_080e3e68:
	movs r3, #4
	adds r7, #1
	add r9, r3
	cmp r7, #79
	ble .L_080e3dc8
	b .L_080e3ee6
.L_080e3e74:
	ldr r0, [sp, #16]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #56
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080e3e94
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #164
	add r2, r8
	movs r3, #2
	b .L_080e3e9e
.L_080e3e94:
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #164
	add r2, r8
	movs r3, #3
.L_080e3e9e:
	strb r3, [r2]
	mov r2, r10
	ldr r3, [r2, #20]
	cmp r3, #0
	beq .L_080e3ebc
	ldr r3, [r3, #80]
	ldr r3, [r3, #40]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_08020000
	movs r3, #8
	ldrsb r3, [r0, r3]
	lsls r1, r3, #16
	b .L_080e3ec0
.L_080e3ebc:
	movs r1, #160
	lsls r1, r1, #13
.L_080e3ec0:
	mov r0, r10
	ldr r3, [r0, #4]
	movs r2, #196
	lsls r2, r2, #5
	add r2, r8
	str r3, [r2]
	movs r2, #192
	ldr r3, [r0, #8]
	lsls r2, r2, #5
	adds r2, #132
	add r2, r8
	adds r3, r3, r1
	str r3, [r2]
	movs r2, #192
	lsls r2, r2, #5
	ldr r3, [r0, #12]
	adds r2, #136
	add r2, r8
	str r3, [r2]
.L_080e3ee6:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #164
	add r3, r8
	movs r6, #0
	ldrsb r6, [r3, r6]
	cmp r6, #1
	bne .L_080e3f4a
	movs r3, #196
	lsls r3, r3, #5
	add r3, r8
	ldr r1, [r3]
	add r0, sp, #20
	movs r3, #192
	str r1, [r0]
	lsls r3, r3, #5
	adds r3, #132
	add r3, r8
	ldr r2, [r3]
	movs r3, #192
	str r2, [r0, #4]
	lsls r3, r3, #5
	adds r3, #136
	add r3, r8
	ldr r3, [r3]
	str r3, [r0, #8]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_080dc10c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080e3f2a
	b .L_080e4222
.L_080e3f2a:
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r1, #2
	bl Object_SetMode
	movs r0, #186
	bl Audio_PlayCue
	movs r0, #50
	b .L_080e3f96
.L_080e3f4a:
	cmp r6, #0
	bne .L_080e3fa8
	movs r3, #196
	lsls r3, r3, #5
	add r3, r8
	ldr r1, [r3]
	add r0, sp, #20
	movs r3, #192
	str r1, [r0]
	lsls r3, r3, #5
	adds r3, #132
	add r3, r8
	ldr r2, [r3]
	movs r3, #192
	str r2, [r0, #4]
	lsls r3, r3, #5
	adds r3, #136
	add r3, r8
	ldr r3, [r3]
	str r3, [r0, #8]
	movs r0, #112
	adds r0, #255
	bl Func_080dc10c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080e3f82
	b .L_080e4222
.L_080e3f82:
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r1, #2
	bl Object_SetMode
	movs r0, #186
	bl Audio_PlayCue
	movs r0, #45
.L_080e3f96:
	bl WaitFrames
	adds r0, r5, #0
	bl Func_080200c8
	movs r0, #10
	bl WaitFrames
	b .L_080e4222
.L_080e3fa8:
	movs r0, #207
	bl Audio_PlayCue
	mov r1, r10
	ldr r0, [r1, #16]
	movs r1, #0
	bl Func_080e1420
	mov r3, r10
	ldrh r2, [r3, #2]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #144
	add r3, r8
	strh r2, [r3]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #146
	add r3, r8
	mov r0, r11
	strh r2, [r3]
	cmp r0, #1
	beq .L_080e3fe6
	cmp r0, #1
	ble .L_080e4010
	mov r1, r11
	cmp r1, #2
	beq .L_080e3fea
	cmp r1, #3
	beq .L_080e3fee
	b .L_080e4010
.L_080e3fe6:
	ldr r0, .L_080e4004
	b .L_080e4012
.L_080e3fea:
	ldr r0, .L_080e4008
	b .L_080e4012
.L_080e3fee:
	ldr r0, .L_080e400c
	b .L_080e4012
.L_080e3ff2:
	ldr r0, [sp, #16]
	ldrh r2, [r5]
	movs r1, #211
	lsls r1, r1, #4
	adds r3, r0, r1
	strb r2, [r3]
	b .L_080e41c4
.L_080e4000:
	.4byte Data_080f0f90
.L_080e4004:
	.4byte 0x000001dd
.L_080e4008:
	.4byte 0x000001de
.L_080e400c:
	.4byte 0x000001df
.L_080e4010:
	ldr r0, .L_080e4184
.L_080e4012:
	bl Resource_GetTableEntry
	mov r1, r8
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #5
	adds r1, r5, #0
	mov r2, r8
	str r0, [sp, #8]
	bl VramBlock_LoadCached
	movs r3, #192
	lsls r3, r3, #5
	mov r9, r0
	adds r3, #140
	add r3, r8
	mov r2, r9
	movs r6, #148
	strh r2, [r3]
	lsls r6, r6, #5
	add r6, r8
	add r5, r8
	movs r7, #15
.L_080e4046:
	mov r3, r9
	str r3, [sp, #0]
	movs r3, #128
	movs r1, #15
	movs r2, #24
	lsls r3, r3, #24
	adds r0, r5, #0
	bl Func_080eaf98
	ldr r0, [sp, #12]
	bl Func_080db9cc
	adds r0, #10
	strh r0, [r5, #30]
	ldr r0, [sp, #12]
	bl Func_080db9c0
	ldrb r1, [r5, #9]
	movs r2, #13
	movs r3, #3
	negs r2, r2
	ands r0, r3
	adds r3, r2, #0
	ands r1, r3
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	lsls r0, r0, #2
	strb r3, [r5, #5]
	orrs r1, r0
	movs r3, #15
	ands r1, r3
	subs r7, #1
	subs r3, #16
	strb r1, [r5, #9]
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r7, #0
	bge .L_080e4046
	ldr r0, .L_080e4188
	bl Resource_GetTableEntry
	mov r1, r8
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	mov r2, r8
	mov r11, r0
	bl VramBlock_LoadCached
	movs r3, #192
	lsls r3, r3, #5
	mov r9, r0
	adds r3, #142
	add r3, r8
	mov r0, r9
	movs r6, #182
	movs r5, #162
	strh r0, [r3]
	lsls r6, r6, #5
	lsls r5, r5, #5
	add r6, r8
	add r5, r8
	movs r7, #15
.L_080e40ce:
	mov r1, r9
	movs r3, #128
	str r1, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #15
	lsls r3, r3, #23
	bl Func_080eaf98
	movs r3, #192
	ldrb r1, [r5, #9]
	lsls r3, r3, #8
	movs r2, #13
	str r3, [r5, #20]
	str r3, [r5, #24]
	negs r2, r2
	movs r3, #250
	strh r3, [r5, #30]
	adds r3, r2, #0
	ands r1, r3
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r5, #5]
	movs r3, #15
	ands r1, r3
	subs r7, #1
	subs r3, #16
	strb r1, [r5, #9]
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r7, #0
	bge .L_080e40ce
	movs r0, #1
	bl WaitFrames
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #151
	movs r3, #1
	add r2, r8
	movs r1, #144
	strb r3, [r2]
	ldr r0, .L_080e418c
	lsls r1, r1, #3
	bl Func_080145a8
	movs r3, #0
	mov r9, r3
.L_080e4132:
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #150
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e41ae
	movs r5, #192
	lsls r5, r5, #5
	adds r5, #148
	mov r2, r10
	add r5, r8
	movs r0, #30
	ldrsh r1, [r2, r0]
	ldr r6, .L_080e4180
	ldrh r2, [r5]
	movs r0, #192
	orrs r2, r6
	lsls r2, r2, #16
	lsls r0, r0, #23
	asrs r2, r2, #16
	adds r0, #5
	bl Func_080ce458
	cmp r0, #0
	beq .L_080e4190
	mov r2, r10
	movs r3, #24
	ldrsh r1, [r2, r3]
	movs r3, #0
	ldrsh r2, [r5, r3]
	bl Func_080ceafc
	b .L_080e4190
.L_080e4180:
	.4byte 0x00000100
.L_080e4184:
	.4byte 0x000001dc
.L_080e4188:
	.4byte 0x000001e1
.L_080e418c:
	.4byte Func_080e38c8
.L_080e4190:
	mov r2, r10
	movs r0, #30
	ldrsh r1, [r2, r0]
	ldrh r2, [r5]
	movs r0, #128
	orrs r2, r6
	lsls r2, r2, #16
	lsls r0, r0, #22
	asrs r2, r2, #16
	adds r0, #5
	bl Func_080ce458
	cmp r0, #0
	beq .L_080e41ae
	b .L_080e3ff2
.L_080e41ae:
	ldr r3, .L_080e423c
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_080e41c4
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #79
	ble .L_080e4132
.L_080e41c4:
	movs r0, #140
	adds r0, #255
	bl Audio_PlayCue
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #151
	movs r3, #0
	add r2, r8
	strb r3, [r2]
	mov r9, r3
.L_080e41da:
	movs r6, #148
	lsls r6, r6, #5
	add r6, r8
	ldr r3, [r6, #24]
	movs r1, #1
	negs r1, r1
	movs r7, #0
	cmp r3, r1
	bne .L_080e41fc
	mov r12, r3
.L_080e41ee:
	adds r7, #1
	adds r6, #28
	cmp r7, #15
	bgt .L_080e41fc
	ldr r3, [r6, #24]
	cmp r3, r12
	beq .L_080e41ee
.L_080e41fc:
	cmp r7, #16
	beq .L_080e4210
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #59
	ble .L_080e41da
.L_080e4210:
	ldr r0, .L_080e4240
	bl Func_08014644
	ldr r0, [sp, #8]
	bl Func_08014274
	mov r0, r11
	bl Func_08014274
.L_080e4222:
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e423c:
	.4byte gInput
.L_080e4240:
	.4byte Func_080e38c8
