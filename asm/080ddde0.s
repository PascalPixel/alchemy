@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd594, 0x080cd594
	.set sub_080d6888, 0x080d6888
	.set sub_080de0d4, 0x080de0d4
	.set sub_080de0da, 0x080de0da
	.set sub_080de1f0, 0x080de1f0
	.set sub_080de2a0, 0x080de2a0
	.set sub_080e0524, 0x080e0524
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080ddde0
Region_080ddde0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #668]
	adds r3, r5, #0
	ldmia r3!, {r1}
	ldr r3, [r3, #0]
	sub sp, #64
	str r3, [sp, #48]
	ldr r3, [pc, #660]
	mov r11, r1
	ldr r2, [r5, #8]
	add r3, r11
	str r2, [sp, #28]
	str r0, [r3, #0]
	movs r0, #1
	bl sub_080cd594
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl sub_080ed408
	ldr r5, [r5, #28]
	ldr r0, [pc, #628]
	mov r1, r11
	movs r2, #1
	movs r3, #0
	str r5, [sp, #32]
	bl sub_080e0524
	ldr r1, [pc, #616]
	ldr r0, [pc, #620]
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	movs r2, #0
	ldr r1, [sp, #28]
	movs r3, #0
	ldr r0, [pc, #604]
	bl sub_080e0524
	movs r3, #0
	mov r10, r3
	movs r2, #128
	ldr r3, [pc, #596]
	movs r1, #0
	lsls r2, r2, #3
.L_080dde52:
	movs r4, #1
	add r10, r4
	str r1, [r3, #0]
	adds r3, #28
	cmp r10, r2
	bne .L_080dde52
	ldr r3, [pc, #584]
	movs r0, #0
	movs r2, #1
	mov r10, r0
	negs r2, r2
	add r3, r11
.L_080dde6a:
	movs r1, #1
	add r10, r1
	mov r4, r10
	str r2, [r3, #0]
	adds r3, #28
	cmp r4, #64
	bne .L_080dde6a
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2, #0]
	ldr r2, [pc, #552]
	movs r3, #75
	add r2, r11
	movs r1, #144
	str r3, [r2, #0]
	lsls r1, r1, #3
	ldr r0, [pc, #544]
	bl sub_080041d8
	movs r0, #138
	bl sub_080f9010
	movs r0, #0
	ldr r3, [pc, #496]
	str r0, [sp, #40]
	add r3, r11
	ldr r3, [r3, #0]
	ldr r3, [r3, #20]
	movs r1, #40
	lsls r3, r3, #3
	negs r1, r1
	cmp r3, r1
	bne .L_080ddeb2
	b sub_080de2a0
.L_080ddeb2:
	ldr r2, [pc, #476]
	add r2, r11
	str r2, [sp, #20]
	ldr r3, [sp, #40]
	cmp r3, #24
	bne .L_080ddec4
	movs r0, #133
	bl sub_080b50e8
.L_080ddec4:
	movs r4, #0
	str r4, [sp, #44]
	ldr r0, [sp, #20]
	ldr r3, [r0, #0]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080ddefc
	ldr r5, [pc, #480]
.L_080dded4:
	ldr r1, [sp, #44]
	ldr r2, [sp, #40]
	lsls r3, r1, #3
	cmp r2, r3
	bne .L_080ddeea
	movs r1, #128
	ldr r0, [sp, #48]
	lsls r1, r1, #7
	ldr r2, [pc, #464]
	bl sub_080072f8
.L_080ddeea:
	ldr r3, [sp, #44]
	adds r3, #1
	str r3, [sp, #44]
	ldr r4, [sp, #20]
	ldr r3, [r4, #0]
	ldr r0, [sp, #44]
	ldr r3, [r3, #20]
	cmp r0, r3
	bne .L_080dded4
.L_080ddefc:
	movs r1, #0
	str r1, [sp, #44]
	ldr r2, [pc, #396]
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080ddf0e
	b sub_080de1f0
.L_080ddf0e:
	mov r0, sp
	adds r0, #52
	movs r1, #36
	movs r3, #0
	str r0, [sp, #24]
	str r1, [sp, #16]
	str r3, [sp, #12]
	mov r0, r11
	adds r5, r0, r2
	ldr r3, [r5, #0]
	ldr r1, [sp, #16]
	ldr r4, [sp, #44]
	ldrsh r0, [r3, r1]
	lsls r4, r4, #3
	ldr r1, [sp, #24]
	mov r8, r4
	bl sub_080e3980
	ldr r4, [sp, #24]
	ldr r3, [r4, #0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r4, #0]
	ldr r0, [sp, #40]
	mov r3, r8
	adds r3, #1
	cmp r0, r3
	bne .L_080ddf50
	ldr r2, [pc, #368]
	movs r3, #4
	add r2, r11
	str r3, [r2, #0]
.L_080ddf50:
	mov r3, r8
	ldr r1, [sp, #40]
	adds r3, #4
	cmp r1, r3
	bne .L_080ddf7a
	ldr r3, [r5, #0]
	ldr r2, [sp, #16]
	ldrsh r0, [r3, r2]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #44]
	bl sub_080d6888
	ldr r3, [r5, #0]
	ldr r1, [sp, #16]
	ldrsh r0, [r3, r1]
	movs r1, #6
	bl sub_080b5088
.L_080ddf7a:
	movs r3, #2
	ldr r4, [sp, #40]
	add r3, r8
	mov r9, r3
	cmp r4, r8
	bge .L_080ddf88
	b sub_080de0da
.L_080ddf88:
	mov r3, r8
	adds r3, #16
	cmp r4, r3
	blt .L_080ddf92
	b sub_080de0d4
.L_080ddf92:
	mov r0, r8
	subs r3, r4, r0
	lsls r6, r3, #6
	cmp r6, #104
	ble .L_080ddf9e
	movs r6, #104
.L_080ddf9e:
	ldr r3, [r5, #0]
	ldr r3, [r3, #24]
	ldr r7, [pc, #284]
	lsls r3, r3, #2
	adds r3, #3
	adds r2, r7, #0
	ldrb r3, [r2, r3]
	movs r1, #0
	mov r10, r1
	cmp r3, #0
	beq .L_080de004
	ldr r3, [sp, #44]
	ldr r4, [sp, #40]
	mov r9, r7
	adds r5, r3, r4
.L_080ddfbc:
	mov r0, r10
	adds r3, r5, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #3
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r1, r2, #4
	subs r1, r1, r2
	lsls r1, r1, #6
	ldr r2, [pc, #192]
	add r1, r11
	adds r1, r1, r2
	ldr r2, [sp, #52]
	movs r3, #24
	subs r2, #12
	str r3, [sp, #0]
	str r6, [sp, #4]
	movs r3, #0
	ldr r0, [sp, #48]
	ldr r4, [sp, #32]
	bl sub_080072f4
	ldr r1, [sp, #20]
	ldr r3, [r1, #0]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	adds r3, #3
	mov r2, r9
	movs r0, #1
	ldrb r3, [r2, r3]
	add r10, r0
	cmp r10, r3
	bne .L_080ddfbc
.L_080de004:
	movs r3, #2
	add r3, r8
	ldr r4, [sp, #40]
	mov r9, r3
	cmp r4, r9
	bne sub_080de0da
	ldr r1, [sp, #20]
	ldr r3, [r1, #0]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	ldrb r3, [r7, r3]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq sub_080de0da
	ldr r2, [sp, #12]
	ldr r3, [pc, #156]
	adds r7, r2, r3
.L_080de028:
	bl sub_08004458
	ldr r6, [pc, #152]
	ands r6, r0
	bl sub_08004458
	ldr r3, [sp, #52]
	ldr r5, [pc, #148]
	ldr r4, [pc, #148]
	lsls r3, r3, #16
	str r3, [r7, #0]
	ands r5, r0
	movs r3, #208
	adds r5, r5, r4
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl sub_08002322
	adds r6, #64
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r5, #0
	bl sub_0800231c
	adds r3, r6, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl sub_08004458
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	ldr r1, [sp, #20]
	ldr r3, [r1, #0]
	ldr r3, [r3, #24]
	ldr r2, [pc, #68]
	lsls r3, r3, #2
	movs r0, #1
	ldrb r3, [r2, r3]
	add r10, r0
	adds r7, #28
	cmp r10, r3
	bne .L_080de028
	b sub_080de0da
