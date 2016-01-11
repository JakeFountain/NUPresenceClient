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

#ifndef MODULES_VISUALISATION_OCULUSVIEWER_H
#define MODULES_VISUALISATION_OCULUSVIEWER_H

#include <nuclear>

#include "Renderer.h"

namespace module {
namespace visualisation {

    class OculusViewer : public NUClear::Reactor {

    public:
        /// @brief Called by the powerplant to build and setup the OculusViewer reactor.
        explicit OculusViewer(std::unique_ptr<NUClear::Environment> environment); 

		Renderer renderer;
    };

}
}

#endif  // MODULES_VISUALISATION_OCULUSVIEWER_H
