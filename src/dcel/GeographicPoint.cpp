//
// Created by nrtc on 4/17/2023.
//

#include "GeographicPoint.h"

GeographicPoint::GeographicPoint(double lat_, double long_) : latitude(lat_), longitude(long_) {}

std::ostream &operator<<(std::ostream &os, const GeographicPoint &point) {
    os << "latitude: " << point.latitude << " longitude: " << point.longitude;
    return os;
}

bool GeographicPoint::operator==(const GeographicPoint &rhs) const {
    return latitude == rhs.latitude &&
           longitude == rhs.longitude;
}

bool GeographicPoint::operator!=(const GeographicPoint &rhs) const {
    return !(rhs == *this);
}

bool GeographicPoint::operator<(const GeographicPoint &rhs) const {
    if (latitude < rhs.latitude)
        return true;
    if (rhs.latitude < latitude)
        return false;
    return longitude < rhs.longitude;
}

bool GeographicPoint::operator>(const GeographicPoint &rhs) const {
    return rhs < *this;
}

bool GeographicPoint::operator<=(const GeographicPoint &rhs) const {
    return !(rhs < *this);
}

bool GeographicPoint::operator>=(const GeographicPoint &rhs) const {
    return !(*this < rhs);
}

double GeographicPoint::getLatitude() const {
    return latitude;
}

double GeographicPoint::getLongitude() const {
    return longitude;
}
