@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_08002f3c, 0x08002f3c
	.set sub_08002f40, 0x08002f40
	.set sub_080040e8, 0x080040e8
	.set sub_080048b0, 0x080048b0
	.set sub_080048f4, 0x080048f4
	.set sub_08005340, 0x08005340
	.set sub_080f4028, 0x080f4028
	.set sub_080f4100, 0x080f4100
	.set sub_080f4318, 0x080f4318
	.global Region_080f4168
Region_080f4168:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #336]
	ldr r1, [pc, #336]
	movs r0, #41
	add sp, r5
	bl sub_080048b0
	movs r1, #128
	lsls r1, r1, #2
	movs r0, #40
	bl sub_080048b0
	ldr r1, [pc, #320]
	str r0, [sp, #116]
	movs r0, #39
	bl sub_080048f4
	movs r1, #195
	str r0, [sp, #112]
	lsls r1, r1, #3
	movs r0, #45
	bl sub_080048f4
	movs r1, #76
	str r0, [sp, #108]
	movs r0, #12
	bl sub_080048f4
	str r0, [sp, #104]
	ldr r0, [pc, #292]
	bl sub_08002f3c
	bl sub_080f4028
	bl sub_080040e8
	ldr r2, [pc, #280]
	movs r3, #0
	movs r0, #0
	strb r3, [r2, #0]
	ldr r6, [pc, #276]
	mov r11, r0
	movs r5, #0
	movs r4, #0
.L_080f41cc:
	movs r1, #0
	adds r0, r4, #0
.L_080f41d0:
	mov r3, r11
	adds r2, r3, r6
	movs r7, #2
	adds r3, r1, r0
	adds r1, #1
	strh r3, [r2, #0]
	add r11, r7
	cmp r1, #32
	bne .L_080f41d0
	adds r5, #1
	adds r4, #30
	cmp r5, #20
	bne .L_080f41cc
	ldr r0, [pc, #244]
	bl sub_08002f40
	movs r1, #160
	adds r4, r0, #0
	ldr r3, [pc, #236]
	lsls r1, r1, #19
	ldr r2, [pc, #236]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #160
	lsls r0, r0, #19
	add r1, sp, #256
	ldr r2, [pc, #228]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #224
	lsls r0, r0, #1
	adds r4, r4, r0
	adds r0, r4, #0
	ldr r1, [pc, #220]
	bl sub_08005340
	ldr r3, [pc, #200]
	ldr r0, [pc, #212]
	ldr r1, [pc, #212]
	ldr r2, [pc, #216]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [pc, #212]
	bl sub_08002f40
	ldr r3, [pc, #184]
	adds r4, r0, #0
	ldr r1, [sp, #116]
	ldr r2, [pc, #184]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r4, #0
	ldr r1, [pc, #196]
	ldr r2, [pc, #176]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #2
	adds r4, r4, r1
	adds r0, r4, #0
	ldr r1, [pc, #164]
	bl sub_08005340
	ldr r3, [pc, #148]
	ldr r0, [pc, #156]
	ldr r1, [pc, #176]
	ldr r2, [pc, #176]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r5, #128
	lsls r5, r5, #1
	adds r3, r5, #0
	ldr r1, [pc, #156]
	ldr r0, [sp, #116]
	movs r2, #0
	bl sub_080f4100
	movs r1, #160
	adds r3, r5, #0
	lsls r1, r1, #19
	movs r2, #0
	add r0, sp, #256
	bl sub_080f4100
	ldr r2, [pc, #144]
	ldr r3, [pc, #48]
	strh r3, [r2, #0]
	ldr r3, [pc, #48]
	adds r2, #62
	strh r3, [r2, #0]
	ldr r3, [pc, #44]
	adds r2, #2
	strh r3, [r2, #0]
	ldr r3, [pc, #44]
	subs r2, #74
	strh r3, [r2, #0]
	ldr r3, [pc, #40]
	adds r2, #80
	strh r3, [r2, #0]
	ldr r3, [pc, #40]
	adds r2, #2
	strh r3, [r2, #0]
	ldr r3, [pc, #112]
	movs r2, #0
	strh r2, [r3, #4]
	strh r2, [r3, #6]
	ldr r3, [pc, #108]
	ldr r1, [pc, #28]
	strh r2, [r3, #0]
	b sub_080f4318
