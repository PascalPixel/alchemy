.syntax unified
	.thumb
	.global Func_080daea8
	.thumb_func
Func_080daea8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r3, [r3]
	movs r2, #0
	adds r0, r3, #0
	adds r0, #228
.L_080daeb8:
	movs r3, #18
	ldrsb r3, [r0, r3]
	cmp r3, #0
	beq .L_080daeca
	adds r2, #1
	adds r0, #32
	cmp r2, #127
	ble .L_080daeb8
	movs r0, #0
.L_080daeca:
	pop {pc}
