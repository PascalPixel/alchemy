.syntax unified
	.thumb
	.global Func_080144c0
	.thumb_func
Func_080144c0:
	push {lr}
	ldr r3, .L_080144f4
	movs r2, #0
	strb r2, [r3]
	ldr r3, .L_080144f8
	ldr r1, .L_080144fc
	ldr r4, .L_080144f0
	strb r2, [r3]
	movs r0, #0
	movs r2, #23
.L_080144d4:
	ldrh r3, [r1, #4]
	subs r2, #1
	orrs r3, r4
	str r0, [r1]
	strh r3, [r1, #4]
	strb r0, [r1, #6]
	adds r1, #8
	cmp r2, #0
	bge .L_080144d4
	ldr r2, .L_080144f4
	movs r3, #1
	strb r3, [r2]
	b .L_08014500
	.2byte 0x0000
.L_080144f0:
	.4byte 0x0000ffff
.L_080144f4:
	.4byte Data_03001228
.L_080144f8:
	.4byte Data_03001108
.L_080144fc:
	.4byte Data_02003610
.L_08014500:
	pop {pc}
	.2byte 0x0000
