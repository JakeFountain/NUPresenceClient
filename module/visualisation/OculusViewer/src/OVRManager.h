
#include <OVR_CAPI_GL.h>
#include "Extras/OVR_Math.h"
#include <GL\GL\Extensions.hpp>
#include <GL\OOGL.hpp>
#include <iostream>
#include <memory>
#include "TextureToScreen.h"

#ifndef MODULES_VISUALISATION_NUPRESENCE_OVR_MANAGER
#define MODULES_VISUALISATION_NUPRESENCE_OVR_MANAGER


namespace module {
namespace visualisation {

	struct EyePose{
		GL::Mat4 view;
		GL::Mat4 proj;
	};

	class OVRManager {
	public:
		enum RenderTarget {
			LEFT_EYE = 0,
			RIGHT_EYE = 1,
			MIRROR = 2
		};

	    OVRManager();
		GLuint getLastEyeTexture();
		~OVRManager();

		bool init();

		std::vector<EyePose> getCurrentPoses();
		bool renderToRift();
		ovrSizei getResolution();
		ovrSizei getMirrorResolution();

		bool createEyeBuffers();

		void setRenderTarget(GL::Context & gl, OVRManager::RenderTarget target);

	private:
		bool riftPresent;
	    ovrSession session;
		ovrGraphicsLuid luid;
		ovrHmdDesc hmdDesc;
		ovrEyeRenderDesc EyeRenderDesc[2];
	 	ovrVector3f ViewOffset[2];
	 	ovrPosef EyeRenderPose[2];
		OVR::Sizei bufferSize;

	 	ovrSwapTextureSet* pTextureSet;
		int lastEyeTextureIndex;

		std::vector<std::unique_ptr<GL::Framebuffer>> eyeBuffer;
		std::unique_ptr<GL::Framebuffer> mirrorBuffer;

	};

}
}

#endif