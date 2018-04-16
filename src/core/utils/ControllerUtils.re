let run = (store, ()) => {
  SceneEditorService.setIsRun(true) |> StateLogicService.getEditorState;
  AllStateData.getHistoryState()
  |> ControllerHistoryUtils.copyHistoryStack(store, StateHistoryService.getStateForHistory());
  LoopEngineService.loop() |> ignore
};

let stop = (dispatch, ()) => {
  /* TODO bug: stop loop */
  SceneEditorService.setIsRun(false) |> StateLogicService.getEditorState;
  AllStateData.getHistoryState()
  |> ControllerHistoryUtils.restoreHistoryStack(
       dispatch,
       StateLogicService.getEditEngineState(),
       StateLogicService.getRunEngineState()
     );
  (
    (editorState) =>
      editorState
      |> LoopEditorService.getLoopId
      |> WonderLog.Log.print
      |> LoopEngineService.stopLoop
  )
  |> StateLogicService.getEditorState
};