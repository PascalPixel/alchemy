.syntax unified
	.thumb
	.section .text.x02008096,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008098,"ax",%progbits
	.global KorimaMagari_DrawPanel
	.thumb_func
KorimaMagari_DrawPanel:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	lsls r1, r1, #7
	ldr r4, [sp, #48]
	mov r10, r2
	adds r1, r1, r0
	ldr r2, .L_02008138
	lsls r1, r1, #2
	adds r3, r4, r3
	adds r5, r1, r2
	cmp r4, r3
	bge .L_02008126
	str r3, [sp, #4]
	mov r6, r10
	movs r3, #128
	subs r3, r3, r6
	lsls r3, r3, #2
	mov r11, r3
	ldr r3, [sp, #40]
	lsls r3, r3, #4
	mov r9, r3
.L_020080ce:
	ldr r0, [sp, #44]
	mov r1, r10
	adds r2, r0, r1
	cmp r0, r2
	bge .L_0200811c
	ldr r3, .L_0200813c
	movs r7, #15
	mov r8, r3
	adds r3, r4, #0
	ands r3, r7
	add r3, r9
	lsls r3, r3, #5
	ldr r6, .L_02008140
	str r3, [sp, #0]
	mov lr, r6
	mov r12, r2
.L_020080ee:
	ldr r6, [sp, #0]
	ldmia r5!, {r1}
	adds r3, r0, #0
	mov r2, r8
	ands r3, r7
	ands r1, r2
	adds r3, r6, r3
	ldr r6, .L_02008144
	lsls r1, r1, #3
	adds r2, r1, r6
	ldr r2, [r2]
	lsls r3, r3, #2
	mov r6, lr
	str r2, [r3, r6]
	ldr r6, .L_02008148
	adds r2, r1, r6
	ldr r1, .L_0200814c
	ldr r2, [r2]
	adds r3, r3, r1
	adds r0, #1
	str r2, [r3]
	cmp r0, r12
	blt .L_020080ee
.L_0200811c:
	ldr r2, [sp, #4]
	adds r4, #1
	add r5, r11
	cmp r4, r2
	blt .L_020080ce
.L_02008126:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_02008138:
	.4byte gMapCellBuffer
.L_0200813c:
	.4byte 0x00000fff
.L_02008140:
	.4byte 0x06002800
.L_02008144:
	.4byte gMapBlocks
.L_02008148:
	.4byte gMapBlocks + 0x4
.L_0200814c:
	.4byte 0x06002840
	.section .rodata.x0200911c,"a",%progbits
	.global KorimaMagari_DefaultRecords
KorimaMagari_DefaultRecords:
	.4byte 0x000b00cd
	.4byte 0x00010009
	.4byte 0x00000000
	.4byte 0x000c00cf
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x001100cf
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x000d00cd
	.4byte 0x0001000d
	.4byte 0x00000000
	.4byte 0x000a00cf
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorimaMagari_PushAnimations
KorimaMagari_PushAnimations:
	.4byte 0x03030202
	.global KorimaMagari_PushStepX
KorimaMagari_PushStepX:
	.4byte 0x00f80008
	.global KorimaMagari_PushStepZ
KorimaMagari_PushStepZ:
	.4byte 0xf8000800
	.4byte 0xffff0000
	.4byte 0x00000197
	.4byte 0x80000114
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00200032
	.4byte 0x00000171
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x002000a2
	.4byte 0x40000054
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002b
	.4byte 0x0010202a
	.4byte 0x0020102c
	.4byte 0x000001ff
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte Scene_CallHelper118c
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte KorimaMagari_RedrawBoard
	.4byte 0x00008602
	.4byte 0xffff000d
	.4byte KorimaMagari_RunReturnSequence
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte KorimaMagari_RunDepartSequence
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001526
	.4byte 0x00000003
	.4byte 0xffff0009
	.4byte Scene_RunKorimaMagariSequence
	.4byte 0x00000013
	.4byte 0x0f000064
	.4byte 0x0010005b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 8
	.global gKorimaMagariRecords
gKorimaMagariRecords:
	.space 4
	.global gKorimaMagariReturned
gKorimaMagariReturned:
	.space 4
	.global gKorimaMagariLayout
gKorimaMagariLayout:
	.space 8
	.global KorimaPalette_First
KorimaPalette_First:
	.space 1792
	.global KorimaPalette_Second
KorimaPalette_Second:
	.space 896
	.space 896
	.global KorimaMagari_ShakeScroll
KorimaMagari_ShakeScroll:
	.space 12
	.global KorimaMagari_ShakeChance
KorimaMagari_ShakeChance:
	.space 4
