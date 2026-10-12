@ Each overlay's code and data is one compressed directory resource.
@ Streams are rounded to four bytes. PACKER rows name a proven buffer
@ writer or mark unused residue after the native terminator. Resource_LoadCode
@ uses decoded RAM and output length; an unknown residue writer stays unknown.
	.section .overlays,"a"

	.global Resource_Overlay649
Resource_Overlay649:
	.incbin "overlays/resource_649.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x00, 0x01, 0xff	@ PACKER: left in the buffer by Field_Map299+0x48d
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x00, 0x01, 0xff	@ PACKER: left in the buffer by Field_Map299+0x48d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x00, 0x01, 0xff	@ PACKER: left in the buffer by Field_Map299+0x48d
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0x01, 0xff	@ PACKER: left in the buffer by Field_Map299+0x48d
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x00, 0x01, 0xff	@ PACKER: left in the buffer by Field_Map299+0x48d
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x00, 0x01, 0xff	@ PACKER: left in the buffer by Field_Map299+0x48d
	.endif
	.balign 4, 0

	.global Resource_Overlay64A
Resource_Overlay64A:
	.incbin "overlays/resource_64a.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x0c, 0x2d	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1c86
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x03, 0x3a	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1d0e
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x41	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1d07
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x41	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1d07
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x3a, 0xb2	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1d0a
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x3a, 0xb2	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1d0a
	.endif
	.balign 4, 0

	.global Resource_Overlay64B
Resource_Overlay64B:
	.incbin "overlays/resource_64b.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x0b, 0x80	@ PACKER: left in the buffer by Field_Map097+0x3dee
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xde, 0x0b, 0x80	@ PACKER: left in the buffer by Field_Map097+0x3ded
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xdd, 0x03	@ PACKER: left in the buffer by Field_Map097+0x3dea
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x0b, 0x80	@ PACKER: left in the buffer by Field_Map097+0x3dee
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x0b, 0x80	@ PACKER: left in the buffer by Field_Map097+0x3dee
	.endif
	.balign 4, 0

	.global Resource_Overlay64C
Resource_Overlay64C:
	.incbin "overlays/resource_64c.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x01, 0x9d	@ PACKER: left in the buffer by Resource_Overlay64B+0x2dee
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xab, 0x3c	@ PACKER: left in the buffer by Resource_Overlay64B+0x2dee
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x95, 0xe7	@ PACKER: left in the buffer by Resource_Overlay64B+0x2dee
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xc3	@ PACKER: left in the buffer by Resource_Overlay64B+0x2def
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x55, 0x9e	@ PACKER: left in the buffer by Resource_Overlay64B+0x2dee
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x5e, 0xe5	@ PACKER: left in the buffer by Resource_Overlay64B+0x2dee
	.endif
	.balign 4, 0

	.global Resource_Overlay64D
Resource_Overlay64D:
	.incbin "overlays/resource_64d.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xe5, 0x47, 0x21	@ PACKER: left in the buffer by Resource_Overlay64C+0x19e9
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xe5, 0x47, 0x21	@ PACKER: left in the buffer by Resource_Overlay64C+0x19e9
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x2f, 0x3f, 0x0a	@ PACKER: left in the buffer by Resource_Overlay64C+0x19e9
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x2f, 0x3f, 0x0a	@ PACKER: left in the buffer by Resource_Overlay64C+0x19e9
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe5, 0x47, 0x21	@ PACKER: left in the buffer by Resource_Overlay64C+0x19e9
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe5, 0x47, 0x21	@ PACKER: left in the buffer by Resource_Overlay64C+0x19e9
	.endif
	.balign 4, 0

	.global Resource_Overlay64E
Resource_Overlay64E:
	.incbin "overlays/resource_64e.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x89, 0x46	@ PACKER: left in the buffer by Resource_Overlay64D+0x1196
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x0a, 0x11, 0x0a	@ PACKER: left in the buffer by Resource_Overlay64D+0x11b5
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x05, 0x11, 0xfd	@ PACKER: left in the buffer by Resource_Overlay64D+0x11b5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x03	@ PACKER: left in the buffer by Resource_Overlay64D+0x11b3
	.endif
	.balign 4, 0

	.global Resource_Overlay64F
Resource_Overlay64F:
	.incbin "overlays/resource_64f.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x31, 0xe0	@ PACKER: left in the buffer by Resource_Overlay64E+0x7c6
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x0a, 0x31, 0x4e	@ PACKER: left in the buffer by Resource_Overlay64E+0x7c5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x0a, 0x31, 0x4e	@ PACKER: left in the buffer by Resource_Overlay64E+0x7c5
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x31, 0x4e	@ PACKER: left in the buffer by Resource_Overlay64E+0x7c6
	.endif
	.balign 4, 0

	.global Resource_Overlay650
Resource_Overlay650:
	.incbin "overlays/resource_650.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x91, 0xa4, 0x89	@ PACKER: left in the buffer by Resource_Overlay64D+0x185d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x89, 0x01	@ PACKER: left in the buffer by Resource_Overlay64D+0x185e
	.endif
	.balign 4, 0

	.global Resource_Overlay651
Resource_Overlay651:
	.incbin "overlays/resource_651.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x7c, 0x02	@ PACKER: left in the buffer by Resource_Overlay650+0x9ce
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay650+0x9d3
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay650+0x9d3
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay650+0x9d3
	.endif
	.balign 4, 0

	.global Resource_Overlay652
Resource_Overlay652:
	.incbin "overlays/resource_652.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xe3, 0x90, 0x59	@ PACKER: left in the buffer by Resource_Overlay650+0xb3d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x65	@ PACKER: left in the buffer by Resource_Overlay650+0xb37
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x36, 0xee, 0x8b	@ PACKER: left in the buffer by Resource_Overlay650+0xb31
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xee, 0x8b	@ PACKER: left in the buffer by Resource_Overlay650+0xb32
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x36, 0xee, 0x8b	@ PACKER: left in the buffer by Resource_Overlay650+0xb31
	.endif
	.balign 4, 0

	.global Resource_Overlay653
Resource_Overlay653:
	.incbin "overlays/resource_653.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x6f, 0x21, 0x42	@ PACKER: left in the buffer by Resource_Overlay64B+0x3235
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x42	@ PACKER: left in the buffer by Resource_Overlay64B+0x3237
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x7d, 0x09	@ PACKER: left in the buffer by Resource_Overlay64B+0x3236
	.endif
	.balign 4, 0

	.global Resource_Overlay654
Resource_Overlay654:
	.incbin "overlays/resource_654.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x01, 0x0a	@ PACKER: left in the buffer by Resource_Overlay653+0x24be
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x5c, 0x26, 0x04	@ PACKER: left in the buffer by Resource_Overlay653+0x24bd
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x26, 0x04, 0xab	@ PACKER: left in the buffer by Resource_Overlay653+0x24bd
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x04	@ PACKER: left in the buffer by Resource_Overlay653+0x24bb
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x26, 0x04, 0xab	@ PACKER: left in the buffer by Resource_Overlay653+0x24bd
	.endif
	.balign 4, 0

	.global Resource_Overlay655
Resource_Overlay655:
	.incbin "overlays/resource_655.lz"
	.ifdef TLA_EDITION_ES
	.byte 0x68, 0xd0, 0x15	@ PACKER: left in the buffer by Resource_Overlay653+0x2da9
	.endif
	.balign 4, 0

	.global Resource_Overlay656
Resource_Overlay656:
	.incbin "overlays/resource_656.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xe7	@ PACKER: left in the buffer by Resource_Overlay655+0x1cef
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x7c, 0x52, 0xcc	@ PACKER: left in the buffer by Resource_Overlay655+0x1cf9
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7c, 0x52, 0xcc	@ PACKER: left in the buffer by Resource_Overlay655+0x1cf9
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe0	@ PACKER: left in the buffer by Resource_Overlay655+0x1cf7
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe0	@ PACKER: left in the buffer by Resource_Overlay655+0x1cf7
	.endif
	.balign 4, 0

	.global Resource_Overlay657
Resource_Overlay657:
	.incbin "overlays/resource_657.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x06, 0xd1	@ PACKER: left in the buffer by Resource_Overlay656+0x1142
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xa0, 0x05	@ PACKER: left in the buffer by Resource_Overlay656+0x1142
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x15	@ PACKER: left in the buffer by Resource_Overlay656+0x114b
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x15	@ PACKER: left in the buffer by Resource_Overlay656+0x114b
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x1b, 0x15	@ PACKER: left in the buffer by Resource_Overlay656+0x114a
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x1b, 0x15	@ PACKER: left in the buffer by Resource_Overlay656+0x114a
	.endif
	.balign 4, 0

	.global Resource_Overlay658
