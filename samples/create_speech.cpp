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

#include <string>

#include "ReadWAV.h"
#include "SoundUtil.h"
#include "WriteWAV.h"

int read_line(FILE *fp, std::string &line)
{
  line.clear();

  while (true)
  {
    int ch = getc(fp);
    if (ch == EOF) { break; }
    if (ch == '\r') { continue; }

    if (ch == '\n')
    {
      if (line.size() == 0) { continue; }
      break;
    }

    line += ch;
  } 

  if (line.size() == 0) { return -1; }

  return line.size();
}

int main(int argc, char *argv[])
{
  WriteWAV write_wav;

  if (argc != 3)
  {
    printf("Usage: %s <in.txt> <out.wav>\n", argv[0]);
    exit(0);
  }

  FILE *fp = fopen(argv[1], "rb");

  write_wav.create(argv[2]);

#if 0
  append_sound(write_wav, "assets/m.wav");
  append_sound(write_wav, "assets/iy.wav");
  append_sound(write_wav, "assets/k.wav");

  append_pause(write_wav);

  append_sound(write_wav, "assets/k.wav");
  append_sound(write_wav, "assets/oh.wav");
  append_sound(write_wav, "assets/n.wav");
#endif

  while (true)
  {
    std::string line;

    int n = read_line(fp, line);

    if (n == -1) { break; }

    if (line == "-")
    {
      write_wav.append_pause(0.4);
    }
      else
    {
      line = "assets/" + line + ".wav";
      SoundUtil::append_sound(write_wav, line.c_str());
    }
  }

  write_wav.finish();

  fclose(fp);

  return 0;
}

