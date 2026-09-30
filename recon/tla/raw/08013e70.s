.syntax unified
	.thumb
	.global Func_08013e70
	.thumb_func
Func_08013e70:
	ldr r2, .L_08013e98
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_08013e9c
	ldr r3, .L_08013e94
	ldr r1, .L_08013ea0
	strh r3, [r2]
	ldr r2, .L_08013ea4
	ldrb r3, [r2]
	strb r3, [r1]
	movs r3, #16
	strb r3, [r2]
	ldr r3, .L_08013ea8
	ldr r2, .L_08013eac
	strb r0, [r3]
	ldrb r3, [r3]
	strb r3, [r2]
	b .L_08013eb0
.L_08013e94:
	.4byte 0x0000003e
.L_08013e98:
	.4byte Data_030011dc
.L_08013e9c:
	.4byte Data_030011f4
.L_08013ea0:
	.4byte Data_0300113c
.L_08013ea4:
	.4byte Data_030011b0
.L_08013ea8:
	.4byte Data_03001178
.L_08013eac:
	.4byte Data_0300110c
.L_08013eb0:
	bx lr
	.2byte 0x0000