Resource_Overlay658:
	.incbin "overlays/resource_658.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x79, 0x01	@ PACKER: left in the buffer by Resource_Overlay656+0x1606
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay656+0x1607
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x08	@ PACKER: left in the buffer by Resource_Overlay656+0x1607
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x51, 0x06, 0x08	@ PACKER: left in the buffer by Resource_Overlay656+0x1609
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x08	@ PACKER: left in the buffer by Resource_Overlay656+0x1607
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x08	@ PACKER: left in the buffer by Resource_Overlay656+0x1607
	.endif
	.balign 4, 0

	.global Resource_Overlay659
Resource_Overlay659:
	.incbin "overlays/resource_659.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x1a	@ PACKER: left in the buffer by Resource_Overlay658+0x14ab
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x39, 0x24, 0x09	@ PACKER: left in the buffer by Resource_Overlay658+0x14ad
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x1e, 0x39, 0x24	@ PACKER: left in the buffer by Resource_Overlay658+0x14ad
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x24, 0xde	@ PACKER: left in the buffer by Resource_Overlay658+0x14ae
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x24, 0xde	@ PACKER: left in the buffer by Resource_Overlay658+0x14ae
	.endif
	.balign 4, 0

	.global Resource_Overlay65A
Resource_Overlay65A:
	.incbin "overlays/resource_65a.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x84, 0xb1	@ PACKER: left in the buffer by Resource_Overlay659+0xe42
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xe0, 0x05, 0x28	@ PACKER: left in the buffer by Resource_Overlay659+0xe45
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xe0, 0x05, 0x28	@ PACKER: left in the buffer by Resource_Overlay659+0xe45
	.endif
	.balign 4, 0

	.global Resource_Overlay65B
Resource_Overlay65B:
	.incbin "overlays/resource_65b.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x4b, 0xe1, 0x25	@ PACKER: left in the buffer by Resource_Overlay65A+0x271
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x4b, 0xe1, 0x25	@ PACKER: left in the buffer by Resource_Overlay65A+0x271
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x25	@ PACKER: left in the buffer by Resource_Overlay65A+0x273
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x4b, 0xe1, 0x25	@ PACKER: left in the buffer by Resource_Overlay65A+0x271
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x4b, 0xe1, 0x25	@ PACKER: left in the buffer by Resource_Overlay65A+0x271
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x4b, 0xe1, 0x25	@ PACKER: left in the buffer by Resource_Overlay65A+0x271
	.endif
	.balign 4, 0

	.global Resource_Overlay65C
Resource_Overlay65C:
	.incbin "overlays/resource_65c.lz"
	.balign 4, 0

	.global Resource_Overlay65D
Resource_Overlay65D:
	.incbin "overlays/resource_65d.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x77, 0x7b	@ PACKER: left in the buffer by Resource_Overlay65A+0x6e2
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x7e, 0x77, 0x7b	@ PACKER: left in the buffer by Resource_Overlay65A+0x6e1
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xc4, 0x45, 0xcf	@ PACKER: left in the buffer by Resource_Overlay65A+0x6e5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xb7, 0x27	@ PACKER: left in the buffer by Resource_Overlay65A+0x6e2
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xb7, 0x27	@ PACKER: left in the buffer by Resource_Overlay65A+0x6e2
	.endif
	.balign 4, 0

	.global Resource_Overlay65E
Resource_Overlay65E:
	.incbin "overlays/resource_65e.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xf4	@ PACKER: left in the buffer by Resource_Overlay65D+0x56b
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x7b, 0x05	@ PACKER: left in the buffer by Resource_Overlay65D+0x56e
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xc8	@ PACKER: left in the buffer by Resource_Overlay65D+0x56b
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xc9, 0x03	@ PACKER: left in the buffer by Resource_Overlay65D+0x572
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x7c, 0x3a, 0x79	@ PACKER: left in the buffer by Resource_Overlay65D+0x571
	.endif
	.balign 4, 0

	.global Resource_Overlay65F
Resource_Overlay65F:
	.incbin "overlays/resource_65f.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x39	@ PACKER: left in the buffer by Resource_Overlay655+0x25a7
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x08, 0x36, 0x39	@ PACKER: left in the buffer by Resource_Overlay655+0x25a5
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x2d, 0xd0	@ PACKER: left in the buffer by Resource_Overlay655+0x25a2
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0xf3, 0x60	@ PACKER: left in the buffer by Resource_Overlay655+0x25a1
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x1e, 0x6c, 0x81	@ PACKER: left in the buffer by Resource_Overlay655+0x25a1
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x0f, 0xb6, 0x40	@ PACKER: left in the buffer by Resource_Overlay655+0x25a1
	.endif
	.balign 4, 0

	.global Resource_Overlay660
Resource_Overlay660:
	.incbin "overlays/resource_660.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x2c, 0x22, 0x29	@ PACKER: left in the buffer by Resource_Overlay65F+0x721
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x2c, 0x22	@ PACKER: left in the buffer by Resource_Overlay65F+0x722
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x13	@ PACKER: left in the buffer by Resource_Overlay65F+0x71f
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xbc, 0x11	@ PACKER: left in the buffer by Resource_Overlay65F+0x712
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x11, 0xbc, 0x11	@ PACKER: left in the buffer by Resource_Overlay65F+0x711
	.endif
	.balign 4, 0

	.global Resource_Overlay661
Resource_Overlay661:
	.incbin "overlays/resource_661.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xa1, 0xe2	@ PACKER: left in the buffer by Resource_Overlay653+0x321e
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x4a, 0x08, 0xe9	@ PACKER: left in the buffer by Resource_Overlay653+0x3221
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x4a	@ PACKER: left in the buffer by Resource_Overlay653+0x321f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe2, 0x4a	@ PACKER: left in the buffer by Resource_Overlay653+0x321e
	.endif
	.balign 4, 0

	.global Resource_Overlay662
Resource_Overlay662:
	.incbin "overlays/resource_662.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x94, 0xab, 0xf3	@ PACKER: left in the buffer by Resource_Overlay661+0x14bd
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x2f, 0xf3	@ PACKER: left in the buffer by Resource_Overlay661+0x14ba
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xf3	@ PACKER: left in the buffer by Resource_Overlay661+0x14bb
	.endif
	.balign 4, 0

	.global Resource_Overlay663
Resource_Overlay663:
	.incbin "overlays/resource_663.lz"
	.ifdef TLA_EDITION_DE
	.byte 0x35, 0xd7, 0xa4	@ PACKER: left in the buffer by Resource_Overlay661+0x294d
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe6, 0x9a, 0xb4	@ PACKER: left in the buffer by Resource_Overlay661+0x294d
	.endif
	.balign 4, 0

	.global Resource_Overlay664
Resource_Overlay664:
	.incbin "overlays/resource_664.lz"
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0x01, 0xa8	@ PACKER: left in the buffer by Field_Map097+0x4155
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x00, 0x01, 0xa8	@ PACKER: left in the buffer by Field_Map097+0x4155
	.endif
	.balign 4, 0

	.global Resource_Overlay665
Resource_Overlay665:
	.incbin "overlays/resource_665.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x8f, 0xb0	@ PACKER: left in the buffer by Resource_Overlay664+0x14fa
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xf7, 0xcd	@ PACKER: left in the buffer by Resource_Overlay664+0x14fa
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x88, 0x00, 0x3f	@ PACKER: left in the buffer by Resource_Overlay664+0x1521
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xf6	@ PACKER: left in the buffer by Resource_Overlay664+0x151f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xf6	@ PACKER: left in the buffer by Resource_Overlay664+0x151f
	.endif
	.balign 4, 0

	.global Resource_Overlay666
Resource_Overlay666:
	.incbin "overlays/resource_666.lz"
	.ifdef TLA_EDITION_DE
	.byte 0xe6, 0x01	@ PACKER: left in the buffer by Resource_Overlay665+0xa9a
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x66, 0x31, 0x33	@ PACKER: left in the buffer by Resource_Overlay665+0xa9d
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe6, 0x01	@ PACKER: left in the buffer by Resource_Overlay665+0xa9a
	.endif
	.balign 4, 0

	.global Resource_Overlay667
