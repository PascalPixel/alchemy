.syntax unified
	.thumb
	.global Func_0810a2d8
	.thumb_func
Func_0810a2d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r1, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	adds r7, r0, #0
	str r3, [sp, #8]
	bl Owner_GetState
	ldr r2, [sp, #12]
	mov r8, r0
	lsls r2, r2, #1
	str r2, [sp, #4]
	adds r6, r2, #0
	adds r6, #216
	ldrh r3, [r0, r6]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	mov r10, r2
	mov r0, r10
	bl Item_Get
	adds r5, r0, #0
	ldrb r1, [r5, #2]
	adds r0, r7, #0
	bl Inventory_FindEquippedFar
	str r0, [sp, #0]
	mov r3, r8
	ldrh r0, [r3, r6]
	bl Func_0810a2ac
	ldrb r1, [r5, #12]
	mov r9, r0
	cmp r1, #2
	beq .L_0810a344
	mov r0, r10
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_0810a478
	bl Func_0810857c
	b .L_0810a468
.L_0810a344:
	mov r3, r8
	ldrh r2, [r3, r6]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	bne .L_0810a362
	mov r0, r10
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_0810a47c
	bl Func_0810857c
	b .L_0810a468
.L_0810a362:
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810a384
	ldrb r3, [r5, #3]
	ands r3, r1
	cmp r3, #0
	beq .L_0810a384
	mov r0, r10
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r0, .L_0810a480
	bl Func_0810857c
	b .L_0810a468
.L_0810a384:
	ldr r3, .L_0810a484
	ldr r3, [r3, #16]
	cmp r9, r3
	bls .L_0810a394
	ldr r0, .L_0810a488
	bl Func_0810857c
	b .L_0810a468
.L_0810a394:
	mov r0, r10
	movs r1, #2
	bl UiText_DrawQuantity
	mov r0, r9
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r2, .L_0810a48c
	mov r11, r2
	mov r0, r11
	bl Func_0810857c
	movs r0, #0
	bl Func_08108630
	cmp r0, #0
	beq .L_0810a3c2
	mov r0, r11
	adds r0, #1
	bl Func_0810857c
	b .L_0810a468
.L_0810a3c2:
	ldr r5, [sp, #4]
	mov r2, r8
	adds r5, #216
	mov r3, r8
	ldrh r6, [r3, r5]
	strh r0, [r2, r5]
	ldr r3, [sp, #8]
	adds r1, r7, #0
	ldr r0, [r3, #36]
	bl Func_0810a004
	movs r1, #2
	mov r0, r10
	bl UiText_DrawQuantity
	mov r0, r11
	adds r0, #2
	bl Func_0810857c
	bl UiWork_FinalizePendingCoreFar
	movs r0, #10
	bl WaitFrames
	movs r0, #100
	bl Audio_PlayCue
	movs r0, #110
	bl WaitFrames
	movs r0, #100
	bl Audio_PlayCue
	movs r0, #110
	bl WaitFrames
	movs r0, #100
	bl Audio_PlayCue
	movs r0, #110
	bl WaitFrames
	movs r0, #112
	bl Audio_PlayCue
	movs r0, #20
	bl WaitFrames
	mov r2, r8
	strh r6, [r2, r5]
	ldr r1, [sp, #12]
	adds r0, r7, #0
	bl Inventory_RepairFar
	mov r3, r9
	negs r0, r3
	bl Party_AdjustSixDigitCounterAFar
	bl Func_08109188
	ldr r2, [sp, #8]
	adds r1, r7, #0
	ldr r0, [r2, #36]
	bl Func_0810a004
	movs r1, #2
	mov r0, r10
	bl UiText_DrawQuantity
	mov r0, r11
	adds r0, #3
	bl Func_0810857c
	adds r0, r7, #0
	ldr r1, [sp, #12]
	bl Func_0810993c
	cmp r0, #0
	beq .L_0810a468
	adds r0, r7, #0
	ldr r1, [sp, #0]
	bl Func_08109a3c
.L_0810a468:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810a478:
	.4byte 0x0000126b
.L_0810a47c:
	.4byte 0x0000126c
.L_0810a480:
	.4byte 0x0000126d
.L_0810a484:
	.4byte gPartyState
.L_0810a488:
	.4byte 0x0000126e
.L_0810a48c:
	.4byte 0x0000126f
