.syntax unified
	.thumb
	.global Func_0812786c
	.thumb_func
Func_0812786c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	movs r0, #36
	mov r10, r1
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	ldr r0, [sp, #8]
	bl Owner_GetState
	adds r7, r0, #0
	adds r6, r7, #0
	adds r6, #16
	movs r2, #36
	ldr r3, .L_08127a08
	adds r1, r6, #0
	mov r0, r9
	mov lr, r3
	.2byte 0xf800
	mov r1, r10
	lsls r3, r1, #1
	add r3, r10
	movs r0, #0
	ldrsh r5, [r6, r0]
	lsls r0, r3, #5
	movs r1, #10
	add r0, r10
	mov r11, r3
	bl __divsi3
	adds r5, r5, r0
	mov r0, r9
	movs r2, #0
	ldrsh r3, [r0, r2]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_081278ce
	adds r5, r0, #0
.L_081278ce:
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	cmp r5, r3
	ble .L_081278da
	adds r5, r3, #0
.L_081278da:
	strh r5, [r6]
	mov r2, r10
	lsls r2, r2, #4
	mov r3, r10
	movs r1, #18
	ldrsh r5, [r7, r1]
	subs r0, r2, r3
	movs r1, #10
	str r2, [sp, #4]
	bl __divsi3
	mov r1, r9
	adds r5, r5, r0
	movs r0, #2
	ldrsh r3, [r1, r0]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_08127908
	adds r5, r0, #0
.L_08127908:
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #15
	cmp r5, r3
	ble .L_08127914
	adds r5, r3, #0
.L_08127914:
	mov r2, r10
	mov r3, r10
	lsls r2, r2, #5
	subs r0, r2, r3
	lsls r0, r0, #2
	strh r5, [r7, #18]
	subs r0, r0, r3
	movs r1, #10
	mov r8, r2
	bl __divsi3
	ldrh r5, [r7, #24]
	movs r1, #10
	adds r5, r5, r0
	mov r0, r9
	ldrh r3, [r0, #8]
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_08127942
	adds r5, r0, #0
.L_08127942:
	movs r6, #186
	lsls r6, r6, #2
	adds r6, #255
	cmp r5, r6
	ble .L_0812794e
	adds r5, r6, #0
.L_0812794e:
	mov r0, r8
	strh r5, [r7, #24]
	movs r1, #10
	add r0, r10
	bl __divsi3
	mov r1, r9
	ldrh r3, [r1, #10]
	ldrh r5, [r7, #26]
	movs r1, #10
	adds r5, r5, r0
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_08127972
	adds r5, r0, #0
.L_08127972:
	cmp r5, r6
	ble .L_08127978
	adds r5, r6, #0
.L_08127978:
	mov r2, r11
	lsls r0, r2, #4
	strh r5, [r7, #26]
	movs r1, #10
	add r0, r11
	bl __divsi3
	ldrh r5, [r7, #28]
	movs r1, #10
	adds r5, r5, r0
	mov r0, r9
	ldrh r3, [r0, #12]
	lsls r0, r3, #3
	subs r0, r0, r3
	bl __divsi3
	cmp r5, r0
	bge .L_0812799e
	adds r5, r0, #0
.L_0812799e:
	cmp r5, r6
	ble .L_081279a4
	adds r5, r6, #0
.L_081279a4:
	strh r5, [r7, #28]
	movs r1, #20
	mov r8, r1
	movs r6, #36
	movs r4, #3
.L_081279ae:
	ldr r0, [sp, #4]
	ldrsh r2, [r6, r7]
	mov r1, r10
	subs r3, r0, r1
	adds r5, r2, r3
	mov r1, r9
	mov r2, r8
	ldrsh r3, [r2, r1]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	str r4, [sp, #0]
	bl __divsi3
	ldr r4, [sp, #0]
	cmp r5, r0
	bge .L_081279d2
	adds r5, r0, #0
.L_081279d2:
	cmp r5, #200
	ble .L_081279d8
	movs r5, #200
.L_081279d8:
	movs r2, #4
	subs r4, #1
	strh r5, [r6, r7]
	add r8, r2
	adds r6, #4
	cmp r4, #0
	bge .L_081279ae
	ldrb r3, [r7, #15]
	add r3, r10
	strb r3, [r7, #15]
	ldr r0, [sp, #8]
	bl BattleUnit_Recalculate
	mov r0, r9
	bl Sys_Free
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08127a08:
	.4byte IwramCopyWords
