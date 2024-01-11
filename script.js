let isSteering = false;
let start_degree = 0;
let steeringWheelCenter;
let toggleButtons;
let steeringWheel, gasPedal, headlight, leftSignal, flasher, rightSignal, gear, gearText;
const steerLimit = [-90, 90];

(function (window, document, undefined) {

  window.onload = init;

  function init() {
    steeringWheel = document.getElementById("steering-wheel");
    bb = steeringWheel.getBoundingClientRect();
    steeringWheelCenter = [bb.left + bb.width / 2, bb.top + bb.height / 2];
    gasPedal = document.getElementById("gas-pedal");
    headlight = document.getElementById("headlight");
    leftSignal = document.getElementById("left-signal");
    flasher = document.getElementById("flasher");
    rightSignal = document.getElementById("right-signal");
    gear = document.getElementById("gear");
    gear.value = 2;
    gearText = document.getElementById("gear-text");

    toggleButtons = document.getElementsByClassName('toggle-button');

    for (var i = 0; i < toggleButtons.length; i++) {
      toggleButtons[i].addEventListener('click', toggleClick);
      if (toggleButtons[i].classList.contains('flasher'))
        toggleButtons[i]['flashInterval'] = null;
    }
    headlight.addEventListener("click", headLightsClick);
    leftSignal.addEventListener("click", leftSignalClick);
    flasher.addEventListener("click", flasherClick);
    rightSignal.addEventListener("click", rightSignalClick);
    gear.addEventListener("input", gearChanged);
  }

})(window, document, undefined);

function calculateDegrees(event) {
  var x = event.x - steeringWheelCenter[0],
    y = event.y - steeringWheelCenter[1];
  var degrees = Math.atan2(y, x) * (180 / Math.PI) - 90
  degrees = (degrees + 360) % 360;
  return degrees;
}

function steerStart(event) {
  isSteering = true;
  start_degree = calculateDegrees(event);
  steeringWheel.style.transition = 'none';
}

document.addEventListener('pointermove', (e) => {
  if (isSteering) {
    var rotation = calculateDegrees(e) - start_degree;
    rotation = limit(rotation, steerLimit[0], steerLimit[1]);
    steeringWheel.style.transform = "rotate(" + rotation + "deg)";
  }
}, { passive: false })

function limit(num, min, max) {
  return Math.min(Math.max(num, min), max);
}

document.addEventListener('pointerup', (e) => {
  if (isSteering) {
    isSteering = false;
    steeringWheel.style.transition = 'transform 0.3s ease';
    steeringWheel.style.transform = `rotate(0deg)`;
  }
}, { passive: false })

function flash(e) {
  e.classList.toggle('active');
}

var flashingInterval;

function toggleClick() {
  if (this.classList.contains('flasher'))
    if (this.classList.contains('on')) {
      clearInterval(this['flashInterval'])
      this.classList.remove('active');
    }
    else {
      this.classList.add('active');
      this['flashInterval'] = setInterval(flash, 500, this);
    }
  else
    this.classList.toggle('active');

  this.classList.toggle('on');
}

document.addEventListener('contextmenu', function (e) {
  e.preventDefault();
});

function gasDown() {
  gasPedal.style.transform = "scaleY(0.9)"
}

function gasUp() {
  gasPedal.style.transform = "scaleY(1)"
}

function hornClick() {

}

function flasherClick() {
  console.log(this.classList.contains('on'))
}

function rightSignalClick() {

}

function leftSignalClick() {

}

function headLightsClick() {
  console.log(this.classList.contains('on'))
}

function gearChanged() {
  const value = parseFloat(gear.value);
  switch (value) {
    case 1:
      gearText.innerText = "R";
      break;
    case 2:
      gearText.innerText = "N";
      break;
    case 3:
      gearText.innerText = "D";
      break;
  }
}