/**
 *  simple_wav
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2025 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SoundUtil.h"
#include "ReadWAV.h"
#include "WriteWAV.h"

int SoundUtil::normalize(int16_t *samples, int start, int end)
{
  int16_t high = 0;

  for (int i = start; i < end; i++)
  {
    int16_t value = samples[i];
    if (value < 0) { value = -value; }

    if (value > high)
    {
      high = value;
    }
  }

  double ratio = 32767.0  / (float)high;

  for (int i = start; i < end; i++)
  {
    float value = samples[i];
    value = value * ratio;
    samples[i] = value;
  }

  printf("high=%d\n", high);

  return 0;
}

int SoundUtil::trim(int16_t *samples, int &start, int &end, int threshold)
{
  while (true)
  {
    if (start == end)
    {
      return -1;
    }

    if (samples[start] < -threshold || samples[start] > threshold) { break; }

    start += 2;
  }

  while (true)
  {
    if (start == end)
    {
      return -1;
    }

    if (samples[end - 2] < -threshold || samples[end - 2] > threshold)
    {
      break;
    }

    end -= 2;
  }

  return 0;
}

int SoundUtil::append_sound(WriteWAV &write_wav, const char *filename)
{
  ReadWAV read_wav;

  printf("append_sound(%s)\n", filename);

  read_wav.set_quiet_mode(true);
  read_wav.open_file(filename);

  const int threshold = 1200;

  // Allow up to 1MB of sound.
  const int length = 1024 * 1024 / 4;
  int16_t samples[length * 2];

  memset(samples, 0, sizeof(samples));

  int count = read_wav.read_data(samples, length);

  if (count <= 0)
  {
    printf("No data in %s\n", filename);
    return -1;
  }

  // Find where sound starts and where it stops.
  int start = 0;
  int end = length * 2;

  trim(samples, start, end, threshold);
  normalize(samples, start, end);

  // It appears the sound samples recorded are only on the one side.
  for (int i = start; i < end; i += 2)
  {
    samples[i + 1] = samples[i];
  }

  write_wav.write_data(samples + start, (end - start) / 2);

  return 0;
}

