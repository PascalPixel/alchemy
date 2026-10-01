.syntax unified
	.thumb
	.global Func_0803fd28
	.thumb_func
Func_0803fd28:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	sub sp, #8
	movs r5, #2
	mov r9, r3
	movs r1, #5
	movs r2, #27
	movs r3, #14
	movs r0, #1
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r1, #0
	movs r2, #2
	movs r3, #26
	str r5, [sp, #0]
	mov r11, r0
	bl UiWindow_DrawDividerLine
	movs r3, #4
	str r3, [sp, #0]
	mov r0, r11
	movs r1, #0
	movs r2, #4
	movs r3, #26
	bl UiWindow_DrawDividerLine
	movs r3, #7
	str r3, [sp, #0]
	mov r0, r11
	movs r1, #0
	movs r2, #7
	movs r3, #26
	bl UiWindow_DrawDividerLine
	movs r3, #10
	str r3, [sp, #0]
	mov r0, r11
	movs r1, #0
	movs r2, #10
	movs r3, #26
	bl UiWindow_DrawDividerLine
	ldr r5, .L_080400b8
	mov r1, r11
	adds r0, r5, #0
	movs r2, #8
	movs r3, #0
	adds r5, #1
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #0
	mov r1, r11
	movs r2, #8
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	ldr r5, .L_080400bc
	mov r1, r11
	adds r0, r5, #0
	movs r2, #8
	movs r3, #32
	adds r5, #1
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #0
	mov r1, r11
	movs r2, #32
	movs r3, #40
	bl UiText_DrawCharacterAtOffset
	ldr r0, .L_080400c0
	mov r1, r11
	movs r2, #8
	movs r3, #64
	bl UiText_DrawCharacterAtOffset
	mov r1, r11
	movs r2, #8
	movs r3, #88
	ldr r0, .L_080400c4
	bl UiText_DrawCharacterAtOffset
	bl Func_08044460
	movs r1, #0
	str r1, [sp, #0]
	movs r1, #128
	mov r2, r11
	lsls r1, r1, #23
	movs r3, #0
	bl RenderOutput_Create
	adds r4, r0, #0
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r9
	str r4, [r0]
	mov r1, r11
	movs r2, #12
	ldrsh r3, [r1, r2]
	lsls r6, r3, #3
	movs r2, #14
	ldrsh r3, [r1, r2]
	adds r1, r6, #0
	lsls r3, r3, #3
	adds r7, r3, #0
	adds r7, #12
	adds r2, r7, #0
	bl Func_08108048
	bl Resource_FindFreeEntry
	adds r6, r0, #0
	cmp r6, #95
	bgt .L_0803feec
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080400c8
	ldr r1, .L_080400cc
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, .L_080400d0
	adds r0, r6, #0
	bl VramBlock_LoadCached
	ldr r3, .L_080400d4
	movs r7, #0
	mov r8, r3
	mov r1, r8
	mov r2, r11
	movs r3, #134
	adds r0, r6, #0
	str r7, [sp, #0]
	bl RenderOutput_Create
	adds r4, r0, #0
	ldrb r2, [r4, #25]
	movs r5, #15
	movs r1, #224
	mov r10, r1
	adds r3, r5, #0
	ands r3, r2
	mov r2, r10
	orrs r3, r2
	strb r3, [r4, #25]
	mov r1, r8
	mov r2, r11
	movs r3, #166
	adds r0, r6, #0
	str r7, [sp, #0]
	bl RenderOutput_Create
	adds r4, r0, #0
	ldrh r1, [r4, #24]
	movs r3, #192
	lsls r2, r1, #22
	lsrs r2, r2, #22
	lsls r3, r3, #2
	adds r3, #255
	adds r2, #4
	ands r2, r3
	ldr r3, .L_080400d8
	movs r7, #16
	ands r3, r1
	orrs r3, r2
	strh r3, [r4, #24]
	mov r1, r10
	ldrb r3, [r4, #25]
	mov r2, r11
	ands r5, r3
	orrs r5, r1
	strb r5, [r4, #25]
	mov r1, r8
	movs r3, #134
	adds r0, r6, #0
	str r7, [sp, #0]
	bl RenderOutput_Create
	adds r4, r0, #0
	ldrb r3, [r4, #25]
	movs r5, #240
	orrs r3, r5
	strb r3, [r4, #25]
	mov r1, r8
	mov r2, r11
	movs r3, #166
	adds r0, r6, #0
	str r7, [sp, #0]
	bl RenderOutput_Create
	adds r4, r0, #0
	ldrh r2, [r4, #24]
	movs r1, #192
	lsls r3, r2, #22
	lsrs r3, r3, #22
	lsls r1, r1, #2
	adds r1, #255
	adds r3, #4
	ands r3, r1
	ldr r1, .L_080400d8
	ands r1, r2
	orrs r1, r3
	str r1, [sp, #4]
	add r2, sp, #4
	ldrh r2, [r2]
	strh r2, [r4, #24]
	ldrb r3, [r4, #25]
	orrs r3, r5
	strb r3, [r4, #25]
.L_0803feec:
	bl Resource_FindFreeEntry
	adds r6, r0, #0
	cmp r6, #95
	bgt .L_0803ff6a
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	bl VramBlock_LoadCached
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	adds r0, r6, #0
	mov r2, r11
	str r3, [sp, #0]
	bl RenderOutput_Create
	adds r4, r0, #0
	ldrb r3, [r4, #21]
	movs r5, #160
	lsls r5, r5, #3
	movs r2, #32
	adds r5, #180
	orrs r3, r2
	add r5, r9
	strb r3, [r4, #21]
	str r4, [r5]
	mov r2, r11
	movs r1, #12
	ldrsh r3, [r2, r1]
	lsls r3, r3, #3
	adds r6, r3, #0
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #148
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r6, #140
	lsls r0, r3, #4
	subs r0, r0, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #153
	add r3, r9
	movs r1, #0
	ldrsb r1, [r3, r1]
	lsls r0, r0, #2
	bl __divsi3
	mov r2, r11
	movs r1, #14
	ldrsh r3, [r2, r1]
	adds r6, r6, r0
	lsls r3, r3, #3
	adds r7, r3, #4
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_08108048
.L_0803ff6a:
	bl Resource_FindFreeEntry
	adds r6, r0, #0
	cmp r6, #95
	bgt .L_0803ffea
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	bl VramBlock_LoadCached
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	adds r0, r6, #0
	mov r2, r11
	str r3, [sp, #0]
	bl RenderOutput_Create
	adds r4, r0, #0
	ldrb r3, [r4, #21]
	movs r5, #160
	lsls r5, r5, #3
	movs r2, #32
	adds r5, #196
	orrs r3, r2
	add r5, r9
	strb r3, [r4, #21]
	str r4, [r5]
	mov r2, r11
	movs r1, #12
	ldrsh r3, [r2, r1]
	lsls r3, r3, #3
	adds r6, r3, #0
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #149
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r6, #140
	lsls r0, r3, #4
	subs r0, r0, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #154
	add r3, r9
	movs r1, #0
	ldrsb r1, [r3, r1]
	lsls r0, r0, #2
	bl __divsi3
	mov r2, r11
	movs r1, #14
	ldrsh r3, [r2, r1]
	adds r6, r6, r0
	lsls r3, r3, #3
	adds r7, r3, #0
	adds r7, #20
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_08108048
.L_0803ffea:
	ldr r5, .L_080400dc
	movs r7, #28
	movs r0, #0
	ldrsb r0, [r5, r0]
	mov r2, r11
	movs r1, #0
	movs r3, #84
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #236
	add r3, r9
	str r0, [r3]
	mov r2, r11
	movs r0, #1
	ldrsb r0, [r5, r0]
	movs r1, #0
	movs r3, #108
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #190
	lsls r3, r3, #3
	add r3, r9
	str r0, [r3]
	mov r2, r11
	movs r0, #2
	ldrsb r0, [r5, r0]
	movs r1, #0
	movs r3, #132
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #160
	ldr r5, .L_080400e0
	lsls r3, r3, #3
	adds r3, #244
	add r3, r9
	str r0, [r3]
	movs r7, #52
	movs r0, #0
	ldrsb r0, [r5, r0]
	mov r2, r11
	movs r1, #0
	movs r3, #100
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #191
	lsls r3, r3, #3
	add r3, r9
	str r0, [r3]
	mov r2, r11
	movs r0, #1
	ldrsb r0, [r5, r0]
	movs r1, #0
	movs r3, #124
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #160
	ldr r5, .L_080400e4
	lsls r3, r3, #3
	adds r3, #252
	add r3, r9
	str r0, [r3]
	movs r7, #76
	movs r0, #0
	ldrsb r0, [r5, r0]
	mov r2, r11
	movs r1, #0
	movs r3, #100
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	add r3, r9
	str r0, [r3]
	movs r1, #0
	movs r0, #1
	ldrsb r0, [r5, r0]
	mov r2, r11
	movs r3, #124
	str r7, [sp, #0]
	bl RenderResource_CreateFrame
	movs r3, #193
	lsls r3, r3, #3
	add r3, r9
	str r0, [r3]
	add sp, #8
	mov r0, r11
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080400b8:
	.4byte 0x00001138
.L_080400bc:
	.4byte 0x0000113e
.L_080400c0:
	.4byte 0x00001140
.L_080400c4:
	.4byte 0x00001143
.L_080400c8:
	.4byte Data_080aa0e8
.L_080400cc:
	.4byte 0x050003c0
.L_080400d0:
	.4byte Data_0804e584
.L_080400d4:
	.4byte 0x40004000
.L_080400d8:
	.4byte 0xfffffc00
.L_080400dc:
	.4byte Data_0805ea7c
.L_080400e0:
	.4byte Data_0805ea7f
.L_080400e4:
	.4byte Data_0805ea81
