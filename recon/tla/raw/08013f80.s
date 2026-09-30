.syntax unified
	.thumb
	.global Func_08013f80
	.thumb_func
Func_08013f80:
	push {r5, lr}
	adds r5, r3, #0
	ldr r3, .L_08013fa4
	adds r4, r2, #0
	strb r0, [r3]
	ldr r3, .L_08013fa0
	ldr r2, .L_08013fa8
	ands r1, r3
	strh r1, [r2]
	cmp r4, #16
	bls .L_08013fb4
	ldr r2, .L_08013fac
	ldr r3, .L_08013fb0
	ldrb r2, [r2]
	strb r2, [r3]
	b .L_08013fb8
.L_08013fa0:
	.4byte 0x0000003f
.L_08013fa4:
	.4byte Data_030011dc
.L_08013fa8:
	.4byte Data_030011f4
.L_08013fac:
	.4byte Data_030011b0
.L_08013fb0:
	.4byte Data_0300113c
.L_08013fb4:
	ldr r3, .L_08013fcc
	strb r4, [r3]
.L_08013fb8:
	ldr r3, .L_08013fd0
	ldr r2, .L_08013fd4
	strb r5, [r3]
	ldr r3, [sp, #8]
	ldr r1, .L_08013fd8
	strb r3, [r2]
	ldrb r3, [r2]
	strb r3, [r1]
	pop {r5, pc}
	.2byte 0x0000
.L_08013fcc:
	.4byte Data_0300113c
.L_08013fd0:
	.4byte Data_030011b0
.L_08013fd4:
	.4byte Data_03001178
.L_08013fd8:
	.4byte Data_0300110c
