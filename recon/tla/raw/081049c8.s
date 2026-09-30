.syntax unified
	.thumb
	.global Func_081049c8
	.thumb_func
Func_081049c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #0
	mov r10, r3
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	mov r8, r2
	mov r11, r0
	cmp r8, r3
	bge .L_08104a4c
	movs r7, #129
	movs r3, #1
	lsls r7, r7, #2
	mov r9, r3
	add r7, r10
.L_081049fe:
	ldrh r0, [r7]
	bl Owner_GetState
	movs r6, #0
	movs r4, #0
	adds r0, #248
.L_08104a0a:
	ldr r5, [r0, #16]
	movs r1, #0
.L_08104a0e:
	mov r2, r9
	lsls r2, r1
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_08104a22
	ldr r3, [r0]
	ands r3, r2
	cmp r3, #0
	beq .L_08104a24
.L_08104a22:
	adds r4, #1
.L_08104a24:
	adds r1, #1
	cmp r1, #19
	ble .L_08104a0e
	adds r6, #1
	adds r0, #4
	cmp r6, #3
	ble .L_08104a0a
	mov r3, r8
	mov r2, r11
	strb r4, [r2, r3]
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	movs r2, #1
	add r8, r2
	adds r7, #2
	cmp r8, r3
	blt .L_081049fe
.L_08104a4c:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
