.syntax unified
	.thumb
	.global Func_080162f0
	.thumb_func
Func_080162f0:
	ldr r0, .L_08016334
	ldr r4, .L_08016324
	ldr r1, .L_08016338
	strh r4, [r0]
	movs r3, #255
	ldrh r2, [r1]
	lsls r3, r3, #8
	adds r3, #63
	ands r3, r2
	strh r3, [r1]
	ldr r3, .L_08016328
	ldr r2, .L_0801633c
	strh r3, [r0]
	ldr r3, .L_0801632c
	strh r3, [r2]
	movs r3, #201
	lsls r3, r3, #8
	subs r2, #28
	adds r3, #99
	str r3, [r2]
	ldr r3, .L_08016330
	adds r2, #246
	strh r3, [r2]
	ldr r3, .L_08016340
	strb r4, [r3, #8]
	b .L_08016344
.L_08016324:
	.4byte 0x00000000
.L_08016328:
	.4byte 0x00000001
.L_0801632c:
	.4byte 0x00002003
.L_08016330:
	.4byte 0x000000c0
.L_08016334:
	.4byte 0x04000208
.L_08016338:
	.4byte 0x04000200
.L_0801633c:
	.4byte 0x04000128
.L_08016340:
	.4byte Data_02005360
.L_08016344:
	bx lr
	.2byte 0x0000
