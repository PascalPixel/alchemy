.syntax unified
	.thumb
	.global Func_080cd91c
	.thumb_func
Func_080cd91c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #1
	sub sp, #4
	negs r1, r1
	movs r2, #32
	mov r9, r0
	str r1, [sp, #0]
	mov r11, r2
	bl ObjectTable_Get
	adds r7, r0, #0
	cmp r7, #0
	beq .L_080cda3a
	movs r3, #0
	mov r10, r3
.L_080cd946:
	cmp r10, r9
	beq .L_080cda30
	mov r0, r10
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080cda30
	movs r1, #89
	adds r1, r1, r6
	ldrb r2, [r1]
	movs r3, #8
	ands r3, r2
	mov r8, r1
	cmp r3, #0
	bne .L_080cda30
	ldr r4, [r6, #12]
	ldr r1, [r7, #12]
	subs r3, r4, r1
	cmp r3, #0
	blt .L_080cd978
	ldr r2, .L_080cda4c
	cmp r3, r2
	ble .L_080cd980
	b .L_080cda30
.L_080cd978:
	ldr r2, .L_080cda4c
	subs r3, r1, r4
	cmp r3, r2
	bgt .L_080cda30
.L_080cd980:
	ldr r2, [r6, #8]
	ldr r3, [r7, #8]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080cd992
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080cd992:
	subs r2, r4, r1
	asrs r0, r0, #16
	cmp r2, #0
	bge .L_080cd9a2
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r2, r2, r1
.L_080cd9a2:
	asrs r1, r2, #16
	ldr r3, [r7, #16]
	ldr r2, [r6, #16]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080cd9b6
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080cd9b6:
	asrs r3, r2, #16
	adds r2, r0, #0
	muls r2, r0
	adds r0, r2, #0
	adds r2, r1, #0
	muls r2, r1
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_080cda50
	mov lr, r3
	.2byte 0xf800
	mov r2, r10
	adds r5, r0, #0
	cmp r2, #63
	ble .L_080cd9dc
	lsls r5, r5, #1
.L_080cd9dc:
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080cd9f6
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #1
	movs r1, #13
	bl __divsi3
	adds r5, r0, #0
.L_080cd9f6:
	cmp r5, r11
	bge .L_080cda30
	ldr r3, [r7, #16]
	ldr r0, [r6, #16]
	ldr r1, [r6, #8]
	subs r0, r0, r3
	ldr r3, [r7, #8]
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	cmp r5, #11
	ble .L_080cda2a
	ldrh r3, [r7, #6]
	ldr r1, .L_080cda54
	subs r3, r0, r3
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, r1
	blt .L_080cda30
	movs r2, #188
	lsls r2, r2, #6
	adds r2, #255
	cmp r0, r2
	bgt .L_080cda30
.L_080cda2a:
	mov r3, r10
	str r3, [sp, #0]
	mov r11, r5
.L_080cda30:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #80
	ble .L_080cd946
.L_080cda3a:
	ldr r0, [sp, #0]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cda4c:
	.4byte 0x002fffff
.L_080cda50:
	.4byte IwramFillWords + 0x74
.L_080cda54:
	.4byte 0xffffd001
