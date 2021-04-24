/*////////////////////////////////////////////////////////////////////////////
// This Program is Salat Time Calculator with timezone,DST(Date Saving Time)//
// longitude and latitude of your location and also time.                   //
// For muslims, this is very important to know about Salat Times, So I built//
// This Code For free for all of muslims and whom they want to know salat   //
// Times.                                                                   //
//																			//
// Written By : S.S.Alavi													//
////////////////////////////////////////////////////////////////////////////*/



#include <iostream>
#include <math.h>
// Time Structure for input and output times
typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	
}Time;
// Salat Time Structure for final result
typedef struct {
	Time fajr;
	Time sunrise;
	Time dhuhr;
	Time sunset;
	Time maghrib;
	Time midnight;
}SalatTimes;	
// Converts Deg. To Rad.
double dtr(double d)
{
	return (d * 3.1415926535) / 180.0; 
}
// Converts Rad. To Deg.
double rtd(double r) 
{ 
	return (r * 180.0) / 3.1415926535; 
}
// triangles functions in Deg.
double sindeg(double d) { return sin(dtr(d)); }
double cosdeg(double d) { return cos(dtr(d)); }
double tandeg(double d) { return tan(dtr(d)); }

double arcsindeg(double d) { return rtd(asin(d)); }
double arccosdeg(double d) { return rtd(acos(d)); }
double arctandeg(double d) { return rtd(atan(d)); }

double arccotdeg(double x) { return rtd(atan(1.0/x)); }
double arctan2deg(double y,double x) { return rtd(atan2(y, x)); }
// for resolve negetive and out-of-range valuse
double fix(double a,double b) { 
	a = a- b* ((int)(a/ b));
	return (a < 0) ? a+ b : a;
}

double fixAngle(double a) { return fix(a, 360); }
double fixHour(double a) { return fix(a, 24 ); }

// Calculator of Julian Date. Note: Every Salat time must be calculated 
// from julian date.

double juliandate(int year,int month , int day)
{
	if (month <= 2) {
		year -= 1;
		month += 12;
	};
	int A = year/ 100;
	int B = 2 - A + (A/ 4);

	double JD = ((int)(365.25* (year+ 4716)))+ ((int)(30.6001* (month+ 1)))+ day+ B- 1524.5;
	return JD;
	
}

double sunPosition(double jd,double *dec1) {
	double D = jd - 2451545.0;
	double g = fixAngle(357.529 + 0.98560028* D);
	double q = fixAngle(280.459 + 0.98564736* D);
	double L = fixAngle(q + 1.915* sindeg(g) + 0.020* sindeg(2*g));

	double R = 1.00014 - 0.01671* cosdeg(g) - 0.00014* cosdeg(2*g);
	double e = 23.439 - 0.00000036* D;

	double RA = arctan2deg(cosdeg(e)* sindeg(L), cosdeg(L))/ 15.0;
	double eqt = q/15.0 - fixHour(RA);
	*dec1 = arcsindeg(sindeg(e) * sindeg(L)) ;

	return eqt;
}

// Dhuhr
double midDay(double jd,double timezone,double lng,double dst,double *lec)
{
	return fixHour(12+timezone+dst-(lng/15.0)-sunPosition(jd,lec));
}

double T(double alpha,double Lat,double D)
{
	double t=0;
	t = 0-sindeg(alpha);
	t -= sindeg(Lat)*sindeg(D);
	t /= cosdeg(Lat)*cosdeg(D);
	t = arccosdeg(t);
	t /= 15;
	return t;
}
// Helping function for make Time
Time makeTime(int year,int month,int day,int hour,int minute, int second)
{
	Time t;
	t.year = year;
	t.month = month;
	t.day = day;
	t.hour = hour;
	t.minute = minute;
	t.second = second;
	return t;
}
// Toloo'e
Time sunRise(double longitude,double latitude,Time t,double timezone,double DST)
{
	double lec,jul=juliandate((t.year < 100)?(t.year+2000):t.year,t.month,t.day);
	double midday = midDay(jul,timezone,longitude,DST,&lec);
	double rise = midday - T(0.833,latitude,lec);
	rise = fixHour(rise);
	int h = (int) rise;
	int m = (int) ((rise - h)*60);
	return makeTime(0,0,0,h,m,0);
}
// Qoroob
Time sunSet(double longitude,double latitude,Time t,double timezone,double DST)
{
	double lec,jul=juliandate((t.year < 100)?(t.year+2000):t.year,t.month,t.day);
	double midday = midDay(jul,timezone,longitude,DST,&lec);
	double rise = midday + T(0.833,latitude,lec);
	rise = fixHour(rise);
	int h = (int) rise;
	int m = (int) ((rise - h)*60);
	return makeTime(0,0,0,h,m,0);
}
// Fajr(Sobh)
Time Fajr(double longitude,double latitude,Time t,double timezone,double DST)
{
	double lec,jul=juliandate((t.year < 100)?(t.year+2000):t.year,t.month,t.day);
	double midday = midDay(jul,timezone,longitude,DST,&lec);
	double rise = midday - T(17.7,latitude,lec);
	rise = fixHour(rise);
	int h = (int) rise;
	int m = (int) ((rise - h)*60);
	return makeTime(0,0,0,h,m,0);
	
}
Time Maghrib(double longitude,double latitude,Time t,double timezone,double DST)
{
	double lec,jul=juliandate((t.year < 100)?(t.year+2000):t.year,t.month,t.day);
	double midday = midDay(jul,timezone,longitude,DST,&lec);
	double rise = midday + T(4,latitude,lec);
	rise = fixHour(rise);
	int h = (int) rise;
	int m = (int) ((rise - h)*60);
	return makeTime(0,0,0,h,m,0);
	
}
// Nime Shab Shar'ee
Time Midnight(double longitude,double latitude,Time t,double timezone,double DST)
{
	double m1 = Fajr(longitude,latitude,t,timezone,DST).hour + Fajr(longitude,latitude,t,timezone,DST).minute / 60.0;
	double m2 = sunSet(longitude,latitude,t,timezone,DST).hour + sunSet(longitude,latitude,t,timezone,DST).minute / 60.0;	 
	double m3 = m1 + m2;
	m3 += 24;
	m3 /= 2.0;
	int h = (int) m3;
	int m = (int) ((m3 - h)*60);
	return makeTime(0,0,0,(int)h,(int)m,0);
}
// Do Full calculation and return salat times 
SalatTimes calculate(double longitude,double latitude,Time t,double timezone,double DST)
{
	SalatTimes salattime;
	double lec,jul=juliandate((t.year < 100)?(t.year+2000):t.year,t.month,t.day);
	double midday= midDay(jul,timezone,longitude,DST,&lec);
	midday = fixHour(midday);
	int h = (int)midday;
	int m = (int)((midday - h)*60);
	salattime.dhuhr = makeTime(0,0,0,h,m,0);
	salattime.fajr = Fajr(longitude,latitude,t,timezone,DST);
	salattime.maghrib = Maghrib(longitude,latitude,t,timezone,DST);
	salattime.sunrise = sunRise(longitude,latitude,t,timezone,DST);
	salattime.sunset = sunSet(longitude,latitude,t,timezone,DST);
	salattime.midnight = Midnight(longitude,latitude,t,timezone,DST);
	return salattime;
}
// Example of using this code In Tehran
int main(int argc, char** argv) {
	Time t = makeTime(2016,11,24,0,0,0);
	SalatTimes S = calculate(53.25,37.6856,t,3.5,0);
	t = S.dhuhr;
	printf("%d:%d",t.hour,t.minute);
	return 0;
}
