#ifndef PRINTFILEMETHOD_H
#define PRINTFILEMETHOD_H

#include "WeatherLogType.h"
#include <string>

using std::string;

/**
 * @file PrintFileMethod.h
 * @brief Functions for writing weather summary data to a CSV file.
 *
 * This file contains the function used to create the WindTempSolar.csv
 * output file required by the assignment.
 */

/**
 * @brief Writes monthly weather statistics to a CSV file.
 *
 * For the selected year, this function writes:
 * - Average wind speed and sample standard deviation
 * - Average temperature and sample standard deviation
 * - Total solar radiation
 *
 * @param data The container storing all weather records.
 * @param year The year to analyse.
 * @param filename The name of the output CSV file.
 *
 * @return true if the file was written successfully, false otherwise.
 */

bool WriteWindTempSolarCSV(const WeatherLogType& data, int year, const string& filename);

#endif
