// Flight Math
#include "flightmath.h"
#include <math.h>



double compute_bearing_deg(struct _COORD_ dest, struct _COORD_ org)
{
	double dlon = (dest.lon-org.lon)*DEG2RAD;
	double lat1 = org.lat*DEG2RAD;
	double lon1 = org.lon*DEG2RAD;
	double lat2 = dest.lat*DEG2RAD;
	double lon2 = dest.lon*DEG2RAD;
	
	double y = sin(dlon) * cos(lat2);
	double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon);
	double bearing = atan2(y, x)*RAD2DEG;

	if(bearing < 0.0)
		bearing += 360.0;
	return bearing;
}



// http://www.movable-type.co.uk/scripts/latlong.html
// computes distance between 2 points on a speriod using the haversine formula
double compute_distance_haversine(struct _COORD_ pos1, struct _COORD_ pos2)
{
	double lat1 = pos1.lat*DEG2RAD;
	double lat2 = pos2.lat*DEG2RAD;
	double lon1 = pos1.lon*DEG2RAD;
	double lon2 = pos2.lon*DEG2RAD;
	double EARTH_RADIUS = geocentric_radius_m(pos1.lat);
	
	//JFK to LAX
/*	lat1=0.592539;
	lon1=2.066470;
	lat2=0.709186;
	lon2=1.287762;
*/	


	double dLat = lat2-lat1;
	double dLon = lon2-lon1;

	double a = sin(0.5*dLat) * sin(0.5*dLat) + sin(0.5*dLon) * sin(0.5*dLon) * cos(lat1) * cos(lat2); 
	double c = 2.0 * atan2(sqrt(a), sqrt(1.0-a)); 
	double distance = EARTH_RADIUS * c;

	return distance;  // km
}

// computes distance between 2 points on a speriod based on spherical trigonometry
// return value in km
double compute_distance_spherical_law_of_cosine(struct _COORD_ pos1, struct _COORD_ pos2)
{
	double lat1 = pos1.lat*DEG2RAD;
	double lat2 = pos2.lat*DEG2RAD;
	double lon1 = pos1.lon*DEG2RAD;
	double lon2 = pos2.lon*DEG2RAD;
	double EARTH_RADIUS = geocentric_radius_m(pos1.lat);
	
	// JFK to LAX
/*	lat1=0.592539;
	lon1=2.066470;
	lat2=0.709186;
	lon2=1.287762;
*/	

	double distance = acos(sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon2-lon1)) * EARTH_RADIUS;
	
	return distance;
}



// http://www.movable-type.co.uk/scripts/latlong-vincenty-direct.html
// This function finds location of the destination point
// given a start point, an initial bearing, and a distance travelled
struct _COORD_ compute_coord_vincenty(struct _COORD_ pos, double brng, double dist)
{
	struct _COORD_ retpos;	
	double a = 6378137.0, b = 6356752.3142,  f = 1.0/298.257223563;  // WGS-84 ellipsiod
	double s = dist;
	double alpha1 = brng*pi/180.0;
	double sinAlpha1 = sin(alpha1);
	double cosAlpha1 = cos(alpha1);

	double tanU1 = (1.0-f) * tan(pos.lat*pi/180.0);
	double cosU1 = 1.0 / sqrt((1.0 + tanU1*tanU1)), sinU1 = tanU1*cosU1;
	double sigma1 = atan2(tanU1, cosAlpha1);
	double sinAlpha = cosU1 * sinAlpha1;
	double cosSqAlpha = 1.0 - sinAlpha*sinAlpha;
	double uSq = cosSqAlpha * (a*a - b*b) / (b*b);
	double A = 1.0 + uSq/16384.0*(4096.0+uSq*(-768.0+uSq*(320.0-175.0*uSq)));
	double B = uSq/1024.0 * (256.0+uSq*(-128.0+uSq*(74.0-47.0*uSq)));

	double sigma = s / (b*A), sigmaP = twopi;


