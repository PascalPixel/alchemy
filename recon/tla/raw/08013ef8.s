.syntax unified
	.thumb
	.global Func_08013ef8
	.thumb_func
Func_08013ef8:
	ldr r2, .L_08013f20
	movs r3, #1
	strb r3, [r2]
	ldr r2, .L_08013f24
	ldr r3, .L_08013f1c
	ldr r1, .L_08013f28
	strh r3, [r2]
	ldr r2, .L_08013f2c
	ldrb r3, [r2]
	strb r3, [r1]
	movs r3, #16
	strb r3, [r2]
	ldr r3, .L_08013f30
	ldr r2, .L_08013f34
	strb r0, [r3]
	ldrb r3, [r3]
	strb r3, [r2]
	b .L_08013f38
.L_08013f1c:
	.4byte 0x0000003e
.L_08013f20:
	.4byte Data_030011dc
.L_08013f24:
	.4byte Data_030011f4
.L_08013f28:
	.4byte Data_0300113c
.L_08013f2c:
	.4byte Data_030011b0
.L_08013f30:
	.4byte Data_03001178
.L_08013f34:
	.4byte Data_0300110c
.L_08013f38:
	bx lr
	.2byte 0x0000
