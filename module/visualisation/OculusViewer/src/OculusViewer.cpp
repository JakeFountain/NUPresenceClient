/*
 * This file is part of NUbots Codebase.
 *
 * The NUbots Codebase is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The NUbots Codebase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the NUbots Codebase.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 NUbots <nubots@nubots.net>
*/

#include "OculusViewer.h"
#include <oglplus/all.hpp>

// #include "extension/Configuration.h"

namespace module {
namespace visualisation {

    // using extension::Configuration;

    OculusViewer::OculusViewer(std::unique_ptr<NUClear::Environment> environment)
    : Reactor(std::move(environment)) {
		start = NUClear::clock::now();

        // on<Configuration>("OculusViewer.yaml").then([this] (const Configuration& config) {
        //     // Use configuration here from file OculusViewer.yaml
        // });

		on<Always>().then([this] {
			auto now = NUClear::clock::now();
			float time_elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1e6;
            //try{
                bool running = renderer.render(time_elapsed_seconds);
                if(!running) powerplant.shutdown();
            //} catch(oglplus::Error& err) {
           // }
		});

    }
}
}