Resource_Overlay667:
	.incbin "overlays/resource_667.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x04, 0x0c	@ PACKER: left in the buffer by Resource_Overlay665+0xbf6
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x0c, 0xb6	@ PACKER: left in the buffer by Resource_Overlay665+0xbf6
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7b, 0x04, 0x0c	@ PACKER: left in the buffer by Resource_Overlay665+0xbf5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x7b, 0x04, 0x0c	@ PACKER: left in the buffer by Resource_Overlay665+0xbf5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x04, 0x0c	@ PACKER: left in the buffer by Resource_Overlay665+0xbf6
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x04, 0x0c	@ PACKER: left in the buffer by Resource_Overlay665+0xbf6
	.endif
	.balign 4, 0

	.global Resource_Overlay668
Resource_Overlay668:
	.incbin "overlays/resource_668.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x0f, 0x2d, 0x69	@ PACKER: left in the buffer by Resource_Overlay664+0x2985
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x41	@ PACKER: left in the buffer by Resource_Overlay664+0x2983
	.endif
	.balign 4, 0

	.global Resource_Overlay669
Resource_Overlay669:
	.incbin "overlays/resource_669.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xe6, 0x02, 0xf2	@ PACKER: left in the buffer by Resource_Overlay668+0x1459
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x02, 0xf2, 0x5b	@ PACKER: left in the buffer by Resource_Overlay668+0x1459
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xe6	@ PACKER: left in the buffer by Resource_Overlay668+0x1457
	.endif
	.balign 4, 0

	.global Resource_Overlay66A
Resource_Overlay66A:
	.incbin "overlays/resource_66a.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x08	@ PACKER: left in the buffer by Resource_Overlay669+0xd93
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x23, 0x4e, 0xad	@ PACKER: left in the buffer by Resource_Overlay669+0xda9
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x4e, 0x2f, 0x4c	@ PACKER: left in the buffer by Resource_Overlay669+0xda9
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x4e, 0x2f, 0x4c	@ PACKER: left in the buffer by Resource_Overlay669+0xda9
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x2f, 0x4c	@ PACKER: left in the buffer by Resource_Overlay669+0xdaa
	.endif
	.balign 4, 0

	.global Resource_Overlay66B
Resource_Overlay66B:
	.incbin "overlays/resource_66b.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay669+0x1273
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay669+0x1273
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x06, 0x60	@ PACKER: left in the buffer by Resource_Overlay669+0x1272
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x06, 0x60	@ PACKER: left in the buffer by Resource_Overlay669+0x1272
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x60	@ PACKER: left in the buffer by Resource_Overlay669+0x1273
	.endif
	.balign 4, 0

	.global Resource_Overlay66C
Resource_Overlay66C:
	.incbin "overlays/resource_66c.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x8c, 0x37, 0x35	@ PACKER: left in the buffer by Resource_Overlay66B+0x1d5
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x8c, 0x37, 0x35	@ PACKER: left in the buffer by Resource_Overlay66B+0x1d5
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x8c, 0x37, 0x35	@ PACKER: left in the buffer by Resource_Overlay66B+0x1d5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x8c, 0x37, 0x35	@ PACKER: left in the buffer by Resource_Overlay66B+0x1d5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x8c, 0x37, 0x35	@ PACKER: left in the buffer by Resource_Overlay66B+0x1d5
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x8c, 0x37, 0x35	@ PACKER: left in the buffer by Resource_Overlay66B+0x1d5
	.endif
	.balign 4, 0

	.global Resource_Overlay66D
Resource_Overlay66D:
	.incbin "overlays/resource_66d.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x80	@ PACKER: left in the buffer by Resource_Overlay66B+0x5af
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xbc, 0xd6, 0x80	@ PACKER: left in the buffer by Resource_Overlay66B+0x5ad
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xe0	@ PACKER: left in the buffer by Resource_Overlay66B+0x5a7
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe0	@ PACKER: left in the buffer by Resource_Overlay66B+0x5a7
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe0	@ PACKER: left in the buffer by Resource_Overlay66B+0x5a7
	.endif
	.balign 4, 0

	.global Resource_Overlay66E
Resource_Overlay66E:
	.incbin "overlays/resource_66e.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x89, 0x8d	@ PACKER: left in the buffer by Resource_Overlay66B+0xaa2
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x1c, 0xda	@ PACKER: left in the buffer by Resource_Overlay66B+0xa96
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xda	@ PACKER: left in the buffer by Resource_Overlay66B+0xa97
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x1c, 0xda	@ PACKER: left in the buffer by Resource_Overlay66B+0xa96
	.endif
	.balign 4, 0

	.global Resource_Overlay66F
Resource_Overlay66F:
	.incbin "overlays/resource_66f.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x3c	@ PACKER: left in the buffer by Resource_Overlay66B+0xde3
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x96, 0xa4, 0x3c	@ PACKER: left in the buffer by Resource_Overlay66B+0xde1
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x63	@ PACKER: left in the buffer by Resource_Overlay66B+0xddf
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x58, 0x92	@ PACKER: left in the buffer by Resource_Overlay66B+0xde2
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x1e, 0xe8	@ PACKER: left in the buffer by Resource_Overlay66B+0xde2
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay66B+0xde3
	.endif
	.balign 4, 0

	.global Resource_Overlay670
Resource_Overlay670:
	.incbin "overlays/resource_670.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x86, 0xb4, 0x2a	@ PACKER: left in the buffer by Resource_Overlay664+0x3751
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x53, 0x04, 0x61	@ PACKER: left in the buffer by Resource_Overlay664+0x375d
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x20	@ PACKER: left in the buffer by Resource_Overlay664+0x374f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x66, 0xe8	@ PACKER: left in the buffer by Resource_Overlay664+0x374a
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe8	@ PACKER: left in the buffer by Resource_Overlay664+0x374b
	.endif
	.balign 4, 0

	.global Resource_Overlay671
Resource_Overlay671:
	.incbin "overlays/resource_671.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x9f	@ PACKER: left in the buffer by Resource_Overlay664+0x404f
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x86, 0x3c, 0xc0	@ PACKER: left in the buffer by Resource_Overlay664+0x404d
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x0f, 0xb0, 0x30	@ PACKER: left in the buffer by Resource_Overlay664+0x404d
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x43, 0x1e, 0x60	@ PACKER: left in the buffer by Resource_Overlay664+0x404d
	.endif
	.balign 4, 0

	.global Resource_Overlay672
Resource_Overlay672:
	.incbin "overlays/resource_672.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x76	@ PACKER: left in the buffer by Resource_Overlay671+0x12b3
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x44	@ PACKER: left in the buffer by Resource_Overlay671+0x12e7
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xc1, 0xd1, 0x19	@ PACKER: left in the buffer by Resource_Overlay671+0x12e5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xd1, 0x19	@ PACKER: left in the buffer by Resource_Overlay671+0x12e6
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xd2	@ PACKER: left in the buffer by Resource_Overlay671+0x12e3
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe8, 0x8c, 0xc8	@ PACKER: left in the buffer by Resource_Overlay671+0x12e5
	.endif
	.balign 4, 0

	.global Resource_Overlay673
Resource_Overlay673:
	.incbin "overlays/resource_673.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x93, 0xc1	@ PACKER: left in the buffer by Resource_Overlay672+0xa6a
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay672+0xa67
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xe0, 0x54, 0x11	@ PACKER: left in the buffer by Resource_Overlay672+0xa65
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay672+0xa67
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x11, 0x3b	@ PACKER: left in the buffer by Resource_Overlay672+0xa66
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x54, 0x11, 0x3b	@ PACKER: left in the buffer by Resource_Overlay672+0xa65
	.endif
	.balign 4, 0

	.global Resource_Overlay674
Resource_Overlay674:
	.incbin "overlays/resource_674.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x11, 0xb4, 0x04	@ PACKER: left in the buffer by Resource_Overlay671+0x2aa9
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x02, 0x32, 0xca	@ PACKER: left in the buffer by Resource_Overlay671+0x2b29
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x04, 0x64	@ PACKER: left in the buffer by Resource_Overlay671+0x2b2a
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xb2, 0xb4	@ PACKER: left in the buffer by Resource_Overlay671+0x2b2e
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x02, 0x32	@ PACKER: left in the buffer by Resource_Overlay671+0x2b2a
	.endif
	.balign 4, 0

	.global Resource_Overlay675
Resource_Overlay675:
	.incbin "overlays/resource_675.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x18, 0xaa, 0x80	@ PACKER: left in the buffer by Resource_Overlay674+0xd91
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x43, 0xd0, 0x5e	@ PACKER: left in the buffer by Resource_Overlay674+0xd91
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x04	@ PACKER: left in the buffer by Resource_Overlay674+0xd93
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xe5, 0x43	@ PACKER: left in the buffer by Resource_Overlay674+0xd92
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x2f, 0xce	@ PACKER: left in the buffer by Resource_Overlay674+0xd92
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x2f, 0xce	@ PACKER: left in the buffer by Resource_Overlay674+0xd92
	.endif
	.balign 4, 0

	.global Resource_Overlay676
