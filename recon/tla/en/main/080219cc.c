// Draft, differ 2 of 56, instruction order only: steps each channel's byte
// script by command and argument pairs until a wait command (0xef, 0xf1,
// 0xfd, 0xfe), which backs up over itself and flags the player and the
// channel. No ☀️ twin found. Left: the native stores the advanced position
// before it narrows the old one (lsls r3 after the strb); here the shift is
// scheduled first. The case bodies sit in the native's order (0xf0, 0xf3,
// 0xf2, 0xff, default, then the waits); in value order, differ 37.
#include "types.h"

struct ScriptChannel {
  u8 unk00[2];
  u16 unk02;
  u8 unk04;
  u8 unk05[11];
  u8 *script;
  u8 position;
  u8 unk15[2];
  u8 unk17;
};

struct ScriptPlayer {
  u8 unk00[22];
  u8 unk16;
  u8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 channelCount;
  u8 unk1c[12];
  struct ScriptChannel *channels[1];
};

void Func_080219cc(struct ScriptPlayer *player) {
  s32 i;

  for (i = 0; i < player->channelCount; i++) {
    struct ScriptChannel *channel = player->channels[i];
    u8 command;
    u8 argument;

    if (channel == 0 || channel->script == 0)
      continue;
    for (;;) {
      command = channel->script[channel->position++];
      argument = channel->script[channel->position++];
      switch (command) {
      case 0xf0:
        channel->unk04 = argument;
        continue;
      case 0xf3:
        player->unk16 = argument;
        continue;
      case 0xf2:
        player->unk17 = argument;
        continue;
      case 0xff:
        channel->unk17 = 0xff;
        continue;
      case 0xf4:
      case 0xf5:
      case 0xf6:
      case 0xf7:
      case 0xf8:
      case 0xf9:
      case 0xfa:
      case 0xfc:
        continue;
      default:
        channel->unk17 = command;
        continue;
      case 0xef:
      case 0xf1:
      case 0xfd:
      case 0xfe:
        break;
      }
      break;
    }
    channel->position -= 2;
    player->unk19 = 1;
    channel->unk02 = 1;
  }
}
