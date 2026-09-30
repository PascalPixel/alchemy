.syntax unified
	.thumb
	.global Func_080cb1dc
	.thumb_func
Func_080cb1dc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r0, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #8]
	ldr r3, .L_080cb2b4
	ldr r0, [r3, #44]
	mov lr, r0
	.2byte 0xf800
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080cb2a4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	movs r1, #0
	ldrsh r6, [r5, r1]
	cmp r6, r2
	beq .L_080cb2a4
.L_080cb214:
	movs r1, #2
	ldrsh r3, [r5, r1]
	mov r8, r3
	movs r3, #4
	ldrsh r2, [r5, r3]
	movs r3, #12
	ldrsh r0, [r5, r3]
	mov r11, r2
	movs r2, #6
	ldrsh r1, [r5, r2]
	movs r3, #8
	ldrsh r7, [r5, r3]
	mov r10, r1
	movs r2, #10
	ldrsh r1, [r5, r2]
	mov r9, r1
	movs r2, #14
	ldrsh r1, [r5, r2]
	str r1, [sp, #4]
	bl GameFlag_IsConditionActive
	cmp r0, #0
	beq .L_080cb28e
	ldr r2, [sp, #12]
	mov r1, r8
	lsls r3, r1, #16
	cmp r2, r3
	blt .L_080cb28e
	lsls r3, r7, #16
	cmp r2, r3
	bge .L_080cb28e
	ldr r1, [sp, #16]
	lsls r3, r6, #16
	cmp r1, r3
	blt .L_080cb28e
	mov r2, r10
	lsls r3, r2, #16
	cmp r1, r3
	bge .L_080cb28e
	ldr r2, [sp, #8]
	mov r1, r11
	lsls r3, r1, #16
	cmp r2, r3
	blt .L_080cb28e
	mov r1, r9
	lsls r3, r1, #16
	cmp r2, r3
	bge .L_080cb28e
	ldr r2, [sp, #0]
	movs r1, #172
	lsls r1, r1, #1
	adds r3, r2, r1
	add r2, sp, #4
	ldrh r2, [r2]
	movs r0, #123
	strh r2, [r3]
	bl Audio_PlayCue
	bl Func_080d2260
	b .L_080cb2a4
.L_080cb28e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, #16
	str r3, [sp, #0]
	movs r1, #1
	movs r3, #0
	ldrsh r6, [r5, r3]
	negs r1, r1
	cmp r6, r1
	bne .L_080cb214
.L_080cb2a4:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cb2b4:
	.4byte gOverlayArea