Resource_Overlay676:
	.incbin "overlays/resource_676.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x7f, 0xe3, 0xc0	@ PACKER: left in the buffer by Resource_Overlay674+0x15a1
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xdd, 0xf9, 0x00	@ PACKER: left in the buffer by Resource_Overlay674+0x15c9
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x09	@ PACKER: left in the buffer by Resource_Overlay674+0x15c7
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x20, 0xba, 0xf3	@ PACKER: left in the buffer by Resource_Overlay674+0x15c9
	.endif
	.balign 4, 0

	.global Resource_Overlay677
Resource_Overlay677:
	.incbin "overlays/resource_677.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x10	@ PACKER: left in the buffer by Resource_Overlay676+0x70b
	.endif
	.balign 4, 0

	.global Resource_Overlay678
Resource_Overlay678:
	.incbin "overlays/resource_678.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x21, 0xff	@ PACKER: left in the buffer by Resource_Overlay674+0x1eb2
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x43	@ PACKER: left in the buffer by Resource_Overlay674+0x1eeb
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x43	@ PACKER: left in the buffer by Resource_Overlay674+0x1eeb
	.endif
	.balign 4, 0

	.global Resource_Overlay679
Resource_Overlay679:
	.incbin "overlays/resource_679.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x24, 0xa8	@ PACKER: left in the buffer by Resource_Overlay678+0x18e2
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xce	@ PACKER: left in the buffer by Resource_Overlay678+0x18df
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x8f, 0x07	@ PACKER: left in the buffer by Resource_Overlay678+0x18e2
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xff, 0x73, 0xc6	@ PACKER: left in the buffer by Resource_Overlay678+0x18dd
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xc6	@ PACKER: left in the buffer by Resource_Overlay678+0x18df
	.endif
	.balign 4, 0

	.global Resource_Overlay67A
Resource_Overlay67A:
	.incbin "overlays/resource_67a.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x02, 0xa6	@ PACKER: left in the buffer by Resource_Overlay679+0x10a
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x02, 0xa6	@ PACKER: left in the buffer by Resource_Overlay679+0x10a
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x02, 0xa6	@ PACKER: left in the buffer by Resource_Overlay679+0x10a
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x02, 0xa6	@ PACKER: left in the buffer by Resource_Overlay679+0x10a
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x02, 0xa6	@ PACKER: left in the buffer by Resource_Overlay679+0x10a
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x02, 0xa6	@ PACKER: left in the buffer by Resource_Overlay679+0x10a
	.endif
	.balign 4, 0

	.global Resource_Overlay67B
Resource_Overlay67B:
	.incbin "overlays/resource_67b.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x69, 0x10, 0x35	@ PACKER: left in the buffer by Resource_Overlay674+0x1fdd
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x11, 0x4e, 0x76	@ PACKER: left in the buffer by Resource_Overlay674+0x1fdd
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x92, 0x22, 0x9c	@ PACKER: left in the buffer by Resource_Overlay674+0x1fdd
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x92, 0x22, 0x9c	@ PACKER: left in the buffer by Resource_Overlay674+0x1fdd
	.endif
	.balign 4, 0

	.global Resource_Overlay67C
Resource_Overlay67C:
	.incbin "overlays/resource_67c.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x9b, 0x12	@ PACKER: left in the buffer by Resource_Overlay671+0x3362
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x18, 0x95	@ PACKER: left in the buffer by Resource_Overlay671+0x336a
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x89	@ PACKER: left in the buffer by Resource_Overlay671+0x3367
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay671+0x3367
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x44, 0xbc, 0x44	@ PACKER: left in the buffer by Resource_Overlay671+0x3365
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x26, 0x0e, 0x18	@ PACKER: left in the buffer by Resource_Overlay671+0x3369
	.endif
	.balign 4, 0

	.global Resource_Overlay67D
Resource_Overlay67D:
	.incbin "overlays/resource_67d.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x42, 0x26, 0x21	@ PACKER: left in the buffer by Resource_Overlay67C+0x1161
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x09	@ PACKER: left in the buffer by Resource_Overlay67C+0x1163
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x74, 0x64, 0xd5	@ PACKER: left in the buffer by Resource_Overlay67C+0x1165
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x74, 0x64, 0xd5	@ PACKER: left in the buffer by Resource_Overlay67C+0x1165
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x74, 0x64, 0xd5	@ PACKER: left in the buffer by Resource_Overlay67C+0x1165
	.endif
	.balign 4, 0

	.global Resource_Overlay67E
Resource_Overlay67E:
	.incbin "overlays/resource_67e.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x90, 0xb6, 0x1e	@ PACKER: left in the buffer by Resource_Overlay67D+0xa79
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x33	@ PACKER: left in the buffer by Resource_Overlay67D+0xa87
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xa7, 0xcc, 0x33	@ PACKER: left in the buffer by Resource_Overlay67D+0xa85
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xa3, 0xd6, 0x03	@ PACKER: left in the buffer by Resource_Overlay67D+0xa81
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xa7, 0xcc, 0x33	@ PACKER: left in the buffer by Resource_Overlay67D+0xa85
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xcc, 0x33	@ PACKER: left in the buffer by Resource_Overlay67D+0xa86
	.endif
	.balign 4, 0

	.global Resource_Overlay67F
Resource_Overlay67F:
	.incbin "overlays/resource_67f.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x88, 0x06	@ PACKER: left in the buffer by Resource_Overlay67D+0xfbe
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x0c, 0x03	@ PACKER: left in the buffer by Resource_Overlay67D+0xfbe
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x03, 0x88	@ PACKER: left in the buffer by Resource_Overlay67D+0xfbe
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x06, 0x0c, 0x03	@ PACKER: left in the buffer by Resource_Overlay67D+0xfbd
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x0c, 0x03	@ PACKER: left in the buffer by Resource_Overlay67D+0xfbe
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x0c, 0x03	@ PACKER: left in the buffer by Resource_Overlay67D+0xfbe
	.endif
	.balign 4, 0

	.global Resource_Overlay680
Resource_Overlay680:
	.incbin "overlays/resource_680.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xd1, 0x5d	@ PACKER: left in the buffer by Resource_Overlay67F+0xcfa
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x22	@ PACKER: left in the buffer by Resource_Overlay67F+0xd0b
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0x73	@ PACKER: left in the buffer by Resource_Overlay67F+0xd0e
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x73	@ PACKER: left in the buffer by Resource_Overlay67F+0xd0f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x73	@ PACKER: left in the buffer by Resource_Overlay67F+0xd0f
	.endif
	.balign 4, 0

	.global Resource_Overlay681
Resource_Overlay681:
	.incbin "overlays/resource_681.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x7d, 0x00	@ PACKER: left in the buffer by Resource_Overlay671+0x36ba
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xe6, 0x3d, 0x49	@ PACKER: left in the buffer by Resource_Overlay671+0x3841
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x31, 0xef, 0x49	@ PACKER: left in the buffer by Resource_Overlay671+0x3841
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xb0	@ PACKER: left in the buffer by Resource_Overlay671+0x383b
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xa1	@ PACKER: left in the buffer by Resource_Overlay671+0x383f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x50	@ PACKER: left in the buffer by Resource_Overlay671+0x383f
	.endif
	.balign 4, 0

	.global Resource_Overlay682
Resource_Overlay682:
	.incbin "overlays/resource_682.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x60, 0xb0	@ PACKER: left in the buffer by Resource_Overlay681+0x14ba
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x05	@ PACKER: left in the buffer by Resource_Overlay681+0x14c7
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x80	@ PACKER: left in the buffer by Resource_Overlay681+0x14c7
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xa7, 0x60	@ PACKER: left in the buffer by Resource_Overlay681+0x14c6
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xe2, 0xa7, 0x60	@ PACKER: left in the buffer by Resource_Overlay681+0x14c5
	.endif
	.balign 4, 0

	.global Resource_Overlay683
Resource_Overlay683:
	.incbin "overlays/resource_683.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xa8	@ PACKER: left in the buffer by Resource_Overlay682+0x35b
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xa8	@ PACKER: left in the buffer by Resource_Overlay682+0x35b
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xa8	@ PACKER: left in the buffer by Resource_Overlay682+0x35b
	.endif
	.balign 4, 0

	.global Resource_Overlay684
