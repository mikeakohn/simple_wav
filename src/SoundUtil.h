/**
 *  simple_wav
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2025-2026 by Michael Kohn
 *
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "WriteWAV.h"

class SoundUtil
{
public:
  static int normalize(int16_t *samples, int start, int end);
  static int trim(int16_t *samples, int &start, int &end, int threshold);
  static int append_sound(WriteWAV &write_wav, const char *filename);

private:
  SoundUtil() { }
  ~SoundUtil() { }

};

