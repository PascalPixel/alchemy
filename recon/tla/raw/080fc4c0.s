.syntax unified
	.thumb
	.global Func_080fc4c0
	.thumb_func
Func_080fc4c0:
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
	movs r2, #133
	mov r8, r3
	lsls r2, r2, #2
	adds r5, r1, #0
	mov r6, r8
	adds r5, r5, r2
	adds r6, #2
	mov r10, r0
	ldrb r0, [r6, r5]
	bl Func_080fad1c
	adds r7, r0, #0
	ldrb r0, [r6, r5]
	bl Owner_GetState
	ldrb r3, [r6, r5]
	movs r2, #153
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r2, r8
	ldrsb r6, [r2, r3]
	mov r11, r0
	adds r3, r6, #1
	cmp r3, r7
	ble .L_080fc50a
	subs r6, r7, #1
.L_080fc50a:
	movs r1, #5
	adds r0, r6, #0
	bl __divsi3
	movs r1, #5
	mov r9, r0
	adds r0, r6, #0
	bl Math_Mod
	movs r1, #5
	mov r8, r0
	adds r0, r7, #0
	bl __divsi3
	movs r1, #5
	adds r5, r0, #0
	adds r0, r7, #0
	bl Math_Mod
	cmp r0, #0
	beq .L_080fc536
	adds r5, #1
.L_080fc536:
	mov r2, r10
	mov r3, r11
	str r3, [r2]
	mov r3, r9
	str r3, [r2, #8]
	mov r3, r8
	str r5, [r2, #12]
	str r3, [r2, #16]
	str r7, [r2, #20]
	str r6, [r2, #24]
	movs r0, #1
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
