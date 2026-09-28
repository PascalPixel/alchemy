.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ENTRY.INC"
	.section .text.x0200816c,"ax",%progbits
	.align 2
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200016c_0
	ldr r0, [pc, #24]
	b .L_0200016c_1
.L_0200016c_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_0200016c_2
	ldr r0, [pc, #24]
	b .L_0200016c_1
.L_0200016c_2:
	ldr r0, [pc, #24]
.L_0200016c_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000027
	.4byte 0x0200af80
	.4byte 0x00000026
	.4byte 0x0200afc8
	.4byte 0x0200ae60
	.global Func_020001ac
	.thumb_func
Func_020001ac:
	push {lr}
	ldr r3, [pc, #24]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #16]
	movs r0, #0
	cmp r2, r3
	bne .L_020001ac_0
	ldr r0, [pc, #12]
.L_020001ac_0:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000026
	.4byte 0x0200b010
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b040
	.global Func_020001dc
	.thumb_func
Func_020001dc:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020001dc_0
	ldr r0, [pc, #40]
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_020001dc_1
	ldr r0, [pc, #36]
	bl 0x020080a0
.L_020001dc_1:
	ldr r0, [pc, #28]
	b .L_020001dc_2
.L_020001dc_0:
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_020001dc_3
	ldr r0, [pc, #28]
	b .L_020001dc_2
.L_020001dc_3:
	ldr r0, [pc, #28]
.L_020001dc_2:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000024
	.4byte 0x00000845
	.4byte 0x0200b098
	.4byte 0x00000027
	.4byte 0x0200b368
	.4byte 0x0200b080
	.section .text.x02008284,"ax",%progbits
	.align 2
	.global Func_02000284
	.thumb_func
Func_02000284:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000284_0
	ldr r0, [pc, #16]
	b 0x0200829e
.L_02000284_0:
	ldr r0, [pc, #16]
.L_0200029e:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0027
	.2byte 0x0000
	.2byte 0xb590
	.2byte 0x0200
	.2byte 0xb3b0
	.2byte 0x0200
	.section .text.x02008694,"ax",%progbits
	.align 2
	.global Func_02000694
	.thumb_func
Func_02000694:
	push {r5, lr}
	ldr r3, [pc, #328]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #320]
	sub sp, #8
	cmp r2, r3
	bne .L_02000694_0
	bl 0x0200a910
	b .L_02000694_1
.L_02000694_0:
	ldr r3, [pc, #308]
	cmp r2, r3
	bne .L_02000694_2
	ldr r3, [pc, #308]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	b .L_02000694_1
.L_02000694_2:
	movs r0, #23
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #24
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #25
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	movs r0, #26
	bl 0x0200aa54
	movs r1, #0
	bl 0x0200aa0c
	ldr r5, [pc, #248]
	movs r0, #23
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #24
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #25
	adds r1, r5, #0
	bl 0x0200aa6c
	movs r0, #26
	adds r1, r5, #0
	bl 0x0200aa6c
	ldr r0, [pc, #216]
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_02000694_3
	movs r5, #8
.L_02000694_4:
	adds r0, r5, #0
	bl 0x0200aa54
	adds r5, #1
	movs r1, #0
	bl 0x0200aa0c
	cmp r5, #16
	bls .L_02000694_4
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #8
	movs r0, #13
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200aa04
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #13
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200aa04
	movs r3, #14
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #13
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl 0x0200aa04
.L_02000694_3:
	ldr r0, [pc, #132]
	bl 0x0200aa1c
	cmp r0, #0
	bne .L_02000694_5
	ldr r3, [pc, #100]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02000694_5
	bl 0x020088ec
.L_02000694_5:
	ldr r0, [pc, #104]
	bl 0x0200aa1c
	cmp r0, #0
	beq .L_02000694_1
	movs r0, #1
	bl 0x0200aa5c
	movs r0, #2
	bl 0x0200aa5c
	movs r0, #3
	bl 0x0200aa5c
	movs r0, #17
	bl 0x0200aa5c
	movs r0, #18
	bl 0x0200aa5c
	movs r0, #19
	bl 0x0200aa5c
	movs r0, #20
	bl 0x0200aa5c
	movs r0, #21
	bl 0x0200aa5c
	movs r0, #22
	bl 0x0200aa5c
	ldr r0, [pc, #44]
	bl 0x0200aa44
.L_02000694_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000027
	.4byte 0x00000026
	.4byte 0x03001ebc
	.4byte 0x0200add8
	.4byte 0x00000845
	.4byte 0x00000843
	.4byte 0x0200b2d8
	.section .text.x0200a99c,"ax",%progbits
	.align 2
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.global KorimaMura_ActionTable1
KorimaMura_ActionTable1:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00610000
	.4byte 0x00000000
	.4byte 0x00dd0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KorimaMura_ActionTable2
KorimaMura_ActionTable2:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00650000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KorimaMura_ActionTable3
KorimaMura_ActionTable3:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x004e0000
	.4byte 0x00000000
	.4byte 0x00f70000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KorimaMura_ActionTable4
KorimaMura_ActionTable4:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KorimaMura_ActionTable5
KorimaMura_ActionTable5:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global KorimaMura_ActionTable6
KorimaMura_ActionTable6:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000051e
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000051e
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000046
	.4byte 0xc0010000
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.global KorimaMura_ActionTable7
KorimaMura_ActionTable7:
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc29
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc29
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000005a
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000022
	.4byte 0x02008051
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KorimaMura_DebrisScript
KorimaMura_DebrisScript:
	.4byte 0x00000022
	.4byte 0x020080cd
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KorimaMura_Actor8Path
KorimaMura_Actor8Path:
	.4byte 0x00000022
	.4byte 0x02008115
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KorimaMura_SwitchCells
KorimaMura_SwitchCells:
	.4byte 0x00200042
	.4byte 0x00020001
	.4byte 0x00430004
	.4byte 0x00010020
	.4byte 0x00040002
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x000000e8
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x0000002c
	.4byte 0x0000010b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001a2
	.4byte 0x8000011d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000077
	.4byte 0x400000b6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000097
	.4byte 0x40000136
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000108
	.4byte 0x40000175
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000149
	.4byte 0x400000e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000107
	.4byte 0xc0000146
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000168
	.4byte 0x4000015a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x0000002c
	.4byte 0x0000010b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000000e8
	.4byte 0x400000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000aa
	.4byte 0xc0000182
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000032
	.4byte 0x00300000
	.4byte 0x01200000
	.4byte 0x000001a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0x40000140
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x40000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001f00cf
	.4byte 0x00e10137
	.4byte 0x01490031
	.4byte 0x0001ffff
	.4byte 0x001f012f
	.4byte 0x0141012f
	.4byte 0x01410031
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000024
	.4byte 0x00107002
	.4byte 0x00226002
	.4byte 0x00504025
	.4byte 0x00603025
	.4byte 0x00707025
	.4byte 0x0080a025
	.4byte 0x00908025
	.4byte 0x00a02026
	.4byte 0x00000027
	.4byte 0x0010b002
	.4byte 0x0022e002
	.4byte 0x00000026
	.4byte 0x00109025
	.4byte 0x00208024
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00008000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00034000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00008000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00010000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00018000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0x08430037
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x08430038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0x084300dc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01004000
	.4byte 0x0031005a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01004000
	.4byte 0x0845011d
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00035000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0063
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008305
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008305
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008305
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008305
	.4byte 0x00004602
	.4byte 0xffff0009
	.4byte 0x02008305
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x18450008
	.4byte 0x0000168d
	.4byte 0x00000000
	.4byte 0x18450009
	.4byte 0x0000168e
	.4byte 0x00000000
	.4byte 0x1845000a
	.4byte 0x0000168f
	.4byte 0x00000000
	.4byte 0x1845000b
	.4byte 0x00001690
	.4byte 0x00000000
	.4byte 0x1845000c
	.4byte 0x00001691
	.4byte 0x00000000
	.4byte 0x1845000d
	.4byte 0x00001692
	.4byte 0x00000000
	.4byte 0x1845000e
	.4byte 0x00001693
	.4byte 0x00000000
	.4byte 0x1845000f
	.4byte 0x00001694
	.4byte 0x00000000
	.4byte 0x18450010
	.4byte 0x02008231
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008275
	.4byte 0x00008d15
	.4byte 0x18450008
	.4byte 0x00001695
	.4byte 0x00008d15
	.4byte 0x18450009
	.4byte 0x00001696
	.4byte 0x00008d15
	.4byte 0x1845000a
	.4byte 0x00001697
	.4byte 0x00008d15
	.4byte 0x1845000b
	.4byte 0x00001698
	.4byte 0x00008d15
	.4byte 0x1845000c
	.4byte 0x00001699
	.4byte 0x00008d15
	.4byte 0x1845000d
	.4byte 0x0000169a
	.4byte 0x00008d15
	.4byte 0x1845000e
	.4byte 0x0000169b
	.4byte 0x00008d15
	.4byte 0x1845000f
	.4byte 0x0000169c
	.4byte 0x00008d15
	.4byte 0x18450010
	.4byte 0x000016b6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001675
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001676
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001677
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001678
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001679
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000167a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000167b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000167c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001688
	.4byte 0x00000023
	.4byte 0x0f570064
	.4byte 0x001000c1
	.4byte 0x00000023
	.4byte 0x0f580065
	.4byte 0x001000b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0847000a
	.4byte 0x020083c9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001788
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001789
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorimaMura_Object26Script
KorimaMura_Object26Script:
	.4byte 0x00000022
	.4byte 0x0200a5d9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KorimaMura_RegionScript
KorimaMura_RegionScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001b
	.global gFallingEffectScript
gFallingEffectScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000022
	.4byte 0x0200a6a5
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000022
	.4byte 0x0200a6a5
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000022
	.4byte 0x0200a6a5
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0xc0010000
	.4byte 0x00000022
	.4byte 0x0200a6cd
	.4byte 0x0000001b
