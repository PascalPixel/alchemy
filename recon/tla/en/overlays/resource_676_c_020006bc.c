// Near miss (721 of 1,163 instructions differ, nearly all one register
// swap): the native keeps the actor id in r6 and the object in r5, the
// draft the other way round, so every one of the 162 path steps differs
// by its first argument's register. The case structure (each step moves
// the actor and ends with its own increment and return, odd steps wait for
// arrival) and the state switch match.
#include "types.h"

struct YallamObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4e];
  u8 state;
  u8 unk63;
  s16 timer;
};

struct YallamObject *Actor_GetById(s32 id);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Func_02002448(struct YallamObject *object, s32 mode);
void Func_02002450(struct YallamObject *object, s32 x, s32 y, s32 z);
s32 Func_020006a4(struct YallamObject *object);

void Func_020006bc(s32 actor) {
  struct YallamObject *object = Actor_GetById(actor);
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
    Actor_ResetAndSetPosition(actor, 344, 824);
    (*timer)++;
    return;
  case 2:
    Actor_ResetAndSetPosition(actor, 324, 820);
    (*timer)++;
    return;
  case 4:
    Actor_ResetAndSetPosition(actor, 320, 808);
    (*timer)++;
    return;
  case 6:
    Actor_ResetAndSetPosition(actor, 324, 796);
    (*timer)++;
    return;
  case 8:
    Actor_ResetAndSetPosition(actor, 344, 792);
    (*timer)++;
    return;
  case 10:
    Actor_ResetAndSetPosition(actor, 364, 796);
    (*timer)++;
    return;
  case 12:
    Actor_ResetAndSetPosition(actor, 368, 808);
    (*timer)++;
    return;
  case 14:
    Actor_ResetAndSetPosition(actor, 364, 820);
    (*timer)++;
    return;
  case 16:
    Actor_ResetAndSetPosition(actor, 344, 824);
    (*timer)++;
    return;
  case 18:
    Actor_ResetAndSetPosition(actor, 324, 820);
    (*timer)++;
    return;
  case 20:
    Actor_ResetAndSetPosition(actor, 320, 808);
    (*timer)++;
    return;
  case 22:
    Actor_ResetAndSetPosition(actor, 324, 796);
    (*timer)++;
    return;
  case 24:
    Actor_ResetAndSetPosition(actor, 344, 792);
    (*timer)++;
    return;
  case 26:
    Actor_ResetAndSetPosition(actor, 364, 796);
    (*timer)++;
    return;
  case 28:
    Actor_ResetAndSetPosition(actor, 368, 808);
    (*timer)++;
    return;
  case 30:
    Actor_ResetAndSetPosition(actor, 364, 820);
    (*timer)++;
    return;
  case 32:
    Actor_ResetAndSetPosition(actor, 344, 824);
    (*timer)++;
    return;
  case 34:
    Actor_SetSpeedParameters(actor, 0x1cccc, 0x1cccc);
    Actor_ResetAndSetPosition(actor, 364, 820);
    (*timer)++;
    return;
  case 36:
    Actor_ResetAndSetPosition(actor, 368, 808);
    (*timer)++;
    return;
  case 38:
    Actor_ResetAndSetPosition(actor, 392, 808);
    (*timer)++;
    return;
  case 40:
    Actor_ResetAndSetPosition(actor, 392, 744);
    (*timer)++;
    return;
  case 42:
    Actor_SetSpeedParameters(actor, 0x13333, 0x13333);
    Actor_ResetAndSetPosition(actor, 372, 740);
    (*timer)++;
    return;
  case 44:
    Actor_ResetAndSetPosition(actor, 368, 728);
    (*timer)++;
    return;
  case 46:
    Actor_ResetAndSetPosition(actor, 372, 716);
    (*timer)++;
    return;
  case 48:
    Actor_ResetAndSetPosition(actor, 392, 712);
    (*timer)++;
    return;
  case 50:
    Actor_ResetAndSetPosition(actor, 412, 716);
    (*timer)++;
    return;
  case 52:
    Actor_ResetAndSetPosition(actor, 416, 728);
    (*timer)++;
    return;
  case 54:
    Actor_ResetAndSetPosition(actor, 412, 740);
    (*timer)++;
    return;
  case 56:
    Actor_ResetAndSetPosition(actor, 392, 744);
    (*timer)++;
    return;
  case 58:
    Actor_ResetAndSetPosition(actor, 372, 740);
    (*timer)++;
    return;
  case 60:
    Actor_ResetAndSetPosition(actor, 368, 728);
    (*timer)++;
    return;
  case 62:
    Actor_ResetAndSetPosition(actor, 372, 716);
    (*timer)++;
    return;
  case 64:
    Actor_ResetAndSetPosition(actor, 392, 712);
    (*timer)++;
    return;
  case 66:
    Actor_ResetAndSetPosition(actor, 412, 716);
    (*timer)++;
    return;
  case 68:
    Actor_ResetAndSetPosition(actor, 416, 728);
    (*timer)++;
    return;
  case 70:
    Actor_ResetAndSetPosition(actor, 412, 740);
    (*timer)++;
    return;
  case 72:
    Actor_ResetAndSetPosition(actor, 392, 744);
    (*timer)++;
    return;
  case 74:
    Actor_ResetAndSetPosition(actor, 372, 740);
    (*timer)++;
    return;
  case 76:
    Actor_ResetAndSetPosition(actor, 368, 728);
    (*timer)++;
    return;
  case 78:
    Actor_ResetAndSetPosition(actor, 372, 716);
    (*timer)++;
    return;
  case 80:
    Actor_ResetAndSetPosition(actor, 392, 712);
    (*timer)++;
    return;
  case 82:
    Actor_ResetAndSetPosition(actor, 412, 716);
    (*timer)++;
    return;
  case 84:
    Actor_ResetAndSetPosition(actor, 416, 728);
    (*timer)++;
    return;
  case 86:
    Actor_ResetAndSetPosition(actor, 412, 740);
    (*timer)++;
    return;
  case 88:
    Actor_ResetAndSetPosition(actor, 392, 744);
    (*timer)++;
    return;
  case 90:
    Actor_ResetAndSetPosition(actor, 372, 740);
    (*timer)++;
    return;
  case 92:
    Actor_ResetAndSetPosition(actor, 368, 728);
    (*timer)++;
    return;
  case 94:
    Actor_ResetAndSetPosition(actor, 372, 716);
    (*timer)++;
    return;
  case 96:
    Actor_ResetAndSetPosition(actor, 392, 712);
    (*timer)++;
    return;
  case 98:
    Actor_ResetAndSetPosition(actor, 392, 696);
    (*timer)++;
    return;
  case 100:
    Actor_ResetAndSetPosition(actor, 412, 692);
    (*timer)++;
    return;
  case 102:
    Actor_ResetAndSetPosition(actor, 416, 680);
    (*timer)++;
    return;
  case 104:
    Actor_ResetAndSetPosition(actor, 412, 668);
    (*timer)++;
    return;
  case 106:
    Actor_ResetAndSetPosition(actor, 392, 664);
    (*timer)++;
    return;
  case 108:
    Actor_ResetAndSetPosition(actor, 372, 668);
    (*timer)++;
    return;
  case 110:
    Actor_ResetAndSetPosition(actor, 368, 680);
    (*timer)++;
    return;
  case 112:
    Actor_ResetAndSetPosition(actor, 320, 680);
    (*timer)++;
    return;
  case 114:
    Actor_ResetAndSetPosition(actor, 316, 692);
    (*timer)++;
    return;
  case 116:
    Actor_ResetAndSetPosition(actor, 296, 696);
    (*timer)++;
    return;
  case 118:
    Actor_ResetAndSetPosition(actor, 276, 692);
    (*timer)++;
    return;
  case 120:
    Actor_ResetAndSetPosition(actor, 272, 680);
    (*timer)++;
    return;
  case 122:
    Actor_ResetAndSetPosition(actor, 276, 668);
    (*timer)++;
    return;
  case 124:
    Actor_ResetAndSetPosition(actor, 296, 664);
    (*timer)++;
    return;
  case 126:
    Actor_ResetAndSetPosition(actor, 316, 668);
    (*timer)++;
    return;
  case 128:
    Actor_ResetAndSetPosition(actor, 320, 680);
    (*timer)++;
    return;
  case 130:
    Actor_ResetAndSetPosition(actor, 316, 692);
    (*timer)++;
    return;
  case 132:
    Actor_ResetAndSetPosition(actor, 296, 696);
    (*timer)++;
    return;
  case 134:
    Actor_ResetAndSetPosition(actor, 276, 692);
    (*timer)++;
    return;
  case 136:
    Actor_ResetAndSetPosition(actor, 272, 680);
    (*timer)++;
    return;
  case 138:
    Actor_ResetAndSetPosition(actor, 276, 668);
    (*timer)++;
    return;
  case 140:
    Actor_ResetAndSetPosition(actor, 296, 664);
    (*timer)++;
    return;
  case 142:
    Actor_ResetAndSetPosition(actor, 316, 668);
    (*timer)++;
    return;
  case 144:
    Actor_ResetAndSetPosition(actor, 320, 680);
    (*timer)++;
    return;
  case 146:
    Actor_ResetAndSetPosition(actor, 316, 692);
    (*timer)++;
    return;
  case 148:
    Actor_ResetAndSetPosition(actor, 296, 696);
    (*timer)++;
    return;
  case 150:
    Actor_ResetAndSetPosition(actor, 296, 728);
    (*timer)++;
    return;
  case 152:
    Actor_ResetAndSetPosition(actor, 272, 728);
    (*timer)++;
    return;
  case 154:
    Actor_ResetAndSetPosition(actor, 268, 740);
    (*timer)++;
    return;
  case 156:
    Actor_ResetAndSetPosition(actor, 248, 744);
    (*timer)++;
    return;
  case 158:
    Actor_ResetAndSetPosition(actor, 228, 740);
    (*timer)++;
    return;
  case 160:
    Actor_ResetAndSetPosition(actor, 224, 728);
    (*timer)++;
    return;
  case 162:
    Actor_ResetAndSetPosition(actor, 228, 716);
    (*timer)++;
    return;
  case 164:
    Actor_ResetAndSetPosition(actor, 248, 712);
    (*timer)++;
    return;
  case 166:
    Actor_ResetAndSetPosition(actor, 268, 716);
    (*timer)++;
    return;
  case 168:
    Actor_ResetAndSetPosition(actor, 272, 728);
    (*timer)++;
    return;
  case 170:
    Actor_ResetAndSetPosition(actor, 268, 740);
    (*timer)++;
    return;
  case 172:
    Actor_ResetAndSetPosition(actor, 248, 744);
    (*timer)++;
    return;
  case 174:
    Actor_ResetAndSetPosition(actor, 228, 740);
    (*timer)++;
    return;
  case 176:
    Actor_ResetAndSetPosition(actor, 224, 728);
    (*timer)++;
    return;
  case 178:
    Actor_ResetAndSetPosition(actor, 228, 716);
    (*timer)++;
    return;
  case 180:
    Actor_ResetAndSetPosition(actor, 248, 712);
    (*timer)++;
    return;
  case 182:
    Actor_ResetAndSetPosition(actor, 268, 716);
    (*timer)++;
    return;
  case 184:
    Actor_ResetAndSetPosition(actor, 272, 728);
    (*timer)++;
    return;
  case 186:
    Actor_ResetAndSetPosition(actor, 268, 740);
    (*timer)++;
    return;
  case 188:
    Actor_ResetAndSetPosition(actor, 248, 744);
    (*timer)++;
    return;
  case 190:
    Actor_ResetAndSetPosition(actor, 228, 740);
    (*timer)++;
    return;
  case 192:
    Actor_ResetAndSetPosition(actor, 224, 728);
    (*timer)++;
    return;
  case 194:
    Actor_ResetAndSetPosition(actor, 228, 716);
    (*timer)++;
    return;
  case 196:
    Actor_ResetAndSetPosition(actor, 248, 712);
    (*timer)++;
    return;
  case 198:
    Actor_ResetAndSetPosition(actor, 268, 716);
    (*timer)++;
    return;
  case 200:
    Actor_ResetAndSetPosition(actor, 272, 728);
    (*timer)++;
    return;
  case 202:
    Actor_ResetAndSetPosition(actor, 268, 740);
    (*timer)++;
    return;
  case 204:
    Actor_ResetAndSetPosition(actor, 248, 744);
    (*timer)++;
    return;
  case 206:
    Actor_ResetAndSetPosition(actor, 248, 760);
    (*timer)++;
    return;
  case 208:
    Actor_ResetAndSetPosition(actor, 228, 764);
    (*timer)++;
    return;
  case 210:
    Actor_ResetAndSetPosition(actor, 224, 776);
    (*timer)++;
    return;
  case 212:
    Actor_ResetAndSetPosition(actor, 228, 788);
    (*timer)++;
    return;
  case 214:
    Actor_ResetAndSetPosition(actor, 248, 792);
    (*timer)++;
    return;
  case 216:
    Actor_ResetAndSetPosition(actor, 248, 840);
    (*timer)++;
    return;
  case 218:
    Actor_ResetAndSetPosition(actor, 228, 844);
    (*timer)++;
    return;
  case 220:
    Actor_ResetAndSetPosition(actor, 224, 856);
    (*timer)++;
    return;
  case 222:
    Actor_ResetAndSetPosition(actor, 228, 868);
    (*timer)++;
    return;
  case 226:
    Actor_ResetAndSetPosition(actor, 268, 868);
    (*timer)++;
    return;
  case 228:
    Actor_ResetAndSetPosition(actor, 272, 856);
    (*timer)++;
    return;
  case 230:
    Actor_ResetAndSetPosition(actor, 268, 844);
    (*timer)++;
    return;
  case 232:
    Actor_ResetAndSetPosition(actor, 248, 840);
    (*timer)++;
    return;
  case 234:
    Actor_ResetAndSetPosition(actor, 228, 844);
    (*timer)++;
    return;
  case 236:
    Actor_ResetAndSetPosition(actor, 224, 856);
    (*timer)++;
    return;
  case 238:
    Actor_ResetAndSetPosition(actor, 228, 868);
    (*timer)++;
    return;
  case 223:
  case 239:
    if (Func_020006a4(object) != 0)
      (*timer)++;
  case 224:
  case 240:
    Actor_ResetAndSetPosition(actor, 248, 872);
    object->timer++;
    return;
  case 242:
    Actor_ResetAndSetPosition(actor, 268, 868);
    (*timer)++;
    return;
  case 244:
    Actor_ResetAndSetPosition(actor, 272, 856);
    (*timer)++;
    return;
  case 246:
    Actor_ResetAndSetPosition(actor, 268, 844);
    (*timer)++;
    return;
  case 248:
    Actor_ResetAndSetPosition(actor, 248, 840);
    (*timer)++;
    return;
  case 250:
    Actor_ResetAndSetPosition(actor, 228, 844);
    (*timer)++;
    return;
  case 252:
    Actor_ResetAndSetPosition(actor, 224, 856);
    (*timer)++;
    return;
  case 254:
    Actor_SetSpeedParameters(actor, 0x1cccc, 0x1cccc);
    Actor_ResetAndSetPosition(actor, 200, 856);
    (*timer)++;
    return;
  case 256:
    Actor_ResetAndSetPosition(actor, 200, 744);
    (*timer)++;
    return;
  case 258:
    Actor_SetSpeedParameters(actor, 0x13333, 0x13333);
    Actor_ResetAndSetPosition(actor, 180, 740);
    (*timer)++;
    return;
  case 260:
    Actor_ResetAndSetPosition(actor, 176, 728);
    (*timer)++;
    return;
  case 262:
    Actor_ResetAndSetPosition(actor, 180, 716);
    (*timer)++;
    return;
  case 264:
    Actor_ResetAndSetPosition(actor, 200, 712);
    (*timer)++;
    return;
  case 266:
    Actor_ResetAndSetPosition(actor, 220, 716);
    (*timer)++;
    return;
  case 268:
    Actor_ResetAndSetPosition(actor, 224, 728);
    (*timer)++;
    return;
  case 270:
    Actor_ResetAndSetPosition(actor, 220, 740);
    (*timer)++;
    return;
  case 272:
    Actor_ResetAndSetPosition(actor, 200, 744);
    (*timer)++;
    return;
  case 274:
    Actor_ResetAndSetPosition(actor, 180, 740);
    (*timer)++;
    return;
  case 276:
    Actor_ResetAndSetPosition(actor, 176, 728);
    (*timer)++;
    return;
  case 278:
    Actor_ResetAndSetPosition(actor, 180, 716);
    (*timer)++;
    return;
  case 280:
    Actor_ResetAndSetPosition(actor, 200, 712);
    (*timer)++;
    return;
  case 282:
    Actor_ResetAndSetPosition(actor, 220, 716);
    (*timer)++;
    return;
  case 284:
    Actor_ResetAndSetPosition(actor, 224, 728);
    (*timer)++;
    return;
  case 286:
    Actor_ResetAndSetPosition(actor, 220, 740);
    (*timer)++;
    return;
  case 288:
    Actor_ResetAndSetPosition(actor, 200, 744);
    (*timer)++;
    return;
  case 290:
    Actor_ResetAndSetPosition(actor, 180, 740);
    (*timer)++;
    return;
  case 292:
    Actor_ResetAndSetPosition(actor, 176, 728);
    (*timer)++;
    return;
  case 294:
    Actor_ResetAndSetPosition(actor, 180, 716);
    (*timer)++;
    return;
  case 296:
    Actor_ResetAndSetPosition(actor, 200, 712);
    (*timer)++;
    return;
  case 298:
    Actor_ResetAndSetPosition(actor, 220, 716);
    (*timer)++;
    return;
  case 300:
    Actor_ResetAndSetPosition(actor, 224, 728);
    (*timer)++;
    return;
  case 302:
    Actor_ResetAndSetPosition(actor, 220, 740);
    (*timer)++;
    return;
  case 304:
    Actor_ResetAndSetPosition(actor, 200, 744);
    (*timer)++;
    return;
  case 306:
    Actor_ResetAndSetPosition(actor, 180, 740);
    (*timer)++;
    return;
  case 308:
    Actor_ResetAndSetPosition(actor, 176, 728);
    (*timer)++;
    return;
  case 310:
    Actor_ResetAndSetPosition(actor, 180, 716);
    (*timer)++;
    return;
  case 312:
    Actor_ResetAndSetPosition(actor, 200, 712);
    (*timer)++;
    return;
  case 314:
    Actor_ResetAndSetPosition(actor, 200, 680);
    (*timer)++;
    return;
  case 316:
    Actor_ResetAndSetPosition(actor, 212, 676);
    (*timer)++;
    return;
  case 318:
    Actor_ResetAndSetPosition(actor, 216, 664);
    (*timer)++;
    return;
  case 320:
    Actor_ResetAndSetPosition(actor, 212, 652);
    (*timer)++;
    return;
  case 322:
    Actor_ResetAndSetPosition(actor, 200, 648);
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
