.syntax unified
	.thumb
	.global Func_081a68b4
	.thumb_func
Func_081a68b4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #172
	ldr r3, [r3]
	adds r5, r0, #0
	mov r8, r3
	movs r0, #4
	ldrsh r3, [r5, r0]
	ldrh r1, [r5, #4]
	cmp r3, #0
	bgt .L_081a68d2
	b .L_081a69f6
.L_081a68d2:
	movs r1, #0
	ldrsh r7, [r5, r1]
	movs r2, #2
	ldrsh r3, [r5, r2]
	cmp r7, r3
	blt .L_081a68e0
	b .L_081a6a9c
.L_081a68e0:
	ldr r0, [r5, #28]
	bl Func_081a6094
	cmp r0, #0
	beq .L_081a68ec
	b .L_081a69f2
.L_081a68ec:
	movs r0, #22
	ldrsh r3, [r5, r0]
	adds r2, r3, #0
	cmp r3, #0
	ble .L_081a6978
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, .L_081a6970
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r5, #8]
	movs r2, #240
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	adds r6, r5, #0
	adds r3, r3, r2
	str r3, [r5, #12]
	adds r6, #16
	adds r0, r6, #0
	movs r4, #29
.L_081a691e:
	ldrh r2, [r5, #20]
	ldr r1, [r0]
	adds r3, r2, #1
	strh r3, [r5, #20]
	subs r4, #1
	strh r2, [r1]
	adds r1, #2
	str r1, [r0]
	cmp r4, #0
	bge .L_081a691e
	ldr r3, [r6]
	ldr r2, .L_081a696c
	movs r1, #128
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	str r3, [r6]
	lsls r1, r1, #14
	ldrh r3, [r5, #24]
	movs r2, #0
	adds r3, #1
	strh r3, [r5, #24]
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_081a6960
	movs r3, #192
	lsls r3, r3, #19
	str r3, [r5, #12]
	ldr r3, .L_081a6974
	strh r2, [r5, #24]
	strh r2, [r5, #20]
	str r3, [r6]
.L_081a6960:
	ldrh r3, [r5, #22]
	subs r3, #8
	strh r3, [r5, #22]
	adds r2, r3, #0
	b .L_081a6978
	.2byte 0x0000
.L_081a696c:
	.4byte 0x000001ff
.L_081a6970:
	.4byte 0x840001e0
.L_081a6974:
	.4byte 0x0600f800
.L_081a6978:
	ldrh r1, [r5, #4]
	adds r3, r2, r1
	ldr r2, .L_081a6aa8
	strh r3, [r5, #22]
	ldrh r3, [r2, #6]
	adds r3, r3, r1
	strh r3, [r2, #6]
	mov r3, r8
	ldrh r2, [r3, #10]
	movs r0, #10
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_081a69b4
	subs r3, r2, #1
	mov r1, r8
	movs r2, #128
	strh r3, [r1, #10]
	lsls r2, r2, #9
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_081a69b4
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_081a81d4
	movs r0, #10
	bl Func_081a8228
.L_081a69b4:
	mov r3, r8
	ldrh r2, [r3, #12]
	movs r0, #12
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_081a69e0
	subs r3, r2, #1
	mov r1, r8
	movs r2, #128
	strh r3, [r1, #12]
	lsls r2, r2, #9
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_081a69e0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_081a81d4
	movs r0, #10
	bl Func_081a8228
.L_081a69e0:
	movs r0, #4
	ldrsh r3, [r5, r0]
	adds r7, r7, r3
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r7, r3
	bge .L_081a69f0
	b .L_081a68e0
.L_081a69f0:
	b .L_081a6a9c
.L_081a69f2:
	movs r0, #1
	b .L_081a6aa0
.L_081a69f6:
	movs r2, #0
	ldrsh r7, [r5, r2]
	movs r0, #2
	ldrsh r3, [r5, r0]
	cmp r7, r3
	ble .L_081a6a9c
.L_081a6a02:
	ldr r3, .L_081a6aa8
	ldrh r2, [r3, #6]
	adds r2, r2, r1
	strh r2, [r3, #6]
	ldrh r3, [r5, #22]
	subs r3, r3, r1
	movs r1, #128
	strh r3, [r5, #22]
	lsls r1, r1, #12
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_081a6a80
	ldrh r3, [r5, #24]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r5, #24]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_081a6a7a
	movs r3, #1
	strh r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, .L_081a6aac
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r5, #8]
	ldr r2, .L_081a6ab0
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	adds r0, #16
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r4, #29
.L_081a6a54:
	ldrh r2, [r5, #20]
	ldr r1, [r0]
	adds r3, r2, #1
	strh r3, [r5, #20]
	subs r4, #1
	strh r2, [r1]
	adds r1, #2
	str r1, [r0]
	cmp r4, #0
	bge .L_081a6a54
	ldr r3, [r5, #16]
	movs r0, #255
	subs r3, #60
	str r3, [r5, #16]
	ldrh r3, [r5, #20]
	lsls r0, r0, #8
	adds r0, #226
	adds r3, r3, r0
	strh r3, [r5, #20]
.L_081a6a7a:
	ldrh r3, [r5, #22]
	subs r3, #8
	strh r3, [r5, #22]
.L_081a6a80:
	ldr r0, [r5, #28]
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a69f2
	ldrh r3, [r5, #4]
	adds r1, r3, #0
	lsls r3, r1, #16
	asrs r3, r3, #16
	adds r7, r7, r3
	movs r2, #2
	ldrsh r3, [r5, r2]
	cmp r7, r3
	bgt .L_081a6a02
.L_081a6a9c:
	strh r7, [r5]
	movs r0, #0
.L_081a6aa0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a6aa8:
	.4byte Data_03001120
.L_081a6aac:
	.4byte 0x840001e0
.L_081a6ab0:
	.4byte 0xfffff880
