.syntax unified
	.thumb
	.global Func_080d9104
	.thumb_func
Func_080d9104:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	adds r7, r1, #0
	mov r8, r2
	mov r11, r0
	movs r2, #2
	adds r0, r7, #0
	sub sp, #12
	mov r10, r3
	mov r9, r2
	bl ObjectTable_Get
	adds r5, r0, #0
	mov r0, r8
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r5, #0
	beq .L_080d9186
	cmp r6, #0
	beq .L_080d9186
	adds r3, r5, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d9154
	ldr r3, [r5, #80]
	ldrb r3, [r3, #9]
	lsls r3, r3, #28
	lsrs r3, r3, #30
	mov r9, r3
.L_080d9154:
	mov r2, r11
	lsls r3, r2, #5
	add r3, r10
	adds r3, #12
	mov r2, r8
	strh r2, [r3, #2]
	mov r2, r9
	strh r7, [r3]
	strh r2, [r3, #6]
	movs r4, #128
	ldr r0, [r6, #8]
	ldr r2, [r5, #12]
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	str r0, [sp, #0]
	lsls r4, r4, #12
	ldr r0, [r6, #12]
	adds r2, r2, r4
	adds r0, r0, r4
	str r0, [sp, #4]
	ldr r0, [r6, #16]
	str r0, [sp, #8]
	mov r0, r11
	bl Func_080d9194
.L_080d9186:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
