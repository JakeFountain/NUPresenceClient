

#include "OVRManager.h"
#include "utility/math/Conversion.h"
#include <algorithm>

namespace module {
namespace visualisation {

	OVRManager::OVRManager(){}

	bool OVRManager::init(){
		riftPresent = true;
		
	   	//Init OVR
	    ovrResult initResult = ovr_Initialize(nullptr);
		if (!OVR_SUCCESS(initResult)) {
			std::cout << "Failed to Init OVR" << std::endl;
			std::cout << "Error code: " << initResult << std::endl;
			riftPresent = false;
			return false;
		}

		//Init session
	    ovrResult result = ovr_Create(&session, &luid);
		if (!OVR_SUCCESS(result)) {
			std::cout << "Rift not found!" << std::endl;
			std::cout << "Error code: " << result << std::endl;
			riftPresent = false;
			ovr_Shutdown();
			return false;
		}

		//std::unique_ptr<ovrSession, decltype(ovr_Destroy)> thing(session, ovr_Destroy);

		hmdDesc = ovr_GetHmdDesc(session);

		bool eyeBufferResult = createEyeBuffers();

		EyeRenderDesc[0] = ovr_GetRenderDesc(session, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
		EyeRenderDesc[1] = ovr_GetRenderDesc(session, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

		ViewOffset[2] = { EyeRenderDesc[0].HmdToEyeOffset.y,
						  EyeRenderDesc[1].HmdToEyeOffset.y };
	

		// Turn off vsync to let the compositor do its magic
		wglSwapIntervalEXT(0);
		
		return eyeBufferResult;

	}

	void OVRManager::recenter() {
		ovrSessionStatus status;
		if (OVR_SUCCESS(ovr_GetSessionStatus(session, &status)) 
			&& status.HmdPresent) {
			ovr_RecenterTrackingOrigin(session);
		}
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

	bool OVRManager::createEyeBuffers()
	{
		// Configure Stereo settings.
		OVR::Sizei recommenedTex0Size = ovr_GetFovTextureSize(session, ovrEye_Left,
			hmdDesc.DefaultEyeFov[0], 1.0f);
		OVR::Sizei recommenedTex1Size = ovr_GetFovTextureSize(session, ovrEye_Right,
			hmdDesc.DefaultEyeFov[1], 1.0f);

		bufferSize.w = recommenedTex0Size.w + recommenedTex1Size.w;
		bufferSize.h = max(recommenedTex0Size.h, recommenedTex1Size.h);
		
		ovrTextureSwapChainDesc swapChainDesc;
		swapChainDesc.Type = ovrTextureType::ovrTexture_2D;
		swapChainDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;//GL_SRGB8_ALPHA8;
		swapChainDesc.ArraySize = 1;
		swapChainDesc.Width = bufferSize.w;
		swapChainDesc.Height = bufferSize.h;
		swapChainDesc.MipLevels = 1;
		swapChainDesc.SampleCount = 1;
		swapChainDesc.StaticImage = ovrFalse;
		swapChainDesc.MiscFlags = 0;
		swapChainDesc.BindFlags = 0;

		if (ovr_CreateTextureSwapChainGL(session, &swapChainDesc,
			&pTextureSet) != ovrSuccess)
		{
			std::cout << "OVR SWAP TEXTURE ALLOCATION FAILED" << std::endl;
			ovrErrorInfo err;
			ovr_GetLastErrorInfo(&err);
			std::cout << "ERROR INFO: " << err.ErrorString << " " << err.Result << std::endl;
			return false;
		}
		else {
			std::cout << "OVR SWAP TEXTURE ALLOCATION SUCCEEDED" << std::endl;
		}

		int numBuffs = 0;
		ovr_GetTextureSwapChainLength(session, pTextureSet, &numBuffs);

		for (int i = 0; i < numBuffs; i++) {
			//ovrGLTexture* tex = (ovrGLTexture*)&pTextureSet->Textures[i];
			GLuint id;
			ovr_GetTextureSwapChainBufferGL(session, pTextureSet, i, &id);
			eyeBuffer.push_back(std::make_unique<GL::Framebuffer>(bufferSize.w, bufferSize.h, &id));
		}
		
		return true;
	}

	void OVRManager::setRenderTarget(GL::Context& gl, OVRManager::RenderTarget target)
	{
		if (target == MIRROR) {

		}
		else {
			int currentIndex;
			ovr_GetTextureSwapChainCurrentIndex(session, pTextureSet, &currentIndex);
			gl.BindFramebuffer(*eyeBuffer[currentIndex]);
			glViewport(bufferSize.w * int(target) / 2, 0, bufferSize.w / 2, bufferSize.h);
		}
	}

	GL::Mat4 OVRManager::getRawHeadPose(){
        //TODO: documentation of this method suggests  incrementing the second argument (currently 0)
        double           ftiming = ovr_GetPredictedDisplayTime(session, 0);
        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
        // double           sensorSampleTime = ovr_GetTimeInSeconds();
        ovrTrackingState hmdState = ovr_GetTrackingState(session, ftiming, ovrTrue);
        auto M = OVR::Matrix4f(hmdState.HeadPose.ThePose.Orientation);
        M = OVR::Matrix4f::Translation(hmdState.HeadPose.ThePose.Position) * M;
		M = M.InvertedHomogeneousTransform();

		GL::Mat4 glview;
		memcpy(&(glview.m), &(M.M), 16 * sizeof(float));
		return glview.Transpose();
	}

	std::vector<EyePose> OVRManager::getCurrentPoses(){
		std::vector<EyePose> eyePoses;
		if(riftPresent){
			//Struct to emit
	       	
	       	// Get eye poses, feeding in correct IPD offset
			ViewOffset[0] = EyeRenderDesc[0].HmdToEyeOffset;
			ViewOffset[1] = EyeRenderDesc[1].HmdToEyeOffset;

	        ovrPosef                  EyeRenderPose[2];

        	//TODO: documentation of this method suggests  incrementing the second argument (currently 0)
	        double           ftiming = ovr_GetPredictedDisplayTime(session, 0);
	        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
	        // double           sensorSampleTime = ovr_GetTimeInSeconds();
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
				OVR::Matrix4f proj = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[eye], 0.01f, 300.0f, ovrProjectionModifier::ovrProjection_None);


				GL::Mat4 glview;
				memcpy(&(glview.m), &(view.M), 16 * sizeof(float));
				GL::Mat4 glproj;
				memcpy(&(glproj.m), &(proj.M), 16 * sizeof(float));
				eyePoses.push_back({ glview.Transpose(), glproj.Transpose()});
			}
				
		} else {
			//std::cout << "RIFT NOT CONNECTED!!!" << std::endl;
		}
		return eyePoses;
	}

	bool OVRManager::renderToRift(){
		
		double           ftiming = ovr_GetPredictedDisplayTime(session, 0);
		// Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
		double           sensorSampleTime = ovr_GetTimeInSeconds();
		ovrTrackingState hmdState = ovr_GetTrackingState(session, ftiming, ovrTrue);
		ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);

		// Create eye layer.
		ovrLayerEyeFov eyeLayer;
		eyeLayer.Header.Type = ovrLayerType_EyeFov;
		eyeLayer.Header.Flags = 0;

		for (int eye = 0; eye < 2; eye++)
		{
			eyeLayer.ColorTexture[eye] = pTextureSet;
			eyeLayer.Viewport[eye] = ovrRecti{ ovrVector2i{eye * bufferSize.w / 2,0}, ovrSizei{bufferSize.w / 2,bufferSize.h}};
			eyeLayer.Fov[eye] = EyeRenderDesc[eye].Fov;
			eyeLayer.RenderPose[eye] = EyeRenderPose[eye];
		}

		// The list of layers.
		ovrLayerHeader *layerList = &eyeLayer.Header;

		// Set up positional data.
		ovrViewScaleDesc viewScaleDesc;
		viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
		viewScaleDesc.HmdToEyeOffset[0] = ViewOffset[0];
		viewScaleDesc.HmdToEyeOffset[1] = ViewOffset[1];

		ovr_CommitTextureSwapChain(session, pTextureSet);
		ovrResult result = ovr_SubmitFrame(session, 0, &viewScaleDesc, &layerList, 1);
		ovr_GetTextureSwapChainCurrentIndex(session, pTextureSet, &lastEyeTextureIndex);
		//TODO: how to increment the current index?
		//pTextureSet->CurrentIndex = (pTextureSet->CurrentIndex + 1) % pTextureSet->TextureCount;

		if (result != ovrSuccess) {
			ovrErrorInfo err;
			ovr_GetLastErrorInfo(&err);
			std::cout << "Submit frame failed! - ";
			std::cout << "ERROR INFO: " << err.ErrorString << " " << err.Result << std::endl;
		}

		return result == ovrSuccess;
	}


	GLuint OVRManager::getLastEyeTexture() {
		return eyeBuffer[lastEyeTextureIndex]->GetTexture();
	}
	   
	OVRManager::~OVRManager(){
		ovr_DestroyTextureSwapChain(session, pTextureSet);
	    ovr_Destroy(session);
		ovr_Shutdown();
		delete pTextureSet;
	} 

}
}