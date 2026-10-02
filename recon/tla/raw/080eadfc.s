.syntax unified
	.thumb
	.global Func_080eadfc
	.thumb_func
Func_080eadfc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r7, [r3]
	sub sp, #4
	cmp r7, #0
	beq .L_080eae9e
	movs r3, #34
	adds r3, r3, r7
	ldrb r0, [r3]
	mov r11, r3
	lsls r3, r0, #3
	ldr r2, [r2, #32]
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldr r6, .L_080eaeac
	mov r8, r3
	movs r3, #212
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r2, [r2]
	ldr r1, .L_080eaeb0
	add r6, r8
	asrs r6, r6, #2
	mov r10, r2
	adds r6, r6, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	bl Func_080202f0
	mov r2, r11
	ldr r1, [r7, #8]
	mov r9, r0
	ldrb r0, [r2]
	ldr r2, [r7, #16]
	bl Map_GetTerrainHeightFar
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_080eab70
	asrs r5, r5, #19
	mov r3, r11
	adds r5, #4
	adds r4, r0, #0
	mov r1, r9
	ldrb r0, [r3]
	adds r2, r5, #0
	adds r6, r6, r4
	str r4, [sp, #0]
	bl Func_080eab98
	ldr r4, [sp, #0]
	movs r2, #128
	lsls r4, r4, #2
	add r8, r4
	mov r1, r8
	ldrb r3, [r1, #3]
	add r10, r4
	str r3, [r7, #76]
	orrs r3, r2
	strb r3, [r1, #3]
	mov r2, r10
	movs r3, #255
	strb r0, [r6]
	strb r3, [r2, #2]
.L_080eae9e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080eaeac:
	.4byte 0xfdff0000
.L_080eaeb0:
	.4byte gMapShapeGrid
