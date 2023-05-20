// Copyright (c) 2023 EDAA Group C. All Rights Reserved.
#include <pybind11/pybind11.h>

#include <memory>
#include <string>

#include "./GeographicPoint.h"
#include "./Parser.h"
#include "./dcel.hpp"

// Parsejson to dcel
// MapOverlay overlay dcel
// methods to handle the structure

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(dcel_binds, m) {
  m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

  py::class_<DCEL<GeographicPoint>>(m, "DCEL");

  py::class_<Parser>(m, "Parser")
      .def_static("parseJSONtoDCEL", &Parser::parseJSONtoDCEL);

  m.attr("__version__") = "0.0.1";
}
