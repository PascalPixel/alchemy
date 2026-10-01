.syntax unified
	.thumb
	.global Func_080e8db0
	.thumb_func
Func_080e8db0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #84
	movs r0, #92
	sub sp, #56
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #52]
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	ldr r2, [r2, #108]
	adds r3, #224
	ldr r3, [r3]
	str r2, [sp, #48]
	mov r10, r3
	ldr r1, [r3, #20]
	str r1, [sp, #44]
	ldr r2, [r1, #80]
	str r2, [sp, #40]
	cmp r1, #0
	beq .L_080e8dfe
	cmp r2, #0
	beq .L_080e8dfe
	ldrb r3, [r2, #20]
	ldrb r2, [r2, #21]
	movs r1, #128
	muls r3, r2
	lsls r1, r1, #4
	cmp r3, r1
	ble .L_080e8dfe
	b .L_080e9064
.L_080e8dfe:
	bl BattleEffect_InitializeSharedScene
	ldr r0, .L_080e9078
	bl Resource_GetTableEntry
	ldr r7, [sp, #52]
	adds r7, #84
	adds r1, r7, #0
	bl Resource_DecodeType01
	bl Resource_FindFreeEntry
	movs r1, #64
	adds r2, r7, #0
	str r0, [sp, #36]
	bl VramBlock_LoadCached
	movs r3, #208
	ldr r2, [sp, #52]
	movs r1, #128
	lsls r3, r3, #5
	lsls r1, r1, #5
	adds r3, #84
	adds r1, #84
	mov r8, r0
	movs r4, #0
	adds r6, r2, r3
	adds r5, r2, r1
.L_080e8e36:
	movs r3, #1
	ands r3, r4
	add r3, r8
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	str r4, [sp, #4]
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r1, #33
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	adds r1, #20
	movs r2, #224
	orrs r3, r2
	ldr r4, [sp, #4]
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	adds r4, #1
	subs r3, #241
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r4, #63
	ble .L_080e8e36
	mov r2, r10
	ldr r0, [r2, #16]
	movs r1, #0
	bl Func_080e1420
	ldr r3, [sp, #44]
	cmp r3, #0
	beq .L_080e8eae
	ldr r1, [sp, #40]
	cmp r1, #0
	beq .L_080e8eae
	adds r0, r1, #0
	movs r1, #0
	bl Animation_ApplyChildValueFar
	ldr r2, [sp, #40]
	ldr r1, [sp, #40]
	ldr r0, [r2, #40]
	ldrb r3, [r1, #21]
	ldrb r2, [r2, #20]
	adds r1, r7, #0
	muls r2, r3
	bl Func_080e43a4
.L_080e8eae:
	movs r0, #1
	bl Func_080e89e4
	bl Func_080e8cfc
	movs r0, #10
	bl WaitFrames
	movs r2, #1
	negs r2, r2
	str r2, [sp, #32]
	ldr r3, [sp, #40]
	movs r1, #0
	ldrb r3, [r3, #20]
	movs r2, #128
	str r3, [sp, #28]
	str r1, [sp, #24]
	str r1, [sp, #16]
	str r1, [sp, #20]
	str r1, [sp, #8]
	lsls r2, r2, #10
	mov r8, r2
	mov r10, r1
	mov r9, r1
.L_080e8ede:
	ldr r1, [sp, #20]
	movs r3, #15
	ands r3, r1
	cmp r3, #15
	bne .L_080e8eee
	movs r0, #152
	bl Audio_PlayCue
.L_080e8eee:
	ldr r2, [sp, #48]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_080e8f62
	ldr r2, [sp, #52]
	movs r4, #0
	ldr r3, [r2, #80]
	cmp r4, r3
	bge .L_080e8f62
	adds r7, r2, #0
	ldr r3, [sp, #8]
	adds r2, #32
	str r2, [sp, #12]
	mov r11, r3
	adds r7, #64
.L_080e8f16:
	ldr r2, [sp, #12]
	ldmia r2!, {r3}
	adds r1, r2, #0
	str r1, [sp, #12]
	ldr r6, [r3, #80]
	movs r3, #128
	lsls r3, r3, #11
	ldrh r5, [r7]
	adds r7, #2
	cmp r8, r3
	ble .L_080e8f56
	mov r1, r11
	lsls r0, r1, #11
	str r4, [sp, #4]
	bl Trig_Sin
	ldr r1, .L_080e907c
	ldr r4, [sp, #4]
	add r1, r8
	cmp r1, #0
	bge .L_080e8f44
	ldr r1, .L_080e9080
	add r1, r8
.L_080e8f44:
	str r4, [sp, #4]
	asrs r1, r1, #7
	ldr r2, .L_080e9084
	mov lr, r2
	.2byte 0xf800
	adds r0, r5, r0
	strh r0, [r6, #18]
	ldr r4, [sp, #4]
	b .L_080e8f58
.L_080e8f56:
	strh r5, [r6, #18]
.L_080e8f58:
	ldr r1, [sp, #52]
	adds r4, #1
	ldr r3, [r1, #80]
	cmp r4, r3
	blt .L_080e8f16
.L_080e8f62:
	ldr r1, [sp, #16]
	ldr r2, [sp, #8]
	adds r3, r1, #0
	adds r2, #3
	str r2, [sp, #8]
	adds r3, #1
	movs r0, #1
	mov r2, r8
	str r3, [sp, #16]
	bl Func_080e8b44
	mov r1, r10
	cmp r1, #1
	beq .L_080e8fb2
	cmp r1, #1
	bgt .L_080e8f88
	cmp r1, #0
	beq .L_080e8f94
	b .L_080e8ff4
.L_080e8f88:
	mov r2, r10
	cmp r2, #2
	beq .L_080e8fbe
	cmp r2, #3
	beq .L_080e8fe2
	b .L_080e8ff4
.L_080e8f94:
	mov r3, r9
	cmp r3, #40
	bne .L_080e8f9e
	movs r1, #1
	str r1, [sp, #24]
.L_080e8f9e:
	movs r2, #200
	lsls r2, r2, #5
	ldr r3, .L_080e9088
	adds r2, #153
	add r8, r2
	cmp r8, r3
	ble .L_080e8ff4
	movs r2, #1
	movs r1, #1
	b .L_080e8fda
.L_080e8fb2:
	mov r3, r9
	cmp r3, #40
	bne .L_080e8ff4
	movs r2, #1
	movs r1, #2
	b .L_080e8fda
.L_080e8fbe:
	mov r3, r9
	cmp r3, #50
	bne .L_080e8fca
	movs r1, #1
	negs r1, r1
	str r1, [sp, #24]
.L_080e8fca:
	ldr r2, .L_080e908c
	movs r3, #128
	add r8, r2
	lsls r3, r3, #10
	cmp r8, r3
	bgt .L_080e8ff4
	movs r2, #1
	movs r1, #3
.L_080e8fda:
	negs r2, r2
	mov r10, r1
	mov r9, r2
	b .L_080e8ff4
.L_080e8fe2:
	movs r3, #0
	mov r1, r9
	mov r8, r3
	cmp r1, #0
	bne .L_080e8ff4
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	mov r10, r2
.L_080e8ff4:
	ldr r3, [sp, #24]
	cmp r3, #1
	bne .L_080e9006
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	cmp r1, r2
	bgt .L_080e9006
	adds r1, #1
	str r1, [sp, #32]
.L_080e9006:
	ldr r3, [sp, #24]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_080e901a
	ldr r2, [sp, #32]
	cmp r2, #0
	blt .L_080e901a
	subs r2, #2
	str r2, [sp, #32]
.L_080e901a:
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #20]
	movs r2, #186
	lsls r2, r2, #2
	movs r3, #1
	adds r1, #1
	adds r2, #255
	add r9, r3
	str r1, [sp, #20]
	cmp r10, r2
	beq .L_080e9036
	b .L_080e8ede
.L_080e9036:
	ldr r3, [sp, #44]
	cmp r3, #0
	beq .L_080e9052
	ldr r1, [sp, #40]
	cmp r1, #0
	beq .L_080e9052
	adds r0, r1, #0
	movs r1, #16
	bl Animation_ApplyChildValueFar
	ldr r3, [sp, #44]
	ldr r2, [r3, #80]
	movs r3, #1
	strb r3, [r2, #25]
.L_080e9052:
	bl Func_080e8c9c
	bl Func_080e8d80
	ldr r0, [sp, #36]
	bl Resource_ResetEntry
	bl BattleFx_PrepareBufferInterpolation
.L_080e9064:
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080e9078:
	.4byte 0x000001ee
.L_080e907c:
	.4byte 0xfffc0000
.L_080e9080:
	.4byte 0xfffc007f
.L_080e9084:
	.4byte IwramMulQ16
.L_080e9088:
	.4byte 0x000bffff
.L_080e908c:
	.4byte 0xffffe667
