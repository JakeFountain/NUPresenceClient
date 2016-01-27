
#ifndef MODULE_VISUALISATION_WORLD_STATE_H
#define MODULE_VISUALISATION_WORLD_STATE_H

namespace module {
	namespace visualisation {

		struct WorldState {
			struct Image {
				std::vector<char> data;
				int width;
				int	height;
			};

			Image latestImage;
		};

	}
}

#endif