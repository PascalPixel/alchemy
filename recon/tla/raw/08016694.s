.syntax unified
	.thumb
	.global Func_08016694
	.thumb_func
Func_08016694:
	push {r5, r6, r7, lr}
	ldr r3, .L_08016710
	sub sp, #8
	ldr r4, [r3, #4]
	ldr r3, [r3]
	ldr r0, .L_08016714
	mov lr, sp
	mov r2, lr
	str r3, [r2]
	str r4, [r2, #4]
	ldr r3, [r0]
	ldr r1, .L_08016718
	lsls r3, r3, #25
	lsrs r3, r3, #31
	ldr r2, [r1, #20]
	strb r3, [r1, #9]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	bne .L_080166ce
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #254
	ldr r2, [r1, #44]
	strh r3, [r0, #2]
	ldr r3, [r1, #40]
	str r2, [r1, #40]
	str r3, [r1, #44]
	b .L_080166da
.L_080166ce:
	cmp r2, #0
	blt .L_080166da
	ldr r3, [r1, #44]
	lsls r2, r2, #1
	ldrh r3, [r2, r3]
	strh r3, [r0, #2]
.L_080166da:
	ldr r7, .L_08016718
	ldr r3, [r7, #20]
	cmp r3, #14
	bgt .L_080166e6
	adds r3, #1
	str r3, [r7, #20]
.L_080166e6:
	movs r6, #0
.L_080166e8:
	mov r2, lr
	lsls r5, r6, #1
	ldrh r3, [r2, r5]
	movs r2, #254
	lsls r2, r2, #8
	adds r2, #254
	cmp r3, r2
	bne .L_0801671c
	lsls r3, r6, #2
	mov r12, r3
	mov r2, r12
	adds r2, #24
	ldr r3, [r7, r2]
	cmp r3, #13
	ble .L_08016720
	movs r3, #1
	negs r3, r3
	str r3, [r7, r2]
	b .L_0801674c
	.2byte 0x0000
.L_08016710:
	.4byte 0x04000120
.L_08016714:
	.4byte 0x04000128
.L_08016718:
	.4byte Data_02005360
.L_0801671c:
	lsls r2, r6, #2
	mov r12, r2
.L_08016720:
	mov r3, r12
	adds r3, #24
	mov r4, r12
	ldr r1, [r7, r3]
	adds r4, #48
	mov r3, lr
	ldr r0, [r7, r4]
	ldrh r5, [r3, r5]
	lsls r2, r1, #1
	strh r5, [r2, r0]
	cmp r1, #13
	bne .L_0801674c
	mov r3, r12
	adds r3, #64
	ldr r2, [r7, r3]
	adds r1, r6, #4
	str r0, [r7, r3]
	str r2, [r7, r4]
	ldrb r3, [r7, r1]
	movs r2, #1
	orrs r3, r2
	strb r3, [r7, r1]
.L_0801674c:
	ldr r0, .L_0801679c
	ldrb r3, [r0, #9]
	cmp r3, #0
	beq .L_0801675e
	adds r3, r6, #4
	ldrb r2, [r0, r3]
	movs r1, #2
	orrs r2, r1
	strb r2, [r0, r3]
.L_0801675e:
	mov r2, r12
	adds r2, #24
	ldr r3, [r0, r2]
	cmp r3, #14
	bgt .L_0801676c
	adds r3, #1
	str r3, [r0, r2]
.L_0801676c:
	adds r6, #1
	adds r7, r0, #0
	cmp r6, #1
	ble .L_080166e8
	ldrb r3, [r0]
	cmp r3, #8
	bne .L_080167a8
	ldr r0, .L_080167a0
	ldr r3, .L_08016790
	ldr r1, .L_080167a4
	strh r3, [r0]
	ldr r2, .L_08016794
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	ldr r3, .L_08016798
	strh r3, [r0]
	b .L_080167a8
.L_08016790:
	.4byte 0x00000000
.L_08016794:
	.4byte 0x00000080
.L_08016798:
	.4byte 0x000000c0
.L_0801679c:
	.4byte Data_02005360
.L_080167a0:
	.4byte 0x0400010e
.L_080167a4:
	.4byte 0x04000128
.L_080167a8:
	add sp, #8
	pop {r5, r6, r7, pc}
