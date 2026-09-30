.syntax unified
	.thumb
	.global Func_08143114
	.thumb_func
Func_08143114:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #92]
	ldr r1, .L_08143168
	ldrh r3, [r1]
	adds r0, r3, #0
	strh r1, [r1]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #236
	adds r2, r5, r3
	ldr r3, [r2]
	cmp r3, #1
	bne .L_08143164
	movs r3, #0
	str r3, [r2]
	strh r0, [r1]
	movs r2, #246
	lsls r2, r2, #7
	adds r2, #116
	adds r3, r5, r2
	ldr r0, [r3]
	movs r3, #207
	lsls r3, r3, #7
	adds r5, r5, r3
	movs r2, #128
	ldr r3, .L_0814316c
	adds r1, r5, #0
	lsls r2, r2, #5
	mov lr, r3
	.2byte 0xf800
	movs r1, #128
	ldr r3, .L_08143170
	adds r0, r5, #0
	lsls r1, r1, #5
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	b .L_08143166
.L_08143164:
	strh r0, [r1]
.L_08143166:
	pop {r5, pc}
.L_08143168:
	.4byte 0x04000208
.L_0814316c:
	.4byte IwramCopyWords
.L_08143170:
	.4byte IwramFillWords
