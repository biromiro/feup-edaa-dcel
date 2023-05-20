//
// Created by nrtc on 5/20/2023.
//

#ifndef DCEL_MAPOVERLAY_H
#define DCEL_MAPOVERLAY_H


#include "LineSweep.h"

class MapOverlay {
public:
    static std::shared_ptr<DCEL<GeographicPoint>> overlayDCELs(const std::shared_ptr<DCEL<GeographicPoint>>& dcel1,
                                                               const std::shared_ptr<DCEL<GeographicPoint>>& dcel2);

    static void cyclicOrderingOfEdges(
                GeographicPoint intersectionPoint,
                std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges
            );
};


#endif //DCEL_MAPOVERLAY_H
