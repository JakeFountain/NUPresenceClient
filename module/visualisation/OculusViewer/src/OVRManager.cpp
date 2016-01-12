

#include "OVRManager.h"
#include <algorithm>

OVRManager::OVRManager(){}

void OVRManager::init(){
	riftPresent = true;
   
   	//Init OVR
    ovrResult initResult = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(initResult)) {
		std::cout << "Failed to Init" << std::endl;
		std::cout << initResult << std::endl;
		riftPresent = false;
	}

	//Init HMD
    ovrResult result = ovr_Create(&session, &luid);
	if (!OVR_SUCCESS(result)) {
		std::cout << "Rift not found!" << std::endl;
		std::cout << result << std::endl;
		riftPresent = false;
		ovr_Shutdown();
	}

    if(riftPresent) {
    	hmdDesc = ovr_GetHmdDesc(session);
	}

	

	// Make eye render buffers
	for (int eye = 0; eye < 2; ++eye)
	{
		ovrSizei idealTextureSize = ovr_GetFovTextureSize(session, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
		
		eyeBuffers.push_back(std::make_unique<GL::Framebuffer>(idealTextureSize.w,idealTextureSize.h));

		//if (!eyeRenderTexture[eye]->TextureSet)
		//{
		//	if (retryCreate) goto Done;
		//	VALIDATE(false, "Failed to create texture.");
		//}
	}

	// Create mirror texture and an FBO used to copy mirror texture to back buffer
	ovrSizei windowSize = getMirrorResolution();
	mirrorBuffer = std::make_unique<GL::Framebuffer>(windowSize.w,windowSize.h);

	//Oculus way:
	// ovrTexture mirrorTexture;
	// result = ovr_CreateMirrorTextureGL(session, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h, reinterpret_cast<ovrTexture**>(&mirrorTexture));

	// // Configure the mirror read buffer
	// glGenFramebuffers(1, &mirrorFBO);
	// glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
	// glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
	// glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	// glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);


	EyeRenderDesc[0] = ovr_GetRenderDesc(session, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	EyeRenderDesc[1] = ovr_GetRenderDesc(session, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

	// Turn off vsync to let the compositor do its magic
	wglSwapIntervalEXT(0);

}

ovrSizei OVRManager::getResolution(){
	return hmdDesc.Resolution;
}

ovrSizei OVRManager::getMirrorResolution(){
	//Configure stereo settings
	// Setup Window and Graphics
	// Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
	return { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };
}

std::vector<EyePose> OVRManager::getCurrentPoses(){
	std::vector<EyePose> eyePoses;
	if(riftPresent){
		//Struct to emit
       	
       	// Get eye poses, feeding in correct IPD offset
        ovrVector3f               ViewOffset[2] = { EyeRenderDesc[0].HmdToEyeViewOffset,
                                                    EyeRenderDesc[1].HmdToEyeViewOffset };
        ovrPosef                  EyeRenderPose[2];

        double           ftiming = ovr_GetPredictedDisplayTime(session, 0);
        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
        double           sensorSampleTime = ovr_GetTimeInSeconds();
        ovrTrackingState hmdState = ovr_GetTrackingState(session, ftiming, ovrTrue);
        ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);


		for (int eye = 0; eye < 2; ++eye)
		{
			// Get view and projection matrices
			//TODO: revise once working
			OVR::Matrix4f finalRollPitchYaw = OVR::Matrix4f(EyeRenderPose[eye].Orientation);
			OVR::Vector3f finalUp = finalRollPitchYaw.Transform(OVR::Vector3f(0, 1, 0));
			OVR::Vector3f finalForward = finalRollPitchYaw.Transform(OVR::Vector3f(0, 0, -1));
			OVR::Vector3f eyePos = EyeRenderPose[eye].Position;

			OVR::Matrix4f view = OVR::Matrix4f::LookAtRH(eyePos, eyePos + finalForward, finalUp);
			OVR::Matrix4f proj = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[eye], 0.2f, 1000000.0f, ovrProjection_RightHanded);


			GL::Mat4 glview;
			memcpy(&(glview.m), &(view.M), 16 * sizeof(float));
			GL::Mat4 glproj;
			memcpy(&(glproj.m), &(proj.M), 16 * sizeof(float));
			eyePoses.push_back({glview.Inverse(), glproj});
		}
			
	} else {
		std::cout << "RIFT NOT CONNECTED!!!" << std::endl;
	}
	return eyePoses;
}
   
OVRManager::~OVRManager(){
    ovr_Destroy(session);
	ovr_Shutdown();
} 