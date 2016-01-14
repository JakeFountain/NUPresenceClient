
#include <OVR_CAPI_GL.h>
#include "Extras/OVR_Math.h"
#include <GL\GL\Extensions.hpp>
#include <GL\OOGL.hpp>
#include <iostream>
#include <memory>

#ifndef NUPRESENCE_OVR_MANAGER
#define NUPRESENCE_OVR_MANAGER


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

	std::vector<std::unique_ptr<GL::Framebuffer>> eyeBuffer;
	std::unique_ptr<GL::Framebuffer> mirrorBuffer;


};

#endif