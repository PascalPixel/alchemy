.syntax unified
	.thumb
	.global Func_08014aa8
	.thumb_func
Func_08014aa8:
	push {lr}
	ldr r3, .L_08014ad4
	ldr r1, .L_08014ad8
	ldrb r3, [r3]
	ldr r2, [r1]
	cmp r3, #0
	beq .L_08014adc
	movs r3, #0
	cmp r3, r0
	bcs .L_08014aca
	ldr r1, .L_08014ad0
.L_08014abe:
	adds r3, #1
	strh r1, [r2]
	adds r2, #2
	cmp r3, r0
	bcc .L_08014abe
	ldr r1, .L_08014ad8
.L_08014aca:
	str r2, [r1]
	b .L_08014adc
	.2byte 0x0000
.L_08014ad0:
	.4byte 0x0000f000
.L_08014ad4:
	.4byte Data_03001110
.L_08014ad8:
	.4byte Data_030011c4
.L_08014adc:
	pop {pc}
	.2byte 0x0000
