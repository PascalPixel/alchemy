/* Convert MIDI notes and conductor metadata into sound-engine assembly. */
#include "midi.h"
#include "../common/assembly.h"
#include <errno.h>

typedef struct {
  char *kind;
  Strings args;
} Event;
typedef struct {
  Event event;
  int64_t tick, raw;
  size_t order;
  bool grid, pending;
} Node;
typedef struct {
  int64_t tick, length;
} Meter;
static const unsigned durations[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 28, 30, 32, 36, 40, 42, 44, 48, 52,
    54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96};
static const char *controls[] = {"priority",
                                 "tempo",
                                 "key_shift",
                                 "voice",
                                 "volume",
                                 "pan",
                                 "pitch_bend",
                                 "pitch_bend_range",
                                 "lfo_speed",
                                 "lfo_delay",
                                 "modulation_depth",
                                 "modulation_type",
                                 NULL,
                                 NULL,
                                 "tuning"};
static int controlOpcode(const char *name) {
  for (unsigned i = 0; i < 15; i++)
    if (controls[i] && !strcmp(controls[i], name))
      return 0xba + i;
  return -1;
}

static int64_t signedNumber(const char *text) {
  errno = 0;
  char *end;
  long long n = strtoll(text, &end, 10);
  if (!*text || *end || errno)
    fail("invalid sequence integer: %s", text);
  return n;
}

static int64_t arg(const Event *e, size_t i) {
  if (i >= e->args.len)
    fail("%s is missing an argument", e->kind);
  return signedNumber(e->args.items[i]);
}

static unsigned bounded(const Event *e, size_t i, unsigned max) {
  int64_t n = arg(e, i);
  if (n < 0 || (uint64_t)n > max)
    fail("%s argument out of range", e->kind);
  return (unsigned)n;
}

static unsigned duration(int64_t n) {
  for (unsigned i = 0; i < 49; i++)
    if (durations[i] == n)
      return i;
  fail("duration %lld is not in the engine's duration table", (long long)n);
}

static Event event(const char *kind) { return (Event){copyString(kind), {0}}; }
static void addInt(Event *e, int64_t n) {
  char *s = formatString("%lld", (long long)n);
  addString(&e->args, s);
  free(s);
}

static Event control(const char *kind, int64_t n) {
  Event e = event(kind);
  addInt(&e, n);
  return e;
}

static void freeEvent(Event *e) {
  free(e->kind);
  freeStrings(&e->args);
  *e = (Event){0};
}

static Event parseEvent(const char *text) {
  Strings parts = split(text, ' ');
  if (!identifier(parts.items[0]))
    fail("invalid sequence event: %s", text);
  Event e = event(parts.items[0]);
  for (size_t i = 1; i < parts.len; i++) {
    if (!identifier(parts.items[i]))
      signedNumber(parts.items[i]);
    addString(&e.args, parts.items[i]);
  }
  freeStrings(&parts);
  return e;
}

static void arity(const Event *e, size_t n) {
  if (e->args.len != n)
    fail("%s needs %zu arguments", e->kind, n);
}

static unsigned parameter(const Event *e, size_t i, const char *kind) {
  int64_t n = arg(e, i);
  bool sign = !strcmp(kind, "key_shift") || !strcmp(kind, "pan") ||
              !strcmp(kind, "pitch_bend") || !strcmp(kind, "tuning");
  if (n < (sign ? -128 : 0) || n > (sign ? 127 : 255))
    fail("%s parameter out of range", kind);
  return (uint8_t)n;
}

