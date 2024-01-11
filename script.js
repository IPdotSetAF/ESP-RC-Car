let isSteering = false;
let start_degree = 0;
let toggleButtons;
let steeringWheel, gasPedal, headlight, leftSignal, flasher, rightSignal;


(function (window, document, undefined) {

  window.onload = init;

  function init() {
    steeringWheel = document.getElementById("steering-wheel");
    gasPedal = document.getElementById("gas-pedal");
    headlight = document.getElementById("headlight");
    leftSignal = document.getElementById("left-signal");
    flasher = document.getElementById("flasher");
    rightSignal = document.getElementById("right-signal");


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
  }

})(window, document, undefined);


function calculateDegrees(element, event) {
  bb = element.getBoundingClientRect()
  var x = event.offsetX - bb.width / 2,
    y = event.offsetY - bb.height / 2;
  return Math.atan2(y, x) * (180 / Math.PI);
}


function steerStart(event) {
  isSteering = true;
  start_degree = 90;//calculateDegrees(steeringWheel, event);
  steeringWheel.style.transition = 'none';
}

document.addEventListener('pointermove', (e) => {
  if (isSteering) {
    var rotation = calculateDegrees(steeringWheel, e) + start_degree;
    steeringWheel.style.transform = "rotate(" + rotation + "deg)";
    console.log(rotation);
  }
}, { passive: false })

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

}