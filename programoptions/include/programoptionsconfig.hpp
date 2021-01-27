#pragma once

// library headers
#include "configuration.hpp"
#include <boost/program_options.hpp>

/** An implementation of Configuration that reads the parameters using the
 * library Boost.Program_options
 */
class BoostConfig:
        public Configuration
{
    /// number of iterations to execute
    int m_nb_iter;

    /// shape of the global data field
    Coord2D m_global_shape;

    /// shape of the data distribution
    Coord2D m_dist_extents;

    /// time difference between two consecutive points
    double m_delta_t;

    /// space difference between two consecutive points
    std::array<double, 2> m_delta_space;

public:
    /** Construct a new B
     * @param argc the number of command-line arguments
     * @param argv the values of command-line arguments
     */
    BoostConfig(const int argc, const char* const argv[]);

    // see overridden function
    int nb_iter() const override { return m_nb_iter; }

    // see overridden function
    Coord2D global_shape() const override { return m_global_shape; }

    // see overridden function
    Coord2D dist_extents() const override { return m_dist_extents; }

    // see overridden function
    double delta_t() const override { return m_delta_t; }

    // see overridden function
    std::array<double, 2> delta_space() const override { return m_delta_space; }

};
