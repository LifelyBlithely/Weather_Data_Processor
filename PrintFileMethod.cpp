#include "PrintFileMethod.h"
#include "Calculations.h"
#include "MonthKey.h"
#include <fstream>

using std::ofstream;

bool WriteWindTempSolarCSV(const WeatherLogType& data, int year, const string& filename)
{
    ofstream fout(filename.c_str());

    if (!fout.is_open())
    {
        return false;
    }

    fout << year << "\n";
    fout << "Month,Average Wind Speed(stdev, mad),Average Ambient Temperature(stdev, mad),Total Solar Radiation\n";

    for (int m = 1; m <= 12; m++)
    {
        MonthKey key(year, m);
        const Vector<WeatherRecType>* records = data.Find(key);

        bool monthHasData = (records != 0);
        bool monthHasAnyField = false;

        Vector<float> windValues;
        Vector<float> tempValues;

        if (records != 0)
        {
            for (int i = 0; i < records->Size(); i++)
            {
                const WeatherRecType& r = (*records)[i];

                if (r.hasSpeed || r.hasTemp || r.hasSolar)
                {
                    monthHasAnyField = true;
                }

                if (r.hasSpeed)
                {
                    windValues.Insert(r.speed * 3.6f, windValues.Size());
                }

                if (r.hasTemp)
                {
                    tempValues.Insert(r.temp, tempValues.Size());
                }
            }
        }

        float windAvg = 0.0f;
        float windSd = 0.0f;
        float windMad = 0.0f;
        bool hasWind = CalculateWindStatsForMonth(data, m, year, windAvg, windSd);

        if (hasWind && windValues.Size() > 0)
        {
            windMad = Round1(CalculateMAD(windValues, windAvg));
        }

        float tempAvg = 0.0f;
        float tempSd = 0.0f;
        float tempMad = 0.0f;
        bool hasTemp = CalculateTempStatsForMonth(data, m, year, tempAvg, tempSd);

        if (hasTemp && tempValues.Size() > 0)
        {
            tempMad = Round1(CalculateMAD(tempValues, tempAvg));
        }

        float solarTotal = 0.0f;
        bool monthExists = false;
        CalculateSolarTotalForMonth(data, m, year, solarTotal, monthExists);

        bool hasSolarValue = false;

        if (records != 0)
        {
            for (int i = 0; i < records->Size(); i++)
            {
                const WeatherRecType& r = (*records)[i];

                if (r.hasSolar && r.solar >= 100.0f)
                {
                    hasSolarValue = true;
                    break;
                }
            }
        }

        fout << MonthName(m) << ",";

        if (!monthHasData || !monthHasAnyField)
        {
            fout << "No Data,No Data,No Data\n";
            continue;
        }

        if (hasWind)
        {
            fout << windAvg << "(" << windSd << ", " << windMad << ")";
        }
        else
        {
            fout << "No Data";
        }

        fout << ",";

        if (hasTemp)
        {
            fout << tempAvg << "(" << tempSd << ", " << tempMad << ")";
        }
        else
        {
            fout << "No Data";
        }

        fout << ",";

        if (hasSolarValue)
        {
            fout << solarTotal;
        }
        else
        {
            fout << "No Data";
        }

        fout << "\n";
    }

    return true;
}
