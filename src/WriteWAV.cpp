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
#include <stdint.h>

#include "WriteWAV.h"

WriteWAV::WriteWAV() :
  fp          { nullptr },
  channels    {       2 },
  sample_rate {   44100 }
{
}

WriteWAV::~WriteWAV()
{
  if (fp != NULL) { finish(); }
}

int WriteWAV::create(const char *filename)
{
  fp = fopen(filename, "wb+");

  if (fp == NULL) { return -1; }

  write_header();

  return 0;
}

void WriteWAV::finish()
{
  write_footer();
  fclose(fp);

  fp = NULL;
}

void WriteWAV::write_data(int16_t *samples, int length)
{
  for (int i = 0; i < length; i += 2)
  {
    write_int16(samples[i + 0]);
    write_int16(samples[i + 1]);
  }
}

void WriteWAV::write_tone(float frequency, int volume, float seconds)
{
  if (frequency < 1)
  {
    frequency = 1.0;
    volume = 0;
  }

  int samples = sample_rate * seconds;
  float half_wave = sample_rate / frequency / 2;

  float t = 0.0;
  int i = 0;
  int value = volume;

  printf("frequence=%.1f samples=%d half_wave=%f\n",
    frequency,
    samples,
    half_wave);

  for (int n = 0; n < samples; n++)
  {
    write_int16(value);
    write_int16(0);

    i = i + 1;

    if (i >= (int)t)
    {
      value = -value;
      t += half_wave;
    }
  }
}

void WriteWAV::write_header()
{
  fwrite("RIFF", 1, 4, fp);
  marker.riff = ftell(fp);
  write_int32(0);
  fwrite("WAVE", 1, 4, fp);

  const int audio_format    = 1;
  const int bits_per_sample = 16;

  fwrite("fmt ", 1, 4, fp);
  write_int32(16);
  write_int16(audio_format);
  write_int16(channels);
  write_int32(sample_rate);
  write_int32(44100 * 2 * 2);
  write_int16(channels * (bits_per_sample) / 8);
  write_int16(bits_per_sample);

  fwrite("data", 1, 4, fp);
  marker.data = ftell(fp);
  write_int32(0);
}

void WriteWAV::write_footer()
{
  long offset_end = ftell(fp);

  fseek(fp, marker.data, SEEK_SET);
  write_int32(offset_end - marker.data - 4);

  fseek(fp, marker.riff, SEEK_SET);
  write_int32(offset_end - marker.riff - 4);

  fseek(fp, offset_end, SEEK_SET);
}

void WriteWAV::write_int8(int value)
{
  putc(value, fp);
}

void WriteWAV::write_int16(int value)
{
  putc(value & 0xff, fp);
  putc((value >> 8) & 0xff, fp);
}

void WriteWAV::write_int32(int value)
{
  putc(value & 0xff, fp);
  putc((value >> 8) & 0xff, fp);
  putc((value >> 16) & 0xff, fp);
  putc((value >> 24) & 0xff, fp);
}

