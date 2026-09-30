.syntax unified
	.thumb
	.global Func_0810a490
	.thumb_func
Func_0810a490:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	sub sp, #8
	movs r3, #0
	mov r10, r0
	movs r0, #128
	str r3, [sp, #4]
	lsls r0, r0, #3
	adds r0, #252
	adds r7, r6, r0
	mov r9, r1
	ldrh r1, [r7]
	movs r4, #158
	str r1, [sp, #0]
	ldr r1, .L_0810a64c
	lsls r4, r4, #1
	mov r8, r3
	adds r3, r1, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, .L_0810a650
	lsls r3, r3, #1
	ldrsh r0, [r2, r3]
	mov r11, r0
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r1, r0
	ldr r3, [r3]
	cmp r11, r3
	ble .L_0810a4e2
	b .L_0810a63e
.L_0810a4e2:
	movs r3, #228
	strh r3, [r7]
	movs r1, #2
	movs r0, #228
	bl UiText_DrawQuantity
	ldr r5, .L_0810a654
	adds r0, r5, #0
	bl Func_0810857c
	adds r5, #1
	ldrh r0, [r7]
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r5, #0
	bl Func_0810857c
	movs r7, #2
.L_0810a508:
	cmp r7, #0
	beq .L_0810a57e
	ldr r4, [sp, #4]
	movs r1, #153
	lsls r1, r1, #3
	lsls r2, r4, #1
	adds r3, r6, #2
	adds r2, r2, r1
	ldrsh r0, [r3, r2]
	adds r3, r4, #0
	mov r8, r0
	cmp r4, #0
	bge .L_0810a524
	adds r3, r4, #3
.L_0810a524:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	movs r2, #0
	subs r1, #12
	mov r0, r10
	bl Func_08108af0
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #5
	adds r2, r6, r3
	movs r3, #3
	strb r3, [r2]
	cmp r7, #2
	bne .L_0810a560
	ldr r0, [sp, #4]
	cmp r0, #0
	bge .L_0810a552
	adds r0, #3
.L_0810a552:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_080f8058
	movs r0, #1
	bl WaitFrames
.L_0810a560:
	movs r4, #128
	lsls r4, r4, #3
	adds r4, #252
	adds r5, r6, r4
	ldr r1, [sp, #4]
	ldrh r2, [r5]
	mov r0, r10
	bl Func_0810928c
	ldrh r2, [r5]
	mov r0, r9
	mov r1, r8
	bl Func_081095b0
	movs r7, #0
.L_0810a57e:
	ldr r1, .L_0810a658
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_0810a5fa
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #252
	adds r5, r6, r0
	ldrh r1, [r5]
	mov r0, r8
	bl Inventory_AddItemFar
	adds r1, r0, #0
	cmp r1, #0
	bge .L_0810a5d0
	movs r0, #113
	bl Audio_PlayCue
	mov r0, r8
	movs r1, #1
	bl UiText_DrawQuantity
	ldrh r0, [r5]
	movs r1, #2
	bl UiText_DrawQuantity
	mov r0, r8
	bl Item_AdjustCounterFar + 0x8
	cmp r0, #15
	bne .L_0810a5c8
	ldr r0, .L_0810a65c
	bl Func_081084f4
	b .L_0810a508
.L_0810a5c8:
	ldr r0, .L_0810a660
	bl Func_081084f4
	b .L_0810a508
.L_0810a5d0:
	mov r0, r8
	bl Inventory_RemoveFar
	movs r0, #101
	bl Audio_PlayCue
	ldr r0, .L_0810a664
	bl Func_0810857c
	ldrh r1, [r5]
	mov r0, r8
	bl Inventory_AddItemFar
	mov r1, r11
	negs r0, r1
	bl Djinn_AddToLeastLoadedOwnerFar + 0x10
	movs r0, #1
	bl Djinn_AddToLeastLoadedOwnerFar + 0x18
	b .L_0810a630
.L_0810a5fa:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810a612
	ldr r0, .L_0810a668
	bl Func_0810857c
	movs r0, #113
	bl Audio_PlayCue
	b .L_0810a630
.L_0810a612:
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #4
	adds r3, r6, r2
	movs r1, #0
	ldrsb r1, [r3, r1]
	add r0, sp, #4
	movs r2, #4
	bl Func_08108690
	adds r7, r0, #0
	movs r0, #1
	bl WaitFrames
	b .L_0810a508
.L_0810a630:
	movs r4, #128
	mov r0, sp
	lsls r4, r4, #3
	ldrh r0, [r0]
	adds r4, #252
	adds r3, r6, r4
	strh r0, [r3]
.L_0810a63e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0810a64c:
	.4byte gPartyState
.L_0810a650:
	.4byte Data_0810c38e
.L_0810a654:
	.4byte 0x00001274
.L_0810a658:
	.4byte gInput
.L_0810a65c:
	.4byte 0x0000124f
.L_0810a660:
	.4byte 0x00001257
.L_0810a664:
	.4byte 0x00001252
.L_0810a668:
	.4byte 0x00001276
