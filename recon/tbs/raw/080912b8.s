.syntax unified
	.thumb
	.global Object_EffectSpawnCallback
	.thumb_func
Object_EffectSpawnCallback:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08091398
	ldr r1, [r3]
	subs r3, #112
	ldr r3, [r3]
	adds r3, #228
	mov r11, r1
	movs r1, #2
	ldrsh r2, [r3, r1]
	sub sp, #16
	str r2, [sp, #8]
	movs r1, #6
	ldrsh r2, [r3, r1]
	str r2, [sp, #4]
	mov r5, r11
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080912ea
	b .L_08091480
.L_080912ea:
	ldr r2, [r3, #16]
	mov r10, r2
	movs r2, #22
	ldrsh r1, [r3, r2]
	ldr r7, [r3, #8]
	str r1, [sp, #0]
	adds r3, #34
	ldrb r3, [r3]
	movs r1, #189
	mov r8, r3
	ldr r3, .L_0809139c
	lsls r1, r1, #1
	ldr r2, .L_080913a0
	adds r3, r3, r1
	adds r7, r7, r2
	ldrh r3, [r3]
	movs r2, #128
	lsls r2, r2, #13
	lsrs r3, r3, #5
	add r2, r10
	adds r1, r7, #0
	mov r0, r8
	mov r9, r3
	bl Func_080091a8
	movs r2, #128
	lsls r2, r2, #14
	asrs r6, r0, #16
	add r2, r10
	mov r0, r8
	adds r1, r7, #0
	bl Func_080091a8
	asrs r0, r0, #16
	subs r0, #16
	cmp r0, r6
	ble .L_08091336
	adds r6, r0, #0
.L_08091336:
	cmp r6, #0
	ble .L_080913c8
	ldr r3, [sp, #0]
	cmp r6, r3
	ble .L_080913c8
	ldr r3, .L_080913a4
	movs r1, #13
	str r3, [r5, #4]
	movs r3, #128
	ldrb r2, [r5, #9]
	lsls r3, r3, #3
	negs r1, r1
	str r3, [r5, #8]
	adds r3, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	ldr r3, .L_0809138c
	mov r2, r9
	ands r2, r3
	ldrh r0, [r5, #8]
	ldr r3, .L_080913a8
	ands r3, r0
	orrs r3, r2
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	ands r1, r3
	movs r3, #4
	orrs r1, r3
	strb r1, [r5, #5]
	ldr r3, .L_08091390
	ldr r1, [sp, #8]
	asrs r2, r7, #16
	ands r2, r3
	ldr r3, .L_08091394
	subs r2, r2, r1
	ands r2, r3
	ldrh r1, [r5, #6]
	ldr r3, .L_080913ac
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	mov r2, r10
	b .L_080913b0
.L_0809138c:
	.4byte 0x000003ff
.L_08091390:
	.4byte 0x0000fff0
.L_08091394:
	.4byte 0x000001ff
.L_08091398:
	.4byte gActorEffectWork
.L_0809139c:
	.4byte ResourceTableEntries
.L_080913a0:
	.4byte 0xfff80000
.L_080913a4:
	.4byte 0x40000800
.L_080913a8:
	.4byte 0xfffffc00
.L_080913ac:
	.4byte 0xfffffe00
.L_080913b0:
	ldr r1, [sp, #4]
	asrs r3, r2, #16
	movs r2, #240
	ands r3, r2
	subs r3, r3, r1
	subs r3, r3, r6
	adds r3, #16
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #0
	bl Runtime_PushSlotEntry
.L_080913c8:
	movs r2, #128
	lsls r2, r2, #13
	adds r7, r7, r2
	adds r1, r7, #0
	add r2, r10
	mov r0, r8
	bl Func_080091a8
	movs r2, #128
	lsls r2, r2, #14
	asrs r6, r0, #16
	add r2, r10
	mov r0, r8
	adds r1, r7, #0
	bl Func_080091a8
	asrs r0, r0, #16
	mov r5, r11
	subs r0, #16
	adds r5, #12
	cmp r0, r6
	ble .L_080913f6
	adds r6, r0, #0
.L_080913f6:
	cmp r6, #0
	ble .L_08091480
	ldr r2, [sp, #0]
	cmp r6, r2
	ble .L_08091480
	ldr r3, .L_08091458
	movs r2, #13
	ldrb r1, [r5, #9]
	str r3, [r5, #4]
	negs r2, r2
	movs r3, #0
	str r3, [r5, #8]
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r5, #9]
	ldr r3, .L_0809144c
	mov r1, r9
	ands r1, r3
	mov r9, r1
	ldr r3, .L_0809145c
	ldrh r1, [r5, #8]
	ands r3, r1
	mov r1, r9
	orrs r3, r1
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldr r3, .L_08091450
	strb r2, [r5, #5]
	asrs r2, r7, #16
	ands r2, r3
	ldr r3, [sp, #8]
	subs r2, r2, r3
	ldr r3, .L_08091454
	ldrh r1, [r5, #6]
	ands r2, r3
	ldr r3, .L_08091460
	ands r3, r1
	orrs r3, r2
	b .L_08091464
	.2byte 0x0000
.L_0809144c:
	.4byte 0x000003ff
.L_08091450:
	.4byte 0x0000fff0
.L_08091454:
	.4byte 0x000001ff
.L_08091458:
	.4byte 0x40000800
.L_0809145c:
	.4byte 0xfffffc00
.L_08091460:
	.4byte 0xfffffe00
.L_08091464:
	mov r1, r10
	strh r3, [r5, #6]
	movs r2, #240
	asrs r3, r1, #16
	ands r3, r2
	ldr r2, [sp, #4]
	subs r3, r3, r2
	subs r3, r3, r6
	adds r3, #16
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #0
	bl Runtime_PushSlotEntry
.L_08091480:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
