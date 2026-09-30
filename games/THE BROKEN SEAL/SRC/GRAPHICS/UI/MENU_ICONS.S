@ The 16x16 icons the menus draw with the system palette's first row: the
@ item, Psynergy and misc tables, each a word pointer per icon and a -1
@ word, then each icon's pixels in the 4-bit icon coder, padded to a word.
	.section .rodata
	.balign 4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.4byte UiIcon_Item000
	.4byte UiIcon_Item001
	.4byte UiIcon_Item002
	.4byte UiIcon_Item003
	.4byte UiIcon_Item004
	.4byte UiIcon_Item005
	.4byte UiIcon_Item006
	.4byte UiIcon_Item007
	.4byte UiIcon_Item008
	.4byte UiIcon_Item009
	.4byte UiIcon_Item010
	.4byte UiIcon_Item011
	.4byte UiIcon_Item012
	.4byte UiIcon_Item013
	.4byte UiIcon_Item014
	.4byte UiIcon_Item015
	.4byte UiIcon_Item016
	.4byte UiIcon_Item017
	.4byte UiIcon_Item018
	.4byte UiIcon_Item019
	.4byte UiIcon_Item020
	.4byte UiIcon_Item021
	.4byte UiIcon_Item022
	.4byte UiIcon_Item023
	.4byte UiIcon_Item024
	.4byte UiIcon_Item025
	.4byte UiIcon_Item026
	.4byte UiIcon_Item027
	.4byte UiIcon_Item028
	.4byte UiIcon_Item029
	.4byte UiIcon_Item030
	.4byte UiIcon_Item031
	.4byte UiIcon_Item032
	.4byte UiIcon_Item033
	.4byte UiIcon_Item034
	.4byte UiIcon_Item035
	.4byte UiIcon_Item036
	.4byte UiIcon_Item037
	.4byte UiIcon_Item038
	.4byte UiIcon_Item039
	.4byte UiIcon_Item040
	.4byte UiIcon_Item041
	.4byte UiIcon_Item042
	.4byte UiIcon_Item043
	.4byte UiIcon_Item044
	.4byte UiIcon_Item045
	.4byte UiIcon_Item046
	.4byte UiIcon_Item047
	.4byte UiIcon_Item048
	.4byte UiIcon_Item049
	.4byte UiIcon_Item050
	.4byte UiIcon_Item051
	.4byte UiIcon_Item052
	.4byte UiIcon_Item053
	.4byte UiIcon_Item054
	.4byte UiIcon_Item055
	.4byte UiIcon_Item056
	.4byte UiIcon_Item057
	.4byte UiIcon_Item058
	.4byte UiIcon_Item059
	.4byte UiIcon_Item060
	.4byte UiIcon_Item061
	.4byte UiIcon_Item062
	.4byte UiIcon_Item063
	.4byte UiIcon_Item064
	.4byte UiIcon_Item065
	.4byte UiIcon_Item066
	.4byte UiIcon_Item067
	.4byte UiIcon_Item068
	.4byte UiIcon_Item069
	.4byte UiIcon_Item070
	.4byte UiIcon_Item071
	.4byte UiIcon_Item072
	.4byte UiIcon_Item073
	.4byte UiIcon_Item074
	.4byte UiIcon_Item075
	.4byte UiIcon_Item076
	.4byte UiIcon_Item077
	.4byte UiIcon_Item078
	.4byte UiIcon_Item079
	.4byte UiIcon_Item080
	.4byte UiIcon_Item081
	.4byte UiIcon_Item082
	.4byte UiIcon_Item083
	.4byte UiIcon_Item084
	.4byte UiIcon_Item085
	.4byte UiIcon_Item086
	.4byte UiIcon_Item087
	.4byte UiIcon_Item088
	.4byte UiIcon_Item089
	.4byte UiIcon_Item090
	.4byte UiIcon_Item091
	.4byte UiIcon_Item092
	.4byte UiIcon_Item093
	.4byte UiIcon_Item094
	.4byte UiIcon_Item095
	.4byte UiIcon_Item096
	.4byte UiIcon_Item097
	.4byte UiIcon_Item098
	.4byte UiIcon_Item099
	.4byte UiIcon_Item100
	.4byte UiIcon_Item101
	.4byte UiIcon_Item102
	.4byte UiIcon_Item103
	.4byte UiIcon_Item104
	.4byte UiIcon_Item105
	.4byte UiIcon_Item106
	.4byte UiIcon_Item107
	.4byte UiIcon_Item108
	.4byte UiIcon_Item109
	.4byte UiIcon_Item110
	.4byte UiIcon_Item111
	.4byte UiIcon_Item112
	.4byte UiIcon_Item113
	.4byte UiIcon_Item114
	.4byte UiIcon_Item115
	.4byte UiIcon_Item116
	.4byte UiIcon_Item117
	.4byte UiIcon_Item118
	.4byte UiIcon_Item119
	.4byte UiIcon_Item120
	.4byte UiIcon_Item121
	.4byte UiIcon_Item122
	.4byte UiIcon_Item123
	.4byte UiIcon_Item124
	.4byte UiIcon_Item125
	.4byte UiIcon_Item126
	.4byte UiIcon_Item127
	.4byte UiIcon_Item128
	.4byte UiIcon_Item129
	.4byte UiIcon_Item130
	.4byte UiIcon_Item131
	.4byte UiIcon_Item132
	.4byte UiIcon_Item133
	.4byte UiIcon_Item134
	.4byte UiIcon_Item135
	.4byte UiIcon_Item136
	.4byte UiIcon_Item137
	.4byte UiIcon_Item138
	.4byte UiIcon_Item139
	.4byte UiIcon_Item140
	.4byte UiIcon_Item141
	.4byte UiIcon_Item142
	.4byte UiIcon_Item143
	.4byte UiIcon_Item144
	.4byte UiIcon_Item145
	.4byte UiIcon_Item146
	.4byte UiIcon_Item147
	.4byte UiIcon_Item148
	.4byte UiIcon_Item149
	.4byte UiIcon_Item150
	.4byte UiIcon_Item151
	.4byte UiIcon_Item152
	.4byte UiIcon_Item153
	.4byte UiIcon_Item154
	.4byte UiIcon_Item155
	.4byte UiIcon_Item156
	.4byte UiIcon_Item157
	.4byte UiIcon_Item158
	.4byte UiIcon_Item159
	.4byte UiIcon_Item160
	.4byte UiIcon_Item161
	.4byte UiIcon_Item162
	.4byte UiIcon_Item163
	.4byte UiIcon_Item164
	.4byte UiIcon_Item165
	.4byte UiIcon_Item166
	.4byte UiIcon_Item167
	.4byte UiIcon_Item168
	.4byte UiIcon_Item169
	.4byte UiIcon_Item170
	.4byte UiIcon_Item171
	.4byte UiIcon_Item172
	.4byte UiIcon_Item173
	.4byte UiIcon_Item174
	.4byte UiIcon_Item175
	.4byte UiIcon_Item176
	.4byte UiIcon_Item177
	.4byte UiIcon_Item178
	.4byte UiIcon_Item179
	.4byte UiIcon_Item180
	.4byte UiIcon_Item181
	.4byte UiIcon_Item182
	.4byte UiIcon_Item183
	.4byte UiIcon_Item184
	.4byte UiIcon_Item185
	.4byte UiIcon_Item186
	.4byte UiIcon_Item187
	.4byte UiIcon_Item188
	.4byte UiIcon_Item189
	.4byte UiIcon_Item190
	.4byte UiIcon_Item191
	.4byte UiIcon_Item192
	.4byte UiIcon_Item193
	.4byte UiIcon_Item194
	.4byte UiIcon_Item195
	.4byte UiIcon_Item196
	.4byte UiIcon_Item197
	.4byte UiIcon_Item198
	.4byte UiIcon_Item199
	.4byte UiIcon_Item200
	.4byte UiIcon_Item201
	.4byte UiIcon_Item202
	.4byte UiIcon_Item203
	.4byte UiIcon_Item204
	.4byte UiIcon_Item205
	.4byte UiIcon_Item206
	.4byte UiIcon_Item207
	.4byte UiIcon_Item208
	.4byte UiIcon_Item209
	.4byte UiIcon_Item210
	.4byte UiIcon_Item211
	.4byte UiIcon_Item212
	.4byte UiIcon_Item213
	.4byte UiIcon_Item214
	.4byte UiIcon_Item215
	.4byte UiIcon_Item216
	.4byte UiIcon_Item217
	.4byte UiIcon_Item218
	.4byte UiIcon_Item219
	.4byte UiIcon_Item220
	.4byte UiIcon_Item221
	.4byte UiIcon_Item222
	.4byte UiIcon_Item223
	.4byte UiIcon_Item224
	.4byte UiIcon_Item225
	.4byte UiIcon_Item226
	.4byte UiIcon_Item227
	.4byte UiIcon_Item228
	.4byte UiIcon_Item229
	.4byte UiIcon_Item230
	.4byte UiIcon_Item231
	.4byte UiIcon_Item232
	.4byte UiIcon_Item233
	.4byte UiIcon_Item234
	.4byte UiIcon_Item235
	.4byte UiIcon_Item236
	.4byte UiIcon_Item237
	.4byte UiIcon_Item238
	.4byte UiIcon_Item239
	.4byte UiIcon_Item240
	.4byte UiIcon_Item241
	.4byte UiIcon_Item242
	.4byte UiIcon_Item243
	.4byte UiIcon_Item244
	.4byte UiIcon_Item245
	.4byte UiIcon_Item246
	.4byte UiIcon_Item247
	.4byte UiIcon_Item248
	.4byte UiIcon_Item249
	.4byte UiIcon_Item250
	.4byte UiIcon_Item251
	.4byte UiIcon_Item252
	.4byte UiIcon_Item253
	.4byte UiIcon_Item254
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.4byte -1
UiIcon_Item000:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_000.icon4"
UiIcon_Item001:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_001.icon4"
UiIcon_Item002:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_002.icon4"
UiIcon_Item003:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_003.icon4"
UiIcon_Item004:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_004.icon4"
UiIcon_Item005:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_005.icon4"
UiIcon_Item006:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_006.icon4"
UiIcon_Item007:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_007.icon4"
UiIcon_Item008:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_008.icon4"
UiIcon_Item009:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_009.icon4"
UiIcon_Item010:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_010.icon4"
UiIcon_Item011:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_011.icon4"
UiIcon_Item012:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_012.icon4"
UiIcon_Item013:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_013.icon4"
UiIcon_Item014:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_014.icon4"
UiIcon_Item015:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_015.icon4"
UiIcon_Item016:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_016.icon4"
UiIcon_Item017:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_017.icon4"
UiIcon_Item018:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_018.icon4"
UiIcon_Item019:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_019.icon4"
UiIcon_Item020:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_020.icon4"
UiIcon_Item021:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_021.icon4"
UiIcon_Item022:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_022.icon4"
UiIcon_Item023:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_023.icon4"
UiIcon_Item024:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_024.icon4"
UiIcon_Item025:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_025.icon4"
UiIcon_Item026:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_026.icon4"
UiIcon_Item027:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_027.icon4"
UiIcon_Item028:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_028.icon4"
UiIcon_Item029:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_029.icon4"
UiIcon_Item030:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_030.icon4"
UiIcon_Item031:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_031.icon4"
UiIcon_Item032:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_032.icon4"
UiIcon_Item033:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_033.icon4"
UiIcon_Item034:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_034.icon4"
UiIcon_Item035:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_035.icon4"
UiIcon_Item036:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_036.icon4"
UiIcon_Item037:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_037.icon4"
UiIcon_Item038:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_038.icon4"
UiIcon_Item039:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_039.icon4"
UiIcon_Item040:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_040.icon4"
UiIcon_Item041:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_041.icon4"
UiIcon_Item042:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_042.icon4"
UiIcon_Item043:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_043.icon4"
UiIcon_Item044:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_044.icon4"
UiIcon_Item045:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_045.icon4"
UiIcon_Item046:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_046.icon4"
UiIcon_Item047:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_047.icon4"
UiIcon_Item048:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_048.icon4"
UiIcon_Item049:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_049.icon4"
UiIcon_Item050:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_050.icon4"
UiIcon_Item051:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_051.icon4"
UiIcon_Item052:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_052.icon4"
UiIcon_Item053:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_053.icon4"
UiIcon_Item054:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_054.icon4"
UiIcon_Item055:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_055.icon4"
UiIcon_Item056:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_056.icon4"
UiIcon_Item057:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_057.icon4"
UiIcon_Item058:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_058.icon4"
UiIcon_Item059:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_059.icon4"
UiIcon_Item060:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_060.icon4"
UiIcon_Item061:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_061.icon4"
UiIcon_Item062:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_062.icon4"
UiIcon_Item063:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_063.icon4"
UiIcon_Item064:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_064.icon4"
UiIcon_Item065:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_065.icon4"
UiIcon_Item066:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_066.icon4"
UiIcon_Item067:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_067.icon4"
UiIcon_Item068:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_068.icon4"
UiIcon_Item069:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_069.icon4"
UiIcon_Item070:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_070.icon4"
UiIcon_Item071:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_071.icon4"
UiIcon_Item072:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_072.icon4"
UiIcon_Item073:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_073.icon4"
UiIcon_Item074:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_074.icon4"
UiIcon_Item075:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_075.icon4"
UiIcon_Item076:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_076.icon4"
UiIcon_Item077:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_077.icon4"
UiIcon_Item078:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_078.icon4"
UiIcon_Item079:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_079.icon4"
UiIcon_Item080:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_080.icon4"
UiIcon_Item081:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_081.icon4"
UiIcon_Item082:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_082.icon4"
UiIcon_Item083:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_083.icon4"
UiIcon_Item084:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_084.icon4"
UiIcon_Item085:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_085.icon4"
UiIcon_Item086:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_086.icon4"
UiIcon_Item087:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_087.icon4"
UiIcon_Item088:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_088.icon4"
UiIcon_Item089:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_089.icon4"
UiIcon_Item090:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_090.icon4"
UiIcon_Item091:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_091.icon4"
UiIcon_Item092:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_092.icon4"
UiIcon_Item093:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_093.icon4"
UiIcon_Item094:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_094.icon4"
UiIcon_Item095:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_095.icon4"
UiIcon_Item096:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_096.icon4"
UiIcon_Item097:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_097.icon4"
UiIcon_Item098:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_098.icon4"
UiIcon_Item099:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_099.icon4"
UiIcon_Item100:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_100.icon4"
UiIcon_Item101:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_101.icon4"
UiIcon_Item102:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_102.icon4"
UiIcon_Item103:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_103.icon4"
UiIcon_Item104:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_104.icon4"
UiIcon_Item105:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_105.icon4"
UiIcon_Item106:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_106.icon4"
UiIcon_Item107:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_107.icon4"
UiIcon_Item108:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_108.icon4"
UiIcon_Item109:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_109.icon4"
UiIcon_Item110:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_110.icon4"
UiIcon_Item111:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_111.icon4"
UiIcon_Item112:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_112.icon4"
UiIcon_Item113:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_113.icon4"
UiIcon_Item114:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_114.icon4"
UiIcon_Item115:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_115.icon4"
UiIcon_Item116:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_116.icon4"
UiIcon_Item117:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_117.icon4"
UiIcon_Item118:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_118.icon4"
UiIcon_Item119:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_119.icon4"
UiIcon_Item120:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_120.icon4"
UiIcon_Item121:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_121.icon4"
UiIcon_Item122:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_122.icon4"
UiIcon_Item123:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_123.icon4"
UiIcon_Item124:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_124.icon4"
UiIcon_Item125:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_125.icon4"
UiIcon_Item126:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_126.icon4"
UiIcon_Item127:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_127.icon4"
UiIcon_Item128:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_128.icon4"
UiIcon_Item129:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_129.icon4"
UiIcon_Item130:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_130.icon4"
UiIcon_Item131:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_131.icon4"
UiIcon_Item132:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_132.icon4"
UiIcon_Item133:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_133.icon4"
UiIcon_Item134:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_134.icon4"
UiIcon_Item135:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_135.icon4"
UiIcon_Item136:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_136.icon4"
UiIcon_Item137:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_137.icon4"
UiIcon_Item138:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_138.icon4"
UiIcon_Item139:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_139.icon4"
UiIcon_Item140:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_140.icon4"
UiIcon_Item141:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_141.icon4"
UiIcon_Item142:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_142.icon4"
UiIcon_Item143:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_143.icon4"
UiIcon_Item144:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_144.icon4"
UiIcon_Item145:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_145.icon4"
UiIcon_Item146:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_146.icon4"
UiIcon_Item147:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_147.icon4"
UiIcon_Item148:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_148.icon4"
UiIcon_Item149:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_149.icon4"
UiIcon_Item150:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_150.icon4"
UiIcon_Item151:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_151.icon4"
UiIcon_Item152:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_152.icon4"
UiIcon_Item153:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_153.icon4"
UiIcon_Item154:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_154.icon4"
UiIcon_Item155:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_155.icon4"
UiIcon_Item156:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_156.icon4"
UiIcon_Item157:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_157.icon4"
UiIcon_Item158:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_158.icon4"
UiIcon_Item159:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_159.icon4"
UiIcon_Item160:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_160.icon4"
UiIcon_Item161:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_161.icon4"
UiIcon_Item162:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_162.icon4"
UiIcon_Item163:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_163.icon4"
UiIcon_Item164:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_164.icon4"
UiIcon_Item165:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_165.icon4"
UiIcon_Item166:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_166.icon4"
UiIcon_Item167:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_167.icon4"
UiIcon_Item168:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_168.icon4"
UiIcon_Item169:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_169.icon4"
UiIcon_Item170:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_170.icon4"
UiIcon_Item171:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_171.icon4"
UiIcon_Item172:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_172.icon4"
UiIcon_Item173:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_173.icon4"
UiIcon_Item174:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_174.icon4"
UiIcon_Item175:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_175.icon4"
UiIcon_Item176:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_176.icon4"
UiIcon_Item177:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_177.icon4"
UiIcon_Item178:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_178.icon4"
UiIcon_Item179:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_179.icon4"
UiIcon_Item180:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_180.icon4"
UiIcon_Item181:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_181.icon4"
UiIcon_Item182:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_182.icon4"
UiIcon_Item183:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_183.icon4"
UiIcon_Item184:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_184.icon4"
UiIcon_Item185:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_185.icon4"
UiIcon_Item186:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_186.icon4"
UiIcon_Item187:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_187.icon4"
UiIcon_Item188:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_188.icon4"
UiIcon_Item189:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_189.icon4"
UiIcon_Item190:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_190.icon4"
UiIcon_Item191:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_191.icon4"
UiIcon_Item192:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_192.icon4"
UiIcon_Item193:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_193.icon4"
UiIcon_Item194:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_194.icon4"
UiIcon_Item195:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_195.icon4"
UiIcon_Item196:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_196.icon4"
UiIcon_Item197:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_197.icon4"
UiIcon_Item198:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_198.icon4"
UiIcon_Item199:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_199.icon4"
UiIcon_Item200:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_200.icon4"
UiIcon_Item201:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_201.icon4"
UiIcon_Item202:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_202.icon4"
UiIcon_Item203:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_203.icon4"
UiIcon_Item204:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_204.icon4"
UiIcon_Item205:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_205.icon4"
UiIcon_Item206:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_206.icon4"
UiIcon_Item207:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_207.icon4"
UiIcon_Item208:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_208.icon4"
UiIcon_Item209:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_209.icon4"
UiIcon_Item210:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_210.icon4"
UiIcon_Item211:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_211.icon4"
UiIcon_Item212:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_212.icon4"
UiIcon_Item213:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_213.icon4"
UiIcon_Item214:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_214.icon4"
UiIcon_Item215:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_215.icon4"
UiIcon_Item216:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_216.icon4"
UiIcon_Item217:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_217.icon4"
UiIcon_Item218:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_218.icon4"
UiIcon_Item219:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_219.icon4"
UiIcon_Item220:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_220.icon4"
UiIcon_Item221:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_221.icon4"
UiIcon_Item222:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_222.icon4"
UiIcon_Item223:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_223.icon4"
UiIcon_Item224:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_224.icon4"
UiIcon_Item225:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_225.icon4"
UiIcon_Item226:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_226.icon4"
UiIcon_Item227:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_227.icon4"
UiIcon_Item228:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_228.icon4"
UiIcon_Item229:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_229.icon4"
UiIcon_Item230:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_230.icon4"
UiIcon_Item231:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_231.icon4"
UiIcon_Item232:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_232.icon4"
UiIcon_Item233:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_233.icon4"
UiIcon_Item234:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_234.icon4"
UiIcon_Item235:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_235.icon4"
UiIcon_Item236:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_236.icon4"
UiIcon_Item237:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_237.icon4"
UiIcon_Item238:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_238.icon4"
UiIcon_Item239:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_239.icon4"
UiIcon_Item240:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_240.icon4"
UiIcon_Item241:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_241.icon4"
UiIcon_Item242:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_242.icon4"
UiIcon_Item243:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_243.icon4"
UiIcon_Item244:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_244.icon4"
UiIcon_Item245:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_245.icon4"
UiIcon_Item246:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_246.icon4"
UiIcon_Item247:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_247.icon4"
UiIcon_Item248:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_248.icon4"
UiIcon_Item249:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_249.icon4"
UiIcon_Item250:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_250.icon4"
UiIcon_Item251:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_251.icon4"
UiIcon_Item252:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_252.icon4"
UiIcon_Item253:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_253.icon4"
UiIcon_Item254:
	.incbin "GRAPHICS/UI/MENU_ICONS/ITEM_254.icon4"
	.balign 4, 0
	.balign 4
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.4byte UiIcon_Psynergy000
	.4byte UiIcon_Psynergy001
	.4byte UiIcon_Psynergy002
	.4byte UiIcon_Psynergy003
	.4byte UiIcon_Psynergy004
	.4byte UiIcon_Psynergy005
	.4byte UiIcon_Psynergy006
	.4byte UiIcon_Psynergy007
	.4byte UiIcon_Psynergy008
	.4byte UiIcon_Psynergy009
	.4byte UiIcon_Psynergy010
	.4byte UiIcon_Psynergy011
	.4byte UiIcon_Psynergy012
	.4byte UiIcon_Psynergy013
	.4byte UiIcon_Psynergy014
	.4byte UiIcon_Psynergy015
	.4byte UiIcon_Psynergy016
	.4byte UiIcon_Psynergy017
	.4byte UiIcon_Psynergy018
	.4byte UiIcon_Psynergy019
	.4byte UiIcon_Psynergy020
	.4byte UiIcon_Psynergy021
	.4byte UiIcon_Psynergy022
	.4byte UiIcon_Psynergy023
	.4byte UiIcon_Psynergy024
	.4byte UiIcon_Psynergy025
	.4byte UiIcon_Psynergy026
	.4byte UiIcon_Psynergy027
	.4byte UiIcon_Psynergy028
	.4byte UiIcon_Psynergy029
	.4byte UiIcon_Psynergy030
	.4byte UiIcon_Psynergy031
	.4byte UiIcon_Psynergy032
	.4byte UiIcon_Psynergy033
	.4byte UiIcon_Psynergy034
	.4byte UiIcon_Psynergy035
	.4byte UiIcon_Psynergy036
	.4byte UiIcon_Psynergy037
	.4byte UiIcon_Psynergy038
	.4byte UiIcon_Psynergy039
	.4byte UiIcon_Psynergy040
	.4byte UiIcon_Psynergy041
	.4byte UiIcon_Psynergy042
	.4byte UiIcon_Psynergy043
	.4byte UiIcon_Psynergy044
	.4byte UiIcon_Psynergy045
	.4byte UiIcon_Psynergy046
	.4byte UiIcon_Psynergy047
	.4byte UiIcon_Psynergy048
	.4byte UiIcon_Psynergy049
	.4byte UiIcon_Psynergy050
	.4byte UiIcon_Psynergy051
	.4byte UiIcon_Psynergy052
	.4byte UiIcon_Psynergy053
	.4byte UiIcon_Psynergy054
	.4byte UiIcon_Psynergy055
	.4byte UiIcon_Psynergy056
	.4byte UiIcon_Psynergy057
	.4byte UiIcon_Psynergy058
	.4byte UiIcon_Psynergy059
	.4byte UiIcon_Psynergy060
	.4byte UiIcon_Psynergy061
	.4byte UiIcon_Psynergy062
	.4byte UiIcon_Psynergy063
	.4byte UiIcon_Psynergy064
	.4byte UiIcon_Psynergy065
	.4byte UiIcon_Psynergy066
	.4byte UiIcon_Psynergy067
	.4byte UiIcon_Psynergy068
	.4byte UiIcon_Psynergy069
	.4byte UiIcon_Psynergy070
	.4byte UiIcon_Psynergy071
	.4byte UiIcon_Psynergy072
	.4byte UiIcon_Psynergy073
	.4byte UiIcon_Psynergy074
	.4byte UiIcon_Psynergy075
	.4byte UiIcon_Psynergy076
	.4byte UiIcon_Psynergy077
	.4byte UiIcon_Psynergy078
	.4byte UiIcon_Psynergy079
	.4byte UiIcon_Psynergy080
	.4byte UiIcon_Psynergy081
	.4byte UiIcon_Psynergy082
	.4byte UiIcon_Psynergy083
	.4byte UiIcon_Psynergy084
	.4byte UiIcon_Psynergy085
	.4byte UiIcon_Psynergy086
	.4byte UiIcon_Psynergy087
	.4byte UiIcon_Psynergy088
	.4byte UiIcon_Psynergy089
	.4byte UiIcon_Psynergy090
	.4byte UiIcon_Psynergy091
	.4byte UiIcon_Psynergy092
	.4byte UiIcon_Psynergy093
	.4byte UiIcon_Psynergy094
	.4byte UiIcon_Psynergy095
	.4byte UiIcon_Psynergy096
	.4byte UiIcon_Psynergy097
	.4byte UiIcon_Psynergy098
	.4byte UiIcon_Psynergy099
	.4byte UiIcon_Psynergy100
	.4byte UiIcon_Psynergy101
	.4byte UiIcon_Psynergy102
	.4byte UiIcon_Psynergy103
	.4byte UiIcon_Psynergy104
	.4byte UiIcon_Psynergy105
	.4byte UiIcon_Psynergy106
	.4byte UiIcon_Psynergy107
	.4byte UiIcon_Psynergy108
	.4byte UiIcon_Psynergy109
	.4byte UiIcon_Psynergy110
	.4byte UiIcon_Psynergy111
	.4byte UiIcon_Psynergy112
	.4byte UiIcon_Psynergy113
	.4byte UiIcon_Psynergy114
	.4byte UiIcon_Psynergy115
	.4byte UiIcon_Psynergy116
	.4byte UiIcon_Psynergy117
	.4byte UiIcon_Psynergy118
	.4byte UiIcon_Psynergy119
	.4byte UiIcon_Psynergy120
	.4byte UiIcon_Psynergy121
	.4byte UiIcon_Psynergy122
	.4byte UiIcon_Psynergy123
	.4byte UiIcon_Psynergy124
	.4byte UiIcon_Psynergy125
	.4byte UiIcon_Psynergy126
	.4byte UiIcon_Psynergy127
	.4byte UiIcon_Psynergy128
	.4byte UiIcon_Psynergy129
	.4byte UiIcon_Psynergy130
	.4byte UiIcon_Psynergy131
	.4byte UiIcon_Psynergy132
	.4byte UiIcon_Psynergy133
	.4byte UiIcon_Psynergy134
	.4byte UiIcon_Psynergy135
	.4byte UiIcon_Psynergy136
	.4byte UiIcon_Psynergy137
	.4byte UiIcon_Psynergy138
	.4byte UiIcon_Psynergy139
	.4byte UiIcon_Psynergy140
	.4byte UiIcon_Psynergy141
	.4byte UiIcon_Psynergy142
	.4byte UiIcon_Psynergy143
	.4byte UiIcon_Psynergy144
	.4byte UiIcon_Psynergy145
	.4byte UiIcon_Psynergy146
	.4byte UiIcon_Psynergy147
	.4byte UiIcon_Psynergy148
	.4byte UiIcon_Psynergy149
	.4byte UiIcon_Psynergy150
	.4byte UiIcon_Psynergy151
	.4byte UiIcon_Psynergy152
	.4byte UiIcon_Psynergy153
	.4byte UiIcon_Psynergy154
	.4byte UiIcon_Psynergy155
	.4byte UiIcon_Psynergy156
	.4byte UiIcon_Psynergy157
	.4byte UiIcon_Psynergy158
	.4byte UiIcon_Psynergy159
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.4byte -1
UiIcon_Psynergy000:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_000.icon4"
UiIcon_Psynergy001:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_001.icon4"
UiIcon_Psynergy002:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_002.icon4"
UiIcon_Psynergy003:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_003.icon4"
UiIcon_Psynergy004:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_004.icon4"
UiIcon_Psynergy005:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_005.icon4"
UiIcon_Psynergy006:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_006.icon4"
UiIcon_Psynergy007:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_007.icon4"
UiIcon_Psynergy008:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_008.icon4"
UiIcon_Psynergy009:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_009.icon4"
UiIcon_Psynergy010:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_010.icon4"
UiIcon_Psynergy011:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_011.icon4"
UiIcon_Psynergy012:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_012.icon4"
UiIcon_Psynergy013:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_013.icon4"
UiIcon_Psynergy014:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_014.icon4"
UiIcon_Psynergy015:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_015.icon4"
UiIcon_Psynergy016:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_016.icon4"
UiIcon_Psynergy017:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_017.icon4"
UiIcon_Psynergy018:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_018.icon4"
UiIcon_Psynergy019:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_019.icon4"
UiIcon_Psynergy020:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_020.icon4"
UiIcon_Psynergy021:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_021.icon4"
UiIcon_Psynergy022:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_022.icon4"
UiIcon_Psynergy023:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_023.icon4"
UiIcon_Psynergy024:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_024.icon4"
UiIcon_Psynergy025:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_025.icon4"
UiIcon_Psynergy026:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_026.icon4"
UiIcon_Psynergy027:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_027.icon4"
UiIcon_Psynergy028:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_028.icon4"
UiIcon_Psynergy029:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_029.icon4"
UiIcon_Psynergy030:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_030.icon4"
UiIcon_Psynergy031:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_031.icon4"
UiIcon_Psynergy032:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_032.icon4"
UiIcon_Psynergy033:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_033.icon4"
UiIcon_Psynergy034:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_034.icon4"
UiIcon_Psynergy035:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_035.icon4"
UiIcon_Psynergy036:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_036.icon4"
UiIcon_Psynergy037:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_037.icon4"
UiIcon_Psynergy038:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_038.icon4"
UiIcon_Psynergy039:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_039.icon4"
UiIcon_Psynergy040:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_040.icon4"
UiIcon_Psynergy041:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_041.icon4"
UiIcon_Psynergy042:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_042.icon4"
UiIcon_Psynergy043:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_043.icon4"
UiIcon_Psynergy044:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_044.icon4"
UiIcon_Psynergy045:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_045.icon4"
UiIcon_Psynergy046:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_046.icon4"
UiIcon_Psynergy047:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_047.icon4"
UiIcon_Psynergy048:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_048.icon4"
UiIcon_Psynergy049:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_049.icon4"
UiIcon_Psynergy050:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_050.icon4"
UiIcon_Psynergy051:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_051.icon4"
UiIcon_Psynergy052:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_052.icon4"
UiIcon_Psynergy053:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_053.icon4"
UiIcon_Psynergy054:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_054.icon4"
UiIcon_Psynergy055:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_055.icon4"
UiIcon_Psynergy056:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_056.icon4"
UiIcon_Psynergy057:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_057.icon4"
UiIcon_Psynergy058:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_058.icon4"
UiIcon_Psynergy059:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_059.icon4"
UiIcon_Psynergy060:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_060.icon4"
UiIcon_Psynergy061:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_061.icon4"
UiIcon_Psynergy062:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_062.icon4"
UiIcon_Psynergy063:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_063.icon4"
UiIcon_Psynergy064:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_064.icon4"
UiIcon_Psynergy065:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_065.icon4"
UiIcon_Psynergy066:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_066.icon4"
UiIcon_Psynergy067:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_067.icon4"
UiIcon_Psynergy068:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_068.icon4"
UiIcon_Psynergy069:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_069.icon4"
UiIcon_Psynergy070:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_070.icon4"
UiIcon_Psynergy071:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_071.icon4"
UiIcon_Psynergy072:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_072.icon4"
UiIcon_Psynergy073:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_073.icon4"
UiIcon_Psynergy074:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_074.icon4"
UiIcon_Psynergy075:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_075.icon4"
UiIcon_Psynergy076:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_076.icon4"
UiIcon_Psynergy077:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_077.icon4"
UiIcon_Psynergy078:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_078.icon4"
UiIcon_Psynergy079:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_079.icon4"
UiIcon_Psynergy080:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_080.icon4"
UiIcon_Psynergy081:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_081.icon4"
UiIcon_Psynergy082:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_082.icon4"
UiIcon_Psynergy083:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_083.icon4"
UiIcon_Psynergy084:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_084.icon4"
UiIcon_Psynergy085:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_085.icon4"
UiIcon_Psynergy086:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_086.icon4"
UiIcon_Psynergy087:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_087.icon4"
UiIcon_Psynergy088:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_088.icon4"
UiIcon_Psynergy089:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_089.icon4"
UiIcon_Psynergy090:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_090.icon4"
UiIcon_Psynergy091:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_091.icon4"
UiIcon_Psynergy092:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_092.icon4"
UiIcon_Psynergy093:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_093.icon4"
UiIcon_Psynergy094:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_094.icon4"
UiIcon_Psynergy095:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_095.icon4"
UiIcon_Psynergy096:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_096.icon4"
UiIcon_Psynergy097:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_097.icon4"
UiIcon_Psynergy098:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_098.icon4"
UiIcon_Psynergy099:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_099.icon4"
UiIcon_Psynergy100:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_100.icon4"
UiIcon_Psynergy101:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_101.icon4"
UiIcon_Psynergy102:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_102.icon4"
UiIcon_Psynergy103:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_103.icon4"
UiIcon_Psynergy104:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_104.icon4"
UiIcon_Psynergy105:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_105.icon4"
UiIcon_Psynergy106:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_106.icon4"
UiIcon_Psynergy107:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_107.icon4"
UiIcon_Psynergy108:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_108.icon4"
UiIcon_Psynergy109:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_109.icon4"
UiIcon_Psynergy110:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_110.icon4"
UiIcon_Psynergy111:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_111.icon4"
UiIcon_Psynergy112:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_112.icon4"
UiIcon_Psynergy113:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_113.icon4"
UiIcon_Psynergy114:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_114.icon4"
UiIcon_Psynergy115:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_115.icon4"
UiIcon_Psynergy116:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_116.icon4"
UiIcon_Psynergy117:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_117.icon4"
UiIcon_Psynergy118:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_118.icon4"
UiIcon_Psynergy119:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_119.icon4"
UiIcon_Psynergy120:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_120.icon4"
UiIcon_Psynergy121:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_121.icon4"
UiIcon_Psynergy122:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_122.icon4"
UiIcon_Psynergy123:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_123.icon4"
UiIcon_Psynergy124:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_124.icon4"
UiIcon_Psynergy125:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_125.icon4"
UiIcon_Psynergy126:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_126.icon4"
UiIcon_Psynergy127:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_127.icon4"
UiIcon_Psynergy128:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_128.icon4"
UiIcon_Psynergy129:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_129.icon4"
UiIcon_Psynergy130:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_130.icon4"
UiIcon_Psynergy131:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_131.icon4"
UiIcon_Psynergy132:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_132.icon4"
UiIcon_Psynergy133:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_133.icon4"
UiIcon_Psynergy134:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_134.icon4"
UiIcon_Psynergy135:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_135.icon4"
UiIcon_Psynergy136:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_136.icon4"
UiIcon_Psynergy137:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_137.icon4"
UiIcon_Psynergy138:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_138.icon4"
UiIcon_Psynergy139:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_139.icon4"
UiIcon_Psynergy140:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_140.icon4"
UiIcon_Psynergy141:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_141.icon4"
UiIcon_Psynergy142:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_142.icon4"
UiIcon_Psynergy143:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_143.icon4"
UiIcon_Psynergy144:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_144.icon4"
UiIcon_Psynergy145:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_145.icon4"
UiIcon_Psynergy146:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_146.icon4"
UiIcon_Psynergy147:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_147.icon4"
UiIcon_Psynergy148:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_148.icon4"
UiIcon_Psynergy149:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_149.icon4"
UiIcon_Psynergy150:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_150.icon4"
UiIcon_Psynergy151:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_151.icon4"
UiIcon_Psynergy152:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_152.icon4"
UiIcon_Psynergy153:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_153.icon4"
UiIcon_Psynergy154:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_154.icon4"
UiIcon_Psynergy155:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_155.icon4"
UiIcon_Psynergy156:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_156.icon4"
UiIcon_Psynergy157:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_157.icon4"
UiIcon_Psynergy158:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_158.icon4"
UiIcon_Psynergy159:
	.incbin "GRAPHICS/UI/MENU_ICONS/PSYNERGY_159.icon4"
	.balign 4, 0
	.balign 4
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.4byte UiIcon_Misc00
	.4byte UiIcon_Misc01
	.4byte UiIcon_Misc02
	.4byte UiIcon_Misc03
	.4byte UiIcon_Misc04
	.4byte UiIcon_Misc05
	.4byte UiIcon_Misc06
	.4byte UiIcon_Misc07
	.4byte UiIcon_Misc08
	.4byte UiIcon_Misc09
	.4byte UiIcon_Misc10
	.4byte UiIcon_Misc11
	.4byte UiIcon_Misc12
	.4byte UiIcon_Misc13
	.4byte UiIcon_Misc14
	.4byte UiIcon_Misc15
	.4byte UiIcon_Misc16
	.4byte UiIcon_Misc17
	.4byte UiIcon_Misc18
	.4byte UiIcon_Misc19
	.4byte UiIcon_Misc20
	.4byte UiIcon_Misc21
	.4byte UiIcon_Misc22
	.4byte UiIcon_Misc23
	.4byte UiIcon_Misc24
	.4byte UiIcon_Misc25
	.4byte UiIcon_Misc26
	.4byte UiIcon_Misc27
	.4byte UiIcon_Misc28
	.4byte UiIcon_Misc29
	.4byte UiIcon_Misc30
	.4byte UiIcon_Misc31
	.4byte -1
