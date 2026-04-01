#include "ReadFileMethod.h"
#include "Date.h"
#include "Time.h"
#include "MonthKey.h"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::getline;
using std::cout;
using std::endl;
using std::stoi;
using std::stof;

// Declaration
static bool ReadWeatherDataFile(const string& filename, WeatherLogType& weather_data);

// Remove leading spaces, tabs, and carriage returns from a string
static string Trim(const string& str)
{
    int start = 0;

    while (start < (int)str.length() &&
           (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
    {
        start++;
    }

    string result = "";

    for (int i = start; i < (int)str.length(); i++)
    {
        if (str[i] != '\r' && str[i] != '\t')
        {
            result = result + str[i];
        }
    }

    return result;
}

// Detect whether the file uses tab or comma as delimiter
static char DetectDelimiter(const string& headerLine)
{
    for (int i = 0; i < (int)headerLine.length(); i++)
    {
        if (headerLine[i] == '\t')
        {
            return '\t';
        }
    }

    return ',';
}

// Convert "dd/mm/yyyy hh:mm" string into Date and Time objects
static bool ParseDateTime(const string& wast, Date& date, Time& time)
{
    string datePart = "";
    string timePart = "";
    bool spaceFound = false;

    for (int i = 0; i < (int)wast.length(); i++)
    {
        if (wast[i] == ' ')
        {
            spaceFound = true;
            continue;
        }

        if (!spaceFound)
        {
            datePart = datePart + wast[i];
        }
        else
        {
            timePart = timePart + wast[i];
        }
    }

    if (!spaceFound)
    {
        return false;
    }

    try
    {
        string temp = "";
        int part = 0;
        int day = 0;
        int month = 0;
        int year = 0;

        for (int i = 0; i < (int)datePart.length(); i++)
        {
            if (datePart[i] == '/')
            {
                if (part == 0)
                {
                    day = stoi(temp);
                }
                else if (part == 1)
                {
                    month = stoi(temp);
                }

                temp = "";
                part++;
            }
            else
            {
                temp = temp + datePart[i];
            }
        }

        year = stoi(temp);

        date.SetDay(day);
        date.SetMonth(month);
        date.SetYear(year);
    }
    catch (...)
    {
        return false;
    }

    try
    {
        string temp = "";
        int hour = 0;
        int minute = 0;

        for (int i = 0; i < (int)timePart.length(); i++)
        {
            if (timePart[i] == ':')
            {
                hour = stoi(temp);
                temp = "";
            }
            else
            {
                temp = temp + timePart[i];
            }
        }

        minute = stoi(temp);

        time.SetHour(hour);
        time.SetMinute(minute);
        time.SetSecond(0);
    }
    catch (...)
    {
        return false;
    }

    return date.IsValid() && time.IsValid();
}

// Check if field is missing
static bool IsOffline(const string& s)
{
    if (s.empty()) return true;
    if (s == "N/A") return true;
    if (s == "NA") return true;
    return false;
}

// Convert string to float safely
static bool TryParseFloat(const string& s, float& out)
{
    if (IsOffline(s))
    {
        return false;
    }

    try
    {
        out = stof(s);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

// Check for duplicate timestamp within one month-year bucket
static bool RecordExistsInVector(const Vector<WeatherRecType>& records, const WeatherRecType& rec)
{
    for (int i = 0; i < records.Size(); i++)
    {
        if (records[i].d == rec.d && records[i].t == rec.t)
        {
            return true;
        }
    }

    return false;
}

// Read one CSV weather file
static bool ReadWeatherDataFile(const string& filename, WeatherLogType& weather_data)
{
    ifstream file(filename.c_str());

    if (!file.is_open())
    {
        cout << "Could not open CSV file: " << filename << endl;
        return false;
    }

    string headerLine;

    if (!getline(file, headerLine))
    {
        cout << "Could not read header from: " << filename << endl;
        return false;
    }

    cout << "Header in " << filename << ": " << headerLine << endl;

    char delim = DetectDelimiter(headerLine);

    int wastCol = -1;
    int sCol = -1;
    int srCol = -1;
    int tCol = -1;

    int colIndex = 0;
    string temp = "";

    for (int i = 0; i <= (int)headerLine.length(); i++)
    {
        if (i == (int)headerLine.length() || headerLine[i] == delim)
        {
            string col = Trim(temp);

            if (col == "WAST") wastCol = colIndex;
            if (col == "S")    sCol = colIndex;
            if (col == "SR")   srCol = colIndex;
            if (col == "T")    tCol = colIndex;

            temp = "";
            colIndex++;
        }
        else
        {
            temp = temp + headerLine[i];
        }
    }

    if (wastCol == -1 || sCol == -1 || srCol == -1 || tCol == -1)
    {
        cout << "Missing required columns in file: " << filename << endl;
        cout << "Detected columns -> "
             << "WAST: " << wastCol << ", "
             << "S: " << sCol << ", "
             << "SR: " << srCol << ", "
             << "T: " << tCol << endl;
        return false;
    }

    string line;

    while (getline(file, line))
    {
        string wastStr = "";
        string sStr = "";
        string srStr = "";
        string tStr = "";

        int idx = 0;
        temp = "";

        for (int i = 0; i <= (int)line.length(); i++)
        {
            if (i == (int)line.length() || line[i] == delim)
            {
                string field = Trim(temp);

                if (idx == wastCol) wastStr = field;
                if (idx == sCol)    sStr = field;
                if (idx == srCol)   srStr = field;
                if (idx == tCol)    tStr = field;

                temp = "";
                idx++;
            }
            else
            {
                temp = temp + line[i];
            }
        }

        if (IsOffline(wastStr))
        {
            continue;
        }

        Date date;
        Time time;

        if (!ParseDateTime(wastStr, date, time))
        {
            continue;
        }

        WeatherRecType rec;
        rec.d = date;
        rec.t = time;
        rec.speed = 0.0f;
        rec.solar = 0.0f;
        rec.temp = 0.0f;

        rec.hasSpeed = TryParseFloat(sStr, rec.speed);
        rec.hasSolar = TryParseFloat(srStr, rec.solar);
        rec.hasTemp  = TryParseFloat(tStr, rec.temp);

        MonthKey key(date.GetYear(), date.GetMonth());

        Vector<WeatherRecType>* monthRecords = weather_data.Find(key);

        if (monthRecords != 0)
        {
            // Duplicate check
            if (!RecordExistsInVector(*monthRecords, rec))
            {
                if (!monthRecords->Insert(rec, monthRecords->Size()))
                {
                    cout << "Insert into month bucket failed in file: " << filename << endl;
                    return false;
                }
            }
        }
        else
        {
            Vector<WeatherRecType> newRecords;

            if (!newRecords.Insert(rec, 0))
            {
                cout << "Initial vector insert failed in file: " << filename << endl;
                return false;
            }

            if (!weather_data.Insert(key, newRecords))
            {
                cout << "Map insert failed in file: " << filename << endl;
                return false;
            }
        }
    }

    return true;
}

// Read all CSV file names from data_source.txt
bool ReadWeatherDataFromSource(const string& sourceFile, WeatherLogType& weather_data)
{
    ifstream src(sourceFile.c_str());

    if (!src.is_open())
    {
        cout << "Could not open source file: " << sourceFile << endl;
        return false;
    }

    string dataFileName;
    bool loadedAtLeastOneFile = false;

    while (getline(src, dataFileName))
    {
        dataFileName = Trim(dataFileName);

        if (dataFileName.empty())
        {
            continue;
        }

        string fullPath = "data/" + dataFileName;
        cout << "Trying file: " << fullPath << endl;

        if (!ReadWeatherDataFile(fullPath, weather_data))
        {
            cout << "Failed file: " << fullPath << endl;
            return false;
        }

        loadedAtLeastOneFile = true;
    }

    if (!loadedAtLeastOneFile)
    {
        cout << "No filenames found in source file." << endl;
        return false;
    }

    return true;
}
