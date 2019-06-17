let updateJob = (_, engineState) =>
  engineState
  |> Wonderjs.UpdatePerspectiveCameraProjectionMainService.update
  |> Wonderjs.UpdateArcballCameraControllerMainService.updateAll
  |> Wonderjs.UpdateFlyCameraControllerMainService.update;