UiIcon_Misc00:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_00.icon4"
UiIcon_Misc01:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_01.icon4"
UiIcon_Misc02:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_02.icon4"
UiIcon_Misc03:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_03.icon4"
UiIcon_Misc04:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_04.icon4"
UiIcon_Misc05:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_05.icon4"
UiIcon_Misc06:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_06.icon4"
UiIcon_Misc07:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_07.icon4"
UiIcon_Misc08:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_08.icon4"
UiIcon_Misc09:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_09.icon4"
UiIcon_Misc10:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_10.icon4"
UiIcon_Misc11:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_11.icon4"
UiIcon_Misc12:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_12.icon4"
UiIcon_Misc13:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_13.icon4"
UiIcon_Misc14:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_14.icon4"
UiIcon_Misc15:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_15.icon4"
UiIcon_Misc16:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_16.icon4"
UiIcon_Misc17:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_17.icon4"
UiIcon_Misc18:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_18.icon4"
UiIcon_Misc19:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_19.icon4"
UiIcon_Misc20:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_20.icon4"
UiIcon_Misc21:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_21.icon4"
UiIcon_Misc22:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_22.icon4"
UiIcon_Misc23:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_23.icon4"
UiIcon_Misc24:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_24.icon4"
UiIcon_Misc25:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_25.icon4"
UiIcon_Misc26:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_26.icon4"
UiIcon_Misc27:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_27.icon4"
UiIcon_Misc28:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_28.icon4"
UiIcon_Misc29:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_29.icon4"
UiIcon_Misc30:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_30.icon4"
UiIcon_Misc31:
	.incbin "GRAPHICS/UI/MENU_ICONS/MISC_31.icon4"
	.balign 4, 0
