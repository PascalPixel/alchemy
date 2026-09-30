.syntax unified
	.thumb
	.global Func_081b3e70
	.thumb_func
Func_081b3e70:
	push {r5, lr}
	ldr r3, .L_081b3eac
	lsls r1, r0, #1
	ldr r4, [r3]
	movs r3, #208
	lsls r3, r3, #6
	adds r1, r1, r0
	adds r3, #4
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r2, [r4, r0]
	movs r0, #192
	lsls r2, r2, #2
	lsls r1, r1, #2
	adds r3, r4, r2
	lsls r0, r0, #6
	adds r3, r3, r0
	adds r5, r1, #4
	str r3, [r4, r5]
	adds r2, r2, r0
	ldr r3, [r4, r2]
	str r3, [r4, r1]
	adds r3, r4, r1
	str r3, [r4, r2]
	ldr r2, [r3]
	cmp r2, #0
	beq .L_081b3ea8
	str r3, [r2, #4]
.L_081b3ea8:
	pop {r5, pc}
	.2byte 0x0000
.L_081b3eac:
	.4byte Flash_Handler3