static void encodeEvent(Assembly *a, const Event *e, int *running) {
  const char *k = e->kind;
  if (!strcmp(k, "label")) {
    arity(e, 1);
    labelAt(a, e->args.items[0], a->bytes.len, false);
  } else if (!strcmp(k, "wait")) {
    arity(e, 1);
    byte(&a->bytes, 0x80 + duration(arg(e, 0)));
  } else if (!strcmp(k, "fine") || !strcmp(k, "pattern_end")) {
    arity(e, 0);
    byte(&a->bytes, !strcmp(k, "fine") ? 0xb1 : 0xb4);
  } else if (!strcmp(k, "goto") || !strcmp(k, "pattern") ||
             !strcmp(k, "repeat")) {
    bool repeat = !strcmp(k, "repeat");
    arity(e, repeat ? 2 : 1);
    byte(&a->bytes, repeat ? 0xb5 : !strcmp(k, "goto") ? 0xb2 : 0xb3);
    if (repeat)
      byte(&a->bytes, bounded(e, 0, 255));
    addPointer(a, e->args.items[repeat ? 1 : 0], 0);
  } else if (!strcmp(k, "note") || !strcmp(k, "note_running")) {
    if (!e->args.len || e->args.len > 4)
      fail("note needs duration and up to three parameters");
    int opcode = 0xcf + (int)duration(arg(e, 0));
    bool run = !strcmp(k, "note_running");
    if (run && (e->args.len == 1 || (*running >= 0 && *running != opcode)))
      fail("invalid running note status");
    if (!run)
      byte(&a->bytes, opcode);
    for (size_t i = 1; i < e->args.len; i++)
      byte(&a->bytes, bounded(e, i, 127));
    *running = opcode;
  } else if (!strcmp(k, "note_end") || !strcmp(k, "note_end_running")) {
    bool run = !strcmp(k, "note_end_running");
    if (e->args.len > 1 || (run && e->args.len != 1))
      fail("invalid note end parameters");
    if (run && *running >= 0 && *running != 0xce)
      fail("invalid running note end status");
    if (!run)
      byte(&a->bytes, 0xce);
    if (e->args.len)
      byte(&a->bytes, bounded(e, 0, 127));
    *running = 0xce;
  } else {
    bool run = !strcmp(k, "control_running");
    arity(e, run ? 2 : 1);
    const char *name = run ? e->args.items[0] : k;
    int opcode = controlOpcode(name);
    if (opcode < 0)
      fail("unsupported sequence command: %s", name);
    unsigned value = parameter(e, run ? 1 : 0, name);
    if (run && (opcode == 0xbb || value >= 128 ||
                (*running >= 0 && *running != opcode)))
      fail("invalid running control status");
    if (!run)
      byte(&a->bytes, opcode);
    byte(&a->bytes, value);
    if (opcode != 0xbb)
      *running = opcode;
  }
}
typedef struct {
  int running, encoded, key, velocity;
  bool afterRest;
} State;
/* Derive the engine's running status from editable MIDI events, not markers. */
static void emit(Assembly *a, const Event *e, State *s) {
  const char *kind = e->kind;
  Event rebuilt = {0};
  const Event *out = e;
  if (!strcmp(kind, "note") || !strcmp(kind, "note_running")) {
    if (e->args.len < 3)
      fail("MIDI note needs duration, key and velocity");
    int key = bounded(e, 1, 127), velocity = bounded(e, 2, 127),
        opcode = 0xcf + (int)duration(arg(e, 0));
    rebuilt = event("note");
    addInt(&rebuilt, arg(e, 0));
    if (s->velocity != velocity) {
      addInt(&rebuilt, key);
      addInt(&rebuilt, velocity);
    } else if (s->key != key)
      addInt(&rebuilt, key);
    if (s->afterRest && s->running == opcode && rebuilt.args.len > 1) {
      free(rebuilt.kind);
      rebuilt.kind = copyString("note_running");
    }
    s->running = opcode;
    s->key = key;
    s->velocity = velocity;
    out = &rebuilt;
  } else if (!strcmp(kind, "label") || !strcmp(kind, "pattern") ||
             !strcmp(kind, "repeat") || !strcmp(kind, "goto")) {
    s->running = s->key = s->velocity = -1;
  } else if (!strcmp(kind, "control_running") ||
             !strcmp(kind, "note_end_running"))
    fail("running status is derived by the converter");
  else if (!strcmp(kind, "note_end")) {
    if (s->running == 0xce && e->args.len == 1) {
      rebuilt = control("note_end_running", arg(e, 0));
      out = &rebuilt;
    }
    s->running = 0xce;
  } else {
    int opcode = controlOpcode(kind);
    if (opcode >= 0 && opcode != 0xbb) {
      if (s->afterRest && s->running == opcode && e->args.len == 1 &&
          parameter(e, 0, kind) < 128) {
        rebuilt = event("control_running");
        addString(&rebuilt.args, kind);
        addString(&rebuilt.args, e->args.items[0]);
        out = &rebuilt;
      }
      s->running = opcode;
    }
  }
  encodeEvent(a, out, &s->encoded);
  if (strcmp(kind, "pattern_end"))
    s->afterRest = !strcmp(kind, "wait");
  freeEvent(&rebuilt);
}

