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

#include "ReadWAV.h"
#include "WriteWAV.h"

int main(int argc, char *argv[])
{
  ReadWAV read_wav;
  WriteWAV write_wav;

  if (argc != 3)
  {
    printf("Usage: %s <in.wav> <out.wav>\n", argv[0]);
    exit(0);
  }

  read_wav.set_quiet_mode(false);
  read_wav.open_file(argv[1]);

  write_wav.create(argv[2]);

  int16_t samples[2];

  while (true)
  {
    int count = read_wav.read_data(samples, 1);

    if (count != 1) { break; }

    write_wav.write_data(samples, 1);
  }

  write_wav.finish();

  return 0;
}

