/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
  simulator connection for ardupilot version of Xplane
*/

#pragma once

#include <AP_HAL/utility/Socket.h>

#include "SIM_Aircraft.h"

namespace SITL {

/*
  a Xplane simulator
 */
class XPlane : public Aircraft {
public:
    XPlane(const char *home_str, const char *frame_str);

    /* update model by one time step */
    void update(const struct sitl_input &input);

    /* static object creator */
    static Aircraft *create(const char *home_str, const char *frame_str) {
        return new XPlane(home_str, frame_str);
    }

private:
    bool receive_data(void);
    void send_data(const struct sitl_input &input);
    void select_data(uint64_t usel_mask, uint64_t sel_mask);

    const char *xplane_ip = "127.0.0.1";
    uint16_t xplane_port = 49000;
    uint16_t bind_port = 49001;
    // udp socket, input and output
    SocketAPM socket_in{true};
    SocketAPM socket_out{true};

    uint64_t time_base_us;
    uint32_t last_data_time_ms;
    Vector3f position_zero;
    Vector3f accel_earth;
    float throttle_sent = -1;
    bool connected = false;
    uint32_t xplane_frame_time;
    struct {
        uint32_t last_report_ms;
        uint32_t data_count;
        uint32_t frame_count;
    } report;

    // throttle joystick input is very weird. See comments in the main code
    const uint32_t throttle_magic = 123;
    
    // DATA@ frame types. Thanks to TauLabs xplanesimulator.h
    // (which strangely enough acknowledges APM as a source!)
    enum {
		FramRate            = 0,
		Times               = 1,
		SimStats            = 2,
		Speed               = 3,
		Gload               = 4,
		AtmosphereWeather   = 5,
		AtmosphereAircraft  = 6,
		SystemPressures     = 7,
		Joystick1           = 8,
		Joystick2           = 9,
		ArtStab             = 10,
		FlightCon           = 11,
		WingSweep           = 12,
		Trim                = 13,
		Brakes              = 14,
		AngularMoments      = 15,
        AngularVelocities   = 16,
        PitchRollHeading    = 17,
		AoA                 = 18,
        MagCompass          = 19,
        LatLonAlt           = 20,
		LocVelDistTraveled  = 21,
        ThrottleCommand     = 25,
	};
};


} // namespace SITL
