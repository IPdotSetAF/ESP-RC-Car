let isSteering = false;
let start_degree = 0;
let last_requested_steer = 0;
let toggleButtons;
let steeringWheel, gasPedal, headlight, leftSignal, flasher, rightSignal, gear, gearText, debugConsole;
const steerLimit = [-90, 90];
const gears = ["R", "N", "D"];
const baseUrl = "/api";
const flashInterval = 500;

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
    debugConsole = document.getElementById("debug-txt");

    getAll();

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

function getAll() {
  httpRequest("/all", "GET", null, (result) => {
    setToggle(headlight, result.headLight);
    setToggle(leftSignal, result.signal == "both" || result.signal == "left");
    setToggle(flasher, result.signal == "both");
    setToggle(rightSignal, result.signal == "both" || result.signal == "right");

    let gearIndex = gears.indexOf(result.gear.toUpperCase());
    gear.value = gearIndex;
    gearText.innerText = gears[gearIndex];
  });
}

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
  if (e.target.id == "gas-pedal" || e.target.id == "gear" || e.target.id == "horn-button")
    return;
  if (!isSteering)
    return;
  var rotation = calculateDegrees(e) - start_degree;
  rotation = limit(rotation, steerLimit[0], steerLimit[1]);
  steeringWheel.style.transform = "rotate(" + rotation + "deg)";
  round = Math.round(rotation);
  if (Math.abs(round - last_requested_steer) >= 5) {
    last_requested_steer = round;
    httpRequest("/steer", "PUT", round);
  }
}, { passive: false })

function limit(num, min, max) {
  return Math.min(Math.max(num, min), max);
}

document.addEventListener('pointerup', (e) => {
  if (e.target.id == "gas-pedal" || e.target.id == "gear" || e.target.id == "horn-button")
    return;
  if (!isSteering)
    return;
  isSteering = false;
  steeringWheel.style.transition = 'transform 0.3s ease';
  steeringWheel.style.transform = `rotate(0deg)`;
  httpRequest("/steer", "PUT", 0);
}, { passive: false })

function flash(e) {
  e.classList.toggle('active');
}

function toggleClick(toggle) {
  if (toggle instanceof Event)
    toggle = toggle.currentTarget;
  if (isToggleOn(toggle))
    setToggle(toggle, false);
  else
    setToggle(toggle, true);
}

function isToggleOn(toggle) {
  return toggle.classList.contains('on');
}

function resetToggle(toggle) {
  setToggle(toggle, false);
}

function setToggle(toggle, state) {
  if (state) {
    if (toggle.classList.contains('flasher')) {
      toggle.classList.add('active');
      toggle['flashInterval'] = setInterval(flash, flashInterval, toggle);
    }
    toggle.classList.add('on');
  } else {
    toggle.classList.remove('active');
    toggle.classList.remove('on');
    if (toggle.classList.contains('flasher'))
      clearInterval(toggle['flashInterval']);
  }
}

document.addEventListener('contextmenu', function (e) {
  e.preventDefault();
});

function gasDown() {
  gasPedal.style.transform = "scaleY(0.9)"
  httpRequest("/gas", "PUT", true);
}

function gasUp() {
  gasPedal.style.transform = "scaleY(1)"
  httpRequest("/gas", "PUT", false);
}

function hornDown() {
  httpRequest("/horn", "PUT", true);
}

function hornUp() {
  httpRequest("/horn", "PUT", false);
}

function flasherClick() {
  resetToggle(leftSignal);
  resetToggle(rightSignal);
  if (isToggleOn(this)) {
    toggleClick(leftSignal);
    toggleClick(rightSignal);
  }
  httpRequest("/signal", "PUT", isToggleOn(this) ? "both" : "off");
}

function rightSignalClick() {
  resetToggle(leftSignal);
  resetToggle(flasher);
  httpRequest("/signal", "PUT", isToggleOn(this) ? "right" : "off");
}

function leftSignalClick() {
  resetToggle(rightSignal);
  resetToggle(flasher);
  httpRequest("/signal", "PUT", isToggleOn(this) ? "left" : "off");
}

function headLightsClick() {
  httpRequest("/headLight", "PUT", isToggleOn(this));
}

function gearChanged() {
  const value = parseFloat(gear.value);
  gearText.innerText = gears[value];
  httpRequest("/gear", "PUT", gears[value]);
}

async function httpRequest(url, type, param = null, func = null) {
  var urlstring = buildUrl(url, param);
  req = new QRequest(urlstring, type);
  requests.push(req);
  refreshConsole();
  try {
    const response = await fetch(urlstring, { method: type });
    req.setStatus(response.status);
    refreshConsole();
    text = await response.text();
    try {
      data = JSON.parse(text);
      req.setResult(data);
      if (func !== null)
        func(data);
    } catch (error) {
      req.setResult(text);
    }
    refreshConsole();
  } catch (error) { }
}

function buildUrl(url, param) {
  var urlstring = baseUrl + url;
  if (param !== null)
    urlstring += "/" + param;
  return urlstring
}

function refreshConsole() {
  if (requests.length > 30)
    requests.splice(0, 1);
  tmp = "";
  requests.forEach(req => {
    tmp += req.toString() + "\n";
  });
  debugConsole.innerText = tmp;
}

var requests = [];

class QRequest {
  constructor(url, type) {
    this.url = url;
    this.type = type;
  }

  setStatus(status) {
    this.status = status;
  }

  setResult(body) {
    this.result = JSON.stringify(body);
  }

  toString() {
    return `${this.type}\t${this.url}\t->\t${this.status !== undefined ? this.status : ""}\t\t${this.result !== undefined ? this.result : ""}`;
  }
}