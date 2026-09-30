.syntax unified
	.thumb
	.global Func_080f8c94
	.thumb_func
Func_080f8c94:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	movs r2, #56
	sub sp, #4
	ldr r3, [r3]
	mov r8, r2
	cmp r0, #1
	beq .L_080f8cb2
	movs r2, #40
	mov r8, r2
.L_080f8cb2:
	adds r5, r3, #0
	adds r5, #76
	movs r3, #5
	movs r6, #0
	adds r7, r5, #0
	mov r10, r3
.L_080f8cbe:
	ldmia r7!, {r3}
	cmp r3, #0
	beq .L_080f8cd4
	mov r2, r10
	str r2, [sp, #0]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #116
	mov r3, r8
	bl ItemMenu_PosOwner
.L_080f8cd4:
	adds r6, #1
	adds r5, #4
	cmp r6, #14
	ble .L_080f8cbe
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
