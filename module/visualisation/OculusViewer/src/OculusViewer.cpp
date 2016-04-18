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
			//reset center position
			if(join.name.compare("robot1") <= 1 || join.name.compare("nubotsvm") == 0){
				recenter = true;
			}
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
			
			worldState->cam_to_feet.m[0] = fragment.cam_to_feet().x().x();
			worldState->cam_to_feet.m[1] = fragment.cam_to_feet().x().y();
			worldState->cam_to_feet.m[2] = fragment.cam_to_feet().x().z();
			worldState->cam_to_feet.m[3] = fragment.cam_to_feet().x().t();

			worldState->cam_to_feet.m[4] = fragment.cam_to_feet().y().x();
			worldState->cam_to_feet.m[5] = fragment.cam_to_feet().y().y();
			worldState->cam_to_feet.m[6] = fragment.cam_to_feet().y().z();
			worldState->cam_to_feet.m[7] = fragment.cam_to_feet().y().t();

			worldState->cam_to_feet.m[8] = fragment.cam_to_feet().z().x();
			worldState->cam_to_feet.m[9] = fragment.cam_to_feet().z().y();
			worldState->cam_to_feet.m[10] = fragment.cam_to_feet().z().z();
			worldState->cam_to_feet.m[11] = fragment.cam_to_feet().z().t();

			worldState->cam_to_feet.m[12] = fragment.cam_to_feet().t().x();
			worldState->cam_to_feet.m[13] = fragment.cam_to_feet().t().y();
			worldState->cam_to_feet.m[14] = fragment.cam_to_feet().t().z();
			worldState->cam_to_feet.m[15] = fragment.cam_to_feet().t().t();
			
			emit(worldState);
		});

		//Main render loop
		on<Always, Optional<With<WorldState>>>().then([this] (const std::shared_ptr<const WorldState> worldState) {
			//TODO: get head pose!!
			auto now = NUClear::clock::now();
			float time_elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1e6;
			GL::Mat4 userState;
			if(recenter){
				renderer.recenter();
				recenter = false;
			}
			//TODO: rearchitect so oculus manager is not inside renderer
			if (!renderer.render(time_elapsed_seconds, worldState, userState)) {
				powerplant.shutdown();
			} else {
				//Emit current head pose
				auto state = std::make_unique<PresenceUserState>();
				state->mutable_head_pose()->mutable_x()->set_x(userState.m[0]);
				state->mutable_head_pose()->mutable_x()->set_y(userState.m[1]);
				state->mutable_head_pose()->mutable_x()->set_z(userState.m[2]);
				state->mutable_head_pose()->mutable_x()->set_t(userState.m[3]);

				state->mutable_head_pose()->mutable_y()->set_x(userState.m[4]);
				state->mutable_head_pose()->mutable_y()->set_y(userState.m[5]);
				state->mutable_head_pose()->mutable_y()->set_z(userState.m[6]);
				state->mutable_head_pose()->mutable_y()->set_t(userState.m[7]);

				state->mutable_head_pose()->mutable_z()->set_x(userState.m[8]);
				state->mutable_head_pose()->mutable_z()->set_y(userState.m[9]);
				state->mutable_head_pose()->mutable_z()->set_z(userState.m[10]);
				state->mutable_head_pose()->mutable_z()->set_t(userState.m[11]);

				state->mutable_head_pose()->mutable_t()->set_x(userState.m[12]);
				state->mutable_head_pose()->mutable_t()->set_y(userState.m[13]);
				state->mutable_head_pose()->mutable_t()->set_z(userState.m[14]);
				state->mutable_head_pose()->mutable_t()->set_t(userState.m[15]);

				emit<Scope::NETWORK>(state);
			}
		});

    }
}
}