static Event midiControl(const MidiEvent *e, int parameter[2], bool preamble) {
  const uint8_t *v = e->data.data;
  unsigned status = e->status & 240;
  if (status == 192)
    return control("voice", v[0]);
  if (status == 224)
    return control("pitch_bend", v[1]);
  if (status != 176)
    return (Event){0};
  switch (v[0]) {
  case 7:
    return control("volume", v[1]);
  case 10:
    return control("pan", v[1]);
  case 1:
    return control("modulation_depth", v[1]);
  case 101:
    parameter[0] = v[1];
    break;
  case 100:
    parameter[1] = v[1];
    break;
  case 6:
    if (parameter[0] == 0 && parameter[1] == 0) {
      if (!preamble)
        return control("pitch_bend_range", v[1]);
    } else if (parameter[0] == 0 && parameter[1] == 1)
      return control("tuning", v[1]);
    else
      fail("MIDI data entry names no registered parameter");
    break;
  case 0:
  case 38:
  case 91:
    break;
  default:
    fail("MIDI controller %u is not a sequence command", v[0]);
  }
  return (Event){0};
}

static int compareNodes(const void *left, const void *right) {
  const Node *a = left, *b = right;
  if (a->tick != b->tick)
    return a->tick < b->tick ? -1 : 1;
  return a->order < b->order ? -1 : a->order > b->order;
}

static void waits(Assembly *a, State *state, int64_t start, int64_t length,
                  const Meter *meter, size_t count) {
  if (start < 0 || length < 0)
    fail("MIDI rest starts before its stream");
  int64_t end = start + length;
  while (start < end) {
    size_t i = 0;
    while (i + 1 < count && meter[i + 1].tick <= start)
      i++;
    int64_t stop =
        meter[i].tick +
        ((start - meter[i].tick) / meter[i].length + 1) * meter[i].length;
    if (i + 1 < count && stop > meter[i + 1].tick)
      stop = meter[i + 1].tick;
    if (stop > end)
      stop = end;
    int64_t gap = stop - start;
    while (gap) {
      unsigned index = 48;
      while (durations[index] > gap)
        index--;
      Event e = control("wait", durations[index]);
      emit(a, &e, state);
      freeEvent(&e);
      gap -= durations[index];
    }
    start = stop;
  }
}

