.syntax unified
	.thumb
	.global Func_080de154
	.thumb_func
Func_080de154:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #134
	sub sp, #12
	bl Audio_PlayCue
	ldr r1, [r5, #8]
	mov r6, sp
	str r1, [r6]
	ldr r4, .L_080de208
	ldr r2, [r5, #12]
	movs r0, #208
	str r2, [r6, #4]
	lsls r0, r0, #1
	ldr r3, [r5, #16]
	adds r0, #255
	str r3, [r6, #8]
	adds r2, r2, r4
	bl Func_080dc10c
	cmp r0, #0
	beq .L_080de19a
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #9
	movs r3, #20
	strh r3, [r2]
	ldr r1, .L_080de20c
	bl Object_SetCallback
.L_080de19a:
	movs r0, #128
	lsls r0, r0, #9
	mov r8, r6
	movs r7, #11
	mov r10, r0
.L_080de1a4:
	movs r0, #209
	mov r3, r8
	lsls r0, r0, #1
	ldr r1, [r3]
	ldr r2, [r3, #4]
	adds r0, #255
	ldr r3, [r3, #8]
	bl Func_080dc10c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080de1f4
	ldr r1, .L_080de210
	bl Object_SetCallback
	bl Random16
	adds r2, r6, #0
	adds r2, #85
	mov r4, r10
	movs r3, #0
	add r0, r10
	str r4, [r6, #52]
	str r0, [r6, #48]
	strb r3, [r2]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #12
	lsls r5, r5, #3
	adds r5, r5, r0
	bl Random16
	adds r1, r5, #0
	adds r2, r0, #0
	adds r0, r6, #0
	bl Func_080db974
.L_080de1f4:
	subs r7, #1
	cmp r7, #0
	bge .L_080de1a4
	movs r0, #0
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080de208:
	.4byte 0xffe00000
.L_080de20c:
	.4byte Data_080f0e54
.L_080de210:
	.4byte Data_080f0e78
