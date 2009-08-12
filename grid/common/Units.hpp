//===========================================================================
//
// File: Units.hpp
//
// Created: Thu Jul  2 09:19:08 2009
//
// Author(s): Halvor M Nilsen <hnil@sintef.no>
//
// $Date$
//
// $Revision$
//
//===========================================================================

/*
  Copyright 2009 SINTEF ICT, Applied Mathematics.
  Copyright 2009 Statoil ASA.

  This file is part of The Open Reservoir Simulator Project (OpenRS).

  OpenRS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenRS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenRS.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPENRS_UNITS_HEADER
#define OPENRS_UNITS_HEADER

#include <algorithm>

#include <boost/bind.hpp>

namespace Dune
{
    namespace prefix
    {
        const double micro = 1.0e-6;
        const double milli = 1.0e-3;
        const double centi = 1.0e-2;
        const double deci  = 1.0e-1;
        const double kilo  = 1.0e3;
        const double mega  = 1.0e6;
        const double giga  = 1.0e9;
    } // namespace prefix

    namespace unit
    {
        // Common powers
        template<typename T>
        inline T square(const T& v) { return v * v;     }

        template<typename T>
        inline T cubic (const T& v) { return v * v * v; }

        // --------------------------------------------------------------
        // Basic (fundamental) units and conversions
        // --------------------------------------------------------------

        // Length:
        const double meter =  1;
        const double inch  =  2.54 * prefix::centi*meter;
        const double feet  = 12    * inch;

        // Time:
        const double second =   1;
        const double minute =  60 * second;
        const double hour   =  60 * minute;
        const double day    =  24 * hour;
        const double year   = 365 * day;

        // Mass:
        const double kilogram = 1;

        // http://en.wikipedia.org/wiki/Pound_(mass)#Avoirdupois_pound
        const double pound    = 0.45359237 * kilogram;

        // --------------------------------------------------------------
        // Standardised constants
        // --------------------------------------------------------------

        const double gravity = 9.80665 * meter/square(second);

        // --------------------------------------------------------------
        // Derived units and conversions
        // --------------------------------------------------------------

        // Force:
        const double Newton = kilogram*meter / square(second); // == 1
        const double lbf    = pound * gravity; // Pound-force

        // Pressure:
        const double Pascal = Newton / square(meter); // == 1
        const double barsa  = 100000 * Pascal;
        const double atm    = 101325 * Pascal;
        const double psia   = lbf / square(inch);

        // Viscosity:
        const double Pas   = Pascal * second; // == 1
        const double Poise = prefix::deci*Pas;

        // Permeability:
        //
        // A porous medium with a permeability of 1 darcy permits a
        // flow (flux) of 1 cm�/s of a fluid with viscosity 1 cP (1
        // mPa�s) under a pressure gradient of 1 atm/cm acting across
        // an area of 1 cm�.
        //
        namespace {
            const double p_grad   = atm / (prefix::centi*meter);
            const double area     = square(prefix::centi*meter);
            const double flux     = cubic (prefix::centi*meter) / second;
            const double velocity = flux / area;
            const double visc     = prefix::centi*Poise;
        }

        const double darcy = (velocity * visc) / p_grad;
        //                 == 1e-7 [m^2] / 101325
        //                 == 9.869232667160130e-13 [m^2]

        // Unit conversion support.
        //
        // Note: Under the penalty of treason will you be
        //
        //    using namespace Dune::unit::convert;
        //
        // I mean it!
        //
        namespace convert {
            // Convert from external units of measurements to equivalent
            // internal units of measurements.  Note: The internal units
            // of measurements are *ALWAYS*, and exclusively, SI.
            //
            // Example: Convert a std::vector<double> kx, containing
            // permeability values in units of milli-darcy (mD) to the
            // equivalent values in m^2.
            //
            //    using namespace Dune::unit;
            //    using namespace Dune::prefix;
            //    convert::from(kx, milli*darcy);
            //
            template<typename T>
            inline T from(const T q, const T unit)
            {
                return q * unit;
            }

            // Convert from internal units of measurements to equivalent
            // external units of measurements.  Note: The internal units
            // of measurements are *ALWAYS*, and exclusively, SI.
            //
            // Example: Convert a std::vector<double> p, containing
            // pressure values in Pascal (i.e., unit::Pascal) to the
            // equivalent values in Psi (unit::psia).
            //
            //    using namespace Dune::unit;
            //    std::transform(p.begin(), p.end(), p.begin(),
            //                   boost::bind(convert::to<double>, _1, psia));
            //
            template<typename T>
            inline T to(const T q, const T unit)
            {
                return q / unit;
            }
        } // namespace convert
    } // namespace unit

    namespace units {
        //     const double MILLIDARCY = 1.0;//9.86923e-16;
        //     const double VISCOSITY_UNIT = 1.0;//1e-3;
        //     const double DAYS2SECONDS = 1.0;//86400;
        const double MILLIDARCY = 9.86923e-16;
        const double VISCOSITY_UNIT = 1e-3;
        const double DAYS2SECONDS = 86400;
        const double FEET = 0.30479999798832;
        const double WELL_INDEX_UNIT = VISCOSITY_UNIT/(DAYS2SECONDS*1e5);
    } // namespace units
} // namespace Dune
#endif // OPENRS_UNITS_HEADER
