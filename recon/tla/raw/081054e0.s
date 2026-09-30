.syntax unified
	.thumb
	.global Func_081054e0
	.thumb_func
Func_081054e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r3, [sp, #4]
	adds r6, r2, #0
	movs r2, #12
	ldrsh r3, [r0, r2]
	mov r10, r1
	lsls r3, r3, #3
	add r3, r10
	adds r3, #8
	str r3, [sp, #0]
	movs r1, #0
	mov r11, r1
	movs r1, #14
	ldrsh r3, [r0, r1]
	movs r2, #16
	lsls r3, r3, #3
	adds r3, r3, r6
	adds r7, r3, #0
	negs r2, r2
	mov r8, r0
	adds r7, #8
	movs r5, #0
	mov r9, r2
.L_0810551c:
	ldr r1, [sp, #4]
	ldrb r3, [r1, r5]
	cmp r3, #0
	beq .L_08105548
	ldr r1, [sp, #0]
	adds r2, r7, #0
	adds r0, r5, #0
	bl Func_08105498
	ldr r0, .L_08105578
	mov r2, r10
	adds r2, #16
	adds r3, r6, #0
	adds r0, r5, r0
	mov r1, r8
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #1
	adds r6, #16
	adds r7, #16
	add r11, r2
	b .L_08105552
.L_08105548:
	adds r0, r5, #0
	mov r1, r9
	mov r2, r9
	bl Func_08105498
.L_08105552:
	adds r5, #1
	cmp r5, #4
	ble .L_0810551c
	mov r3, r11
	cmp r3, #0
	bne .L_0810556a
	ldr r0, .L_0810557c
	mov r1, r8
	mov r2, r10
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_0810556a:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08105578:
	.4byte 0x00001106
.L_0810557c:
	.4byte 0x00001105
