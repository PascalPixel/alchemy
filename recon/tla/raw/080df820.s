.syntax unified
	.thumb
	.global Func_080df820
	.thumb_func
Func_080df820:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #138
	adds r6, r1, #0
	mov r8, r2
	adds r7, r3, #0
	bl Audio_PlayCue
	movs r0, #139
	adds r1, r5, #0
	lsls r0, r0, #1
	adds r2, r6, #0
	mov r3, r8
	bl Func_080dc10c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080df89e
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r3, #192
	lsls r3, r3, #10
	adds r2, r5, #0
	str r3, [r5, #48]
	str r3, [r5, #52]
	adds r2, #90
	movs r3, #0
	strb r3, [r2]
	movs r1, #1
	bl Object_SetMode
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_080df89c
	ldr r6, .L_080df898
.L_080df872:
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #28]
	str r3, [r5, #24]
	ldrh r3, [r5, #6]
	movs r0, #1
	adds r3, r3, r6
	strh r3, [r5, #6]
	bl WaitFrames
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	ble .L_080df872
	b .L_080df89c
	.2byte 0x0000
.L_080df898:
	.4byte 0x00002000
.L_080df89c:
	strh r7, [r5, #6]
.L_080df89e:
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
