.syntax unified
	.thumb
	.section .text.x02009262,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200929a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a17c,"ax",%progbits
	.balign 4
	.global Func_0200217c
	.thumb_func
Func_0200217c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r2
	ldr r2, [sp, #32]
	ldr r4, [sp, #28]
	lsls r2, r2, #7
	adds r2, r2, r4
	ldr r4, .L_0200a22c
	ldr r5, .L_0200a230
	lsls r1, r1, #7
	adds r1, r1, r0
	subs r3, #1
	adds r6, r2, r4
	adds r0, r1, r4
	lsls r2, r2, #2
	lsls r1, r1, #2
	lsls r3, r3, #16
	adds r4, r2, r5
	adds r5, r1, r5
	asrs r1, r3, #16
	cmp r1, #0
	blt .L_0200a220
	ldr r2, .L_0200a234
	lsls r3, r1, #16
	adds r2, r2, r3
	lsls r3, r1, #7
	mov r1, r10
	mov r8, r2
	adds r2, r0, r1
	adds r2, r2, r3
	mov lr, r2
	adds r2, r6, r1
	adds r2, r2, r3
	add r3, r10
	lsls r3, r3, #2
	mov r12, r2
	adds r7, r3, r5
	adds r6, r3, r4
.L_0200a1ce:
	mov r3, r10
	subs r3, #1
	lsls r3, r3, #16
	mov r4, r12
	mov r0, lr
	asrs r3, r3, #16
	subs r5, r6, #4
	subs r1, r7, #4
	subs r4, #1
	subs r0, #1
	cmp r3, #0
	blt .L_0200a208
	lsls r2, r3, #16
	ldr r3, .L_0200a234
	adds r2, r2, r3
.L_0200a1ec:
	ldr r3, [r1]
	mov r9, r2
	str r3, [r5]
	subs r1, #4
	ldrb r3, [r0]
	subs r5, #4
	strb r3, [r4]
	ldr r3, .L_0200a234
	subs r4, #1
	adds r2, r2, r3
	mov r3, r9
	subs r0, #1
	cmp r3, #0
	bge .L_0200a1ec
.L_0200a208:
	movs r3, #128
	negs r3, r3
	ldr r1, .L_0200a234
	add lr, r3
	add r12, r3
	ldr r3, .L_0200a238
	mov r2, r8
	adds r7, r7, r3
	add r8, r1
	adds r6, r6, r3
	cmp r2, #0
	bge .L_0200a1ce
.L_0200a220:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a22c:
	.4byte gMapShapeGrid
.L_0200a230:
	.4byte gMapCellBuffer
.L_0200a234:
	.4byte 0xffff0000
.L_0200a238:
	.4byte 0xfffffe00
	.section .text.x0200bda0,"ax",%progbits
	.balign 4
	.global Func_02003da0
	.thumb_func
Func_02003da0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200bf54
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r9, r0
	movs r0, #158
	mov r4, r9
	lsls r0, r0, #1
	movs r3, #2
	ldrsh r2, [r4, r3]
	adds r3, r1, r0
	ldr r3, [r3]
	sub sp, #4
	str r3, [sp, #0]
	movs r4, #156
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r1, .L_0200bf58
	ldr r3, [r3]
	lsls r2, r2, #2
	mov r5, r9
	movs r0, #0
	adds r1, r1, r2
	adds r5, #4
	mov r11, r3
	mov r10, r0
	mov r8, r1
.L_0200bde4:
	movs r2, #0
	ldrsh r0, [r5, r2]
	cmp r0, #0
	bne .L_0200bdee
	b .L_0200bf34
.L_0200bdee:
	bl Actor_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_0200be02
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_0200be02
	b .L_0200bf34
.L_0200be02:
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #8
	ldrsh r3, [r5, r4]
	cmp r2, r3
	bne .L_0200be18
	movs r0, #206
	bl Func_02005a68
	movs r3, #4
	strh r3, [r5, #18]
.L_0200be18:
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #10
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bne .L_0200be34
	movs r0, #140
	adds r0, #255
	bl Func_02005a68
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	strh r3, [r5, #18]
.L_0200be34:
	movs r4, #18
	ldrsh r3, [r5, r4]
	ldrh r2, [r5, #18]
	cmp r3, #0
	beq .L_0200be6e
	ldrh r3, [r5, #4]
	movs r0, #0
	adds r3, r3, r2
	movs r4, #12
	ldrsh r2, [r5, r4]
	strh r3, [r5, #4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldrh r1, [r5, #12]
	cmp r3, r2
	blt .L_0200be5a
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200be5a:
	movs r1, #4
	ldrsh r2, [r5, r1]
	movs r4, #14
	ldrsh r3, [r5, r4]
	ldrh r1, [r5, #14]
	cmp r2, r3
	bgt .L_0200be6e
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200be6e:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200be8e
	ldrh r3, [r5, #2]
	movs r1, #6
	ldrsh r2, [r5, r1]
	adds r3, #1
	strh r3, [r5, #2]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_0200be8e
	strh r0, [r5, #2]
.L_0200be8e:
	ldr r3, [r7, #16]
	ldr r2, [r7, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	lsls r3, r3, #7
	adds r1, r2, r3
	mov r2, r9
	ldrb r3, [r2, #2]
	ldr r4, [sp, #0]
	add r3, r10
	strb r3, [r4, r1]
	movs r0, #14
	ldrsh r2, [r7, r0]
	movs r4, #4
	ldrsh r3, [r5, r4]
	adds r2, r2, r3
	cmp r2, #0
	bge .L_0200beb4
	adds r2, #7
.L_0200beb4:
	asrs r3, r2, #3
	lsls r3, r3, #8
	mov r0, r8
	str r3, [r0]
	mov r2, r11
	lsls r3, r1, #2
	adds r1, r2, r3
	movs r4, #18
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_0200bece
	movs r3, #0
	b .L_0200bed4
.L_0200bece:
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
.L_0200bed4:
	strb r3, [r1, #3]
	movs r0, #4
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_0200bee4
	ldrb r2, [r1, #3]
	movs r3, #16
	orrs r3, r2
.L_0200bee4:
	strb r3, [r1, #3]
	ldr r6, [r5, #24]
	cmp r6, #0
	beq .L_0200bf34
	movs r1, #4
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_0200befc
	ldr r3, [r7, #12]
	ldr r2, .L_0200bf5c
	adds r3, r3, r2
	b .L_0200bf2a
.L_0200befc:
	ldrh r0, [r5, #22]
	movs r3, #128
	lsls r3, r3, #5
	adds r0, r0, r3
	strh r0, [r5, #22]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r3, .L_0200bf60
	lsls r0, r0, #10
	mov lr, r3
	.2byte 0xf800
	movs r1, #4
	ldrsh r2, [r5, r1]
	ldr r3, [r7, #12]
	ldr r4, .L_0200bf64
	lsls r2, r2, #16
	adds r0, r0, r4
	adds r3, r3, r2
	adds r3, r3, r0
.L_0200bf2a:
	str r3, [r6, #12]
	ldr r3, [r7, #8]
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
.L_0200bf34:
	movs r3, #1
	add r10, r3
	movs r2, #4
	mov r4, r10
	add r8, r2
	adds r5, #28
	cmp r4, #15
	bgt .L_0200bf46
	b .L_0200bde4
.L_0200bf46:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bf54:
	.4byte gData_020023c4 + 0x188
.L_0200bf58:
	.4byte gMapCollision
.L_0200bf5c:
	.4byte 0xfff00000
.L_0200bf60:
	.4byte IwramMulQ16
.L_0200bf64:
	.4byte 0xfff20000
	.section .text.x0200c6a4,"ax",%progbits
	.balign 4
	.global Func_020046a4
	.thumb_func
Func_020046a4:
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
	ldr r5, .L_0200c800
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_020058f0
	ldr r1, [r5]
	movs r0, #9
	bl Func_020058f0
	ldr r1, [r5]
	movs r0, #10
	bl Func_020058f0
	movs r0, #1
	bl Task_Wait
	movs r0, #8
	bl Func_0200468c
	movs r0, #9
	bl Func_0200468c
	movs r0, #10
	bl Func_0200468c
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl Task_Wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_020058e0
	movs r0, #1
	bl Task_Wait
	b .L_0200c7de
.L_0200c72a:
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
	ldr r2, .L_0200c804
	adds r3, r6, r2
	ldr r2, .L_0200c808
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c780
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	b .L_0200c7da
.L_0200c780:
	adds r0, r5, #0
	bl Func_02005a18
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02005848
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02005958
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02005a20
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7da
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200c7da:
	movs r3, #4
	add r11, r3
.L_0200c7de:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c72a
	movs r0, #10
	bl Task_Wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c800:
	.4byte gMainRoutineWork
.L_0200c804:
	.4byte 0xfdff0000
.L_0200c808:
	.4byte gMapShapeGrid
	.section .text.x0200cddc,"ax",%progbits
	.balign 4
	.global Func_02004ddc
	.thumb_func
Func_02004ddc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_0200d0f0
	adds r3, #236
	ldr r3, [r3]
	mov r11, r0
	movs r1, #4
	ldrsh r0, [r0, r1]
	mov r8, r3
	sub sp, #8
	bl Actor_GetById
	ldr r5, .L_0200d0f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	adds r7, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	cmp r7, #0
	bne .L_0200ce18
	b .L_0200d190
.L_0200ce18:
	mov r1, r11
	movs r3, #0
	ldrsh r2, [r1, r3]
	cmp r2, #4
	bne .L_0200ce9e
	ldr r4, [r0, #16]
	movs r2, #255
	ldr r6, [r0, #8]
	lsls r2, r2, #24
	movs r0, #152
	adds r3, r4, r2
	lsls r0, r0, #17
	cmp r3, r0
	bls .L_0200ce3e
	ldr r3, .L_0200d0f8
	movs r2, #132
	ands r3, r6
	lsls r2, r2, #18
	subs r6, r2, r3
.L_0200ce3e:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200ce56
	movs r2, #152
	lsls r2, r2, #17
	adds r6, r6, r2
	b .L_0200ce5a
.L_0200ce56:
	ldr r3, .L_0200d0fc
	adds r6, r6, r3
.L_0200ce5a:
	ldr r1, [r7, #8]
	cmp r1, r6
	bne .L_0200ce68
	ldr r3, [r7, #16]
	cmp r3, r4
	bne .L_0200ce68
	b .L_0200d0d2
.L_0200ce68:
	ldr r3, [r7, #16]
	subs r1, r6, r1
	subs r0, r4, r3
	str r4, [sp, #0]
	bl ArcTan2
	ldrh r3, [r7, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	movs r2, #128
	asrs r0, r0, #16
	lsls r2, r2, #5
	ldr r4, [sp, #0]
	cmp r0, r2
	ble .L_0200ce8c
	adds r0, r2, #0
.L_0200ce8c:
	ldr r2, .L_0200d100
	cmp r0, r2
	bge .L_0200ce94
	adds r0, r2, #0
.L_0200ce94:
	adds r3, r3, r0
	strh r3, [r7, #6]
	str r6, [r7, #8]
	str r4, [r7, #16]
	b .L_0200d0d2
.L_0200ce9e:
	cmp r2, #3
	beq .L_0200cea4
	b .L_0200cfdc
.L_0200cea4:
	movs r0, #190
	lsls r0, r0, #2
	bl GameFlag_GetByte
	movs r1, #1
	eors r0, r1
	lsls r3, r0, #1
	ldr r2, .L_0200d104
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r5, r3, r2
	ldr r3, .L_0200d108
	ldrh r3, [r3]
	asrs r3, r0
	ands r3, r1
	cmp r3, #0
	bne .L_0200cec8
	b .L_0200d190
.L_0200cec8:
	movs r2, #0
	ldrsh r6, [r5, r2]
	adds r5, #2
	movs r3, #0
	ldrsh r4, [r5, r3]
	adds r5, #2
	movs r1, #0
	ldrsh r0, [r5, r1]
	adds r5, #2
	ldrh r3, [r5]
	mov r9, r0
	lsls r3, r3, #16
	asrs r2, r3, #24
	str r2, [sp, #4]
	movs r2, #255
	lsls r2, r2, #16
	movs r0, #137
	ands r2, r3
	lsls r0, r0, #1
	asrs r2, r2, #16
	adds r0, #255
	str r4, [sp, #0]
	mov r10, r2
	bl GameFlag_Test
	adds r5, #2
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_0200cf7e
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_0200cf7e
	movs r3, #9
	mov r0, r11
	strh r3, [r0]
	movs r1, #1
	adds r0, r7, #0
	bl Func_020057c8
	mov r3, r8
	adds r3, #236
	ldr r2, [r3]
	ldr r3, [r7, #8]
	movs r1, #192
	lsls r1, r1, #12
	adds r6, r2, r1
	cmp r2, r3
	blt .L_0200cf2c
	ldr r3, .L_0200d10c
	adds r6, r2, r3
.L_0200cf2c:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	mov r3, r8
	adds r2, r7, #0
	adds r3, #240
	adds r2, #100
	cmp r0, #0
	bne .L_0200cf50
	ldr r3, [r3]
	movs r0, #128
	lsls r0, r0, #13
	adds r4, r3, r0
	mov r3, r8
	adds r3, #232
	b .L_0200cf5a
.L_0200cf50:
	ldr r3, [r3]
	ldr r1, .L_0200d110
	adds r4, r3, r1
	mov r3, r8
	adds r3, #230
.L_0200cf5a:
	ldrh r3, [r3]
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r3, r4, #0
	bl Func_020057f8
	adds r0, r7, #0
	bl Func_020057f0
	b .L_0200cf92
.L_0200cf7e:
	lsls r6, r6, #16
	lsls r4, r4, #16
	mov r2, r9
	strh r2, [r7, #6]
	str r6, [r7, #8]
	str r4, [r7, #16]
	adds r0, r7, #0
	mov r1, r10
	bl Func_020057c8
.L_0200cf92:
	movs r3, #0
	ldrsh r0, [r5, r3]
	adds r5, #2
	movs r1, #0
	ldrsh r6, [r5, r1]
	adds r5, #2
	movs r2, #0
	ldrsh r4, [r5, r2]
	adds r5, #2
	movs r1, #0
	ldrsh r3, [r5, r1]
	mov r9, r3
	movs r3, #2
	ldrsh r2, [r5, r3]
	mov r10, r2
	cmp r0, #0
	bne .L_0200cfb6
	b .L_0200d190
.L_0200cfb6:
	str r4, [sp, #0]
	bl Actor_GetById
	adds r7, r0, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bne .L_0200cfc6
	b .L_0200d190
.L_0200cfc6:
	lsls r3, r6, #16
	mov r0, r9
	str r3, [r7, #8]
	lsls r3, r4, #16
	strh r0, [r7, #6]
	str r3, [r7, #16]
	adds r0, r7, #0
	mov r1, r10
	bl Func_020057c8
	b .L_0200d190
.L_0200cfdc:
	cmp r2, #1
	beq .L_0200cfe2
	b .L_0200d114
.L_0200cfe2:
	mov r5, r8
	adds r5, #234
	mov r2, r11
	movs r1, #10
	ldrsh r0, [r2, r1]
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r0, r3
	beq .L_0200d006
	bl Resource_GetTableEntry
	mov r1, r8
	adds r1, #244
	bl Func_02005770
	mov r2, r11
	ldrh r3, [r2, #10]
	strh r3, [r5]
.L_0200d006:
	mov r1, r11
	movs r0, #6
	ldrsh r3, [r1, r0]
	mov r0, r8
	lsls r3, r3, #1
	adds r2, r3, #0
	adds r2, #244
	adds r3, #246
	ldrsh r6, [r0, r2]
	ldrsh r4, [r0, r3]
	movs r0, #130
	lsls r0, r0, #1
	str r4, [sp, #0]
	bl GameFlag_Test
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0200d034
	mov r0, r11
	ldrh r3, [r0, #6]
	mov r1, r11
	adds r3, #2
	strh r3, [r1, #6]
.L_0200d034:
	cmp r6, #0
	bne .L_0200d040
	cmp r4, #0
	bne .L_0200d040
	movs r3, #9
	b .L_0200d186
.L_0200d040:
	mov r1, r11
	movs r0, #2
	ldrsh r3, [r1, r0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	cmp r3, #0
	beq .L_0200d062
	movs r2, #255
	lsls r2, r2, #24
	movs r0, #152
	adds r3, r4, r2
	lsls r0, r0, #17
	cmp r3, r0
	bls .L_0200d062
	movs r3, #132
	lsls r3, r3, #18
	subs r6, r3, r6
.L_0200d062:
	ldr r3, .L_0200d0f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200d07c
	movs r2, #152
	lsls r2, r2, #17
	adds r6, r6, r2
	b .L_0200d080
.L_0200d07c:
	ldr r3, .L_0200d0fc
	adds r6, r6, r3
.L_0200d080:
	ldr r1, [r7, #8]
	cmp r1, r6
	bne .L_0200d08c
	ldr r3, [r7, #16]
	cmp r3, r4
	beq .L_0200d0c8
.L_0200d08c:
	ldr r3, [r7, #16]
	subs r1, r6, r1
	subs r0, r4, r3
	str r4, [sp, #0]
	bl ArcTan2
	ldrh r3, [r7, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	movs r2, #128
	asrs r0, r0, #16
	lsls r2, r2, #5
	ldr r4, [sp, #0]
	cmp r0, r2
	ble .L_0200d0b0
	adds r0, r2, #0
.L_0200d0b0:
	ldr r2, .L_0200d100
	cmp r0, r2
	bge .L_0200d0b8
	adds r0, r2, #0
.L_0200d0b8:
	adds r3, r3, r0
	movs r2, #0
	mov r0, r11
	strh r3, [r7, #6]
	str r6, [r7, #8]
	str r4, [r7, #16]
	strh r2, [r0, #8]
	b .L_0200d0d2
.L_0200d0c8:
	mov r1, r11
	ldrh r3, [r1, #8]
	mov r2, r11
	adds r3, #1
	strh r3, [r2, #8]
.L_0200d0d2:
	mov r1, r11
	movs r0, #8
	ldrsh r3, [r1, r0]
	cmp r3, #2
	ble .L_0200d0e6
	adds r0, r7, #0
	movs r1, #1
	bl Func_020057c8
	b .L_0200d190
.L_0200d0e6:
	adds r0, r7, #0
	movs r1, #5
	bl Func_020057c8
	b .L_0200d190
.L_0200d0f0:
	.4byte gSceneState
.L_0200d0f4:
	.4byte gMainRoutineWork
.L_0200d0f8:
	.4byte 0xffff0000
.L_0200d0fc:
	.4byte 0xfed00000
.L_0200d100:
	.4byte 0xfffff000
.L_0200d104:
	.4byte gLinkPeerSignatures
.L_0200d108:
	.4byte gLinkStatus
.L_0200d10c:
	.4byte 0xfff40000
.L_0200d110:
	.4byte 0xfff00000
.L_0200d114:
	cmp r2, #2
	bne .L_0200d190
	mov r1, r11
	movs r3, #18
	ldrsh r4, [r7, r3]
	movs r0, #6
	ldrsh r3, [r1, r0]
	movs r2, #10
	ldrsh r6, [r7, r2]
	lsls r3, r3, #1
	adds r2, r3, #0
	mov r0, r8
	adds r2, #244
	strh r6, [r0, r2]
	adds r3, #246
	mov r1, r8
	movs r0, #130
	strh r4, [r1, r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d14c
	mov r2, r11
	ldrh r3, [r2, #6]
	mov r0, r11
	adds r3, #2
	strh r3, [r0, #6]
.L_0200d14c:
	mov r1, r11
	ldrh r3, [r1, #6]
	movs r0, #6
	ldrsh r2, [r1, r0]
	movs r1, #224
	lsls r1, r1, #5
	adds r1, #30
	cmp r2, r1
	bne .L_0200d190
	ldr r1, .L_0200d18c
	adds r3, #1
	lsls r2, r2, #1
	lsls r3, r3, #16
	adds r2, #244
	mov r0, r8
	asrs r3, r3, #15
	strh r1, [r0, r2]
	adds r3, #244
	mov r2, r8
	strh r1, [r2, r3]
	mov r3, r8
	adds r3, #228
	ldrh r3, [r3]
	movs r2, #0
	mov r0, r11
	mov r1, r11
	strh r3, [r0, #4]
	strh r2, [r1, #6]
	movs r3, #1
.L_0200d186:
	mov r2, r11
	strh r3, [r2]
	b .L_0200d190
.L_0200d18c:
	.4byte 0x00000000
.L_0200d190:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200d1a0,"ax",%progbits
	.balign 4
	.global Func_020051a0
	.thumb_func
Func_020051a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r1, #0
	adds r3, #236
	ldr r3, [r3]
	sub sp, #40
	str r3, [sp, #36]
	str r3, [sp, #32]
	adds r7, r3, #0
	adds r7, #216
	adds r0, r3, #0
	movs r4, #0
	ldrsh r3, [r7, r4]
	ldr r2, .L_0200d508
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r11, r0
	lsrs r3, r3, #5
	str r3, [sp, #20]
	adds r3, r0, #0
	adds r3, #224
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r5, r11
	ldr r6, [r1, #108]
	adds r5, #218
	cmp r3, #0
	beq .L_0200d1ee
	movs r3, #2
	strh r3, [r5]
	b .L_0200d26a
.L_0200d1ee:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d208
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200d218
.L_0200d208:
	movs r4, #0
	ldrsh r3, [r5, r4]
	ldrh r2, [r5]
	cmp r3, #0
	ble .L_0200d26a
	subs r3, r2, #1
	strh r3, [r5]
	b .L_0200d26a
.L_0200d218:
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #1
	bgt .L_0200d26a
	adds r3, r2, #1
	movs r1, #128
	strh r3, [r5]
	lsls r1, r1, #9
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_0200d26a
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0200d50c
	ldr r1, .L_0200d510
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #144
	lsls r0, r0, #2
	bl Heap_AllocEwram
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200d514
	bl Func_02005770
	movs r1, #144
	movs r2, #0
	ldrsh r0, [r7, r2]
	lsls r1, r1, #2
	adds r2, r5, #0
	bl VramCache_Load
	adds r0, r5, #0
	bl Heap_Release
.L_0200d26a:
	ldr r3, [sp, #36]
	adds r3, #218
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_0200d284
	ldr r3, [sp, #36]
	adds r3, #216
	movs r7, #0
	ldrsh r0, [r3, r7]
	bl VramCache_ReleaseVram
	b .L_0200d5b2
.L_0200d284:
	movs r0, #0
	str r0, [sp, #28]
.L_0200d288:
	mov r1, r11
	adds r1, #222
	str r1, [sp, #12]
	mov r4, r11
	movs r2, #0
	ldrsh r3, [r1, r2]
	adds r4, #220
	adds r7, r3, #0
	adds r7, #8
	movs r3, #255
	ands r7, r3
	ldr r3, [sp, #28]
	subs r1, #4
	cmp r3, #0
	bne .L_0200d2cc
	movs r0, #0
	ldrsh r2, [r1, r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r4, r1]
	lsls r3, r3, #1
	adds r3, r3, r2
	subs r3, #12
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r9
	ands r2, r3
	movs r3, #0
	mov r9, r2
	str r3, [sp, #16]
	b .L_0200d2f4
.L_0200d2cc:
	movs r0, #0
	ldrsh r2, [r1, r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r4, r1]
	lsls r3, r3, #1
	negs r3, r3
	subs r3, r3, r2
	adds r3, #236
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r9
	ands r2, r3
	movs r3, #128
	lsls r3, r3, #21
	str r3, [sp, #16]
	mov r9, r2
.L_0200d2f4:
	movs r4, #0
	str r4, [sp, #24]
	cmp r7, #159
	bgt .L_0200d37a
	ldr r4, [sp, #36]
.L_0200d2fe:
	movs r0, #0
	str r0, [r4]
	ldr r2, [sp, #16]
	mov r1, r9
	lsls r6, r1, #16
	adds r3, r7, #0
	orrs r3, r6
	orrs r3, r2
	ldr r2, .L_0200d518
	movs r5, #228
	orrs r3, r2
	str r3, [r4, #4]
	ldr r3, [sp, #20]
	lsls r5, r5, #8
	orrs r3, r5
	str r3, [r4, #8]
	ldr r3, [sp, #32]
	mov r8, r0
	adds r3, #12
	ldr r0, [sp, #32]
	movs r1, #255
	mov r10, r3
	str r4, [sp, #4]
	bl Func_02005790
	ldr r4, [sp, #4]
	mov r0, r8
	str r0, [r4, #12]
	ldr r1, [sp, #16]
	adds r3, r7, #0
	adds r3, #32
	orrs r3, r6
	movs r2, #128
	orrs r3, r1
	lsls r2, r2, #7
	orrs r3, r2
	str r3, [r4, #16]
	ldr r3, [sp, #20]
	mov r0, r10
	adds r3, #8
	orrs r3, r5
	str r3, [r4, #20]
	ldr r2, [sp, #36]
	ldr r3, [sp, #32]
	adds r4, #24
	adds r2, #24
	adds r3, #24
	movs r1, #255
	str r4, [sp, #4]
	str r2, [sp, #36]
	str r3, [sp, #32]
	bl Func_02005790
	ldr r0, [sp, #24]
	adds r7, #36
	adds r0, #1
	str r0, [sp, #24]
	ldr r4, [sp, #4]
	cmp r0, #3
	bhi .L_0200d37a
	cmp r7, #159
	ble .L_0200d2fe
.L_0200d37a:
	ldr r1, [sp, #28]
	adds r1, #1
	str r1, [sp, #28]
	cmp r1, #1
	bls .L_0200d288
	ldr r3, .L_0200d51c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #4
	bhi .L_0200d392
	b .L_0200d5b2
.L_0200d392:
	mov r3, r11
	adds r3, #226
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200d492
	ldr r3, .L_0200d520
	ldr r7, [sp, #36]
	movs r4, #241
	ldr r0, [sp, #32]
	lsls r4, r4, #1
	adds r3, r3, r4
	adds r7, #4
	ldrh r3, [r3]
	adds r0, #12
	mov r10, r7
	ldr r7, [sp, #20]
	str r0, [sp, #8]
	movs r2, #1
	ands r2, r3
	mov r1, r11
	mov r4, r11
	mov r3, r11
	mov r6, r11
	adds r7, #10
	adds r1, #218
	adds r4, #220
	adds r3, #236
	ldr r0, [r5, #8]
	adds r6, #240
	mov r8, r7
	cmp r2, #0
	beq .L_0200d400
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d3e6
	ldr r0, .L_0200d524
	adds r2, r2, r0
.L_0200d3e6:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #0
	adds r2, r2, r3
	subs r2, #19
	b .L_0200d424
.L_0200d400:
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d40c
	ldr r0, .L_0200d524
	adds r2, r2, r0
.L_0200d40c:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	subs r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #0
	subs r2, r2, r3
	adds r2, #232
.L_0200d424:
	mov r9, r2
	bl Func_02002ce8
	ldr r2, [r5, #16]
	ldr r3, [r6]
	adds r1, r0, #0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_0200d43a
	ldr r2, .L_0200d524
	adds r0, r0, r2
.L_0200d43a:
	ldr r7, [sp, #12]
	asrs r2, r0, #20
	movs r4, #0
	ldrsh r3, [r7, r4]
	adds r2, r2, r3
	lsls r3, r1, #3
	adds r3, r3, r1
	lsls r3, r3, #2
	subs r2, r2, r3
	adds r7, r2, #4
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, [sp, #36]
	adds r3, #255
	mov r0, r9
	ands r0, r3
	movs r3, #255
	ands r7, r3
	movs r3, #0
	str r3, [r1]
	lsls r3, r0, #16
	orrs r7, r3
	movs r3, #128
	mov r2, r10
	lsls r3, r3, #23
	adds r0, r2, #0
	orrs r7, r3
	stmia r0!, {r7}
	movs r3, #228
	lsls r3, r3, #8
	mov r1, r8
	adds r4, r0, #0
	orrs r1, r3
	str r4, [sp, #36]
	stmia r0!, {r1}
	ldr r3, [sp, #32]
	ldr r4, [sp, #8]
	adds r2, r0, #0
	movs r1, #255
	adds r0, r3, #0
	str r2, [sp, #36]
	str r4, [sp, #32]
	bl Func_02005790
.L_0200d492:
	mov r3, r11
	adds r3, #228
	movs r7, #0
	ldrsh r0, [r3, r7]
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200d4a6
	b .L_0200d5b2
.L_0200d4a6:
	ldr r0, [r5, #8]
	cmp r0, #0
	bne .L_0200d4ae
	b .L_0200d5b2
.L_0200d4ae:
	ldr r3, .L_0200d520
	movs r1, #241
	ldr r7, [sp, #36]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r3, [r3]
	adds r7, #4
	mov r10, r7
	ldr r7, [sp, #20]
	movs r2, #1
	ands r2, r3
	mov r3, r11
	adds r3, #222
	mov r1, r11
	mov r4, r11
	mov r6, r11
	adds r7, #14
	str r3, [sp, #0]
	adds r1, #218
	adds r4, #220
	adds r3, #14
	adds r6, #240
	mov r8, r7
	cmp r2, #0
	beq .L_0200d528
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d4ec
	ldr r0, .L_0200d524
	adds r2, r2, r0
.L_0200d4ec:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #1
	adds r2, r2, r3
	adds r2, #189
	b .L_0200d54c
	.2byte 0x0000
.L_0200d508:
	.4byte gResourceTableEntries
.L_0200d50c:
	.4byte Data_020062b4
.L_0200d510:
	.4byte 0x050003c0
.L_0200d514:
	.4byte gData_020062d4
.L_0200d518:
	.4byte 0x80008000
.L_0200d51c:
	.4byte gFrameCount
.L_0200d520:
	.4byte gMainRoutineWork
.L_0200d524:
	.4byte 0x000fffff
.L_0200d528:
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d534
	ldr r0, .L_0200d5c0
	adds r2, r2, r0
.L_0200d534:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	subs r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #1
	adds r2, r2, r3
	adds r2, #23
.L_0200d54c:
	mov r9, r2
	bl Func_02002ce8
	ldr r2, [r5, #16]
	ldr r3, [r6]
	adds r1, r0, #0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_0200d562
	ldr r2, .L_0200d5c0
	adds r0, r0, r2
.L_0200d562:
	ldr r7, [sp, #0]
	asrs r2, r0, #20
	movs r4, #0
	ldrsh r3, [r7, r4]
	adds r2, r2, r3
	lsls r3, r1, #3
	adds r3, r3, r1
	lsls r3, r3, #2
	subs r2, r2, r3
	adds r7, r2, #4
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, [sp, #36]
	adds r3, #255
	mov r0, r9
	ands r0, r3
	movs r3, #255
	ands r7, r3
	movs r3, #0
	str r3, [r1]
	lsls r3, r0, #16
	orrs r7, r3
	movs r3, #128
	lsls r3, r3, #23
	mov r2, r10
	adds r0, r2, #0
	orrs r7, r3
	stmia r0!, {r7}
	movs r3, #228
	lsls r3, r3, #8
	mov r1, r8
	orrs r1, r3
	adds r4, r0, #0
	str r4, [sp, #36]
	str r1, [r0]
	ldr r3, [sp, #32]
	movs r1, #255
	adds r0, r3, #0
	bl Func_02005790
.L_0200d5b2:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200d5c0:
	.4byte 0x000fffff
	.section .bss,"aw",%nobits
	.global gData_02007e28
gData_02007e28:
