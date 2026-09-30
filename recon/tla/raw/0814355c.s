.syntax unified
	.thumb
	.global Func_0814355c
	.thumb_func
Func_0814355c:
	ldr r3, .L_08143594
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08143598
	adds r2, #2
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #0
	adds r3, #40
	str r1, [r3]
	ldr r3, .L_081435a4
	subs r2, #38
	str r3, [r2]
	ldr r3, .L_0814359c
	subs r2, #12
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #34
	strh r1, [r3]
	adds r3, #2
	strh r1, [r3]
	ldr r3, .L_081435a0
	adds r2, #6
	b .L_081435a8
.L_08143594:
	.4byte 0x00003f44
.L_08143598:
	.4byte 0x0000100e
.L_0814359c:
	.4byte 0x00000080
.L_081435a0:
	.4byte 0x00000100
.L_081435a4:
	.4byte 0xfffff000
.L_081435a8:
	strh r3, [r2]
	ldr r1, .L_081435d0
	movs r3, #128
	ldr r2, .L_081435d4
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_081435d8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #74
	strh r3, [r2]
	b .L_081435dc
	.2byte 0x0000
.L_081435d0:
	.4byte 0x000000f0
.L_081435d4:
	.4byte 0x00001088
.L_081435d8:
	.4byte 0x00003f21
.L_081435dc:
	bx lr
	.2byte 0x0000
