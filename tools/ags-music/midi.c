/* Read standard MIDI tracks, retaining event order and absolute tick positions.
 */
#include "midi.h"

static unsigned be16(const uint8_t *p) { return (unsigned)p[0] << 8 | p[1]; }
static uint32_t be32(const uint8_t *p) {
  return (uint32_t)be16(p) << 16 | be16(p + 2);
}

static unsigned take(const Buffer *data, size_t *at) {
  if (*at >= data->len)
    fail("truncated MIDI event");
  return data->data[(*at)++];
}

static uint32_t vlq(const Buffer *data, size_t *at) {
  uint32_t value = 0;
  for (unsigned i = 0; i < 4; i++) {
    unsigned b = take(data, at);
    value = value << 7 | (b & 127);
    if (b < 128)
      return value;
  }
  fail("overlong MIDI variable-length quantity");
}

static Track track(const uint8_t *p, size_t size) {
  Buffer data = {(uint8_t *)p, size, size};
  Track result = {0};
  size_t at = 0;
  int64_t tick = 0;
  unsigned running = 0;
  while (at < size) {
    tick += vlq(&data, &at);
    if (at >= size)
      fail("truncated MIDI event");
    unsigned status = data.data[at];
    if (status < 128) {
      if (!running)
        fail("MIDI running status has no channel status");
      status = running;
    } else
      at++;
    MidiEvent event = {tick, result.count, status, 0, {0}};
    size_t length;
    if (status == 255) {
      event.meta = take(&data, &at);
      length = vlq(&data, &at);
      running = 0;
    } else if (status == 240 || status == 247) {
      length = vlq(&data, &at);
      running = 0;
    } else if (status >= 128 && status <= 239) {
      running = status;
      length = (status & 240) == 192 || (status & 240) == 208 ? 1 : 2;
    } else
      fail("unsupported MIDI system event");
    if (length > size - at)
      fail("truncated MIDI event payload");
    append(&event.data, p + at, length);
    at += length;
    if (status < 240)
      for (size_t i = 0; i < length; i++)
        if (event.data.data[i] >= 128)
          fail("invalid MIDI channel parameter");
    result.events = resize(result.events, result.count + 1, sizeof(MidiEvent));
    result.events[result.count++] = event;
  }
  return result;
}
Midi readMidi(const Buffer *data) {
  if (data->len < 14 || memcmp(data->data, "MThd", 4) ||
      be32(data->data + 4) != 6)
    fail("invalid MIDI header");
  unsigned format = be16(data->data + 8), count = be16(data->data + 10),
           division = be16(data->data + 12);
  if (format > 1 || !count || !division || division & 0x8000)
    fail("only format 0/1 PPQN MIDI is supported");
  Midi midi = {allocate(count, sizeof(Track)), 0, division};
  for (size_t at = 14; at < data->len;) {
    if (data->len - at < 8)
      fail("truncated MIDI chunk");
    size_t size = be32(data->data + at + 4);
    if (size > data->len - at - 8)
      fail("truncated MIDI chunk payload");
    if (!memcmp(data->data + at, "MTrk", 4)) {
      if (midi.count == count)
        fail("too many MIDI tracks");
      midi.tracks[midi.count++] = track(data->data + at + 8, size);
    }
    at += size + 8;
  }
  if (midi.count != count)
    fail("MIDI track count mismatch");
  return midi;
}

void freeMidi(Midi *m) {
  for (size_t i = 0; i < m->count; i++) {
    for (size_t j = 0; j < m->tracks[i].count; j++)
      free(m->tracks[i].events[j].data.data);
    free(m->tracks[i].events);
  }
  free(m->tracks);
  *m = (Midi){0};
}
