.syntax unified
	.thumb
	.global Func_081263fc
	.thumb_func
Func_081263fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	mov r11, r0
	adds r0, r1, #0
	adds r7, r2, #0
	mov r9, r3
	bl Resource_GetTableEntry
	ldr r5, [r5, #36]
	mov r8, r0
	mov r10, r5
	ldr r5, .L_08126520
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08126524
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #1
	ldr r1, .L_08126528
	add r0, r8
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	movs r4, #160
	lsls r4, r4, #3
	adds r4, #108
	movs r3, #128
	movs r2, #132
	add r4, r10
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r0, r8
	adds r1, r4, #0
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	cmp r7, #0
	blt .L_081264a0
	lsls r3, r7, #4
	adds r3, r3, r7
	movs r0, #192
	lsls r3, r3, #4
	lsls r0, r0, #3
	adds r3, r3, r7
	movs r2, #128
	lsls r3, r3, #2
	movs r1, #160
	adds r0, #108
	lsls r2, r2, #9
	add r0, r10
	subs r2, r2, r3
	lsls r1, r1, #19
	str r2, [r0]
	adds r1, #192
	adds r0, r4, #0
	movs r3, #128
	bl ColorBuffer_Scale
.L_081264a0:
	movs r3, #237
	lsls r3, r3, #3
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_081264d2
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0812652c
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08126530
	movs r3, #160
	ldrh r2, [r2]
	lsls r3, r3, #19
	adds r3, #188
	strh r2, [r3]
.L_081264d2:
	ldr r0, .L_08126534
	bl Graphics_BuildSequentialTileTable
	ldr r0, .L_08126538
	bl Func_08125bb8
	ldr r3, .L_0812653c
	ldr r0, .L_08126540
	movs r1, #64
	mov lr, r3
	.2byte 0xf800
	mov r2, r9
	ldr r3, [r2, #8]
	cmp r3, #0
	bne .L_081264fc
	movs r1, #192
	lsls r1, r1, #4
	ldr r0, .L_08126544
	adds r1, #255
	bl Scheduler_AddOrUpdateCallback
.L_081264fc:
	mov r3, r11
	mov r2, r9
	str r3, [r2, #8]
	cmp r3, #1
	bne .L_08126510
	movs r2, #128
	ldr r3, .L_0812651c
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
.L_08126510:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0812651c:
	.4byte 0x00001f83
.L_08126520:
	.4byte 0x00000234
.L_08126524:
	.4byte Model_DecodeCode
.L_08126528:
	.4byte 0x06008000
.L_0812652c:
	.4byte 0x05000200
.L_08126530:
	.4byte 0x050001e8
.L_08126534:
	.4byte 0x06003800
.L_08126538:
	.4byte 0x0600f800
.L_0812653c:
	.4byte IwramClearWords
.L_08126540:
	.4byte 0x0600ffc0
.L_08126544:
	.4byte Func_08125c0c
