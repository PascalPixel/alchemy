.syntax unified
	.thumb
	.global Unnamed_080b56e0
	.thumb_func
Unnamed_080b56e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #0
	mov r10, r2
	bl GameState_InitDefaultsFar
.L_080b56f6:
	movs r5, #181
	lsls r5, r5, #1
	bl Ui_LoadWindowGraphics
	bl Bg0_ClearTilemap
	bl Scheduler_ResetTaskTable
	bl Runtime_InitializeHeap
	bl Resource_InitializeTable
	adds r0, r5, #0
	bl GameFlag_SetBitFar
	ldr r3, .L_080b5850
	ldr r3, [r3]
	movs r2, #128
	ands r3, r2
	ldr r6, .L_080b5854
	cmp r3, #0
	bne .L_080b5724
	b .L_080b583e
.L_080b5724:
	movs r3, #1
	negs r3, r3
	adds r0, r5, #0
	mov r8, r3
	bl GameFlag_ClearBitFar
	ldr r2, .L_080b5858
	movs r3, #85
	mov r9, r2
	negs r3, r3
	add r3, r9
	ldr r5, .L_080b585c
	movs r7, #0
	mov r11, r3
.L_080b5740:
	movs r0, #32
	bl GameFlag_ClearBitFar
	movs r0, #1
	bl WaitFrames
	b .L_080b579a
.L_080b574e:
	ldr r3, [r5]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080b575c
	bl Unnamed_080b5534
.L_080b575c:
	ldr r3, [r5]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080b576a
	bl Battle_ReservedNoOp2A08
.L_080b576a:
	ldr r3, [r5]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_080b577a
	mov r2, r10
	cmp r2, #0
	beq .L_080b5784
.L_080b577a:
	movs r3, #1
	mov r10, r3
	movs r3, #5
	mov r2, r9
	strb r3, [r2]
.L_080b5784:
	cmp r7, r8
	beq .L_080b5794
	bl GameState_InitDefaultsFar
	adds r0, r7, #0
	bl DebugParty_LoadPreset
	mov r8, r7
.L_080b5794:
	movs r0, #1
	bl WaitFrames
.L_080b579a:
	ldr r3, [r5]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080b57a6
	adds r6, #1
.L_080b57a6:
	ldr r3, [r5]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080b57b2
	subs r6, #1
.L_080b57b2:
	ldr r3, [r5]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080b57be
	subs r6, #10
.L_080b57be:
	ldr r3, [r5]
	movs r1, #128
	ands r3, r1
	cmp r3, #0
	beq .L_080b57ca
	adds r6, #10
.L_080b57ca:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080b57d8
	adds r7, #1
.L_080b57d8:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080b57e6
	subs r7, #1
.L_080b57e6:
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080b574e
	ldr r3, .L_080b5850
	ldr r3, [r3]
	ands r3, r1
	cmp r3, #0
	beq .L_080b5802
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
.L_080b5802:
	movs r0, #0
	bl Owner_RecalculateStatsFar
	ldr r3, .L_080b5860
	mov r2, r11
	strh r3, [r2]
	cmp r6, #28
	bne .L_080b581a
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
.L_080b581a:
	movs r0, #177
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
	adds r0, r6, #0
	bl Battle_RunEncounter
	bl Ui_LoadWindowGraphics
	bl Bg0_ClearTilemap
	bl Scheduler_ResetTaskTable
	bl Runtime_InitializeHeap
	bl Resource_InitializeTable
	b .L_080b5740
.L_080b583e:
	movs r0, #177
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
	ldr r0, .L_080b5854
	bl Battle_RunEncounter
	b .L_080b56f6
	.2byte 0x0000
.L_080b5850:
	.4byte gKeysHeld
.L_080b5854:
	.4byte 0x00000101
.L_080b5858:
	.4byte gItemCounters + 0xeb
.L_080b585c:
	.4byte gKeysRepeat
.L_080b5860:
	.4byte 0x0000001d
