.syntax unified
	.thumb
	.global Func_080dcf2c
	.thumb_func
Func_080dcf2c:
	push {lr}
	subs r3, r1, #1
	cmp r1, #0
	bne .L_080dcf42
	ldr r3, .L_080dcf4c
	ldr r1, .L_080dcf50
	ldr r3, [r3]
	movs r2, #7
	lsrs r3, r3, #1
	ands r3, r2
	ldrb r3, [r1, r3]
.L_080dcf42:
	adds r1, r3, #0
	bl Animation_ApplyChildValuesFar
	pop {pc}
	.2byte 0x0000
.L_080dcf4c:
	.4byte gFrameCount
.L_080dcf50:
	.4byte Data_080f0e9c
