.syntax unified
	.thumb
	.global Game_ResetForNewGame
	.thumb_func
Game_ResetForNewGame:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080c9a00
	ldr r1, .L_080c9a04
	ldrb r3, [r3]
	mov r11, r1
	cmp r3, #0
	beq .L_080c99a6
	cmp r0, #1
	bne .L_080c9978
	ldr r1, .L_080c9a08
	ldr r3, .L_080c9a0c
	movs r0, #240
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #241
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #10
	b .L_080c99be
.L_080c9978:
	cmp r0, #2
	bne .L_080c9982
	ldr r1, .L_080c9a08
	ldr r3, .L_080c9a10
	b .L_080c99ae
.L_080c9982:
	cmp r0, #3
	bne .L_080c998c
	ldr r1, .L_080c9a08
	ldr r3, .L_080c9a14
	b .L_080c99ae
.L_080c998c:
	cmp r0, #4
	bne .L_080c99a6
	ldr r1, .L_080c9a08
	ldr r3, .L_080c9a18
	movs r0, #240
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #241
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #8
	b .L_080c99be
.L_080c99a6:
	bl Func_080ad090
	ldr r1, .L_080c9a08
	ldr r3, .L_080c9a18
.L_080c99ae:
	movs r0, #240
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #241
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #1
.L_080c99be:
	strh r3, [r2]
	ldr r3, .L_080c9a08
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #1
	lsls r1, r1, #2
	adds r0, #255
	adds r1, #38
	adds r2, r3, r0
	adds r3, r3, r1
	ldrb r0, [r2]
	ldrb r1, [r3]
	bl Func_08038348
	bl Func_08014368
	bl Scheduler_ResetTaskTable
	bl Scheduler_ResetTaskTable
	ldr r2, .L_080c9a1c
	mov r9, r2
.L_080c99ea:
	movs r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080c9a20
	movs r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_080c9a28
.L_080c9a00:
	.4byte gDebugMode
.L_080c9a04:
	.4byte Field_SceneTable
.L_080c9a08:
	.4byte gPartyState
.L_080c9a0c:
	.4byte 0x00000004
.L_080c9a10:
	.4byte 0x00000001
.L_080c9a14:
	.4byte 0x00000137
.L_080c9a18:
	.4byte 0x00000000
.L_080c9a1c:
	.4byte 0x050001c0
.L_080c9a20:
	movs r0, #144
	lsls r0, r0, #1
	bl Audio_PlayCue
.L_080c9a28:
	ldr r7, .L_080c9a90
	movs r3, #240
	lsls r3, r3, #1
	adds r3, r3, r7
	mov r8, r3
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r1, #241
	lsls r3, r3, #3
	add r3, r11
	lsls r1, r1, #1
	mov r10, r3
	adds r3, r7, r1
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #176
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldrh r3, [r1, #10]
	ldr r2, .L_080c9a88
	ands r3, r2
	strh r3, [r1, #10]
	ldr r2, .L_080c9a8c
	ldrh r3, [r1, #10]
	ands r3, r2
	strh r3, [r1, #10]
	ldr r2, .L_080c9a94
	ldrh r3, [r1, #10]
	movs r3, #1
	strb r3, [r2]
	bl Scheduler_ResetTaskTable
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	bl Func_08014c6c
	bl Func_08014b70
	b .L_080c9a98
	.2byte 0x0000
.L_080c9a88:
	.4byte 0x0000c5ff
.L_080c9a8c:
	.4byte 0x00007fff
.L_080c9a90:
	.4byte gPartyState
.L_080c9a94:
	.4byte gRenderOamEnabled
.L_080c9a98:
	bl Func_08014368
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	movs r2, #253
	lsls r2, r2, #1
	cmp r3, r2
	ble .L_080c9b82
	adds r2, #2
	cmp r3, r2
	beq .L_080c9afc
	cmp r3, r2
	bgt .L_080c9abe
	movs r0, #252
	adds r0, #255
	cmp r3, r0
	beq .L_080c9b3c
	b .L_080c9b7a
.L_080c9abe:
	movs r1, #254
	adds r1, #255
	cmp r3, r1
	beq .L_080c9ad8
	movs r2, #255
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_080c9b7a
	adds r0, r6, #0
	bl Func_081180a0
	adds r6, r0, #0
	b .L_080c9b7a
.L_080c9ad8:
	movs r0, #64
	bl Runtime_BumpAllocate
	movs r3, #128
	movs r2, #132
	adds r5, r0, #0
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r0, r9
	adds r1, r5, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Runtime_BlankDisplayAndRunFar
	b .L_080c9b1e
.L_080c9afc:
	movs r0, #64
	bl Runtime_BumpAllocate
	movs r3, #128
	movs r2, #132
	adds r5, r0, #0
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r0, r9
	adds r1, r5, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Resource_FarCall00E
.L_080c9b1e:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r6, r0, #0
	adds r3, #212
	adds r0, r5, #0
	mov r1, r9
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	b .L_080c9b7a
.L_080c9b3c:
	movs r0, #64
	bl Runtime_BumpAllocate
	movs r3, #128
	movs r2, #132
	adds r5, r0, #0
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r0, r9
	adds r1, r5, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Resource_FarCall00F
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r6, r0, #0
	adds r3, #212
	adds r0, r5, #0
	mov r1, r9
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
.L_080c9b7a:
	adds r0, r6, #0
	bl Func_080c977c
	b .L_080c99ea
.L_080c9b82:
	movs r5, #10
	adds r5, #255
	adds r0, r5, #0
	bl GameFlag_Test
	mov r2, r8
	adds r1, r0, #0
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Func_080c9c38
	bl Func_080ca1bc
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080c9bce
	movs r0, #141
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080c9bc4
	movs r0, #28
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080c9bc4
	bl Func_080c9dc8
	b .L_080c9bec
.L_080c9bc4:
	movs r0, #141
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_080c9bec
.L_080c9bce:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	adds r3, r7, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_080c9be8
	bl Audio_PlayCue
	b .L_080c9bec
.L_080c9be8:
	bl Func_080c9dc8
.L_080c9bec:
	ldr r3, .L_080c9c0c
	mov r0, r10
	ldrh r2, [r0, #4]
	movs r1, #253
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r2, [r3]
	movs r0, #0
	bl Func_080c9c10
	adds r0, r6, #0
	bl Func_080cb91c
	bl Func_080c9694
	b .L_080c99ea
.L_080c9c0c:
	.4byte gPartyState
