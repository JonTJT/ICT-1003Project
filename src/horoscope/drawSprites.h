
int backgroundColor = TS_16b_Black;

// Decommissioned, prints line by line, can be used for cool appearances
void drawMap(sprite bitmap, int start_x, int start_y) {
  display.goTo(start_x, start_y);
  for (int i = 0; i < bitmap.width * bitmap.height; i++) {
    if (i % bitmap.width == 0) {
      display.goTo(start_x, start_y + (i / bitmap.height));
    }
    display.writePixel(bitmap.bitmap[i]);
  }
}

// findMiddleY function returns the y value in which your bitmap should be printed in order for the image to be centered on the y-axis
int findMiddleY(sprite bitmap) {
  return (64 - bitmap.height) / 2;
}

// findMiddleX function returns the x value in which your bitmap should be printed in order for the image to be centered on the x-axis
int findMiddleX(sprite bitmap) {
  return (96 - bitmap.width) / 2;
}

// drawBuffer function draws a bitmap at the start_x and the start_y provided.
void drawBuffer(sprite bitmap, int start_x, int start_y) {
  uint8_t lineBuffer[96 * 64 * 1];
  display.goTo(0, 0);
  display.startData();
  for (int y = 0; y < 64; y++) {
    for (int b = 0; b < 96; b++) {
      lineBuffer[b * 2] = backgroundColor >> 8;
      lineBuffer[b * 2 + 1] = backgroundColor;
    }
    if (y >= start_y && y < start_y + bitmap.height) {
      int endX = start_x + bitmap.width;
      if (start_x < 96 && endX > 0) {
        int xBitmapOffset = 0;
        int xStart = 0;
        if (start_x < 0) xBitmapOffset -= start_x;
        if (start_x > 0) xStart = start_x;
        int yBitmapOffset = (y - start_y) * bitmap.width;
        for (int x = xStart; x < endX; x++) {
          unsigned int color = bitmap.bitmap[xBitmapOffset + yBitmapOffset++];
          if (color != backgroundColor) {
            lineBuffer[(x) * 2] = color >> 8;
            lineBuffer[(x) * 2 + 1] = color;
          }
        }
      }
    }
    display.writeBuffer(lineBuffer, 96 * 2);
  }
  display.endTransfer();
}
