var xhr = new XMLHttpRequest();
var url = 'http://apis.data.go.kr/B090041/openapi/service/RiseSetInfoService/getAreaRiseSetInfo'; /*URL*/
var queryParams = '?' + encodeURIComponent('serviceKey') + '='+'sz%2BT0LU2UAtWcd3lRFWm8FX1EhVcy51Dd77mxJe%2FxyaZdi4uKvJzBg12sZm3dlpsl9p6soutm8O13bdAKeamFQ%3D%3D'; /*Service Key*/
queryParams += '&' + encodeURIComponent('locdate') + '=' + encodeURIComponent('20150101'); /**/
queryParams += '&' + encodeURIComponent('location') + '=' + encodeURIComponent('서울'); /**/
xhr.open('GET', url + queryParams);
xhr.onreadystatechange = function () {
    if (this.readyState == 4) {
        alert('Status: '+this.status+'nHeaders: '+JSON.stringify(this.getAllResponseHeaders())+'nBody: '+this.responseText);
    }
};

xhr.send('');


var colorPicker = new iro.ColorPicker(".colorPicker", {
  width: 280,
  sliderSize: 10,
  borderWidth : 1.5,
  handleRadius : 12,
  layout: [
    { 
      component: iro.ui.Slider,
      options: {
        // can also be 'saturation', 'value', 'red', 'green', 'blue', 'alpha' or 'kelvin'
        sliderType: 'kelvin'
      }
    },
  ]
});


var values = document.getElementById("values");
var hexInput = document.getElementById("hexInput");
var hexset = document.getElementById("hexset");

var redInput = document.getElementById("redInput");
var greenInput = document.getElementById("greenInput");
var blueInput = document.getElementById("blueInput");
var rgbset_button = document.getElementById("rgbset_button");

var Socket;

var optionButton = document.getElementsByClassName('optionButton');
var sensorButton = document.getElementById('sensorButton');
var on_button = document.getElementById('on_button');

for (var i = 0; i < optionButton.length; i++) {
  optionButton[i].addEventListener('click', function(){
    if(this.value == 1){
      for (var j = 0; j < optionButton.length; j++) {
        optionButton[j].style.backgroundColor = "white";
        optionButton[j].value = '0';
      }
    }
    else{
      for (var j = 0; j < optionButton.length; j++) {
        optionButton[j].style.backgroundColor = "white";
        optionButton[j].value = '0';
      }
        this.style.backgroundColor = "#858585";
        this.value = '1';
    }
    Socket.send('@' + this.id + this.value); //옵션 전달 함수
  })
}

sensorButton.addEventListener('click', function () {
  document.getElementById("sensorButton").style.fontSize = "10px";
  if(sensorButton.value =='ON'){
    sensorButton.style.backgroundColor = "white";
    sensorButton.value ='OFF';
    document.getElementById("sensorButton").style.color = "#858585";
    Socket.send('!' + sensorButton.value); //센서 onoff 전달 함수
  }
  else {
    sensorButton.style.backgroundColor = "#858585";
    sensorButton.value ='ON';
    document.getElementById("sensorButton").style.color = "white";
    Socket.send('!' + sensorButton.value); //센서 onoff 전달 함수
  }
})

function init() {
  Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
}
function sendRgbColor() { //색상 전달 함수
  Socket.send(document.getElementById("hexInput").value);
  for (var j = 0; j < optionButton.length; j++) {
    optionButton[j].style.backgroundColor = "white";
    optionButton[j].value = '0';
  }
}

colorPicker.on(["color:init", "color:change"], function (color) {
  hexInput.value = color.hexString,
    redInput.value = color.rgb.r,
    greenInput.value = color.rgb.g,
    blueInput.value = color.rgb.b;
});

hexInput.addEventListener('change', function () {
  colorPicker.color.hexString = this.value;
  sendRgbColor();
});

redInput.addEventListener('change', function () {
  colorPicker.color.hexString = ((this.value) * 256 * 256 + (colorPicker.color.rgb.g) * 256 + (colorPicker.color.rgb.b)).toString(16).padStart(6, '0');
  sendRgbColor();
});
greenInput.addEventListener('change', function () {
  colorPicker.color.hexString = ((colorPicker.color.rgb.r) * 256 * 256 + (this.value) * 256 + (colorPicker.color.rgb.b)).toString(16).padStart(6, '0');
  sendRgbColor();
});
blueInput.addEventListener('change', function () {
  colorPicker.color.hexString = ((colorPicker.color.rgb.r) * 256 * 256 + (colorPicker.color.rgb.g) * 256 + (this.value) * 1).toString(16).padStart(6, '0');
  sendRgbColor();
});

hexset_button.addEventListener('change', function () {
  colorPicker.color.hexString = hexInput.value;
  sendRgbColor();
});

rgbset_button.addEventListener('change', function () {
  colorPicker.color.hexString = ((redInput.value) * 256 * 256 + (greenInput.value) * 256 + (blueInput.value)).toString(16).padStart(6, '0');
  sendRgbColor();
});


$("#file-input").change(function (e) {
  var file = e.target.files[0]

  var reader = new FileReader();
  reader.onload = fileOnload;
  reader.readAsDataURL(file);
});
var canvas = $("#canvas")[0];
var context = canvas.getContext("2d");
var fileLoad = false;

function fileOnload(e) {
  var $img = $("<img>", { src: e.target.result });
  $img.on("load", function () {
    fileLoad = true;

    var w, h;

    if (this.naturalWidth <= 350 && this.naturalHeight <= 350) {
      w = this.naturalWidth;
      h = this.naturalHeight;
    } else if (this.naturalWidth / this.naturalHeight >= 1) {
      w = 350;
      h = (this.naturalHeight / this.naturalWidth) * 350;
    } else {
      h = 350;
      w = (this.naturalWidth / this.naturalHeight) * 350;
    }
    canvas.width = w;
    canvas.height = h;
    context.drawImage(this, 0, 0, w, h);
  });
}

$("#canvas").on("click", function (e) {
  var pos = findPos(this);
  var x = e.pageX - pos.x;
  var y = e.pageY - pos.y;

  var imgData = context.getImageData(0, 0, canvas.width, canvas.height)
  var oData = imgData.data
  if (fileLoad == true) {
    const i = (y * imgData.width + x) * 4
    var hex = ("000000" + rgbToHex(oData[i], oData[i + 1], oData[i + 2])).slice(-6)
    $('#output').html("R : " + oData[i] + " G : " + oData[i+1] + " B : " + oData[i+2])
    //$('#output').html("R : " + oData[i] + " G : " + oData[i+1] + " B : " + oData[i+2] + "</br>" + "HEX : #" + hex + "</br>")
    colorPicker.color.hexString = "#" + hex;
  }
})

function findPos(obj) {
  var curleft = 0,
    curtop = 0;
  if (obj.offsetParent) {
    do {
      curleft += obj.offsetLeft;
      curtop += obj.offsetTop;
    } while ((obj = obj.offsetParent));
    return { x: curleft, y: curtop };
  }
  return undefined;
}

function rgbToHex(r, g, b) {
  if (r > 255 || g > 255 || b > 255) throw "Invalid color component";
  return ((r << 16) | (g << 8) | b).toString(16);
}
