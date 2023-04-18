//
// Created by nrtc on 4/17/2023.
//

#include "GeographicPoint.h"

GeographicPoint::GeographicPoint(double lat_, double long_) : latitude(lat_), longitude(long_) {}

std::ostream &operator<<(std::ostream &os, const GeographicPoint &point) {
    os << "latitude: " << point.latitude << " longitude: " << point.longitude;
    return os;
}
