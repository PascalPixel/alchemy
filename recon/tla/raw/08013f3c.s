.syntax unified
	.thumb
	.global Func_08013f3c
	.thumb_func
Func_08013f3c:
	ldr r2, .L_08013f64
	movs r3, #1
	strb r3, [r2]
	ldr r2, .L_08013f68
	ldr r3, .L_08013f60
	ldr r1, .L_08013f6c
	strh r3, [r2]
	ldr r2, .L_08013f70
	ldrb r3, [r2]
	strb r3, [r1]
	movs r3, #0
	strb r3, [r2]
	ldr r3, .L_08013f74
	ldr r2, .L_08013f78
	strb r0, [r3]
	ldrb r3, [r3]
	strb r3, [r2]
	b .L_08013f7c
.L_08013f60:
	.4byte 0x0000003e
.L_08013f64:
	.4byte Data_030011dc
.L_08013f68:
	.4byte Data_030011f4
.L_08013f6c:
	.4byte Data_0300113c
.L_08013f70:
	.4byte Data_030011b0
.L_08013f74:
	.4byte Data_03001178
.L_08013f78:
	.4byte Data_0300110c
.L_08013f7c:
	bx lr
	.2byte 0x0000
