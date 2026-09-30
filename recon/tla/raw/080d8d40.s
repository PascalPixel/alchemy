.syntax unified
	.thumb
	.global Func_080d8d40
	.thumb_func
Func_080d8d40:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	movs r2, #134
	lsls r2, r2, #1
	adds r0, r3, r2
	movs r2, #0
.L_080d8d52:
	movs r3, #18
	ldrsb r3, [r0, r3]
	cmp r3, #0
	beq .L_080d8d64
	adds r2, #1
	adds r0, #32
	cmp r2, #127
	ble .L_080d8d52
	movs r0, #0
.L_080d8d64:
	pop {pc}
	.2byte 0x0000
