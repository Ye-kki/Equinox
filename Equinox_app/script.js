let Red, Green, Blue;
let brightness;

function kelvinToRGB(Temperature) {
  Temperature = Temperature / 100

  if(Temperature <= 66) Red = 255;
  else {
    Red = Temperature - 60;
    Red = parseInt(329.698727446 * (Red ** -0.1332047592));
    if (Red < 0) Red = 0;
    if (Red > 255) Red = 255;
  }

  if (Temperature <= 66){
    Green = Temperature;
    Green = parseInt(99.4708025861 * Math.log(Green) - 161.1195681661);
    if (Green < 0) Green = 0;
    if (Green > 255) Green = 255;
  }
  else {
    Green = Temperature - 60;
    Green = parseInt(288.1221695283 * (Green ** -0.0755148492));
    if (Green < 0) Green = 0;
    if (Green > 255) Green = 255;
  }

  if (Temperature >= 66) Blue = 255;
  else {
    if (Temperature <= 19) Blue = 0;
    else {
      Blue = Temperature - 10;
      Blue = parseInt(138.5177312231 * Math.log(Blue) - 305.0447927307);
      if (Blue < 0) Blue = 0;
      if (Blue > 255)  Blue = 255;
    }
  }
}

var kelvin = new iro.ColorPicker(".kelvin", {
    width: 315,
    sliderSize: 20,
    borderWidth : 3,
    borderColor : "#FFFFFF",
    handleRadius : 15,
    layout: [
      { 
        component: iro.ui.Slider,
        options: {
          sliderType: 'kelvin'
        }
      },
    ]
});

var value = new iro.ColorPicker(".value", {
    width: 315,
    sliderSize: 20,
    borderWidth : 3,
    borderColor : "#787878",
    handleRadius : 15,
    layout: [
      { 
        component: iro.ui.Slider,
        options: {
          sliderType: 'value'
        }
      },
    ]
});

const background = document.querySelector(".background");
const blurCircle = document.querySelector(".blur-circle");
const lightCircle = document.querySelector(".light-circle");

kelvin.on(["color:init", "color:change"], function (color) {
    kelvinToRGB(color.kelvin);
    let rgb = String(Red) + String(Green) + String(Blue);
    console.log('K' + rgb);
    // Socket.send('K' + rgbHex);
    gsap.to(background, 0.7, {
      background: "linear-gradient(179.99deg, rgba(0, 0, 0, 0.9) -9.21%, rgba(" + Red + ', ' + Green + ', ' + Blue + ',' + brightness/200 + ") 37%, rgba(0, 0, 0, 0.9) 76.28%)",
    });
    gsap.to(blurCircle, 0.7, {
      border: "40px solid rgba(" + Red + ', ' + Green + ', ' + Blue + ','  + brightness/100 +")",
    });
    gsap.to(lightCircle, 0.7, {
      border: "5px solid rgba(" + (Red+70) + ', ' + (Green+70) + ', ' + (Blue+70) + ',' + brightness/100 +")",
    });
});

value.on(["color:init", "color:change"], function (color1) {
    console.log('B' + parseInt(color1.value/100*255));
    // Socket.send('B' + parseInt(color1.value/100*255));
    brightness = color1.value;
    gsap.to(background, 0.7, {
      background: "linear-gradient(179.99deg, rgba(0, 0, 0, 0.9) -9.21%, rgba(" + Red + ', ' + Green + ', ' + Blue + ',' + brightness/200 + ") 37%, rgba(0, 0, 0, 0.9) 76.28%)",
    });
    gsap.to(blurCircle, 0.7, {
      border: "40px solid rgba(" + Red + ', ' + Green + ', ' + Blue + ','  + brightness/100 +")",
    });
    gsap.to(lightCircle, 0.7, {
      border: "5px solid rgba(" + (Red+70) + ', ' + (Green+70) + ', ' + (Blue+70) + ',' + brightness/100 +")",
    });
});

const leftBtn = document.querySelector(".left-btn");
const rightBtn = document.querySelector(".right-btn");
const colorOption = document.querySelector(".color-option");
let sunLoop;

leftBtn.addEventListener('click', function () {
  gsap.to(blurCircle, 0, {
    display: "block",
  });
  gsap.to(lightCircle, 0, {
    display: "block",
  });
  gsap.to(colorOption, 0.2, {
    right: "0",
  });
  gsap.to(background, 0.2, {
    background: "linear-gradient(179.99deg, rgba(0, 0, 0, 0.9) -9.21%, rgba(" + Red + ', ' + Green + ', ' + Blue + ',' + brightness/200 + ") 37%, rgba(0, 0, 0, 0.9) 76.28%)",
  });
  gsap.to(sun, 0, {
    display: "none",
  });
  clearInterval(sunLoop);
});

