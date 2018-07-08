

import * as SparseMapService$WonderEditor from "../../atom/SparseMapService.js";
import * as SparseMapService$WonderCommonlib from "../../../../../../node_modules/wonder-commonlib/lib/es6_global/src/SparseMapService.js";

function unsafeGetJsonNodeMap(assetState) {
  return assetState[/* jsonNodeMap */5];
}

function setJsonNodeMap(jsonNodeMap, assetState) {
  return /* record */[
          /* assetTreeRoot */assetState[/* assetTreeRoot */0],
          /* index */assetState[/* index */1],
          /* currentNodeData */assetState[/* currentNodeData */2],
          /* currentNodeParentId */assetState[/* currentNodeParentId */3],
          /* textureNodeMap */assetState[/* textureNodeMap */4],
          /* jsonNodeMap */jsonNodeMap,
          /* folderNodeMap */assetState[/* folderNodeMap */6],
          /* imageBase64Map */assetState[/* imageBase64Map */7]
        ];
}

function clearJsonNodeMap(assetState) {
  return /* record */[
          /* assetTreeRoot */assetState[/* assetTreeRoot */0],
          /* index */assetState[/* index */1],
          /* currentNodeData */assetState[/* currentNodeData */2],
          /* currentNodeParentId */assetState[/* currentNodeParentId */3],
          /* textureNodeMap */assetState[/* textureNodeMap */4],
          /* jsonNodeMap */SparseMapService$WonderCommonlib.createEmpty(/* () */0),
          /* folderNodeMap */assetState[/* folderNodeMap */6],
          /* imageBase64Map */assetState[/* imageBase64Map */7]
        ];
}

function setResult(index, result, assetState) {
  return /* record */[
          /* assetTreeRoot */assetState[/* assetTreeRoot */0],
          /* index */assetState[/* index */1],
          /* currentNodeData */assetState[/* currentNodeData */2],
          /* currentNodeParentId */assetState[/* currentNodeParentId */3],
          /* textureNodeMap */assetState[/* textureNodeMap */4],
          /* jsonNodeMap */SparseMapService$WonderEditor.immutableSet(index, result, assetState[/* jsonNodeMap */5]),
          /* folderNodeMap */assetState[/* folderNodeMap */6],
          /* imageBase64Map */assetState[/* imageBase64Map */7]
        ];
}

export {
  unsafeGetJsonNodeMap ,
  setJsonNodeMap ,
  clearJsonNodeMap ,
  setResult ,
  
}
/* SparseMapService-WonderEditor Not a pure module */