static void stream(Assembly *a, const Track *track, const Meter *meter,
                   size_t meterCount) {
  Node *nodes = NULL;
  size_t count = 0;
  int depth = 0, parameter[2] = {-1, -1};
  int64_t bracket = 0, removed = 0;
  bool preamble = true;
  for (size_t i = 0; i < track->count; i++) {
    const MidiEvent *e = track->events + i;
    Event derived = {0};
    const char *text = (char *)e->data.data;
    unsigned status = e->status & 240;
    if (e->status == 255 && e->meta == 0x51) {
      if (e->data.len != 3)
        fail("MIDI tempo needs three bytes");
      uint64_t micros = (unsigned)e->data.data[0] << 16 |
                        (unsigned)e->data.data[1] << 8 | e->data.data[2];
      if (!micros)
        fail("zero MIDI tempo");
      uint64_t numerator = UINT64_C(7200000000) * 280896,
               denominator = UINT64_C(16777216) * micros;
      derived = control("tempo", (int64_t)((2 * numerator + denominator) /
                                           (2 * denominator)));
    } else if (e->status < 240 && status != 128 && status != 144)
      derived = midiControl(e, parameter, preamble);
    if (e->status == 255 || status == 144)
      preamble = false;
    if (derived.kind) {
      if (!depth) {
        nodes = resize(nodes, count + 1, sizeof(Node));
        nodes[count++] =
            (Node){derived, e->tick - removed, e->tick, e->order, true, false};
      } else
        freeEvent(&derived);
      continue;
    }
    if (e->status == 255 && e->meta == 7) {
      if (!strcmp(text, "pattern+")) {
        if (!depth)
          bracket = e->tick;
        depth++;
      } else if (!strcmp(text, "pattern-")) {
        if (--depth < 0)
          fail("MIDI pattern bracket underflow");
        if (!depth)
          removed += e->tick - bracket;
      }
    } else if (e->status == 255 && e->meta == 6) {
      Event marked = parseEvent(text);
      const char *controls[] = {"tempo",
                                "voice",
                                "volume",
                                "pan",
                                "modulation_depth",
                                "pitch_bend",
                                "pitch_bend_range",
                                "tuning"};
      for (size_t c = 0; c < 8; c++)
        if (!strcmp(marked.kind, controls[c]))
          fail("MIDI marker duplicates a channel control");
      if (depth) {
        freeEvent(&marked);
        continue;
      }
      nodes = resize(nodes, count + 1, sizeof(Node));
      nodes[count++] =
          (Node){marked, e->tick - removed, e->tick, e->order, true, false};
    } else if (e->status < 240 && (status == 144 || status == 128)) {
      unsigned key = e->data.data[0], velocity = e->data.data[1];
      if (status == 144 && velocity) {
        Event note = control("note", 0);
        addInt(&note, key);
        addInt(&note, velocity);
        nodes = resize(nodes, count + 1, sizeof(Node));
        nodes[count++] = (Node){note,     e->tick - removed, e->tick,
                                e->order, depth == 0,        true};
      } else {
        size_t n = 0;
        while (n < count &&
               (!nodes[n].pending || arg(&nodes[n].event, 1) != key))
          n++;
        if (n == count)
          fail("MIDI note-off has no note-on");
        free(nodes[n].event.args.items[0]);
        nodes[n].event.args.items[0] =
            formatString("%lld", (long long)(e->tick - nodes[n].raw));
        nodes[n].pending = false;
      }
    }
  }
  if (depth)
    fail("MIDI pattern bracket is unclosed");
  for (size_t i = 0; i < count; i++)
    if (nodes[i].pending)
      fail("MIDI has an unclosed note-on");
  if (count > 1)
    qsort(nodes, count, sizeof(Node), compareNodes);
  State state = {-1, -1, -1, -1, false};
  int64_t cursor = 0;
  for (size_t i = 0; i < count; i++) {
    Node *n = nodes + i;
    if (n->grid) {
      if (n->tick < cursor)
        fail("MIDI event order moves backwards");
      int64_t gap = n->tick - cursor;
      waits(a, &state, n->raw - gap, gap, meter, meterCount);
      emit(a, &n->event, &state);
      cursor = n->tick;
    }
    freeEvent(&n->event);
  }
  free(nodes);
}

static const char *field(const Strings *fields, const char *key) {
  size_t n = strlen(key);
  const char *value = NULL;
  for (size_t i = 2; i < fields->len; i++)
    if (!strncmp(fields->items[i], key, n) && fields->items[i][n] == '=') {
      if (value)
        fail("repeated sequence header field: %s", key);
      value = fields->items[i] + n + 1;
    }
  if (!value)
    fail("missing sequence header field: %s", key);
  return value;
}

