.syntax unified
	.thumb
	.global Func_080c9c10
	.thumb_func
Func_080c9c10:
	push {lr}
	ldr r3, .L_080c9c2c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r2, .L_080c9c30
	lsls r3, r3, #3
	ldrsh r0, [r3, r2]
	ldr r1, .L_080c9c34
	bl Func_0801336c
	pop {pc}
.L_080c9c2c:
	.4byte gPartyState
.L_080c9c30:
	.4byte Data_080f17a8
.L_080c9c34:
	.4byte Data_02008000
