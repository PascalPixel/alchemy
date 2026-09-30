.syntax unified
	.thumb
	.global Func_08043358
	.thumb_func
Func_08043358:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	ldr r5, .L_080434d4
	sub sp, #28
	mov r8, r0
	bl Func_080ad2c8
	ldr r2, .L_080434d8
	ldr r3, .L_080434dc
	str r0, [r2]
	movs r0, #128
	ldr r1, [r3]
	ldr r3, .L_080434e0
	str r1, [r2, #4]
	str r1, [r3]
	ldr r3, .L_080434e4
	lsls r0, r0, #2
	ldrb r1, [r3]
	adds r0, #74
	adds r3, r2, r0
	strb r1, [r3]
	movs r1, #133
	lsls r1, r1, #2
	adds r2, r2, r1
	ldr r0, [r2]
	bl Owner_GetState
	adds r7, r5, #0
	adds r6, r0, #0
	adds r1, r5, #0
	subs r7, #16
	adds r2, r6, #0
	movs r5, #11
.L_0804339e:
	ldrb r3, [r2]
	subs r5, #1
	strb r3, [r1]
	adds r2, #1
	adds r1, #1
	cmp r5, #0
	bge .L_0804339e
	ldrb r3, [r6, #15]
	ldr r5, .L_080434d8
	strb r3, [r7, #28]
	movs r2, #240
	ldr r3, [r5, #4]
	lsls r2, r2, #1
	str r3, [r7, #32]
	adds r3, r5, r2
	adds r2, #2
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_080c8648
	strh r0, [r7, #30]
	movs r0, #42
	adds r0, #255
	adds r3, r6, r0
	ldrb r3, [r3]
	movs r0, #0
	strb r3, [r7, #29]
	ldr r3, [r5, #16]
	movs r5, #0
	str r3, [r7, #36]
	bl Djinn_AddToLeastLoadedOwnerFar + 0x8
	adds r3, r7, #0
	adds r3, #40
	strb r0, [r3]
	movs r0, #1
	bl Djinn_AddToLeastLoadedOwnerFar + 0x8
	adds r3, r7, #0
	adds r3, #41
	strb r0, [r3]
	movs r0, #2
	bl Djinn_AddToLeastLoadedOwnerFar + 0x8
	adds r3, r7, #0
	adds r3, #42
	strb r0, [r3]
	movs r0, #3
	bl Djinn_AddToLeastLoadedOwnerFar + 0x8
	adds r3, r7, #0
	adds r3, #43
	mov r6, sp
	strb r0, [r3]
	adds r0, r6, #0
	bl Party_ListActiveOwnersFar
	ldrh r3, [r6, r5]
	cmp r3, #255
	beq .L_08043438
	adds r1, r7, #0
	adds r0, r6, #0
	adds r1, #44
	movs r2, #0
.L_08043424:
	ldrh r3, [r2, r0]
	adds r5, #1
	strb r3, [r1]
	adds r2, #2
	adds r1, #1
	cmp r5, #3
	bgt .L_08043438
	ldrh r3, [r2, r6]
	cmp r3, #255
	bne .L_08043424
.L_08043438:
	movs r1, #1
	adds r2, r5, #0
	negs r1, r1
	adds r3, r1, #0
	adds r2, #44
	strb r3, [r7, r2]
	movs r0, #147
	ldr r2, .L_080434d8
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r2, r0
	ldrb r1, [r3]
	adds r3, r7, #0
	adds r3, #52
	strb r1, [r3]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #38
	adds r3, r2, r1
	ldrb r3, [r3]
	adds r1, r7, #0
	adds r1, #53
	strb r3, [r1]
	movs r3, #152
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
	ldrb r3, [r2]
	adds r2, r7, #0
	adds r2, #49
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #50
	movs r0, #0
	strb r0, [r3]
	movs r5, #48
	adds r6, r3, #0
.L_08043482:
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08043492
	ldrb r3, [r6]
	adds r3, #1
	strb r3, [r6]
.L_08043492:
	adds r5, #1
	cmp r5, #127
	ble .L_08043482
	movs r0, #34
	bl GameFlag_Test
	negs r3, r0
	orrs r3, r0
	adds r2, r7, #0
	lsrs r3, r3, #31
	adds r2, #51
	strb r3, [r2]
	ldr r3, .L_080434d8
	ldr r2, .L_080434e8
	ldr r3, [r3]
	movs r1, #242
	movs r5, #0
	lsls r1, r1, #2
	strh r3, [r7, #54]
	b .L_080434c0
.L_080434ba:
	ldmia r2!, {r3}
	adds r5, #1
	add r8, r3
.L_080434c0:
	cmp r5, r1
	blt .L_080434ba
	mov r0, r8
	str r0, [r7, #60]
	add sp, #28
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080434d4:
	.4byte Data_02000000
.L_080434d8:
	.4byte gPartyState
.L_080434dc:
	.4byte Data_0300117c
.L_080434e0:
	.4byte Data_02001000
.L_080434e4:
	.4byte Data_03001200
.L_080434e8:
	.4byte GameFlagBytes
