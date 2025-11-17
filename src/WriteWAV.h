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

#ifndef WRITE_WAV_H
#define WRITE_WAV_H

#include <stdio.h>
#include <stdlib.h>

class WriteWAV
{
public:
  WriteWAV();
  ~WriteWAV();

  int create(const char *filename);
  void finish();

  void write_data(int16_t *samples, int length);
  void write_tone(float frequency, int volume, float seconds);

  void append_pause(float seconds)
  {
    write_tone(0, 0, seconds);
  }

private:
  void write_header();
  void write_footer();

  void write_int8(int value);
  void write_int16(int value);
  void write_int32(int value);

  FILE *fp;

  struct Marker
  {
    long riff;
    long data;
  } marker;

  int channels;
  int sample_rate;

};

#endif

