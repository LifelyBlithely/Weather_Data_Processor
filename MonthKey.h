#ifndef MONTHKEY_H
#define MONTHKEY_H

#include <iostream>
#include <string>

using std::ostream;
using std::string;

/**
 * @file MonthKey.h
 * @brief Defines the MonthKey class used as the key in the custom Map.
 */

/**
 * @class MonthKey
 * @brief Represents a month-year key for grouping weather records.
 *
 * The MonthKey class stores:
 * - month
 * - year
 *
 * It supports:
 * - validation
 * - formatted output
 * - chronological comparison
 */
class MonthKey
{
public:
    /**
     * @brief Default constructor.
     */
    MonthKey();

    /**
     * @brief Parameterised constructor.
     * @param year Year value.
     * @param month Month value.
     */
    MonthKey(int year, int month);

    /**
     * @brief Checks whether the stored month-year is valid.
     * @return True if valid, false otherwise.
     */
    bool IsValid() const;

    /**
     * @brief Gets the month value.
     * @return Month value.
     */
    int GetMonth() const;

    /**
     * @brief Sets the month value.
     * @param month Month value.
     */
    void SetMonth(int month);

    /**
     * @brief Gets the year value.
     * @return Year value.
     */
    int GetYear() const;

    /**
     * @brief Sets the year value.
     * @param year Year value.
     */
    void SetYear(int year);

    /**
     * @brief Returns the key as a string.
     * @return MonthKey in mm/yyyy format.
     */
    string GetKey() const;

    /**
     * @brief Less-than comparison operator for MonthKey.
     * @param other The MonthKey to compare with.
     * @return True if this MonthKey is earlier than other.
     */
    bool operator<(const MonthKey& other) const;

    /**
     * @brief Greater-than comparison operator for MonthKey.
     * @param other The MonthKey to compare with.
     * @return True if this MonthKey is later than other.
     */
    bool operator>(const MonthKey& other) const;

    /**
     * @brief Equality comparison operator for MonthKey.
     * @param other The MonthKey to compare with.
     * @return True if both MonthKey objects are identical.
     */
    bool operator==(const MonthKey& other) const;

private:
    int m_month;
    int m_year;
};

/**
 * @brief Output operator for MonthKey.
 * @param os Output stream.
 * @param key MonthKey to print.
 * @return Output stream.
 */
ostream& operator<<(ostream& os, const MonthKey& key);

#endif
