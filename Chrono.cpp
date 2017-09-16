
//
// This is example code from Chapter 9.8 "The Date class" of 
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Chrono.h"

namespace Chrono {

//------------------------------------------------------------------------------
// Date member function definitions:
//------------------------------------------------------------------------------

Date::Date(int yy, Month mm, int dd)
    : y(yy), m(mm), d(dd)
{
    if (!is_date(yy,mm,dd)) throw Invalid();
}

//------------------------------------------------------------------------------

Date::Date(Month mm, int dd, int yy)
    : m(mm), d(dd), y(yy)
{
    if (!is_date(yy,mm,dd)) throw Invalid();
}

//------------------------------------------------------------------------------

const Date& default_date()
{
    static const Date dd(2001,Date::jan,1); // start of 21st century
    return dd;
}

//------------------------------------------------------------------------------

Date::Date()
    //:y(default_date().year()),
    //m(default_date().month()),
    //d(default_date().day())
{
    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream oss;
    oss << put_time(&tm, "%d %m %Y");
    auto str = oss.str();

    //cout << "Current Date is " << str << endl;

    int day; int month; int year;
    istringstream iss(str);
    iss >> day >> month >> year;
    if (!is_date(year,Month(month),day)) throw Invalid();
    y = year;
    m = Month(month);
    d = day;
}

//------------------------------------------------------------------------------

void Date:: add_day(int n)
{
    // ...
}

//------------------------------------------------------------------------------

void Date::add_month(int n)
{
    // ...
}

//------------------------------------------------------------------------------

void Date::add_year(int n)
{
    if (m==feb && d==29 && !leapyear(y+n)) { // beware of leap years!
        m = mar;        // use March 1 instead of February 29
        d = 1;
    }
    y+=n;
}

//------------------------------------------------------------------------------
// Date helper functions:
//------------------------------------------------------------------------------

bool is_date(int y, Date::Month  m, int d)
{
    // check that y is valid
    if (y<0) return false;             // y must be positive

    // check that m is valid
    int mint = m;                      // mint must be 1 to 12
    if ((mint<1) || (mint>12)) return false;

    if (d<=0) return false;            // d must be positive

    int days_in_month = 31;            // most months have 31 days

    switch (m) {
        case Date::feb:                // the length of February varies
            days_in_month = (leapyear(y))?29:28;
            break;
        case Date::apr: case Date::jun: case Date::sep: case Date::nov:
            days_in_month = 30;        // the rest have 30 days
            break;
    }

    if (days_in_month<d) return false;

    return true;
} 

//------------------------------------------------------------------------------

bool leapyear(int y)
{
    // See exercise ???
    return false;
}

//------------------------------------------------------------------------------

bool operator==(const Date& a, const Date& b)
{
    return a.year()==b.year()
        && a.month()==b.month()
        && a.day()==b.day();
}

//------------------------------------------------------------------------------

bool operator!=(const Date& a, const Date& b)
{
    return !(a==b);
}

//------------------------------------------------------------------------------

ostream& operator<<(ostream& os, const Date& d)
{
    return os << '(' << d.year()
              << ',' << d.month()
              << ',' << d.day() 
              << ')';
}

//------------------------------------------------------------------------------

istream& operator>>(istream& is, Date& dd)
{
    int y, m, d;
    char ch1, ch2, ch3, ch4;
    is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
    if (!is) return is;
    if (ch1!='(' || ch2!=',' || ch3!=',' || ch4!=')') { // oops: format error
        is.clear(ios_base::failbit);                    // set the fail bit
        return is;
    }
    dd = Date(y,Date::Month(m),d);     // update dd
    return is;
}

//------------------------------------------------------------------------------

enum Day {
    sunday, monday, tuesday, wednesday, thursday, friday, saturday
};

//------------------------------------------------------------------------------

Day day_of_week(const Date& d)
{
    // ...
    return sunday;
}

//------------------------------------------------------------------------------

Date next_Sunday(const Date& d)
{
    // ...
    return d;
}

//------------------------------------------------------------------------------

Date next_weekday(const Date& d)
{
    // ...
    return d;
}

//------------------------------------------------------------------------------
// Time member function definitions:
//------------------------------------------------------------------------------

Time::Time(int hh, int mm, int ss)
    : h(hh), m(mm), s(ss)
{
    if (!is_time_24(hh,mm,ss)) throw Invalid();
    set_am_pm(); // valid, so now fix up the am_pm setting
}

//------------------------------------------------------------------------------

Time::Time(int hh, int mm, int ss, AmPm aa)
  : h(hh), m(mm), s(ss), a(aa)
{
    if (!is_time_12(hh,mm,ss,aa)) throw Invalid();
    //if (aa == pm) h = hh + 12;  // only do this if internal Time class format is 24-hr
}

//------------------------------------------------------------------------------

const Time& default_time()
{
    static const Time dt(0,0,0,Time::am); // 12am midnight
    return dt;
}

//------------------------------------------------------------------------------

Time::Time()
    //:h(default_time().hour()),
    //m(default_time().minute()),
    //s(default_time().second()),
    //a(default_time().am_pm())
{
    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream oss;
    oss << put_time(&tm, "%H %M %S");
    auto str = oss.str();

    //cout << "Current Time is " << str << endl;

    int hour; int minute; int second;
    istringstream iss(str);
    iss >> hour >> minute >> second;
    if (!is_time_24(hour,minute,second)) throw Invalid();
    h = hour;
    m = minute;
    s = second;
    set_am_pm(); // now fix up the am_pm setting
}

//------------------------------------------------------------------------------

void Time::set_am_pm()
{
    if (h > 11) { // afternoon?
        h -= 12;  // only do this if internal Time class format is 12-hr
        a = pm;
    } else { // morning
        a = am;
    }
}

//------------------------------------------------------------------------------
// Time helper functions:
//------------------------------------------------------------------------------

bool is_time_24(int h, int m, int s)
{

    if ((h<0)||(m<0)||(s<0)) return false;      // test all must be positive
    if ((h>23)||(m>59)||(s>59)) return false;   // test all must be less than max

    return true;
} 

//------------------------------------------------------------------------------
bool is_time_12(int h, int m, int s, Time::AmPm a)
{

    if ((h<0)||(m<0)||(s<0)) return false;      // test all must be positive
    if ((h>11)||(m>59)||(s>59)) return false;   // test all must be less than max
    //if ((h<12)&&(a!=Time::am)) return false;    // test legal am
    //if ((h>11)&&(a!=Time::pm)) return false;    // test legal pm

    return true;
} 

//------------------------------------------------------------------------------

bool operator==(const Time& a, const Time& b)
{
    return a.hour()==b.hour()
        && a.minute()==b.minute()
        && a.second()==b.second()
        && a.am_pm()==b.am_pm();
}

//------------------------------------------------------------------------------

bool operator!=(const Time& a, const Time& b)
{
    return !(a==b);
}

//------------------------------------------------------------------------------

ostream& operator<<(ostream& os, const Time& t)
{
    return os << '(' << t.hour()
              << ':' << t.minute()
              << ':' << t.second() 
              << ':' << t.am_pm() 
              << ')';
}

//------------------------------------------------------------------------------

istream& operator>>(istream& is, Time& tt)
{
    int h, m, s, a;
    char ch1, ch2, ch3, ch4, ch5;
    is >> ch1 >> h >> ch2 >> m >> ch3 >> s >> ch4 >> a >> ch5;
    if (!is) return is;
    if (ch1!='(' || ch2!=':' || ch3!=':' || ch4!=':' || ch5!=')') { // oops: format error
        is.clear(ios_base::failbit);                                // set the fail bit
        return is;
    }
    tt = Time(h,m,s,Time::AmPm(a));     // update tt
    return is;
}

//------------------------------------------------------------------------------

} // Chrono

//------------------------------------------------------------------------------
