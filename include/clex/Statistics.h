#ifndef CLEX_STATISTICS_H
#define CLEX_STATISTICS_H

#include "clex/Token.h"

void ClearStatistics();

void AddTokenToStatistics(const struct Token *tok);

void PrintStatistics();

#endif // CLEX_STATISTICS_H
