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

#include "message/input/proto/ImageFragment.pb.h"
#include "message/input/proto/PresenceUserState.pb.h"

// #include "extension/Configuration.h"

namespace module {
namespace visualisation {

    // using extension::Configuration;
    using message::input::proto::ImageFragment;
    using message::input::proto::PresenceUserState;

    OculusViewer::OculusViewer(std::unique_ptr<NUClear::Environment> environment)
    : Reactor(std::move(environment)) {
		//Record start time
		start = NUClear::clock::now();

		//Setup and monitor network status
		auto networkConfig = std::make_unique<NUClear::message::NetworkConfiguration>();
		networkConfig->name = "nupresenceclient";
		networkConfig->multicastGroup = "239.226.152.162";
		networkConfig->multicastPort = 7447;

		emit<Scope::DIRECT>(networkConfig);

		on<Trigger<NUClear::message::NetworkJoin>>().then([this](const NUClear::message::NetworkJoin& join) {
			std::cout << "Connected to " << join.name << std::endl;
		});

		on<Trigger<NUClear::message::NetworkLeave>>().then([this](const NUClear::message::NetworkLeave& leave) {
			std::cout << "Disconnected from " << leave.name << std::endl;
		});

		//Process images
		on<Network<ImageFragment>>().then([this](const NetworkSource& source, const ImageFragment& fragment) {
			std::cout << "Image received from : " << source.name << std::endl;
			fragment.data().data();

			auto worldState = std::make_unique<WorldState>();
			worldState->latestImage.width = fragment.dimensions().x();
			worldState->latestImage.height = fragment.dimensions().y();
			worldState->latestImage.data.resize(fragment.data().size());
			worldState->latestImage.format = WorldState::Image::Format::YCBCR_422;
			std::memcpy(worldState->latestImage.data.data(), fragment.data().data(), fragment.data().size());

			emit(worldState);
		});

		//Main render loop
		on<Always, Optional<With<WorldState>>>().then([this] (const std::shared_ptr<const WorldState> worldState) {
			//TODO: get head pose!!
			auto now = NUClear::clock::now();
			Transform3D userState;
			float time_elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1e6;
			if (!renderer.render(time_elapsed_seconds, worldState, userState)) {
				powerplant.shutdown();
			}
		});

    }
}
}
