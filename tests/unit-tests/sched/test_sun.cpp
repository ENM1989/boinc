// This file is part of BOINC.
// https://boinc.berkeley.edu
// Copyright (C) 2025 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

#include "gtest/gtest.h"
#include "sched_sun.h"

namespace test_sun {
    class test_sun : public ::testing::Test {};

    TEST_F(test_sun, sun_rise_set) {
        double sunrise, sunset;
        // Test data for New York, NY on September 28, 2025
        // Expected sunrise: ~10:45 UTC (10.75 hours)
        // Expected sunset:  ~22:30 UTC (22.5 hours)
        sun_rise_set(2025, 9, 28, 40.7128, -74.0060, sunrise, sunset);

        ASSERT_NEAR(sunrise, 10.75, 0.5);
        ASSERT_NEAR(sunset, 22.5, 0.5);
    }
}