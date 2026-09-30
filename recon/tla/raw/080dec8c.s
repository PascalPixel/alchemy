.syntax unified
	.thumb
	.global Func_080dec8c
	.thumb_func
Func_080dec8c:
	push {lr}
	ldr r3, .L_080decb0
	movs r2, #183
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080decac
	movs r1, #144
	ldr r0, .L_080decb4
	lsls r1, r1, #3
	bl Func_080145a8
.L_080decac:
	pop {pc}
	.2byte 0x0000
.L_080decb0:
	.4byte gPartyState
.L_080decb4:
	.4byte Func_080deba4
