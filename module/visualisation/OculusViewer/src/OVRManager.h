
// #include <OVR_CAPI_GL.h>
// #include "Extras/OVR_Math.h"
// #include <iostream>
// #include <memory>
// #include "TextureToScreen.h"

// #ifndef NUPRESENCE_OVR_MANAGER
// #define NUPRESENCE_OVR_MANAGER


// struct EyePose{
// 	oglplus::Mat4 view;
// 	oglplus::Mat4 proj;
// };

// class OVRManager {
// public:
// 	enum RenderTarget {
// 		LEFT_EYE = 0,
// 		RIGHT_EYE = 1,
// 		MIRROR = 2
// 	};

//     OVRManager();
// 	~OVRManager();

// 	bool init();

// 	std::vector<EyePose> getCurrentPoses();
// 	bool renderToRift();
// 	void drawMirror(oglplus::Context & gl);
// 	ovrSizei getResolution();
// 	ovrSizei getMirrorResolution();

// 	bool createEyeBuffers();

// 	void setRenderTarget(oglplus::Context & gl, OVRManager::RenderTarget target);

// private:
// 	bool riftPresent;
//     ovrSession session;
// 	ovrGraphicsLuid luid;
// 	ovrHmdDesc hmdDesc;
// 	ovrEyeRenderDesc EyeRenderDesc[2];
//  	ovrVector3f ViewOffset[2];
//  	ovrPosef EyeRenderPose[2];
// 	OVR::Sizei bufferSize;

//  	ovrSwapTextureSet* pTextureSet;

// 	std::vector<std::unique_ptr<oglplus::Framebuffer>> eyeBuffer;
// 	std::unique_ptr<oglplus::Framebuffer> mirrorBuffer;

// 	std::unique_ptr<TextureToScreen> texToScreenRenderer;
// };

// #endif