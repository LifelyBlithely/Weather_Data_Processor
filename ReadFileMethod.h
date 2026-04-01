#ifndef READFILEMETHOD_H
#define READFILEMETHOD_H

#include "WeatherLogType.h"
#include <string>

using std::string;

/**
 * @file ReadFileMethod.h
 * @brief Functions used to read weather data from CSV files.
 *
 * This module loads weather records from the files listed in
 * data_source.txt and stores them in the WeatherLogType container.
 */

/**
 * @brief Reads weather data files listed in a source file.
 *
 * The source file (usually data_source.txt) contains the names of
 * one or more CSV files. Each CSV file is read and the weather
 * records are inserted into the WeatherLogType container.
 *
 * @param sourceFile The path to the source file containing CSV filenames.
 * @param weather_data The container where weather records will be stored.
 *
 * @return true if the data files were read successfully, false otherwise.
 */

bool ReadWeatherDataFromSource(const string& sourceFile, WeatherLogType& weather_data);

#endif
