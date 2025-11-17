/**
 *  simple_wav
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2025 by Michael Kohn
 *
 */

#ifndef READ_WAV_H
#define READ_WAV_H

#include <stdint.h>

class ReadWAV
{
public:
  ReadWAV();
  virtual ~ReadWAV();

  int open_file(const char *filename);
  int read_data(double *samples, int count);
  int read_data(int16_t *samples, int count);
  int get_sample_rate() { return fmt_chunk.sample_rate; }
  void set_quiet_mode(bool value) { quiet = value; }

private:
  FILE *in;
  int read_count;
  int data_length;

  bool quiet;

  struct fmt_chunk_t
  {
    uint16_t format_type;
    uint16_t channel_numbers;
    uint32_t sample_rate;
    uint32_t bytes_per_second;
    uint16_t bytes_per_sample;
    uint16_t bits_per_sample;
  } fmt_chunk;

  int read_chars(char *s, int n);
  uint32_t read_int32();
  uint16_t read_int16();

  int parse_header();
  int parse_fmt_chunk();
  int parse_data_chunk();
};

#endif

