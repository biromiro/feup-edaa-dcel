#ifndef DCEL_GEOGRAPHICPOINT_H
#define DCEL_GEOGRAPHICPOINT_H


#include <ostream>

class GeographicPoint {
public:
    GeographicPoint(double lat_, double long_);

    friend std::ostream &operator<<(std::ostream &os, const GeographicPoint &point);

private:

    double latitude;
    double longitude;
};


#endif //DCEL_GEOGRAPHICPOINT_H
