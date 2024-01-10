
function calculateDegrees(element, event) {
  bb = element.getBoundingClientRect()
  var x = event.offsetX - bb.width / 2,
    y = event.offsetY - bb.height / 2;
  return Math.atan2(y, x) * (180 / Math.PI);
}


function steerStart(event) {
  isSteering = true;
  start_degree = calculateDegrees(steeringWheel, event);
  steeringWheel.style.transition = 'none';
}

document.addEventListener('onpointermove', (e) => {
  if (isSteering) {
    var rotation = calculateDegrees(steeringWheel, event) + start_degree;
    steeringWheel.style.transform = "rotate(" + rotation + "deg)";
    console.log(rotation);
  }
})

document.addEventListener('onpointerup', (e) => {
  if (isSteering) {
    isSteering = false;
    steeringWheel.style.transition = 'transform 0.3s ease';
    steeringWheel.style.transform = `rotate(0deg)`;
  }
})

function gasDown() {
  gasPedal.style.transform = "scaleY(0.9)"
}

function gasUp() {
  gasPedal.style.transform = "scaleY(1)"
}

function hornClick(){

}

function flasherClick(){

}

function rightSignalClick(){

}

function leftSignalClick(){

}

function headLightsClick(){

}

function gearChanged(event){

}

let isSteering = false;
let start_degree = 0;
let steeringWheel;
let gasPedal;

function init(){
  steeringWheel = document.getElementById("steering-wheel");
  gasPedal = document.getElementById("gas-pedal");
}

window.onload = init();