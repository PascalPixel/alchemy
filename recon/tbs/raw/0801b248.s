.syntax unified
	.thumb
	.global Menu_SetupSelectionSide
	.thumb_func
Menu_SetupSelectionSide:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #52
	muls r3, r1
	adds r6, r0, #0
	adds r2, r6, r3
	movs r0, #0
	adds r2, #40
	mov r11, r0
	adds r3, #8
	mov r10, r2
	adds r4, r6, r3
	mov r2, r11
	strh r2, [r4, #2]
	cmp r1, #0
	beq .L_0801b2ac
	movs r0, #229
	lsls r0, r0, #2
	adds r3, r6, r0
	ldrh r2, [r3]
	movs r3, #231
	lsls r3, r3, #2
	adds r0, r6, r3
	ldrh r3, [r0]
	ldr r5, .L_0801b35c
	cmp r3, #0
	beq .L_0801b28a
	subs r2, r2, r3
.L_0801b28a:
	cmp r2, #5
	bls .L_0801b294
	movs r3, #1
	strh r3, [r4, #2]
	movs r2, #5
.L_0801b294:
	ldr r4, .L_0801b360
	adds r3, r6, r4
	ldrh r3, [r3]
	subs r2, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	adds r2, r6, #0
	adds r3, #17
	adds r2, #68
	mov r11, r5
	strh r3, [r2]
	b .L_0801b2cc
.L_0801b2ac:
	ldr r2, .L_0801b360
	adds r3, r6, r2
	ldr r0, .L_0801b364
	ldrh r3, [r3]
	ldr r4, .L_0801b368
	mov r11, r0
	adds r3, r3, r4
	movs r0, #231
	strh r3, [r6, #16]
	lsls r0, r0, #2
	adds r3, r6, r0
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0801b2cc
	movs r3, #1
	strh r3, [r6, #10]
.L_0801b2cc:
	movs r3, #52
	adds r7, r1, #0
	muls r7, r3
	adds r3, r7, #0
	adds r3, #16
	adds r3, r3, r6
	movs r4, #2
	ldrsh r2, [r3, r4]
	mov r8, r3
	mov r9, r2
	cmp r2, #0
	bne .L_0801b34c
	bl Resource_FindFreeEntry
	adds r5, r7, #0
	adds r5, #12
	strh r0, [r6, r5]
	movs r1, #128
	ldrh r0, [r6, r5]
	mov r2, r11
	bl VramBlock_LoadCached
	adds r5, r6, r5
	strh r0, [r5, #2]
	movs r0, #230
	lsls r0, r0, #2
	adds r3, r6, r0
	ldrh r3, [r3]
	mov r2, r8
	strh r3, [r2, #2]
	adds r3, r7, #0
	adds r3, #8
	mov r4, r9
	strh r4, [r6, r3]
	mov r0, r10
	ldrb r3, [r0, #5]
	movs r0, #13
	negs r0, r0
	adds r2, r0, #0
	ands r2, r3
	movs r3, #17
	negs r3, r3
	ands r2, r3
	movs r3, #32
	orrs r2, r3
	movs r3, #4
	negs r3, r3
	ands r2, r3
	mov r3, r10
	ldrb r1, [r3, #7]
	movs r3, #63
	negs r3, r3
	ands r3, r1
	movs r1, #63
	mov r4, r10
	ands r3, r1
	strb r3, [r4, #7]
	ands r2, r1
	movs r3, #128
	orrs r2, r3
	ldrb r3, [r4, #9]
	ands r0, r3
	strb r2, [r4, #5]
	strb r0, [r4, #9]
.L_0801b34c:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0801b35c:
	.4byte Menu_CursorObjectTiles
.L_0801b360:
	.4byte 0x00000396
.L_0801b364:
	.4byte Menu_CursorLeftObjectTiles
.L_0801b368:
	.4byte 0x0000fff7
