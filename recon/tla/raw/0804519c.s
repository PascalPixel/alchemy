.syntax unified
	.thumb
	.global Func_0804519c
	.thumb_func
Func_0804519c:
	push {lr}
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080451ae
	ldr r0, .L_080451b4
	b .L_080451b0
.L_080451ae:
	ldr r0, .L_080451b8
.L_080451b0:
	pop {pc}
	.2byte 0x0000
.L_080451b4:
	.4byte 0x00003007
.L_080451b8:
	.4byte 0x00002fd2
