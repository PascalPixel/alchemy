.syntax unified
	.thumb
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	mov r10, r3
	ldr r3, [sp, #52]
	mov r8, r2
	ldr r2, [sp, #48]
	lsls r3, r3, #7
	adds r3, r3, r2
	ldr r4, .L_0802b444
	ldr r2, .L_0802b448
	lsls r1, r1, #7
	adds r5, r3, r2
	mov r9, r4
	adds r1, r1, r0
	lsls r3, r3, #2
	adds r2, r1, r2
	add r3, r9
	lsls r1, r1, #2
	mov r7, r10
	str r5, [sp, #12]
	str r2, [sp, #8]
	mov r11, r3
	add r9, r1
	cmp r7, #0
	ble .L_0802b436
	movs r1, #0
	str r1, [sp, #0]
.L_0802b3ce:
	ldr r2, [sp, #0]
	mov r5, r11
	lsrs r3, r2, #16
	lsls r2, r3, #9
	adds r4, r5, r2
	add r2, r9
	str r2, [sp, #4]
	ldr r7, [sp, #12]
	ldr r2, [sp, #8]
	lsls r3, r3, #7
	adds r0, r7, r3
	adds r1, r2, r3
	mov r3, r8
	cmp r3, #0
	ble .L_0802b426
	movs r5, #240
	ldr r7, .L_0802b44c
	lsls r5, r5, #4
	adds r5, #255
	movs r6, #0
	mov lr, r5
	mov r12, r7
.L_0802b3fa:
	ldr r7, [sp, #4]
	ldr r2, [r4]
	mov r3, lr
	ands r2, r3
	ldmia r7!, {r3}
	adds r5, r7, #0
	str r5, [sp, #4]
	mov r5, r12
	ands r3, r5
	orrs r2, r3
	stmia r4!, {r2}
	movs r7, #128
	ldrb r3, [r1]
	lsls r7, r7, #9
	strb r3, [r0]
	adds r3, r6, r7
	adds r6, r3, #0
	lsrs r3, r6, #16
	adds r0, #1
	adds r1, #1
	cmp r3, r8
	blt .L_0802b3fa
.L_0802b426:
	ldr r1, [sp, #0]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r1, r2
	str r3, [sp, #0]
	lsrs r3, r3, #16
	cmp r3, r10
	blt .L_0802b3ce
.L_0802b436:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802b444:
	.4byte gMapCellBuffer
.L_0802b448:
	.4byte gMapShapeGrid
.L_0802b44c:
	.4byte 0xfffff000
