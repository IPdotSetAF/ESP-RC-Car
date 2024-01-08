
function calculateDegrees(element, event){
    bb = element.getBoundingClientRect()
    var x = event.offsetX - bb.width/2,
        y = event.offsetY - bb.height/2;
    return Math.atan2(y,x) * (180/Math.PI);
}

$start_degree = 0;

function dragStart(event) {
    steeringWheel = document.getElementById("steering-wheel");
    $start_degree = calculateDegrees(steeringWheel, event);
  }
  
  function dragging(event) {
    steeringWheel = document.getElementById("steering-wheel");
    var rotation = calculateDegrees(steeringWheel, event) - $start_degree;
    steeringWheel.style.transform = "rotate(" + rotation +"deg)";
    console.log(rotation);
  }
  