Resource_Overlay684:
	.incbin "overlays/resource_684.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x66, 0x8c	@ PACKER: left in the buffer by Resource_Overlay683+0xce
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x66, 0x8c	@ PACKER: left in the buffer by Resource_Overlay683+0xce
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x66, 0x8c	@ PACKER: left in the buffer by Resource_Overlay683+0xce
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x66, 0x8c	@ PACKER: left in the buffer by Resource_Overlay683+0xce
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x66, 0x8c	@ PACKER: left in the buffer by Resource_Overlay683+0xce
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x66, 0x8c	@ PACKER: left in the buffer by Resource_Overlay683+0xce
	.endif
	.balign 4, 0

	.global Resource_Overlay685
Resource_Overlay685:
	.incbin "overlays/resource_685.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x07, 0x3d, 0x72	@ PACKER: left in the buffer by Resource_Overlay681+0x1ff5
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xdb, 0xec	@ PACKER: left in the buffer by Resource_Overlay681+0x1ff6
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x2a, 0xde, 0x22	@ PACKER: left in the buffer by Resource_Overlay681+0x1ffd
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x3a	@ PACKER: left in the buffer by Resource_Overlay681+0x1ffb
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x3a	@ PACKER: left in the buffer by Resource_Overlay681+0x1ffb
	.endif
	.balign 4, 0

	.global Resource_Overlay686
Resource_Overlay686:
	.incbin "overlays/resource_686.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x0a, 0xb6, 0x0a	@ PACKER: left in the buffer by Resource_Overlay685+0x1fe5
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x30	@ PACKER: left in the buffer by Resource_Overlay681+0x218f
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x73, 0xd5	@ PACKER: left in the buffer by Resource_Overlay681+0x218e
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xed	@ PACKER: left in the buffer by Resource_Overlay681+0x219b
	.endif
	.balign 4, 0

	.global Resource_Overlay687
Resource_Overlay687:
	.incbin "overlays/resource_687.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xc8	@ PACKER: left in the buffer by Resource_Overlay686+0x16df
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xef	@ PACKER: left in the buffer by Resource_Overlay686+0x16df
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xa6, 0xef	@ PACKER: left in the buffer by Resource_Overlay686+0x16de
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x4d, 0xdf	@ PACKER: left in the buffer by Resource_Overlay686+0x16de
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xa6, 0xef	@ PACKER: left in the buffer by Resource_Overlay686+0x16de
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xdf	@ PACKER: left in the buffer by Resource_Overlay686+0x16df
	.endif
	.balign 4, 0

	.global Resource_Overlay688
Resource_Overlay688:
	.incbin "overlays/resource_688.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x10, 0x6b	@ PACKER: left in the buffer by Resource_Overlay687+0x49e
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x10, 0x6b	@ PACKER: left in the buffer by Resource_Overlay687+0x49e
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x10, 0x6b	@ PACKER: left in the buffer by Resource_Overlay687+0x49e
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x10, 0x6b	@ PACKER: left in the buffer by Resource_Overlay687+0x49e
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x10, 0x6b	@ PACKER: left in the buffer by Resource_Overlay687+0x49e
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x10, 0x6b	@ PACKER: left in the buffer by Resource_Overlay687+0x49e
	.endif
	.balign 4, 0

	.global Resource_Overlay689
Resource_Overlay689:
	.incbin "overlays/resource_689.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xa3, 0x03	@ PACKER: left in the buffer by Resource_Overlay688+0x3e6
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x3a, 0xe0	@ PACKER: left in the buffer by Resource_Overlay688+0x3e6
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xe0	@ PACKER: left in the buffer by Resource_Overlay688+0x3e7
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x3a, 0xe0	@ PACKER: left in the buffer by Resource_Overlay688+0x3e6
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xa3, 0x03	@ PACKER: left in the buffer by Resource_Overlay688+0x3e6
	.endif
	.balign 4, 0

	.global Resource_Overlay68A
Resource_Overlay68A:
	.incbin "overlays/resource_68a.lz"
	.ifdef TLA_EDITION_DE
	.byte 0x91	@ PACKER: left in the buffer by Resource_Overlay687+0xc0f
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x91	@ PACKER: left in the buffer by Resource_Overlay687+0xc0f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x91	@ PACKER: left in the buffer by Resource_Overlay687+0xc0f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x91	@ PACKER: left in the buffer by Resource_Overlay687+0xc0f
	.endif
	.balign 4, 0

	.global Resource_Overlay68B
Resource_Overlay68B:
	.incbin "overlays/resource_68b.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x40, 0x0c	@ PACKER: left in the buffer by Resource_Overlay687+0x12de
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x81, 0xe8	@ PACKER: left in the buffer by Resource_Overlay687+0x12da
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x0c	@ PACKER: left in the buffer by Resource_Overlay687+0x12df
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x81, 0x1e	@ PACKER: left in the buffer by Resource_Overlay687+0x12e6
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xd5, 0x81, 0x1e	@ PACKER: left in the buffer by Resource_Overlay687+0x12e5
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xd5, 0x81, 0x1e	@ PACKER: left in the buffer by Resource_Overlay687+0x12e5
	.endif
	.balign 4, 0

	.global Resource_Overlay68C
Resource_Overlay68C:
	.incbin "overlays/resource_68c.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xc4, 0xd7	@ PACKER: left in the buffer by Resource_Overlay68B+0x1226
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xae	@ PACKER: left in the buffer by Resource_Overlay68B+0x1227
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7b, 0x8b, 0x7b	@ PACKER: left in the buffer by Resource_Overlay68B+0x1225
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xd7	@ PACKER: left in the buffer by Resource_Overlay68B+0x1227
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xbd	@ PACKER: left in the buffer by Resource_Overlay68B+0x1227
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xeb	@ PACKER: left in the buffer by Resource_Overlay68B+0x1227
	.endif
	.balign 4, 0

	.global Resource_Overlay68D
Resource_Overlay68D:
	.incbin "overlays/resource_68d.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x9e, 0xd1	@ PACKER: left in the buffer by Resource_Overlay686+0x1c22
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xdb, 0x1b, 0x18	@ PACKER: left in the buffer by Resource_Overlay686+0x1c25
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x60, 0xc8	@ PACKER: left in the buffer by Resource_Overlay686+0x1c26
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xd5, 0xde, 0x30	@ PACKER: left in the buffer by Resource_Overlay686+0x1c25
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x06, 0x86	@ PACKER: left in the buffer by Resource_Overlay686+0x1c26
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xb5, 0x37	@ PACKER: left in the buffer by Resource_Overlay686+0x1c26
	.endif
	.balign 4, 0

	.global Resource_Overlay68E
Resource_Overlay68E:
	.incbin "overlays/resource_68e.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xcb, 0x24	@ PACKER: left in the buffer by Resource_Overlay68D+0xdf6
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x27, 0xb8, 0x4c	@ PACKER: left in the buffer by Resource_Overlay68D+0xdf5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x27, 0xb8, 0x4c	@ PACKER: left in the buffer by Resource_Overlay68D+0xdf5
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x27, 0xb8, 0x4c	@ PACKER: left in the buffer by Resource_Overlay68D+0xdf5
	.endif
	.balign 4, 0

	.global Resource_Overlay68F
Resource_Overlay68F:
	.incbin "overlays/resource_68f.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xb9	@ PACKER: left in the buffer by Resource_Overlay681+0x244b
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xf6, 0x00, 0x83	@ PACKER: left in the buffer by Resource_Overlay681+0x246d
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x00, 0x83	@ PACKER: left in the buffer by Resource_Overlay681+0x246e
	.endif
	.balign 4, 0

	.global Resource_Overlay690
Resource_Overlay690:
	.incbin "overlays/resource_690.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x88, 0x04, 0x3f	@ PACKER: left in the buffer by Resource_Overlay68F+0x18f1
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x8f, 0xc4, 0x36	@ PACKER: left in the buffer by Resource_Overlay68F+0x1911
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x16	@ PACKER: left in the buffer by Resource_Overlay68F+0x190f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x16	@ PACKER: left in the buffer by Resource_Overlay68F+0x190f
	.endif
	.balign 4, 0

	.global Resource_Overlay691
Resource_Overlay691:
	.incbin "overlays/resource_691.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x48	@ PACKER: left in the buffer by Resource_Overlay681+0x2cc3
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x4a	@ PACKER: left in the buffer by Resource_Overlay681+0x2cc3
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xae	@ PACKER: left in the buffer by Resource_Overlay681+0x2cc3
	.endif
	.balign 4, 0

	.global Resource_Overlay692
