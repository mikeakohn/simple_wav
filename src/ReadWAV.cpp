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
#include <string.h>

#include "ReadWAV.h"

ReadWAV::ReadWAV() :
  read_count  { 0 },
  data_length { 0 },
  quiet       { true }
{
}

ReadWAV::~ReadWAV()
{
  fclose(in);
}

int ReadWAV::open_file(const char *filename)
{
  in = fopen(filename, "rb");

  if (in == NULL) { return -1; }

  parse_header();
  parse_fmt_chunk();
  parse_data_chunk();

  read_count = 0;

  return 0;
}

int ReadWAV::read_data(double *samples, int count)
{
  int16_t sample;
  int ch;
  int n;

  if (fmt_chunk.bits_per_sample == 8)
  {
    read_count += count;
    if (read_count > data_length) { return -1; }

    for (n = 0; n < count; n++)
    {
      ch = getc(in);
      sample = ch - 128;
      sample = (sample < -127) ? -127 : sample;
      samples[n] = (double)sample / -127.0;
    }
  }
    else
  if (fmt_chunk.bits_per_sample == 16)
  {
    read_count += count * 2;
    if (read_count > data_length) { return -1; }

    for (n = 0; n < count; n++)
    {
      sample = read_int16();
      sample = (sample < -32767) ? -32767 : sample;
      samples[n] = (double)sample / 32767.0;
    }
  }
    else
  {
    printf("Unknown wav format.\n");
    return 0;
  }

  return 0;
}

int ReadWAV::read_data(int16_t *samples, int count)
{
  int total_read = 0;
  int16_t sample;
  int ch;
  int n;

  if (fmt_chunk.bits_per_sample == 8)
  {
    for (n = 0; n < count; n++)
    {
      if (read_count > data_length) { return -1; }
      ch = getc(in);
      sample = ch - 128;
      samples[n] = sample << 8;

      read_count += 1;
      total_read += 1;
    }
  }
    else
  if (fmt_chunk.bits_per_sample == 16)
  {
    int i = 0;

    for (n = 0; n < count; n++)
    {
      if (read_count > data_length) { break; }

      sample = read_int16();
      samples[i++] = sample;
      read_count += 2;

      if (fmt_chunk.channel_numbers == 2)
      {
        sample = read_int16();
        samples[i++] = sample;
        read_count += 2;
      }

      total_read += 1;
    }
  }
    else
  {
    printf("Unknown wav format.\n");
    return -1;
  }

  return total_read;
}

int ReadWAV::read_chars(char *s, int n)
{
  int t,ch;

  for (t = 0; t < n; t++)
  {
    ch = getc(in);
    if (ch == EOF) return -1;
    s[t] = ch;
  }
  s[t] = 0;

  return 0;
}

uint32_t ReadWAV::read_int32()
{
unsigned int t;

  t = getc(in);
  t = (getc(in) << 8) | t;
  t = (getc(in) << 16) | t;
  t = (getc(in) << 24) | t;

  return t;
}

uint16_t ReadWAV::read_int16()
{
  uint16_t t;

  t = getc(in);
  t = (getc(in) << 8) | t;

  return t;
}


int ReadWAV::parse_header()
{
  int length;
  char fourcc[5];
  char riff_type[5];

  read_chars(fourcc, 4);
  if (strncmp(fourcc,"RIFF", 4) != 0)
  {
    printf("This file is not a RIFF/ReadWAV\n");
    return -1;
  }

  length = read_int32();
  read_chars(riff_type, 4);

  if (!quiet)
  {
    printf("RIFF Header\n");
    printf("----------------------------\n");
    printf("          Length: %d\n", length);
    printf("            Type: %s\n", riff_type);
    printf("----------------------------\n");
  }

  return 0;
}

int ReadWAV::parse_fmt_chunk()
{
  char chunk_type[5];
  int length;

  read_chars(chunk_type, 4);
  length = read_int32();
  fmt_chunk.format_type      = read_int16();
  fmt_chunk.channel_numbers  = read_int16();
  fmt_chunk.sample_rate      = read_int32();
  fmt_chunk.bytes_per_second = read_int32();
  fmt_chunk.bytes_per_sample = read_int16();
  fmt_chunk.bits_per_sample  = read_int16();

  if (!quiet)
  {
    printf("FMT Chunk\n");
    printf("----------------------------\n");
    printf("      Chunk Type: %s\n", chunk_type);
    printf("          Length: %d\n", length);
    printf("     Format Type: ");

    switch (fmt_chunk.format_type)
    {
      case 0:
        printf("Mono\n");
        break;
      case 1:
        printf("Stereo\n");
        break;
      default:
        printf("Unkown\n");
        break;
    }

    printf(" Channel Numbers: %d\n", fmt_chunk.channel_numbers);
    printf("     Sample Rate: %d\n", fmt_chunk.sample_rate);
    printf("Bytes Per Second: %d\n", fmt_chunk.bytes_per_second);
    printf("Bytes Per Sample: ");

    switch(fmt_chunk.bytes_per_sample)
    {
      case 1:
        printf("8 bit mono\n");
        break;
      case 2:
        printf("8 bit stereo or 16 bit mono");
        break;
      case 4:
       printf("16 bit stereo");
        break;
      default:
        printf("?\n");
        break;
    }

    printf(" (%d)\n", fmt_chunk.bytes_per_sample);

    printf(" Bits Per Sample: %d\n", fmt_chunk.bits_per_sample);
    printf("----------------------------\n");
  }

  return 0;
}

int ReadWAV::parse_data_chunk()
{
  char riff_type[5];

  read_chars(riff_type, 4);
  data_length = read_int32();

  if (!quiet)
  {
    printf("Data Header\n");
    printf("----------------------------\n");
    printf("          Length: %d\n", data_length);
    printf("            Type: %s\n", riff_type);
    printf("----------------------------\n");
  }

  if (strncmp(riff_type, "data", 4) != 0)
  {
    return -1;
  }

  return 0;
}

