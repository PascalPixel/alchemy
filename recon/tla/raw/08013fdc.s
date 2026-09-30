.syntax unified
	.thumb
	.global Func_08013fdc
	.thumb_func
Func_08013fdc:
	push {r5, lr}
	ldr r2, .L_08013ff8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08013ff4
	adds r5, r2, #0
.L_08013fe8:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_08013fe8
.L_08013ff4:
	pop {r5, pc}
	.2byte 0x0000
.L_08013ff8:
	.4byte gBlendDuration
