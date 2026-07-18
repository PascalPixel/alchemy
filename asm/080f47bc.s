@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002304, 0x08002304
	.set sub_08004458, 0x08004458
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_08077230, 0x08077230
	.set sub_080772c8, 0x080772c8
	.set sub_080f4af0, 0x080f4af0
	.set sub_080f4af4, 0x080f4af4
	.set sub_080f535e, 0x080f535e
	.set sub_080f9010, 0x080f9010
	.global Region_080f47bc
Region_080f47bc:
	ldr r5, [sp, #96]
	cmp r5, #3
	bne .L_080f484c
	ldr r6, [sp, #76]
	cmp r6, #0
	bne .L_080f47f8
	movs r7, #0
	ldr r0, [sp, #108]
	ldr r1, [pc, #740]
	str r7, [sp, #80]
	adds r3, r0, r1
	ldr r2, [r3, #0]
	movs r0, #0
	ldr r1, [sp, #72]
	movs r3, #40
	str r7, [sp, #0]
	bl sub_080150b0
	ldr r3, [pc, #724]
	ldr r2, [sp, #108]
	movs r4, #153
	lsls r4, r4, #3
	ldr r0, [r3, #16]
	adds r3, r2, r4
	ldr r2, [r3, #0]
	movs r1, #6
	movs r3, #0
	str r7, [sp, #0]
	bl sub_080150b0
.L_080f47f8:
	ldr r5, [sp, #76]
	cmp r5, #15
	bne .L_080f4844
	ldr r7, [sp, #108]
	ldr r0, [pc, #688]
	movs r6, #0
	str r6, [sp, #96]
	adds r3, r7, r0
	ldr r0, [r3, #0]
	movs r1, #1
	bl sub_08015018
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #8
	movs r3, #4
	movs r0, #22
	bl sub_08015010
	ldr r5, [pc, #664]
	ldr r2, [sp, #64]
	adds r1, r0, #0
	str r1, [r2, #0]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl sub_08015080
	ldr r3, [sp, #64]
	adds r0, r5, #1
	ldr r1, [r3, #0]
	movs r2, #0
	movs r3, #8
	bl sub_08015080
	movs r4, #1
	mov r8, r4
.L_080f4844:
	ldr r5, [sp, #76]
	adds r5, #1
	str r5, [sp, #76]
	b .L_080f49e6
.L_080f484c:
	ldr r6, [sp, #96]
	cmp r6, #0
	beq .L_080f4854
	b .L_080f49e6
.L_080f4854:
	ldr r7, [sp, #100]
	cmp r7, #17
	bgt .L_080f485c
	b .L_080f49e6
.L_080f485c:
	ldr r7, [pc, #608]
	ldr r0, [pc, #612]
	ldr r3, [r7, #0]
	ldr r3, [r7, #0]
	ldr r3, [r0, #0]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080f488a
	ldr r1, [sp, #112]
	ldr r3, [pc, #596]
	adds r2, r1, r3
	ldr r3, [r2, #0]
	movs r1, #128
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r2, #0]
	ldr r4, [sp, #112]
	ldr r5, [pc, #584]
	adds r2, r4, r5
	ldr r3, [r2, #0]
	adds r3, r3, r1
	str r3, [r2, #0]
.L_080f488a:
	ldr r3, [r0, #0]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080f48ac
	ldr r6, [sp, #112]
	ldr r0, [pc, #560]
	adds r2, r6, r0
	ldr r3, [r2, #0]
	ldr r1, [pc, #560]
	adds r3, r3, r1
	str r3, [r2, #0]
	ldr r3, [pc, #552]
	adds r2, r6, r3
	ldr r3, [r2, #0]
	adds r3, r3, r1
	str r3, [r2, #0]
.L_080f48ac:
	ldr r4, [sp, #112]
	ldr r5, [pc, #536]
	adds r2, r4, r5
	ldr r3, [r2, #0]
	ldr r0, [pc, #540]
	cmp r3, r0
	bgt .L_080f48c0
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #0]
.L_080f48c0:
	movs r1, #160
	lsls r1, r1, #16
	cmp r3, r1
	ble .L_080f48ca
	str r1, [r2, #0]
.L_080f48ca:
	ldr r3, [pc, #512]
	ldr r6, [sp, #112]
	adds r2, r6, r3
	ldr r3, [r2, #0]
	cmp r3, r0
	bgt .L_080f48dc
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #0]
.L_080f48dc:
	cmp r3, r1
	ble .L_080f48e2
	str r1, [r2, #0]
.L_080f48e2:
	ldr r6, [pc, #468]
	bl sub_080772c8
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [r6, #16]
	lsls r3, r3, #1
	cmp r2, r3
	bcs .L_080f4902
	ldr r4, [sp, #64]
	movs r1, #1
	ldr r0, [r4, #0]
	bl sub_08015018
	bl sub_080f535e
.L_080f4902:
	ldr r5, [r7, #0]
	movs r3, #2
	ands r5, r3
	cmp r5, #0
	beq .L_080f4920
	movs r0, #113
	bl sub_080f9010
	ldr r5, [sp, #64]
	movs r1, #1
	ldr r0, [r5, #0]
	bl sub_08015018
	bl sub_080f535e
.L_080f4920:
	ldr r3, [r7, #0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080f49e6
	movs r0, #151
	lsls r0, r0, #1
	bl sub_080f9010
	ldr r7, [sp, #64]
	movs r1, #1
	ldr r0, [r7, #0]
	bl sub_08015018
	movs r0, #1
	str r0, [sp, #96]
	bl sub_080772c8
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	str r3, [sp, #92]
	ldr r1, [sp, #92]
	ldr r3, [r6, #16]
	cmp r1, r3
	bls .L_080f4956
	str r3, [sp, #92]
.L_080f4956:
	ldr r2, [sp, #92]
	negs r0, r2
	bl sub_08077230
	ldr r0, [r6, #16]
	ldr r4, [sp, #108]
	movs r6, #153
	lsls r6, r6, #3
	adds r3, r4, r6
	ldr r2, [r3, #0]
	movs r1, #6
	movs r3, #0
	str r5, [sp, #0]
	bl sub_080150b0
	movs r7, #0
	ldr r0, [sp, #112]
	movs r1, #225
	mov r11, r7
	lsls r1, r1, #7
	ldr r7, [sp, #36]
	adds r6, r0, r1
.L_080f4982:
	mov r2, r11
	cmp r2, #1
	ble .L_080f4992
	ldr r3, [pc, #332]
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r6, #4]
.L_080f4992:
	bl sub_08004458
	movs r3, #127
	ands r3, r0
	subs r3, #64
	lsls r3, r3, #12
	str r3, [r6, #12]
	bl sub_08004458
	movs r3, #15
	ands r3, r0
	adds r3, #48
	lsls r3, r3, #13
	str r3, [r6, #16]
	bl sub_08004458
	movs r5, #63
	movs r1, #6
	ands r5, r0
	ldr r0, [r6, #4]
	bl sub_080022ec
	adds r5, #140
	lsls r5, r5, #12
	adds r5, r5, r0
	str r5, [r6, #20]
	bl sub_08004458
	movs r1, #144
	lsls r1, r1, #7
	bl sub_08002304
	movs r3, #150
	lsls r3, r3, #1
	str r0, [r6, #24]
	stmia r7!, {r3}
	movs r3, #1
	add r11, r3
	mov r4, r11
	adds r6, #28
	cmp r4, #2
	bne .L_080f4982
.L_080f49e6:
	mov r5, r8
	cmp r5, #1
	bne .L_080f4a40
	ldr r6, [sp, #112]
	movs r7, #225
	ldr r0, [pc, #212]
	lsls r7, r7, #7
	movs r3, #160
	adds r2, r6, r7
	lsls r3, r3, #14
	str r3, [r2, #0]
	ldr r1, [pc, #220]
	adds r3, r6, r0
	movs r0, #192
	lsls r0, r0, #15
	str r0, [r3, #0]
	adds r3, r6, r1
	ldr r1, [pc, #212]
	str r1, [r3, #0]
	ldr r3, [pc, #212]
	ldr r4, [pc, #188]
	adds r2, r6, r3
	ldr r3, [pc, #212]
	ldr r5, [pc, #212]
	str r3, [r2, #0]
	adds r3, r6, r4
	str r0, [r3, #0]
	adds r3, r6, r5
	str r1, [r3, #0]
	bl sub_08004458
	movs r1, #6
	bl sub_08002304
	str r0, [sp, #136]
.L_080f4a2c:
	bl sub_08004458
	movs r1, #6
	bl sub_08002304
	ldr r6, [sp, #40]
	str r0, [r6, #4]
	ldr r3, [sp, #136]
	cmp r3, r0
	beq .L_080f4a2c
.L_080f4a40:
	movs r7, #0
	mov r11, r7
	mov r0, sp
	adds r0, #128
	mov r1, r11
	str r0, [sp, #44]
	movs r2, #1
	str r1, [r0, #0]
	ldr r3, [pc, #136]
	movs r5, #0
	str r2, [sp, #132]
	movs r4, #1
	ldr r2, [sp, #44]
	mov r10, r5
	mov r12, r3
	mov lr, r0
	lsls r3, r4, #3
	mov r0, r10
	ldr r1, [r0, r2]
	subs r3, r3, r4
	lsls r3, r3, #2
	mov r6, r12
	adds r7, r3, r6
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r2, r3, #2
	ldr r3, [sp, #112]
	add r2, r12
	ldr r2, [r3, r2]
	ldr r3, [r3, r7]
	mov r8, r3
	add r6, sp, #132
	cmp r2, r8
	ble sub_080f4af4
.L_080f4a84:
	movs r0, #1
	negs r0, r0
	add r10, r0
	str r1, [r6, #0]
	mov r1, r10
	subs r6, #4
	subs r5, #4
	cmp r1, #0
	blt sub_080f4af0
	mov r2, lr
	adds r0, r5, #0
	ldr r1, [r0, r2]
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r2, r3, #2
	ldr r3, [sp, #112]
	add r2, r12
	ldr r2, [r3, r2]
	ldr r3, [r3, r7]
	mov r8, r3
	cmp r2, r8
	bgt .L_080f4a84
	b sub_080f4af4
