.syntax unified
	.thumb
	.global Func_08043b70
	.thumb_func
Func_08043b70:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #0
	adds r3, #220
	ldr r3, [r3]
	mov r8, r2
	movs r2, #250
	lsls r2, r2, #17
	mov r10, r2
	movs r2, #140
	sub sp, #28
	lsls r2, r2, #1
	adds r5, r3, #0
	add r4, sp, #4
	add r6, sp, #12
	adds r7, r3, r2
	adds r5, #248
.L_08043b9a:
	ldr r0, [r5]
	cmp r0, #0
	beq .L_08043bd2
	ldr r3, [r5, #64]
	adds r1, r6, #0
	str r3, [sp, #4]
	str r4, [sp, #0]
	ldr r3, [r5, #64]
	str r3, [r4, #4]
	movs r2, #0
	ldrsh r3, [r7, r2]
	lsls r3, r3, #16
	str r3, [r6]
	mov r3, r10
	str r3, [r6, #4]
	movs r2, #16
	ldrsh r3, [r7, r2]
	adds r2, r4, #0
	lsls r3, r3, #16
	add r3, r10
	str r3, [r6, #8]
	movs r3, #0
	str r3, [r6, #12]
	movs r3, #128
	lsls r3, r3, #7
	bl Func_08020010
	ldr r4, [sp, #0]
.L_08043bd2:
	movs r3, #1
	add r8, r3
	mov r2, r8
	adds r7, #2
	adds r5, #4
	cmp r2, #3
	ble .L_08043b9a
	add sp, #28
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
