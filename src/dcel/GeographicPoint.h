// Copyright (c) 2023 EDAA Group C. All Rights Reserved.
#ifndef DCEL_GEOGRAPHICPOINT_H
#define DCEL_GEOGRAPHICPOINT_H

#include <ostream>

class GeographicPoint {
public:
  GeographicPoint(double lat_, double long_);

  GeographicPoint();

  friend std::ostream &operator<<(std::ostream &os,
                                  const GeographicPoint &point);

  bool operator==(const GeographicPoint &rhs) const;

  bool operator!=(const GeographicPoint &rhs) const;

  bool operator<(const GeographicPoint &rhs) const;

  bool operator>(const GeographicPoint &rhs) const;

  bool operator<=(const GeographicPoint &rhs) const;

  double getLatitude() const;

  double getLongitude() const;

  bool operator>=(const GeographicPoint &rhs) const;

 private:
  double latitude;
  double longitude;
};

#endif  // DCEL_GEOGRAPHICPOINT_H
