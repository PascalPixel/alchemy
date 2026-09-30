.syntax unified
	.thumb
	.global Func_080cad9c
	.thumb_func
Func_080cad9c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080cae58
	movs r3, #128
	lsls r3, r3, #5
	adds r3, r3, r6
	mov r11, r3
	movs r3, #129
	lsls r3, r3, #5
	adds r3, r3, r6
	mov r9, r3
	movs r3, #130
	lsls r3, r3, #5
	adds r3, r3, r6
	adds r7, r6, #0
	mov r10, r3
	movs r3, #0
	subs r7, #32
	mov r8, r3
	movs r5, #0
.L_080cadce:
	adds r0, r5, #0
	bl ObjectTable_Get
	adds r4, r0, #0
	cmp r4, #0
	beq .L_080cae30
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	strb r5, [r7]
	adds r3, #212
	adds r7, #1
	adds r1, r6, #0
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r3, r4, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080cae08
	ldr r3, [r4, #80]
	ldrb r2, [r3, #24]
	ldrb r1, [r3, #26]
	ldrb r3, [r3, #9]
	lsls r3, r3, #28
	lsrs r0, r3, #30
	b .L_080cae0e
.L_080cae08:
	movs r2, #0
	movs r1, #0
	movs r0, #0
.L_080cae0e:
	mov r3, r11
	strb r2, [r3]
	movs r3, #1
	add r11, r3
	mov r3, r9
	strb r1, [r3]
	movs r3, #1
	add r9, r3
	mov r3, r10
	strb r0, [r3]
	movs r3, #1
	add r8, r3
	add r10, r3
	mov r3, r8
	adds r6, #128
	cmp r3, #31
	bhi .L_080cae36
.L_080cae30:
	adds r5, #1
	cmp r5, #80
	blt .L_080cadce
.L_080cae36:
	mov r5, r8
	cmp r5, #31
	bgt .L_080cae4c
	movs r3, #32
	movs r2, #255
	subs r5, r3, r5
.L_080cae42:
	subs r5, #1
	strb r2, [r7]
	adds r7, #1
	cmp r5, #0
	bne .L_080cae42
.L_080cae4c:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080cae58:
	.4byte Data_02001024
