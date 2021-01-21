
//Written by William Luk
// posts data to an Adafuit.io feed
let url = 'yourAPIdata';

var data = 0;

function setup() {
  createCanvas(400,400);
  myButton = createButton('Click me to turn on the LED!');
  myButton.mousePressed(press);
  myButton.mouseReleased(off);
}

function draw() {
  background(120);
}

function press() {
  data = 1;
  console.log(data);
  sendData(data);
}

function off() {
  data = 0;
  console.log(data);
  sendData(data);
}

function sendData(turnOn){
  let postData ={
    "value": turnOn,
    "X-AIO-Key": "yourAPIkey"
  };
  httpPost(url, 'json', postData, function(result){
    console.log(result);
  });
}