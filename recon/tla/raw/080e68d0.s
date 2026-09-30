.syntax unified
	.thumb
	.global Func_080e68d0
	.thumb_func
Func_080e68d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #60
	movs r0, #92
	sub sp, #44
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	mov r9, r0
	ldr r0, [r3, #16]
	ldr r5, [r2, #108]
	movs r1, #0
	movs r2, #96
	str r0, [sp, #28]
	str r1, [sp, #24]
	str r2, [sp, #20]
	str r2, [sp, #12]
	movs r4, #197
	lsls r4, r4, #1
	mov r10, r3
	adds r3, r5, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080e694a
	ldr r3, [r0, #8]
	ldr r1, .L_080e6be0
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	mov r0, r10
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r4, [sp, #28]
	mov r7, r10
	ldr r3, [r4, #12]
	adds r7, #4
	str r3, [r0, #8]
	ldr r3, [r4, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r0, #12]
	mov r2, r10
	movs r0, #128
	ldrh r1, [r2]
	lsls r0, r0, #13
	adds r2, r7, #0
	bl Func_0801489c
.L_080e694a:
	bl BattleEffect_InitializeSharedScene
	movs r0, #98
	movs r3, #0
	adds r0, #255
	movs r1, #0
	movs r2, #0
	bl Func_080dc10c
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #48
	add r3, r9
	mov r11, r0
	str r0, [r3]
	cmp r0, #0
	bne .L_080e696e
	b .L_080e70ce
.L_080e696e:
	add r4, sp, #24
	ldrb r4, [r4]
	movs r0, #208
	mov r3, r11
	lsls r0, r0, #4
	adds r3, #85
	adds r0, #76
	strb r4, [r3]
	adds r3, r5, r0
	ldrh r2, [r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080e69b8
	mov r3, r11
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_080e69c0
	mov r2, r11
	ldr r1, [r2, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	mov r1, r11
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	b .L_080e69c0
.L_080e69b8:
	mov r2, r11
	adds r2, #35
	movs r3, #1
	strb r3, [r2]
.L_080e69c0:
	ldr r4, [sp, #28]
	mov r0, r11
	ldrh r3, [r4, #6]
	movs r1, #0
	strh r3, [r0, #6]
	bl Object_SetMode
	mov r1, r10
	adds r1, #65
	str r1, [sp, #4]
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_080e69ee
	mov r0, r11
	movs r1, #6
	bl Animation_ApplyChildValuesFar
	ldr r2, [sp, #4]
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	bne .L_080e6a02
.L_080e69ee:
	mov r3, r10
	movs r0, #177
	ldr r1, [r3, #4]
	ldr r2, [r3, #8]
	lsls r0, r0, #1
	ldr r3, [r3, #12]
	b .L_080e6a10
.L_080e69fc:
	movs r4, #1
	str r4, [sp, #8]
	b .L_080e6b1a
.L_080e6a02:
	mov r2, r10
	mov r4, r10
	movs r0, #100
	ldr r1, [r2, #4]
	adds r0, #255
	ldr r2, [r2, #8]
	ldr r3, [r4, #12]
.L_080e6a10:
	bl Func_080dc10c
	str r0, [sp, #24]
	movs r3, #200
	lsls r3, r3, #5
	ldr r0, [sp, #24]
	adds r3, #52
	add r3, r9
	str r0, [r3]
	cmp r0, #0
	bne .L_080e6a28
	b .L_080e70ce
.L_080e6a28:
	ldr r3, [r0, #16]
	movs r1, #192
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r0, #16]
	adds r3, r0, #0
	movs r2, #0
	adds r3, #85
	adds r1, r0, #0
	strb r2, [r3]
	adds r1, #35
	movs r3, #1
	movs r4, #208
	strb r3, [r1]
	lsls r4, r4, #4
	str r2, [r0, #24]
	adds r4, #76
	adds r2, r5, r4
	ldrh r2, [r2]
	ands r3, r2
	cmp r3, #0
	bne .L_080e6a58
	bl Func_080dba5c
.L_080e6a58:
	movs r0, #138
	bl Audio_PlayCue
	mov r0, r10
	ldr r2, [r0, #8]
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r2, r3
	ldr r1, [r0, #4]
	ldr r3, [r0, #12]
	mov r0, r11
	bl Func_080dbed0
	movs r0, #10
	bl WaitFrames
	mov r0, r11
	movs r1, #1
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r4, #9
	mov r8, r4
.L_080e6a8a:
	mov r0, r11
	ldr r3, [r0, #12]
	ldr r1, .L_080e6be4
	adds r3, r3, r1
	str r3, [r0, #12]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	bge .L_080e6a8a
	mov r0, r11
	movs r1, #2
	bl Object_SetMode
	movs r0, #132
	bl Audio_PlayCue
	movs r4, #0
	str r4, [sp, #8]
	mov r7, r10
	adds r6, r5, #0
	mov r8, r4
	adds r7, #4
	adds r6, #20
.L_080e6ac2:
	ldmia r6!, {r5}
	cmp r5, #0
	beq .L_080e6b10
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080e6b10
	adds r0, r5, #0
	bl Func_08020330
	movs r1, #203
	lsls r1, r1, #1
	cmp r0, r1
	beq .L_080e6aea
	adds r3, r5, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080e6b10
.L_080e6aea:
	mov r2, r10
	ldr r3, [r2, #16]
	cmp r5, r3
	beq .L_080e6b10
	ldr r3, [r5, #80]
	ldrb r3, [r3, #27]
	cmp r3, #0
	beq .L_080e6b10
	ldrh r3, [r5, #32]
	adds r2, r5, #0
	adds r2, #8
	subs r3, #2
	adds r0, r7, #0
	movs r1, #4
	bl Func_080dbe80
	cmp r0, #0
	blt .L_080e6b10
	b .L_080e69fc
.L_080e6b10:
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #79
	ble .L_080e6ac2
.L_080e6b1a:
	ldr r5, [sp, #28]
	mov r1, r10
	adds r5, #34
	ldr r0, [r1, #4]
	ldrb r2, [r5]
	ldr r1, [r1, #12]
	bl Func_080dbdc8
	cmp r0, #255
	bne .L_080e6b32
	movs r2, #4
	str r2, [sp, #8]
.L_080e6b32:
	mov r4, r10
	ldr r1, [sp, #28]
	movs r3, #30
	ldrsh r0, [r4, r3]
	adds r2, r7, #0
	bl Func_080cda84
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_080e6b4c
	movs r2, #2
	str r2, [sp, #8]
.L_080e6b4c:
	mov r3, r10
	ldr r0, [r7]
	ldr r1, [r3, #12]
	ldrb r2, [r5]
	bl Func_080dbc04
	cmp r0, #0
	bne .L_080e6b60
	movs r4, #3
	str r4, [sp, #8]
.L_080e6b60:
	ldr r0, [sp, #8]
	cmp r0, #0
	beq .L_080e6b96
	movs r0, #2
	bl WaitFrames
	movs r1, #15
	mov r8, r1
.L_080e6b70:
	mov r2, r11
	ldr r0, [r2, #8]
	ldr r1, [r2, #12]
	ldr r2, [r2, #16]
	bl Func_080dc044
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r4, r8
	cmp r4, #0
	bge .L_080e6b70
	movs r0, #114
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	b .L_080e70ce
.L_080e6b96:
	mov r3, r10
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e6be8
	bl Random16
	movs r1, #128
	lsls r1, r1, #5
	cmp r0, r1
	bcs .L_080e6bbc
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #56
	add r2, r9
	movs r3, #1
	b .L_080e6bf2
.L_080e6bbc:
	ldr r3, [sp, #4]
	movs r2, #0
	ldrsb r2, [r3, r2]
	cmp r2, #0
	bne .L_080e6bd2
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #56
	add r3, r9
	strh r2, [r3]
	b .L_080e6bf4
.L_080e6bd2:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #56
	add r2, r9
	movs r3, #3
	b .L_080e6bf2
	.2byte 0x0000
.L_080e6be0:
	.4byte 0xfff00000
.L_080e6be4:
	.4byte 0xfffd8000
.L_080e6be8:
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #56
	add r2, r9
	movs r3, #2
.L_080e6bf2:
	strh r3, [r2]
.L_080e6bf4:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #56
	add r3, r9
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_080e6ce0
	ldr r0, .L_080e6f94
	bl Resource_GetTableEntry
	mov r1, r9
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #4
	adds r1, r5, #0
	mov r2, r9
	str r0, [sp, #20]
	bl VramBlock_LoadCached
	str r0, [sp, #16]
	movs r3, #200
	add r0, sp, #16
	ldrh r0, [r0]
	lsls r3, r3, #5
	adds r3, #44
	movs r7, #144
	add r3, r9
	movs r1, #0
	lsls r7, r7, #5
	mov r2, r9
	strh r0, [r3]
	mov r8, r1
	add r7, r9
	adds r0, r2, r5
.L_080e6c40:
	ldr r3, [sp, #16]
	movs r1, #8
	str r3, [sp, #0]
	movs r3, #128
	movs r2, #8
	lsls r3, r3, #23
	adds r6, r0, #0
	bl Func_080eaf98
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	ldr r0, [sp, #24]
	bl Func_080db9c0
	movs r3, #3
	ands r0, r3
	movs r4, #13
	ldrb r3, [r6, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	ldr r0, [sp, #24]
	bl Func_080db9cc
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	adds r0, #1
	strh r0, [r6, #30]
	str r3, [r6, #20]
	str r3, [r6, #24]
	mov r0, r10
	ldr r3, [r0, #4]
	str r3, [r7]
	ldr r3, [r0, #8]
	str r3, [r7, #4]
	ldr r3, [r0, #12]
	str r3, [r7, #8]
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	adds r2, r7, #0
	lsls r0, r0, #10
	bl Func_0801489c
	movs r1, #0
	str r1, [r7, #12]
	str r1, [r7, #16]
	str r1, [r7, #20]
	bl Random16
	adds r2, r7, #0
	adds r1, r0, #0
	adds r2, #12
	ldr r0, .L_080e6f98
	bl Func_0801489c
	mov r2, r8
	negs r3, r2
	lsls r3, r3, #1
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	adds r0, r6, #0
	mov r4, r8
	adds r0, #40
	adds r7, #28
	cmp r4, #63
	ble .L_080e6c40
	b .L_080e6ebe
.L_080e6ce0:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #56
	add r3, r9
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	bne .L_080e6dd6
	ldr r1, [sp, #4]
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_080e6d02
	ldr r0, .L_080e6f94
	bl Resource_GetTableEntry
	b .L_080e6d08
.L_080e6d02:
	ldr r0, .L_080e6f9c
	bl Resource_GetTableEntry
.L_080e6d08:
	mov r1, r9
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #4
	mov r2, r9
	adds r1, r5, #0
	str r0, [sp, #20]
	bl VramBlock_LoadCached
	str r0, [sp, #16]
	movs r3, #200
	add r2, sp, #16
	lsls r3, r3, #5
	ldrh r2, [r2]
	adds r3, #44
	add r3, r9
	movs r7, #144
	strh r2, [r3]
	lsls r7, r7, #5
	movs r3, #0
	mov r4, r9
	mov r8, r3
	add r7, r9
	adds r0, r4, r5
.L_080e6d3e:
	adds r6, r0, #0
	ldr r0, [sp, #16]
	movs r3, #128
	str r0, [sp, #0]
	movs r1, #8
	adds r0, r6, #0
	movs r2, #8
	lsls r3, r3, #23
	bl Func_080eaf98
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	ldr r0, [sp, #24]
	bl Func_080db9c0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	ldr r0, [sp, #24]
	bl Func_080db9cc
	adds r0, #1
	strh r0, [r6, #30]
	mov r2, r10
	ldr r3, [r2, #4]
	str r3, [r7]
	ldr r3, [r2, #8]
	str r3, [r7, #4]
	ldr r3, [r2, #12]
	str r3, [r7, #8]
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	adds r2, r7, #0
	lsls r0, r0, #10
	bl Func_0801489c
	movs r3, #0
	str r3, [r7, #12]
	str r3, [r7, #16]
	str r3, [r7, #20]
	bl Random16
	adds r2, r7, #0
	adds r1, r0, #0
	movs r0, #128
	adds r2, #12
	lsls r0, r0, #9
	bl Func_0801489c
	movs r1, #1
	mov r4, r8
	add r8, r1
	negs r3, r4
	adds r0, r6, #0
	mov r2, r8
	str r3, [r7, #24]
	adds r0, #40
	adds r7, #28
	cmp r2, #63
	ble .L_080e6d3e
	b .L_080e6ebe
.L_080e6dd6:
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #56
	add r3, r9
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #3
	bne .L_080e6ebe
	ldr r0, .L_080e6fa0
	bl Resource_GetTableEntry
	mov r1, r9
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [sp, #20]
	bl VramBlock_LoadCached
	str r0, [sp, #16]
	movs r3, #200
	add r0, sp, #16
	lsls r3, r3, #5
	ldrh r0, [r0]
	adds r3, #44
	add r3, r9
	movs r7, #144
	movs r6, #128
	strh r0, [r3]
	movs r1, #0
	lsls r7, r7, #5
	lsls r6, r6, #4
	mov r8, r1
	add r7, r9
	add r6, r9
.L_080e6e20:
	ldr r4, [sp, #16]
	mov r2, r8
	movs r3, #1
	ands r3, r2
	lsls r3, r3, #1
	adds r3, r4, r3
	str r3, [sp, #0]
	movs r1, #4
	adds r0, r6, #0
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	mov r0, r11
	bl Func_080db9c0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	mov r0, r11
	bl Func_080db9cc
	subs r0, #1
	strh r0, [r6, #30]
	mov r2, r10
	ldr r3, [r2, #4]
	movs r4, #160
	str r3, [r7]
	lsls r4, r4, #14
	ldr r3, [r2, #8]
	adds r3, r3, r4
	str r3, [r7, #4]
	ldr r3, [r2, #12]
	str r3, [r7, #8]
	bl Random16
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #10
	lsls r5, r5, #2
	adds r5, r5, r0
	bl Random16
	adds r2, r7, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_0801489c
	mov r1, r8
	negs r2, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	cmp r3, #0
	bge .L_080e6eac
	adds r3, #3
.L_080e6eac:
	movs r2, #1
	asrs r3, r3, #2
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r6, #40
	adds r7, #28
	cmp r3, #63
	ble .L_080e6e20
.L_080e6ebe:
	movs r7, #200
	lsls r7, r7, #5
	adds r7, #56
	add r7, r9
	movs r4, #0
	ldrsh r3, [r7, r4]
	ldrh r2, [r7]
	cmp r3, #0
	bne .L_080e6f6a
	ldr r0, .L_080e6fa4
	bl Resource_GetTableEntry
	mov r1, r9
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	movs r2, #0
	str r0, [sp, #12]
	bl VramBlock_LoadCached
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #46
	movs r6, #200
	add r3, r9
	lsls r6, r6, #5
	add r6, r9
	strh r0, [r3]
	movs r3, #128
	str r0, [sp, #0]
	lsls r3, r3, #24
	movs r2, #0
	adds r0, r6, #0
	movs r1, #16
	bl Func_080eaf98
	mov r1, r10
	ldr r0, [r1, #16]
	bl Func_080db9cc
	mov r2, r10
	strh r0, [r6, #30]
	ldr r0, [r2, #16]
	bl Func_080db9c0
	ldrb r2, [r6, #9]
	movs r3, #3
	ands r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #5]
	movs r1, #32
	orrs r2, r1
	lsls r0, r0, #2
	strb r2, [r6, #5]
	orrs r3, r0
	movs r2, #15
	ands r3, r2
	strb r3, [r6, #9]
	mov r4, r11
	ldr r3, [r4, #8]
	add r5, sp, #32
	str r3, [r5]
	movs r0, #128
	ldr r3, [r4, #12]
	lsls r0, r0, #14
	adds r3, r3, r0
	str r3, [r5, #4]
	adds r0, r5, #0
	ldr r3, [r4, #16]
	str r3, [r5, #8]
	bl Func_080dc390
	ldr r3, [r5]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	ldrh r2, [r7]
	movs r1, #0
	ldrsh r3, [r7, r1]
	cmp r3, #0
	beq .L_080e6f74
.L_080e6f6a:
	lsls r3, r2, #16
	movs r2, #192
	lsls r2, r2, #10
	cmp r3, r2
	bne .L_080e6fa8
.L_080e6f74:
	movs r0, #6
	bl WaitFrames
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #52
	add r3, r9
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #24]
	movs r0, #4
	bl WaitFrames
	b .L_080e6fae
	.2byte 0x0000
.L_080e6f94:
	.4byte 0x000001e1
.L_080e6f98:
	.4byte 0x00013333
.L_080e6f9c:
	.4byte 0x000001e2
.L_080e6fa0:
	.4byte 0x000001f3
.L_080e6fa4:
	.4byte 0x000001e9
.L_080e6fa8:
	movs r0, #10
	bl WaitFrames
.L_080e6fae:
	movs r5, #200
	movs r3, #200
	lsls r5, r5, #5
	lsls r3, r3, #5
	adds r5, #40
	adds r3, #42
	movs r2, #0
	add r3, r9
	add r5, r9
	movs r1, #144
	strh r2, [r5]
	lsls r1, r1, #3
	strh r2, [r3]
	ldr r0, .L_080e70f4
	bl Func_080145a8
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #56
	add r3, r9
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #1
	bne .L_080e7012
	movs r2, #186
	movs r0, #0
	ldrsh r3, [r5, r0]
	lsls r2, r2, #2
	adds r2, #255
	movs r6, #0
	cmp r3, r2
	beq .L_080e709c
	movs r5, #200
	lsls r5, r5, #5
	adds r5, #40
	mov r8, r2
	add r5, r9
.L_080e6ff8:
	cmp r6, #24
	bne .L_080e7000
	bl Func_080e716c
.L_080e7000:
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	ldrsh r3, [r5, r1]
	adds r6, #1
	cmp r3, r8
	bne .L_080e6ff8
	b .L_080e709c
.L_080e7012:
	mov r3, r10
	movs r2, #30
	ldrsh r0, [r3, r2]
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	mov r3, r10
	lsls r0, r0, #23
	movs r4, #30
	ldrsh r1, [r3, r4]
	adds r0, #5
	bl Func_080ce458
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #40
	add r3, r9
	movs r2, #186
	movs r4, #0
	ldrsh r3, [r3, r4]
	lsls r2, r2, #2
	adds r2, #255
	adds r7, r0, #0
	movs r6, #0
	cmp r3, r2
	beq .L_080e708e
	mov r8, r2
.L_080e704a:
	movs r0, #1
	movs r5, #0
	bl WaitFrames
	cmp r6, #30
	bne .L_080e7058
	movs r5, #1
.L_080e7058:
	cmp r6, #50
	bne .L_080e705e
	movs r5, #2
.L_080e705e:
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #196
	add r3, r10
	strh r5, [r3]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #198
	add r3, r10
	strh r6, [r3]
	adds r2, r6, #0
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_080ceafc
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #40
	add r3, r9
	movs r0, #0
	ldrsh r3, [r3, r0]
	adds r6, #1
	cmp r3, r8
	bne .L_080e704a
.L_080e708e:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	adds r0, r7, #0
	adds r2, r6, #0
	bl Func_080ceafc
.L_080e709c:
	ldr r0, .L_080e70f4
	bl Func_08014644
	movs r0, #114
	bl Audio_PlayCue
	mov r2, r11
	mov r4, r11
	ldr r1, [r2, #8]
	mov r0, r11
	ldr r2, [r2, #12]
	ldr r3, [r4, #16]
	bl Func_080dbf94
	ldr r0, [sp, #20]
	cmp r0, #96
	beq .L_080e70c2
	bl Func_08014274
.L_080e70c2:
	ldr r1, [sp, #12]
	cmp r1, #96
	beq .L_080e70ce
	adds r0, r1, #0
	bl Func_08014274
.L_080e70ce:
	mov r0, r11
	bl Func_080200c8
	ldr r0, [sp, #24]
	bl Func_080200c8
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e70f4:
	.4byte Func_080e6400
