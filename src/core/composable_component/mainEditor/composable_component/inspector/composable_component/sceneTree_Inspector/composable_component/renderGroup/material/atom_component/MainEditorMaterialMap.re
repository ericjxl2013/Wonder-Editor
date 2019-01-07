open NodeAssetType;

type state = {
  style: ReactDOMRe.Style.t,
  isShowTextureGroup: bool,
  currentTextureComponent: option(int),
};

type retainedProps = {map: option(int)};

type action =
  | Nothing
  | DragEnter
  | DragLeave
  | DragDrop(int)
  | SetTextureToEngine(int, int)
  | ShowTextureGroup
  | HideTextureGroup;

module Method = {
  let isWidget = startWidget =>
    switch (startWidget) {
    | None => false
    | Some(widget) => widget == AssetWidgetService.getWidget()
    };

  let isTypeValid = (startNodeId, editorState) =>
    switch (startNodeId) {
    | None => false
    | Some(id) =>
      OperateTreeAssetEditorService.isNodeExistById(id, editorState)
    };

  let _isTriggerAction = (isWidgetFunc, isTypeValidFunc) => {
    let (widget, startNodeId) =
      StateEditorService.getState()
      |> CurrentDragSourceEditorService.getCurrentDragSource;

    isWidgetFunc(widget)
    && isTypeValidFunc(startNodeId, StateEditorService.getState());
  };

  let handleDragEnter = (isWidgetFunc, isTypeValidFunc, _event) =>
    _isTriggerAction(isWidgetFunc, isTypeValidFunc) ? DragEnter : Nothing;

  let handleDragLeave = (isWidgetFunc, isTypeValidFunc, event) => {
    ReactEventType.convertReactMouseEventToJsEvent(event)
    |> DomHelper.stopPropagation;

    _isTriggerAction(isWidgetFunc, isTypeValidFunc) ? DragLeave : Nothing;
  };

  let handleDragOver = event =>
    ReactEventType.convertReactMouseEventToJsEvent(event)
    |> DomHelper.preventDefault;

  let handleSetTextureToEngine = (isWidgetFunc, isTypeValidFunc, event) => {
    let e = ReactEventType.convertReactMouseEventToJsEvent(event);
    let startNodeId = e |> DragUtils.getDragedId;

    DomHelper.preventDefault(e);

    _isTriggerAction(isWidgetFunc, isTypeValidFunc) ?
      DragDrop(startNodeId) : DragLeave;
  };

  let showMapComponent = currentTextureComponent =>
    switch (currentTextureComponent) {
    | None => ReasonReact.null
    | Some(map) =>
      let source =
        BasicSourceTextureEngineService.unsafeGetSource(
          map,
          StateEngineService.unsafeGetState(),
        );

      <img
        src=ImageType.convertImageElementToSrcImageElements(source)##src
      />;
    };

  let _sortByName = (engineState, allTextureNodes) =>
    allTextureNodes
    |> Js.Array.sortInPlaceWith((textureNode1, textureNode2) =>
         Js.String.localeCompare(
           NodeNameAssetLogicService.getTextureNodeName(
             ~texture=
               TextureNodeAssetService.getTextureComponent(textureNode2),
             ~engineState,
           )
           |> Js.String.charAt(0),
           NodeNameAssetLogicService.getTextureNodeName(
             ~texture=
               TextureNodeAssetService.getTextureComponent(textureNode1),
             ~engineState,
           )
           |> Js.String.charAt(0),
         )
         |> NumberType.convertFloatToInt
       );

  let _buildTextureUIComponent =
      ((className, nodeId, textureComponent, imgSrc), send, engineState) =>
    <div
      className
      key=(DomHelper.getRandomKey())
      onClick=(_e => send(SetTextureToEngine(nodeId, textureComponent)))>
      <img src=imgSrc className="imgContent-img" />
      <div className="imgContent-text">
        (
          DomHelper.textEl(
            NodeNameAssetLogicService.getTextureNodeName(
              ~texture=textureComponent,
              ~engineState,
            ),
          )
        )
      </div>
    </div>;

  let _buildTextureUIComponentResult = ((nodeId, textureComponent, imageDataIndex),send,(editorState, engineState)) =>
    ImageDataMapUtils.getImgSrc(imageDataIndex, editorState)
    |> Result.SameDataResult.either(imgSrc =>
         _buildTextureUIComponent(
           ("select-item-imgContent", nodeId, textureComponent, imgSrc),
           send,
           engineState,
         )
         |> Result.SameDataResult.success
       );

  let showTextureAssets = (state, send) => {
    let editorState = StateEditorService.getState();
    let engineState = StateEngineService.unsafeGetState();

    TextureNodeAssetEditorService.findAllTextureNodes(editorState)
    |> _sortByName(engineState)
    |> ArrayService.traverseSameDataResultAndCollectByApply(textureNode => {
         let {textureComponent, imageDataIndex} =
           TextureNodeAssetService.getNodeData(textureNode);
         let nodeId = NodeAssetService.getNodeId(~node=textureNode);

         switch (state.currentTextureComponent) {
         | None => _buildTextureUIComponentResult((nodeId, textureComponent, imageDataIndex),send,(editorState, engineState))
         | Some(map) =>
           let className =
             map === textureComponent ?
               "select-item-imgContent select-item-active" :
               "select-item-imgContent";

           _buildTextureUIComponentResult((nodeId, textureComponent, imageDataIndex),send,(editorState, engineState));
         };
       });
  };
};

