#pragma once

#include "raylib.h"
#include "constants.h"
#include "xor_shift32.h"
#include <pthread.h>
#include <stdatomic.h>

void* DrawPointTriangle(void *arg);
void* DrawPointFern(void *arg);
void DrawFractalMultithread(void *(*function)(void *));