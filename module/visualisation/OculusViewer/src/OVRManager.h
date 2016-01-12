
#include <OVR_CAPI_GL.h>
#include "Extras/OVR_Math.h"
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
    OVRManager();
	~OVRManager();

	void OVRManager::init();

	std::vector<EyePose> getCurrentPoses();
	ovrSizei getResolution();
	ovrSizei getMirrorResolution();
    
private:
	bool riftPresent;
    ovrSession session;
	ovrGraphicsLuid luid;
	ovrHmdDesc hmdDesc;
	ovrEyeRenderDesc EyeRenderDesc[2];

	std::vector<std::unique_ptr<GL::Framebuffer>> eyeBuffers;
	std::unique_ptr<GL::Framebuffer> mirrorBuffer;

};

#endif