Resource_Overlay692:
	.incbin "overlays/resource_692.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x59	@ PACKER: left in the buffer by Resource_Overlay671+0x39e7
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x17	@ PACKER: left in the buffer by Resource_Overlay671+0x39e7
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xc0, 0xb9	@ PACKER: left in the buffer by Resource_Overlay671+0x39e6
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x59, 0x18, 0x01	@ PACKER: left in the buffer by Resource_Overlay671+0x39e5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x11, 0x70	@ PACKER: left in the buffer by Resource_Overlay671+0x39e6
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x38	@ PACKER: left in the buffer by Resource_Overlay671+0x39e7
	.endif
	.balign 4, 0

	.global Resource_Overlay693
Resource_Overlay693:
	.incbin "overlays/resource_693.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xad, 0x18, 0x3a	@ PACKER: left in the buffer by Resource_Overlay692+0xbe5
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x5d	@ PACKER: left in the buffer by Resource_Overlay692+0xbe3
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x5d	@ PACKER: left in the buffer by Resource_Overlay692+0xbe3
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xe2, 0x5d	@ PACKER: left in the buffer by Resource_Overlay692+0xbe2
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x5d	@ PACKER: left in the buffer by Resource_Overlay692+0xbe3
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x5d	@ PACKER: left in the buffer by Resource_Overlay692+0xbe3
	.endif
	.balign 4, 0

	.global Resource_Overlay694
Resource_Overlay694:
	.incbin "overlays/resource_694.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x08, 0x81	@ PACKER: left in the buffer by Resource_Overlay692+0x2e02
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xbb, 0x80	@ PACKER: left in the buffer by Resource_Overlay692+0x2e1e
	.endif
	.balign 4, 0

	.global Resource_Overlay695
Resource_Overlay695:
	.incbin "overlays/resource_695.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x06, 0x20	@ PACKER: left in the buffer by Resource_Overlay694+0xb19
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x20	@ PACKER: left in the buffer by Resource_Overlay694+0xb1b
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x20, 0x1d	@ PACKER: left in the buffer by Resource_Overlay694+0xb1a
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x06, 0x20	@ PACKER: left in the buffer by Resource_Overlay694+0xb1a
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x06, 0x20, 0x02	@ PACKER: left in the buffer by Resource_Overlay694+0xb19
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x02	@ PACKER: left in the buffer by Resource_Overlay694+0xb1b
	.endif
	.balign 4, 0

	.global Resource_Overlay696
Resource_Overlay696:
	.incbin "overlays/resource_696.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x9c	@ PACKER: left in the buffer by Resource_Overlay692+0x3313
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xc4, 0xf2	@ PACKER: left in the buffer by Resource_Overlay692+0x331e
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x02, 0x67	@ PACKER: left in the buffer by Resource_Overlay692+0x3312
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x48	@ PACKER: left in the buffer by Resource_Overlay692+0x330f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xc0, 0xef, 0x8c	@ PACKER: left in the buffer by Resource_Overlay692+0x330d
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xc0	@ PACKER: left in the buffer by Resource_Overlay692+0x3313
	.endif
	.balign 4, 0

	.global Resource_Overlay697
Resource_Overlay697:
	.incbin "overlays/resource_697.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xcc	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xcc	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x01, 0x44, 0xcc	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x01, 0x44, 0xcc	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xcc	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.balign 4, 0

	.global Resource_Overlay698
Resource_Overlay698:
	.incbin "overlays/resource_698.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x50	@ PACKER: left in the buffer by Resource_Overlay697+0x1977
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x50	@ PACKER: left in the buffer by Resource_Overlay697+0x1977
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x80	@ PACKER: left in the buffer by Resource_Overlay697+0x1977
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xa0	@ PACKER: left in the buffer by Resource_Overlay697+0x1977
	.endif
	.balign 4, 0

	.global Resource_Overlay699
Resource_Overlay699:
	.incbin "overlays/resource_699.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x68	@ PACKER: left in the buffer by Resource_Overlay697+0x1af7
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x68	@ PACKER: left in the buffer by Resource_Overlay697+0x1af7
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xfc, 0x27, 0x47	@ PACKER: left in the buffer by Resource_Overlay697+0x1af5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xfc, 0x27, 0x47	@ PACKER: left in the buffer by Resource_Overlay697+0x1af5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe4, 0x68	@ PACKER: left in the buffer by Resource_Overlay697+0x1af6
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x1a	@ PACKER: left in the buffer by Resource_Overlay697+0x1af7
	.endif
	.balign 4, 0

	.global Resource_Overlay69A
Resource_Overlay69A:
	.incbin "overlays/resource_69a.lz"
	.ifdef TLA_EDITION_DE
	.byte 0x34	@ PACKER: left in the buffer by Resource_Overlay699+0x19ab
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x34	@ PACKER: left in the buffer by Resource_Overlay699+0x19ab
	.endif
	.balign 4, 0

	.global Resource_Overlay69B
Resource_Overlay69B:
	.incbin "overlays/resource_69b.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xae, 0x0d, 0x63	@ PACKER: left in the buffer by Resource_Overlay697+0x1efd
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xae, 0x0d, 0x63	@ PACKER: left in the buffer by Resource_Overlay697+0x1efd
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xda, 0x30, 0x96	@ PACKER: left in the buffer by Resource_Overlay697+0x1efd
	.endif
	.balign 4, 0

	.global Resource_Overlay69C
Resource_Overlay69C:
	.incbin "overlays/resource_69c.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x5a, 0x00, 0x0c	@ PACKER: left in the buffer by Resource_Overlay697+0x213d
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x6b, 0x01, 0x30	@ PACKER: left in the buffer by Resource_Overlay697+0x213d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xaf, 0x05	@ PACKER: left in the buffer by Resource_Overlay697+0x213e
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xaf, 0x05	@ PACKER: left in the buffer by Resource_Overlay697+0x213e
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xe1, 0xb5, 0x00	@ PACKER: left in the buffer by Resource_Overlay697+0x213d
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x5e, 0x0b, 0x80	@ PACKER: left in the buffer by Resource_Overlay697+0x213d
	.endif
	.balign 4, 0

	.global Resource_Overlay69D
Resource_Overlay69D:
	.incbin "overlays/resource_69d.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x13, 0x2b	@ PACKER: left in the buffer by Resource_Overlay69C+0x1cae
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x2b	@ PACKER: left in the buffer by Resource_Overlay69C+0x1caf
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x4e, 0xac, 0x08	@ PACKER: left in the buffer by Resource_Overlay69C+0x1cad
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xb1, 0x22	@ PACKER: left in the buffer by Resource_Overlay69C+0x1cae
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x22, 0xdc	@ PACKER: left in the buffer by Resource_Overlay69C+0x1cae
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x2b	@ PACKER: left in the buffer by Resource_Overlay69C+0x1caf
	.endif
	.balign 4, 0

	.global Resource_Overlay69E
Resource_Overlay69E:
	.incbin "overlays/resource_69e.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xc6, 0xc9, 0x4f	@ PACKER: left in the buffer by Resource_Overlay697+0x2469
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x27, 0x3f	@ PACKER: left in the buffer by Resource_Overlay697+0x246a
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xf0, 0x7b	@ PACKER: left in the buffer by Resource_Overlay697+0x2466
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xc9, 0x38, 0xf9	@ PACKER: left in the buffer by Resource_Overlay697+0x2469
	.endif
	.balign 4, 0

	.global Resource_Overlay69F
Resource_Overlay69F:
	.incbin "overlays/resource_69f.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x8e, 0x02, 0xf9	@ PACKER: left in the buffer by Resource_Overlay69E+0x1a85
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x85, 0xa3, 0x40	@ PACKER: left in the buffer by Resource_Overlay69E+0x1a85
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xb0	@ PACKER: left in the buffer by Resource_Overlay69E+0x1a83
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x04, 0xfb	@ PACKER: left in the buffer by Resource_Overlay69E+0x1a82
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x38, 0x0a, 0xe4	@ PACKER: left in the buffer by Resource_Overlay69E+0x1a85
	.endif
	.balign 4, 0

	.global Resource_Overlay6A0
Resource_Overlay6A0:
	.incbin "overlays/resource_6a0.lz"
	.ifdef TLA_EDITION_DE
	.byte 0xb8	@ PACKER: left in the buffer by Resource_Overlay69E+0x23af
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x65, 0x2e	@ PACKER: left in the buffer by Resource_Overlay69E+0x23ae
	.endif
	.balign 4, 0

	.global Resource_Overlay6A1
