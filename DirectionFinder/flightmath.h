// Flight Math
#pragma once


#define pi				3.1415926535897932384626433832795
#define twopi			6.283185307179586476925286766559
#define halfpi			1.5707963267948966192313216916398
#define DEG2RAD			0.01745329251994329576923690768489
#define RAD2DEG			57.295779513082320876798154814105
//#define EARTH_RADIUS_km	6371.0 // km
//#define EARTH_RADIUS_m	6378137.0 // m
#define EARTH_RADIUS_EQUITORIAL	6378137.0 //m
#define EARTH_RADIUS_POLAR		6356752.314245  //m

struct _COORD_
{
	double lat;
	double lon;
};

struct _POLAR_COOR_
{
	double dist;
	double angle;
};

double compute_bearing_deg(struct _COORD_ dest, struct _COORD_ org);
double compute_distance_haversine(struct _COORD_ pos1, struct _COORD_ pos2);
double compute_distance_spherical_law_of_cosine(struct _COORD_ pos1, struct _COORD_ pos2);
struct _COORD_ compute_coord_vincenty(struct _COORD_ pos, double brng, double dist);
struct _COORD_ compute_coord_per_bearing_and_distance(struct _COORD_ pos, double brng, double dist);
double inner_tangent_angle(double r1,double r2,double dist);
double turn_radius(double TAS, double ROT);
double inner_tangent_angle_spherical(double r,double dist);
double angle_from_sides_spherical(double a, double C);
struct _POLAR_COOR_ compute_distance_bearing_vincenty(struct _COORD_ pos1, struct _COORD_ pos2);
double geocentric_radius_m(double latdeg);