.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/ENTRY.INC"
	.global SceneData_GetTable93c8
	.thumb_func
SceneData_GetTable93c8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020093c8
	.global SceneData_ReturnZero
	.thumb_func
SceneData_ReturnZero:
	movs r0, #0
	bx lr
	.global SceneData_GetTable93f8
	.thumb_func
SceneData_GetTable93f8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020093f8
	.global SceneData_GetTable93fc
	.thumb_func
SceneData_GetTable93fc:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020093fc
	.global SceneDialogue_ShowMessageAndWait
	.thumb_func
SceneDialogue_ShowMessageAndWait:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020091f0
	adds r0, r5, #0
	movs r1, #5
	movs r2, #0
	movs r3, #34
	bl 0x020091a8
	b .L_0200004c_0
.L_0200004c_1:
	movs r0, #1
	bl 0x02009190
.L_0200004c_0:
	bl 0x020091b8
	cmp r0, #0
	beq .L_0200004c_1
	movs r0, #1
	bl 0x02009190
	pop {r5}
	pop {r0}
	bx r0
	.global CommandTable_RunDirectionalInput
	.thumb_func
CommandTable_RunDirectionalInput:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #156]
	movs r2, #131
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	adds r6, r0, #0
	mov r8, r1
	movs r0, #125
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl 0x020091e0
	movs r7, #0
	mov r10, r0
	cmp r7, r8
	bge .L_0200007c_0
	ldr r5, [pc, #124]
.L_0200007c_7:
	movs r0, #1
	movs r1, #1
	bl 0x020091e8
	movs r0, #141
	movs r1, #2
	bl 0x020091e8
	ldr r0, [pc, #112]
	movs r1, #5
	bl 0x020091e8
	adds r0, r6, #0
	bl 0x0200804c
	b .L_0200007c_2
.L_0200007c_6:
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0200007c_3
	movs r0, #1
	bl 0x02009190
.L_0200007c_2:
	ldr r3, [r5]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0200007c_0
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0200007c_4
	ldr r3, [r5]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0200007c_5
.L_0200007c_4:
	adds r6, #1
	b .L_0200007c_3
.L_0200007c_5:
	ldr r3, [r5]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_0200007c_6
	subs r6, #1
.L_0200007c_3:
	adds r7, #1
	cmp r7, r8
	blt .L_0200007c_7
.L_0200007c_0:
	bl 0x020091f0
	mov r0, r10
	movs r1, #2
.L_0200007c_1:
	bl 0x020091a0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001ae8
	.4byte 0x0001e240
	.global SceneState_ApplyBlockC9b
	.thumb_func
SceneState_ApplyBlockC9b:
	push {lr}
	ldr r0, [pc, #12]
	ldr r1, [pc, #12]
	subs r1, r1, r0
	bl 0x0200807c
	pop {r0}
	bx r0
	.4byte 0x00000c9b
	.4byte 0x00000cc6
	.global SceneState_ApplyBlockCc6
	.thumb_func
SceneState_ApplyBlockCc6:
	push {lr}
	ldr r0, [pc, #12]
	ldr r1, [pc, #12]
	subs r1, r0, r1
	bl 0x0200807c
	pop {r0}
	bx r0
	.4byte 0x00000cc6
	.4byte 0x00000c9b
	.global SceneState_ApplyBlockCf1
	.thumb_func
SceneState_ApplyBlockCf1:
	push {lr}
	ldr r3, [pc, #16]
	ldr r1, [pc, #16]
	ldr r0, [pc, #20]
	subs r1, r1, r3
	bl 0x0200807c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000c9b
	.4byte 0x00000cc6
	.4byte 0x00000cf1
	.global SceneState_ApplyBlockD21
	.thumb_func
SceneState_ApplyBlockD21:
	push {lr}
	ldr r0, [pc, #12]
	ldr r1, [pc, #12]
	subs r1, r1, r0
	bl 0x0200807c
	pop {r0}
	bx r0
	.4byte 0x00000d21
	.4byte 0x00000d4c
	.global SceneState_ApplyBlockD4c
	.thumb_func
SceneState_ApplyBlockD4c:
	push {lr}
	ldr r3, [pc, #16]
	ldr r1, [pc, #16]
	ldr r0, [pc, #20]
	subs r1, r1, r3
	bl 0x0200807c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000c9b
	.4byte 0x00000cc6
	.4byte 0x00000d4c
	.global SceneState_ApplyBlockD77
	.thumb_func
SceneState_ApplyBlockD77:
	push {lr}
	ldr r3, [pc, #16]
	ldr r1, [pc, #16]
	ldr r0, [pc, #20]
	subs r1, r1, r3
	bl 0x0200807c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000c9b
	.4byte 0x00000cc6
	.4byte 0x00000d77
	.global SceneState_ApplyBlockDa2
	.thumb_func
SceneState_ApplyBlockDa2:
	push {lr}
	ldr r3, [pc, #16]
	ldr r1, [pc, #16]
	ldr r0, [pc, #20]
	subs r1, r1, r3
	bl 0x0200807c
	pop {r0}
.L_020001e8:
	bx r0
	.2byte 0x0000
	.2byte 0x0c9b
	.2byte 0x0000
	.2byte 0x0cc6
	.2byte 0x0000
	.2byte 0x0da2
	.2byte 0x0000
	.global SceneState_ApplyOne
	.thumb_func
SceneState_ApplyOne:
	push {lr}
	movs r0, #1
	bl 0x02009290
	pop {r0}
	bx r0
	.global SceneState_NoOp
	.thumb_func
SceneState_NoOp:
	bx lr
	.2byte 0x0000
	.global SceneState_QueryTwoValues
	.thumb_func
SceneState_QueryTwoValues:
	push {lr}
	sub sp, #8
	mov r1, sp
	add r0, sp, #4
	bl 0x020092a0
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global SceneState_ApplyZero
	.thumb_func
SceneState_ApplyZero:
	push {lr}
	movs r0, #0
.L_02000220:
	bl 0x02009208
	pop {r0}
	bx r0
	.global CommandTable_NoOpCallback
	.thumb_func
CommandTable_NoOpCallback:
	bx lr
	.2byte 0x0000
	.global SceneState_SetRecordFlag53
	.thumb_func
SceneState_SetRecordFlag53:
	ldr r3, [pc, #8]
	ldr r3, [r3]
	movs r2, #1
	adds r3, #53
	strb r2, [r3]
	bx lr
	.4byte 0x03001f30
	.global SceneData_GetTable9564
	.thumb_func
SceneData_GetTable9564:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009564
	.global FieldScene_ApplyTable9684ValueToFourSlots
	.thumb_func
FieldScene_ApplyTable9684ValueToFourSlots:
	push {r5, lr}
	ldr r0, [pc, #76]
	movs r1, #1
	bl 0x020091b0
	ldr r5, [pc, #72]
	movs r0, #0
	ldr r1, [r5]
	bl 0x02009270
	ldr r1, [r5]
	movs r0, #1
	bl 0x02009270
.L_02000260:
	ldr r1, [r5]
	movs r0, #3
	bl 0x02009270
	ldr r1, [r5]
	movs r0, #2
	bl 0x02009270
	ldr r3, [r5]
	adds r3, #10
	str r3, [r5]
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0c1a
	.2byte 0x0000
	.2byte 0x9684
	.2byte 0x0200
	.global FieldScene_GrantItemListToSlots
	.thumb_func
FieldScene_GrantItemListToSlots:
	push {lr}
	ldr r0, [pc, #1020]
	movs r1, #1
	bl 0x020091b0
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #187
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #180
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #181
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #182
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #183
	movs r0, #0
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #186
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #187
	movs r0, #1
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #188
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	b .L_0200029c_0
	.2byte 0x0000
	.4byte 0x00000c1e
.L_0200029c_0:
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #236
	movs r0, #2
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #191
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #192
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #193
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #194
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #195
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r1, #196
	movs r0, #3
	bl 0x02009230
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	pop {r0}
	bx r0
	.2byte 0x0000
	.global CommandTable_ConfigureCommandGroups
	.thumb_func
CommandTable_ConfigureCommandGroups:
	push {lr}
	ldr r0, [pc, #580]
	movs r1, #1
	sub sp, #256
	bl 0x020091b0
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #1
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #2
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #3
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #4
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #5
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #6
	movs r0, #0
	bl 0x02009260
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x02009268
	movs r1, #0
	movs r2, #1
	movs r0, #0
	bl 0x02009268
	movs r1, #0
	movs r2, #2
	movs r0, #0
	bl 0x02009268
	movs r1, #0
	movs r2, #3
	movs r0, #0
	bl 0x02009268
	movs r1, #0
	movs r2, #4
	movs r0, #0
	bl 0x02009268
	movs r1, #0
	movs r2, #5
	movs r0, #0
	bl 0x02009268
	movs r1, #0
	movs r2, #6
	movs r0, #0
	bl 0x02009268
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #1
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #2
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #3
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #4
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #5
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #6
	movs r0, #1
	bl 0x02009260
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x02009268
	movs r1, #2
	movs r2, #1
	movs r0, #1
	bl 0x02009268
	movs r1, #2
	movs r2, #2
	movs r0, #1
	bl 0x02009268
	movs r1, #2
	movs r2, #3
	movs r0, #1
	bl 0x02009268
	movs r1, #2
	movs r2, #4
	movs r0, #1
	bl 0x02009268
	movs r1, #2
	movs r2, #5
	movs r0, #1
	bl 0x02009268
	movs r1, #2
	movs r2, #6
	movs r0, #1
	bl 0x02009268
	movs r1, #1
	movs r2, #0
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #1
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #2
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #3
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #4
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #5
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #6
	movs r0, #3
	bl 0x02009260
	movs r1, #1
	movs r2, #0
	movs r0, #3
	bl 0x02009268
	movs r1, #1
	movs r2, #1
	movs r0, #3
	bl 0x02009268
	movs r1, #1
	movs r2, #2
	movs r0, #3
	bl 0x02009268
	movs r1, #1
	movs r2, #3
	movs r0, #3
	bl 0x02009268
	movs r1, #1
	movs r2, #4
	movs r0, #3
	bl 0x02009268
	movs r1, #1
	movs r2, #5
	movs r0, #3
	bl 0x02009268
	movs r1, #1
	movs r2, #6
	movs r0, #3
	bl 0x02009268
	movs r1, #3
	movs r2, #0
	movs r0, #2
	bl 0x02009260
	movs r1, #3
	movs r2, #1
	movs r0, #2
	bl 0x02009260
	movs r1, #3
	movs r2, #2
	movs r0, #2
	bl 0x02009260
	movs r1, #3
	movs r2, #3
	movs r0, #2
	bl 0x02009260
	movs r1, #3
	movs r2, #4
	movs r0, #2
	bl 0x02009260
	movs r1, #3
	movs r2, #5
	movs r0, #2
	bl 0x02009260
	movs r1, #3
	movs r2, #0
	movs r0, #2
	bl 0x02009268
	movs r1, #3
	movs r2, #1
	movs r0, #2
	bl 0x02009268
	movs r1, #3
	movs r2, #2
	movs r0, #2
	bl 0x02009268
	movs r1, #3
	movs r2, #3
	movs r0, #2
	bl 0x02009268
	movs r1, #3
	movs r2, #4
	movs r0, #2
	bl 0x02009268
	movs r1, #3
	movs r2, #5
	movs r0, #2
	bl 0x02009268
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	sub sp, #-256
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000c1d
	.global FieldScene_ApplySlotOffsetsAndFlags
	.thumb_func
FieldScene_ApplySlotOffsetsAndFlags:
	push {r5, r6, lr}
	ldr r0, [pc, #156]
	movs r1, #1
	bl 0x020091b0
	movs r1, #100
	negs r1, r1
	movs r0, #0
	bl 0x02009240
	movs r1, #100
	negs r1, r1
	movs r0, #1
	bl 0x02009240
	movs r1, #33
	negs r1, r1
	movs r0, #2
	bl 0x02009240
	movs r1, #100
	negs r1, r1
	movs r0, #3
	bl 0x02009240
	movs r1, #50
	negs r1, r1
	movs r0, #0
	bl 0x02009248
	movs r1, #40
	negs r1, r1
	movs r0, #1
	bl 0x02009248
	movs r1, #35
	negs r1, r1
	movs r0, #2
	bl 0x02009248
	movs r1, #20
	negs r1, r1
	movs r0, #3
	bl 0x02009248
	movs r0, #0
	bl 0x02009218
	ldr r6, [pc, #64]
	movs r2, #160
	movs r5, #1
	lsls r2, r2, #1
	strb r5, [r0, r6]
	adds r0, r0, r2
	strb r5, [r0]
	movs r0, #1
	bl 0x02009218
	movs r2, #152
	lsls r2, r2, #1
	adds r3, r0, r2
	strb r5, [r3]
	movs r3, #2
	strb r3, [r0, r6]
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000c1b
	.4byte 0x00000131
	.global FieldScene_AssignCodeSetAToSlots
	.thumb_func
FieldScene_AssignCodeSetAToSlots:
	push {lr}
	ldr r0, [pc, #308]
	movs r1, #1
	bl 0x020091b0
	movs r1, #85
	movs r0, #0
	bl 0x02009230
	movs r1, #84
	movs r0, #0
	bl 0x02009230
	movs r1, #124
	movs r0, #0
	bl 0x02009230
	movs r1, #123
	movs r0, #0
	bl 0x02009230
	movs r1, #9
	movs r0, #0
	bl 0x02009230
	movs r1, #11
	movs r0, #0
	bl 0x02009230
	movs r1, #27
	movs r0, #0
	bl 0x02009230
	movs r1, #26
	movs r0, #0
	bl 0x02009230
	movs r1, #38
	movs r0, #1
	bl 0x02009230
	movs r1, #37
	movs r0, #1
	bl 0x02009230
	movs r1, #50
	movs r0, #1
	bl 0x02009230
	movs r1, #49
	movs r0, #1
	bl 0x02009230
	movs r1, #83
	movs r0, #1
	bl 0x02009230
	movs r1, #82
	movs r0, #1
	bl 0x02009230
	movs r1, #134
	movs r0, #1
	bl 0x02009230
	movs r1, #133
	movs r0, #1
	bl 0x02009230
	movs r1, #152
	movs r0, #1
	bl 0x02009230
	movs r1, #64
	movs r0, #2
	bl 0x02009230
	movs r1, #65
	movs r0, #2
	bl 0x02009230
	movs r1, #98
	movs r0, #2
	bl 0x02009230
	movs r1, #97
	movs r0, #2
	bl 0x02009230
	movs r1, #124
	movs r0, #2
	bl 0x02009230
	movs r1, #131
	movs r0, #2
	bl 0x02009230
	movs r1, #141
	movs r0, #2
	bl 0x02009230
	movs r1, #163
	movs r0, #2
	bl 0x02009230
	movs r1, #61
	movs r0, #3
	bl 0x02009230
	movs r1, #63
	movs r0, #3
	bl 0x02009230
	movs r1, #96
	movs r0, #3
	bl 0x02009230
	movs r1, #95
	movs r0, #3
	bl 0x02009230
	movs r1, #113
	movs r0, #3
	bl 0x02009230
	movs r1, #112
	movs r0, #3
	bl 0x02009230
	movs r1, #130
	movs r0, #3
	bl 0x02009230
	movs r1, #142
	movs r0, #3
	bl 0x02009230
	movs r1, #171
	movs r0, #3
	bl 0x02009230
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000c1f
	.global Func_02000cf4
	.thumb_func
Func_02000cf4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #112
	sub sp, #4
	bl 0x020092a8
	movs r5, #2
	movs r1, #0
	movs r2, #30
	movs r3, #7
	movs r0, #0
	str r5, [sp, #0]
	bl 0x02009198
	movs r1, #8
	adds r7, r0, #0
	movs r2, #13
	movs r3, #10
	movs r0, #0
	str r5, [sp, #0]
	bl 0x02009198
	movs r6, #1
	mov r10, r0
	mov r8, r6
	ldr r3, [pc, #420]
	ldr r0, [pc, #420]
	ldr r1, [pc, #424]
	ldr r2, [pc, #424]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r1, #28
	ldr r0, [pc, #420]
	ldr r2, [pc, #424]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl 0x02009190
.L_02000cf4_12:
	mov r3, r8
	cmp r3, #0
	beq .L_02000cf4_0
	movs r3, #0
	mov r8, r3
	movs r3, #135
	lsls r3, r3, #1
	movs r1, #135
	adds r0, r6, r3
	lsls r1, r1, #1
	bl 0x02009188
	adds r6, r0, #0
	adds r0, r7, #0
	bl 0x020091f8
	adds r0, r7, #0
	bl 0x02009200
	ldr r0, [pc, #376]
	adds r1, r7, #0
	movs r2, #0
	movs r3, #0
	bl 0x020091d0
	mov r3, r8
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r7, #0
	movs r3, #80
	bl 0x020091d8
	bl 0x02009280
	cmp r0, #0
	beq .L_02000cf4_1
	ldr r5, [pc, #344]
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	ands r5, r6
	ldr r0, [pc, #340]
	bl 0x020091d0
	adds r0, r5, #0
	bl 0x02009228
	ldr r0, [pc, #332]
	adds r1, r7, #0
	adds r0, r5, r0
	movs r2, #120
	movs r3, #0
	bl 0x020091c8
	ldr r3, [pc, #320]
	adds r5, r5, r3
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl 0x020091c0
	mov r0, r10
	bl 0x020091f8
	mov r0, r10
	adds r1, r6, #0
	bl 0x02009288
	b .L_02000cf4_0
.L_02000cf4_1:
	ldr r0, [pc, #292]
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	bl 0x020091d0
.L_02000cf4_0:
	ldr r5, [pc, #284]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_2
	adds r0, r6, #0
	bl 0x02009238
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02000cf4_3
	movs r0, #175
	bl 0x020092a8
.L_02000cf4_2:
	ldr r3, [r5]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_4
.L_02000cf4_3:
	movs r0, #113
	bl 0x020092a8
	b .L_02000cf4_5
.L_02000cf4_4:
	ldr r5, [pc, #240]
	ldr r3, [r5]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_6
	movs r3, #1
	movs r0, #111
	subs r6, #1
	mov r8, r3
	bl 0x020092a8
.L_02000cf4_6:
	ldr r3, [r5]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_7
	movs r3, #1
	movs r0, #111
	adds r6, #1
	mov r8, r3
	bl 0x020092a8
.L_02000cf4_7:
	ldr r3, [r5]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_8
	movs r3, #1
	movs r0, #111
	adds r6, #10
	mov r8, r3
	bl 0x020092a8
.L_02000cf4_8:
	ldr r3, [r5]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_9
	movs r3, #1
	movs r0, #111
	subs r6, #10
	mov r8, r3
	bl 0x020092a8
.L_02000cf4_9:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_10
	movs r3, #1
	movs r0, #111
	adds r6, #30
	mov r8, r3
	bl 0x020092a8
.L_02000cf4_10:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02000cf4_11
	movs r3, #1
	movs r0, #111
	subs r6, #30
	mov r8, r3
	bl 0x020092a8
.L_02000cf4_11:
	movs r0, #1
	bl 0x02009190
	b .L_02000cf4_12
.L_02000cf4_5:
	adds r0, r7, #0
	bl 0x020091f8
	movs r0, #1
	bl 0x02009190
	adds r0, r7, #0
	movs r1, #1
	bl 0x020091a0
	mov r0, r10
	movs r1, #1
	bl 0x020091a0
	sub sp, #-4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x80000001
	.4byte 0x02009390
	.4byte 0x000001ff
	.4byte 0x0200939c
	.4byte 0x00000182
	.4byte 0x00000075
	.4byte 0x020093b4
	.4byte 0x03001c94
	.4byte 0x03001b04
	.global SceneState_RunCall1c00
	.thumb_func
SceneState_RunCall1c00:
	push {lr}
	bl 0x02008cf4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global FieldScene_AssignCodeSetBToSlots
	.thumb_func
FieldScene_AssignCodeSetBToSlots:
	push {lr}
	ldr r0, [pc, #388]
	movs r1, #1
	bl 0x020091b0
	movs r1, #184
	movs r0, #0
	bl 0x02009230
	movs r1, #204
	movs r0, #0
	bl 0x02009230
	movs r1, #220
	movs r0, #0
	bl 0x02009230
	movs r1, #221
	movs r0, #0
	bl 0x02009230
	movs r1, #222
	movs r0, #0
	bl 0x02009230
	movs r1, #223
	movs r0, #0
	bl 0x02009230
	movs r1, #224
	movs r0, #0
	bl 0x02009230
	movs r1, #226
	movs r0, #1
	bl 0x02009230
	movs r1, #227
	movs r0, #1
	bl 0x02009230
	movs r1, #230
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #228
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #229
	movs r0, #1
	bl 0x02009230
	movs r1, #232
	movs r0, #1
	bl 0x02009230
	movs r1, #231
	movs r0, #1
	bl 0x02009230
	movs r1, #237
	movs r0, #1
	bl 0x02009230
	movs r1, #242
	movs r0, #2
	bl 0x02009230
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl 0x02009230
	ldr r1, [pc, #108]
	movs r0, #2
	bl 0x02009230
	ldr r1, [pc, #104]
	movs r0, #2
	bl 0x02009230
	movs r1, #252
	movs r0, #2
	bl 0x02009230
	movs r1, #189
	movs r0, #3
	bl 0x02009230
	movs r1, #200
	movs r0, #3
	bl 0x02009230
	movs r1, #201
	movs r0, #3
	bl 0x02009230
	movs r1, #202
	movs r0, #3
	bl 0x02009230
	movs r1, #203
	movs r0, #3
	bl 0x02009230
	movs r1, #204
	movs r0, #3
	bl 0x02009230
	movs r1, #207
	movs r0, #3
	bl 0x02009230
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	pop {r0}
	bx r0
	.4byte 0x00000c1c
	.4byte 0x0000010b
	.4byte 0x00000109
	.global CommandTable_ConfigureCommandList
	.thumb_func
CommandTable_ConfigureCommandList:
	push {lr}
	movs r0, #5
	bl 0x02009258
	movs r0, #1
	bl 0x02009250
	movs r0, #3
	bl 0x02009250
	movs r0, #2
	bl 0x02009250
	movs r1, #1
	movs r0, #5
	bl 0x02009278
	movs r1, #1
	movs r0, #5
	bl 0x02009278
	movs r1, #1
	movs r0, #5
	bl 0x02009278
	movs r1, #1
	movs r0, #6
	bl 0x02009278
	movs r1, #1
	movs r0, #6
	bl 0x02009278
	movs r1, #1
	movs r0, #7
	bl 0x02009278
	movs r1, #1
	movs r0, #106
	bl 0x02009278
	movs r1, #1
	movs r0, #108
	bl 0x02009278
	movs r1, #1
	movs r0, #109
	bl 0x02009278
	movs r1, #1
	movs r0, #113
	bl 0x02009278
	movs r1, #1
	movs r0, #123
	bl 0x02009278
	movs r1, #1
	movs r0, #130
	bl 0x02009278
	movs r1, #1
	movs r0, #140
	bl 0x02009278
	movs r1, #1
	movs r0, #151
	bl 0x02009278
	movs r0, #0
	movs r1, #50
	bl 0x02009270
	movs r0, #1
	movs r1, #30
	bl 0x02009270
	movs r0, #3
	movs r1, #30
	bl 0x02009270
	movs r1, #30
	movs r0, #2
	bl 0x02009270
	movs r0, #0
	bl 0x02009220
	movs r0, #1
	bl 0x02009220
	movs r0, #3
	bl 0x02009220
	movs r0, #2
	bl 0x02009220
	movs r0, #0
	pop {r1}
	bx r1
	.global SceneState_GetFarResult2384
	.thumb_func
SceneState_GetFarResult2384:
	push {lr}
	bl 0x02009210
	pop {r1}
	bx r1
	.2byte 0x0000
	.global SceneState_GetFarResult2418
	.thumb_func
SceneState_GetFarResult2418:
	push {lr}
	bl 0x02009298
	pop {r1}
	bx r1
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x00500050
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00500000
	.4byte 0x00010003
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00780078
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00780000
	.4byte 0x00010003
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x0028003c
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00010002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0028003c
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00010002
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00800080
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00020003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00800080
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00020003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00200020
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00800080
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00020003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.4byte 0xffff0000
	.4byte 0x0000003c
	.4byte 0xc000005e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x00002000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00002000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00002000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00002000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x00002000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00002000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00002000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00002000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x00002000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00002000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00002000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00002000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x00002000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x02008245
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x02008bb9
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x0200821d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008209
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020088c5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008b11
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008f15
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020081f9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200917d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000e5c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02009171
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte 0x02008131
	.4byte 0x00008400
	.4byte 0xffff0010
	.4byte 0x02008149
	.4byte 0x00000400
	.4byte 0xffff0010
	.4byte 0x02008161
	.4byte 0x00004400
	.4byte 0xffff0010
	.4byte 0x02008181
	.4byte 0x0000c400
	.4byte 0xffff0011
	.4byte 0x02008199
	.4byte 0x00008400
	.4byte 0xffff0011
	.4byte 0x020081b9
	.4byte 0x00000400
	.4byte 0xffff0011
	.4byte 0x020081d9
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte 0x02008229
	.4byte 0x00008e15
	.4byte 0xffff000f
	.4byte 0x02008229
	.4byte 0x00009415
	.4byte 0xffff000f
	.4byte 0x02008229
	.4byte 0x10008e15
	.4byte 0xffff000f
	.4byte 0x0200822d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000041
