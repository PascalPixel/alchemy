@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08003f3c, 0x08003f3c
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080091e0, 0x080091e0
	.set sub_08009240, 0x08009240
	.set sub_08015040, 0x08015040
	.set sub_080770c0, 0x080770c0
	.set sub_080f9010, 0x080f9010
	.global Region_0809b698
Region_0809b698:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #316]
	ldr r3, [r3, #0]
	sub sp, #8
	str r3, [sp, #4]
	ldr r1, [r3, #16]
	ldr r6, [r1, #80]
	ldrh r3, [r1, #6]
	ldr r2, [r6, #40]
	mov r10, r1
	str r3, [sp, #0]
	mov r9, r2
	bl sub_08004080
	ldr r2, [pc, #292]
	ldr r1, [sp, #4]
	adds r3, r1, r2
	movs r1, #0
	mov r8, r1
	strh r0, [r3, #0]
	movs r1, #128
	lsls r0, r0, #16
	lsls r1, r1, #1
	ldr r2, [pc, #280]
	asrs r0, r0, #16
	bl sub_08003fa4
	ldr r5, [pc, #276]
	movs r3, #145
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #150
	lsls r3, r3, #20
	str r3, [r2, #0]
	ldr r0, [pc, #264]
	bl sub_080770c0
	movs r1, #146
	lsls r1, r1, #2
	adds r3, r5, r1
	strb r0, [r3, #0]
	movs r1, #0
	mov r0, r10
	bl sub_08009240
	ldr r3, [pc, #248]
	mov r2, r10
	mov r5, r10
	str r3, [r2, #108]
	adds r5, #100
	mov r3, r8
	strh r3, [r5, #0]
	mov r3, r10
	mov r1, r8
	adds r3, #102
	strh r1, [r3, #0]
	movs r0, #140
	bl sub_080f9010
	movs r0, #15
	bl sub_080030f8
	movs r3, #1
	strh r3, [r5, #0]
	movs r0, #10
	bl sub_080030f8
	movs r2, #38
	adds r2, r2, r6
	movs r3, #7
	mov r8, r2
	adds r6, #37
	movs r7, #1
	movs r5, #19
	mov r11, r3
.L_0809b73a:
	mov r1, r11
	mov r2, r9
	strb r1, [r2, #5]
	movs r0, #2
	strb r7, [r6, #0]
	bl sub_080030f8
	movs r3, #0
	mov r1, r9
	mov r2, r8
	strb r7, [r6, #0]
	strb r3, [r1, #5]
	strb r7, [r2, #0]
	movs r0, #3
	subs r5, #1
	bl sub_080030f8
	cmp r5, #0
	bge .L_0809b73a
	mov r2, sp
	ldrh r2, [r2, #0]
	ldr r5, [pc, #148]
	movs r3, #0
	mov r1, r10
	str r3, [r1, #108]
	mov r3, r10
	movs r1, #200
	strh r2, [r3, #6]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl sub_080041d8
	movs r0, #15
	bl sub_080030f8
	movs r0, #174
	bl sub_080f9010
	movs r0, #55
	bl sub_080030f8
	adds r0, r5, #0
	bl sub_08004278
	ldr r3, [pc, #92]
	movs r1, #147
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0809b7ac
	mov r0, r10
	movs r1, #2
	bl sub_080091e0
	b .L_0809b7b4
.L_0809b7ac:
	mov r0, r10
	movs r1, #1
	bl sub_080091e0
.L_0809b7b4:
	mov r0, r10
	movs r1, #0
	bl sub_08009240
	ldr r2, [pc, #40]
	ldr r1, [sp, #4]
	adds r3, r1, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl sub_08003f3c
	ldr r0, [pc, #52]
	movs r1, #1
	bl sub_08015040
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
