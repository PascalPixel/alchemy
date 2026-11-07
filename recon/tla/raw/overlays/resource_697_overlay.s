.syntax unified
	.thumb
	.section .text.x02009262,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200929a,"ax",%progbits
	.2byte 0x0000
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
