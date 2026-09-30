.syntax unified
	.thumb
	.global Func_08100e5c
	.thumb_func
Func_08100e5c:
	push {lr}
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08100e6e
	ldr r0, .L_08100e74
	b .L_08100e70
.L_08100e6e:
	ldr r0, .L_08100e78
.L_08100e70:
	pop {pc}
	.2byte 0x0000
.L_08100e74:
	.4byte 0x00003007
.L_08100e78:
	.4byte 0x00002fd2
