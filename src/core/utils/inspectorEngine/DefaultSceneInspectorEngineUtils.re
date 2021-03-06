let getAmbientLightArr = () => [|0.6, 0.6, 0.6|];

let _initCameraAddToSceneGameObject = (camera, inspectorEngineState) => {
  let (inspectorEngineState, arcballCameraController) =
    ArcballCameraEngineService.create(inspectorEngineState);

  inspectorEngineState
  /* |> TransformEngineService.setLocalPosition(
       (0., 0., 1.1),
       GameObjectComponentEngineService.unsafeGetTransformComponent(
         camera,
         inspectorEngineState,
       ),
     ) */
  |> ArcballCameraEngineService.setArcballCameraControllerDistance(
       InspectorCanvasUtils.getCameraDefaultDistance(),
       arcballCameraController,
     )
  |> ArcballCameraEngineService.setArcballCameraControllerWheelSpeed(
       arcballCameraController,
       0.5,
     )
  |> ArcballCameraEngineService.setArcballCameraControllerMoveSpeedX(
       arcballCameraController,
       1.,
     )
  |> ArcballCameraEngineService.setArcballCameraControllerMoveSpeedY(
       arcballCameraController,
       1.,
     )
  |> InspectorCanvasUtils.initArcballCameraControllerAngle(
       arcballCameraController,
     )
  |> ArcballCameraControllerLogicService.bindArcballCameraControllerEventForInspector(
       arcballCameraController,
     )
  |> BasicCameraViewEngineService.activeBasicCameraView(
       inspectorEngineState
       |> GameObjectComponentEngineService.unsafeGetBasicCameraViewComponent(
            camera,
          ),
     )
  |> GameObjectComponentEngineService.addArcballCameraControllerComponent(
       camera,
       arcballCameraController,
     )
  |> SceneEngineService.addSceneChild(camera);
};

let _initDirectionLightAddToSceneGameObject =
    (directionLight, inspectorEngineState) =>
  inspectorEngineState
  |> TransformEngineService.setTransformLocalEulerAngles(
       (145., 15., 0.),
       GameObjectComponentEngineService.unsafeGetTransformComponent(
         directionLight,
         inspectorEngineState,
       ),
     )
  |> DirectionLightEngineService.setDirectionLightIntensity(
       0.25,
       GameObjectComponentEngineService.unsafeGetDirectionLightComponent(
         directionLight,
         inspectorEngineState,
       ),
     )
  |> SceneEngineService.addSceneChild(directionLight);

let _initEmptyGameObjectAddToSceneGameObject =
    (emptyGameObject, inspectorEngineState) =>
  inspectorEngineState |> SceneEngineService.addSceneChild(emptyGameObject);

let _initAmbientLight = inspectorEngineState =>
  inspectorEngineState
  |> SceneEngineService.setAmbientLightColor(getAmbientLightArr());

let createDefaultScene = inspectorEngineState => {
  let (inspectorEngineState, camera) =
    PrimitiveEngineService.createCamera(inspectorEngineState);
  let (inspectorEngineState, directionLight) =
    PrimitiveEngineService.createDirectionLight(inspectorEngineState);
  let (inspectorEngineState, emptyGameObject) =
    GameObjectEngineService.create(inspectorEngineState);

  let inspectorEngineState =
    inspectorEngineState
    |> _initCameraAddToSceneGameObject(camera)
    |> _initDirectionLightAddToSceneGameObject(directionLight)
    |> _initEmptyGameObjectAddToSceneGameObject(emptyGameObject)
    |> _initAmbientLight;

  (emptyGameObject, inspectorEngineState);
};