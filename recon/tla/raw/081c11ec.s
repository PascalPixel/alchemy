.syntax unified
	.thumb
	.global Func_081c11ec
	.thumb_func
Func_081c11ec:
	push {r5, lr}
	ldr r3, .L_081c1264
	lsrs r0, r0, #16
	ldr r2, [r3]
	movs r3, #15
	ands r0, r3
	movs r3, #3
	strb r3, [r2, #11]
	movs r3, #246
	lsls r3, r3, #7
	adds r3, #48
	str r3, [r2, #20]
	movs r1, #132
	movs r3, #133
	lsls r1, r1, #2
	lsls r3, r3, #1
	strb r0, [r2, #8]
	str r1, [r2, #16]
	str r3, [r2, #24]
	movs r2, #0
	ldr r3, .L_081c1268
	strh r2, [r3]
	ldr r0, .L_081c126c
	bl Math_Div
	ldr r3, .L_081c1270
	strh r0, [r3]
	ldr r5, .L_081c1274
	movs r1, #198
	ldr r3, .L_081c1278
	lsls r1, r1, #5
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	bl AudioEngine_ResumeDirectSound
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #188
	str r5, [r3]
	movs r3, #198
	lsls r3, r3, #4
	adds r5, r5, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #200
	str r5, [r3]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #6
.L_081c1250:
	ldrb r3, [r2]
	cmp r3, #159
	beq .L_081c1250
.L_081c1256:
	ldrb r3, [r2]
	cmp r3, #159
	bne .L_081c1256
	ldr r3, .L_081c1268
	movs r2, #128
	strh r2, [r3]
	pop {r5, pc}
.L_081c1264:
	.4byte Data_03007ff0
.L_081c1268:
	.4byte 0x04000102
.L_081c126c:
	.4byte 0xfffddb60
.L_081c1270:
	.4byte 0x04000100
.L_081c1274:
	.4byte Data_02003a90
.L_081c1278:
	.4byte IwramClearWords
