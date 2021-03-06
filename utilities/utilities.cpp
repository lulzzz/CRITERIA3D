#include <QDateTime>
#include <QVariant>
#include <QString>

#include "commonConstants.h"
#include "crit3dDate.h"
#include "utilities.h"
#include <sys/stat.h>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>

QStringList getFields(QSqlDatabase* db_, QString tableName)
{
    QSqlDriver* driver_ = db_->driver();
    QSqlRecord record_ = driver_->record(tableName);
    QStringList fieldList;
    for (int i=0; i < record_.count(); i++)
        fieldList.append(record_.fieldName(i));

    return fieldList;
}

bool getValue(QVariant myRs, int* myValue)
{
    if (! myRs.isValid() || myRs.isNull())
    {
        *myValue = NODATA;
    }
    else
    {
        if (myRs == "" || myRs == "NULL")
            *myValue = NODATA;
        else
            *myValue = myRs.toInt();
    }

    return (*myValue != NODATA);
}


bool getValue(QVariant myRs, float* myValue)
{
    if (myRs.isNull())
        *myValue = NODATA;
    else
    {
        if (myRs == "")
             *myValue = NODATA;
        else
            *myValue = myRs.toFloat();
    }

    return (int(*myValue) != int(NODATA));
}


bool getValue(QVariant myRs, double* myValue)
{
    if (myRs.isNull())
        *myValue = NODATA;
    else
    {
        if (myRs == "")
             *myValue = NODATA;
        else
            *myValue = myRs.toDouble();
    }

    return (int(*myValue) != int(NODATA));
}

bool getValue(QVariant myRs, QDate* myValue)
{
    if (myRs.isNull())
        return false;
    else
    {
        if (myRs == "")
             return false;
        else
            *myValue = myRs.toDate();
    }

    return true;
}

bool getValue(QVariant myRs, QDateTime* myValue)
{
    if (myRs.isNull())
        return false;
    else
    {
        if (myRs == "")
             return false;
        else
            *myValue = myRs.toDateTime();
    }

    return true;
}


bool getValue(QVariant myRs, QString* myValue)
{
    if (myRs.isNull())
        *myValue = "";
    else
        *myValue = myRs.toString();

    return (*myValue != "");
}


Crit3DDate getCrit3DDate(const QDate& d)
{
    Crit3DDate myDate = Crit3DDate(d.day(), d.month(), d.year());
    return myDate;
}


Crit3DTime getCrit3DTime(const QDateTime& t)
{
    Crit3DTime myTime;

    myTime.date.day = t.date().day();
    myTime.date.month = t.date().month();
    myTime.date.year = t.date().year();
    myTime.time = t.time().hour()*3600 + t.time().minute()*60 + t.time().second();

    return myTime;
}


Crit3DTime getCrit3DTime(const QDate& t, int hour)
{
    Crit3DTime myTime;

    myTime.date.day = t.day();
    myTime.date.month = t.month();
    myTime.date.year = t.year();
    myTime.time = hour * 3600;

    return myTime;
}


QDate getQDate(const Crit3DDate& d)
{
    QDate myDate = QDate(d.year, d.month, d.day);
    return myDate;
}


QDateTime getQDateTime(const Crit3DTime& t)
{
    QDateTime myTime;
    QDate myQDate = QDate(t.date.year, t.date.month, t.date.day);
    myTime.setDate(myQDate);
    QTime myQTime = QTime(t.getHour(), t.getMinutes(), int(t.getSeconds()), 0);
    myTime.setTime(myQTime);

    return myTime;
}


QString getFileName(QString fileNameComplete)
{
    QString c;
    QString fileName = "";
    for (int i = fileNameComplete.length()-1; i >= 0; i--)
    {
        c = fileNameComplete.mid(i,1);
        if ((c != "\\") && (c != "/"))
            fileName = c + fileName;
        else
            return fileName;
    }
    return fileName;
}


QString getFilePath(QString fileNameComplete)
{
    QString fileName = getFileName(fileNameComplete);
    QString filePath = fileNameComplete.left(fileNameComplete.length() - fileName.length());
    return filePath;
}


int decadeFromDate(QDate date)
{

    int day = date.day();
    int decade;
    if ( day <= 10)
    {
        decade = 1;
    }
    else if ( day <= 20)
    {
        decade = 2;
    }
    else
    {
        decade = 3;
    }
    decade = decade + (3 * (date.month() - 1));
    return decade;

}

void intervalDecade(int decade, int year, int* dayStart, int* dayEnd, int* month)
{
    int decMonth;

    *month = ((decade - 1) / 3) + 1;
    if ( (decade % 3) == 0)
    {
        decMonth = 3;
    }
    else
    {
        decMonth = decade % 3;
    }

    if (decMonth == 1)
    {
        *dayStart = 1;
        *dayEnd = 10;
    }
    else if (decMonth == 2)
    {
        *dayStart = 11;
        *dayEnd = 20;
    }
    else
    {
        *dayStart = 21;
        QDate temp(year, *month, 1);
        *dayEnd = temp.daysInMonth();
    }

}

int getSeasonFromDate(QDate date)
{

    int month = date.month();
    switch (month) {
    case 3: case 4: case 5:
        return 1;
    case 6: case 7: case 8:
        return 2;
    case 9: case 10: case 11:
        return 3;
    case 12: case 1: case 2:
        return 4;
    default:
        return NODATA;
    }
}


QString getStringSeasonFromDate(QDate date)
{

    int month = date.month();
    switch (month) {
    case 3: case 4: case 5:
        return "MAM";
    case 6: case 7: case 8:
        return "JJA";
    case 9: case 10: case 11:
        return "SON";
    case 12: case 1: case 2:
        return "DJF";
    default:
        return "";
    }
}


bool getPeriodDates(QString periodSelected, int year, QDate myDate, QDate* startDate, QDate* endDate)
{

   startDate->setDate(year, myDate.month(), myDate.day());
   endDate->setDate(year, myDate.month(), myDate.day());

   if (periodSelected == "Daily")
   {
        return true;
   }
   if (periodSelected == "Decadal")
   {
       int decade = decadeFromDate(myDate);
       int dayStart;
       int dayEnd;
       int month;
       intervalDecade(decade, myDate.year(), &dayStart, &dayEnd, &month);
       startDate->setDate(startDate->year(), startDate->month(), dayStart);
       endDate->setDate(endDate->year(), endDate->month(), dayEnd);
   }
   else if (periodSelected == "Monthly")
   {
       startDate->setDate(startDate->year(), startDate->month(), 1);
       endDate->setDate(endDate->year(), endDate->month(), endDate->daysInMonth());
   }
   else if (periodSelected == "Seasonal")
   {
       int mySeason = getSeasonFromDate(myDate);
       if ( (myDate.month() == 12) || (myDate.month() == 1) || (myDate.month() == 2))
       {
           startDate->setDate( (startDate->year() - 1), 12, 1);
           QDate temp(endDate->year(), 2, 1);
           endDate->setDate(endDate->year(), 2, temp.daysInMonth());
       }
       else
       {
           startDate->setDate(startDate->year(), mySeason*3, 1);
           QDate temp(endDate->year(), mySeason*3+2, 1);
           endDate->setDate(endDate->year(), mySeason*3+2, temp.daysInMonth());
       }

   }
   else if (periodSelected == "Annual")
   {
       startDate->setDate(startDate->year(), 1, 1);
       endDate->setDate(endDate->year(), 12, 31);
   }
   else
   {
       return false;
   }
   return true;

}


