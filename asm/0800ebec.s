@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_0800447c, 0x0800447c
	.set sub_0800ee14, 0x0800ee14
	.set sub_0800ef44, 0x0800ef44
	.set sub_0800f0c6, 0x0800f0c6
	.set sub_080120dc, 0x080120dc
	.set sub_080770c0, 0x080770c0
	.set sub_080f9010, 0x080f9010
	.global Region_0800ebec
Region_0800ebec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #20]
	str r0, [sp, #16]
	ldr r5, [pc, #488]
	ldrb r3, [r5, #0]
	cmp r3, #0
	beq .L_0800ec40
	movs r0, #175
	lsls r0, r0, #1
	bl sub_080770c0
	cmp r0, #0
	beq .L_0800ec40
	movs r2, #128
	ldr r1, [pc, #472]
	movs r0, #0
	lsls r2, r2, #2
.L_0800ec20:
	ldrb r3, [r1, #0]
	adds r1, #1
	cmp r3, #255
	bne .L_0800ec2a
	adds r0, #1
.L_0800ec2a:
	subs r2, #1
	cmp r2, #0
	bne .L_0800ec20
	adds r3, r0, #0
	subs r3, #136
	cmp r3, #0
	bge .L_0800ec3e
	movs r0, #135
	bl sub_080f9010
.L_0800ec3e:
	ldr r5, [pc, #432]
.L_0800ec40:
	ldrb r3, [r5, #0]
	cmp r3, #0
	beq .L_0800ec76
	ldr r5, [pc, #432]
	movs r2, #128
	ldr r3, [r5, #0]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0800ec78
	adds r3, r2, #0
.L_0800ec56:
	subs r3, #1
	cmp r3, #0
	bne .L_0800ec56
	movs r3, #95
.L_0800ec5e:
	subs r3, #1
	cmp r3, #0
	bge .L_0800ec5e
	movs r3, #63
.L_0800ec66:
	subs r3, #1
	cmp r3, #0
	bge .L_0800ec66
	movs r3, #63
.L_0800ec6e:
	subs r3, #1
	cmp r3, #0
	bge .L_0800ec6e
	b .L_0800ec78
.L_0800ec76:
	ldr r5, [pc, #384]
.L_0800ec78:
	ldr r3, [pc, #384]
	movs r1, #135
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrh r2, [r3, #0]
	ldr r3, [r5, #0]
	ands r3, r2
	cmp r3, #0
	beq .L_0800ec9c
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	movs r2, #5
	str r3, [r7, #52]
	str r2, [sp, #8]
	b .L_0800ecac
.L_0800ec9c:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	movs r3, #2
	str r3, [sp, #8]
.L_0800ecac:
	ldr r0, [pc, #336]
	bl sub_080770c0
	cmp r0, #0
	beq .L_0800ecd4
	ldr r5, [pc, #320]
	ldr r3, [r5, #0]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0800ecd6
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	movs r0, #5
	str r3, [r7, #52]
	str r0, [sp, #8]
	b .L_0800ecd6
.L_0800ecd4:
	ldr r5, [pc, #288]
.L_0800ecd6:
	ldr r3, [r5, #0]
	movs r2, #15
	lsrs r3, r3, #4
	ldr r1, [pc, #292]
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	lsls r3, r3, #16
	str r3, [sp, #4]
	lsrs r1, r3, #16
	ldr r3, [pc, #284]
	cmp r1, r3
	bne .L_0800ecfa
	ldr r0, [sp, #20]
	movs r3, #4
	orrs r0, r3
	str r0, [sp, #20]
	b sub_0800f0c6
.L_0800ecfa:
	movs r2, #0
	movs r3, #92
	str r2, [sp, #20]
	add r3, sp
	mov r11, r3
	ldr r3, [r7, #8]
	mov r0, r11
	str r3, [r0, #0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #12
	mov r2, r11
	bl sub_0800447c
	ldr r3, [pc, #208]
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_0800ed38
	ldr r3, [r5, #0]
	movs r2, #128
	ldr r1, [sp, #4]
	lsls r2, r2, #2
	asrs r1, r1, #16
	ands r3, r2
	str r1, [sp, #12]
	cmp r3, #0
	beq .L_0800ed38
	b sub_0800f0c6
.L_0800ed38:
	adds r0, r7, #0
	mov r1, r11
	bl sub_080120dc
	cmp r0, #0
	bne sub_0800ee14
	ldr r3, [r7, #8]
	add r5, sp, #80
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r3, [sp, #4]
	movs r2, #128
	lsls r2, r2, #12
	movs r0, #128
	mov r8, r2
	lsrs r6, r3, #16
	lsls r0, r0, #5
	adds r1, r6, r0
	adds r2, r5, #0
	mov r0, r8
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne sub_0800ee14
	ldr r3, [r7, #8]
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r2, [pc, #136]
	mov r0, r8
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne sub_0800ee14
	ldr r3, [r7, #8]
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r6, r3
	mov r0, r8
	adds r2, r5, #0
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne sub_0800ee14
	ldr r3, [r7, #8]
	str r3, [r5, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r0, [pc, #68]
	str r3, [r5, #8]
	adds r1, r6, r0
	adds r2, r5, #0
	mov r0, r8
	bl sub_0800447c
	adds r0, r7, #0
	adds r1, r5, #0
	bl sub_080120dc
	cmp r0, #0
	bne sub_0800ee14
	ldr r1, [sp, #4]
	asrs r1, r1, #16
	lsls r2, r1, #16
	str r1, [sp, #12]
	str r2, [sp, #0]
	b sub_0800ef44