Resource_Overlay6A1:
	.incbin "overlays/resource_6a1.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x5a, 0x79, 0x1e	@ PACKER: left in the buffer by Resource_Overlay6A0+0x2059
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x5a, 0x79, 0x1e	@ PACKER: left in the buffer by Resource_Overlay6A0+0x2059
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xf3, 0xe8, 0xac	@ PACKER: left in the buffer by Resource_Overlay6A0+0x2059
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xe7, 0xd1, 0x59	@ PACKER: left in the buffer by Resource_Overlay6A0+0x2059
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x5a, 0x79, 0x1e	@ PACKER: left in the buffer by Resource_Overlay6A0+0x2059
	.endif
	.balign 4, 0

	.global Resource_Overlay6A2
Resource_Overlay6A2:
	.incbin "overlays/resource_6a2.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x1e, 0x20	@ PACKER: left in the buffer by Resource_Overlay6A1+0x1b4a
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x1e, 0x20	@ PACKER: left in the buffer by Resource_Overlay6A1+0x1b4a
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x78, 0x80, 0x0c	@ PACKER: left in the buffer by Resource_Overlay6A1+0x1b49
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x32, 0x60	@ PACKER: left in the buffer by Resource_Overlay6A1+0x1b4a
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x1e, 0x20	@ PACKER: left in the buffer by Resource_Overlay6A1+0x1b4a
	.endif
	.balign 4, 0

	.global Resource_Overlay6A3
Resource_Overlay6A3:
	.incbin "overlays/resource_6a3.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xd3, 0x65, 0x90	@ PACKER: left in the buffer by Resource_Overlay697+0x32b5
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7b, 0xba	@ PACKER: left in the buffer by Resource_Overlay697+0x32b6
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x7b, 0xba	@ PACKER: left in the buffer by Resource_Overlay697+0x32b6
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x60, 0x4f, 0x97	@ PACKER: left in the buffer by Resource_Overlay697+0x32b5
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x74, 0x19	@ PACKER: left in the buffer by Resource_Overlay697+0x32b6
	.endif
	.balign 4, 0

	.global Resource_Overlay6A4
Resource_Overlay6A4:
	.incbin "overlays/resource_6a4.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x05, 0x65, 0x89	@ PACKER: left in the buffer by Resource_Overlay6A3+0x245d
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x20, 0x24, 0x0a	@ PACKER: left in the buffer by Resource_Overlay6A3+0x245d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7b	@ PACKER: left in the buffer by Resource_Overlay6A3+0x245b
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xd3, 0x01, 0xf6	@ PACKER: left in the buffer by Resource_Overlay6A3+0x2459
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x12, 0x08, 0x89	@ PACKER: left in the buffer by Resource_Overlay6A3+0x245d
	.endif
	.balign 4, 0

	.global Resource_Overlay6A5
Resource_Overlay6A5:
	.incbin "overlays/resource_6a5.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x33, 0x43, 0x62	@ PACKER: left in the buffer by Resource_Overlay697+0x379d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x20, 0x14, 0x09	@ PACKER: left in the buffer by Resource_Overlay697+0x37a5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x15	@ PACKER: left in the buffer by Resource_Overlay697+0x37a3
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xa6, 0x42	@ PACKER: left in the buffer by Resource_Overlay697+0x37a2
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x62, 0x2a, 0xd4	@ PACKER: left in the buffer by Resource_Overlay697+0x37a1
	.endif
	.balign 4, 0

	.global Resource_Overlay6A6
Resource_Overlay6A6:
	.incbin "overlays/resource_6a6.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xee, 0xe7	@ PACKER: left in the buffer by Resource_Overlay6A5+0xfc6
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xe5, 0x91, 0x01	@ PACKER: left in the buffer by Resource_Overlay6A5+0xfc1
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xc0	@ PACKER: left in the buffer by Resource_Overlay6A5+0xfc3
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xfd, 0x7c	@ PACKER: left in the buffer by Resource_Overlay6A5+0xfc6
	.endif
	.balign 4, 0

	.global Resource_Overlay6A7
Resource_Overlay6A7:
	.incbin "overlays/resource_6a7.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x13, 0x77, 0x13	@ PACKER: left in the buffer by Resource_Overlay6A6+0x811
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xb8, 0x9b, 0xe0	@ PACKER: left in the buffer by Resource_Overlay6A6+0x811
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x00, 0xf2	@ PACKER: left in the buffer by Resource_Overlay6A6+0x80e
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x00, 0xf2	@ PACKER: left in the buffer by Resource_Overlay6A6+0x80e
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xb8, 0x9b, 0xe0	@ PACKER: left in the buffer by Resource_Overlay6A6+0x811
	.endif
	.balign 4, 0

	.global Resource_Overlay6A8
Resource_Overlay6A8:
	.incbin "overlays/resource_6a8.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x34, 0x64	@ PACKER: left in the buffer by Resource_Overlay6A5+0x2d16
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xdf, 0x1c, 0x5b	@ PACKER: left in the buffer by Resource_Overlay6A5+0x2d65
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7c, 0x28, 0x51	@ PACKER: left in the buffer by Resource_Overlay6A5+0x2d69
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x37, 0xab, 0xef	@ PACKER: left in the buffer by Resource_Overlay6A5+0x2d61
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x0b	@ PACKER: left in the buffer by Resource_Overlay6A5+0x2d5b
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x0b	@ PACKER: left in the buffer by Resource_Overlay6A5+0x2d5b
	.endif
	.balign 4, 0

	.global Resource_Overlay6A9
Resource_Overlay6A9:
	.incbin "overlays/resource_6a9.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x33, 0x0b, 0x01	@ PACKER: left in the buffer by Resource_Overlay6A8+0xd6d
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x68, 0x83, 0x60	@ PACKER: left in the buffer by Resource_Overlay6A8+0xd6d
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x60	@ PACKER: left in the buffer by Resource_Overlay6A8+0xd6f
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay6A8+0xd6f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay6A8+0xd6f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x83, 0x60, 0x11	@ PACKER: left in the buffer by Resource_Overlay6A8+0xd6d
	.endif
	.balign 4, 0

	.global Resource_Overlay6AA
Resource_Overlay6AA:
	.incbin "overlays/resource_6aa.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x14, 0x81	@ PACKER: left in the buffer by Resource_Overlay697+0x3b9e
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xe7, 0x80	@ PACKER: left in the buffer by Resource_Overlay697+0x3c06
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xac, 0x10, 0xe8	@ PACKER: left in the buffer by Resource_Overlay697+0x3c05
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Resource_Overlay697+0x3c06
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xfb, 0x39	@ PACKER: left in the buffer by Resource_Overlay697+0x3c06
	.endif
	.balign 4, 0

	.global Resource_Overlay6AB
Resource_Overlay6AB:
	.incbin "overlays/resource_6ab.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xb8, 0x30	@ PACKER: left in the buffer by Resource_Overlay6AA+0x2de2
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xca, 0x90	@ PACKER: left in the buffer by Resource_Overlay6AA+0x2f06
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x90	@ PACKER: left in the buffer by Resource_Overlay6AA+0x2f07
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x92, 0x0c, 0x57	@ PACKER: left in the buffer by Resource_Overlay6AA+0x2f09
	.endif
	.balign 4, 0

	.global Resource_Overlay6AC
Resource_Overlay6AC:
	.incbin "overlays/resource_6ac.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x61	@ PACKER: left in the buffer by Resource_Overlay6AB+0x2273
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x7a, 0x0e, 0x0e	@ PACKER: left in the buffer by Resource_Overlay6AB+0x2271
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x38, 0xac	@ PACKER: left in the buffer by Resource_Overlay6AB+0x2272
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x7a, 0x0e	@ PACKER: left in the buffer by Resource_Overlay6AB+0x2272
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x0e	@ PACKER: left in the buffer by Resource_Overlay6AB+0x2273
	.endif
	.balign 4, 0

	.global Resource_Overlay6AD
Resource_Overlay6AD:
	.incbin "overlays/resource_6ad.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x01, 0xff, 0x00	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xff, 0x00	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x01, 0xff, 0x00	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x01, 0xff, 0x00	@ PACKER: unused post-stream residue; Resource_LoadCode uses decoded RAM and output length; writer unknown
	.endif
	.balign 4, 0

	.global Resource_Overlay6AE
Resource_Overlay6AE:
	.incbin "overlays/resource_6ae.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x09, 0x23, 0x30	@ PACKER: left in the buffer by Resource_Overlay6AD+0x3245
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x34, 0x6b	@ PACKER: left in the buffer by Resource_Overlay6AD+0x3342
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x6b	@ PACKER: left in the buffer by Resource_Overlay6AD+0x3343
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x6b	@ PACKER: left in the buffer by Resource_Overlay6AD+0x3343
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x6b	@ PACKER: left in the buffer by Resource_Overlay6AD+0x3343
	.endif
	.balign 4, 0

	.global Resource_Overlay6AF
