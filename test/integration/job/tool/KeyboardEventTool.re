/* open StateDataMainType; */

let buildKeyboardDomEvent =
    (
      ~ctrlKey=false,
      ~altKey=false,
      ~shiftKey=false,
      ~metaKey=false,
      ~keyCode=8,
      (),
    ) => {
  "ctrlKey": ctrlKey,
  "altKey": altKey,
  "shiftKey": shiftKey,
  "metaKey": metaKey,
  "keyCode": keyCode,
};