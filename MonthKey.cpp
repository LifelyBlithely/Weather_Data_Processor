#include "MonthKey.h"

using std::to_string;

// Default constructor.
MonthKey::MonthKey()
{
    m_month = 0;
    m_year = 0;
}

// Parameterised constructor.
MonthKey::MonthKey(int year, int month)
{
    m_year = year;
    m_month = month;
}

// Checks whether the stored month-year is valid.
bool MonthKey::IsValid() const
{
    return (m_month >= 1 && m_month <= 12 && m_year > 0);
}

// Gets the month value.
int MonthKey::GetMonth() const
{
    return m_month;
}

// Sets the month value.
void MonthKey::SetMonth(int month)
{
    m_month = month;
}

// Gets the year value.
int MonthKey::GetYear() const
{
    return m_year;
}

// Sets the year value.
void MonthKey::SetYear(int year)
{
    m_year = year;
}

// Returns the key as a string in mm/yyyy format.
string MonthKey::GetKey() const
{
    string result;

    if (m_month < 10)
    {
        result += "0";
    }

    result += to_string(m_month);
    result += "/";
    result += to_string(m_year);

    return result;
}

// Less-than comparison operator for MonthKey.
bool MonthKey::operator<(const MonthKey& other) const
{
    if (m_year != other.m_year)
    {
        return m_year < other.m_year;
    }

    return m_month < other.m_month;
}

// Greater-than comparison operator for MonthKey.
bool MonthKey::operator>(const MonthKey& other) const
{
    return other < *this;
}

// Equality comparison operator for MonthKey.
bool MonthKey::operator==(const MonthKey& other) const
{
    return m_month == other.m_month &&
           m_year == other.m_year;
}

// Output operator for MonthKey.
ostream& operator<<(ostream& os, const MonthKey& key)
{
    if (key.IsValid())
    {
        os << key.GetKey();
    }
    else
    {
        os << "Invalid MonthKey";
    }

    return os;
}
