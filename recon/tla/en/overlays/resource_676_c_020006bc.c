/* Near miss (721 of 1,163 instructions differ, nearly all one register
 * swap): the native keeps the actor id in r6 and the object in r5, the
 * draft the other way round, so every one of the 162 path steps differs
 * by its first argument's register. The case structure (each step moves
 * the actor and ends with its own increment and return, odd steps wait for
 * arrival) and the state switch match. */
#include "types.h"

struct YallamObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x4e];
    u8 state;
    u8 unknown_63;
    s16 timer;
};

struct YallamObject *Object_GetById(s32 id);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Func_02002448(struct YallamObject *object, s32 mode);
void Func_02002450(struct YallamObject *object, s32 x, s32 y, s32 z);
s32 Func_020006a4(struct YallamObject *object);

void Func_020006bc(s32 actor)
{
    struct YallamObject *object = Object_GetById(actor);
    u8 *state = &object->state;
    s16 *timer;

    if (*state == 1)
        return;
    timer = &object->timer;
    switch (*state) {
    case 0:
        Func_02002448(object, 2);
        break;
    case 1:
        break;
    case 3:
        Func_02002448(object, 1);
    case 2:
        Func_02002450(object, object->x, object->y, object->z);
        *timer &= ~1;
        *state = 1;
        return;
    }
    switch (*timer) {
    case 0:
        ObjectMotion_ResetAndSetPosition(actor, 344, 824);
        (*timer)++;
        return;
    case 2:
        ObjectMotion_ResetAndSetPosition(actor, 324, 820);
        (*timer)++;
        return;
    case 4:
        ObjectMotion_ResetAndSetPosition(actor, 320, 808);
        (*timer)++;
        return;
    case 6:
        ObjectMotion_ResetAndSetPosition(actor, 324, 796);
        (*timer)++;
        return;
    case 8:
        ObjectMotion_ResetAndSetPosition(actor, 344, 792);
        (*timer)++;
        return;
    case 10:
        ObjectMotion_ResetAndSetPosition(actor, 364, 796);
        (*timer)++;
        return;
    case 12:
        ObjectMotion_ResetAndSetPosition(actor, 368, 808);
        (*timer)++;
        return;
    case 14:
        ObjectMotion_ResetAndSetPosition(actor, 364, 820);
        (*timer)++;
        return;
    case 16:
        ObjectMotion_ResetAndSetPosition(actor, 344, 824);
        (*timer)++;
        return;
    case 18:
        ObjectMotion_ResetAndSetPosition(actor, 324, 820);
        (*timer)++;
        return;
    case 20:
        ObjectMotion_ResetAndSetPosition(actor, 320, 808);
        (*timer)++;
        return;
    case 22:
        ObjectMotion_ResetAndSetPosition(actor, 324, 796);
        (*timer)++;
        return;
    case 24:
        ObjectMotion_ResetAndSetPosition(actor, 344, 792);
        (*timer)++;
        return;
    case 26:
        ObjectMotion_ResetAndSetPosition(actor, 364, 796);
        (*timer)++;
        return;
    case 28:
        ObjectMotion_ResetAndSetPosition(actor, 368, 808);
        (*timer)++;
        return;
    case 30:
        ObjectMotion_ResetAndSetPosition(actor, 364, 820);
        (*timer)++;
        return;
    case 32:
        ObjectMotion_ResetAndSetPosition(actor, 344, 824);
        (*timer)++;
        return;
    case 34:
        ObjectMotion_SetSpeedParameters(actor, 0x1cccc, 0x1cccc);
        ObjectMotion_ResetAndSetPosition(actor, 364, 820);
        (*timer)++;
        return;
    case 36:
        ObjectMotion_ResetAndSetPosition(actor, 368, 808);
        (*timer)++;
        return;
    case 38:
        ObjectMotion_ResetAndSetPosition(actor, 392, 808);
        (*timer)++;
        return;
    case 40:
        ObjectMotion_ResetAndSetPosition(actor, 392, 744);
        (*timer)++;
        return;
    case 42:
        ObjectMotion_SetSpeedParameters(actor, 0x13333, 0x13333);
        ObjectMotion_ResetAndSetPosition(actor, 372, 740);
        (*timer)++;
        return;
    case 44:
        ObjectMotion_ResetAndSetPosition(actor, 368, 728);
        (*timer)++;
        return;
    case 46:
        ObjectMotion_ResetAndSetPosition(actor, 372, 716);
        (*timer)++;
        return;
    case 48:
        ObjectMotion_ResetAndSetPosition(actor, 392, 712);
        (*timer)++;
        return;
    case 50:
        ObjectMotion_ResetAndSetPosition(actor, 412, 716);
        (*timer)++;
        return;
    case 52:
        ObjectMotion_ResetAndSetPosition(actor, 416, 728);
        (*timer)++;
        return;
    case 54:
        ObjectMotion_ResetAndSetPosition(actor, 412, 740);
        (*timer)++;
        return;
    case 56:
        ObjectMotion_ResetAndSetPosition(actor, 392, 744);
        (*timer)++;
        return;
    case 58:
        ObjectMotion_ResetAndSetPosition(actor, 372, 740);
        (*timer)++;
        return;
    case 60:
        ObjectMotion_ResetAndSetPosition(actor, 368, 728);
        (*timer)++;
        return;
    case 62:
        ObjectMotion_ResetAndSetPosition(actor, 372, 716);
        (*timer)++;
        return;
    case 64:
        ObjectMotion_ResetAndSetPosition(actor, 392, 712);
        (*timer)++;
        return;
    case 66:
        ObjectMotion_ResetAndSetPosition(actor, 412, 716);
        (*timer)++;
        return;
    case 68:
        ObjectMotion_ResetAndSetPosition(actor, 416, 728);
        (*timer)++;
        return;
    case 70:
        ObjectMotion_ResetAndSetPosition(actor, 412, 740);
        (*timer)++;
        return;
    case 72:
        ObjectMotion_ResetAndSetPosition(actor, 392, 744);
        (*timer)++;
        return;
    case 74:
        ObjectMotion_ResetAndSetPosition(actor, 372, 740);
        (*timer)++;
        return;
    case 76:
        ObjectMotion_ResetAndSetPosition(actor, 368, 728);
        (*timer)++;
        return;
    case 78:
        ObjectMotion_ResetAndSetPosition(actor, 372, 716);
        (*timer)++;
        return;
    case 80:
        ObjectMotion_ResetAndSetPosition(actor, 392, 712);
        (*timer)++;
        return;
    case 82:
        ObjectMotion_ResetAndSetPosition(actor, 412, 716);
        (*timer)++;
        return;
    case 84:
        ObjectMotion_ResetAndSetPosition(actor, 416, 728);
        (*timer)++;
        return;
    case 86:
        ObjectMotion_ResetAndSetPosition(actor, 412, 740);
        (*timer)++;
        return;
    case 88:
        ObjectMotion_ResetAndSetPosition(actor, 392, 744);
        (*timer)++;
        return;
    case 90:
        ObjectMotion_ResetAndSetPosition(actor, 372, 740);
        (*timer)++;
        return;
    case 92:
        ObjectMotion_ResetAndSetPosition(actor, 368, 728);
        (*timer)++;
        return;
    case 94:
        ObjectMotion_ResetAndSetPosition(actor, 372, 716);
        (*timer)++;
        return;
    case 96:
        ObjectMotion_ResetAndSetPosition(actor, 392, 712);
        (*timer)++;
        return;
    case 98:
        ObjectMotion_ResetAndSetPosition(actor, 392, 696);
        (*timer)++;
        return;
    case 100:
        ObjectMotion_ResetAndSetPosition(actor, 412, 692);
        (*timer)++;
        return;
    case 102:
        ObjectMotion_ResetAndSetPosition(actor, 416, 680);
        (*timer)++;
        return;
    case 104:
        ObjectMotion_ResetAndSetPosition(actor, 412, 668);
        (*timer)++;
        return;
    case 106:
        ObjectMotion_ResetAndSetPosition(actor, 392, 664);
        (*timer)++;
        return;
    case 108:
        ObjectMotion_ResetAndSetPosition(actor, 372, 668);
        (*timer)++;
        return;
    case 110:
        ObjectMotion_ResetAndSetPosition(actor, 368, 680);
        (*timer)++;
        return;
    case 112:
        ObjectMotion_ResetAndSetPosition(actor, 320, 680);
        (*timer)++;
        return;
    case 114:
        ObjectMotion_ResetAndSetPosition(actor, 316, 692);
        (*timer)++;
        return;
    case 116:
        ObjectMotion_ResetAndSetPosition(actor, 296, 696);
        (*timer)++;
        return;
    case 118:
        ObjectMotion_ResetAndSetPosition(actor, 276, 692);
        (*timer)++;
        return;
    case 120:
        ObjectMotion_ResetAndSetPosition(actor, 272, 680);
        (*timer)++;
        return;
    case 122:
        ObjectMotion_ResetAndSetPosition(actor, 276, 668);
        (*timer)++;
        return;
    case 124:
        ObjectMotion_ResetAndSetPosition(actor, 296, 664);
        (*timer)++;
        return;
    case 126:
        ObjectMotion_ResetAndSetPosition(actor, 316, 668);
        (*timer)++;
        return;
    case 128:
        ObjectMotion_ResetAndSetPosition(actor, 320, 680);
        (*timer)++;
        return;
    case 130:
        ObjectMotion_ResetAndSetPosition(actor, 316, 692);
        (*timer)++;
        return;
    case 132:
        ObjectMotion_ResetAndSetPosition(actor, 296, 696);
        (*timer)++;
        return;
    case 134:
        ObjectMotion_ResetAndSetPosition(actor, 276, 692);
        (*timer)++;
        return;
    case 136:
        ObjectMotion_ResetAndSetPosition(actor, 272, 680);
        (*timer)++;
        return;
    case 138:
        ObjectMotion_ResetAndSetPosition(actor, 276, 668);
        (*timer)++;
        return;
    case 140:
        ObjectMotion_ResetAndSetPosition(actor, 296, 664);
        (*timer)++;
        return;
    case 142:
        ObjectMotion_ResetAndSetPosition(actor, 316, 668);
        (*timer)++;
        return;
    case 144:
        ObjectMotion_ResetAndSetPosition(actor, 320, 680);
        (*timer)++;
        return;
    case 146:
        ObjectMotion_ResetAndSetPosition(actor, 316, 692);
        (*timer)++;
        return;
    case 148:
        ObjectMotion_ResetAndSetPosition(actor, 296, 696);
        (*timer)++;
        return;
    case 150:
        ObjectMotion_ResetAndSetPosition(actor, 296, 728);
        (*timer)++;
        return;
    case 152:
        ObjectMotion_ResetAndSetPosition(actor, 272, 728);
        (*timer)++;
        return;
    case 154:
        ObjectMotion_ResetAndSetPosition(actor, 268, 740);
        (*timer)++;
        return;
    case 156:
        ObjectMotion_ResetAndSetPosition(actor, 248, 744);
        (*timer)++;
        return;
    case 158:
        ObjectMotion_ResetAndSetPosition(actor, 228, 740);
        (*timer)++;
        return;
    case 160:
        ObjectMotion_ResetAndSetPosition(actor, 224, 728);
        (*timer)++;
        return;
    case 162:
        ObjectMotion_ResetAndSetPosition(actor, 228, 716);
        (*timer)++;
        return;
    case 164:
        ObjectMotion_ResetAndSetPosition(actor, 248, 712);
        (*timer)++;
        return;
    case 166:
        ObjectMotion_ResetAndSetPosition(actor, 268, 716);
        (*timer)++;
        return;
    case 168:
        ObjectMotion_ResetAndSetPosition(actor, 272, 728);
        (*timer)++;
        return;
    case 170:
        ObjectMotion_ResetAndSetPosition(actor, 268, 740);
        (*timer)++;
        return;
    case 172:
        ObjectMotion_ResetAndSetPosition(actor, 248, 744);
        (*timer)++;
        return;
    case 174:
        ObjectMotion_ResetAndSetPosition(actor, 228, 740);
        (*timer)++;
        return;
    case 176:
        ObjectMotion_ResetAndSetPosition(actor, 224, 728);
        (*timer)++;
        return;
    case 178:
        ObjectMotion_ResetAndSetPosition(actor, 228, 716);
        (*timer)++;
        return;
    case 180:
        ObjectMotion_ResetAndSetPosition(actor, 248, 712);
        (*timer)++;
        return;
    case 182:
        ObjectMotion_ResetAndSetPosition(actor, 268, 716);
        (*timer)++;
        return;
    case 184:
        ObjectMotion_ResetAndSetPosition(actor, 272, 728);
        (*timer)++;
        return;
    case 186:
        ObjectMotion_ResetAndSetPosition(actor, 268, 740);
        (*timer)++;
        return;
    case 188:
        ObjectMotion_ResetAndSetPosition(actor, 248, 744);
        (*timer)++;
        return;
    case 190:
        ObjectMotion_ResetAndSetPosition(actor, 228, 740);
        (*timer)++;
        return;
    case 192:
        ObjectMotion_ResetAndSetPosition(actor, 224, 728);
        (*timer)++;
        return;
    case 194:
        ObjectMotion_ResetAndSetPosition(actor, 228, 716);
        (*timer)++;
        return;
    case 196:
        ObjectMotion_ResetAndSetPosition(actor, 248, 712);
        (*timer)++;
        return;
    case 198:
        ObjectMotion_ResetAndSetPosition(actor, 268, 716);
        (*timer)++;
        return;
    case 200:
        ObjectMotion_ResetAndSetPosition(actor, 272, 728);
        (*timer)++;
        return;
    case 202:
        ObjectMotion_ResetAndSetPosition(actor, 268, 740);
        (*timer)++;
        return;
    case 204:
        ObjectMotion_ResetAndSetPosition(actor, 248, 744);
        (*timer)++;
        return;
    case 206:
        ObjectMotion_ResetAndSetPosition(actor, 248, 760);
        (*timer)++;
        return;
    case 208:
        ObjectMotion_ResetAndSetPosition(actor, 228, 764);
        (*timer)++;
        return;
    case 210:
        ObjectMotion_ResetAndSetPosition(actor, 224, 776);
        (*timer)++;
        return;
    case 212:
        ObjectMotion_ResetAndSetPosition(actor, 228, 788);
        (*timer)++;
        return;
    case 214:
        ObjectMotion_ResetAndSetPosition(actor, 248, 792);
        (*timer)++;
        return;
    case 216:
        ObjectMotion_ResetAndSetPosition(actor, 248, 840);
        (*timer)++;
        return;
    case 218:
        ObjectMotion_ResetAndSetPosition(actor, 228, 844);
        (*timer)++;
        return;
    case 220:
        ObjectMotion_ResetAndSetPosition(actor, 224, 856);
        (*timer)++;
        return;
    case 222:
        ObjectMotion_ResetAndSetPosition(actor, 228, 868);
        (*timer)++;
        return;
    case 226:
        ObjectMotion_ResetAndSetPosition(actor, 268, 868);
        (*timer)++;
        return;
    case 228:
        ObjectMotion_ResetAndSetPosition(actor, 272, 856);
        (*timer)++;
        return;
    case 230:
        ObjectMotion_ResetAndSetPosition(actor, 268, 844);
        (*timer)++;
        return;
    case 232:
        ObjectMotion_ResetAndSetPosition(actor, 248, 840);
        (*timer)++;
        return;
    case 234:
        ObjectMotion_ResetAndSetPosition(actor, 228, 844);
        (*timer)++;
        return;
    case 236:
        ObjectMotion_ResetAndSetPosition(actor, 224, 856);
        (*timer)++;
        return;
    case 238:
        ObjectMotion_ResetAndSetPosition(actor, 228, 868);
        (*timer)++;
        return;
    case 223:
    case 239:
        if (Func_020006a4(object) != 0)
            (*timer)++;
    case 224:
    case 240:
        ObjectMotion_ResetAndSetPosition(actor, 248, 872);
        object->timer++;
        return;
    case 242:
        ObjectMotion_ResetAndSetPosition(actor, 268, 868);
        (*timer)++;
        return;
    case 244:
        ObjectMotion_ResetAndSetPosition(actor, 272, 856);
        (*timer)++;
        return;
    case 246:
        ObjectMotion_ResetAndSetPosition(actor, 268, 844);
        (*timer)++;
        return;
    case 248:
        ObjectMotion_ResetAndSetPosition(actor, 248, 840);
        (*timer)++;
        return;
    case 250:
        ObjectMotion_ResetAndSetPosition(actor, 228, 844);
        (*timer)++;
        return;
    case 252:
        ObjectMotion_ResetAndSetPosition(actor, 224, 856);
        (*timer)++;
        return;
    case 254:
        ObjectMotion_SetSpeedParameters(actor, 0x1cccc, 0x1cccc);
        ObjectMotion_ResetAndSetPosition(actor, 200, 856);
        (*timer)++;
        return;
    case 256:
        ObjectMotion_ResetAndSetPosition(actor, 200, 744);
        (*timer)++;
        return;
    case 258:
        ObjectMotion_SetSpeedParameters(actor, 0x13333, 0x13333);
        ObjectMotion_ResetAndSetPosition(actor, 180, 740);
        (*timer)++;
        return;
    case 260:
        ObjectMotion_ResetAndSetPosition(actor, 176, 728);
        (*timer)++;
        return;
    case 262:
        ObjectMotion_ResetAndSetPosition(actor, 180, 716);
        (*timer)++;
        return;
    case 264:
        ObjectMotion_ResetAndSetPosition(actor, 200, 712);
        (*timer)++;
        return;
    case 266:
        ObjectMotion_ResetAndSetPosition(actor, 220, 716);
        (*timer)++;
        return;
    case 268:
        ObjectMotion_ResetAndSetPosition(actor, 224, 728);
        (*timer)++;
        return;
    case 270:
        ObjectMotion_ResetAndSetPosition(actor, 220, 740);
        (*timer)++;
        return;
    case 272:
        ObjectMotion_ResetAndSetPosition(actor, 200, 744);
        (*timer)++;
        return;
    case 274:
        ObjectMotion_ResetAndSetPosition(actor, 180, 740);
        (*timer)++;
        return;
    case 276:
        ObjectMotion_ResetAndSetPosition(actor, 176, 728);
        (*timer)++;
        return;
    case 278:
        ObjectMotion_ResetAndSetPosition(actor, 180, 716);
        (*timer)++;
        return;
    case 280:
        ObjectMotion_ResetAndSetPosition(actor, 200, 712);
        (*timer)++;
        return;
    case 282:
        ObjectMotion_ResetAndSetPosition(actor, 220, 716);
        (*timer)++;
        return;
    case 284:
        ObjectMotion_ResetAndSetPosition(actor, 224, 728);
        (*timer)++;
        return;
    case 286:
        ObjectMotion_ResetAndSetPosition(actor, 220, 740);
        (*timer)++;
        return;
    case 288:
        ObjectMotion_ResetAndSetPosition(actor, 200, 744);
        (*timer)++;
        return;
    case 290:
        ObjectMotion_ResetAndSetPosition(actor, 180, 740);
        (*timer)++;
        return;
    case 292:
        ObjectMotion_ResetAndSetPosition(actor, 176, 728);
        (*timer)++;
        return;
    case 294:
        ObjectMotion_ResetAndSetPosition(actor, 180, 716);
        (*timer)++;
        return;
    case 296:
        ObjectMotion_ResetAndSetPosition(actor, 200, 712);
        (*timer)++;
        return;
    case 298:
        ObjectMotion_ResetAndSetPosition(actor, 220, 716);
        (*timer)++;
        return;
    case 300:
        ObjectMotion_ResetAndSetPosition(actor, 224, 728);
        (*timer)++;
        return;
    case 302:
        ObjectMotion_ResetAndSetPosition(actor, 220, 740);
        (*timer)++;
        return;
    case 304:
        ObjectMotion_ResetAndSetPosition(actor, 200, 744);
        (*timer)++;
        return;
    case 306:
        ObjectMotion_ResetAndSetPosition(actor, 180, 740);
        (*timer)++;
        return;
    case 308:
        ObjectMotion_ResetAndSetPosition(actor, 176, 728);
        (*timer)++;
        return;
    case 310:
        ObjectMotion_ResetAndSetPosition(actor, 180, 716);
        (*timer)++;
        return;
    case 312:
        ObjectMotion_ResetAndSetPosition(actor, 200, 712);
        (*timer)++;
        return;
    case 314:
        ObjectMotion_ResetAndSetPosition(actor, 200, 680);
        (*timer)++;
        return;
    case 316:
        ObjectMotion_ResetAndSetPosition(actor, 212, 676);
        (*timer)++;
        return;
    case 318:
        ObjectMotion_ResetAndSetPosition(actor, 216, 664);
        (*timer)++;
        return;
    case 320:
        ObjectMotion_ResetAndSetPosition(actor, 212, 652);
        (*timer)++;
        return;
    case 322:
        ObjectMotion_ResetAndSetPosition(actor, 200, 648);
        (*timer)++;
        return;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
    case 13:
    case 15:
    case 17:
    case 19:
    case 21:
    case 23:
    case 25:
    case 27:
    case 29:
    case 31:
    case 33:
    case 35:
    case 37:
    case 39:
    case 41:
    case 43:
    case 45:
    case 47:
    case 49:
    case 51:
    case 53:
    case 55:
    case 57:
    case 59:
    case 61:
    case 63:
    case 65:
    case 67:
    case 69:
    case 71:
    case 73:
    case 75:
    case 77:
    case 79:
    case 81:
    case 83:
    case 85:
    case 87:
    case 89:
    case 91:
    case 93:
    case 95:
    case 97:
    case 99:
    case 101:
    case 103:
    case 105:
    case 107:
    case 109:
    case 111:
    case 113:
    case 115:
    case 117:
    case 119:
    case 121:
    case 123:
    case 125:
    case 127:
    case 129:
    case 131:
    case 133:
    case 135:
    case 137:
    case 139:
    case 141:
    case 143:
    case 145:
    case 147:
    case 149:
    case 151:
    case 153:
    case 155:
    case 157:
    case 159:
    case 161:
    case 163:
    case 165:
    case 167:
    case 169:
    case 171:
    case 173:
    case 175:
    case 177:
    case 179:
    case 181:
    case 183:
    case 185:
    case 187:
    case 189:
    case 191:
    case 193:
    case 195:
    case 197:
    case 199:
    case 201:
    case 203:
    case 205:
    case 207:
    case 209:
    case 211:
    case 213:
    case 215:
    case 217:
    case 219:
    case 221:
    case 225:
    case 227:
    case 229:
    case 231:
    case 233:
    case 235:
    case 237:
    case 241:
    case 243:
    case 245:
    case 247:
    case 249:
    case 251:
    case 253:
    case 255:
    case 257:
    case 259:
    case 261:
    case 263:
    case 265:
    case 267:
    case 269:
    case 271:
    case 273:
    case 275:
    case 277:
    case 279:
    case 281:
    case 283:
    case 285:
    case 287:
    case 289:
    case 291:
    case 293:
    case 295:
    case 297:
    case 299:
    case 301:
    case 303:
    case 305:
    case 307:
    case 309:
    case 311:
    case 313:
    case 315:
    case 317:
    case 319:
    case 321:
    case 323:
        if (Func_020006a4(object) == 0)
            return;
        (*timer)++;
        return;
    default:
        return;
    }
    (*timer)++;
}
