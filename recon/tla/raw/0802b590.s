.syntax unified
	.thumb
	.global Func_0802b590
	.thumb_func
Func_0802b590:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r12, r3
	ldr r3, [sp, #32]
	adds r7, r2, #0
	ldr r2, [sp, #28]
	lsls r3, r3, #7
	adds r3, r3, r2
	ldr r4, .L_0802b618
	ldr r2, .L_0802b61c
	lsls r1, r1, #7
	mov lr, r4
	adds r1, r1, r0
	adds r4, r3, r2
	lsls r3, r3, #2
	adds r2, r2, r1
	add r3, lr
	lsls r1, r1, #2
	mov r0, r12
	mov r9, r4
	mov r10, r2
	mov r8, r3
	add lr, r1
	cmp r0, #0
	ble .L_0802b60c
	movs r6, #0
.L_0802b5ca:
	lsrs r3, r6, #16
	lsls r2, r3, #9
	mov r1, r8
	mov r0, lr
	adds r5, r1, r2
	adds r4, r0, r2
	lsls r3, r3, #7
	mov r2, r9
	mov r0, r10
	adds r1, r2, r3
	adds r2, r0, r3
	cmp r7, #0
	ble .L_0802b5fe
	movs r0, #0
.L_0802b5e6:
	ldmia r4!, {r3}
	stmia r5!, {r3}
	ldrb r3, [r2]
	adds r2, #1
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #9
	adds r0, r0, r3
	lsrs r3, r0, #16
	adds r1, #1
	cmp r3, r7
	blt .L_0802b5e6
.L_0802b5fe:
	movs r4, #128
	lsls r4, r4, #9
	adds r3, r6, r4
	adds r6, r3, #0
	lsrs r3, r6, #16
	cmp r3, r12
	blt .L_0802b5ca
.L_0802b60c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802b618:
	.4byte gMapCellBuffer
.L_0802b61c:
	.4byte gMapShapeGrid
