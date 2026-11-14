/// Follows the third map layer's scroll.
void Layer_TrackScroll(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  s16 y = layer->y >> 16;

  gSettlements_WaterfallLine = 1448 - y;
  gSettlements_WaterfallScrollAbove = layer->x >> 16;
  gSettlements_WaterfallScrollBelow =
      (s16)(layer->x >> 16) - (gFrameCount >> 2);
}