	double  sinSigma;
	double  cos2SigmaM;
	double  cosSigma;
	double  deltaSigma;
	while (fabs(sigma-sigmaP) > 1e-12)
	{
		cos2SigmaM = cos(2.0*sigma1 + sigma);
		sinSigma = sin(sigma);
		cosSigma = cos(sigma);
		deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1.0+2.0*cos2SigmaM*cos2SigmaM)-
			B/6*cos2SigmaM*(-3.0+4.0*sinSigma*sinSigma)*(-3.0+4.0*cos2SigmaM*cos2SigmaM)));
		sigmaP = sigma;
		sigma = s / (b*A) + deltaSigma;
	}

	double  tmp = sinU1*sinSigma - cosU1*cosSigma*cosAlpha1;
	double  lat2 = atan2(sinU1*cosSigma + cosU1*sinSigma*cosAlpha1, 
		(1-f)*sqrt(sinAlpha*sinAlpha + tmp*tmp));
	double  lambda = atan2(sinSigma*sinAlpha1, cosU1*cosSigma - sinU1*sinSigma*cosAlpha1);
	double  C = f/16.0*cosSqAlpha*(4.0+f*(4.0-3.0*cosSqAlpha));
	double  L = lambda - (1.0-C) * f * sinAlpha *
		(sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1.0+2.0*cos2SigmaM*cos2SigmaM)));

	double  lon2 = pos.lon*pi/180.0+L+3.0*pi;
	while(lon2 > twopi)
		lon2-= twopi;
	lon2-=pi;
	
	retpos.lon = lon2*RAD2DEG;
	retpos.lat = lat2*RAD2DEG;

  //double  revAz = atan2(sinAlpha, -tmp);  // final bearing, if required
  //return { lat: lat2.toDeg(), lon: lon2.toDeg(), finalBearing: revAz.toDeg() };
	return retpos;
}


// finds location of the destination point given a start point, an initial bearing, 
// and a distance travelled based on spherical trigonometry
struct _COORD_ compute_coord_per_bearing_and_distance(struct _COORD_ pos, double brng, double dist)
{
	struct _COORD_ retpos;
	double x1 = pos.lon * DEG2RAD;
	double y1 = pos.lat * DEG2RAD;
	double bearing = brng * DEG2RAD;
	double EARTH_RADIUS = geocentric_radius_m(pos.lat);

// Convert arc distance to radians
	double c = dist / EARTH_RADIUS;

	retpos.lat = asin( sin(y1) * cos(c) + cos(y1) * sin(c) * cos(bearing)) * RAD2DEG;


  double a = sin(c) * sin(bearing);
  double b = cos(y1) * cos(c) - sin(y1) * sin(c) * cos(bearing);

  if( b == 0 ) 
	  retpos.lon = x1;
  else
	retpos.lon = (x1 + atan(a/b)) * RAD2DEG;

  return retpos;
}


// http://en.wikipedia.org/wiki/Rate_one_turn
// computes turn radius of an aircraft given true speed and rate of turn
double turn_radius(double TAS, double ROT)
{
	// ROT is in muliple of a 2-minute turn (rate one turn) = 180 degrees/minute = 3 degrees/sec
	// if TAS is in knot, the turn radius is in NM ( 1 NM = 1852 m)
	// if TAS is in m/s, the turn radius is in m

	return TAS/(ROT*188.49555921538759430775860299677);
}


// Based on Napier's pentagon
// Find the angle given 2 sides of a right spherical triangle
// http://en.wikipedia.org/wiki/Spherical_trigonometry#Napier.27s_Pentagon
//
// we are working in radian here!!
// A and C are sides in radian
//
double angle_from_sides_spherical(double a, double C)
{
	//return halfpi - asin( tan(a)*tan(halfpi - C));
	return acos( tan(a)/tan(C));
}

