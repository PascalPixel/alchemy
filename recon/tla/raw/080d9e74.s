.syntax unified
	.thumb
	.global Func_080d9e74
	.thumb_func
Func_080d9e74:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	movs r2, #0
	adds r0, r3, #0
	adds r0, #236
.L_080d9e84:
	movs r3, #16
	ldrsb r3, [r0, r3]
	cmp r3, #0
	beq .L_080d9e96
	adds r2, #1
	adds r0, #32
	cmp r2, #127
	ble .L_080d9e84
	movs r0, #0
.L_080d9e96:
	pop {pc}
