let isSteering = false;
let start_degree = 0;
let last_requested_steer = 0;
let toggleButtons;
let steeringWheel, gasPedal, headlight, leftSignal, flasher, rightSignal, gear, gearText;
const steerLimit = [-90, 90];
const gears = ["R", "N", "D"];
const baseUrl = "/api";

(function (window, document, undefined) {

  window.onload = init;

  function init() {
    steeringWheel = document.getElementById("steering-wheel");
    gasPedal = document.getElementById("gas-pedal");
    headlight = document.getElementById("headlight");
    leftSignal = document.getElementById("left-signal");
    flasher = document.getElementById("flasher");
    rightSignal = document.getElementById("right-signal");
    gear = document.getElementById("gear");
    gear.value = 1;
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
  let bb = steeringWheel.getBoundingClientRect();
  let steeringWheelCenter = [bb.left + bb.width / 2, bb.top + bb.height / 2];
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
  if (!isSteering)
    return;
  var rotation = calculateDegrees(e) - start_degree;
  rotation = limit(rotation, steerLimit[0], steerLimit[1]);
  steeringWheel.style.transform = "rotate(" + rotation + "deg)";
  round = Math.round(rotation);
  if (Math.abs(round - last_requested_steer) >= 5) {
    last_requested_steer = round;
    sendRequest("/steer", round);
  }
}, { passive: false })

function limit(num, min, max) {
  return Math.min(Math.max(num, min), max);
}

document.addEventListener('pointerup', (e) => {
  if (e.target.id == "gas-pedal")
    return;
  if (!isSteering)
    return;
  isSteering = false;
  steeringWheel.style.transition = 'transform 0.3s ease';
  steeringWheel.style.transform = `rotate(0deg)`;
  sendRequest("/steer", 0);
}, { passive: false })

function flash(e) {
  e.classList.toggle('active');
}

function toggleClick(toggle) {
  if (toggle instanceof Event)
    toggle = toggle.currentTarget;
  if (toggle.classList.contains('flasher'))
    if (isToggleOn(toggle)) {
      clearInterval(toggle['flashInterval'])
      toggle.classList.remove('active');
    }
    else {
      toggle.classList.add('active');
      toggle['flashInterval'] = setInterval(flash, 500, toggle);
    }
  else
    toggle.classList.toggle('active');

  toggle.classList.toggle('on');
}

function isToggleOn(toggle) {
  return toggle.classList.contains('on');
}

function resetToggle(toggle) {
  toggle.classList.remove('active');
  toggle.classList.remove('on');
  if (toggle.classList.contains('flasher'))
    clearInterval(toggle['flashInterval']);
}

document.addEventListener('contextmenu', function (e) {
  e.preventDefault();
});

function gasDown() {
  gasPedal.style.transform = "scaleY(0.9)"
  sendRequest("/gas", true);
}

function gasUp() {
  gasPedal.style.transform = "scaleY(1)"
  sendRequest("/gas", false);
}

function hornClick() {
  sendRequest("/horn", null);
}

function flasherClick() {
  resetToggle(leftSignal);
  resetToggle(rightSignal);
  if (isToggleOn(this)) {
    toggleClick(leftSignal);
    toggleClick(rightSignal);
  }
  sendRequest("/signal", isToggleOn(this) ? "both" : "off");
}

function rightSignalClick() {
  resetToggle(leftSignal);
  resetToggle(flasher);
  sendRequest("/signal", isToggleOn(this) ? "right" : "off");
}

function leftSignalClick() {
  resetToggle(rightSignal);
  resetToggle(flasher);
  sendRequest("/signal", isToggleOn(this) ? "left" : "off");
}

function headLightsClick() {
  sendRequest("/headLight", isToggleOn(this));
}

function gearChanged() {
  const value = parseFloat(gear.value);
  gearText.innerText = gears[value];
  sendRequest("/gear", gears[value].toLowerCase());
}

function sendRequest(url, param) {
  var urlstring = baseUrl + url;
  if (param !== null)
    urlstring += "/" + param;
  fetch(urlstring, { method: 'PUT' }).then((result) => {

  });
}