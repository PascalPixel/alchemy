.syntax unified
	.thumb
	.section .text.x02008584,"ax",%progbits
	.balign 4
	.global Func_02000584
	.thumb_func
Func_02000584:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #16
	ldr r5, .L_020086e0
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_020014a8
	ldr r1, [r5]
	movs r0, #9
	bl Func_020014a8
	ldr r1, [r5]
	movs r0, #10
	bl Func_020014a8
	movs r0, #1
	bl wait
	movs r0, #8
	bl Func_0200056c
	movs r0, #9
	bl Func_0200056c
	movs r0, #10
	bl Func_0200056c
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_020014a0
	movs r0, #1
	bl wait
	b .L_020086be
.L_0200860a:
	mov r3, r11
	ldrh r3, [r3]
	mov r9, r3
	mov r0, r9
	bl Actor_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r5, r0, #0
	str r2, [sp, #8]
	adds r7, r5, #0
	adds r7, #34
	adds r0, r2, #0
	ldrb r2, [r7]
	adds r0, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r6, [r2, r3]
	ldr r2, .L_020086e4
	adds r3, r6, r2
	ldr r2, .L_020086e8
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008660
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	b .L_020086ba
.L_02008660:
	adds r0, r5, #0
	bl Func_02001508
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02001478
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_020014c0
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02001528
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ba
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_020086ba:
	movs r3, #4
	add r11, r3
.L_020086be:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200860a
	movs r0, #10
	bl wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020086e0:
	.4byte gMainRoutineWork
.L_020086e4:
	.4byte 0xfdff0000
.L_020086e8:
	.4byte gMapShapeGrid
	.section .text.x02008caa,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008ce2,"ax",%progbits
	.2byte 0x0000