Resource_Overlay6AF:
	.incbin "overlays/resource_6af.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x03	@ PACKER: left in the buffer by Resource_Overlay6AE+0xd7f
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xbc, 0x43, 0xac	@ PACKER: left in the buffer by Resource_Overlay6AE+0xd7d
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xac	@ PACKER: left in the buffer by Resource_Overlay6AE+0xd7f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xac	@ PACKER: left in the buffer by Resource_Overlay6AE+0xd7f
	.endif
	.balign 4, 0

	.global Resource_Overlay6B0
Resource_Overlay6B0:
	.incbin "overlays/resource_6b0.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xc4, 0x01	@ PACKER: left in the buffer by Resource_Overlay6AE+0x28b2
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x02	@ PACKER: left in the buffer by Resource_Overlay6AE+0x28b3
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x04, 0x01, 0x02	@ PACKER: left in the buffer by Resource_Overlay6AE+0x28b1
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x02, 0x04, 0x01	@ PACKER: left in the buffer by Resource_Overlay6AE+0x28b1
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x02, 0x04, 0x01	@ PACKER: left in the buffer by Resource_Overlay6AE+0x28b1
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x01	@ PACKER: left in the buffer by Resource_Overlay6AE+0x28b3
	.endif
	.balign 4, 0

	.global Resource_Overlay6B1
Resource_Overlay6B1:
	.incbin "overlays/resource_6b1.lz"
	.ifdef TLA_EDITION_DE
	.byte 0xf4, 0x21, 0xd8	@ PACKER: left in the buffer by Resource_Overlay6B0+0x15a9
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xf4, 0x21, 0xd8	@ PACKER: left in the buffer by Resource_Overlay6B0+0x15a9
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xf4, 0x21, 0xd8	@ PACKER: left in the buffer by Resource_Overlay6B0+0x15a9
	.endif
	.balign 4, 0

	.global Resource_Overlay6B2
Resource_Overlay6B2:
	.incbin "overlays/resource_6b2.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x10	@ PACKER: left in the buffer by Resource_Overlay6B1+0x42f
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x99	@ PACKER: left in the buffer by Resource_Overlay6B1+0x42f
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x99	@ PACKER: left in the buffer by Resource_Overlay6B1+0x42f
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x99	@ PACKER: left in the buffer by Resource_Overlay6B1+0x42f
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x99	@ PACKER: left in the buffer by Resource_Overlay6B1+0x42f
	.endif
	.balign 4, 0

	.global Resource_Overlay6B3
Resource_Overlay6B3:
	.incbin "overlays/resource_6b3.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x14, 0xa6	@ PACKER: left in the buffer by Resource_Overlay6B1+0x552
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x4a, 0x4e	@ PACKER: left in the buffer by Resource_Overlay6B1+0x552
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x0c, 0x2c	@ PACKER: left in the buffer by Resource_Overlay6B1+0x562
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x45, 0xc9	@ PACKER: left in the buffer by Resource_Overlay6B1+0x552
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x45, 0xc9	@ PACKER: left in the buffer by Resource_Overlay6B1+0x552
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x45, 0xc9	@ PACKER: left in the buffer by Resource_Overlay6B1+0x552
	.endif
	.balign 4, 0

	.global Resource_Overlay6B4
Resource_Overlay6B4:
	.incbin "overlays/resource_6b4.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x80	@ PACKER: left in the buffer by Resource_Overlay6B1+0xe4f
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x57	@ PACKER: left in the buffer by Resource_Overlay6B1+0xe4f
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0xe2, 0x04	@ PACKER: left in the buffer by Resource_Overlay6B1+0xe4e
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x9c, 0xe0	@ PACKER: left in the buffer by Resource_Overlay6B1+0xe4e
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x7d, 0xe2, 0x04	@ PACKER: left in the buffer by Resource_Overlay6B1+0xe4d
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x9c, 0xe0	@ PACKER: left in the buffer by Resource_Overlay6B1+0xe4e
	.endif
	.balign 4, 0

	.global Resource_Overlay6B5
Resource_Overlay6B5:
	.incbin "overlays/resource_6b5.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xbf, 0xec, 0x7f	@ PACKER: left in the buffer by Resource_Overlay6B1+0x13e5
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0xc7, 0x83	@ PACKER: left in the buffer by Resource_Overlay6B1+0x13e6
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x07, 0xaf, 0xff	@ PACKER: left in the buffer by Resource_Overlay6B1+0x13f5
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0xe0, 0xf5, 0xbf	@ PACKER: left in the buffer by Resource_Overlay6B1+0x13f5
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0xfd	@ PACKER: left in the buffer by Resource_Overlay6B1+0x13f7
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0xaf, 0xff	@ PACKER: left in the buffer by Resource_Overlay6B1+0x13f6
	.endif
	.balign 4, 0

	.global Resource_Overlay6B6
Resource_Overlay6B6:
	.incbin "overlays/resource_6b6.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x74, 0x08	@ PACKER: left in the buffer by Resource_Overlay6B5+0xae
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x74, 0x08	@ PACKER: left in the buffer by Resource_Overlay6B5+0xae
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x74, 0x08	@ PACKER: left in the buffer by Resource_Overlay6B5+0xae
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x74, 0x08	@ PACKER: left in the buffer by Resource_Overlay6B5+0xae
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x74, 0x08	@ PACKER: left in the buffer by Resource_Overlay6B5+0xae
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x74, 0x08	@ PACKER: left in the buffer by Resource_Overlay6B5+0xae
	.endif
	.balign 4, 0

	.global Resource_Overlay6B7
Resource_Overlay6B7:
	.incbin "overlays/resource_6b7.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x9b	@ PACKER: left in the buffer by Resource_Overlay6B5+0x76f
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x09, 0x9b	@ PACKER: left in the buffer by Resource_Overlay6B5+0x76e
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x03, 0x3b, 0x79	@ PACKER: left in the buffer by Resource_Overlay6B5+0x771
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x3b, 0x79	@ PACKER: left in the buffer by Resource_Overlay6B5+0x772
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x3b, 0x79	@ PACKER: left in the buffer by Resource_Overlay6B5+0x772
	.endif
	.balign 4, 0

	.global Resource_Overlay6B8
Resource_Overlay6B8:
	.incbin "overlays/resource_6b8.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x12, 0x1a, 0x28	@ PACKER: left in the buffer by Resource_Overlay6B5+0xcdd
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x9c, 0x0d	@ PACKER: left in the buffer by Resource_Overlay6B5+0xcd6
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x72	@ PACKER: left in the buffer by Resource_Overlay6B5+0xcd3
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x79, 0xac	@ PACKER: left in the buffer by Resource_Overlay6B5+0xcd6
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x2c, 0xf3	@ PACKER: left in the buffer by Resource_Overlay6B5+0xcd6
	.endif
	.balign 4, 0

	.global Resource_Overlay6B9
Resource_Overlay6B9:
	.incbin "overlays/resource_6b9.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x2f, 0x2c	@ PACKER: left in the buffer by Resource_Overlay6B8+0x516
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x2c, 0x82	@ PACKER: left in the buffer by Resource_Overlay6B8+0x516
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x85, 0x45	@ PACKER: left in the buffer by Resource_Overlay6B8+0x516
	.endif
	.ifdef TLA_EDITION_ES
	.byte 0x85, 0x45	@ PACKER: left in the buffer by Resource_Overlay6B8+0x516
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x45, 0x70	@ PACKER: left in the buffer by Resource_Overlay6B8+0x516
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x45, 0x70	@ PACKER: left in the buffer by Resource_Overlay6B8+0x516
	.endif
	.balign 4, 0

	.global Resource_Overlay6BA
Resource_Overlay6BA:
	.incbin "overlays/resource_6ba.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x2c, 0x81	@ PACKER: left in the buffer by Resource_Overlay6B8+0xaaa
	.endif
	.ifdef TLA_EDITION_EN
	.byte 0x42, 0x03	@ PACKER: left in the buffer by Resource_Overlay6B8+0xa9a
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x27	@ PACKER: left in the buffer by Resource_Overlay6B8+0xa9b
	.endif
	.ifdef TLA_EDITION_FR
	.byte 0x1a, 0x00	@ PACKER: left in the buffer by Resource_Overlay6B8+0xa9a
	.endif
	.ifdef TLA_EDITION_IT
	.byte 0x00, 0x21, 0x34	@ PACKER: left in the buffer by Resource_Overlay6B8+0xa99
	.endif
	.balign 4, 0
