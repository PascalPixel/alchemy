.syntax unified
	.thumb
	.global Func_080eace8
	.thumb_func
Func_080eace8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	adds r3, #224
	str r1, [sp, #4]
	ldr r0, [r3]
	cmp r0, #0
	beq .L_080ead1c
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080ead1c
	ldr r7, [r0, #20]
	b .L_080ead22
.L_080ead1c:
	bl Func_080eaeb4
	adds r7, r0, #0
.L_080ead22:
	ldr r2, [sp, #4]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r2, r1
	movs r2, #0
	str r2, [r3]
	ldr r2, [sp, #8]
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_080eade6
	adds r3, r7, #0
	adds r3, #34
	str r3, [sp, #0]
.L_080ead44:
	ldr r1, [sp, #8]
	ldrh r0, [r1]
	bl Object_GetById
	cmp r7, r0
	bne .L_080eadd4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	ldr r3, [sp, #0]
	movs r1, #156
	ldrb r0, [r3]
	lsls r1, r1, #1
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldr r1, .L_080eadf4
	mov r10, r3
	movs r3, #212
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r2, [r2]
	add r1, r10
	mov r9, r2
	ldr r2, .L_080eadf8
	asrs r1, r1, #2
	mov r8, r1
	add r8, r2
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_080202f0
	ldr r3, [sp, #0]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r11, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeightFar
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_080eab70
	ldr r1, [sp, #0]
	asrs r5, r5, #19
	subs r5, #4
	adds r6, r0, #0
	adds r2, r5, #0
	ldrb r0, [r1]
	mov r1, r11
	bl Func_080eab98
	add r8, r6
	ldr r3, [r7, #76]
	lsls r6, r6, #2
	add r10, r6
	mov r2, r8
	mov r1, r10
	add r9, r6
	strb r3, [r1, #3]
	strb r0, [r2]
	mov r3, r9
	movs r2, #0
	strb r2, [r3, #2]
	ldr r1, [sp, #4]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r1, r2
	str r7, [r3]
.L_080eadd4:
	ldr r3, [sp, #8]
	movs r1, #255
	adds r3, #2
	str r3, [sp, #8]
	lsls r1, r1, #8
	ldrh r3, [r3]
	adds r1, #255
	cmp r3, r1
	bne .L_080ead44
.L_080eade6:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080eadf4:
	.4byte 0xfdff0000
.L_080eadf8:
	.4byte gMapShapeGrid