rightBtn.addEventListener('click', function () {
  gsap.to(blurCircle, 0, {
    display: "none",
  });
  gsap.to(lightCircle, 0, {
    display: "none",
  });
  gsap.to(colorOption, 0.2, {
    right: "100%",
  });
  gsap.to(background, 0.2, {
    background: "linear-gradient(179.99deg, rgba(0, 0, 0, 0.9) -9.21%, rgba(" + Red + ', ' + Green + ', ' + Blue + ',' + brightness/200 + ") 37%, rgba(0, 0, 0, 0.9) 76.28%)",
  });
  gsap.to(sun, 0, {
    display: "block",
  });
  update();
  clearInterval(sunLoop);
  sunLoop = setInterval(update, 60*1000);
});

let time, year, month, date, hours, minutes, today;
let sunrise=0, sunset=0, sunriseMin, sunsetMin;
let now;

window.onload = () => {
  time = new Date();
  year = time.getFullYear(); // 년도
  month = time.getMonth() + 1;  // 월
  date = time.getDate();  // 날짜
  if(month < 10) month = String('0' + month);
  if(date < 10) date = String('0' + date);
  today = String(year) + String(month) + String(date);
  sunRequest();
  setInterval(sunRequest, 600*1000);
}

function sunRequest (){
  var xhr = new XMLHttpRequest();
  var url = 'http://apis.data.go.kr/B090041/openapi/service/RiseSetInfoService/getAreaRiseSetInfo'; /*URL*/
  var queryParams = '?' + encodeURIComponent('serviceKey') + '='+'VTFhDsOqym7hhhLzM7libBE5cz%2FhhxSRzr347qPOH4lEWCurspAftgFk2co5enlAzrFM45llt0jhce9ogU8vfQ%3D%3D'; /*Service Key*/
  queryParams += '&' + encodeURIComponent('locdate') + '=' + encodeURIComponent(today); /**/
  queryParams += '&' + encodeURIComponent('location') + '=' + encodeURIComponent('서울'); /**/
  xhr.open('GET', url + queryParams);
  xhr.onreadystatechange = function () {
      if (this.readyState == 4) {
          // alert('Status: '+this.status+'nHeaders: '+JSON.stringify(this.getAllResponseHeaders())+'nBody: '+this.responseText);
          let xmlParser = new DOMParser();
          let xmlDoc = xmlParser.parseFromString(xhr.responseText, "text/xml"); // parseFromString() 메소드를 이용해 문자열을 파싱함. 
          sunrise = xmlDoc.getElementsByTagName("sunrise")[0].textContent; 
          sunset = xmlDoc.getElementsByTagName("sunset")[0].textContent;
          sunriseMin = (parseInt(sunrise[0]*10) + parseInt(sunrise[1])) * 60 + parseInt(sunrise[2]*10) + parseInt(sunrise[3]);
          sunsetMin = (parseInt(sunset[0]*10) + parseInt(sunset[1])) * 60 + parseInt(sunset[2]*10) + parseInt(sunset[3]);
          document.getElementById('sunrise-time').textContent= "AM " + sunrise[0] + sunrise[1] + ':' + sunrise[2] + sunrise[3];
          document.getElementById('sunset-time').textContent= "PM " + sunset[0] + sunset[1] + ':' + sunset[2] + sunset[3];
          update();
        }
  };
  xhr.send('');
}

const canvas = document.getElementById('sun');
canvas.setAttribute("width", 390);
canvas.setAttribute("height", 390);

const context  = canvas.getContext("2d");

var currentDegree = 0;

function update() {
  time = new Date();
  hours = time.getHours(); // 시
  minutes = time.getMinutes();  // 분
  now = hours*60 + minutes;
  context.clearRect(0, 0, canvas.width, canvas.height);

  if(hours < 10) hours = String('0' + hours);
  if(minutes < 10) minutes = String('0' + minutes);
  document.getElementById('real-time').textContent= hours + ':' + minutes;
  if(now>=720 && now <1440){
    document.getElementById('ampm').textContent= 'PM';
  }
  else document.getElementById('ampm').textContent= 'AM';

  if(now >= sunriseMin && now <= sunsetMin){
    currentDegree = 180 + ((now-sunriseMin)/(sunsetMin-sunriseMin))*180;
  }
  else if(now >=sunsetMin && now < 1440 ){
    currentDegree =(now - sunsetMin)/(1440-sunsetMin) * 90;
  }
  else {
    currentDegree = 90 + now/sunriseMin * 90;
  }
  console.log('D' + parseInt(currentDegree));
  // Socket.send('D' + parseInt(currentDegree));
  drawSun();
}

function drawSun() {
  var distanceFromSun = 160;

  context.beginPath();

  var dx = (canvas.width / 2) + Math.cos(deg2Rad(currentDegree)) * distanceFromSun;
  var dy = (canvas.height / 2) + Math.sin(deg2Rad(currentDegree)) * distanceFromSun;

  var radius = 37.5/2; 

  context.arc(dx, dy, radius, 0, 2 * Math.PI);
  context.fillStyle = "#FFFFFF";
  context.shadowBlur = 20;
  context.shadowColor = "#FF7A00";
  context.fill();
}

function deg2Rad(degree) {
  return Math.PI / 180 * degree; 
}

var Socket;

function init() {
  Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
}