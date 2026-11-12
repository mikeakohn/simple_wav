/**
 *  a440
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Create a .wav file playing a 440Hz tone with:
 *   2 channels
 *   16 bit sound
 *   48000 samples a second
 *
 * Copyright 2025-2026 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ReadWAV.h"
#include "WriteWAV.h"

int main(int argc, char *argv[])
{
  WriteWAV write_wav;

  if (argc != 2)
  {
    printf("Usage: %s <out.wav>\n", argv[0]);
    exit(0);
  }

  write_wav.create(argv[1]);
  write_wav.set_sample_rate(48000);
  write_wav.set_channels(1);
  //write_wav.set_bits(16);
  write_wav.start();

#define USE_WRITE_TONE

#ifndef USE_WRITE_TONE 
  const int length = 48000 / 880;
  int16_t sample = 16384;

  for (int i = 0; i < 48000; i++)
  {
    if ((i % length) == 0) { sample = -sample; }
    write_wav.write_data(&sample, 1);
  }
#else
  write_wav.write_tone(440, 16384, 1.0);
#endif

  write_wav.finish();

  return 0;
}