int main(int argc, char **argv) {
  const char *usage = "usage: ags-music INPUT.mid OUTPUT.s";
  startTool(argc, argv, usage);
  if (argc != 3)
    fail("%s", usage);
  Buffer input = readFile(argv[1]);
  Midi midi = readMidi(&input);
  free(input.data);
  const char *skeleton = NULL;
  Meter *meter = NULL;
  size_t meterCount = 0;
  for (size_t i = 0; i < midi.tracks[0].count; i++) {
    MidiEvent *e = midi.tracks[0].events + i;
    if (e->status != 255)
      continue;
    if (e->meta == 0x7f && e->data.len >= 15 &&
        !memcmp(e->data.data, "alchemy-mid2agb\0", 15))
      fail("MIDI contains a retired sidecar directive");
    if (e->meta == 1 && !skeleton)
      skeleton = (char *)e->data.data;
    if (e->meta == 0x58) {
      if (e->data.len < 2 || !e->data.data[0] || e->data.data[1] > 7)
        fail("invalid MIDI time signature");
      int64_t span = (int64_t)e->data.data[0] * midi.division * 4,
              denominator = INT64_C(1) << e->data.data[1];
      if (span % denominator ||
          (meterCount && meter[meterCount - 1].tick >= e->tick))
        fail("time signature does not fit the MIDI grid");
      meter = resize(meter, meterCount + 1, sizeof(Meter));
      meter[meterCount++] = (Meter){e->tick, span / denominator};
    }
  }
  if (!meterCount || meter[0].tick) {
    meter = resize(meter, meterCount + 1, sizeof(Meter));
    memmove(meter + 1, meter, meterCount * sizeof(Meter));
    meter[0] = (Meter){0, (int64_t)midi.division * 4};
    meterCount++;
  }
  if (!skeleton)
    fail("MIDI conductor skeleton is missing");
  Strings lines = split(skeleton, '\n');
  if (strcmp(lines.items[0], "smsh-sequence 1"))
    fail("unsupported MIDI conductor skeleton");
  if (lines.len && !*lines.items[lines.len - 1])
    free(lines.items[--lines.len]);
  Assembly a = {0};
  size_t streamIndex = 0;
  Strings toneBanks = {0};
  for (size_t i = 1; i < lines.len; i++) {
    Strings f = split(lines.items[i], ' ');
    if (!f.len)
      fail("empty skeleton row");
    if (!strcmp(f.items[0], "stream") && f.len == 2) {
      labelAt(&a, f.items[1], a.bytes.len, false);
      streamIndex++;
      Track empty = {0};
      stream(&a, streamIndex < midi.count ? midi.tracks + streamIndex : &empty,
             meter, meterCount);
    } else if (!strcmp(f.items[0], "align") && f.len == 3) {
      size_t n = number(f.items[1], 10, 256);
      if (n < 2)
        fail("sequence alignment must be 2..256");
      alignAssembly(&a, n, number(f.items[2], 10, 255));
    } else if (!strcmp(f.items[0], "header") && f.len >= 2) {
      const char *block = field(&f, "block_count"),
                 *priority = field(&f, "priority"),
                 *reverb = field(&f, "reverb"), *tone = field(&f, "tone_bank"),
                 *tracks = field(&f, "tracks");
      if (f.len != 7)
        fail("sequence header has unexpected fields");
      if (number(block, 10, 255))
        fail("nonzero sequence block count is unsupported");
      Strings list = split(tracks, ',');
      size_t count = 0;
      for (size_t j = 0; j < list.len; j++)
        count += *list.items[j] != 0;
      if (!count || count > 16)
        fail("sequence header needs 1..16 tracks");
      labelAt(&a, f.items[1], a.bytes.len, true);
      byte(&a.bytes, count);
      byte(&a.bytes, 0);
      byte(&a.bytes, number(priority, 10, 255));
      byte(&a.bytes, number(reverb, 10, 255));
      addPointer(&a, tone, 0);
      if (!hasString(&toneBanks, tone))
        addString(&toneBanks, tone);
      for (size_t j = 0; j < list.len; j++)
        if (*list.items[j])
          addPointer(&a, list.items[j], 0);
      freeStrings(&list);
    } else
      fail("unsupported conductor skeleton line: %s", lines.items[i]);
    freeStrings(&f);
  }
  for (size_t i = 0; i < a.pointerCount; i++) {
    bool found = hasString(&toneBanks, a.pointers[i].symbol);
    for (size_t j = 0; j < a.labelCount; j++)
      found |= !strcmp(a.labels[j].name, a.pointers[i].symbol);
    if (!found)
      fail("undefined sequence symbol: %s", a.pointers[i].symbol);
  }
  Buffer out = {0}, source = assemblySource(&a);
  print(&out, "\t.section .rodata\n");
  append(&out, source.data, source.len);
  writeFile(argv[2], &out);
  free(out.data);
  free(source.data);
  freeAssembly(&a);
  freeStrings(&toneBanks);
  freeStrings(&lines);
  free(meter);
  freeMidi(&midi);
  return 0;
}