// http://en.wikipedia.org/wiki/Belt_problem
// finds the angle of an inner tangent line between two circles
// the angle is with respect to the line through centers of both circles
double inner_tangent_angle(double r1,double r2,double dist)
{
	if(r1+r2 <= dist) // general case
		return acos( (r1+r2)/dist)*RAD2DEG;
	else  // two circles overlap
		return -1.0;
}

// same as the previous function but use spherical trig and assume r1 and r2 are equal
double inner_tangent_angle_spherical(double r,double dist)
{
	if(dist < 2.0*r)
		return -1.0;
	else
		return angle_from_sides_spherical(r,0.5*dist)*RAD2DEG;  // phi
}


struct _POLAR_COOR_ compute_distance_bearing_vincenty(struct _COORD_ pos1, struct _COORD_ pos2)
{
  double a = EARTH_RADIUS_EQUITORIAL, b = EARTH_RADIUS_POLAR,  f = 1/298.257223563;  // WGS-84 ellipsoid params
  double L = (pos2.lon-pos1.lon)*DEG2RAD;
  double U1 = atan((1-f) * tan(pos1.lat*DEG2RAD));
  double U2 = atan((1-f) * tan(pos2.lat*DEG2RAD));
  double sinU1 = sin(U1), cosU1 = cos(U1);
  double sinU2 = sin(U2), cosU2 = cos(U2);
  double cosSqAlpha,cos2SigmaM,cosSigma,sigma,sinAlpha,sinSigma,sinLambda,cosLambda;
  double C;
   struct _POLAR_COOR_ ret;
  
  double lambda = L, lambdaP, iterLimit = 100;
  do {
    sinLambda = sin(lambda), cosLambda = cos(lambda);
    sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) + 
      (cosU1*sinU2-sinU1*cosU2*cosLambda) * (cosU1*sinU2-sinU1*cosU2*cosLambda));
	if (sinSigma==0)
	{
		ret.dist = 0;  // co-incident points
		ret.angle = 0;
		return ret;
	}
    cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;
    sigma = atan2(sinSigma, cosSigma);
    sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
    cosSqAlpha = 1 - sinAlpha*sinAlpha;

	if(cosSqAlpha == 0)
		cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (§6)
	else
		cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;

    C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
    lambdaP = lambda;
    lambda = L + (1-C) * f * sinAlpha *
      (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
  } while (fabs(lambda-lambdaP) > 1e-12 && --iterLimit>0);

  if (iterLimit==0)   // formula failed to converge
  {
	  ret.dist = -1;
	  ret.angle = 0;
	  return ret;
  }

  double uSq = cosSqAlpha * (a*a - b*b) / (b*b);
  double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
  double deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
    B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
  double s = b*A*(sigma-deltaSigma);
  
//  s = s.toFixed(3); // round to 1mm precision
  
  // note: to return initial/final bearings in addition to distance, use something like:
  double fwdAz = atan2(cosU2*sinLambda,  cosU1*sinU2-sinU1*cosU2*cosLambda);
  double revAz = atan2(cosU1*sinLambda, -sinU1*cosU2+cosU1*sinU2*cosLambda);
//  return { distance: s, initialBearing: fwdAz.toDeg(), finalBearing: revAz.toDeg() };

  ret.dist = s;
  ret.angle = revAz*RAD2DEG;
  if(ret.angle < 0)
	  ret.angle += 360.0;

  return ret;
}

double geocentric_radius_m(double latdeg)
{
	double a2 = EARTH_RADIUS_EQUITORIAL*EARTH_RADIUS_EQUITORIAL;
	double b2 = EARTH_RADIUS_POLAR*EARTH_RADIUS_POLAR;
	double slat = sin(latdeg*DEG2RAD);
	double clat = cos(latdeg*DEG2RAD);
	double slat2 = slat*slat;
	double clat2 = clat*clat;
	double r = sqrt((a2*a2*clat2 + b2*b2*slat2) / (a2*clat2 + b2*slat2));

	return r;
}

