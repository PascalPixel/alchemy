.syntax unified
	.thumb
	.global Func_080fbe6c
	.thumb_func
Func_080fbe6c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	str r2, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r1, #128
	lsls r1, r1, #3
	mov r11, r0
	movs r0, #56
	mov r9, r3
	bl Runtime_AllocateBlock
	movs r2, #1
	movs r3, #0
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r3, r9
	adds r3, #240
	ldr r7, [r3]
	mov r10, r0
	bl Func_080fbe24
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	ldr r2, [sp, #20]
	mov r8, r11
	cmp r2, #0
	bne .L_080fbed8
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	ands r1, r3
	bl Func_080fad48
	str r0, [sp, #8]
.L_080fbed8:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	ands r1, r3
	bl Func_080fad48
	str r0, [sp, #4]
	bl Resource_FindFreeEntry
	str r0, [sp, #16]
	cmp r0, #96
	bne .L_080fbf04
	b .L_080fc0e6
.L_080fbf04:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	bl VramBlock_LoadCached
	ldr r6, .L_080fbf50
	movs r5, #32
	ldr r0, [sp, #16]
	adds r1, r6, #0
	adds r2, r7, #0
	movs r3, #48
	str r5, [sp, #0]
	bl RenderOutput_CreateFar
	adds r1, r6, #0
	adds r2, r7, #0
	ldr r0, [sp, #16]
	movs r3, #80
	str r5, [sp, #0]
	bl RenderOutput_CreateFar
	ldrh r1, [r0, #24]
	ldr r3, .L_080fbf4c
	lsls r2, r1, #22
	lsrs r2, r2, #22
	adds r2, #4
	ands r2, r3
	ldr r3, .L_080fbf54
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #24]
	movs r1, #40
	movs r0, #128
	bl Func_080f8ab4
	b .L_080fc0d8
.L_080fbf4c:
	.4byte 0x000003ff
.L_080fbf50:
	.4byte 0x40004000
.L_080fbf54:
	.4byte 0xfffffc00
.L_080fbf58:
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_080fbf60
	b .L_080fc070
.L_080fbf60:
	ldr r0, [sp, #24]
	ldr r1, [sp, #24]
	movs r2, #0
	add r0, r8
	str r2, [sp, #12]
	bl Math_Mod
	mov r8, r0
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r1, r7, #0
	ldr r0, .L_080fc01c
	movs r2, #32
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080fc020
	mov r1, r10
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r10
	movs r0, #30
	movs r1, #14
	bl Func_08108048 + 0x8
	ldr r0, [sp, #24]
	movs r1, #0
	add r0, r11
	mov r2, r10
	bl Func_08108048 + 0x8
	mov r0, r11
	add r0, r8
	adds r0, #1
	movs r1, #10
	mov r2, r10
	bl Func_08108048 + 0x8
	mov r0, r11
	movs r1, #2
	mov r2, r10
	bl Func_08108048 + 0x8
	movs r1, #128
	ldr r0, [sp, #16]
	lsls r1, r1, #1
	mov r2, r10
	bl VramBlock_LoadCached
	mov r0, r8
	movs r3, #32
	adds r0, #1
	movs r1, #2
	adds r2, r7, #0
	str r3, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	ldr r0, .L_080fc018
	adds r1, r7, #0
	ands r0, r3
	ldr r3, .L_080fc024
	movs r2, #16
	adds r0, r0, r3
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, [sp, #4]
	mov r2, r8
	subs r0, r3, r2
	subs r0, #1
	movs r3, #16
	movs r5, #24
	movs r1, #2
	adds r2, r7, #0
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r3, [sp, #20]
	cmp r3, #0
	bne .L_080fc03a
	b .L_080fc028
.L_080fc018:
	.4byte 0x000001ff
.L_080fc01c:
	.4byte 0x0000100d
.L_080fc020:
	.4byte Data_08105838
.L_080fc024:
	.4byte 0x0000025f
.L_080fc028:
	ldr r0, [sp, #8]
	movs r1, #2
	add r0, r8
	adds r0, #1
	adds r2, r7, #0
	movs r3, #80
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
.L_080fc03a:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r9
	ldrb r0, [r3]
	bl Owner_GetState
	movs r2, #16
	adds r1, r7, #0
	movs r3, #16
	bl UiText_DrawStringAtOffsetFar
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_080fc070
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r9
	ldrb r0, [r3]
	bl Owner_GetState
	adds r1, r7, #0
	movs r2, #80
	movs r3, #16
	bl UiText_DrawStringAtOffsetFar
.L_080fc070:
	ldr r5, .L_080fc128
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080fc084
	movs r0, #112
	bl Audio_PlayCue
	b .L_080fc0e6
.L_080fc084:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fc09c
	movs r3, #1
	negs r3, r3
	movs r0, #113
	mov r8, r3
	bl Audio_PlayCue
	b .L_080fc0e6
.L_080fc09c:
	movs r0, #128
	movs r1, #40
	bl Func_080f8a44
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080fc0bc
	subs r2, #33
	movs r3, #1
	movs r0, #111
	add r8, r2
	str r3, [sp, #12]
	bl Audio_PlayCue
.L_080fc0bc:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080fc0d2
	movs r2, #1
	movs r0, #111
	add r8, r2
	str r2, [sp, #12]
	bl Audio_PlayCue
.L_080fc0d2:
	movs r0, #1
	bl WaitFrames
.L_080fc0d8:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fc0e6
	b .L_080fbf58
.L_080fc0e6:
	adds r0, r7, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r0, r7, #0
	bl RenderOutput_ClearListFar
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r3, #134
	lsls r3, r3, #2
	add r3, r9
	ldr r2, [r3]
	movs r0, #168
	movs r3, #13
	strb r3, [r2, #5]
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fc116
	movs r3, #1
	negs r3, r3
	mov r8, r3
.L_080fc116:
	mov r0, r8
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fc128:
	.4byte gInput