let component = ReasonReact.reducerComponent("MainEditorMaterialMap");

let reducer =
    ((store, dispatchFunc), (materialComponent, onDropFunc), action, state) =>
  switch (action) {
  | DragEnter =>
    ReasonReact.Update({
      ...state,
      style:
        ReactUtils.addStyleProp("border", "2px solid coral", state.style),
    })

  | DragLeave =>
    ReasonReact.Update({
      ...state,
      style:
        ReactUtils.addStyleProp(
          "border",
          "2px solid rgb(167,165,165)",
          state.style,
        ),
    })

  | DragDrop(startNodeId) =>
    ReasonReactUtils.sideEffects(() =>
      onDropFunc((store, dispatchFunc), materialComponent, startNodeId)
    )

  | SetTextureToEngine(textureNodeId, textureComponent) =>
    switch (state.currentTextureComponent) {
    | None =>
      ReasonReactUtils.updateWithSideEffects(
        {...state, currentTextureComponent: Some(textureComponent)}, _state =>
        onDropFunc((store, dispatchFunc), materialComponent, textureNodeId)
      )
    | Some(sourceTextureComponent) =>
      sourceTextureComponent === textureComponent ?
        ReasonReact.NoUpdate :
        ReasonReactUtils.updateWithSideEffects(
          {...state, currentTextureComponent: Some(textureComponent)}, _state =>
          onDropFunc((store, dispatchFunc), materialComponent, textureNodeId)
        )
    }

  | Nothing => ReasonReact.NoUpdate

  | ShowTextureGroup =>
    ReasonReact.Update({...state, isShowTextureGroup: true})

  | HideTextureGroup =>
    ReasonReact.Update({...state, isShowTextureGroup: false})
  };

let _renderDragableImage =
    (store, {state, send}: ReasonReact.self('a, 'b, 'c)) =>
  <div
    className="texture-img"
    onClick=(_e => send(ShowTextureGroup))
    style=state.style>
    <div
      className="img-dragBg"
      onDragEnter=(
        _e =>
          send(
            Method.handleDragEnter(Method.isWidget, Method.isTypeValid, _e),
          )
      )
      onDragLeave=(
        _e =>
          send(
            Method.handleDragLeave(Method.isWidget, Method.isTypeValid, _e),
          )
      )
      onDragOver=Method.handleDragOver
      onDrop=(
        _e =>
          send(
            Method.handleSetTextureToEngine(
              Method.isWidget,
              Method.isTypeValid,
              _e,
            ),
          )
      )
    />
    (Method.showMapComponent(state.currentTextureComponent))
  </div>;

let _renderTextureGroup = (state, send) =>
  <div className="select-component-content">
    <div className="select-component-item">
      <div className="select-item-header">
        (DomHelper.textEl("Texture"))
      </div>
      <div className="select-item-imgBody">
        <div className="imgBody-content">
          (
            ReasonReact.array(
              Method.showTextureAssets(state, send) |> ResultUtils.handleError,
            )
          )
        </div>
      </div>
    </div>
    <div
      className="select-component-bg"
      onClick=(_e => send(HideTextureGroup))
    />
  </div>;

let render =
    (
      (store, dispatchFunc),
      (materialComponent, label),
      (getMapFunc, removeTextureFunc),
      ({state, send}: ReasonReact.self('a, 'b, 'c)) as self,
    ) =>
  <article className="inspector-item">
    <div className="item-header"> (DomHelper.textEl(label)) </div>
    <div className="item-content item-texture">
      (_renderDragableImage(store, self))
      <button
        className="texture-remove"
        onClick=(
          e =>
            removeTextureFunc((store, dispatchFunc), (), materialComponent)
        )>
        (DomHelper.textEl("Remove"))
      </button>
    </div>
    (
      state.isShowTextureGroup ?
        _renderTextureGroup(state, send) : ReasonReact.null
    )
  </article>;

let make =
    (
      ~store: AppStore.appState,
      ~dispatchFunc,
      ~materialComponent,
      ~label,
      ~getMapFunc,
      ~onDropFunc,
      ~removeTextureFunc,
      ~isShowTextureGroup,
      _children,
    ) => {
  ...component,
  initialState: () => {
    style: ReactDOMRe.Style.make(~opacity="1", ()),
    isShowTextureGroup,
    currentTextureComponent:
      getMapFunc(materialComponent)
      |> StateLogicService.getEngineStateToGetData,
  },
  reducer: reducer((store, dispatchFunc), (materialComponent, onDropFunc)),
  render: self =>
    render(
      (store, dispatchFunc),
      (materialComponent, label),
      (getMapFunc, removeTextureFunc),
      self,
    ),
};