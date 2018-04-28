open AssetTreeNodeType;

/* open FileType; */
let increaseIndex = (editorState) => {
  let nextIndex = AssetEditorService.getIndex(editorState) + 1;
  (nextIndex, editorState |> AssetEditorService.setIndex(nextIndex))
};

let getRootTreeNodeId = (editorState) =>
  switch (editorState |> AssetEditorService.getAssetTree) {
  | None => 0
  | Some(assetTree) => assetTree |> ArrayService.getFirst |> ((treeNode) => treeNode.id)
  };

let getTargetTreeNodeId = (editorState) =>
  switch (editorState |> AssetEditorService.getCurrentTreeNode) {
  | None => editorState |> getRootTreeNodeId
  | Some(id) => id
  };

let _getTreeNodeName = (index) =>
  index === (getRootTreeNodeId |> StateLogicService.getEditorState) ? "Asset" : "newFolder";

let buildAssetTreeNodeByIndex = (index) => {
  id: index,
  name: _getTreeNodeName(index),
  imgArray: [||],
  children: [||]
};

let buildAssetTree = (editorState) =>
  switch (AssetEditorService.getAssetTree(editorState)) {
  | None => [|editorState |> AssetEditorService.getIndex |> buildAssetTreeNodeByIndex|]
  | Some(assetTree) => assetTree
  };

let rec insertNewTreeNodeToTargetTreeNode = (targetId, newTreeNode, assetTree) =>
  assetTree
  |> Js.Array.map(
       ({id, children} as treeNode) =>
         id == targetId ?
           {...treeNode, children: children |> Js.Array.copy |> ArrayService.push(newTreeNode)} :
           {
             ...treeNode,
             children: insertNewTreeNodeToTargetTreeNode(targetId, newTreeNode, children)
           }
     );

let rec addFileIntoTargetTreeNode = (targetId, fileId, type_, assetTree) =>
  assetTree
  |> Js.Array.map(
       ({id, children, imgArray} as treeNode) =>
         id === targetId ?
           switch type_ {
           | FileType.Image => {
               ...treeNode,
               imgArray: imgArray |> Js.Array.copy |> ArrayService.push(fileId)
             }
           | _ =>
             WonderLog.Log.fatal(
               WonderLog.Log.buildFatalMessage(
                 ~title="addFileIntoTargetTreeNode",
                 ~description={j|the type:$type_ not exist|j},
                 ~reason="",
                 ~solution={j||j},
                 ~params={j|type:$type_|j}
               )
             )
           } :
           {...treeNode, children: addFileIntoTargetTreeNode(targetId, fileId, type_, children)}
     );