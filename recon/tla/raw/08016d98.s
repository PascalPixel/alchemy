.syntax unified
	.thumb
	.global Func_08016d98
	.thumb_func
Func_08016d98:
	push {lr}
	ldr r1, .L_08016db0
	lsls r3, r0, #20
	lsrs r0, r3, #23
	ldrb r2, [r1, r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_08016dac
	adds r3, #255
	strb r3, [r1, r0]
.L_08016dac:
	ldrb r0, [r1, r0]
	pop {pc}
.L_08016db0:
	.4byte GameFlagBytes
