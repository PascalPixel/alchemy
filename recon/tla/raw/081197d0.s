.syntax unified
	.thumb
	.global Func_081197d0
	.thumb_func
Func_081197d0:
	push {lr}
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_081197e2
	ldr r0, .L_081197e8
	b .L_081197e4
.L_081197e2:
	ldr r0, .L_081197ec
.L_081197e4:
	pop {pc}
	.2byte 0x0000
.L_081197e8:
	.4byte 0x00003007
.L_081197ec:
	.4byte 0x00002fd2
