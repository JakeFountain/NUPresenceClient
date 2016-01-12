

#include "OVRManager.h"


OVRManager::OVRManager(){
	riftPresent = true;
   
   	//Init OVR
    ovrResult initResult = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(initResult)) {
		std::cout << "Failed to Init" << std::endl;
		std::cout << initResult << std::endl;
		riftPresent = false;
	}

	//Init HMD
    ovrResult result = ovr_Create(&HMD, &luid);
	if (!OVR_SUCCESS(result)) {
		std::cout << "Rift not found!" << std::endl;
		std::cout << result << std::endl;
		riftPresent = false;
	}

    if(riftPresent) hmdDesc = ovr_GetHmdDesc(HMD);


}

void OVRManager::printCurrentPose(){
	if(riftPresent){
        double ftiming = ovr_GetPredictedDisplayTime(HMD, 0);
        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
        ovrTrackingState hmdState = ovr_GetTrackingState(HMD, ftiming, ovrTrue);

		// std::cout << "hmdState.HeadPose.ThePose = "
		// 	<< hmdState.HeadPose.ThePose.Orientation.w << " "
		// 	<< hmdState.HeadPose.ThePose.Orientation.x << " "
		// 	<< hmdState.HeadPose.ThePose.Orientation.y << " "
		// 	<< hmdState.HeadPose.ThePose.Orientation.z << " "
		// 	<< hmdState.HeadPose.ThePose.Position.x << " "
		// 	<< hmdState.HeadPose.ThePose.Position.y << " "
		// 	<< hmdState.HeadPose.ThePose.Position.z << " " << std::endl;

	}
}
   
OVRManager::~OVRManager(){
    ovr_Destroy(HMD);
} 