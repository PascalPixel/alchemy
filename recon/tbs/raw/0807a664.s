.syntax unified
	.thumb
	.global Func_0807a664
	.thumb_func
Func_0807a664:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0807a6f8
	ldr r2, .L_0807a6fc
	ldrh r3, [r0]
	sub sp, #4
	mov r8, r0
	cmp r3, r2
	bne .L_0807a682
	b .L_0807a780
.L_0807a682:
	mov r1, r8
	ldr r3, .L_0807a700
	movs r0, #136
	strh r2, [r1]
	lsls r0, r0, #2
	movs r2, #2
	add r8, r2
	adds r2, r3, r0
	movs r0, #0
	ldrsh r1, [r2, r0]
	mov r9, r1
	ldr r1, .L_0807a704
	adds r3, r3, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r1, #0
	mov r11, r2
	mov r10, r1
.L_0807a6a6:
	mov r0, r10
	bl Owner_GetState
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #216
	movs r5, #14
.L_0807a6b4:
	ldrh r3, [r2]
	mov r0, r8
	movs r1, #2
	subs r5, #1
	adds r2, #2
	strh r3, [r0]
	add r8, r1
	cmp r5, #0
	bge .L_0807a6b4
	ldr r2, .L_0807a6f4
	movs r6, #216
	movs r5, #14
.L_0807a6cc:
	ldrh r0, [r6, r7]
	str r2, [sp, #0]
	bl Item_GetDirect
	ldrb r3, [r0, #2]
	ldr r2, [sp, #0]
	cmp r3, #6
	beq .L_0807a6de
	strh r2, [r6, r7]
.L_0807a6de:
	subs r5, #1
	adds r6, #2
	cmp r5, #0
	bge .L_0807a6cc
	adds r0, r7, #0
	adds r0, #216
	movs r5, #0
	adds r4, r0, #0
	adds r1, r0, #0
	movs r6, #14
	b .L_0807a708
.L_0807a6f4:
	.4byte 0x00000000
.L_0807a6f8:
	.4byte gInventorySnapshot
.L_0807a6fc:
	.4byte 0x00006774
.L_0807a700:
	.4byte gCell
.L_0807a704:
	.4byte 0x00000222
.L_0807a708:
	ldrh r2, [r4]
	lsls r3, r2, #16
	adds r4, #2
	cmp r3, #0
	beq .L_0807a718
	strh r2, [r1]
	adds r5, #1
	adds r1, #2
.L_0807a718:
	subs r6, #1
	cmp r6, #0
	bge .L_0807a708
	cmp r5, #14
	bgt .L_0807a73c
	lsls r3, r5, #1
	adds r0, r3, r0
	ldr r2, .L_0807a738
	movs r3, #15
	subs r5, r3, r5
.L_0807a72c:
	subs r5, #1
	strh r2, [r0]
	adds r0, #2
	cmp r5, #0
	bne .L_0807a72c
	b .L_0807a73c
.L_0807a738:
	.4byte 0x00000000
.L_0807a73c:
	mov r0, r10
	bl Owner_RefreshDerivedData
	mov r0, r10
	bl Owner_RecalculateStats
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #3
	ble .L_0807a6a6
	movs r2, #2
	mov r1, r8
	mov r0, r9
	add r8, r2
	strh r0, [r1]
	mov r3, r11
	mov r0, r8
	strh r3, [r0]
	ldr r0, .L_0807a798
	add r8, r2
	ldrh r3, [r0]
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r0, #2]
	mov r2, r8
	movs r0, #0
	strh r3, [r2, #2]
	movs r1, #16
	bl Inventory_AddAndEquip
	ldr r0, .L_0807a79c
	bl GameFlag_SetBit
.L_0807a780:
	movs r0, #1
	bl Owner_RefreshActiveRatios
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0807a798:
	.4byte gItemCounters + 0xb8
.L_0807a79c:
	.4byte 0x00000952
