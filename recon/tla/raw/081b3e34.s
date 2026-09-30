.syntax unified
	.thumb
	.global Func_081b3e34
	.thumb_func
Func_081b3e34:
	push {lr}
	ldr r3, .L_081b3e6c
	movs r0, #192
	ldr r3, [r3]
	lsls r0, r0, #2
	movs r2, #0
	adds r0, #255
	movs r1, #0
	adds r3, #4
.L_081b3e46:
	str r2, [r3, #4]
	adds r2, #1
	str r1, [r3]
	adds r3, #12
	cmp r2, r0
	ble .L_081b3e46
	ldr r3, .L_081b3e6c
	movs r2, #192
	ldr r3, [r3]
	lsls r2, r2, #6
	adds r3, r3, r2
	movs r1, #0
	movs r2, #255
.L_081b3e60:
	subs r2, #1
	stmia r3!, {r1}
	cmp r2, #0
	bge .L_081b3e60
	pop {pc}
	.2byte 0x0000
.L_081b3e6c:
	.4byte Flash_Handler3
